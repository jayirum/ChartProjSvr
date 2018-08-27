namespace Test
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.hHippoChart1 = new Hippo.WindowsForm4.hHippoChart();
            this.SuspendLayout();
            // 
            // hHippoChart1
            // 
            this.hHippoChart1.BackColor = System.Drawing.Color.Transparent;
            this.hHippoChart1.ChartGraphic = null;
            this.hHippoChart1.ChartID = null;
            this.hHippoChart1.ChartImage = null;
            this.hHippoChart1.ChartRectangle = ((System.Drawing.RectangleF)(resources.GetObject("hHippoChart1.ChartRectangle")));
            this.hHippoChart1.Designer = ((Hippo.ChartDesigner)(resources.GetObject("hHippoChart1.Designer")));
            this.hHippoChart1.DesignType = Hippo.ChartDesignType.None;
            this.hHippoChart1.Direction = Hippo.GraphAreaLocation.Vertical;
            this.hHippoChart1.Height2 = 270F;
            this.hHippoChart1.IsDrag = false;
            this.hHippoChart1.IsEmptyanalysis = true;
            this.hHippoChart1.IsUseContextMenu = true;
            this.hHippoChart1.Layout = ((Hippo.Layout)(resources.GetObject("hHippoChart1.Layout")));
            this.hHippoChart1.Left = 0F;
            this.hHippoChart1.LegendBox = ((Hippo.LegendBox)(resources.GetObject("hHippoChart1.LegendBox")));
            this.hHippoChart1.Location = new System.Drawing.Point(124, 96);
            this.hHippoChart1.Logo = ((Hippo.Logo)(resources.GetObject("hHippoChart1.Logo")));
            this.hHippoChart1.MinimumSize = new System.Drawing.Size(200, 140);
            this.hHippoChart1.Name = "hHippoChart1";
            this.hHippoChart1.PaletteType = Hippo.PaletteType.Default;
            this.hHippoChart1.ProcessorType = "64";
            this.hHippoChart1.SeriesAreaRate = null;
            this.hHippoChart1.SeriesListDictionary = ((Hippo.SeriesListDictionary)(resources.GetObject("hHippoChart1.SeriesListDictionary")));
            this.hHippoChart1.Size = new System.Drawing.Size(610, 367);
            this.hHippoChart1.TabIndex = 0;
            this.hHippoChart1.Titles = ((Hippo.Title)(resources.GetObject("hHippoChart1.Titles")));
            this.hHippoChart1.Top = 0F;
            this.hHippoChart1.Width2 = 400F;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 273);
            this.Controls.Add(this.hHippoChart1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private Hippo.WindowsForm4.hHippoChart hHippoChart1;
    }
}

