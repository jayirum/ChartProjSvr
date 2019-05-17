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

namespace MyTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void hHippoChart1_ChartSizeChanged(object sender, EventArgs e)
        {
            this.hHippoChart1.DrawChart();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            SeriesList sList = new SeriesList();
            sList.ChartType = ChartType.Column;
            sList.IsShow3D = true;
            sList.Transparency = 66;
            sList.AxisFactor.YAxis.IsShorterFigures = true;
            sList.AxisFactor.YAxis.Decimalpoint = 1;
            Random r = new Random();
            for (int i = 0; i < 3; i++)
            {
                Series sr = new Series();
                for (int x = 0; x < 5; x++)
                {
                    SeriesItem item = new SeriesItem();
                    item.Name = x.ToString();
                    item.YValue = r.Next(50000, 1500000);
                    sr.items.Add(item);
                }
                sList.SeriesCollection.Add(sr);
            }
            this.hHippoChart1.Titles.Label.Text = "히포차트 4.3";
            this.hHippoChart1.SeriesListDictionary.Add(sList);
            this.hHippoChart1.DrawChart();
        }
    }
}
