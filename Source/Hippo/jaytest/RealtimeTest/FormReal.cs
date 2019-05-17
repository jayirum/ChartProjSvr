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



namespace RealtimeTest
{


    public partial class FormReal : Form
    {
        List<SeriesItem> g_items;

        ///

        /// 한번에 몇 개의 포인트를 보여줄 것인지를 나타내는 변수
        ///
        const int viewcount = 50;

        ///

        /// 전체 데이터 카운트
        ///
        int currentIndex = 0;


        SeriesList seriesList = new SeriesList();

        // 차트에 한번에 그려질 데이터 개수 결정
        //double[] dataList = new double[20];
        //const int ItemCnt = 100;
        int nLastIdx = 0;
        const int nMax = 10;
        bool bCandle;

        public FormReal()
        {
            InitializeComponent();
        }

        private void hHippoChart1_ChartSizeChanged(object sender, EventArgs e)
        {
            this.hHippoChart1.DrawChart();

        }
        
        
        private void button1_Click(object sender, EventArgs e)
        {
            bCandle = false;
            InitDraw();
            nLastIdx = 0;
            this.timer1.Start();
        }

        private void InitDraw()
        {
            // 각종 차트 설정 초기화
            //seriesList.ChartType = ChartType.Line;
            seriesList.ChartType = ChartType.CandleStick;
            seriesList.AxisFactor.BackColor = Color.Khaki;
            seriesList.AxisFactor.YAxis.Direction = AxisDirection.Right;
            seriesList.AxisFactor.YAxis.AxisMagin = 50;
            seriesList.AxisFactor.XAxis.Interval = 1;
            //Series series = new Series();
            //series.Line.LineWidth = 2;
            //series.Points.Width = 3;

            seriesList.SeriesCollection.Add(new Series());

            // 시리즈아이템 생성 및 이름 결정
            //for (int i = 0; i < dataList.Length; i++)
            //{
            //    SeriesItem sItem = new SeriesItem();
            //    sItem.Name = "sItem" + i.ToString();

            //    series.items.Add(sItem);
            //}

            //seriesList.SeriesCollection.Add(series);
            hHippoChart1.RealTimeList.Add(new HippoRealTimeAttribute());
            hHippoChart1.SeriesListDictionary.Add(seriesList);

            this.timer1.Interval = 500;
        }
        private void InitDrawCandle()
        {
            this.hHippoChart1.SeriesListDictionary.Clear();
            this.hHippoChart1.Designer.InnerBackColor = Color.White;
            this.hHippoChart1.Titles.Label.Text = "";
            
            seriesList.ChartType = ChartType.CandleStick;
            seriesList.AxisFactor.YAxis.Direction = AxisDirection.Right;
            seriesList.AxisFactor.XAxis.DataType = AxisDataType.DateTime;
            seriesList.AxisFactor.XAxis.DateTimeLabelType = DateTimeLabelType.Day;
            seriesList.AxisFactor.XAxis.DateTimeFormat = "dd";

            seriesList.AxisFactor.XAxis.IsShowTick = false;
            seriesList.AxisFactor.XAxis.IsVisibleFigures = false;
            seriesList.GraphArea.Grid.IsBackGridColor = false;
            seriesList.AxisFactor.YAxis.AxisMagin = 50;

            seriesList.AxisFactor.YAxis.IsZeroStartScale = false;

            this.hHippoChart1.Titles.Label.Text = "주식 차트 기본 샘플";
            this.hHippoChart1.Designer.InnerBackColor = Color.FromArgb(214, 226, 239);
            this.hHippoChart1.LegendBox.Visible = false;

            //seriesList.AxisFactor.XAxis.Interval = 1;
            seriesList.SeriesCollection.Add(new Series());
            hHippoChart1.RealTimeList.Add(new HippoRealTimeAttribute());
            hHippoChart1.SeriesListDictionary.Add(seriesList);
        }
        private void FormReal_Load_1(object sender, EventArgs e)
        {
            

            // 차트로 그릴 데이터셋 초기화
            //for (int i = 0; i < dataList.Length; i++)
            //{
            //    dataList[i] = double.NaN;
            //}

            



        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            // 실시간 데이터 받아오기
            if (bCandle)
                DrawCandleStatic();
            else
            {
                Random r = new Random();
                Draw(r.Next(50,99));
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            bCandle = true;
            InitDrawCandle();
            nLastIdx = 1;
            this.timer1.Start();
            //DrawCandleStatic();
            //DrawCandleOrg();
        }

        private void DrawCandleStatic()
        {
            //this.hHippoChart1.SeriesListDictionary.Clear();
            //this.hHippoChart1.Designer.InnerBackColor = Color.White;
            //this.hHippoChart1.Titles.Label.Text = "";

            //SeriesList sList = new SeriesList();

            Random R = new Random();
            //for (nLastIdx = 22; nLastIdx < 50 + 1; nLastIdx++)
            //{
                SeriesItem item = new SeriesItem();

                item.XDateTimeValue = DateTime.Parse("2013-01-01").AddDays(nLastIdx);

                item.YStartValue = R.Next(1000, 1050) * nLastIdx - 11 * nLastIdx; // 시가
                item.LowValue = R.Next(1000, 1050) * nLastIdx - 12 * nLastIdx; // 저가 
                item.YValue = R.Next(1000, 1050) * nLastIdx - 15 * nLastIdx; // 종가
                item.HighValue = R.Next(1000, 1200) * nLastIdx - 10 * nLastIdx; // 고가

                if (nLastIdx > 0 && nLastIdx < 10)
                {
                    item.YStartValue = R.Next(750, 1000) * nLastIdx - 33 * nLastIdx; // 시가
                    item.LowValue = R.Next(950, 1000) * nLastIdx - 22 * nLastIdx; // 저가 
                    item.YValue = R.Next(950, 1000) * nLastIdx - 30 * nLastIdx; // 종가
                    item.HighValue = R.Next(555, 1200) * nLastIdx - 14 * nLastIdx; // 고가
                }

                if (nLastIdx > 30 && nLastIdx < 50)
                {
                    item.YStartValue = R.Next(1100, 1200) * nLastIdx - 40 * nLastIdx; // 시가
                    item.LowValue = R.Next(999, 1200) * nLastIdx - 60 * nLastIdx; // 저가 
                    item.YValue = R.Next(1100, 1200) * nLastIdx - 30 * nLastIdx; // 종가
                    item.HighValue = R.Next(888, 1200) * nLastIdx - 10 * nLastIdx; // 고가
                }



                seriesList.SeriesCollection[0].items.Add(item);
                //break;

            //}
            nLastIdx++;
            //seriesList.ChartType = ChartType.CandleStick;
            //seriesList.AxisFactor.YAxis.Direction = AxisDirection.Right;
            //seriesList.AxisFactor.XAxis.DataType = AxisDataType.DateTime;
            //seriesList.AxisFactor.XAxis.DateTimeLabelType = DateTimeLabelType.Day;
            //seriesList.AxisFactor.XAxis.DateTimeFormat = "dd";

            //seriesList.AxisFactor.XAxis.IsShowTick = false;
            //seriesList.AxisFactor.XAxis.IsVisibleFigures = false;
            //seriesList.GraphArea.Grid.IsBackGridColor = false;
            //seriesList.AxisFactor.YAxis.AxisMagin = 50;

            //seriesList.AxisFactor.YAxis.IsZeroStartScale = false;

            //this.hHippoChart1.Titles.Label.Text = "주식 차트 기본 샘플";
            //this.hHippoChart1.Designer.InnerBackColor = Color.FromArgb(214, 226, 239);
            //this.hHippoChart1.LegendBox.Visible = false;
            //this.hHippoChart1.SeriesListDictionary.Add(seriesList);
            //this.hHippoChart1.DrawChart();
            if(nLastIdx>2)
                this.hHippoChart1.DrawRealTimeChart( item,1);
        }

        private void DrawCandleOrg()
        {
            this.hHippoChart1.SeriesListDictionary.Clear();
            this.hHippoChart1.Designer.InnerBackColor = Color.White;
            this.hHippoChart1.Titles.Label.Text = "";

            SeriesList sList = new SeriesList();
            sList.SeriesCollection.Add(new Series());


            Random R = new Random();
            for (nLastIdx = 22; nLastIdx < 50 + 1; nLastIdx++)
            {
                SeriesItem item = new SeriesItem();

                item.XDateTimeValue = DateTime.Parse("2013-01-01").AddDays(nLastIdx);

                item.YStartValue = R.Next(1000, 1050) * nLastIdx - 11 * nLastIdx; // 시가
                item.LowValue = R.Next(1000, 1050) * nLastIdx - 12 * nLastIdx; // 저가 
                item.YValue = R.Next(1000, 1050) * nLastIdx - 15 * nLastIdx; // 종가
                item.HighValue = R.Next(1000, 1200) * nLastIdx - 10 * nLastIdx; // 고가

                if (nLastIdx > 0 && nLastIdx < 10)
                {
                    item.YStartValue = R.Next(750, 1000) * nLastIdx - 33 * nLastIdx; // 시가
                    item.LowValue = R.Next(950, 1000) * nLastIdx - 22 * nLastIdx; // 저가 
                    item.YValue = R.Next(950, 1000) * nLastIdx - 30 * nLastIdx; // 종가
                    item.HighValue = R.Next(555, 1200) * nLastIdx - 14 * nLastIdx; // 고가
                }

                if (nLastIdx > 30 && nLastIdx < 50)
                {
                    item.YStartValue = R.Next(1100, 1200) * nLastIdx - 40 * nLastIdx; // 시가
                    item.LowValue = R.Next(999, 1200) * nLastIdx - 60 * nLastIdx; // 저가 
                    item.YValue = R.Next(1100, 1200) * nLastIdx - 30 * nLastIdx; // 종가
                    item.HighValue = R.Next(888, 1200) * nLastIdx - 10 * nLastIdx; // 고가
                }

                sList.SeriesCollection[0].items.Add(item);
               // break;

            }
            //nLastIdx++;
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

            this.hHippoChart1.Titles.Label.Text = "주식 차트 기본 샘플";
            this.hHippoChart1.Designer.InnerBackColor = Color.FromArgb(214, 226, 239);
            this.hHippoChart1.LegendBox.Visible = false;
            this.hHippoChart1.SeriesListDictionary.Add(sList);
            this.hHippoChart1.DrawChart();
        }

        private void DrawCandle()
        { 
            

            Random R = new Random();
            Random r2 = new Random();
            int nRand = r2.Next(1, 10);
            int nRemain = nRand % 2;

            //for (int i = 0; i < 50 + 1; i++)
            //{
                SeriesItem item = new SeriesItem();

                //item.XDateTimeValue = DateTime.Parse("2018-12-27").AddDays(nLastIdx);

                if (nRemain == 0)
                {
                    item.YStartValue = R.Next(1000, 1050)  - 11 ; // 시가
                    item.LowValue = R.Next(1000, 1050)  - 12 ; // 저가 
                    item.YValue = R.Next(1000, 1050)  - 15 ; // 종가
                    item.HighValue = R.Next(1000, 1200) - 10 ; // 고가
                }
                else
                {
                    item.YStartValue = R.Next(1100, 1200)  - 40 ; // 시가
                    item.LowValue = R.Next(999, 1200)  - 60 ; // 저가 
                    item.YValue = R.Next(1100, 1200)  - 30 ; // 종가
                    item.HighValue = R.Next(888, 1200) - 10 ; // 고가
                }

                seriesList.SeriesCollection[0].items.Add(item);
            //}


            //this.hHippoChart1.DrawRealTimeChart(20, item);
            this.hHippoChart1.DrawChart();

            nLastIdx++;
        }


        bool bRealStart = false;
        SeriesItem realItem = new SeriesItem();
        string itemName = "ITEM-1";
        private void Draw(double addData)
        {
            if (!bRealStart)
            {
                realItem.Name = itemName;
                realItem.YStartValue = addData;
                realItem.LowValue = addData;
                realItem.YValue = addData;
                realItem.HighValue = addData;
                seriesList.SeriesCollection[0].items.Add(realItem);
            }
            else {
                realItem.YValue = addData;
                if (realItem.LowValue > addData)
                    realItem.LowValue = addData;

                if (realItem.HighValue < addData)
                    realItem.HighValue = addData;
            }
            bRealStart = true;
            //
            //int cnt = seriesList.SeriesCollection[0].items.Count();
            //hHippoChart1.DrawRealTimeChart(20, sItem);
            //System.Threading.Thread.Sleep(1000);

            //sItem.LowValue = addData - 20;
            // seriesList.SeriesCollection[0].items[cnt-1].LowValue = addData - 20;
            // hHippoChart1.DrawRealTimeChart(20, sItem);
            // System.Threading.Thread.Sleep(1000);

            // //sItem.YValue = addData + 15;
            // seriesList.SeriesCollection[0].items[cnt - 1].YValue = addData +15;
            // hHippoChart1.DrawRealTimeChart(20, sItem);
            // System.Threading.Thread.Sleep(1000);

            //// sItem.HighValue = addData + 30;
            // seriesList.SeriesCollection[0].items[cnt - 1].HighValue = addData +30;
            hHippoChart1.DrawRealTimeChart(realItem,1);
           // hHippoChart1.DrawChart(seriesList);
            // 그리기
            //hHippoChart1.DrawRealTimeChart(20, sItem);

            nLastIdx++;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            InitBtn3();
        }

        private void InitBtn3()
        {
            SeriesList sList = new SeriesList();
            sList.ChartType = ChartType.CandleStick;
            sList.AxisFactor.XAxis.Interval = 1;
            Random r = new Random();
            //for (int i = 0; i < 1; i++)
            //{
                Series sr = new Series();
               // sr.AreaType = AreaType.CurvedArea;
                sr.Line.LineWidth = 3;
                sList.SeriesCollection.Add(sr);
            //}

            this.hHippoChart1.SeriesListDictionary.Add(sList);
            //this.hHippoChart1.DrawChart();

            this.timer2.Interval = 2000;
            this.timer2.Start();
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            if (this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Count > 999)
            {
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.RemoveAt(0);
            }

            SeriesItem item = new SeriesItem();
            item.Name = DateTime.Now.Second.ToString();
            int val = new Random().Next(90);
            item.YStartValue = val;

            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Add(item);
            this.hHippoChart1.DrawChart();
            int nCnt = this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Count;
            System.Threading.Thread.Sleep(100);

            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[nCnt-1].LowValue = val - 25;
            this.hHippoChart1.DrawChart();
            System.Threading.Thread.Sleep(100);

            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[nCnt - 1].HighValue = val + 50;
            this.hHippoChart1.DrawChart();
            System.Threading.Thread.Sleep(100);

            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[nCnt - 1].YValue = val + 30;
            this.hHippoChart1.DrawChart();
            //this.DrawChart()
        }

        private void button4_Click(object sender, EventArgs e)
        {
            InitBtn4();
        }

        private void InitBtn4()
        {
            SeriesList sList = new SeriesList();
            sList.ChartType = ChartType.StackedColumn;
            sList.StackType = StackType.FullStacked;

            Random r = new Random();
            for (int i = 0; i < 2; i++)
            {
                Series sr = new Series();

                for (int x = 0; x < 2; x++)
                {
                    SeriesItem item = new SeriesItem();
                    item.Name = x.ToString();
                    item.YValue = 0;

                    sr.items.Add(item);
                }
                sList.SeriesCollection.Add(sr);
            }

            this.hHippoChart1.SeriesListDictionary.Add(sList);
            this.hHippoChart1.DrawChart();

            this.timer3.Interval = 1000;
            this.timer3.Start();
        }

        private void timer3_Tick(object sender, EventArgs e)
        {
            Random R = new Random();

            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[0].YValue = R.Next(78);
            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[1].items[0].YValue = R.Next(34);

            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[1].YValue = R.Next(232);
            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[1].items[1].YValue = R.Next(23);

            this.hHippoChart1.DrawChart();
        }

        private void timer5_Tick(object sender, EventArgs e)
        {
            Random r = new Random();

            SeriesItem newItem = new SeriesItem(r.Next(10, 50));
            newItem.Name = currentIndex.ToString();

            // 데이터 저장
            g_items.Add(newItem);

            hScrollBar1.Maximum = g_items.Count;
            hScrollBar1.LargeChange = viewcount;

            // 차트 그리기
            this.DrawChart(currentIndex, viewcount + currentIndex);

            if (g_items.Count > viewcount)
            {
                currentIndex++;
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            InitBtn5();
        }

        private void InitBtn5()
        {
            g_items = new List<SeriesItem>();

            SeriesList sl = new SeriesList();
            sl.GraphArea.Grid.GridBackColor1 = Color.Black;
            sl.GraphArea.Grid.GridBackColor2 = Color.Black;
            sl.AxisFactor.YAxis.Line.LineColor = Color.White;
            sl.AxisFactor.XAxis.Line.LineColor = Color.White;
            sl.AxisFactor.YAxis.ForeColor = Color.White;
            sl.AxisFactor.XAxis.ForeColor = Color.White;
            sl.GraphArea.Grid.GridDirection = GridDirection.Horizontal;

            Series sr = new Series();
            sr.SeriesColor = Color.Lime;
            sl.SeriesCollection.Add(sr);

            this.hHippoChart1.Designer.InnerBackColor = Color.Black;
            this.hHippoChart1.SeriesListDictionary.Add(sl);

            this.timer5.Interval = 30;
            this.timer5.Start();
        }

        ///

        /// 차트를 그립니다. 
        ///
        /// 시작 인덱스
        /// 마지막 인덱스
        private void DrawChart(int s, int e)
        {
            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Clear();

            for (int i = s; i < e; i++)
            {
                try
                {
                    this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Add(g_items[i]);
                }
                catch { }
            }

            this.hHippoChart1.DrawChart();
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
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


        AxisMarker mk = new AxisMarker();
        private void button6_Click(object sender, EventArgs e)
        {
            SeriesList sList = new SeriesList();
            sList.SeriesCollection.Add(new Series());
            sList.SeriesCollection.Add(new Series());
            sList.SeriesCollection.Add(new Series());

            hHippoChart1.RealTimeList.Add(new HippoRealTimeAttribute());
            hHippoChart1.RealTimeList.Add(new HippoRealTimeAttribute());
            hHippoChart1.RealTimeList.Add(new HippoRealTimeAttribute());

            // AxisMarker mk = new AxisMarker("This is line", 800);
            
            mk.Label.Text = "This is Line";
            mk.Name = "Line Marker";
            mk.BringToFront = true;
            mk.Line.LineColor = Color.BlueViolet;

            //AxisMarker mk2 = new AxisMarker("This is 2nd line", 500);
            //mk2.Name = "Line Marker 2";
            //mk2.BringToFront = true;
            //mk2.Line.LineColor = Color.Yellow;

            sList.AxisFactor.YAxis.Markers.Add(mk);
            //sList.AxisFactor.YAxis.Markers.Add(mk2);


            hHippoChart1.SeriesListDictionary.Add(sList);
            timer6.Interval = 100;
            timer6.Start();
        }

        private void timer6_Tick(object sender, EventArgs e)
        {
            SeriesItem[] items = new SeriesItem[3];//  { 0,0,0};

            items.SetValue(new SeriesItem(new Random().Next(999)), 0);
            items.SetValue(new SeriesItem(new Random().Next(555)), 1);
            items.SetValue(new SeriesItem(new Random().Next(777)), 2);

            hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.Markers[0].Value = new Random().Next(666);


            items[0].ItemColor = Color.Black;
            items[0].ItemColor = Color.Red;
            items[0].ItemColor = Color.Blue;
            items[2].Name = DateTime.Now.ToString("mm:ss");
            hHippoChart1.DrawRealTimeChart(100, items);
        }
    }
    

}
