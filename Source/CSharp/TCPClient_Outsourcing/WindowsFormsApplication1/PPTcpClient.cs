using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Threading;

namespace WindowsFormsApplication1
{
    internal class PPTcpClient
    {
        private Socket m_TcpSocket;
        private string m_sHost;
        private int m_nPort;
        private Thread m_networkThread;
        private Thread m_parserThread;

        private const int PACKET_SIZE = 8192;
        private bool Quit
        {
            get; set;
        }
        private PacketParser m_PacketParser;

        AutoResetEvent m_waitHandle = new AutoResetEvent(false);

        public enum ConnectionStatus 
        {
            DISCONNECTED,
            CONNECTING,
            CONNECTED
        }

        public ConnectionStatus Status
        {
            get;
            set;
        }

        public bool Connected
        {
            get
            {
                return (Status == ConnectionStatus.CONNECTED
                    && (m_TcpSocket != null)
                    && !(m_TcpSocket.Poll(1, SelectMode.SelectRead)
                        && m_TcpSocket.Available == 0)
                    );
            }
        }
        public delegate void StatusUpdateHandler ();
        public event StatusUpdateHandler evStatusUpdateHandler;
        public delegate void OnPacketHandler (byte[] data);
        public event OnPacketHandler evOnPacket;

        public PPTcpClient(string m_sHost, int m_nPort)
        {
            this.m_sHost = m_sHost;
            this.m_nPort = m_nPort;

            m_PacketParser = new PacketParser();
        }

        ~PPTcpClient()
        {
            Quit = true;
        }

        public void StartConnection()
        {
            ThreadStart networkStart = new ThreadStart(NetworkThread);
            ThreadStart parserStart = new ThreadStart(ParserThread);
            m_networkThread = new Thread(networkStart);
            m_networkThread.Start();
            m_parserThread = new Thread(parserStart);
            m_parserThread.Start();
        }

        public void StopConnection()
        {
            Quit = true;
            if(m_TcpSocket != null)
            {
                m_TcpSocket.Close();
                m_TcpSocket = null;
            }
            if (m_networkThread != null)
            {
                //m_networkThread.Abort();
                m_networkThread.Interrupt();
            }
            if (m_parserThread != null)
            {
                m_waitHandle.Set();
                //m_parserThread.Abort();
                m_parserThread.Interrupt();
            }
        }

        private void NetworkThread()
        {
            m_TcpSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            m_TcpSocket.ReceiveBufferSize = PACKET_SIZE;
            m_TcpSocket.SendBufferSize = PACKET_SIZE;
            m_TcpSocket.NoDelay = true;
            m_TcpSocket.DontFragment = true;
            // m_TcpSocket.ReceiveTimeout = 5000;
            // m_TcpSocket.SendTimeout = 5000;

            while (!m_TcpSocket.Connected && !Quit)
            {
                // TODO raise connecting event
                UpdateConnectionStatus(ConnectionStatus.CONNECTING);
                try
                {
                    m_TcpSocket.Connect(this.m_sHost, this.m_nPort);
                }
                catch (SocketException e)
                {
                    Console.WriteLine("Error connecting to server {0} {1}", e.ErrorCode, e.Message);
                }

                if (m_TcpSocket.Connected)
                {
                    // raise connected event
                    UpdateConnectionStatus(ConnectionStatus.CONNECTED);
                }
            }

            while(!Quit && Connected)
            {
                byte[] buffer = new byte[PACKET_SIZE];
                try
                {
                    int r = m_TcpSocket.Receive(buffer);
                    if (r > 0)
                    {
                        // push packet
                        Console.WriteLine(String.Format("{0} Receive", DateTime.Now.ToString("mm:ss.fff")));
                        m_PacketParser.AddPacket(buffer, r);
                        m_waitHandle.Set();
                    }
                } catch(SocketException e)
                {

                    Console.WriteLine("Error receiving from socket {0} {1}", e.ErrorCode, e.Message);

                }
            }

            Quit = true;
            UpdateConnectionStatus(ConnectionStatus.DISCONNECTED);
        }

        public int SendData(byte[] data)
        {
            int s = 0;
            if(m_TcpSocket != null && m_TcpSocket.Connected)
            {
                s = m_TcpSocket.Send(data);
            }

            return s;
        }

        private void UpdateConnectionStatus(ConnectionStatus status)
        {

            ConnectionStatus ostatus = this.Status;
            this.Status = status;

            if(ostatus != status && evStatusUpdateHandler != null)
            {
                evStatusUpdateHandler();
            }
        }

        private void ParserThread()
        {

            while(!Quit)
            {
                m_waitHandle.WaitOne();

                while (m_PacketParser.MorePacket())
                {
                    byte[] msg = m_PacketParser.GetOnePacket();
                    if (msg != null)
                    {

                        if (evOnPacket != null)
                        {
                            evOnPacket(msg);
                        }
                    }
                }
            }
        }
    }
}
