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
using Hippo.WindowsForm4;

namespace HTSCandle
{
    public partial class Form1 : Form
    {
        SeriesList sList;
        SeriesList sList2;

        SeriesItem[] items;
        SeriesItem[] items2;

        // 총 데이터 수
        int totalCount = 1000;

        // 차트 한 화면에 보여줄 개수
        int counts = 100;

        public Form1()
        {
            InitializeComponent();

            Figures.CoordinateXtextLimitEmpty = 0;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Interval = 1000;

            Init();

            Draw();
        }

        private void Init()
        {
            sList = new SeriesList();
            sList2 = new SeriesList();

            sList.SeriesCollection.Add(new Series());
            sList2.SeriesCollection.Add(new Series());

            items = new SeriesItem[totalCount];
            items2 = new SeriesItem[totalCount];

            Random R = new Random();

            for (int i = 1; i < totalCount + 1; i++)
            {
                SeriesItem item = new SeriesItem();
                SeriesItem item2 = new SeriesItem();

                item.XDateTimeValue = DateTime.Parse("2013-01-01").AddDays(i);
                item2.XDateTimeValue = DateTime.Parse("2013-01-01").AddDays(i);

                item.YStartValue = R.Next(1000, 1050) * i - 11 * i; // 시가
                item.LowValue = R.Next(1000, 1050) * i - 12 * i; // 저가 
                item.YValue = R.Next(1000, 1050) * i - 15 * i; // 종가
                item.HighValue = R.Next(1000, 1050) * i - 10 * i; // 고가

                if (i > 950 && i < 990)
                {
                    item.YStartValue = R.Next(950, 1000) * i - 33 * i; // 시가
                    item.LowValue = R.Next(950, 1000) * i - 22 * i; // 저가 
                    item.YValue = R.Next(950, 1000) * i - 30 * i; // 종가
                    item.HighValue = R.Next(950, 1000) * i - 14 * i; // 고가
                }

                if (i > 200 && i < 600)
                {
                    item.YStartValue = R.Next(1100, 1200) * i - 40 * i; // 시가
                    item.LowValue = R.Next(1100, 1200) * i - 60 * i; // 저가 
                    item.YValue = R.Next(1100, 1200) * i - 30 * i; // 종가
                    item.HighValue = R.Next(1100, 1200) * i - 10 * i; // 고가
                }


                item2.YValue = R.Next(10000, 20000);

                items.SetValue(item, i - 1);
                items2.SetValue(item2, i - 1);
            }


            // 가장 뒤에 100 개를 먼저 그린다. 

            for (int i = totalCount - 1; i > totalCount - 1 - counts; i--)
            {
                sList.SeriesCollection[0].items.Add(items[i]);
                sList2.SeriesCollection[0].items.Add(items2[i]);
            }



            sList.ChartType = ChartType.CandleStick;
            sList.AxisFactor.YAxis.Direction = AxisDirection.Right;
            sList.AxisFactor.XAxis.DataType = AxisDataType.DateTime;
            sList.AxisFactor.XAxis.DateTimeLabelType = DateTimeLabelType.Day;
            sList.AxisFactor.XAxis.DateTimeFormat = "dd";

            sList.AxisFactor.XAxis.IsShowTick = false;
            sList.AxisFactor.XAxis.IsVisibleFigures = false;
            sList.GraphArea.Grid.IsBackGridColor = false;
            sList.AxisFactor.YAxis.AxisMagin = 50;

            sList.AxisFactor.YAxis.IsZeroStartScale = false;


            sList.AxisFactor.XAxis.Interval = counts / 10;
            sList2.AxisFactor.XAxis.Interval = counts / 10;

            sList2.ChartType = ChartType.Column;

            sList2.SeriesCollection[0].SeriesColor = Color.YellowGreen;

            sList2.AxisFactor.XAxis.DataType = AxisDataType.DateTime;
            sList2.AxisFactor.YAxis.Direction = AxisDirection.Right;
            sList2.AxisFactor.XAxis.DateTimeLabelType = DateTimeLabelType.Day;
            sList2.AxisFactor.XAxis.DateTimeFormat = "yy-MM-dd";

            sList2.AxisFactor.YAxis.IsZeroStartScale = false;
            sList2.GraphArea.Grid.IsBackGridColor = false;
            sList2.AxisFactor.YAxis.AxisMagin = 50;



            this.hHippoChart1.SeriesAreaRate = "6:2";
            this.hHippoChart1.Designer.InnerBackColor = Color.FromArgb(214, 226, 239);
            this.hHippoChart1.LegendBox.Visible = false;
            this.hHippoChart1.SeriesListDictionary.Add(sList);
            this.hHippoChart1.SeriesListDictionary.Add(sList2);
            this.hHippoChart1.DrawChart();

            AxisTick tk = new AxisTick(sList.SeriesCollection[0].items[sList.SeriesCollection[0].items.Count - 1].YValue);
            tk.Label.Text = sList.SeriesCollection[0].items[sList.SeriesCollection[0].items.Count - 1].YValue.ToString() + "\r\n3.86%";
            tk.BackColor = Color.Red;
            tk.Label.ForeColor = Color.White;
            sList.AxisFactor.YAxis.ExtraTicks.Add(tk);

            AxisTick tkMax = new AxisTick(sList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Max].Value);
            tkMax.Label.Text = sList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Max].Value.ToString();
            tkMax.IsShowGridLine = true;
            tkMax.GridLine.LineColor = Color.Red;
            tkMax.GridLine.LineWidth = 1;
            tkMax.GridLine.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;
            sList.AxisFactor.YAxis.ExtraTicks.Add(tkMax);

