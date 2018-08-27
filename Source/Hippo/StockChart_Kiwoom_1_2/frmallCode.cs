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
    public partial class frmallCode : Form
    {
        AxKHOpenAPILib.AxKHOpenAPI axKHOpenAPI1;
        public string selectedCode;
        public string selectedCodeName;

        public frmallCode(AxKHOpenAPILib.AxKHOpenAPI axKHOpenAPI1copy)
        {
            InitializeComponent();

            axKHOpenAPI1 = axKHOpenAPI1copy;
        }

        private void frmallCode_Load(object sender, EventArgs e)
        {
            Search("");

            this.textBox1.Focus();
        } 

        private void Search(string keyword)
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

                        if (string.IsNullOrEmpty(keyword))
                        {
                            dt.Rows.Add(row);
                        }
                        else
                        {
                            if(StockName.IndexOf(keyword) > -1)
                            {
                                dt.Rows.Add(row);
                            }
                        }

                        i++;
                    }
                }
            }

            dt.AcceptChanges();

            this.dataGridView1.DataSource = dt;

            this.dataGridView1.Columns[0].Width = 80;
            this.dataGridView1.Columns[1].Width = this.dataGridView1.Width - 90;
        }

        private void frmallCode_SizeChanged(object sender, EventArgs e)
        {  

        }

        private void dataGridView1_CellContentDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            this.selectedCode = this.dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString();
            this.selectedCodeName = this.dataGridView1.Rows[e.RowIndex].Cells[1].Value.ToString();

            this.DialogResult = DialogResult.OK;
        }

        /// <summary>
        /// 종목 이름 검색
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        { 
            string codename = this.textBox1.Text.Trim();

            this.Search(codename);
        }

        private void frmallCode_KeyDown(object sender, KeyEventArgs e)
        {
            switch(e.KeyCode)
            {
                case Keys.Enter:
                    button1_Click(null, new EventArgs());
                    break;
                        
            }
        }
    }
}
