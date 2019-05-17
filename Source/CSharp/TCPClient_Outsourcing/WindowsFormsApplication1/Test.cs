using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Windows.Forms;
namespace WindowsFormsApplication1
{
    public static class Test
    {
        public static bool TestPacketParser()
        {
            bool ret = true;

            if(ret && !(ret = Test1()))
            {
                MessageBox.Show("Test1 failed");
            }
            if(ret && !(ret = Test2()))
            {
                MessageBox.Show("Test2 failed");
            }
            if(ret && !(ret = Test3()))
            {
                MessageBox.Show("Test3 failed");
            }
            if(ret && !(ret = Test4()))
            {
                MessageBox.Show("Test4 failed");
            }
            if(ret && !(ret = Test5()))
            {
                MessageBox.Show("Test5 failed");
            }
            if(ret && !(ret = Test6()))
            {
                MessageBox.Show("Test6 failed");
            }
            if(ret && !(ret = Test7()))
            {
                MessageBox.Show("Test7 failed");
            }
            return ret;
        }

        private static byte[] BuildPacket(string s)
        {
            int totallength = s.Length + 1 + 4 + 1;
            byte[] ret = new byte[totallength]; // stx + length + etx
            int idx = 0;
            ret[idx] = 0x02;
            idx++;
            Encoding.ASCII.GetBytes(totallength.ToString("D4")).CopyTo(ret, idx);
            idx += 4;
            Encoding.ASCII.GetBytes(s).CopyTo(ret, idx);
            idx += s.Length;
            ret[idx] = 0x03;

            return ret;
        }

        public static bool CompareTest(byte[] b1, byte[] b2)
        {
            if (b1 == null || b2 == null) return false;
            if (b1.Length != b2.Length) return false;
            for(int i =0; i < b1.Length; i++)
            {
                if (b1[i] != b2[i]) return false;
            }
            return true;
        }
        private static bool Test1()
        {
            PacketParser pp = new PacketParser();

            byte[] test1 = BuildPacket("TEST1");
            pp.AddPacket(test1, test1.Length);

            byte[] verify1 = pp.GetOnePacket();

            return CompareTest(test1, verify1);
        }

        private static bool Test2()
        {
            // single packet added multiple times
            PacketParser pp = new PacketParser();

            byte[] test1 = BuildPacket("TEST1");
            int p1 = 4;
            int p2 = test1.Length - p1;
            byte[] b1 = new byte[p1];
            byte[] b2 = new byte[p2];

            Array.Copy(test1, b1, p1);
            Array.Copy(test1, p1, b2, 0, p2);
            pp.AddPacket(b1, b1.Length);
            pp.AddPacket(b2, b2.Length);

            byte[] verify1 = pp.GetOnePacket();

            return CompareTest(test1, verify1);
        }

        private static bool Test3()
        {
            // multiple packets
            PacketParser pp = new PacketParser();

            byte[] test1 = BuildPacket("TEST1");
            byte[] test2 = BuildPacket("TEST2");
            pp.AddPacket(test1, test1.Length);
            pp.AddPacket(test2, test2.Length);

            byte[] verify1 = pp.GetOnePacket();
            byte[] verify2 = pp.GetOnePacket();

            return CompareTest(test1, verify1) && CompareTest(test2, verify2);
        }

        private static bool Test4()
        {
            // single packet with wrong length
            PacketParser pp = new PacketParser();

            byte[] test1 = BuildPacket("TEST1");
            test1[0x4] = 0x39;
            pp.AddPacket(test1, test1.Length);

            byte[] verify1 = pp.GetOnePacket();

            return (verify1 == null);
        }

        private static bool Test5()
        {
            // invalid packet
            PacketParser pp = new PacketParser();

            byte[] test1 = new byte[10];
            pp.AddPacket(test1, test1.Length);

            byte[] verify1 = pp.GetOnePacket();

            return (verify1 == null || ! CompareTest(test1, verify1));
        }

        private static bool Test6()
        {
            // multiple packet added multiple times
            PacketParser pp = new PacketParser();

            byte[] test1 = BuildPacket("TEST1");
            byte[] test2 = BuildPacket("TEST2");
            byte[] test3 = BuildPacket("TEST3");
            int l2_1 = 4;
            int l2_2 = test2.Length - l2_1;
            int l1 = test1.Length + l2_1;
            int l3 = test3.Length + l2_2;
            byte[] b1 = new byte[l1];
            byte[] b2 = new byte[l3];

            Array.Copy(test1, b1, test1.Length);
            Array.Copy(test2, 0, b1, test1.Length, l2_1);
            Array.Copy(test2, l2_1, b2, 0, l2_2);
            Array.Copy(test3, 0, b2, l2_2, test3.Length);

            pp.AddPacket(b1, b1.Length);
            pp.AddPacket(b2, b2.Length);

            byte[] verify1 = pp.GetOnePacket();
            byte[] verify2 = pp.GetOnePacket();
            byte[] verify3 = pp.GetOnePacket();

            return CompareTest(test1, verify1)
                && CompareTest(test2, verify2)
                && CompareTest(test3, verify3);
        }

        private static bool Test7()
        {
            // packet added after invalid packet
            PacketParser pp = new PacketParser();

            byte[] test1 = new byte[10];
            pp.AddPacket(test1, test1.Length);
            byte[] test2 = BuildPacket("TEST1");

            pp.AddPacket(test2, test2.Length);

            byte[] verify1 = pp.GetOnePacket();
            byte[] verify2 = pp.GetOnePacket();

            return (CompareTest(test2, verify1) && !CompareTest(test1, verify2));
        }
    }
}
