using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Hippo;

namespace TTN
{
    public partial class frmCodeSave : Form
    {
        AxKHOpenAPILib.AxKHOpenAPI axKHOpenAPI1;
        string insertText;
        string selectedCode;
        bool ErrorGo;
        int TotalCount;
        int Codecounts = 0;
        string errorMessage;

        public frmCodeSave(AxKHOpenAPILib.AxKHOpenAPI axKHOpenAPI1copy)
        {
            InitializeComponent();
            axKHOpenAPI1 = axKHOpenAPI1copy;
        }

        private void frmCodeSave_Load(object sender, EventArgs e)
        {
            axKHOpenAPI1.OnReceiveTrData += AxKHOpenAPI1_OnReceiveTrData;
            axKHOpenAPI1.OnReceiveRealData += AxKHOpenAPI1_OnReceiveRealData;

            this.backgroundWorker1.WorkerReportsProgress = true;
        }

        /// <summary>
        /// 저장 시작
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnSaveStart_Click(object sender, EventArgs e)
        {
            this.backgroundWorker1.RunWorkerAsync();
        }

        /// <summary>
        /// 취소
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnCancel_Click(object sender, EventArgs e)
        {
            //this.backgroundWorker1.CancelAsync();
        }

        /// <summary>
        /// 전체 코드 검색
        /// </summary>
        /// <returns></returns>
        private DataTable GetCodes()
        {
            //0:장내, 3:ELW, 4:뮤추얼펀드, 5:신주인수권, 6:리츠, 8:ETF, 9:하이일드펀드, 10:코스닥, 30:제3시장
            string[] marketList = { "0", "10" };

            DataTable dt = new DataTable();
            dt.Columns.Add("종목코드");
            dt.Columns.Add("종목이름");

            int i = 0;
            foreach (string MNumber in marketList)
            {
                string result = axKHOpenAPI1.GetCodeListByMarket(MNumber);
                string[] stockList = result.Split(';');

                foreach (string code in stockList)
                {
                    if (code != "")
                    {
                        string StockName = axKHOpenAPI1.GetMasterCodeName(code);

                        DataRow row = dt.NewRow();
                        row["종목코드"] = code;
                        row["종목이름"] = StockName;

                        dt.Rows.Add(row);

                        i++;
                    }
                }
            }

            dt.AcceptChanges();

            return dt;
        }

        private void AxKHOpenAPI1_OnReceiveTrData(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveTrDataEvent e)
        {
            int nCnt = axKHOpenAPI1.GetRepeatCnt(e.sTrCode, e.sRQName); 

            for (int i = nCnt - 1; i >= 0; i--)
            {
                string dates = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "일자").Trim();

                SeriesItem item = new SeriesItem();
                item.Name = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "일자").Trim();
                item.YValue = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "현재가").Trim());
                item.YStartValue = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "시가").Trim());
                item.HighValue = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "고가").Trim());
                item.LowValue = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "저가").Trim());

                SeriesItem item_vol = new SeriesItem();
                item_vol.Name = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "일자").Trim();
                item_vol.YValue = Math.Abs(Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "거래량").Trim()));

                insertText += string.Format("INSERT INTO " + this.txtTAbleName.Text +" VALUES('{0}', '{1}', '{2}', '{3}', {4}, {5}, {6}, {7}) ; \r\n",
                   selectedCode,
                   axKHOpenAPI1.GetMasterCodeName(selectedCode),
                   item.Name.Substring(0, 4) + "-" + item.Name.Substring(4, 2) + "-" + item.Name.Substring(6, 2),
                   item.YStartValue.ToString(),
                   item.YValue.ToString(),
                   item.HighValue.ToString(),
                   item.LowValue.ToString(),
                   item_vol.YValue.ToString()
                    );

                TotalCount++;
            }

            DBAccess db = new DBAccess(this.txtConnStr.Text);
            bool ret =  db.ExcuteNonquery(insertText);

            errorMessage = db.ErrorMessage;

            if (ret)
            {
                this.label3.Text = Codecounts.ToString() + "/" + this.progressBar1.Maximum.ToString("n0") + " 디비 입력 성공 : " + selectedCode;
            }
            else
            {
                ErrorGo = true;
            }
        }

        private void AxKHOpenAPI1_OnReceiveRealData(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveRealDataEvent e)
        {
            
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            Codecounts = 0;
            DataTable dt = this.GetCodes();

            this.progressBar1.Maximum = dt.Rows.Count;

            foreach (DataRow dr in dt.Rows)
            {
                if (ErrorGo)
                {
                    MessageBox.Show("디비 문제 발생 : " + errorMessage);
                    break;
                }

                insertText = "";
                string code = dr["종목코드"].ToString();

                selectedCode = code;

                axKHOpenAPI1.SetInputValue("종목코드", code);
                axKHOpenAPI1.SetInputValue("기준일자", DateTime.Now.ToString("yyyy-MM-dd").Trim());
                axKHOpenAPI1.SetInputValue("수정주가구분", "1");

                int nRet = axKHOpenAPI1.CommRqData("주식일봉차트조회", "OPT10081", 0, "0101");

                if (Error.IsError(nRet))
                { 
                    this.backgroundWorker1.ReportProgress(Codecounts); // (100 * counts) / this.progressBar1.Maximum

                    Codecounts++;
                }
                else
                {
                    MessageBox.Show(Error.GetErrorMessage());
                    break;
                }

                System.Threading.Thread.Sleep(270);
            }
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            this.progressBar1.Value = e.ProgressPercentage;
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        { 
            if (e.Cancelled == true)
            { 
                MessageBox.Show("cancel");
            }
            else if (e.Error != null)
            { 
                MessageBox.Show("ok");
            }
            else
            {

            }
        }
    }
}
