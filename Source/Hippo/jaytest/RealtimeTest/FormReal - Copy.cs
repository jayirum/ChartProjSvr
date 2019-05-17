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
        SeriesList seriesList = new SeriesList();

        // 차트에 한번에 그려질 데이터 개수 결정
        double[] dataList = new double[20];
        //const int ItemCnt = 100;
       // int nLastIdx = 0;

        public FormReal()
        {
            InitializeComponent();
        }

        private void hHippoChart1_ChartSizeChanged(object sender, EventArgs e)
        {
            this.hHippoChart1.DrawChart();

        }
        

        private void Draw(double addData)
        {
            // 기존 데이터 Shift
            for (int i = 0; i < dataList.Length; i++)
            {
                if (i > 1)
                {
                    dataList[i - 1] = dataList[i];
                }
            }

            // 마지막 인덱스에 현재 생성한 데이터 입력
            dataList[dataList.Length - 1] = addData;

            // 데이터셋을 시리즈아이템에 입력
            for (int i = 0; i < dataList.Length; i++)
            {
                seriesList.SeriesCollection[0].items[i].YValue = dataList[i];
            }

            // 그리기
            hHippoChart1.DrawChart();
        }
        
        private void button1_Click(object sender, EventArgs e)
        {
            // 실시간 데이터 받아오기
            Random r = new Random();
            Draw(r.Next(99));
        }

        private void FormReal_Load_1(object sender, EventArgs e)
        {
            this.timer1.Interval = 1000;

            // 차트로 그릴 데이터셋 초기화
            for (int i = 0; i < dataList.Length; i++)
            {
                dataList[i] = double.NaN;
            }

            // 각종 차트 설정 초기화
            seriesList.ChartType = ChartType.Line;
            seriesList.AxisFactor.BackColor = Color.Khaki;

            Series series = new Series();
            series.Line.LineWidth = 2;
            series.Points.Width = 3;

            seriesList.SeriesCollection.Add(series);

            // 시리즈아이템 생성 및 이름 결정
            for (int i = 0; i < dataList.Length; i++)
            {
                SeriesItem seriesItem = new SeriesItem();
                seriesItem.Name = "seriesItem" + i.ToString();

                series.items.Add(seriesItem);
            }

            seriesList.SeriesCollection.Add(series);

            hHippoChart1.SeriesListDictionary.Add(seriesList);

            this.timer1.Start();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

            // 실시간 데이터 받아오기
            Random r = new Random();
            Draw(r.Next(99));
        }
    }
}
