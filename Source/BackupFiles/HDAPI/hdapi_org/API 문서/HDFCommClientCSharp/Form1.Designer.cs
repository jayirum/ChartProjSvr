namespace HDFCommClientCSharp
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
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.m_CommAgent = new AxHDFCommAgentLib.AxHDFCommAgent();
            this.btnConnect = new System.Windows.Forms.Button();
            this.btnDisConnect = new System.Windows.Forms.Button();
            this.btnLogout = new System.Windows.Forms.Button();
            this.btnLogin = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.txtUserID = new System.Windows.Forms.TextBox();
            this.mtxtUserPwd = new System.Windows.Forms.MaskedTextBox();
            this.mtxtUserCertPwd = new System.Windows.Forms.MaskedTextBox();
            this.lstLog = new System.Windows.Forms.ListBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.rdFX = new System.Windows.Forms.RadioButton();
            this.rdForeign = new System.Windows.Forms.RadioButton();
            this.rdDomestic = new System.Windows.Forms.RadioButton();
            this.panel1 = new System.Windows.Forms.Panel();
            this.rdFID = new System.Windows.Forms.RadioButton();
            this.rdTR = new System.Windows.Forms.RadioButton();
            this.txtFidInput2 = new System.Windows.Forms.TextBox();
            this.txtFidInput1 = new System.Windows.Forms.TextBox();
            this.txtTrInput = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.txtFidCode = new System.Windows.Forms.TextBox();
            this.txtTrCode = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.btnQuery = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.txtRealType = new System.Windows.Forms.TextBox();
            this.txtRealInput = new System.Windows.Forms.TextBox();
            this.btnRealUnReg = new System.Windows.Forms.Button();
            this.btnRealReg = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label12 = new System.Windows.Forms.Label();
            this.txtAccNo = new System.Windows.Forms.TextBox();
            this.btnJumunCheUnReg = new System.Windows.Forms.Button();
            this.btnJumunCheReg = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.txtJumunInput = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.txtJumunCode = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.btnJumun = new System.Windows.Forms.Button();
            this.btnBusinessDay = new System.Windows.Forms.Button();
            this.btnFrBusinessDay = new System.Windows.Forms.Button();
            this.btnGetSystemTp = new System.Windows.Forms.Button();
            this.btnGetForeignMst = new System.Windows.Forms.Button();
            this.btnGetForeignMst2 = new System.Windows.Forms.Button();
            this.btnGetForeignMst3 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.m_CommAgent)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_CommAgent
            // 
            this.m_CommAgent.Enabled = true;
            this.m_CommAgent.Location = new System.Drawing.Point(646, 542);
            this.m_CommAgent.Name = "m_CommAgent";
            this.m_CommAgent.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("m_CommAgent.OcxState")));
            this.m_CommAgent.Size = new System.Drawing.Size(100, 50);
            this.m_CommAgent.TabIndex = 0;
            this.m_CommAgent.OnDataRecv += new AxHDFCommAgentLib._DHDFCommAgentEvents_OnDataRecvEventHandler(this.m_CommAgent_OnDataRecv);
            this.m_CommAgent.OnGetBroadData += new AxHDFCommAgentLib._DHDFCommAgentEvents_OnGetBroadDataEventHandler(this.m_CommAgent_OnGetBroadData);
            this.m_CommAgent.OnGetMsg += new AxHDFCommAgentLib._DHDFCommAgentEvents_OnGetMsgEventHandler(this.m_CommAgent_OnGetMsg);
            this.m_CommAgent.OnGetMsgWithRqId += new AxHDFCommAgentLib._DHDFCommAgentEvents_OnGetMsgWithRqIdEventHandler(this.m_CommAgent_OnGetMsgWithRqId);
            // 
            // btnConnect
            // 
            this.btnConnect.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnConnect.Location = new System.Drawing.Point(558, 17);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 23);
            this.btnConnect.TabIndex = 1;
            this.btnConnect.Text = "통신연결";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // btnDisConnect
            // 
            this.btnDisConnect.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnDisConnect.Location = new System.Drawing.Point(639, 17);
            this.btnDisConnect.Name = "btnDisConnect";
            this.btnDisConnect.Size = new System.Drawing.Size(75, 23);
            this.btnDisConnect.TabIndex = 2;
            this.btnDisConnect.Text = "통신종료";
            this.btnDisConnect.UseVisualStyleBackColor = true;
            this.btnDisConnect.Click += new System.EventHandler(this.btnDisConnect_Click);
            // 
            // btnLogout
            // 
            this.btnLogout.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnLogout.Location = new System.Drawing.Point(97, 17);
            this.btnLogout.Name = "btnLogout";
            this.btnLogout.Size = new System.Drawing.Size(75, 23);
            this.btnLogout.TabIndex = 3;
            this.btnLogout.Text = "로그아웃";
            this.btnLogout.UseVisualStyleBackColor = true;
            this.btnLogout.Click += new System.EventHandler(this.btnLogout_Click);
            // 
            // btnLogin
            // 
            this.btnLogin.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnLogin.Location = new System.Drawing.Point(16, 17);
            this.btnLogin.Name = "btnLogin";
            this.btnLogin.Size = new System.Drawing.Size(75, 23);
            this.btnLogin.TabIndex = 4;
            this.btnLogin.Text = "로그인";
            this.btnLogin.UseVisualStyleBackColor = true;
            this.btnLogin.Click += new System.EventHandler(this.btnLogin_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label1.Location = new System.Drawing.Point(14, 57);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "사용자ID";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label2.Location = new System.Drawing.Point(191, 57);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 12);
            this.label2.TabIndex = 6;
            this.label2.Text = "비밀번호";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label3.Location = new System.Drawing.Point(394, 57);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(114, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "공인인증 비밀번호";
            // 
            // txtUserID
            // 
            this.txtUserID.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.txtUserID.Location = new System.Drawing.Point(77, 53);
            this.txtUserID.MaxLength = 8;
            this.txtUserID.Name = "txtUserID";
            this.txtUserID.Size = new System.Drawing.Size(100, 21);
            this.txtUserID.TabIndex = 8;
            // 
            // mtxtUserPwd
            // 
            this.mtxtUserPwd.Location = new System.Drawing.Point(254, 53);
            this.mtxtUserPwd.Name = "mtxtUserPwd";
            this.mtxtUserPwd.Size = new System.Drawing.Size(120, 21);
            this.mtxtUserPwd.TabIndex = 9;
            this.mtxtUserPwd.UseSystemPasswordChar = true;
            // 
            // mtxtUserCertPwd
            // 
            this.mtxtUserCertPwd.Location = new System.Drawing.Point(514, 53);
            this.mtxtUserCertPwd.Name = "mtxtUserCertPwd";
            this.mtxtUserCertPwd.Size = new System.Drawing.Size(200, 21);
            this.mtxtUserCertPwd.TabIndex = 10;
            this.mtxtUserCertPwd.UseSystemPasswordChar = true;
            // 
            // lstLog
            // 
            this.lstLog.FormattingEnabled = true;
            this.lstLog.HorizontalScrollbar = true;
            this.lstLog.ItemHeight = 12;
            this.lstLog.Location = new System.Drawing.Point(12, 423);
            this.lstLog.Name = "lstLog";
            this.lstLog.Size = new System.Drawing.Size(708, 112);
            this.lstLog.TabIndex = 11;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.mtxtUserCertPwd);
            this.groupBox1.Controls.Add(this.btnConnect);
            this.groupBox1.Controls.Add(this.btnDisConnect);
            this.groupBox1.Controls.Add(this.mtxtUserPwd);
            this.groupBox1.Controls.Add(this.btnLogout);
            this.groupBox1.Controls.Add(this.txtUserID);
            this.groupBox1.Controls.Add(this.btnLogin);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(729, 89);
            this.groupBox1.TabIndex = 12;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "통신연결 및 로그인";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.panel2);
            this.groupBox2.Controls.Add(this.panel1);
            this.groupBox2.Controls.Add(this.txtFidInput2);
            this.groupBox2.Controls.Add(this.txtFidInput1);
            this.groupBox2.Controls.Add(this.txtTrInput);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.txtFidCode);
            this.groupBox2.Controls.Add(this.txtTrCode);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.btnQuery);
            this.groupBox2.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox2.Location = new System.Drawing.Point(12, 107);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(729, 112);
            this.groupBox2.TabIndex = 13;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "TR / FID 조회";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.rdFX);
            this.panel2.Controls.Add(this.rdForeign);
            this.panel2.Controls.Add(this.rdDomestic);
            this.panel2.Location = new System.Drawing.Point(352, 22);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(247, 27);
            this.panel2.TabIndex = 20;
            // 
            // rdFX
            // 
            this.rdFX.AutoSize = true;
            this.rdFX.Location = new System.Drawing.Point(171, 5);
            this.rdFX.Name = "rdFX";
            this.rdFX.Size = new System.Drawing.Size(66, 16);
            this.rdFX.TabIndex = 24;
            this.rdFX.TabStop = true;
            this.rdFX.Text = "FX마진";
            this.rdFX.UseVisualStyleBackColor = true;
            this.rdFX.CheckedChanged += new System.EventHandler(this.rdFX_CheckedChanged);
            // 
            // rdForeign
            // 
            this.rdForeign.AutoSize = true;
            this.rdForeign.Location = new System.Drawing.Point(90, 5);
            this.rdForeign.Name = "rdForeign";
            this.rdForeign.Size = new System.Drawing.Size(75, 16);
            this.rdForeign.TabIndex = 23;
            this.rdForeign.TabStop = true;
            this.rdForeign.Text = "해외선물";
            this.rdForeign.UseVisualStyleBackColor = true;
            this.rdForeign.CheckedChanged += new System.EventHandler(this.rdForeign_CheckedChanged);
            // 
            // rdDomestic
            // 
            this.rdDomestic.AutoSize = true;
            this.rdDomestic.Location = new System.Drawing.Point(9, 5);
            this.rdDomestic.Name = "rdDomestic";
            this.rdDomestic.Size = new System.Drawing.Size(75, 16);
            this.rdDomestic.TabIndex = 22;
            this.rdDomestic.TabStop = true;
            this.rdDomestic.Text = "국내선물";
            this.rdDomestic.UseVisualStyleBackColor = true;
            this.rdDomestic.CheckedChanged += new System.EventHandler(this.rdDomestic_CheckedChanged);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.rdFID);
            this.panel1.Controls.Add(this.rdTR);
            this.panel1.Location = new System.Drawing.Point(97, 20);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(196, 27);
            this.panel1.TabIndex = 19;
            // 
            // rdFID
            // 
            this.rdFID.AutoSize = true;
            this.rdFID.Location = new System.Drawing.Point(98, 5);
            this.rdFID.Name = "rdFID";
            this.rdFID.Size = new System.Drawing.Size(75, 16);
            this.rdFID.TabIndex = 6;
            this.rdFID.TabStop = true;
            this.rdFID.Text = "FID 통신";
            this.rdFID.UseVisualStyleBackColor = true;
            this.rdFID.CheckedChanged += new System.EventHandler(this.rdFID_CheckedChanged);
            // 
            // rdTR
            // 
            this.rdTR.AutoSize = true;
            this.rdTR.Location = new System.Drawing.Point(23, 5);
            this.rdTR.Name = "rdTR";
            this.rdTR.Size = new System.Drawing.Size(72, 16);
            this.rdTR.TabIndex = 5;
            this.rdTR.TabStop = true;
            this.rdTR.Text = "TR 통신";
            this.rdTR.UseVisualStyleBackColor = true;
            this.rdTR.CheckedChanged += new System.EventHandler(this.rdTR_CheckedChanged);
            // 
            // txtFidInput2
            // 
            this.txtFidInput2.Location = new System.Drawing.Point(571, 81);
            this.txtFidInput2.Name = "txtFidInput2";
            this.txtFidInput2.Size = new System.Drawing.Size(150, 21);
            this.txtFidInput2.TabIndex = 15;
            // 
            // txtFidInput1
            // 
            this.txtFidInput1.Location = new System.Drawing.Point(352, 81);
            this.txtFidInput1.Name = "txtFidInput1";
            this.txtFidInput1.Size = new System.Drawing.Size(150, 21);
            this.txtFidInput1.TabIndex = 14;
            // 
            // txtTrInput
            // 
            this.txtTrInput.Location = new System.Drawing.Point(352, 55);
            this.txtTrInput.Name = "txtTrInput";
            this.txtTrInput.Size = new System.Drawing.Size(369, 21);
            this.txtTrInput.TabIndex = 13;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label8.Location = new System.Drawing.Point(512, 84);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(53, 12);
            this.label8.TabIndex = 12;
            this.label8.Text = "INPUT2";
            // 
            // txtFidCode
            // 
            this.txtFidCode.Location = new System.Drawing.Point(97, 81);
            this.txtFidCode.Name = "txtFidCode";
            this.txtFidCode.Size = new System.Drawing.Size(150, 21);
            this.txtFidCode.TabIndex = 11;
            // 
            // txtTrCode
            // 
            this.txtTrCode.Location = new System.Drawing.Point(97, 55);
            this.txtTrCode.Name = "txtTrCode";
            this.txtTrCode.Size = new System.Drawing.Size(150, 21);
            this.txtTrCode.TabIndex = 10;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label7.Location = new System.Drawing.Point(265, 84);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(79, 12);
            this.label7.TabIndex = 9;
            this.label7.Text = "FID INPUT1";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label6.Location = new System.Drawing.Point(265, 58);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(69, 12);
            this.label6.TabIndex = 8;
            this.label6.Text = "TR INPUT";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label5.Location = new System.Drawing.Point(14, 84);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(65, 12);
            this.label5.TabIndex = 7;
            this.label5.Text = "FID Code";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label4.Location = new System.Drawing.Point(14, 58);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(62, 12);
            this.label4.TabIndex = 6;
            this.label4.Text = "TR Code";
            // 
            // btnQuery
            // 
            this.btnQuery.Location = new System.Drawing.Point(16, 20);
            this.btnQuery.Name = "btnQuery";
            this.btnQuery.Size = new System.Drawing.Size(75, 23);
            this.btnQuery.TabIndex = 0;
            this.btnQuery.Text = "조  회";
            this.btnQuery.UseVisualStyleBackColor = true;
            this.btnQuery.Click += new System.EventHandler(this.btnQuery_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Controls.Add(this.txtRealType);
            this.groupBox3.Controls.Add(this.txtRealInput);
            this.groupBox3.Controls.Add(this.btnRealUnReg);
            this.groupBox3.Controls.Add(this.btnRealReg);
            this.groupBox3.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox3.Location = new System.Drawing.Point(12, 225);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(208, 102);
            this.groupBox3.TabIndex = 14;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "실시간 정보 입력(시세 등)";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label10.Location = new System.Drawing.Point(9, 75);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(70, 12);
            this.label10.TabIndex = 14;
            this.label10.Text = "실시간번호";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label9.Location = new System.Drawing.Point(22, 52);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(57, 12);
            this.label9.TabIndex = 13;
            this.label9.Text = "종목코드";
            // 
            // txtRealType
            // 
            this.txtRealType.Location = new System.Drawing.Point(89, 72);
            this.txtRealType.Name = "txtRealType";
            this.txtRealType.Size = new System.Drawing.Size(50, 21);
            this.txtRealType.TabIndex = 12;
            // 
            // txtRealInput
            // 
            this.txtRealInput.Location = new System.Drawing.Point(89, 49);
            this.txtRealInput.Name = "txtRealInput";
            this.txtRealInput.Size = new System.Drawing.Size(100, 21);
            this.txtRealInput.TabIndex = 11;
            // 
            // btnRealUnReg
            // 
            this.btnRealUnReg.Location = new System.Drawing.Point(107, 20);
            this.btnRealUnReg.Name = "btnRealUnReg";
            this.btnRealUnReg.Size = new System.Drawing.Size(85, 23);
            this.btnRealUnReg.TabIndex = 2;
            this.btnRealUnReg.Text = "실시간 해지";
            this.btnRealUnReg.UseVisualStyleBackColor = true;
            this.btnRealUnReg.Click += new System.EventHandler(this.btnRealUnReg_Click);
            // 
            // btnRealReg
            // 
            this.btnRealReg.Location = new System.Drawing.Point(16, 20);
            this.btnRealReg.Name = "btnRealReg";
            this.btnRealReg.Size = new System.Drawing.Size(85, 23);
            this.btnRealReg.TabIndex = 1;
            this.btnRealReg.Text = "실시간 등록";
            this.btnRealReg.UseVisualStyleBackColor = true;
            this.btnRealReg.Click += new System.EventHandler(this.btnRealReg_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label12);
            this.groupBox4.Controls.Add(this.txtAccNo);
            this.groupBox4.Controls.Add(this.btnJumunCheUnReg);
            this.groupBox4.Controls.Add(this.btnJumunCheReg);
            this.groupBox4.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox4.Location = new System.Drawing.Point(12, 333);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(250, 80);
            this.groupBox4.TabIndex = 15;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "실시간 주문/체결 입력";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label12.Location = new System.Drawing.Point(14, 52);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(57, 12);
            this.label12.TabIndex = 13;
            this.label12.Text = "계좌번호";
            // 
            // txtAccNo
            // 
            this.txtAccNo.Location = new System.Drawing.Point(77, 49);
            this.txtAccNo.Name = "txtAccNo";
            this.txtAccNo.Size = new System.Drawing.Size(100, 21);
            this.txtAccNo.TabIndex = 11;
            // 
            // btnJumunCheUnReg
            // 
            this.btnJumunCheUnReg.Location = new System.Drawing.Point(127, 20);
            this.btnJumunCheUnReg.Name = "btnJumunCheUnReg";
            this.btnJumunCheUnReg.Size = new System.Drawing.Size(105, 23);
            this.btnJumunCheUnReg.TabIndex = 2;
            this.btnJumunCheUnReg.Text = "주문/체결 해지";
            this.btnJumunCheUnReg.UseVisualStyleBackColor = true;
            this.btnJumunCheUnReg.Click += new System.EventHandler(this.btnJumunCheUnReg_Click);
            // 
            // btnJumunCheReg
            // 
            this.btnJumunCheReg.Location = new System.Drawing.Point(16, 20);
            this.btnJumunCheReg.Name = "btnJumunCheReg";
            this.btnJumunCheReg.Size = new System.Drawing.Size(105, 23);
            this.btnJumunCheReg.TabIndex = 1;
            this.btnJumunCheReg.Text = "주문/체결 등록";
            this.btnJumunCheReg.UseVisualStyleBackColor = true;
            this.btnJumunCheReg.Click += new System.EventHandler(this.btnJumunCheReg_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.txtJumunInput);
            this.groupBox5.Controls.Add(this.label13);
            this.groupBox5.Controls.Add(this.txtJumunCode);
            this.groupBox5.Controls.Add(this.label11);
            this.groupBox5.Controls.Add(this.btnJumun);
            this.groupBox5.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox5.Location = new System.Drawing.Point(368, 333);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(373, 80);
            this.groupBox5.TabIndex = 16;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "주문 전송";
            // 
            // txtJumunInput
            // 
            this.txtJumunInput.Location = new System.Drawing.Point(114, 52);
            this.txtJumunInput.Name = "txtJumunInput";
            this.txtJumunInput.Size = new System.Drawing.Size(251, 21);
            this.txtJumunInput.TabIndex = 15;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label13.Location = new System.Drawing.Point(7, 55);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(100, 12);
            this.label13.TabIndex = 14;
            this.label13.Text = "주문 TR INPUT";
            // 
            // txtJumunCode
            // 
            this.txtJumunCode.Location = new System.Drawing.Point(209, 22);
            this.txtJumunCode.Name = "txtJumunCode";
            this.txtJumunCode.Size = new System.Drawing.Size(156, 21);
            this.txtJumunCode.TabIndex = 12;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label11.Location = new System.Drawing.Point(112, 25);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(93, 12);
            this.label11.TabIndex = 11;
            this.label11.Text = "주문 TR Code";
            // 
            // btnJumun
            // 
            this.btnJumun.Location = new System.Drawing.Point(6, 20);
            this.btnJumun.Name = "btnJumun";
            this.btnJumun.Size = new System.Drawing.Size(100, 23);
            this.btnJumun.TabIndex = 1;
            this.btnJumun.Text = "전  송";
            this.btnJumun.UseVisualStyleBackColor = true;
            this.btnJumun.Click += new System.EventHandler(this.btnJumun_Click);
            // 
            // btnBusinessDay
            // 
            this.btnBusinessDay.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnBusinessDay.Location = new System.Drawing.Point(339, 288);
            this.btnBusinessDay.Name = "btnBusinessDay";
            this.btnBusinessDay.Size = new System.Drawing.Size(80, 23);
            this.btnBusinessDay.TabIndex = 17;
            this.btnBusinessDay.Text = "국내영업일";
            this.btnBusinessDay.UseVisualStyleBackColor = true;
            this.btnBusinessDay.Click += new System.EventHandler(this.btnBusinessDay_Click);
            // 
            // btnFrBusinessDay
            // 
            this.btnFrBusinessDay.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnFrBusinessDay.Location = new System.Drawing.Point(425, 288);
            this.btnFrBusinessDay.Name = "btnFrBusinessDay";
            this.btnFrBusinessDay.Size = new System.Drawing.Size(80, 23);
            this.btnFrBusinessDay.TabIndex = 18;
            this.btnFrBusinessDay.Text = "해외영업일";
            this.btnFrBusinessDay.UseVisualStyleBackColor = true;
            this.btnFrBusinessDay.Click += new System.EventHandler(this.btnFrBusinessDay_Click);
            // 
            // btnGetSystemTp
            // 
            this.btnGetSystemTp.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnGetSystemTp.Location = new System.Drawing.Point(511, 288);
            this.btnGetSystemTp.Name = "btnGetSystemTp";
            this.btnGetSystemTp.Size = new System.Drawing.Size(114, 23);
            this.btnGetSystemTp.TabIndex = 19;
            this.btnGetSystemTp.Text = "시스템구분(F/H)";
            this.btnGetSystemTp.UseVisualStyleBackColor = true;
            this.btnGetSystemTp.Click += new System.EventHandler(this.btnGetSystemTp_Click);
            // 
            // btnGetForeignMst
            // 
            this.btnGetForeignMst.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnGetForeignMst.Location = new System.Drawing.Point(583, 225);
            this.btnGetForeignMst.Name = "btnGetForeignMst";
            this.btnGetForeignMst.Size = new System.Drawing.Size(158, 23);
            this.btnGetForeignMst.TabIndex = 20;
            this.btnGetForeignMst.Text = "해외_모든MST파일수신";
            this.btnGetForeignMst.UseVisualStyleBackColor = true;
            this.btnGetForeignMst.Click += new System.EventHandler(this.btnGetForeignMst_Click);
            // 
            // btnGetForeignMst2
            // 
            this.btnGetForeignMst2.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnGetForeignMst2.Location = new System.Drawing.Point(583, 245);
            this.btnGetForeignMst2.Name = "btnGetForeignMst2";
            this.btnGetForeignMst2.Size = new System.Drawing.Size(158, 23);
            this.btnGetForeignMst2.TabIndex = 21;
            this.btnGetForeignMst2.Text = "해외_선물MST파일수신";
            this.btnGetForeignMst2.UseVisualStyleBackColor = true;
            this.btnGetForeignMst2.Click += new System.EventHandler(this.btnGetForeignMst2_Click_1);
            // 
            // btnGetForeignMst3
            // 
            this.btnGetForeignMst3.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnGetForeignMst3.Location = new System.Drawing.Point(583, 266);
            this.btnGetForeignMst3.Name = "btnGetForeignMst3";
            this.btnGetForeignMst3.Size = new System.Drawing.Size(158, 23);
            this.btnGetForeignMst3.TabIndex = 22;
            this.btnGetForeignMst3.Text = "해외_옵션MST파일수신";
            this.btnGetForeignMst3.UseVisualStyleBackColor = true;
            this.btnGetForeignMst3.Click += new System.EventHandler(this.btnGetForeignMst3_Click_1);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(753, 598);
            this.Controls.Add(this.btnGetForeignMst3);
            this.Controls.Add(this.btnGetForeignMst2);
            this.Controls.Add(this.btnGetForeignMst);
            this.Controls.Add(this.btnGetSystemTp);
            this.Controls.Add(this.btnFrBusinessDay);
            this.Controls.Add(this.btnBusinessDay);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.lstLog);
            this.Controls.Add(this.m_CommAgent);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.m_CommAgent)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Button btnDisConnect;
        private System.Windows.Forms.Button btnLogout;
        private System.Windows.Forms.Button btnLogin;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtUserID;
        private System.Windows.Forms.MaskedTextBox mtxtUserPwd;
        private System.Windows.Forms.MaskedTextBox mtxtUserCertPwd;
        private System.Windows.Forms.ListBox lstLog;

        public string m_sUserId;
        public string m_sUserPw;
        public string m_sAuthPw;
        public string m_sCode;
        public string m_strNextKey;

        private string m_strOrdNo;

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnQuery;
        private System.Windows.Forms.TextBox txtFidInput2;
        private System.Windows.Forms.TextBox txtFidInput1;
        private System.Windows.Forms.TextBox txtTrInput;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox txtFidCode;
        private System.Windows.Forms.TextBox txtTrCode;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.RadioButton rdFX;
        private System.Windows.Forms.RadioButton rdForeign;
        private System.Windows.Forms.RadioButton rdDomestic;
        private System.Windows.Forms.RadioButton rdFID;
        private System.Windows.Forms.RadioButton rdTR;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox txtRealType;
        private System.Windows.Forms.TextBox txtRealInput;
        private System.Windows.Forms.Button btnRealUnReg;
        private System.Windows.Forms.Button btnRealReg;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox txtAccNo;
        private System.Windows.Forms.Button btnJumunCheUnReg;
        private System.Windows.Forms.Button btnJumunCheReg;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.TextBox txtJumunInput;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox txtJumunCode;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Button btnJumun;
        private System.Windows.Forms.Button btnBusinessDay;
        private System.Windows.Forms.Button btnFrBusinessDay;
        private System.Windows.Forms.Button btnGetSystemTp;
        protected AxHDFCommAgentLib.AxHDFCommAgent m_CommAgent;
        private System.Windows.Forms.Button btnGetForeignMst;
        private System.Windows.Forms.Button btnGetForeignMst2;
        private System.Windows.Forms.Button btnGetForeignMst3;
    }
}

