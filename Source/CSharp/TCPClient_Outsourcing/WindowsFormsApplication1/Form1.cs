using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class frmPacketParser : Form
    {
        private PPTcpClient m_tcpClient;

        public frmPacketParser()
        {
            InitializeComponent();

            txtIP.Text = "127.0.0.1";
            txtPort.Text = "8188";
            UpdateUI();
        }

        private void UpdateUI()
        {
            Color c = Color.Red;
            PPTcpClient.ConnectionStatus status = PPTcpClient.ConnectionStatus.DISCONNECTED;
            string btnText = "Connect";
            bool sendEnabled = false;
            if (m_tcpClient != null)
            {
                status = m_tcpClient.Status;
            }
            if (status == PPTcpClient.ConnectionStatus.CONNECTED)
            {
                btnText = "Disconnect";
                c = Color.Green;
                sendEnabled = true;
            }
            else if (status == PPTcpClient.ConnectionStatus.CONNECTING)
            {
                btnText = "Disconnect";
                c = Color.Gray;
            }
            else
            {
                btnText = "Connect";
                c = Color.Red;
            }
            if (txtStatus.InvokeRequired)
            {
                txtStatus.Invoke((MethodInvoker)delegate {
                    txtStatus.Text = status.ToString();
                    txtStatus.BackColor = c;
                    btnConnect.Text = btnText;
                    txtSend.Enabled = sendEnabled;
                    btnSend.Enabled = sendEnabled;
                    });

            }
            else {
                txtStatus.Text = status.ToString();
                txtStatus.BackColor = c;
                btnConnect.Text = btnText;
                txtSend.Enabled = sendEnabled;
                btnSend.Enabled = sendEnabled;
            }
        }//private void UpdateUI()

        private bool CheckIP(String ip)
        {
            bool ret = true;
            Regex reipv4 = new Regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

            ret = reipv4.IsMatch(ip);
            return ret;

        }
        private bool CheckPort(String portstr)
        {
            int port;
            if (!Int32.TryParse(portstr, out port))
            {
                return false;
            }

            if (port <= 0 || port >= 65535)
            {
                return false;
            }
            return true;
        }


        private void btnConnect_Click(object sender, EventArgs e)
        {
            if(m_tcpClient == null)
            {
                String ip = txtIP.Text.Trim();
                String portstr = txtPort.Text.Trim();
                int port = 0;
                if(!CheckIP(ip))
                {
                    MessageBox.Show("Invalid IPv4 Address");
                    txtIP.Focus();
                    return;
                }
                if(!CheckPort(portstr))
                {
                    MessageBox.Show("Invalid Port");
                    txtPort.Focus();
                    return;
                }


                Int32.TryParse(portstr, out port);

                m_tcpClient = new PPTcpClient(ip, port);
                m_tcpClient.evStatusUpdateHandler += TcpClient_StatusUpdated;
                m_tcpClient.evOnPacket += TcpClient_onPacket;
                m_tcpClient.StartConnection();
            }
            else
            {

                if(m_tcpClient.Status == PPTcpClient.ConnectionStatus.CONNECTED 
                    || m_tcpClient.Status == PPTcpClient.ConnectionStatus.CONNECTING)
                {
                    m_tcpClient.StopConnection();
                    m_tcpClient.evStatusUpdateHandler -= TcpClient_StatusUpdated;
                    m_tcpClient.evOnPacket -= TcpClient_onPacket;
                    m_tcpClient = null;

                }
            }
            UpdateUI();
            
        }

        private void TcpClient_onPacket(byte[] data)
        {
            // call PacketParser

            Console.WriteLine(String.Format("{0} evOnPacket", DateTime.Now.ToString("mm:ss.zzz")));
            txtLog.Invoke((MethodInvoker)delegate {
                Console.WriteLine(String.Format("{0} Write Log", DateTime.Now.ToString("mm:ss.zzz")));
                txtLog.AppendText(String.Format("GetOnePacket - {0}\n", Encoding.ASCII.GetString(data)));
            });
        }

        private void TcpClient_StatusUpdated()
        {
            // Console.WriteLine(String.Format("Tcp Client Status Updated {0}", this.m_tcpClient.Status));
            String newstatus = this.m_tcpClient.Status.ToString();
            txtLog.Invoke((MethodInvoker)delegate
            {
                txtLog.AppendText(String.Format("Tcp Client {0}\n", newstatus));
                
            });
            UpdateUI();
            if(m_tcpClient.Status == PPTcpClient.ConnectionStatus.DISCONNECTED)
            {
                m_tcpClient.StopConnection();
                m_tcpClient.evStatusUpdateHandler -= TcpClient_StatusUpdated;
                m_tcpClient.evOnPacket -= TcpClient_onPacket;
                m_tcpClient = null;
            }
            
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            String s = txtSend.Text;
            if (s.Length <= 0) return;
            if(m_tcpClient != null && m_tcpClient.Connected)
            {
                int buflength = s.Length + 6;
                byte[] buf = new byte[buflength];

                int i = 0;
                buf[i] = 0x02;
                i += 1;

                string lengthstr = buflength.ToString("D4");
                Encoding.ASCII.GetBytes(lengthstr).CopyTo(buf, i);
                i += 4;
                Encoding.ASCII.GetBytes(s).CopyTo(buf, i);
                i += s.Length;
                buf[i] = 0x03;
                i += 1;

                int sent = m_tcpClient.SendData(buf);
                if(sent < buflength)
                {
                    // error
                } else
                {
                    txtLog.AppendText(String.Format("Sent {0} Bytes\n", sent));
                }
            }
        }

        private void frmPacketParser_FormClosing(object sender, FormClosingEventArgs e)
        {

            if(m_tcpClient != null)
            {
                m_tcpClient.StopConnection();
                m_tcpClient.evStatusUpdateHandler -= TcpClient_StatusUpdated;
                m_tcpClient.evOnPacket -= TcpClient_onPacket;
                m_tcpClient = null;
            }

        }
    }
}
