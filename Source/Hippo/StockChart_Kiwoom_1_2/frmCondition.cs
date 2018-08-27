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
    public partial class frmCondition : Form
    {
        AxKHOpenAPILib.AxKHOpenAPI axKHOpenAPI1;
        public string selectedCode;
        public string selectedCodeName;

        private SortedList<string, string> conList;

        public frmCondition(AxKHOpenAPILib.AxKHOpenAPI axKHOpenAPI1copy, SortedList<string, string> conditios)
        {
            InitializeComponent();

            axKHOpenAPI1 = axKHOpenAPI1copy;
            axKHOpenAPI1.OnReceiveConditionVer += AxKHOpenAPI1_OnReceiveConditionVer;
            axKHOpenAPI1.OnReceiveTrCondition += AxKHOpenAPI1_OnReceiveTrCondition;
            //axKHOpenAPI1.OnReceiveTrData += AxKHOpenAPI1_OnReceiveTrData;

            conList = conditios;
        }

        private void frmCondition_Load(object sender, EventArgs e)
        {
            this.listBox1.Items.Clear();

            if(conList != null)
            {
                int i = 0;
                foreach(string conName in conList.Values)
                {
                    this.listBox1.Items.Add(conList.Keys[i] + "^" + conName);
                    i++;
                }
            }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                this.dataGridView1.DataSource = null;

                string[] cons = this.listBox1.Items[this.listBox1.SelectedIndex].ToString().Trim().Split('^');

                int nRet = axKHOpenAPI1.SendCondition("0103", cons[1].Trim(), int.Parse(cons[0].Trim()), 0);

                if (nRet == 1)
                {

                } 
            }
            catch { }
        }

        private void AxKHOpenAPI1_OnReceiveConditionVer(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveConditionVerEvent e)
        {
            string[] conditions = axKHOpenAPI1.GetConditionNameList().Trim().Split(';');

            foreach (string con in conditions)
            {
                if (!string.IsNullOrEmpty(con))
                {
                    this.listBox1.Items.Add(con);
                }
            }
        }

        private void AxKHOpenAPI1_OnReceiveTrCondition(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveTrConditionEvent e)
        {
            string[] codes = e.strCodeList.Trim().Split(';');

            DataTable dt = new DataTable();

            dt.Columns.Add("종목코드");
            dt.Columns.Add("종목이름"); 

            for (int i = 0; i< codes.Length; i++)
            {
                DataRow dr = dt.NewRow();

                dr["종목코드"] = codes[i];
                dr["종목이름"] = axKHOpenAPI1.GetMasterCodeName(codes[i]); 

                dt.Rows.Add(dr);
            }

            dt.AcceptChanges();

            this.dataGridView1.DataSource = dt;

            this.dataGridView1.Columns[0].Width = 80;
            this.dataGridView1.Columns[1].Width = this.dataGridView1.Width - this.dataGridView1.Columns[0].Width;

            //int nRet = axKHOpenAPI1.CommKwRqData(e.strCodeList, 0, codes.Length, 0, "RQ_1", "0101");
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        { 
        }

        private void dataGridView1_CellContentDoubleClick(object sender, DataGridViewCellEventArgs e)
        { 
            this.selectedCode = this.dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString();
            this.selectedCodeName = this.dataGridView1.Rows[e.RowIndex].Cells[1].Value.ToString();

            this.DialogResult = DialogResult.OK;
        }

        //private void AxKHOpenAPI1_OnReceiveTrData(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveTrDataEvent e)
        //{
        //    if (e.sRQName == "RQ_1") // RQ_1 (복수종목 조회)
        //    {
        //        #region
        //        int nCnt = axKHOpenAPI1.GetRepeatCnt(e.sTrCode, e.sRQName);

        //        this.dataGridView1.Columns[0].Width = 80;
        //        this.dataGridView1.Columns[1].Width = (this.dataGridView1.Width - this.dataGridView1.Columns[0].Width) / 2 - 30;
        //        this.dataGridView1.Columns[2].Width = (this.dataGridView1.Width - this.dataGridView1.Columns[0].Width) / 2 - 30;

        //        for (int i = 0; i < nCnt; i++)
        //        {
        //            string dates = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "일자").Trim();
        //            int curVal = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "현재가").Trim());
        //            string upNdown = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "등락율").Trim();

        //            string fllag = "";

        //            if (curVal > 0)
        //            {
        //                this.dataGridView1.Rows[i].Cells[2].Style.ForeColor = Color.Red;
        //                fllag = "▲";
        //            }
        //            else if (curVal < 0)
        //            {
        //                this.dataGridView1.Rows[i].Cells[2].Style.ForeColor = Color.Blue;
        //                fllag = "▼";
        //            }
        //            else
        //            {
        //                this.dataGridView1.Rows[i].Cells[2].Style.ForeColor = Color.Black;
        //            }

        //            this.dataGridView1.Rows[i].Cells[2].Value = fllag + " " + Math.Abs(curVal).ToString("n0") + " " + "(" + upNdown + "%)";
        //        }
        //        #endregion
        //    }
        //}
    }
}
