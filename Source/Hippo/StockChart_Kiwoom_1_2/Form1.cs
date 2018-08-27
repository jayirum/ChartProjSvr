using System; 
using System.Windows.Forms;
using KHOpenAPILib;
using System.Data;
using System.Xml;
using System.Collections.Generic;
using System.Drawing;
using Hippo;
using System.IO;

// ====================================================================================================
// =============  키움증권 api 히포차트 연동 프로젝트(ver 1.2.0)  =====================================
// 1. 제작 : 히포차트 
// 2. 사용 대상 : 히포차트에서 인증하는 라이선스 소유자 혹은 기업
// 3. 본 프로젝트는 절대 외부에 배포하거나 웹에 올리시면 안됩니다. 
// 4. 본 프로젝트는 평가판으로 실행이 되지 않으며 별도의 dll이 있어야 정상 사용 가능합니다.
// 5. 개발 문의는 http://cafe.naver.com/kiwoomhippochart
// 6. 추가 사항 : 일목균형표, 
// ====================================================================================================
namespace TTN
{ 
    public partial class Form1 : Form
    {
        #region 변수들
        int ViewCount = 30;
        int scrollNewIndex;

        List<SeriesItem> items;
        List<SeriesItem> items_vol;

        List<SeriesItem> items_bong;
        List<SeriesItem> items_bong_vol;  

        // 이평선 데이터 (체결)
        SortedList<int, SeriesItem> items5;
        SortedList<int, SeriesItem> items10;
        SortedList<int, SeriesItem> items20; // 볼린저밴드에도 이용함
        SortedList<int, SeriesItem> items60;
        SortedList<int, SeriesItem> items120;

        // 12일선, 26일선
        SortedList<int, SeriesItem> items12;
        SortedList<int, SeriesItem> items26;

        // 거래량 이평선
        SortedList<int, SeriesItem> itemsVol5;
        SortedList<int, SeriesItem> itemsVol20; 
        SortedList<int, SeriesItem> itemsVol60;
        SortedList<int, SeriesItem> itemsVol120; 

        // 볼린저밴드
        SortedList<int, SeriesItem> itemsBollingerBands;  

        //MACD, 시그널, 오실레이터
        SortedList<int, SeriesItem> itemsMACD;
        SortedList<int, SeriesItem> itemsSignal;
        SortedList<int, SeriesItem> itemsOsilator;

        // 일목균형표
        SortedList<int, SeriesItem> itemsILMOK1; // 전환선
        SortedList<int, SeriesItem> itemsILMOK2; // 기준선
        SortedList<int, SeriesItem> itemsILMOK3; // 선행스팬1
        SortedList<int, SeriesItem> itemsILMOK4; // 선행스팬2
        SortedList<int, SeriesItem> itemsILMOK5; // 후행스팬  

        // 스토캐스틱
        SortedList<int, SeriesItem> itemsStochastic;

        // RSI
        SortedList<int, SeriesItem> itemsRSI;


        SeriesList curList;
        SeriesList curVolList;
        //SeriesList curMACD;

        int currentRowIndex;

        // 현재 선택된 코드, 종목
        string selectedCode;
        string selectedCodeName;
         
        // tick, day, mon, min, week 
        string selectedChartType = "tick";
        // 분동, 틱 등에 사용되는 1, 3, 5, 10 ...
        string selectedSU = "1";
        // 선택된 항목 tr 코드
        string selectedTRCODE = "OPT10001";
        // 선택된 종목의 현재가
        int selectedPrice = 0;

        string uID;
        string uName;

        string themeFlag = "default"; 

        // 실시간 데이터 저장
        bool isRealDataSaveStart = false;
        string RealDataSet;

        // 관심종목 
        SortedList<string, int> favorsIdx = new SortedList<string, int>();

        // 조건검색을 담아 놓는다. 
        SortedList<string, string> conditionList = new SortedList<string, string>();

        // 실시간 봉
        SeriesItem item_real_bong;
        SeriesItem item_realVol_bong;

        #endregion

        #region 생성자, 초기화, 폼로드
        public Form1()
        {
            InitializeComponent();

            this.hScrollBar1.Maximum = ViewCount; 

            // 거래구분목록 지정
            for (int i = 0; i < 12; i++)
            {
                comOderType.Items.Add(KOACode.hogaGb[i].name);
            }

            // 주문유형
            for (int i = 0; i < 5; i++)
            {
                comBuySellType.Items.Add(KOACode.orderType[i].name);
            }

            Init();

            this.SetTheme();
        }

        /// <summary>
        /// 테마 변경 - 추후 구현
        /// </summary>
        private void SetTheme()
        {
            if (themeFlag == "black")
            {

            }
            else if (themeFlag == "defalut")
            {

            } 

            this.hHippoChart1.Designer.InnerBackColor = Color.FromArgb(214, 226, 239);
        }

        private void Form1_Load(object sender, EventArgs e)
        { 
            if (axKHOpenAPI1.CommConnect() == 0)
            {
                WriteLog("로그인창 열기 성공");
            }
            else
            {
                WriteLog("로그인창 열기 실패");
            }
        }