            AxisTick tkMin = new AxisTick(sList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Min].Value);
            tkMin.Label.Text = sList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Min].Value.ToString();
            tkMin.Label.ForeColor = Color.Blue;
            tkMin.IsShowGridLine = true;
            tkMin.GridLine.LineColor = Color.Blue;
            tkMin.GridLine.LineWidth = 1;
            tkMin.GridLine.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;
            sList.AxisFactor.YAxis.ExtraTicks.Add(tkMin);

            //AxisTick tk2 = new AxisTick(new PointF(0, this.hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.Y));
            //tk2.IsShowGridLine = true;
            //tk2.GridLine.LineColor = Color.Gray;
            //tk2.GridLine.LineWidth = 1;
            //sList.AxisFactor.XAxis.ExtraTicks.Add(tk2);

            //AxisTick tk1 = new AxisTick(new PointF(this.hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.X, 0));
            //tk1.IsShowGridLine = true;
            //tk1.GridLine.LineColor = Color.Gray;
            //tk1.GridLine.LineWidth = 1;
            //sList.AxisFactor.YAxis.ExtraTicks.Add(tk1);


            AxisTick tk33 = new AxisTick(sList2.SeriesCollection[0].items[sList2.SeriesCollection[0].items.Count - 1].YValue);
            tk33.Label.Text = sList2.SeriesCollection[0].items[sList2.SeriesCollection[0].items.Count - 1].YValue.ToString();
            tk33.BackColor = Color.Green;
            tk33.Label.ForeColor = Color.White;
            sList2.AxisFactor.YAxis.ExtraTicks.Add(tk33);


            this.hHippoChart1.DrawChart();
        }

        private void Draw()
        {
            sList.AxisFactor.XAxis.Interval = counts / 10;
            sList2.AxisFactor.XAxis.Interval = counts / 10;

            sList.AxisFactor.YAxis.ExtraTicks[0].Label.Text = sList.SeriesCollection[0].items[0].YValue.ToString() + "\r\n3.86%";
            sList.AxisFactor.YAxis.ExtraTicks[0].Value = sList.SeriesCollection[0].items[0].YValue;
            sList.AxisFactor.YAxis.ExtraTicks[0].BackColor = sList.SeriesCollection[0].SeriesColor;

            sList.AxisFactor.YAxis.ExtraTicks[1].Label.Text = sList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Max].Value.ToString();
            sList.AxisFactor.YAxis.ExtraTicks[1].Value = sList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Max].Value;

            sList.AxisFactor.YAxis.ExtraTicks[2].Label.Text = sList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Min].Value.ToString();
            sList.AxisFactor.YAxis.ExtraTicks[2].Value = sList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Min].Value;

            sList2.AxisFactor.YAxis.ExtraTicks[0].Label.Text = sList2.SeriesCollection[0].items[0].YValue.ToString();
            sList2.AxisFactor.YAxis.ExtraTicks[0].Value = sList2.SeriesCollection[0].items[0].YValue;

            this.hHippoChart1.DrawChart();
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            // 가상의 시리즈 아이템 주식 데이터 만들기 



            // 가상의 거래량 추가하기 



        }

        private void hHippoChart1_ChartSizeChanged(object sender, EventArgs e)
        {
            this.hHippoChart1.DrawChart();
        }

        private void hHippoChart1_ChartMouseMove(object sender, MouseEventArgs e)
        {
            //if (this.hHippoChart1.SeriesListDictionary.Count > 0)
            //{
            // double zeroX = this.hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.X;
            // double zeroY = this.hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.Y;

            // double coodiRecHeight = this.hHippoChart1.Size.Height;

            // float cX = (float)(zeroX + (e.X - this.hHippoChart1.SeriesListDictionary[0].GraphArea.LeftAxisRectangle.Width));
            // float cY = (float)(zeroY - (coodiRecHeight - e.Y
            // - this.hHippoChart1.SeriesListDictionary[0].GraphArea.BottomAxisRectangle.Height
            // - this.hHippoChart1.SeriesListDictionary[0].Margin
            // - Figures.LogoSpareEmpty - 10
            // ));

            // this.hHippoChart1.SeriesListDictionary[0].AxisFactor.XAxis.ExtraTicks[0].TickPoint = new PointF(cX, this.hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.Y);
            // this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[1].TickPoint = new PointF(this.hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.X, cY);

            // this.hHippoChart1.DrawChart();
            //} 
        }

        private void hScrollBar1_Scroll1(object sender, ScrollEventArgs e)
        {
            try
            {
                sList.SeriesCollection[0].items.Clear();
                sList2.SeriesCollection[0].items.Clear();

                int newIDX = totalCount - 1 - e.NewValue;

                for (int i = newIDX; i > newIDX - counts; i--)
                {
                    sList.SeriesCollection[0].items.Add(items[i]);
                    sList2.SeriesCollection[0].items.Add(items2[i]);
                }

                Draw();
            }
            catch { }
        }

        // 예전데이터로
        private void button1_Click(object sender, EventArgs e)
        {
            this.hScrollBar1.Value++;

            if (this.hScrollBar1.Value > totalCount - 2) this.hScrollBar1.Value = totalCount - 1;

            sList.SeriesCollection[0].items.Clear();
            sList2.SeriesCollection[0].items.Clear();

            int newIDX = totalCount - 1 - this.hScrollBar1.Value;

            for (int i = newIDX; i > newIDX - counts; i--)
            {
                sList.SeriesCollection[0].items.Add(items[i]);
                sList2.SeriesCollection[0].items.Add(items2[i]);
            }

            Draw();
        }

        // 최신
        private void button2_Click(object sender, EventArgs e)
        {
            int idx = this.hScrollBar1.Value;

            idx--;

            if (idx < 0) idx = 0;

            this.hScrollBar1.Value = idx;

            sList.SeriesCollection[0].items.Clear();
            sList2.SeriesCollection[0].items.Clear();

            int newIDX = totalCount - 1 - this.hScrollBar1.Value;

            for (int i = newIDX; i > newIDX - counts; i--)
            {
                sList.SeriesCollection[0].items.Add(items[i]);
                sList2.SeriesCollection[0].items.Add(items2[i]);
            }

            Draw();
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            this.hScrollBar1.Value = 0;

            counts = this.trackBar1.Value;

            sList.SeriesCollection[0].items.Clear();
            sList2.SeriesCollection[0].items.Clear();

            for (int i = totalCount - 1; i > totalCount - 1 - counts; i--)
            {
                sList.SeriesCollection[0].items.Add(items[i]);
                sList2.SeriesCollection[0].items.Add(items2[i]);
            }

            Draw();
        }

        ///

        /// 일
        ///
        /// 
        /// 
        private void button4_Click(object sender, EventArgs e)
        {

        }

        ///

        /// 주
        ///
        /// 
        /// 
        private void button5_Click(object sender, EventArgs e)
        {

        }

        ///

        /// 월
        ///
        /// 
        /// 
        private void button6_Click(object sender, EventArgs e)
        {

        }

        ///

        /// 블랙 테마
        ///
        /// 
        /// 
        private void panel2_Click(object sender, EventArgs e)
        {
            //if (panel2.BackColor != Color.White)
            //{
            //    sList.AxisFactor.BackColor = Color.FromArgb(61, 61, 61);

            //    sList.AxisFactor.YAxis.Line.LineColor = Color.White;
            //    sList.AxisFactor.XAxis.Line.LineColor = Color.White;
            //    sList.AxisFactor.YAxis.ForeColor = Color.White;
            //    sList.AxisFactor.XAxis.ForeColor = Color.White;

            //    sList2.AxisFactor.BackColor = Color.FromArgb(61, 61, 61);

            //    sList2.AxisFactor.YAxis.Line.LineColor = Color.White;
            //    sList2.AxisFactor.XAxis.Line.LineColor = Color.White;
            //    sList2.AxisFactor.YAxis.ForeColor = Color.White;
            //    sList2.AxisFactor.XAxis.ForeColor = Color.White;

            //    hHippoChart1.Designer.InnerBackColor = Color.FromArgb(61, 61, 61);

            //    this.hHippoChart1.DrawChart();

            //    //panel1.BackColor = Color.FromArgb(49, 49, 49);
            //    //panel2.BackColor = Color.White;
            //}
            //else
            {
                Init();

                //panel1.BackColor = SystemColors.Control;
                //panel2.BackColor = Color.FromArgb(61, 61, 61);
            }
        }

        
    }
}
