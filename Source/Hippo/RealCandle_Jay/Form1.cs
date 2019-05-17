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


namespace RealCandle_Jay
{
    struct SRealDraw
    {
        public int CalcCnt;
        public int NowCnt;
    }

    public partial class Form1 : Form
    {
        //List<SeriesItem> g_items;
        const int MAX_CHART_CNT = 20;
        /// 한번에 몇 개의 포인트를 보여줄 것인지를 나타내는 변수
        ///
        const int viewcount = 5;

        SRealDraw realCalc;

        ///

        /// 전체 데이터 카운트
        ///
        int currentIndex = 0;
        int nowIdx = 0;
        int nowSeriesCnt = 0;
        int nItemName = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void hHippoChart1_ChartSizeChanged(object sender, EventArgs e)
        {
            this.hHippoChart1.DrawChart();

        }

        private void button1_Click(object sender, EventArgs e)
        {
            Init();
        }

        private void Init()
        {
            //g_items = new List<SeriesItem>();

            SeriesList sl = new SeriesList();   
            sl.ChartType = ChartType.CandleStick;
            sl.AxisFactor.XAxis.Interval = 1;
            sl.Margin = 0;

            sl.GraphArea.Grid.GridBackColor1 = Color.Black;
            sl.GraphArea.Grid.GridBackColor2 = Color.Black;
            sl.AxisFactor.YAxis.Line.LineColor = Color.White;
            sl.AxisFactor.XAxis.Line.LineColor = Color.White;
            sl.AxisFactor.YAxis.ForeColor = Color.White;
            sl.AxisFactor.XAxis.ForeColor = Color.White;
            sl.GraphArea.Grid.GridDirection = GridDirection.Horizontal;

            //// y축 추가
            //Axis y2 = new Axis(AxisType.Ytype);
            //y2.Direction = AxisDirection.Right;
            //y2.Visible = true;
            //y2.AxisMagin = 0;

            //sl.AxisFactor.AxisItems.Add(y2);

            Series sr = new Series();
            sr.Name = "test";
            sr.SeriesColor = Color.Lime;
            sr.Line.LineWidth = 3;

            sl.SeriesCollection.Add(sr);

            this.hHippoChart1.Designer.InnerBackColor = Color.Black;
            this.hHippoChart1.SeriesListDictionary.Add(sl);
            this.hHippoChart1.LegendBox.Visible = true;
            this.hHippoChart1.LegendBox.Header.ValueLabel.Text = "Legend Header";
            nowSeriesCnt = 0;
            LoadPrevData();
        }

        private void LoadPrevData()
        {
            Random R = new Random();

            for (int i = 0; i < 2; i++)
            {
                int val = R.Next(100, 500);
                bool bOdd = (val % 2 == 0) ? true : false;

                SeriesItem newItem = new SeriesItem();
                newItem.Name = nowSeriesCnt.ToString();

                newItem.YStartValue = val;
                newItem.LowValue = val - 100;
                newItem.YValue = (bOdd) ? val - 100 : val + 100;
                newItem.HighValue = val + 100;

                
                // 데이터 저장
               // g_items.Add(newItem);
                hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Add(newItem);
                hScrollBar1.Maximum = hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Count;
                nowSeriesCnt++;
                nItemName++;
                hScrollBar1.LargeChange = viewcount;
                hHippoChart1.DrawChart();

                //if (g_items.Count > viewcount)
                //{
                //    currentIndex++;
                //}
            }

            realCalc.CalcCnt = 10;
            realCalc.NowCnt = 0;

            AxisMarker mk = new AxisMarker();
            
            mk.BringToFront = true;
            mk.Line.LineColor = Color.BlueViolet;
            hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.Markers.Add(mk);

            //AxisMarker mk2 = new AxisMarker();
            //mk2.BringToFront = true;
            //mk2.Line.LineColor = Color.White;
            //mk2.Line.LineWidth = 10;
            //hHippoChart1.SeriesListDictionary[0].AxisFactor.XAxis.Markers.Add(mk2);



            AxisTick tick = new AxisTick();
            hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks.Add(tick);

            
            //            AxisTick tk2 = new AxisTick(new PointF(
            //this.hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.X + 50,
            //this.hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.Y));
            //            tk2.IsShowGridLine = true;
            //            tk2.GridLine.LineColor = Color.Blue;
            //            tk2.GridLine.LineWidth = 5;




            this.timer1.Interval = 100;
            this.timer1.Start();
        }


        // 실시간 차트를 그린다.
        private void timer1_Tick(object sender, EventArgs e)
        {
            int val = new Random().Next(100, 500);
            SeriesItem item;
          
            if (realCalc.NowCnt == 0)
            {
                item = new SeriesItem();
                item.Name = nItemName.ToString();

                item.YStartValue = val;
                item.LowValue = val;
                item.YValue = val;
                item.HighValue = val;
               // hHippoChart1.SeriesListDictionary[0].AxisFactor.XAxis.Markers[0].Value = nItemName;
                // 데이터 저장
                //g_items.Add(item);

                //hHippoChart1.RealTimeList.Add(new HippoRealTimeAttribute());
                hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Add(item);
                hScrollBar1.Maximum = ++nItemName;
                nowSeriesCnt++;
                
            }
            else
            {
                item = hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[nowSeriesCnt-1];

                item.YValue = val;
                if (item.LowValue > val) item.LowValue = val;
                if (item.HighValue < val) item.HighValue = val;

                //g_items[g_items.Count - 1] = item;
            }


            hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[0].Value = val;
            hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[0].Label.Text = val.ToString();
            hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.Markers[0].Value = val;

            hHippoChart1.DrawChart();


            if (++realCalc.NowCnt == realCalc.CalcCnt)
            {
                //nowSeriesCnt++;
                realCalc.NowCnt = 0;
                ReComposeChart();
            }
        }

        private void ReComposeChart()
        {
            if (nowSeriesCnt  < MAX_CHART_CNT)
                return;

            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.RemoveAt(0);
            nowSeriesCnt--;
            this.hHippoChart1.DrawChart();
        }

        private void DrawChart(int s, int e)
        {
            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Clear();

            for (int i = s; i < e; i++)
            {
                try
                {
                    //this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Add(g_items[i]);
                }
                catch { }
            }

            this.hHippoChart1.DrawChart();
        }

       

        private void hScrollBar1_Scroll_1(object sender, ScrollEventArgs e)
        {
            int startVal = hScrollBar1.Maximum - viewcount - hScrollBar1.Value;
            int EndVal = hScrollBar1.Maximum - hScrollBar1.Value;

            if (startVal < 0)
            {
                startVal = 0;
                EndVal = viewcount;
            }

            this.DrawChart(startVal, EndVal);

        }
    }
}
