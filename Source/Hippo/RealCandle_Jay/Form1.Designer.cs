namespace RealCandle_Jay
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.hHippoChart1 = new Hippo.WindowsForm4.hHippoChart();
            this.button1 = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.hScrollBar1 = new System.Windows.Forms.HScrollBar();
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
            this.hHippoChart1.Location = new System.Drawing.Point(37, 61);
            this.hHippoChart1.Logo = ((Hippo.Logo)(resources.GetObject("hHippoChart1.Logo")));
            this.hHippoChart1.MinimumSize = new System.Drawing.Size(200, 140);
            this.hHippoChart1.Name = "hHippoChart1";
            this.hHippoChart1.PaletteType = Hippo.PaletteType.Default;
            this.hHippoChart1.ProcessorType = "64";
            this.hHippoChart1.SeriesAreaRate = null;
            this.hHippoChart1.SeriesListDictionary = ((Hippo.SeriesListDictionary)(resources.GetObject("hHippoChart1.SeriesListDictionary")));
            this.hHippoChart1.Size = new System.Drawing.Size(689, 367);
            this.hHippoChart1.TabIndex = 0;
            this.hHippoChart1.Titles = ((Hippo.Title)(resources.GetObject("hHippoChart1.Titles")));
            this.hHippoChart1.Top = 0F;
            this.hHippoChart1.Width2 = 400F;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(23, 434);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // hScrollBar1
            // 
            this.hScrollBar1.Location = new System.Drawing.Point(63, 464);
            this.hScrollBar1.Name = "hScrollBar1";
            this.hScrollBar1.Size = new System.Drawing.Size(560, 28);
            this.hScrollBar1.TabIndex = 2;
            this.hScrollBar1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hScrollBar1_Scroll_1);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(845, 513);
            this.Controls.Add(this.hScrollBar1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.hHippoChart1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private Hippo.WindowsForm4.hHippoChart hHippoChart1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.HScrollBar hScrollBar1;
    }
}

