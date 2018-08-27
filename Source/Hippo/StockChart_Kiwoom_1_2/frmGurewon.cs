using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TTN
{
    public partial class frmGurewon : Form
    {
        AxKHOpenAPILib.AxKHOpenAPI axKHOpenAPI1;
        public string selectedCode; 

        public frmGurewon(AxKHOpenAPILib.AxKHOpenAPI axKHOpenAPI1copy, string code)
        {
            InitializeComponent();

            this.axKHOpenAPI1 = axKHOpenAPI1copy;
            this.selectedCode = code;

            axKHOpenAPI1.OnReceiveTrData += AxKHOpenAPI1_OnReceiveTrData;
             
            axKHOpenAPI1.SetInputValue("종목코드", this.selectedCode);
            axKHOpenAPI1.SetInputValue("기준일자", DateTime.Now.ToString("yyyy-MM-dd").Trim());
            axKHOpenAPI1.SetInputValue("수정주가구분", "1");

            int nRet = axKHOpenAPI1.CommRqData("주식당일거래원", "OPT10040", 0, "0101");
        }

        private void AxKHOpenAPI1_OnReceiveTrData(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveTrDataEvent e)
        {
            if (e.sRQName == "주식당일거래원")
            {
                int nCnt = axKHOpenAPI1.GetRepeatCnt(e.sTrCode, e.sRQName); //??

                for (int i = 1; i <= 5; i++)
                {
                    string gubun = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "거래소구분").Trim();

                    string gure1 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매도거래원" + i.ToString()).Trim();
                    string gure2 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매도거래원수량" + i.ToString()).Trim();
                    string gure3 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매도거래원별증감" + i.ToString()).Trim();
                    string gure4 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매도거래원코드" + i.ToString()).Trim();
                    string gure5 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매도거래원색깔" + i.ToString()).Trim();

                    Label la = new Label();
                    la.Width = this.panel1.Width;
                    la.Text = gure1 + ", " + gure2 + ", " + gure3 + ", " + gure4 + ", " + gure5;
                    la.Location = new Point(0, 25 * (i - 1) + 5);

                    if(int.Parse(gure2) > 0)
                    {
                        la.ForeColor = Color.Red;
                    }
                    else
                    {
                        la.ForeColor = Color.Blue;
                    }

                    this.panel1.Controls.Add(la);

                    string gure_mesu1 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매수거래원" + i.ToString()).Trim();
                    string gure_mesu2 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매수거래원수량" + i.ToString()).Trim();
                    string gure_mesu3 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매수거래원별증감" + i.ToString()).Trim();
                    string gure_mesu4 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매수거래원코드" + i.ToString()).Trim();
                    string gure_mesu5 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매수거래원색깔" + i.ToString()).Trim();

                    Label la2 = new Label();
                    la2.Width = this.panel2.Width;
                    la2.Text = gure_mesu1 + ", " + gure_mesu2 + ", " + gure_mesu3 + ", " + gure_mesu4 + ", " + gure_mesu5;
                    la2.Location = new Point(0, 25 * (i - 1) + 5);

                    if (int.Parse(gure_mesu2) > 0)
                    {
                        la2.ForeColor = Color.Red;
                    }
                    else
                    {
                        la2.ForeColor = Color.Blue;
                    }

                    this.panel2.Controls.Add(la2);

                    string forin1 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계매도추정합").Trim();
                    string forin2 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계매도추정합변동").Trim();
                    string forin3 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계매수추정합").Trim();
                    string forin4 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계매수추정합변동").Trim();
                    string forin5 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계순매수추정합").Trim();
                    string forin6 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계순매수변동").Trim();

                    Label la3 = new Label();
                    la3.Text = forin1 + ", " + forin2 + ", " + forin3 + ", " + forin4 + ", " + forin5 + ", " + forin6;
                    la3.Location = new Point(0, 20);

                    if (int.Parse(forin1) > 0)
                    {
                        la3.ForeColor = Color.Red;
                    }
                    else
                    {
                        la3.ForeColor = Color.Blue;
                    }

                    this.panel3.Controls.Add(la3);
                }
            }
        }

        private void frmGurewon_Load(object sender, EventArgs e)
        {

        }
    }
}