        /// <summary>
        /// 초기화 기초 설정 메소드
        /// </summary>
        private void Init()
        { 
            this.hHippoChart1.SeriesListDictionary.Clear();

            // 모든 값을 저장하는 컬렉션
            items = new List<SeriesItem>();
            items_vol = new List<SeriesItem>();
            items_bong = new List<SeriesItem>();
            items_bong_vol = new List<SeriesItem>();

            #region 이평선, 보조지표 컬렉션
            // 이평선 컬렉션 
            items5 = new SortedList<int, SeriesItem>();
            items10 = new SortedList<int, SeriesItem>();
            items20 = new SortedList<int, SeriesItem>();
            items60 = new SortedList<int, SeriesItem>();
            items120 = new SortedList<int, SeriesItem>();

            items12 = new SortedList<int, SeriesItem>();
            items26 = new SortedList<int, SeriesItem>();

            itemsMACD = new SortedList<int, SeriesItem>();
            itemsSignal = new SortedList<int, SeriesItem>();
            itemsOsilator = new SortedList<int, SeriesItem>();

            // 거래량 이동평균선 컬렉션
            itemsVol5 = new SortedList<int, SeriesItem>();
            itemsVol20 = new SortedList<int, SeriesItem>();
            itemsVol60 = new SortedList<int, SeriesItem>();
            itemsVol120 = new SortedList<int, SeriesItem>(); 

            // 볼린저밴드
            itemsBollingerBands = new SortedList<int, SeriesItem>();

            // 일목균형표
            itemsILMOK1 = new SortedList<int, SeriesItem>();
            itemsILMOK2 = new SortedList<int, SeriesItem>();
            itemsILMOK3 = new SortedList<int, SeriesItem>();
            itemsILMOK4 = new SortedList<int, SeriesItem>();
            itemsILMOK5 = new SortedList<int, SeriesItem>();

            // 스토캐스틱
            itemsStochastic = new SortedList<int, SeriesItem>();

            // RSI
            itemsRSI = new SortedList<int, SeriesItem>();

            #endregion

            #region 각종 시리즈 설정
            // 주가 이평선 
            Series sr_line5 = new Series(); sr_line5.Name = "5";
            Series sr_line10 = new Series(); sr_line10.Name = "10";
            Series sr_line20 = new Series(); sr_line20.Name = "20";
            Series sr_line60 = new Series(); sr_line60.Name = "60";
            Series sr_line120 = new Series(); sr_line120.Name = "120"; 

            Series sr_BollingerBands = new Series(); sr_BollingerBands.Name = "BB";
            sr_BollingerBands.ChartType = ChartType.AreaLine;
            sr_BollingerBands.Transparency = 130;
            sr_BollingerBands.SeriesColor = Color.FromArgb(250, 247, 243);
            sr_BollingerBands.Line.AreaDesignType = AreaDesignType.Dark;

            Series sr_MACD = new Series(); sr_MACD.Name = "MACD";
            Series sr_signal = new Series(); sr_signal.Name = "signal";
            Series sr_osilator = new Series(); sr_osilator.Name = "OSC";
            sr_osilator.ChartType = ChartType.Column; // 오실레이터는 막대차트

            Series sr_ilmok1 = new Series(); sr_ilmok1.Name = "전환선";
            Series sr_ilmok2 = new Series(); sr_ilmok2.Name = "기준선";
            Series sr_ilmok3 = new Series(); sr_ilmok3.Name = "선행1";
            sr_ilmok3.ChartType = ChartType.AreaLine;
            sr_ilmok3.Transparency = 100;
            sr_ilmok3.SeriesColor = Color.FromArgb(254, 194, 181); 

            Series sr_ilmok4 = new Series(); sr_ilmok4.Name = "선행2";
            //sr_ilmok4.ChartType = ChartType.AreaLine;
            //sr_ilmok4.Transparency = 200;
            //sr_ilmok4.SeriesColor = Color.FromArgb(210, 225, 254);

            Series sr_ilmok5 = new Series(); sr_ilmok5.Name = "후행"; 
            sr_ilmok5.SeriesColor = Color.Gray;

            Series sr_Stochastic = new Series(); sr_Stochastic.Name = "STC";

            Series sr_RSI = new Series(); sr_RSI.Name = "RSI";

            sr_BollingerBands.Visible = false; 
            sr_MACD.Visible = false;
            sr_signal.Visible = false;
            sr_osilator.Visible = false;
            sr_ilmok1.Visible = false;
            sr_ilmok2.Visible = false;
            sr_ilmok3.Visible = false;
            sr_ilmok4.Visible = false;
            sr_ilmok5.Visible = false;
            sr_Stochastic.Visible = false;
            sr_RSI.Visible = false;

            // 범례
            sr_BollingerBands.LegendVisible = false;
            sr_MACD.LegendVisible = false;
            sr_signal.LegendVisible = false;
            sr_osilator.LegendVisible = false;
            sr_ilmok1.LegendVisible = false;
            sr_ilmok2.LegendVisible = false;
            sr_ilmok3.LegendVisible = false;
            sr_ilmok4.LegendVisible = false;
            sr_ilmok5.LegendVisible = false;
            sr_Stochastic.LegendVisible = false;
            sr_RSI.LegendVisible = false;

            // 포인트 굵기 = 0
            int pointsww = 0;
            sr_line5.Points.Width = pointsww;
            sr_line10.Points.Width = pointsww;
            sr_line20.Points.Width = pointsww;
            sr_line60.Points.Width = pointsww;
            sr_line120.Points.Width = pointsww;
            sr_BollingerBands.Points.Width = pointsww; 
            sr_MACD.Points.Width = pointsww;
            sr_signal.Points.Width = pointsww;
            sr_osilator.Points.Width = pointsww; 
            sr_ilmok1.Points.Width = pointsww;
            sr_ilmok2.Points.Width = pointsww;
            sr_ilmok3.Points.Width = pointsww;
            sr_ilmok4.Points.Width = pointsww;
            sr_ilmok5.Points.Width = pointsww;
            sr_Stochastic.Points.Width = pointsww;
            sr_RSI.Points.Width = pointsww;

            // 라인 굵기
            int lineww = 2;
            sr_line5.Line.LineWidth = lineww;
            sr_line10.Line.LineWidth = lineww;
            sr_line20.Line.LineWidth = lineww;
            sr_line60.Line.LineWidth = lineww;
            sr_line120.Line.LineWidth = lineww;
            sr_BollingerBands.Line.LineWidth = lineww; 
            sr_MACD.Line.LineWidth = lineww;
            sr_signal.Line.LineWidth = lineww;
            sr_osilator.Line.LineWidth = lineww; 
            sr_ilmok1.Line.LineWidth = lineww;
            sr_ilmok2.Line.LineWidth = lineww;
            sr_ilmok3.Line.LineWidth = lineww;
            sr_ilmok4.Line.LineWidth = lineww;
            sr_ilmok5.Line.LineWidth = lineww;
            sr_Stochastic.Line.LineWidth = lineww;
            sr_RSI.Line.LineWidth = lineww;

            #endregion

            #region 주가 차트
            // 주가
            curList = new SeriesList();
            curList.SeriesCollection.Add(new Series()); // 메인 시리즈 추가
            curList.SeriesCollection[0].SeriesColor = System.Drawing.Color.Red;
            curList.SeriesCollection[0].Points.Width = 0;
            curList.SeriesCollection[0].Name = "주가";

            // 1. 시리즈 추가
            // 시리즈 추가는 순서를 정확히 지켜줘야합니다. 번호를 따라가보세요
            curList.SeriesCollection.Add(sr_line5);
            //curList.SeriesCollection.Add(sr_line10);
            curList.SeriesCollection.Add(sr_line20);
            curList.SeriesCollection.Add(sr_line60);
            //curList.SeriesCollection.Add(sr_line120); 
            curList.SeriesCollection.Add(sr_BollingerBands);

            curList.AxisFactor.YAxis.Direction = AxisDirection.Right;
            curList.AxisFactor.YAxis.IsZeroStartScale = false;
            curList.GraphArea.Grid.IsBackGridColor = false;
            curList.GraphArea.Grid.GridDirection = GridDirection.Horizontal;

            curList.AxisFactor.XAxis.IsShowTick = false;
            curList.AxisFactor.XAxis.Visible = false;

            // 차트 내부에 있는 범례를 표시하자
            curList.Legend.Visible = true; 

            curList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Max].Visible = true;
            curList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Min].Visible = true;
            curList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Max].Label.ForeColor = System.Drawing.Color.Red;
            curList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Min].Label.ForeColor = System.Drawing.Color.Blue;
            curList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Max].Line.LineColor = System.Drawing.Color.Red;
            curList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Min].Line.LineColor = System.Drawing.Color.Blue;
            curList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Max].LegendVisible = false;
            curList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Min].LegendVisible = false;
            curList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Max].Decimalpoint = 0;
            curList.AxisFactor.YAxis.AnalysisItems[AnalysisCategory.Min].Decimalpoint = 0;

            // 현재 체결 값 표시
            AxisTick tk = new AxisTick();
            tk.Label.ForeColor = System.Drawing.Color.White;
            curList.AxisFactor.YAxis.ExtraTicks.Add(tk);

            #endregion

            #region 거래량
            // 거래량
            curVolList = new SeriesList();

            curVolList.SeriesCollection.Add(new Series());
            curVolList.SeriesCollection[0].SeriesColor = System.Drawing.Color.YellowGreen;
            curVolList.SeriesCollection[0].Name = "거래량";
            curVolList.Legend.Visible = true;

            curVolList.ChartType = ChartType.Column;
            curVolList.AxisFactor.YAxis.Direction = AxisDirection.Right;
            curVolList.GraphArea.Grid.IsBackGridColor = false;
            curVolList.GraphArea.Grid.GridDirection = GridDirection.Horizontal;

            //curVolList.AxisFactor.XAxis.IsShowTick = false;
            curVolList.AxisFactor.YAxis.IsShorterFigures = true;
            curVolList.AxisFactor.YAxis.Decimalpoint = 1; 

            #endregion

            // 임시 데이터 세팅
            #region
            for (int i = 0; i < ViewCount; i++)
            {
                SeriesItem item = new SeriesItem();
                item.YValue = double.NaN;
                item.Balloon = new Balloon();
                item.Visible = false;

                curList.SeriesCollection[0].items.Add(item);
                curVolList.SeriesCollection[0].items.Add(item);
            }
            #endregion

            // 2. 
            #region MACD - 멀티축
            // MACD   
            curList.SeriesCollection.Add(sr_MACD);
            curList.SeriesCollection.Add(sr_signal);
            curList.SeriesCollection.Add(sr_osilator);

            curList.AxisFactor.AxisItems.Add(new Axis(AxisType.Ytype));
            curList.AxisFactor.AxisItems[0].Visible = false;
            curList.AxisFactor.AxisItems[0].AxisMagin = 0;

            //AxisMarker mk = new AxisMarker("", 0);
            //mk.Line.LineColor = Color.Red;
            //mk.Line.DashStyle = System.Drawing.Drawing2D.DashStyle.Dot;

            //curList.AxisFactor.AxisItems[0].Markers.Add(mk);

            sr_MACD.AxisIndex = 1;
            sr_signal.AxisIndex = 1;
            sr_osilator.AxisIndex = 1;
            sr_MACD.SeriesColor = Color.Gray;
            sr_signal.SeriesColor = Color.Red;
            sr_osilator.SeriesColor = Color.SkyBlue;

            #endregion

            // 3. 
            #region 일목균형표 시리즈 추가

            curList.SeriesCollection.Add(sr_ilmok1);
            curList.SeriesCollection.Add(sr_ilmok2);
            curList.SeriesCollection.Add(sr_ilmok3);
            curList.SeriesCollection.Add(sr_ilmok4);
            curList.SeriesCollection.Add(sr_ilmok5);

            #endregion

            // 4. 
            #region 스토캐스틱 - 멀티축

            curList.SeriesCollection.Add(sr_Stochastic);

            curList.AxisFactor.AxisItems.Add(new Axis(AxisType.Ytype));
            curList.AxisFactor.AxisItems[1].Visible = false;
            curList.AxisFactor.AxisItems[1].AxisMagin = 0;

            sr_Stochastic.AxisIndex = 2;
            sr_Stochastic.SeriesColor = Color.Green;

            #endregion
             
            // 5. 
            #region RSI - 멀티축

            curList.SeriesCollection.Add(sr_RSI);

            curList.AxisFactor.AxisItems.Add(new Axis(AxisType.Ytype));
            curList.AxisFactor.AxisItems[2].Visible = false;
            curList.AxisFactor.AxisItems[2].AxisMagin = 0;

            sr_RSI.AxisIndex = 3;
            sr_RSI.SeriesColor = Color.Pink;

            #endregion

            // 거래량과 주가 차트의 우측 축 부분의 넓이를 같게 만들어 줌
            curList.AxisFactor.YAxis.AxisMagin = 50;
            curVolList.AxisFactor.YAxis.AxisMagin = 50; 

            this.hHippoChart1.SeriesAreaRate = "8:2"; // 주가차트와 거래량의 차트 높이 비율 

            this.hHippoChart1.SeriesListDictionary.Add(curList); 
            this.hHippoChart1.SeriesListDictionary.Add(curVolList);

            #region 십자선
            AxisTick tk2 = new AxisTick(new System.Drawing.PointF(0, hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.Y));
            tk2.IsShowGridLine = true;
            tk2.GridLine.LineColor = System.Drawing.Color.LightGray;
            tk2.GridLine.LineWidth = 1;
            curList.AxisFactor.XAxis.ExtraTicks.Add(tk2);

            AxisTick tk1 = new AxisTick(System.Drawing.Point.Empty);
            tk1.IsShowGridLine = true;
            tk1.GridLine.LineColor = System.Drawing.Color.LightGray;
            tk1.GridLine.LineWidth = 1;
            curList.AxisFactor.YAxis.ExtraTicks.Add(tk1);

            AxisTick tk2_volume = new AxisTick(new System.Drawing.PointF(0, hHippoChart1.SeriesListDictionary[1].AxisFactor.Zero.Y));
            tk2_volume.IsShowGridLine = true;
            tk2_volume.GridLine.LineColor = System.Drawing.Color.LightGray;
            tk2_volume.GridLine.LineWidth = 1;
            curVolList.AxisFactor.XAxis.ExtraTicks.Add(tk2_volume);
            #endregion

            #region 기타 초기화
            this.txtRealData.Text = "";

            comOderType.SelectedIndex = 0;
            comBuySellType.SelectedIndex = 0;

            this.txtRealData.Text = "";

            // 현재 보여지는 개수 데이터 초기화
            ViewCount = 30;

            // 트랙바  
            this.trackBar1.Maximum = 500;
            this.trackBar1.Minimum = 10;
            this.trackBar1.LargeChange = 10;
            this.trackBar1.Value = ViewCount;

            //주가, 이평선, 보조지표 라인 체크 박스 초기화
            this.chkSR0.Checked = true;
            this.chkSR1.Checked = true;
            this.chkSR2.Checked = true;
            this.chkSR3.Checked = true;
            this.chkBB.Checked = false;
            this.checkBox3.Checked = false;
            this.chkOSC.Checked = false;
            this.chkILMOK.Checked = false;
            this.chkStochastic.Checked = false;
            this.chkRSI.Checked = false;
            #endregion
        }
        #endregion

        #region ------------ 현재가 조회 버튼, 종목 변경, 초기화  ------------ 
        /// <summary>
        /// 현재가 조회 버튼 클릭
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnNowVal_Click(object sender, EventArgs e)
        {
            Init();

            this.selectedCode = this.txtCode.Text;
            this.selectedCodeName = axKHOpenAPI1.GetMasterCodeName(selectedCode);

            this.la_codeName.Text = this.selectedCodeName;

            if (!string.IsNullOrEmpty(selectedCode) && !string.IsNullOrEmpty(this.selectedCodeName))
            {
                // 관심종목 등록  
                this.SaveMyCode(this.selectedCode, this.selectedCodeName);
            }

            axKHOpenAPI1.SetInputValue("종목코드", this.selectedCode);

            SearchNowValue(0);

            if (this.selectedChartType == "tick" && this.selectedSU == "1")
            {
                this.SearchTick(0);
            }
            else
            {
                this.SearchBong(0);
            } 
        }
        #endregion 

        #region ------------- 관심종목 선택, 설정 이벤트 ------------------------
        /// <summary>
        /// 종목을 선택했을때 호출되는 메소드 
        /// 대개 그리드의 아이템을 클릭하면 호출됩니다. 
        /// </summary>
        private void SelectGridCode()
        {
            this.Init();

            this.txtCode.Text = this.selectedCode;
            this.toolStripLabel3_code.Text = this.selectedCode;

            this.la_codeName.Text = this.selectedCodeName;
            this.la_currentVal.Text = "----";
            this.laupdawon.Text = "--";
            this.la_volume.Text = "---";
            this.TSL_status.Text = "";

            this.tabControl1.SelectedIndex = 1;

            axKHOpenAPI1.SetInputValue("종목코드", this.selectedCode);

            SearchNowValue(0);

            if (this.selectedChartType == "tick" && this.selectedSU == "1")
            {
                this.SearchTick(0);
            }
            else
            {
                this.SearchBong(0);
            }
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            try
            {
                //this.txtCode.Text = this.dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString();
            }
            catch { }
        }

        /// <summary>
        /// 관심종목 선택
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void dataGridView1_CellContentDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            this.selectedCode = this.dataGridView1.Rows[e.RowIndex].Cells[0].Value.ToString();
            this.selectedCodeName = this.dataGridView1.Rows[e.RowIndex].Cells[1].Value.ToString();

            SelectGridCode();
        }

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            currentRowIndex = e.RowIndex;
            //this.dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].Selected = true; 
        }

        /// <summary>
        /// 관심종목 바인딩
        /// </summary>
        private void SetFrivates()
        {
            try
            {
                favorsIdx = new SortedList<string, int>();

                //DataRow[] rows = this.GetMyCode().Tables[0].Select("", "dates desc");

                this.dataGridView1.DataSource = this.GetMyCode().Tables[0];
                //this.dataGridView1.Sort(this.dataGridView1.Columns[2], System.ComponentModel.ListSortDirection.Descending);

                this.dataGridView1.Columns[0].Visible = false;
                //this.dataGridView1.Columns[2].Visible = false;

                this.dataGridView1.RowHeadersVisible = false;

                this.dataGridView1.Columns[1].HeaderText = "관심종목";
                this.dataGridView1.Columns[2].HeaderText = "종목가격";

                //this.dataGridView1.Columns[1].AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCellsExceptHeader; // 컨텐츠 길이에 맞게
                this.dataGridView1.Columns[1].AutoSizeMode = DataGridViewAutoSizeColumnMode.None;
                this.dataGridView1.Columns[1].Resizable = DataGridViewTriState.False;
                this.dataGridView1.Columns[1].ReadOnly = true;

                this.dataGridView1.Columns[1].Width = 100;
                this.dataGridView1.Columns[2].Width = 140;

                //this.dataGridView1.Columns[1].DefaultCellStyle.BackColor = System.Drawing.Color.FromArgb(67, 67, 67);
                //this.dataGridView1.Columns[1].DefaultCellStyle.ForeColor = System.Drawing.Color.White;

                string codeList = "";
                int counts = 0;
                int totalCodeCount = this.GetMyCode().Tables[0].Rows.Count;

                foreach (DataRow row in this.GetMyCode().Tables[0].Rows)
                {
                    // 관심종목 엑세스 변수
                    favorsIdx.Add(row[0].ToString(), counts);

                    codeList += row[0].ToString();

                    if(counts < totalCodeCount - 1)
                    {
                        codeList += ";";
                    }
                    counts++;
                }
                  
                int nRet = axKHOpenAPI1.CommKwRqData(codeList, 0, totalCodeCount, 0, "RQ_1", "0101");

                if (Error.IsError(nRet))
                {

                }
                else
                {
                    MessageBox.Show("에러");

                }

                this.WriteLog(Error.GetErrorMessage());

                //axKHOpenAPI1.DisconnectRealData("0101"); // 실시간 해제
            }
            catch { }
        }

        /// <summary>
        /// 관심종목 가져오기
        /// </summary>
        /// <returns></returns>
        public DataSet GetMyCode()
        {
            DataSet reDS = new DataSet();
            reDS.ReadXml(System.IO.Directory.GetCurrentDirectory() + @"\mycode.xml");

            return reDS;
        }

        /// <summary>
        /// 관심종목 삭제
        /// </summary>
        /// <param name="index"></param>
        /// <param name="code"></param>
        private void DeleteFavor(int index, string code)
        {
            string path = System.IO.Directory.GetCurrentDirectory() + @"\mycode.xml";

            XmlDocument doc = new XmlDocument();
            doc.Load(path);

            XmlNodeList elements = doc.SelectNodes("//Code");

            foreach (XmlElement element in elements)
            {
                if (element.FirstChild.InnerText == code)
                { 
                    element.ParentNode.RemoveChild(element);

                    doc.Save(path); 
                    break;
                }
            }
        }

        /// <summary>
        /// 관심종목 저장하기
        /// </summary>
        private void SaveMyCode(string code, string codename)
        {
            try
            {
                DataSet ds = GetMyCode();
                bool isAlreadyReg = false;

                if (ds != null && ds.Tables.Count > 0 && ds.Tables[0].Rows.Count > 0)
                {
                    foreach (DataRow dr in ds.Tables[0].Rows)
                    {
                        if (dr["key"].ToString() == code)
                        {
                            isAlreadyReg = true;

                            // 등록 시간 업데이트
                            this.UpdateMyCode(code);
                            break;
                        }
                    }
                }

                if (!isAlreadyReg)
                {
                    string path = System.IO.Directory.GetCurrentDirectory() + @"\mycode.xml";

                    XmlDocument doc = new XmlDocument();
                    doc.Load(path);

                    XmlNode node = doc.CreateNode(XmlNodeType.Element, "Code", null);

                    XmlNode node_code = doc.CreateElement("key");
                    node_code.InnerText = code;

                    XmlNode node_name = doc.CreateElement("name");
                    node_name.InnerText = codename;

                    XmlNode node_date = doc.CreateElement("dates");
                    node_date.InnerText = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

                    node.AppendChild(node_code);
                    node.AppendChild(node_name);
                    node.AppendChild(node_date);

                    doc.DocumentElement.AppendChild(node);

                    doc.Save(path);
                }
            }
            catch
            {
            }
        }

        /// <summary>
        /// 관심종목 정렬위해 날짜 업데이트
        /// </summary>
        /// <param name="code"></param>
        private void UpdateMyCode(string code)
        {
            try
            {
                string path = System.IO.Directory.GetCurrentDirectory() + @"\mycode.xml";

                XmlDocument doc = new XmlDocument();
                doc.Load(path);

                XmlNodeList elements = doc.SelectNodes("//Code");

                foreach (XmlElement element in elements)
                {
                    if (element.FirstChild.InnerText == code)
                    {
                        XmlNode datesNode = element.ChildNodes[2];
                        datesNode.InnerText = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

                        doc.Save(path);

                        break;
                    }
                }
            }
            catch
            {
            }
        }
        #endregion

        #region  상한가 종목

        private void SetUpperCode()
        { 
            axKHOpenAPI1.SetInputValue("시장구분", "000");
            axKHOpenAPI1.SetInputValue("상하한구분", "1");
            axKHOpenAPI1.SetInputValue("정렬구분", "1");
            axKHOpenAPI1.SetInputValue("종목조건", "0");
            axKHOpenAPI1.SetInputValue("거래량구분", "0000");
            axKHOpenAPI1.SetInputValue("신용조건", "0");
            axKHOpenAPI1.SetInputValue("매매급구분", "0");

            int nRet = axKHOpenAPI1.CommRqData("상하한가", "OPT10017", 0, "0101");

            if (Error.IsError(nRet))
            {

            }
            else
            {
                MessageBox.Show("에러");

            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void dataGridView_upper_CellContentDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            this.selectedCode = this.dataGridView_upper.Rows[e.RowIndex].Cells[0].Value.ToString();
            this.selectedCodeName = this.dataGridView_upper.Rows[e.RowIndex].Cells[1].Value.ToString();

            SelectGridCode();
        }

        #endregion

        #region 일, 주, 월, 분, 틱

        /// <summary>
        /// 일봉
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TSBday_Click(object sender, EventArgs e)
        {
            AllBtnFalse();
            TSBday.Checked = true;

            this.selectedChartType = "day";  
            this.selectedTRCODE = "OPT10081";
            this.SearchBong(0);
        }

        /// <summary>
        /// 주봉
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TSBweek_Click(object sender, EventArgs e)
        {
            AllBtnFalse();
            TSBweek.Checked = true;

            this.selectedChartType = "week";  
            this.selectedTRCODE = "OPT10082";
            this.SearchBong(0);
        }

        /// <summary>
        /// 월봉
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TSBmonth_Click(object sender, EventArgs e)
        {
            AllBtnFalse();
            TSBmonth.Checked = true;

            this.selectedChartType = "mon";  
            this.selectedTRCODE = "OPT10083";
            this.SearchBong(0);
        }

        /// <summary>
        /// 분봉
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TSBmin_Click(object sender, EventArgs e)
        {
            AllBtnFalse();
            TSBmin.Checked = true;

            this.selectedChartType = "min";
            this.selectedTRCODE = "OPT10080";
            this.SearchBong(0);  
        }

        /// <summary>
        /// 틱
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TSBtick_Click(object sender, EventArgs e)
        {
            AllBtnFalse();
            TSBtick.Checked = true;

            this.selectedChartType = "tick";  
            this.selectedTRCODE = "OPT10079";

            this.SearchTick(0);
        }

        #endregion 

        #region 1, 3, 5, 10, 30, 60, 120
        private void TSL_su_1_Click(object sender, EventArgs e)
        {
            AllBtnFalse2();
            TSL_su_1.Checked = true;
            this.selectedSU = "1";

            if(this.TSBtick.Checked)
            {
                this.SearchTick(0);
            }
            else
            {
                this.SearchBong(0);
            } 
        }

        private void TSL_su_3_Click(object sender, EventArgs e)
        {
            AllBtnFalse2();
            TSL_su_3.Checked = true;
            this.selectedSU = "3";
            this.SearchBong(0);
        }

        private void TSL_su_5_Click(object sender, EventArgs e)
        {
            AllBtnFalse2();
            TSL_su_5.Checked = true;
            this.selectedSU = "5";
            this.SearchBong(0);
        }

        private void TSL_su_10_Click(object sender, EventArgs e)
        {
            AllBtnFalse2();
            TSL_su_10.Checked = true;
            this.selectedSU = "10";
            this.SearchBong(0);
        }

        private void TSL_su_30_Click(object sender, EventArgs e)
        {
            AllBtnFalse2();
            TSL_su_30.Checked = true;
            this.selectedSU = "30";
            this.SearchBong(0);
        }

        private void TSL_su_60_Click(object sender, EventArgs e)
        {
            AllBtnFalse2();
            TSL_su_60.Checked = true;
            this.selectedSU = "60";
            this.SearchBong(0);
        }

        private void TSL_su_120_Click(object sender, EventArgs e)
        {
            AllBtnFalse2();
            TSL_su_120.Checked = true;
            this.selectedSU = "120";
            this.SearchBong(0);
        }
        #endregion

        #region DrawChart - 메인 메소드
        /// <summary>
        /// 차트 그리기
        /// </summary>
        private void DrawChart()
        {
            try
            {
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Clear(); // 주가
                this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[0].items.Clear(); // 거래량  

                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[1].items.Clear(); // 이평선 5
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[2].items.Clear(); // 20
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[3].items.Clear(); // 60 
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[4].items.Clear(); // 볼린저밴드  
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[5].items.Clear(); // MACD
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[6].items.Clear(); // signal
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[7].items.Clear(); // OSC 
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[8].items.Clear(); // 전환선
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[9].items.Clear(); // 기준선
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[10].items.Clear(); // 선행스팬1
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[11].items.Clear(); // 선행스팬2
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[12].items.Clear(); // 후행스팬
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[13].items.Clear(); // 스토캐스틱
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[14].items.Clear(); // RSI

                int end = 0;
                int start = 0;
                int curIdx = 0;

                if (selectedChartType == "tick" && this.selectedSU == "1")
                {
                    #region 틱 차트
                    end = items.Count - scrollNewIndex;

                    if (end < 0)
                    {
                        end = 0;
                    }

                    start = end - ViewCount;

                    if (start < 0)
                    {
                        start = 0;
                    }

                    //this.hHippoChart1.SeriesListDictionary[0].ChartType = ChartType.Line;
                    this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].ChartType = ChartType.Line;

                    for (int i = start; i <= end; i++)
                    {
                        try
                        {
                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Add(items[i]);
                            this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[0].items.Add(items_vol[i]);
                        }
                        catch
                        {
                        }

                        curIdx++;
                    }
                    #endregion
                }
                else
                {
                    end = items_bong.Count - scrollNewIndex;

                    if (end < 0)
                    {
                        end = 0;
                    } 

                    start = end - ViewCount;

                    if (start < 0)
                    {
                        start = 0;
                    }

                    //this.hHippoChart1.SeriesListDictionary[0].ChartType = ChartType.CandleStick;
                    this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].ChartType = ChartType.CandleStick;

                    for (int i = start; i <= end + 26; i++)
                    {
                        try
                        {
                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Add(items_bong[i]);
                            this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[0].items.Add(items_bong_vol[i]); // 거래량

                            // 이평선
                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[1].items.Add(items5[i]);
                            //this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[2].items.Add(items10[i]);
                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[2].items.Add(items20[i]);
                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[3].items.Add(items60[i]);
                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[4].items.Add(itemsBollingerBands[i]);
                            //this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[5].items.Add(items120[i]);

                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[5].items.Add(itemsMACD[i]);
                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[6].items.Add(itemsSignal[i]);
                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[7].items.Add(itemsOsilator[i]);

                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[13].items.Add(itemsStochastic[i]);
                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[14].items.Add(itemsRSI[i]);


                            // ====================================================
                            // 새로운 지표는 여기 사이에 추가합니다. 일목균형표의 형태적 특이성 때문에.... 






                            // ====================================================
                             

                            if (this.chkILMOK.Checked)
                            {
                                // 일목균형표
                                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[8].items.Add(itemsILMOK1[i]);
                                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[9].items.Add(itemsILMOK2[i]);
                                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[12].items.Add(itemsILMOK5[i]);
                            }

                            //this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[1].items.Add(itemsVol5[i]);
                            //this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[2].items.Add(itemsVol20[i]);
                            //this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[3].items.Add(itemsVol60[i]);
                            //this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[4].items.Add(itemsVol120[i]);
                        }
                        catch
                        { 
                        }

                        try
                        {
                            if (this.chkILMOK.Checked)
                            {
                                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[10].items.Add(itemsILMOK3[i]);
                                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[11].items.Add(itemsILMOK4[i]);
                            }
                        }
                        catch { }

                        curIdx++;
                    } 

                    // 거래량에 일목선행 부분 추가 (길이 맞추기 위해)
                    if (this.chkILMOK.Checked)
                    {
                        for (int x = 0; x < 26; x++)
                        {
                            this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[0].items.Add(new SeriesItem(double.NaN)); // 거래량 (일목을 위한)
                        }
                    }
                }


                this.hHippoChart1.SeriesListDictionary[0].AxisFactor.XAxis.Interval = ViewCount / 5;
                this.hHippoChart1.SeriesListDictionary[1].AxisFactor.XAxis.Interval = ViewCount / 5; 

                this.hHippoChart1.DrawChart();


                try
                {
                    if (selectedChartType == "tick" && selectedSU == "1")
                    {
                        hScrollBar1.Maximum = items.Count;
                    }
                    else
                    {
                        hScrollBar1.Maximum = items_bong.Count + 26;
                    }

                    // 데이터가 추가될 수록 트랙바의 최대값이 변경되야함 

                    hScrollBar1.LargeChange = ViewCount;
                    hScrollBar1.SmallChange = 1;
                }
                catch { }
            }
            catch { }
        }

        #endregion

        #region 현재가 조회 (중요 메소드)
        /// <summary>
        /// 현재가 조회 - 중요 메소드
        /// </summary>
        private void SearchNowValue(int preNext)
        {
            axKHOpenAPI1.SetInputValue("종목코드", this.selectedCode); 

            int nRet = axKHOpenAPI1.CommRqData("주식기본정보", "OPT10001", preNext, "0101");

            //axKHOpenAPI1.SetInputValue("종목코드", this.selectedCode);
            //axKHOpenAPI1.SetInputValue("기준일자", this.dateTimePicker1.Value.ToString("yyyy-MM-dd").Trim());
            //axKHOpenAPI1.SetInputValue("수정주가구분", "1");

            //int nRet2 = axKHOpenAPI1.CommRqData("주식당일거래원", "OPT10040", 0, "0101");


            /*
             * 
             * 001:코스피, 002:코스닥, 003:선물, 004:콜옵션, 005:풋옵션, 006:스타선물
007:주식선물, 008:3년국채, 009:5년국채, 010:10년국채, 011:달러선물, 012:엔선물
013:유로선물, 014:미니금선물, 015:금선물, 016:돈육선물, 017:달러콜옵션, 018:달러풋옵션
             * 
             * */
            //int nRet2 = axKHOpenAPI1.CommInvestRqData("", "RQ_1", "0101");

            if (Error.IsError(nRet))
            {

            }
            else
            {
                MessageBox.Show("에러");

            }

            this.WriteLog("["+ this.selectedTRCODE + "] : " + Error.GetErrorMessage());
        }
        #endregion

        #region 틱차트 조회 -  (중요 메소드)
        private void SearchTick(int preNext)
        {
            if (!string.IsNullOrEmpty(selectedCode))
            {
                axKHOpenAPI1.SetInputValue("종목코드", this.selectedCode);
                axKHOpenAPI1.SetInputValue("기준일자", this.dateTimePicker1.Value.ToString("yyyy-MM-dd").Trim());
                axKHOpenAPI1.SetInputValue("수정주가구분", "1"); 
                axKHOpenAPI1.SetInputValue("틱범위", "1");  

                int nRet = axKHOpenAPI1.CommRqData("틱차트조회", "OPT10079", preNext, "0101");

                if (Error.IsError(nRet))
                {

                }
                else
                {
                    this.WriteLog("[" + this.selectedTRCODE + "] : " + Error.GetErrorMessage());
                }
            }
        }
        #endregion

        #region 봉 차트 조회 공통 메소드 (중요 메소드)
        /// <summary>
        /// 봉 차트 조회 공통 메소드 (중요 메소드)
        /// </summary>
        /// <param name="trCode"></param>
        private void SearchBong(int preNext)
        {
            //Init();

            if(!string.IsNullOrEmpty(selectedCode))
            {
                axKHOpenAPI1.SetInputValue("종목코드", this.selectedCode);
                axKHOpenAPI1.SetInputValue("기준일자", this.dateTimePicker1.Value.ToString("yyyy-MM-dd").Trim());
                axKHOpenAPI1.SetInputValue("수정주가구분", "1");

                // 틱이거나 분봉일경우 만 범위가 필요하다.(1, 3, 5, 10 ... 버튼들)
                if (selectedChartType == "tick" || this.selectedChartType == "min")
                {
                    axKHOpenAPI1.SetInputValue("틱범위", this.selectedSU);
                }

                int nRet = axKHOpenAPI1.CommRqData("주식일봉차트조회", this.selectedTRCODE, preNext, "0101");
                 
                if (Error.IsError(nRet))
                {

                }
                else
                {
                    this.WriteLog("[" + this.selectedTRCODE + "] : " + Error.GetErrorMessage());
                } 
            } 
        }
        #endregion

        #region axKHOpenAPI1_OnReceiveTrData (핵심 이벤트 1)
        /// <summary>
        /// axKHOpenAPI1_OnReceiveTrData
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void axKHOpenAPI1_OnReceiveTrData(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveTrDataEvent e)
        {
            if (e.sRQName == "주식주문")
            {
            }
            else if (e.sRQName == "주식기본정보")
            {
                #region
                int nCnt = axKHOpenAPI1.GetRepeatCnt(e.sTrCode, e.sRQName);

                this.WriteLog("현재가 조회 성공");

                for (int i = 0; i < nCnt; i++)
                {
                    string codeName = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "종목명").Trim();
                    int curVal = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "현재가").Trim());
                    string upNdown = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "등락율").Trim();
                    int volumes = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "거래량").Trim());

                    selectedPrice = Math.Abs(curVal);

                    this.la_currentVal.Text = (Math.Abs(curVal)).ToString("n0");
                    //this.la_codeName.Text = codeName;
                    this.laupdawon.Text = upNdown + "%";
                    this.toolStripLabel3_code.Text = "(" + this.selectedCode + ")";

                    if (upNdown.IndexOf('+') >= 0)
                    {
                        laupdawon.ForeColor = System.Drawing.Color.Red;
                        this.la_currentVal.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (upNdown.IndexOf('-') >= 0)
                    {
                        laupdawon.ForeColor = System.Drawing.Color.Blue;
                        this.la_currentVal.ForeColor = System.Drawing.Color.Blue;
                    }
                    else
                    {
                        laupdawon.ForeColor = System.Drawing.Color.Black;
                        this.la_currentVal.ForeColor = System.Drawing.Color.Black;
                    }

                    this.la_volume.Text = volumes.ToString("n0");


                    // 상장일 가져오기 GetMasterListedStockDate


                    // 종목 상태 가져오기 GetMasterStockState 
                    this.TSL_status.Text =
                        "㉠상장일 : " + axKHOpenAPI1.GetMasterListedStockDate(this.selectedCode) +
                        ", ㉡종목상태 : " + axKHOpenAPI1.GetMasterStockState(this.selectedCode) +
                        ", ㉢상장주식수 : " + axKHOpenAPI1.GetMasterListedStockCnt(this.selectedCode).ToString("n0");
                }
                #endregion
            }
            else if (e.sRQName == "RQ_1") // RQ_1 (복수종목 조회)
            {
                #region
                int nCnt = axKHOpenAPI1.GetRepeatCnt(e.sTrCode, e.sRQName);

                for (int i = 0; i < nCnt; i++)
                {
                    string dates = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "일자").Trim();
                    int curVal = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "현재가").Trim());
                    string upNdown = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "등락율").Trim();

                    string fllag = "";

                    if (curVal > 0)
                    {
                        this.dataGridView1.Rows[i].Cells[2].Style.ForeColor = Color.Red;
                        fllag = "▲";
                    }
                    else if (curVal < 0)
                    {
                        this.dataGridView1.Rows[i].Cells[2].Style.ForeColor = Color.Blue;
                        fllag = "▼";
                    }
                    else
                    {
                        this.dataGridView1.Rows[i].Cells[2].Style.ForeColor = Color.Black;
                    }

                    this.dataGridView1.Rows[i].Cells[2].Value = fllag + " " + Math.Abs(curVal).ToString("n0") + " " + "(" + upNdown + "%)";
                }
                #endregion
            }
            else if (e.sRQName == "틱차트조회")
            {
                #region 
                this.items.Clear();
                this.items_vol.Clear();
                this.items5.Clear();
                this.items20.Clear();
                this.items60.Clear();
                this.itemsBollingerBands.Clear();
                this.items12.Clear();
                this.items26.Clear();
                this.itemsMACD.Clear();
                this.itemsSignal.Clear();
                this.itemsOsilator.Clear();
                this.itemsILMOK1.Clear();
                this.itemsILMOK2.Clear();
                this.itemsILMOK3.Clear();
                this.itemsILMOK4.Clear();
                this.itemsILMOK5.Clear();
                this.itemsStochastic.Clear();
                this.itemsRSI.Clear();

                int nCnt = axKHOpenAPI1.GetRepeatCnt(e.sTrCode, e.sRQName);

                this.WriteLog("틱차트 조회 성공");

                for (int i = nCnt - 1; i >= 0; i--)
                {
                    SeriesItem item = new SeriesItem();
                    item.Name = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "일자").Trim();
                    item.YValue = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "현재가").Trim());

                    SeriesItem item_vol = new SeriesItem();
                    item_vol.Name = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "일자").Trim();
                    item_vol.YValue = Math.Abs(Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "거래량").Trim()));

                    items.Add(item);
                    items_vol.Add(item_vol);
                }

                // 일, 월 등 봉에 따라 틱 합산


                this.DrawChart();
                #endregion
            }
            else if (e.sRQName == "주식일봉차트조회")
            {
                #region 

                #region 컬렉션 초기화
                this.items_bong.Clear();
                this.items_bong_vol.Clear();
                this.items5.Clear();
                this.items20.Clear();
                this.items60.Clear();
                this.itemsBollingerBands.Clear();
                this.items12.Clear();
                this.items26.Clear();
                this.itemsMACD.Clear();
                this.itemsSignal.Clear();
                this.itemsOsilator.Clear();
                this.itemsILMOK1.Clear();
                this.itemsILMOK2.Clear();
                this.itemsILMOK3.Clear();
                this.itemsILMOK4.Clear();
                this.itemsILMOK5.Clear();
                this.itemsStochastic.Clear();
                this.itemsRSI.Clear();
                #endregion

                int nCnt = axKHOpenAPI1.GetRepeatCnt(e.sTrCode, e.sRQName);

                // 실시간 봉 차트
                //item_real_bong = new SeriesItem();
                //item_realVol_bong = new SeriesItem();

                //string preNext = e.sPrevNext;

                this.WriteLog("일봉 데이터 조회 성공");

                #region 봉 데이터 저장
                for (int i = nCnt - 1; i >= 0; i--)
                {
                    SeriesItem item = new SeriesItem();
                    SeriesItem item_vol = new SeriesItem();

                    try
                    {
                        string dates = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "일자").Trim();

                        item.Name = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "일자").Trim();
                        item.YValue = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "현재가").Trim());
                        item.YStartValue = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "시가").Trim());
                        item.HighValue = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "고가").Trim());
                        item.LowValue = Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "저가").Trim());

                        item_vol.Name = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "일자").Trim();
                        item_vol.YValue = Math.Abs(Int32.Parse(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "거래량").Trim()));

                        item.Balloon = new Balloon();
                        item.Balloon.Visible = false;
                        item.Balloon.IsShadow = false;
                        item.Balloon.Label.Font = new System.Drawing.Font("굴림", 8, System.Drawing.FontStyle.Regular);
                        item.Balloon.TextLocation = System.Drawing.StringAlignment.Near;
                        item.Balloon.BackColor = System.Drawing.Color.FromArgb(232, 240, 249);
                        item.Balloon.BalloonType = BalloonType.Rectangle;
                        item.Balloon.Label.Text = "";
                        item.Balloon.Label.Text += "일자  : " + item.Name + "\r\n";
                        item.Balloon.Label.Text += this.selectedCodeName + " \r\n";
                        item.Balloon.Label.Text += "시가  : " + item.YStartValue.ToString("n0") + " \r\n";
                        item.Balloon.Label.Text += "고가  : " + item.HighValue.ToString("n0") + " \r\n";
                        item.Balloon.Label.Text += "저가  : " + item.LowValue.ToString("n0") + " \r\n";
                        item.Balloon.Label.Text += "종가  : " + item.YValue.ToString("n0") + " \r\n";
                        item.Balloon.Label.Text += "거래량 : " + item_vol.YValue.ToString("n0") + " \r\n";

                        item_vol.Balloon = new Balloon();
                        item_vol.Balloon.Visible = false;
                        item_vol.Balloon.IsShadow = false;
                        item_vol.Balloon.Label.Font = new System.Drawing.Font("굴림", 8, System.Drawing.FontStyle.Regular);
                        item_vol.Balloon.TextLocation = System.Drawing.StringAlignment.Near;
                        item_vol.Balloon.BackColor = System.Drawing.Color.FromArgb(232, 240, 249);
                        item_vol.Balloon.BalloonType = BalloonType.Rectangle;
                        item_vol.Balloon.Label.Text = "";
                        item_vol.Balloon.Label.Text += "일자  : " + item.Name + "\r\n";
                        item_vol.Balloon.Label.Text += this.selectedCodeName + " \r\n";
                        item_vol.Balloon.Label.Text += "거래량 : " + item_vol.YValue.ToString("n0") + " \r\n";

                        items_bong.Add(item);
                        items_bong_vol.Add(item_vol);
                    }
                    catch
                    {
                        items_bong.Add(item);
                        items_bong_vol.Add(item_vol);
                    }
                }

                //test
                // 시작점 찾기 
                int max2 = int.MinValue;
                string dates2 = "";
                int stockval = 0;

                int days = 0;
                for (int i = nCnt - 1; i >= 0; i--)
                {
                    if (max2 <= items_bong_vol[i].YValue)
                    {
                        max2 = (int)items_bong_vol[i].YValue;
                        dates2 = items_bong_vol[i].Name;

                        stockval = (int)items_bong[i].YStartValue;
                    }

                    days++;
                    if (days >= 80) break;
                }

                toolStripLabel4.Text = "날짜 : "+ dates2.ToString() + " 주가" + stockval.ToString() + ", 거래량 : " + max2.ToString();

                AxisTick tk_Start = new AxisTick(stockval);
                tk_Start.IsShowGridLine = true;
                tk_Start.GridLine.LineWidth = 3;
                tk_Start.GridLine.LineColor = Color.Red;

                this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks.Add(tk_Start);

                //items_bong.Add(item_real_bong);
                //items_bong_vol.Add(item_realVol_bong);

                #endregion

                #region 이평선 및 각종 지표
                double preClose12 = 0;
                double preClose26 = 0;

                // 후행스팬 배치하기 위한 카운드 26 (앞으로 26칸 땡기기)
                int ilmokPreCount26 = 0;

                // 선행스팬 2 -  26일 후에 배치
                for (int i = 0; i < 26; i++)
                {
                    SeriesItem itemilmok = new SeriesItem();
                    itemilmok.YValue = double.NaN;

                    itemsILMOK4.Add(i, itemilmok);
                }

                for (int i = 0; i < nCnt; i++)
                {
                    #region 5일선
                    SeriesItem item5 = new SeriesItem();
                    try
                    {
                        for (int a = 0; a < 5; a++)
                        {
                            item5.YValue += items_bong[i - a].YValue;
                        }
                        item5.YValue = item5.YValue / 5;
                    }
                    catch
                    {
                        item5.YValue = double.NaN;
                    }
                    items5.Add(i, item5);
                    #endregion

                    #region 10일선
                    //SeriesItem item10 = new SeriesItem();
                    //try
                    //{ 
                    //    for (int a = 0; a < 10; a++)
                    //    {
                    //        item10.YValue += items_bong[i - a].YValue;
                    //    }
                    //    item10.YValue = item10.YValue / 10;
                    //}
                    //catch { item10.YValue = double.NaN; }
                    //items10.Add(i, item10);
                    #endregion

                    #region 20일선과 볼린저밴드
                    // 20일 이동평균선 계산
                    SeriesItem item20 = new SeriesItem();
                    // 볼린저밴드 상한선 
                    SeriesItem itemBB = new SeriesItem();
                    try
                    {
                        double sum = 0;

                        for (int a = 0; a < 20; a++)
                        {
                            sum += items_bong[i - a].YValue;
                        }

                        double avg = sum / 20;
                        double SD_sum = 0;
                        // 표준편차
                        double SD = 0;
                        // 분산
                        double Variance = 0;

                        for (int a = 0; a < 20; a++)
                        {
                            SD_sum += Math.Pow(items_bong[i - a].YValue - avg, 2);
                        }

                        Variance = SD_sum / 20;
                        SD = Math.Sqrt(Variance);

                        item20.YValue = sum / 20;
                        itemBB.YValue = item20.YValue + SD * 2;
                        itemBB.YStartValue = item20.YValue - SD * 2;
                    }
                    catch
                    {
                        item20.YValue = double.NaN;
                        itemBB.YValue = double.NaN;
                    }

                    items20.Add(i, item20);
                    itemsBollingerBands.Add(i, itemBB);

                    #endregion

                    #region 12일, 26일 지수 이동평균

                    double EP12 = 0.15384615;
                    double EP26 = 0.07407407;

                    SeriesItem item12 = new SeriesItem();
                    try
                    {
                        double close = items_bong[i].YValue;
                        double exMV = close * EP12 + preClose12 * (1 - EP12);

                        item12.YValue = exMV;

                        preClose12 = exMV;
                    }
                    catch
                    {
                        item12.YValue = double.NaN;
                    }
                    items12.Add(i, item12);

                    SeriesItem item26 = new SeriesItem();
                    try
                    {
                        double close = items_bong[i].YValue;
                        double exMV = close * EP26 + preClose26 * (1 - EP26);

                        item26.YValue = exMV;

                        preClose26 = exMV;
                    }
                    catch
                    {
                        item26.YValue = double.NaN;
                    }
                    items26.Add(i, item26);

                    #endregion

                    #region 60일 이동평균선 계산
                    SeriesItem item60 = new SeriesItem();
                    try
                    {
                        for (int a = 0; a < 60; a++)
                        {
                            item60.YValue += items_bong[i - a].YValue;
                        }
                        item60.YValue = item60.YValue / 60;
                    }
                    catch { item60.YValue = double.NaN; }
                    items60.Add(i, item60);
                    #endregion

                    #region 일목균형표 - 전환선, 기준선
                    // 전환선, 기준선
                    #region
                    SeriesItem itemilmok1 = new SeriesItem();
                    try
                    {
                        double max = double.MinValue;
                        double min = double.MaxValue;

                        for (int a = 0; a < 9; a++)
                        {
                            if (max <= items_bong[i - a].HighValue)
                            {
                                max = items_bong[i - a].HighValue;
                            }
                            if (min >= items_bong[i - a].LowValue)
                            {
                                min = items_bong[i - a].LowValue;
                            }
                        }

                        itemilmok1.YValue = (max + min) / 2;
                    }
                    catch { itemilmok1.YValue = double.NaN; }
                    itemsILMOK1.Add(i, itemilmok1);

                    // 기준선
                    SeriesItem itemilmok2 = new SeriesItem();
                    try
                    {
                        double max = double.MinValue;
                        double min = double.MaxValue;

                        for (int a = 0; a < 26; a++)
                        {
                            if (max <= items_bong[i - a].HighValue)
                            {
                                max = items_bong[i - a].HighValue;
                            }
                            if (min >= items_bong[i - a].LowValue)
                            {
                                min = items_bong[i - a].LowValue;
                            }
                        }

                        itemilmok2.YValue = (max + min) / 2;
                    }
                    catch { itemilmok2.YValue = double.NaN; }
                    itemsILMOK2.Add(i, itemilmok2);
                    #endregion

                    // 선행스팬2를 위한 값
                    SeriesItem itemilmok4 = new SeriesItem();
                    try
                    {
                        double max = double.MinValue;
                        double min = double.MaxValue;

                        for (int a = 0; a < 52; a++)
                        {
                            if (max <= items_bong[i - a].HighValue)
                            {
                                max = items_bong[i - a].HighValue;
                            }
                            if (min >= items_bong[i - a].LowValue)
                            {
                                min = items_bong[i - a].LowValue;
                            }
                        }

                        itemilmok4.YValue = (max + min) / 2;
                    }
                    catch { itemilmok4.YValue = double.NaN; }
                    itemsILMOK4.Add(i + 26, itemilmok4);

                    // 후행스팬
                    SeriesItem itemilmok5 = new SeriesItem();
                    try
                    {
                        itemilmok5.YValue = items_bong[i + 26].YValue;
                    }
                    catch { itemilmok5.YValue = double.NaN; }

                    if (ilmokPreCount26 > 25)
                    {
                        itemsILMOK5.Add(i, itemilmok5);
                    }

                    ilmokPreCount26++;

                    #endregion

                    #region Stochastic

                    SeriesItem itemStochastic = new SeriesItem();
                    try
                    {
                        double max = double.MinValue;
                        double min = double.MaxValue;

                        for (int a = 0; a < 15; a++)
                        {
                            if (max <= items_bong[i - a].HighValue)
                            {
                                max = items_bong[i - a].HighValue;
                            }
                            if (min >= items_bong[i - a].LowValue)
                            {
                                min = items_bong[i - a].LowValue;
                            }
                        }

                        itemStochastic.YValue = ((items_bong[i].YValue - min) / (max - min)) * 100;
                    }
                    catch { itemStochastic.YValue = double.NaN; }
                    itemsStochastic.Add(i, itemStochastic);

                    #endregion

                    #region RSI

                    SeriesItem itemRSI = new SeriesItem();
                    try
                    {
                        int N = 10; // 보통 14로 한다고 하는데 네이버에 데이터가 10일이라 일단 맞춤. 수정 하시면 됩니다
                        double U_sum = 0;
                        double D_sum = 0;
                        double AU = 0; // 평균
                        double AD = 0; // 평균

                        for (int a = 0; a < N; a++)
                        {
                            double U = 0;
                            double D = 0;
                            double temp = items_bong[i - a].YValue - items_bong[i - a - 1].YValue;

                            if (temp > 0)
                            {
                                U = temp;
                                U_sum += U;
                            }
                            else if (temp < 0)
                            {
                                D = Math.Abs(temp);
                                D_sum += D;
                            }
                        }

                        AU = U_sum / N;
                        AD = D_sum / N;

                        itemRSI.YValue = (AU / (AU + AD)) * 100;
                    }
                    catch { itemRSI.YValue = double.NaN; }
                    itemsRSI.Add(i, itemRSI);

                    #endregion
                }
                #endregion

                #region MACD, signal, ocsilator
                // MACD (데이터 검증 성공)
                for (int i = 0; i < items26.Count; i++)
                {
                    SeriesItem itemMACD = new SeriesItem();
                    try
                    {
                        if (double.IsNaN(items12[i].YValue) || double.IsNaN(items26[i].YValue))
                        {
                            itemMACD.YValue = double.NaN;
                        }
                        else
                        {
                            itemMACD.YValue = items12[i].YValue - items26[i].YValue;
                        }
                    }
                    catch
                    {
                        itemMACD.YValue = double.NaN;
                    }

                    itemsMACD.Add(i, itemMACD);
                }

                // signal  (데이터 검증 성공)
                double preClose9 = 0;
                double EP9 = 0.2;

                // 9일간 지수이동평균
                for (int i = 0; i < items26.Count; i++)
                {
                    SeriesItem item9 = new SeriesItem();
                    try
                    {
                        double close = itemsMACD[i].YValue;
                        double exMV = close * EP9 + preClose9 * (1 - EP9);

                        item9.YValue = exMV;

                        preClose9 = exMV;
                    }
                    catch
                    {
                        item9.YValue = double.NaN;
                    }
                    itemsSignal.Add(i, item9);
                }

                // oscilator (데이터 검증 성공)
                for (int i = 0; i < items26.Count; i++)
                {
                    SeriesItem itemOSC = new SeriesItem();
                    try
                    {
                        itemOSC.YValue = itemsMACD[i].YValue - itemsSignal[i].YValue;

                        if (itemOSC.YValue > 0)
                        {
                            itemOSC.ItemColor = Color.Red;
                        }
                        else
                        {
                            itemOSC.ItemColor = Color.SkyBlue;
                        }
                    }
                    catch { itemOSC.YValue = double.NaN; }

                    itemsOsilator.Add(i, itemOSC);
                }

                #endregion

                #region 일목균형표 - 선행1, 선행2, 후행

                // 선행스팬1 -  26일 선행하여 배치 (미래에 배치)
                for (int i = 0; i < 26; i++)
                {
                    SeriesItem itemilmok = new SeriesItem();
                    itemilmok.YValue = double.NaN;
                    itemilmok.YStartValue = double.NaN;
                    itemsILMOK3.Add(i, itemilmok);
                }

                // 선행스팬1
                for (int i = 0; i < itemsILMOK1.Count; i++)
                {
                    SeriesItem itemilmok3 = new SeriesItem();
                    try
                    {
                        itemilmok3.YValue = (itemsILMOK1[i].YValue + itemsILMOK2[i].YValue) / 2;
                        itemilmok3.YStartValue = itemsILMOK4[i + 26].YValue;

                        //itemsILMOK4[i + 26].YStartValue = itemilmok3.YValue;
                    }
                    catch
                    {
                        itemilmok3.YValue = double.NaN;
                        itemilmok3.YStartValue = double.NaN;
                    }

                    itemsILMOK3.Add(i + 26, itemilmok3);
                }

                // 후행스팬 - 26일 (과거에 배치)
                for (int i = 0; i < 26; i++)
                {
                    SeriesItem itemilmok = new SeriesItem();
                    itemilmok.YValue = double.NaN;
                    itemsILMOK5.Add(i, itemilmok);
                }

                #endregion

                this.DrawChart();

                #endregion
            }
            else if (e.sRQName == "주식당일거래원")
            {
                #region
                //this.pan_medo.Controls.Clear();
                //this.pan_mesu.Controls.Clear();
                //this.pan_forin.Controls.Clear();

                //int nCnt = axKHOpenAPI1.GetRepeatCnt(e.sTrCode, e.sRQName); //??

                //for (int i = 1; i <= 5; i++)
                //{
                //    string gubun = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "거래소구분").Trim();

                //    string gure1 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매도거래원" + i.ToString()).Trim();
                //    string gure2 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매도거래원수량" + i.ToString()).Trim();
                //    string gure3 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매도거래원별증감" + i.ToString()).Trim();
                //    string gure4 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매도거래원코드" + i.ToString()).Trim();
                //    string gure5 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매도거래원색깔" + i.ToString()).Trim();

                //    System.Windows.Forms.Label la = new System.Windows.Forms.Label();
                //    la.Width = this.pan_medo.Width - 20;
                //    la.Text = gure1 + ", " + gure2 + ", " + gure3 + ", " + gure4 + ", " + gure5;
                //    la.Location = new Point(0, 25 * (i - 1) + 5);

                //    if (int.Parse(gure2) > 0)
                //    {
                //        la.ForeColor = Color.Red;
                //    }
                //    else
                //    {
                //        la.ForeColor = Color.Blue;
                //    }

                //    this.pan_medo.Controls.Add(la);

                //    string gure_mesu1 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매수거래원" + i.ToString()).Trim();
                //    string gure_mesu2 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매수거래원수량" + i.ToString()).Trim();
                //    string gure_mesu3 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매수거래원별증감" + i.ToString()).Trim();
                //    string gure_mesu4 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매수거래원코드" + i.ToString()).Trim();
                //    string gure_mesu5 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "매수거래원색깔" + i.ToString()).Trim();

                //    System.Windows.Forms.Label la2 = new System.Windows.Forms.Label();
                //    la2.Width = this.pan_mesu.Width - 20;
                //    la2.Text = gure_mesu1 + ", " + gure_mesu2 + ", " + gure_mesu3 + ", " + gure_mesu4 + ", " + gure_mesu5;
                //    la2.Location = new Point(0, 25 * (i - 1) + 5);

                //    if (int.Parse(gure_mesu2) > 0)
                //    {
                //        la2.ForeColor = Color.Red;
                //    }
                //    else
                //    {
                //        la2.ForeColor = Color.Blue;
                //    }

                //    this.pan_mesu.Controls.Add(la2);

                //    string forin1 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계매도추정합").Trim();
                //    string forin2 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계매도추정합변동").Trim();
                //    string forin3 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계매수추정합").Trim();
                //    string forin4 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계매수추정합변동").Trim();
                //    string forin5 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계순매수추정합").Trim();
                //    string forin6 = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, 0, "외국계순매수변동").Trim();

                //    System.Windows.Forms.Label la3 = new System.Windows.Forms.Label();
                //    la3.Text = forin1 + ", " + forin2 + ", " + forin3 + ", " + forin4 + ", " + forin5 + ", " + forin6;
                //    la3.Location = new Point(0, 20);

                //    if (int.Parse(forin1) > 0)
                //    {
                //        la3.ForeColor = Color.Red;
                //    }
                //    else
                //    {
                //        la3.ForeColor = Color.Blue;
                //    }

                //    this.pan_forin.Controls.Add(la3);
                //}
                #endregion
            }
            else if (e.sRQName == "상하한가")
            {
                #region
                int nCnt = axKHOpenAPI1.GetRepeatCnt(e.sTrCode, e.sRQName);

                DataTable dt = new DataTable();
                dt.Columns.Add("code");
                dt.Columns.Add("종목이름");
                dt.Columns.Add("상한가");
                dt.AcceptChanges();

                for (int i = 0; i < nCnt; i++)
                {
                    DataRow dr = dt.NewRow();

                    dr["code"] = axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "종목코드").Trim();
                    dr["종목이름"] = axKHOpenAPI1.GetMasterCodeName(axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "종목코드").Trim());
                    dr["상한가"] = "† " + axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "현재가").Trim() + "("+ axKHOpenAPI1.CommGetData(e.sTrCode, "", e.sRQName, i, "등락률").Trim() + "%)";

                    dt.Rows.Add(dr);
                }

                this.dataGridView_upper.DataSource = dt;

                this.dataGridView_upper.Columns[0].Visible = false; 

                this.dataGridView_upper.RowHeadersVisible = false;

                this.dataGridView_upper.Columns[1].HeaderText = "종목이름";
                this.dataGridView_upper.Columns[2].HeaderText = "상한가";

                //this.dataGridView1.Columns[1].AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCellsExceptHeader; // 컨텐츠 길이에 맞게
                this.dataGridView_upper.Columns[1].AutoSizeMode = DataGridViewAutoSizeColumnMode.None;
                this.dataGridView_upper.Columns[1].Resizable = DataGridViewTriState.False;
                this.dataGridView_upper.Columns[1].ReadOnly = true;

                this.dataGridView_upper.Columns[1].Width = 100;
                this.dataGridView_upper.Columns[2].Width = 130;

                for (int i = 0; i < nCnt; i++)
                {
                    this.dataGridView_upper.Rows[i].Cells[2].Style.ForeColor = Color.Red;
                }

                #endregion
            }
        }

        #endregion

        #region axKHOpenAPI1_OnReceiveRealData (핵심 이벤트 2 실시간)
        /// <summary>
        /// 실시간 데이터 - 이 이벤트를 통해 실시간 데이터를 통신합니다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void axKHOpenAPI1_OnReceiveRealData(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveRealDataEvent e)
        {
            if (e.sRealType == "주식체결")
            {
                #region 실시간 그리드 
                int curVal = Int32.Parse(axKHOpenAPI1.GetCommRealData(e.sRealType, 10).Trim()); // 현재가
                string upNdown = axKHOpenAPI1.GetCommRealData(e.sRealType, 12).Trim(); // 등락률
                int volumes = Int32.Parse(axKHOpenAPI1.GetCommRealData(e.sRealType, 13).Trim()); // 누적 거래량

                string fllag = "";

                try
                {
                    if (curVal > 0)
                    {
                        this.dataGridView1.Rows[this.favorsIdx[e.sRealKey]].Cells[2].Style.ForeColor = Color.Red;
                        fllag = "▲";
                    }
                    else if (curVal < 0)
                    {
                        this.dataGridView1.Rows[this.favorsIdx[e.sRealKey]].Cells[2].Style.ForeColor = Color.Blue;
                        fllag = "▼";
                    }
                    else
                    {
                        this.dataGridView1.Rows[this.favorsIdx[e.sRealKey]].Cells[2].Style.ForeColor = Color.Black;
                    }

                    this.dataGridView1.Rows[this.favorsIdx[e.sRealKey]].Cells[2].Value = fllag + " " + Math.Abs(curVal).ToString("n0") + " " + "(" + upNdown + "%)";
                }
                catch { }
                #endregion

                #region 실시간 차트
                if (!string.IsNullOrEmpty(selectedCode) && this.selectedCode == e.sRealKey)
                {
                    this.WriteLog("실시간 데이터 조회 및 차트 시작");

                    //int curVal = Int32.Parse(axKHOpenAPI1.GetCommRealData(e.sRealType, 10).Trim()); // 현재가
                    //string upNdown = axKHOpenAPI1.GetCommRealData(e.sRealType, 12).Trim(); // 등락률
                    //int volumes = Int32.Parse(axKHOpenAPI1.GetCommRealData(e.sRealType, 13).Trim()); // 누적 거래량
                     
                    selectedPrice = Math.Abs(curVal);

                    axKHOpenAPI1.SetInputValue("종목코드", this.selectedCode);

                    int nowvolumes = Int32.Parse(axKHOpenAPI1.GetCommRealData(e.sRealType, 15).Trim()); // 체결 거래량

                    string curDate = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

                    //string text_data = string.Format("{0}({1})_{2}_{3}_{4}_{5}",
                    //    this.selectedCodeName,
                    //    this.selectedCode,
                    //    curVal.ToString("n0"),
                    //    upNdown,
                    //    nowvolumes.ToString("n0"),
                    //    curDate
                    //    );

                    string text_data = string.Format("{0}_{1}_{2}",
                      this.selectedCodeName,
                      curVal.ToString("n0"),
                      upNdown
                      );

                    // 실시간 데이터 저장이 시작되면
                    if (isRealDataSaveStart)
                    {
                        RealDataSet += text_data + "\r\n";
                    }

                    // 현재가 계속 뷰
                    this.txtRealData.Text = text_data + "\r\n" + this.txtRealData.Text;

                    this.la_currentVal.Text = curVal.ToString("n0");
                    this.laupdawon.Text = upNdown + "%";

                    this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[0].Value = curVal;
                    this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[0].Visible = true;
                    this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[0].Label.Text = curVal.ToString("n0") + "\r\n" + upNdown + "%";

                    // 색상 조절
                    if (upNdown.IndexOf('+') > -1)
                    {
                        laupdawon.ForeColor = System.Drawing.Color.Red;
                        this.la_currentVal.ForeColor = System.Drawing.Color.Red;
                        this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[0].BackColor = Color.Red;
                    }
                    else if (upNdown.IndexOf('-') > -1)
                    {
                        laupdawon.ForeColor = System.Drawing.Color.Blue;
                        this.la_currentVal.ForeColor = System.Drawing.Color.Blue;
                        this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[0].BackColor = Color.Blue;
                    }
                    else
                    {
                        laupdawon.ForeColor = System.Drawing.Color.Black;
                        this.la_currentVal.ForeColor = System.Drawing.Color.Black;
                        this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[0].BackColor = Color.Black;
                    }

                    this.la_volume.Text = volumes.ToString("n0");

                    // 리얼타임 차트 
                    SeriesItem item = new SeriesItem();
                    item.YValue = Math.Abs(curVal);
                    item.Name = curDate;

                    SeriesItem itemvol = new SeriesItem();
                    itemvol.YValue = Math.Abs(nowvolumes);
                    itemvol.Name = curDate;

                    items.Add(item);
                    items_vol.Add(itemvol);

                    // 실시간 봉 구성
                    //this.MakeCaluateTickItem(item, itemvol);

                    // ============================================================================ 
                    // ============================================================================ 


                    //if(item.YValue > 5000)   // 예를 들어 실시간으로 들어오는 현재 체결가가 5000을 넘으면? 이라는 코드
                    //{

                    //}

                    //if (itemvol.YValue > 5000)  // 예를 들어 실시간으로 들어오는 체결량이 5000을 넘으면? 이라는 코드
                    //{

                    //}


                    // ============================================================================
                    // ============================================================================

                    this.DrawChart();
                }

                //if(this.tabControl1.SelectedIndex == 0)
                //{
                //    this.SetFrivates();
                //}

                #endregion

                // 실시간 봉 구성


                if (this.txtRealData.TextLength > 1000)
                {
                    this.txtRealData.Clear();
                }
            }
            else if (e.sRealType == "잔고")
            {

            }
            else if (e.sRealType == "주식당일거래원")
            {
                #region 주식당일거래원 

                //if(this.pan_medo.Controls.Count > 3)
                //{
                //    this.pan_medo.Controls.Clear();
                //}
                //if (this.pan_mesu.Controls.Count > 3)
                //{
                //    this.pan_mesu.Controls.Clear();
                //} 

                this.pan_medo.Controls.Clear();
                this.pan_mesu.Controls.Clear();

                //string guregubun = axKHOpenAPI1.GetCommRealData(e.sRealType, 337).Trim(); // 거래소 구분

                for (int i = 0; i < 5; i++)
                {
                    string gure1 = axKHOpenAPI1.GetCommRealData(e.sRealType, 141 + i).Trim(); // 매도거래원1
                    string gure2 = axKHOpenAPI1.GetCommRealData(e.sRealType, 161 + i).Trim(); // 매도거래원 수량1
                    string gure3 = axKHOpenAPI1.GetCommRealData(e.sRealType, 166 + i).Trim(); // 매도거래원별 증감1
                    string gure4 = axKHOpenAPI1.GetCommRealData(e.sRealType, 146 + i).Trim(); // 매도거래원 코드1
                    string gure5 = axKHOpenAPI1.GetCommRealData(e.sRealType, 271 + i).Trim(); // 매도거래원 색깔1 

                    string gure_mesu1 = axKHOpenAPI1.GetCommRealData(e.sRealType, 151 + i).Trim(); // 매수거래원1
                    string gure_mesu2 = axKHOpenAPI1.GetCommRealData(e.sRealType, 171 + i).Trim(); // 매수거래원 수량1
                    string gure_mesu3 = axKHOpenAPI1.GetCommRealData(e.sRealType, 176 + i).Trim(); // 매수거래원별 증감1
                    string gure_mesu4 = axKHOpenAPI1.GetCommRealData(e.sRealType, 156 + i).Trim(); // 매수거래원 코드1
                    string gure_mesu5 = axKHOpenAPI1.GetCommRealData(e.sRealType, 281 + i).Trim(); // 매수거래원 색깔1 

                    System.Windows.Forms.Label la = new System.Windows.Forms.Label();
                    la.Width = this.pan_medo.Width - 15;
                    la.Font = new Font("tahoma", 8, FontStyle.Regular);
                    la.Text = gure1 + ", " + gure2 + ", " + gure3 + ", " + gure4 + ", " + gure5;
                    la.Location = new Point(0, 23 * this.pan_medo.Controls.Count);

                    //if (int.Parse(gure2) > 0)
                    //{
                    //    la.ForeColor = Color.Red;
                    //}
                    //else
                    //{
                    la.ForeColor = Color.Blue;
                    //}

                    this.pan_medo.Controls.Add(la);

                    System.Windows.Forms.Label la2 = new System.Windows.Forms.Label();
                    la2.Width = this.pan_mesu.Width - 15;
                    la2.Font = new Font("tahoma", 8, FontStyle.Regular);
                    la2.Text = gure_mesu1 + ", " + gure_mesu2 + ", " + gure_mesu3 + ", " + gure_mesu4 + ", " + gure_mesu5;
                    la2.Location = new Point(0, 23 * this.pan_mesu.Controls.Count);

                    //if (int.Parse(gure_mesu2) > 0)
                    //{
                    la2.ForeColor = Color.Red;
                    //}
                    //else
                    //{
                    //    la2.ForeColor = Color.Blue;
                    //}

                    this.pan_mesu.Controls.Add(la2);
                }
              

                string forin1 = axKHOpenAPI1.GetCommRealData(e.sRealType, 261).Trim(); // 외국계 매도추정합
                string forin2 = axKHOpenAPI1.GetCommRealData(e.sRealType, 262).Trim(); // 외국계 매도추정합 변동
                string forin3 = axKHOpenAPI1.GetCommRealData(e.sRealType, 263).Trim(); // 외국계 매수추정합 
                string forin4 = axKHOpenAPI1.GetCommRealData(e.sRealType, 264).Trim(); // 외국계 매수추정합 변동
                string forin5 = axKHOpenAPI1.GetCommRealData(e.sRealType, 267).Trim(); // 외국계 순매수 추정합
                string forin6 = axKHOpenAPI1.GetCommRealData(e.sRealType, 268).Trim(); // 외국계 순매수 변동
                 
                System.Windows.Forms.Label la3 = new System.Windows.Forms.Label();
                la3.Text = forin1 + ", " + forin2 + ", " + forin3 + ", " + forin4 + ", " + forin5 + ", " + forin6;
                la3.Location = new Point(0, 20);

                if (int.Parse(forin1) > 0)
                {
                    la3.ForeColor = Color.Red;
                }
                else
                {
                    la3.ForeColor = Color.Blue;
                }

                this.pan_forin.Controls.Add(la3);
                #endregion
            }
            else if (e.sRealType == "주식시세")
            {

            }
            else if (e.sRealType == "주식호가잔량")
            {

            }
            else if (e.sRealType == "주식시간외호가")
            {

            }

            //System.Threading.Thread.Sleep(250);
        }

        #endregion

        #region 기타 키움 이벤트들

        /// <summary>
        /// 개인정보 가져오기 종목 가져오기
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void axKHOpenAPI1_OnEventConnect(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnEventConnectEvent e)
        {
            try
            {
                uID = axKHOpenAPI1.GetLoginInfo("USER_ID");
                uName = axKHOpenAPI1.GetLoginInfo("USER_NAME");

                this.toolStripStatusLabelLogin.Text = uName + "님(" + uID + ")은 로그인 중입니다.";

                // 관심종목 세팅
                this.SetFrivates();

                // 상한가 종목
                SetUpperCode();

                // 조건검색 파일 저장  
                int nRet = axKHOpenAPI1.GetConditionLoad(); 
            }
            catch { }
        }

        private void axKHOpenAPI1_OnReceiveMsg(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveMsgEvent e)
        {

        }

        private void axKHOpenAPI1_OnReceiveChejanData(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveChejanDataEvent e)
        {

        }

        /// <summary>
        /// 조건검색 리스트를 로컬 파일로 저장하고 해당 리스트를 담아 놓습니다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void axKHOpenAPI1_OnReceiveConditionVer(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveConditionVerEvent e)
        {
            conditionList.Clear();

            try
            {
                string[] conditions = axKHOpenAPI1.GetConditionNameList().Trim().Split(';');

                foreach (string con in conditions)
                {
                    if (!string.IsNullOrEmpty(con))
                    {
                        if (!string.IsNullOrEmpty(con))
                        {
                            string[] items = con.Split('^');

                            conditionList.Add(items[0], items[1]);
                        }
                    }
                }
            }
            catch { }
        }

        /// <summary>
        /// 투자자 정보 조회
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void axKHOpenAPI1_OnReceiveInvestRealData(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveInvestRealDataEvent e)
        {
            
        }

        private void axKHOpenAPI1_OnReceiveRealCondition(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveRealConditionEvent e)
        {

        }

        private void axKHOpenAPI1_OnReceiveTrCondition(object sender, AxKHOpenAPILib._DKHOpenAPIEvents_OnReceiveTrConditionEvent e)
        {

        }
        #endregion

        #region 각종 이벤트 핸들러들

        /// <summary>
        /// 종목삭제
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                if (this.dataGridView1.Rows.Count > 0)
                {
                    // 그리드를 지울 시 파일 아이템도 지워야함
                    this.DeleteFavor(currentRowIndex, this.dataGridView1.Rows[currentRowIndex].Cells[0].Value.ToString());

                    dataGridView1.AllowUserToAddRows = false;
                    this.dataGridView1.Rows.RemoveAt(currentRowIndex);
                }
            }
            catch (Exception ex)
            {
                this.WriteLog(ex.Message);
            }
            finally
            {
                dataGridView1.AllowUserToAddRows = true;
            }
        }

        /// <summary>
        /// 폼이 닫기면 실시간 종료
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            axKHOpenAPI1.DisconnectRealData("0101");
        }

        /// <summary>
        /// 탭 컨트롤의 선택이 바뀌면 발생하는 이벤트 (관심종목, 현재가, 주문 등의 탭들)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.tabControl1.SelectedIndex == 0)
            {
                WriteLog("최근 등록한 관심 종목 리스트입니다.");

                SetFrivates();

                SetUpperCode();
            }
            else if (this.tabControl1.SelectedIndex == 1)
            {
                WriteLog("코드로 종목을 검색하세요. 한 번 검색한 종목은 관심종목에 자동 등록됩니다. ");

              
            }
            else if (this.tabControl1.SelectedIndex == 2)
            {
                WriteLog("내 계좌번호 조회, 주문 메뉴입니다.");

                this.txtOrderPay.Text = selectedPrice.ToString(); // 현재가 세팅

                accountToolStripMenuItem_Click(null, new EventArgs());

                // 선택된 코드가 없으면 주문 못하게 
                if (string.IsNullOrEmpty(this.selectedCode))
                {
                    this.groupBox1.Enabled = false;
                }
                else
                {
                    this.groupBox1.Enabled = true;
                }


            }
        }

        /// <summary>
        /// 로그인
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void loginToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (axKHOpenAPI1.CommConnect() == 0)
            {
                WriteLog("로그인창 열기 성공");
            }
            else
            {
                WriteLog("로그인창 열기 실패");
            }
        }

        /// <summary>
        /// 히포차트 페이지로 이동
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void infoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("IEXPLORE.EXE", "http://www.hippochart.com");
        }

        /// <summary>
        /// 히포차트 증권 메인 페이지로 이동
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void stockmainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("IEXPLORE.EXE", "http://hippochart.com/stock/kiwoom.aspx");
        }

        /// <summary>
        /// 로그아웃
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void logoutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            axKHOpenAPI1.CommTerminate();
        }

        /// <summary>
        /// 계좌조회
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void accountToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.comAccounts.Items.Clear();

            uID = axKHOpenAPI1.GetLoginInfo("USER_ID");
            uName = axKHOpenAPI1.GetLoginInfo("USER_NAME");

            this.toolStripStatusLabelLogin.Text = uName + "님(" + uID + ")은 로그인 중입니다.";

            string[] accounts = axKHOpenAPI1.GetLoginInfo("ACCNO").Split(';');

            for (int i = 0; i < accounts.Length; i++)
            {
                this.comAccounts.Items.Add(accounts[i]);
            }

            this.comAccounts.SelectedIndex = 0;

            this.WriteLog("정상적으로 계좌번호를 조회했습니다.");
        }

        /// <summary>
        /// 전 종목 조회
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void allcodeToolStripMenuItem_Click(object sender, EventArgs e)
        { 
            frmallCode codes = new frmallCode(axKHOpenAPI1);
            if (codes.ShowDialog() == DialogResult.OK)
            { 
                this.selectedCode = codes.selectedCode;
                this.selectedCodeName = codes.selectedCodeName;

                this.la_codeName.Text = this.selectedCodeName;

                // 관심종목 등록  
                this.SaveMyCode(this.selectedCode, this.selectedCodeName);

                SelectGridCode();
            }
        }

        /// <summary>
        /// 프로그램 종료
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            axKHOpenAPI1.CommTerminate();

            Application.Exit();
        }

        /// <summary>
        /// 관심종목 전체 삭제
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void favordeleToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// 프로그램 소개 버튼
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void duceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frmInfo f = new frmInfo();
            f.ShowDialog();
        }

        /// <summary>
        /// 가운데 넓이 조절하는 라인을 움직일때 발생합니다. 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void splitContainer1_SplitterMoved(object sender, SplitterEventArgs e)
        {
            this.dataGridView1.Columns[1].Width = this.dataGridView1.Width - this.dataGridView1.Columns[2].Width;
        }

        /// <summary>
        /// 폼 사이즈 변경
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_SizeChanged(object sender, EventArgs e)
        {
            try
            {
                toolStripStatusLabel1.Width = this.Width - this.toolStripStatusLabelLogin.Width - 100;
            }
            catch { }
        }

        /// <summary>
        /// 스크롤 이벤트
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            scrollNewIndex = e.NewValue;

            this.hHippoChart1.SeriesListDictionary[0].AxisFactor.XAxis.Interval = ViewCount / 5;
            this.hHippoChart1.SeriesListDictionary[1].AxisFactor.XAxis.Interval = ViewCount / 5; 

            this.DrawChart();
        }

        /// <summary>
        /// 트랙바 - 데이터 개수 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void trackBar1_Scroll(object sender, EventArgs e)
        { 
            ViewCount = trackBar1.Value;

            try
            {
                // 데이터가 추가될 수록 트랙바의 최대값이 변경되야함 

                if (this.selectedChartType == "tick" && this.selectedSU == "1")
                {
                    trackBar1.Maximum = items.Count;
                }
                else
                {
                    trackBar1.Maximum = items_bong.Count;
                }

                trackBar1.LargeChange = ViewCount;
                trackBar1.SmallChange = 1;
            }
            catch { }

            this.hHippoChart1.SeriesListDictionary[0].AxisFactor.XAxis.Interval = ViewCount / 5;
            this.hHippoChart1.SeriesListDictionary[1].AxisFactor.XAxis.Interval = ViewCount / 5; 

            this.DrawChart();
        }

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
        }

        /// <summary>
        /// 투자자별, 거래원
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void gurewonToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frmGurewon g = new frmGurewon(this.axKHOpenAPI1, this.selectedCode);
            g.ShowDialog();
        }

        /// <summary>
        /// 조건검색
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void conditionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frmCondition con = new frmCondition(axKHOpenAPI1, conditionList);

            if (con.ShowDialog() == DialogResult.OK)
            {
                this.selectedCode = con.selectedCode;
                this.selectedCodeName = con.selectedCodeName;

                this.la_codeName.Text = this.selectedCodeName;

                // 관심종목 등록  
                this.SaveMyCode(this.selectedCode, this.selectedCodeName);

                SelectGridCode();
            }
        }

        /// <summary>
        /// 이전 데이터 불러오기
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TSL_btnNext_Click(object sender, EventArgs e)
        {
            axKHOpenAPI1.SetInputValue("종목코드", this.selectedCode);

            SearchNowValue(2);

            if (this.selectedChartType == "tick" && this.selectedSU == "1")
            {
                this.SearchTick(2);
            }
            else
            {
                this.SearchBong(2);
            }
        }

        #endregion

        #region 히포차트 이벤트
        /// <summary>
        /// 폼 사이즈가 변경되면 히포차트 새로고침
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void hHippoChart1_ChartSizeChanged_2(object sender, EventArgs e)
        {
            try
            {
                this.hHippoChart1.DrawChart();
            }
            catch { }
        }

        /// <summary>
        /// 히포차트 마우스 무브 이벤트
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void hHippoChart1_ChartMouseMove(object sender, MouseEventArgs e)
        {
            try
            {
                if (this.hHippoChart1.SeriesListDictionary.Count > 0)
                {
                    double zeroX = this.hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.X;
                    double zeroY = this.hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.Y;
                    double YminusY = this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YMinus.Y;
                    double YminusY_vol = this.hHippoChart1.SeriesListDictionary[1].AxisFactor.YMinus.Y;

                    double zeroY2 = this.hHippoChart1.SeriesListDictionary[1].AxisFactor.Zero.Y;

                    float cX = (float)(zeroX + (e.X - this.hHippoChart1.SeriesListDictionary[0].GraphArea.LeftAxisRectangle.Width - 5));
                    float cY = (float)(zeroY - (e.Y)); 
                    float cY2 = (float)(zeroY2 - (hHippoChart1.Height - e.Y));   

                    // 세로 선
                    this.hHippoChart1.SeriesListDictionary[0].AxisFactor.XAxis.ExtraTicks[0].TickPoint = new System.Drawing.PointF(cX, this.hHippoChart1.SeriesListDictionary[0].AxisFactor.Zero.Y);

                    // 거래량쪽 세로 선
                    this.hHippoChart1.SeriesListDictionary[1].AxisFactor.XAxis.ExtraTicks[0].TickPoint = new System.Drawing.PointF(cX, this.hHippoChart1.SeriesListDictionary[1].AxisFactor.Zero.Y);


                    // 가로 선 
                    this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[1].Value = cY * this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.Interval / this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.UnitPixel;
                    this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[1].BackColor = System.Drawing.Color.Yellow;
                    this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[1].Label.ForeColor = System.Drawing.Color.Black;


                    double mousrVal = this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[1].Value;
                    this.hHippoChart1.SeriesListDictionary[0].AxisFactor.YAxis.ExtraTicks[1].Label.Text = mousrVal.ToString("n0");


                    // 상세 내역
                    for (int i = 0; i < this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items.Count; i++)
                    {
                        float x = this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[i].FigurePoint.X;
                        float y = this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[i].FigurePoint.Y;
                        float w = 0;

                        if (this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[i].Column == null)
                        {
                            w = 5;
                        }
                        else
                        {
                            w = this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[i].Column.Width;
                        }

                        if ((cX >= x && cX <= x + w))
                        {
                            if (e.Y > this.hHippoChart1.SeriesListDictionary[0].AxisFactor.CoordinateRectangle.Height) // 거래량 차트
                            {
                                this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[0].items[i].Balloon.Visible = true;
                                this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[0].items[i].Balloon.Y = cY2;

                                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[i].Balloon.Visible = false;
                            }
                            else // 주식 차트
                            {
                                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[i].Balloon.Visible = true;
                                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[i].Balloon.Y = (float)(YminusY - (this.hHippoChart1.SeriesListDictionary[0].AxisFactor.CoordinateRectangle.Height - e.Y));

                                this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[0].items[i].Balloon.Visible = false;
                            }
                        }
                        else
                        {
                            this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].items[i].Balloon.Visible = false;
                            this.hHippoChart1.SeriesListDictionary[1].SeriesCollection[0].items[i].Balloon.Visible = false;
                        }
                    }

                    this.hHippoChart1.DrawChart();
                }
            }
            catch { }
        }

        #endregion

        #region 기타 메소드들

        /// <summary>
        /// 틱일 경우 누적 합산하여 새 봉을 만듭니다 (새 아이템 생성)
        /// </summary>
        /// <param name="nowItem"></param>
        private void MakeCaluateTickItem(SeriesItem nowItem, SeriesItem nowItemVol)
        {
            item_real_bong.YValue = nowItem.YValue;

            if(item_real_bong.HighValue <= nowItem.YValue)
            {
                item_real_bong.HighValue = nowItem.YValue;
            }

            if (item_real_bong.LowValue >= nowItem.YValue)
            {
                item_real_bong.LowValue = nowItem.YValue;
            }


            item_realVol_bong.YValue += nowItemVol.YValue;

        }

        // 로그를 출력합니다.
        public void WriteLog(string format)
        { 
            this.toolStripStatusLabel1.Text = format;
        }

        /// <summary>
        /// 상단 일, 주, 월 작은 버튼 컨트롤을 위한 메소드
        /// </summary>
        private void AllBtnFalse()
        {
            TSBday.Checked = false;
            TSBweek.Checked = false;
            TSBmonth.Checked = false;
            TSBmin.Checked = false;
            TSBtick.Checked = false;
        }

        private void AllBtnFalse2()
        {
            this.TSL_su_1.Checked = false;
            this.TSL_su_3.Checked = false;
            this.TSL_su_5.Checked = false;
            this.TSL_su_10.Checked = false;
            this.TSL_su_30.Checked = false;
            this.TSL_su_60.Checked = false;
            this.TSL_su_120.Checked = false;
        } 
        #endregion

        #region 오른쪽 파단 3개 버튼들
        /// <summary>
        /// 데이터 개수 플러스
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_plus_Click(object sender, EventArgs e)
        {

            try
            {
                // 데이터가 추가될 수록 트랙바의 최대값이 변경되야함 

                if (this.selectedChartType == "tick" && this.selectedSU == "1")
                {
                    trackBar1.Maximum = items.Count;
                }
                else
                {
                    trackBar1.Maximum = items_bong.Count;
                }

                this.trackBar1.Value = this.trackBar1.Value - (int)(this.trackBar1.Value * 0.15);

                ViewCount = trackBar1.Value;

                trackBar1.LargeChange = ViewCount;
                trackBar1.SmallChange = 1;
            }
            catch { }

            this.hHippoChart1.SeriesListDictionary[0].AxisFactor.XAxis.Interval = ViewCount / 5;
            this.hHippoChart1.SeriesListDictionary[1].AxisFactor.XAxis.Interval = ViewCount / 5;

            this.DrawChart();

        }

        /// <summary>
        /// 데이터 개수 마이너스
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_minus_Click(object sender, EventArgs e)
        {
            try
            {
                // 데이터가 추가될 수록 트랙바의 최대값이 변경되야함 

                if (this.selectedChartType == "tick" && this.selectedSU == "1")
                {
                    trackBar1.Maximum = items.Count;
                }
                else
                {
                    trackBar1.Maximum = items_bong.Count;
                }

                this.trackBar1.Value = this.trackBar1.Value + (int)(this.trackBar1.Value * 0.15);

                if (this.trackBar1.Value < 10)
                {
                    this.trackBar1.Value = this.trackBar1.Value + 1;
                }

                ViewCount = trackBar1.Value;


                trackBar1.LargeChange = ViewCount;
                trackBar1.SmallChange = 1;
            }
            catch { }

            this.hHippoChart1.SeriesListDictionary[0].AxisFactor.XAxis.Interval = ViewCount / 5;
            this.hHippoChart1.SeriesListDictionary[1].AxisFactor.XAxis.Interval = ViewCount / 5;

            this.DrawChart();

            
        }

        /// <summary>
        /// 모든 데이터 다보기
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_all_Click(object sender, EventArgs e)
        { 
            try
            {
                // 데이터가 추가될 수록 트랙바의 최대값이 변경되야함 

                if (this.selectedChartType == "tick" && this.selectedSU == "1")
                {
                    trackBar1.Maximum = items.Count;
                }
                else
                {
                    trackBar1.Maximum = items_bong.Count;
                }

                if (this.selectedChartType == "tick" && this.selectedSU == "1")
                {
                    this.trackBar1.Value = this.items.Count;
                }
                else
                {
                    this.trackBar1.Value = this.items_bong.Count;
                }

                ViewCount = trackBar1.Value;

                trackBar1.LargeChange = ViewCount;
                trackBar1.SmallChange = 1;
            }
            catch { }

            this.hHippoChart1.SeriesListDictionary[0].AxisFactor.XAxis.Interval = ViewCount / 5;
            this.hHippoChart1.SeriesListDictionary[1].AxisFactor.XAxis.Interval = ViewCount / 5; 

            this.DrawChart();
        }
        #endregion

        #region 데이터 저장 시나리오
        /// <summary>
        /// 실시간 데이터 저장 시작
        ///  - 실시간 데이터를 실시간으로 파일로 저장하며 성능에 지장이 있으므로 일단 변수에만 저장을 한다. 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void realsaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            isRealDataSaveStart = !isRealDataSaveStart;

            if(isRealDataSaveStart)
            {
                realsaveToolStripMenuItem.Text = "실시간 데이터 저장 종료";
            }
            else
            {
                realsaveToolStripMenuItem.Text = "실시간 데이터 저장 시작";
            }
        }

        /// <summary>
        /// 장이 끝나고 저장된 실시간 데이터를 파일에 저장합니다. 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void realTextSaveGoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // 실시간 데이터를 파일로 저장하는 코드 삽입
        }

        /// <summary>
        /// 현재 차트에 보여지는 데이터를 저장한다. 
        ///  - 현재 히포차트 그래프에 보여지는 모든 데이터를 파일에 직접 저장한다. 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void nowsaveToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        #endregion

        #region 주가, 이평선 라인을 보일 것인가 여부 이벤트
        private void chkSR0_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].Visible = this.chkSR0.Checked; 
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[0].LegendVisible = this.chkSR0.Checked;

                this.hHippoChart1.DrawChart();
            }
            catch { }
        }

        private void chkSR1_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[1].Visible = this.chkSR1.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[1].LegendVisible = this.chkSR1.Checked;
                this.hHippoChart1.DrawChart();
            }
            catch { }
        }

        private void chkSR2_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[2].Visible = this.chkSR2.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[2].LegendVisible = this.chkSR2.Checked;
                this.hHippoChart1.DrawChart();
            }
            catch { }
        }

        private void chkSR3_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[3].Visible = this.chkSR3.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[3].LegendVisible = this.chkSR3.Checked;
                this.hHippoChart1.DrawChart();
            }
            catch { }
        }
        
        /// <summary>
         /// 볼린저밴드
         /// </summary>
         /// <param name="sender"></param>
         /// <param name="e"></param>
        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[4].Visible = this.chkBB.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[4].LegendVisible = this.chkBB.Checked;
                this.hHippoChart1.DrawChart();
            }
            catch { }
        }
        
        /// <summary>
        /// MACD
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            try
            { 
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[5].Visible = this.checkBox3.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[6].Visible = this.checkBox3.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[5].LegendVisible = this.checkBox3.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[6].LegendVisible = this.checkBox3.Checked;

                this.hHippoChart1.DrawChart();
            }
            catch { }
        }

        /// <summary>
        /// OSC
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void chkOSC_CheckedChanged(object sender, EventArgs e)
        {
            try
            { 
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[7].Visible = this.chkOSC.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[7].LegendVisible = this.chkOSC.Checked;

                this.hHippoChart1.DrawChart();
            }
            catch { }
        }

        /// <summary>
        /// 일목균형표
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void chkILMOK_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[8].Visible = this.chkILMOK.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[9].Visible = this.chkILMOK.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[10].Visible = this.chkILMOK.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[11].Visible = this.chkILMOK.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[12].Visible = this.chkILMOK.Checked;

                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[8].LegendVisible = this.chkILMOK.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[9].LegendVisible = this.chkILMOK.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[10].LegendVisible = this.chkILMOK.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[11].LegendVisible = this.chkILMOK.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[12].LegendVisible = this.chkILMOK.Checked;

                //this.hHippoChart1.DrawChart();
                this.DrawChart();
            }
            catch { }
        }

        /// <summary>
        /// 스토캐스틱
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void chkStochastic_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[13].Visible = this.chkStochastic.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[13].LegendVisible = this.chkStochastic.Checked;

                this.hHippoChart1.DrawChart();
            }
            catch { }
        }

        /// <summary>
        /// RSI
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void chkRSI_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[14].Visible = this.chkRSI.Checked;
                this.hHippoChart1.SeriesListDictionary[0].SeriesCollection[14].LegendVisible = this.chkRSI.Checked;

                if(this.chkRSI.Checked)
                { 
                    curList.AxisFactor.AxisItems[2].Markers.Add(new AxisMarker("RSI - 매도라인", 70));
                    curList.AxisFactor.AxisItems[2].Markers.Add(new AxisMarker("RSI - 매수라인", 30));

                    curList.AxisFactor.AxisItems[2].Markers[0].TextFormat.Alignment = StringAlignment.Near;
                    curList.AxisFactor.AxisItems[2].Markers[1].TextFormat.Alignment = StringAlignment.Near;

                    curList.AxisFactor.AxisItems[2].Markers[0].TextFormat.LineAlignment = StringAlignment.Near;
                    curList.AxisFactor.AxisItems[2].Markers[1].TextFormat.LineAlignment = StringAlignment.Near;

                    curList.AxisFactor.AxisItems[2].Markers[1].Line.LineColor = Color.Red;
                    curList.AxisFactor.AxisItems[2].Markers[1].Label.ForeColor = Color.Red;
                }
                else
                {
                    curList.AxisFactor.AxisItems[2].Markers.Clear();
                }

                this.hHippoChart1.DrawChart();
            }
            catch { }
        }
        #endregion

        #region 주문

        /// <summary>
        /// 주문을 처리합니다. 
        /// </summary>
        /// <param name="sScreenNo"></param>
        /// <param name="sAccNo"></param>
        /// <param name="nOrderType"></param>
        /// <param name="sCode"></param>
        /// <param name="nQty"></param>
        /// <param name="nPrice"></param>
        /// <param name="sHogaGB"></param>
        /// <param name="sOrgOrderNo"></param>
        /// <returns></returns>
        private int Order(string sScreenNo, string sAccNo, int nOrderType, string sCode, int nQty, int nPrice, string sHogaGB, string sOrgOrderNo)
        { 
            int lRet = axKHOpenAPI1.SendOrder(
                   "주식주문",
                   sScreenNo,
                   sAccNo,
                   nOrderType,
                   sCode,
                   nQty,
                   nPrice,
                   sHogaGB,
                   sOrgOrderNo
                   );

            return lRet;
        }
         
        /// <summary>
        /// 주식 주문하기
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnORDER_GOGO_Click(object sender, EventArgs e)
        {
            #region 유효성 체크
            // 계좌번호 입력 여부 확인
            if (comAccounts.Items.Count <= 0)
            {
                this.WriteLog("내 계좌번호가 없습니다."); 
                return;
            }
             
            // 종목코드 입력 여부 확인
            if (string.IsNullOrEmpty(this.selectedCode))
            {
                this.WriteLog("주문할 종목이 선택되지 않았습니다."); 
                return;
            }

            // 주문수량 입력 여부 확인
            int orderCounts = 0;

            try
            {
                if (string.IsNullOrEmpty(txtOrderCounts.Text))
                {
                    this.WriteLog("주문 수량을 입력하세요");
                    return;
                }
                else
                {
                    orderCounts = Int32.Parse(txtOrderCounts.Text);
                }
            }
            catch
            {
                this.WriteLog("주문 수량은 숫자만 입력 가능합니다. 따옴표, 문자 등 제거해주세요.");
                return;
            }
             
            // 거래구분 취득
            // 0:지정가, 3:시장가, 5:조건부지정가, 6:최유리지정가, 7:최우선지정가,
            // 10:지정가IOC, 13:시장가IOC, 16:최유리IOC, 20:지정가FOK, 23:시장가FOK,
            // 26:최유리FOK, 61:장개시전시간외, 62:시간외단일가매매, 81:시간외종가 
            string orderCategory = KOACode.hogaGb[comOderType.SelectedIndex].code; 

            // 주문수량 입력 여부 확인
            int orderPay = 0;

            try
            {
                if (string.IsNullOrEmpty(txtOrderPay.Text))
                {
                    this.WriteLog("주문 가격을 입력하세요");
                    return;
                }
                else
                {
                    orderPay = Int32.Parse(txtOrderPay.Text);
                }
            }
            catch
            {
                this.WriteLog("주문 가격은 숫자만 입력 가능합니다. 따옴표, 문자 등 제거해주세요.");
                return;
            }

            // 매매구분 취득
            // (1:신규매수, 2:신규매도 3:매수취소, 
            // 4:매도취소, 5:매수정정, 6:매도정정) 
            int comBuySellTypeint = KOACode.orderType[comBuySellType.SelectedIndex].code;
             
            // 원주문번호 입력 여부 
            if (comBuySellTypeint > 2 && txtOriginalOdernumber.Text.Length < 1)
            {
                this.WriteLog("원주문번호를 입력해주세요.");
                return;
            }
            #endregion

            string messge = "";

            messge += " - 종목 : " + axKHOpenAPI1.GetMasterCodeName(this.selectedCode);
            messge += "\r\n - 주문가격 : " + orderPay.ToString("n0");
            messge += "\r\n - 주문수량 : " + orderCounts.ToString("n0");
            messge += "\r\n위와 같이 주문을 하시겠습니까?";

            if (MessageBox.Show(messge, "주문", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                // 주식주문
                int lRet = this.Order( 
                    "0101",
                    comAccounts.Text.Trim(),
                    comBuySellTypeint,
                    this.selectedCode,
                    orderCounts,
                    orderPay,
                    orderCategory,
                    txtOriginalOdernumber.Text.Trim()
                    );

                if (lRet == 0)
                {
                    this.WriteLog("주문이 처리되었습니다.");
                }
                else
                {
                    this.WriteLog("주문이 실패했습니다." + lRet.ToString());
                }
            } 
        }

        #endregion

        #region ===== 추가 기능 ==================

        /// <summary>
        /// DB 저장하기
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void dbsaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frmCodeSave sav = new frmCodeSave(axKHOpenAPI1);
            sav.Show();
        }

        /// <summary>
        /// 조회한 900개 데이터 엑셀로 저장하기
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void excelsaveToolStripMenuItem_Click(object sender, EventArgs e)
        { 
            string filename = selectedCodeName + ".csv";

            this.saveFileDialog1.FileName = filename;
            this.saveFileDialog1.DefaultExt = "csv";
            this.saveFileDialog1.Filter = "CSV files (*.csv)|*.csv|All Files(*.*)|*.*";
            this.saveFileDialog1.InitialDirectory = "c:\\"; 

            DialogResult dr = saveFileDialog1.ShowDialog();

            if (dr == DialogResult.OK)
            {
                if (saveFileDialog1.FileName != "")
                { 
                    string[] headerNames = { "Date", "Start", "Close", "High", "Low" };

                    FileStream fs = new FileStream(saveFileDialog1.FileName, System.IO.FileMode.Create, System.IO.FileAccess.Write);
                    StreamWriter sw = new StreamWriter(fs, System.Text.Encoding.Default);

                    string data = "";

                    //열 이름
                    for (int i = 0; i < headerNames.Length; i++)
                    {
                        data += headerNames[i];

                        if (i < headerNames.Length - 1)
                        {
                            data += ",";
                        }
                    }

                    sw.WriteLine(data);
                     
                    for (int i = 0; i < items_bong.Count; i++)
                    {
                        data = "";

                        for (int j = 0; j < headerNames.Length; j++)
                        {
                            if (j == 0)
                            {
                                data += items_bong[i].Name;
                            }
                            else if (j == 1)
                            {
                                data += items_bong[i].YStartValue;
                            }
                            else if (j == 2)
                            {
                                data += items_bong[i].YValue;
                            }
                            else if (j == 3)
                            {
                                data += items_bong[i].HighValue;
                            }
                            else if (j == 4)
                            {
                                data += items_bong[i].LowValue;
                            } 

                            if (j < headerNames.Length - 1)
                            {
                                data += ",";
                            }
                        }

                        sw.WriteLine(data);
                    }

                    sw.Close();
                    fs.Close();
                }
            }
        }
    }


    #endregion
}
