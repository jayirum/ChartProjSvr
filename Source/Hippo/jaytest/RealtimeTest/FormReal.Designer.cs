namespace RealtimeTest
{
    partial class FormReal
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormReal));
            this.hHippoChart1 = new Hippo.WindowsForm4.hHippoChart();
            this.button1 = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.button4 = new System.Windows.Forms.Button();
            this.timer3 = new System.Windows.Forms.Timer(this.components);
            this.timer4 = new System.Windows.Forms.Timer(this.components);
            this.timer5 = new System.Windows.Forms.Timer(this.components);
            this.button5 = new System.Windows.Forms.Button();
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
            this.hHippoChart1.Location = new System.Drawing.Point(26, 35);
            this.hHippoChart1.Logo = ((Hippo.Logo)(resources.GetObject("hHippoChart1.Logo")));
            this.hHippoChart1.MinimumSize = new System.Drawing.Size(200, 140);
            this.hHippoChart1.Name = "hHippoChart1";
            this.hHippoChart1.PaletteType = Hippo.PaletteType.Default;
            this.hHippoChart1.ProcessorType = "64";
            this.hHippoChart1.SeriesAreaRate = null;
            this.hHippoChart1.SeriesListDictionary = ((Hippo.SeriesListDictionary)(resources.GetObject("hHippoChart1.SeriesListDictionary")));
            this.hHippoChart1.Size = new System.Drawing.Size(643, 353);
            this.hHippoChart1.TabIndex = 0;
            this.hHippoChart1.Titles = ((Hippo.Title)(resources.GetObject("hHippoChart1.Titles")));
            this.hHippoChart1.Top = 0F;
            this.hHippoChart1.Width2 = 400F;
            this.hHippoChart1.ChartSizeChanged += new System.EventHandler(this.hHippoChart1_ChartSizeChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(52, 425);
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
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(174, 414);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "button2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(279, 419);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(106, 35);
            this.button3.TabIndex = 3;
            this.button3.Text = "button3";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // timer2
            // 
            this.timer2.Tick += new System.EventHandler(this.timer2_Tick);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(456, 419);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 4;
            this.button4.Text = "button4";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // timer3
            // 
            this.timer3.Tick += new System.EventHandler(this.timer3_Tick);
            // 
            // timer5
            // 
            this.timer5.Tick += new System.EventHandler(this.timer5_Tick);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(580, 419);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(75, 23);
            this.button5.TabIndex = 5;
            this.button5.Text = "button5";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // hScrollBar1
            // 
            this.hScrollBar1.Location = new System.Drawing.Point(52, 457);
            this.hScrollBar1.Name = "hScrollBar1";
            this.hScrollBar1.Size = new System.Drawing.Size(611, 16);
            this.hScrollBar1.TabIndex = 6;
            this.hScrollBar1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hScrollBar1_Scroll);
            // 
            // FormReal
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(681, 481);
            this.Controls.Add(this.hScrollBar1);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.hHippoChart1);
            this.Name = "FormReal";
            this.Text = "FormReal";
            this.Load += new System.EventHandler(this.FormReal_Load_1);
            this.ResumeLayout(false);

        }

        #endregion

        private Hippo.WindowsForm4.hHippoChart hHippoChart1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Timer timer2;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Timer timer3;
        private System.Windows.Forms.Timer timer4;
        private System.Windows.Forms.Timer timer5;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.HScrollBar hScrollBar1;
    }
}

