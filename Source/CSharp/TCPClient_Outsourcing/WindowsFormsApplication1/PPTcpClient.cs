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
        private Socket tcpSocket;
        private string host;
        private int port;
        private Thread networkThread;
        private Thread parserThread;

        private const int PACKET_SIZE = 8192;
        private bool Quit
        {
            get; set;
        }
        private PacketParser packetParser;

        AutoResetEvent waitHandle = new AutoResetEvent(false);

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
                    && (tcpSocket != null)
                    && !(tcpSocket.Poll(1, SelectMode.SelectRead)
                        && tcpSocket.Available == 0)
                    );
            }
        }
        public delegate void StatusUpdateHandler ();
        public event StatusUpdateHandler StatusUpdated;
        public delegate void OnPacketHandler (byte[] data);
        public event OnPacketHandler onPacket;

        public PPTcpClient(string host, int port)
        {
            this.host = host;
            this.port = port;

            packetParser = new PacketParser();
        }

        ~PPTcpClient()
        {
            Quit = true;
        }

        public void StartConnection()
        {
            ThreadStart networkStart = new ThreadStart(NetworkThread);
            ThreadStart parserStart = new ThreadStart(ParserThread);
            networkThread = new Thread(networkStart);
            networkThread.Start();
            parserThread = new Thread(parserStart);
            parserThread.Start();
        }

        public void StopConnection()
        {
            Quit = true;
            if(tcpSocket != null)
            {
                tcpSocket.Close();
                tcpSocket = null;
            }
            if (networkThread != null)
            {
                networkThread.Abort();
            }
            if (parserThread != null)
            {
                waitHandle.Set();
                parserThread.Abort();
            }
        }

        private void NetworkThread()
        {
            tcpSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            tcpSocket.ReceiveBufferSize = PACKET_SIZE;
            tcpSocket.SendBufferSize = PACKET_SIZE;
            tcpSocket.NoDelay = true;
            tcpSocket.DontFragment = true;
            // tcpSocket.ReceiveTimeout = 5000;
            // tcpSocket.SendTimeout = 5000;

            while (!tcpSocket.Connected && !Quit) {
                // TODO raise connecting event
                UpdateStatus(ConnectionStatus.CONNECTING);
                try
                {
                    tcpSocket.Connect(this.host, this.port);
                } catch(SocketException e)
                {
                    Console.WriteLine("Error connecting to server {0} {1}", e.ErrorCode, e.Message);
                }

                if (tcpSocket.Connected)
                {
                    // raise connected event
                    UpdateStatus(ConnectionStatus.CONNECTED);
                }
            }

            while(!Quit && Connected)
            {
                byte[] buffer = new byte[PACKET_SIZE];
                try
                {
                    int r = tcpSocket.Receive(buffer);
                    if (r > 0)
                    {
                        // push packet
                        Console.WriteLine(String.Format("{0} Receive", DateTime.Now.ToString("mm:ss.fff")));
                        packetParser.AddPacket(buffer, r);
                        waitHandle.Set();
                    }
                } catch(SocketException e)
                {

                    Console.WriteLine("Error receiving from socket {0} {1}", e.ErrorCode, e.Message);

                }
            }

            Quit = true;
            UpdateStatus(ConnectionStatus.DISCONNECTED);
        }

        public int SendData(byte[] data)
        {
            int s = 0;
            if(tcpSocket != null && tcpSocket.Connected)
            {
                s = tcpSocket.Send(data);
            }

            return s;
        }

        private void UpdateStatus(ConnectionStatus status)
        {

            ConnectionStatus ostatus = this.Status;
            this.Status = status;

            if(ostatus != status && StatusUpdated != null)
            {
                StatusUpdated();
            }
        }

        private void ParserThread()
        {

            while(!Quit)
            {
                waitHandle.WaitOne();

                while (packetParser.MorePacket())
                {
                    byte[] msg = packetParser.GetOnePacket();
                    if (msg != null)
                    {

                        if (onPacket != null)
                        {
                            onPacket(msg);
                        }
                    }
                }
            }
        }
    }
}
