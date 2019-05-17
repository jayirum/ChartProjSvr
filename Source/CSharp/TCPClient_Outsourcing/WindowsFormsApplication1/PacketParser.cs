using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApplication1
{
    public class PacketParser
    {
        private Object bufLock;
        private byte[] packet;

        public byte[] Packet
        {
            get
            {
                lock (bufLock)
                {
                    byte[] clone = new byte[packet.Length];
                    Array.Copy(packet, clone, packet.Length);
                    return clone;
                }
            }
        }
        public PacketParser()
        {
            bufLock = new object();

        }

        ~PacketParser()
        {
            
        }

        public bool AddPacket(byte[] buf, int size)
        {

            if (buf == null || size <= 0)
                return false;
            lock(bufLock)
            {
                if(packet != null)
                {
                    int buflength = size + packet.Length;

                    byte[] newbuf = new byte[buflength];
                    Array.Copy(packet, newbuf, packet.Length);
                    Array.Copy(buf, 0, newbuf, packet.Length, size);

                    packet = newbuf;
                }
                else
                {
                    byte[] newbuf = new byte[size];
                    Array.Copy(buf, newbuf, size);
                    packet = newbuf;
                }
            }

            return true;
        }

        public byte[] GetOnePacket()
        {
            byte[] ret = null;

            lock(bufLock)
            {
                if(packet != null && packet.Length > 0)
                {
                    int stxIdx = Array.IndexOf(packet, (byte)0x02);
                    int etxIdx = -1;
                    if(stxIdx >= 0)
                    {
                        etxIdx = Array.IndexOf(packet, (byte)0x03, stxIdx);

                        if(etxIdx > stxIdx)
                        {
                            // packet found
                            int pktSize = etxIdx - stxIdx + 1;
                            // read packet size in buffer
                            String slength = Encoding.ASCII.GetString(packet, stxIdx + 1, 4);
                            Int32 pktlength;

                            if (Int32.TryParse(slength, out pktlength)
                                && pktlength == pktSize)
                            {
                                ret = new byte[pktSize];
                                Array.Copy(packet, stxIdx, ret, 0, pktSize);
                            }
                            else
                            {
                                // invalid packet with wrong length
                                // packet will be clear below
                            }
                            if (etxIdx + 1 == packet.Length)
                            {
                                // remove all
                                packet = null;
                            }
                            else
                            {
                                // remove up to etx
                                int newlength = packet.Length - (etxIdx + 1);
                                byte[] newbuf = new byte[newlength];
                                Array.Copy(packet, etxIdx + 1, newbuf, 0, newlength);
                                packet = newbuf;
                            }
                        }//if(etxIdx > stxIdx)
                        else
                        {
                            // etx not found, wait for packet completion
                        }
                    } // if(stxIdx >= 0)
                    else
                    {
                        ret = Encoding.ASCII.GetBytes("There is no STX in the buffer.");
                        packet = null;
                    }
                }//if(packet != null && packet.Length > 0)
                else
                {
                    ret = Encoding.ASCII.GetBytes("There is no packet in the buffer.");
                }
            }
            return ret;
        }

        public bool MorePacket()
        {
            return packet != null;
        }
    }
}
