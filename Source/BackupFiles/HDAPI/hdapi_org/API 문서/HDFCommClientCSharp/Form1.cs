using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Globalization;

namespace HDFCommClientCSharp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            rdDomestic.Checked = true;
            rdTR.Checked = true;
        }

        // 계좌
        const string DEF_ACCT_INFO = "g11004.AQ0101%";

        // 국내
        const string DEF_TR_CODE = "g11002.DQ0622&";
        const string DEF_FID_CODE = "s20001";
        const string DEF_ORD_CODE_NEW = "g12001.DO1601&";
        const string DEF_ORD_CODE_MOD = "g12001.DO1901&";
        const string DEF_ORD_CODE_CNL = "g12001.DO1701&";

        const string DEF_CHE_LIST = "g11002.DQ0107&";
        const string DEF_MICHE_LIST = "g11002.DQ0104&";

        // 해외
        const string DEF_HW_JANGO = "g11004.AQ0605%";
        const string DEF_HW_FID_CODE = "o51000";
        const string DEF_HW_ORD_CODE_NEW = "g12003.AO0401%";
        const string DEF_HW_MSTINFO = "o51211";

        // FX마진	//@lhe 2012.06.22
        const string DEF_FX_JANGO = "g11004.AQ0901%";		// 자산내역
        const string DEF_FX_FID_CODE = "x00001";				// FX마스터
        const string DEF_FX_ORD_CODE_NEW = "g12003.AO0501%";	// 예약,시장가,청산주문
        const string DEF_FX_ORD_CODE_MOD = "g12003.AO0502&";	// 정정
        const string DEF_FX_ORD_CODE_CNL = "g12003.AO0503&";	// 취소주문




        private void btnConnect_Click(object sender, EventArgs e)
        {
            //// 통신을 초기화 한다.
            //if (!axHDFCommAgent1.GetSafeHwnd())
            //{
            //    axHDFCommAgent1.Create("HDF CommAgent", WS_CHILD, CRect(0, 0, 0, 0), this, 2286);
            //}

            //if (axHDFCommAgent1.GetSafeHwnd())
            //{
            
            int nRet = m_CommAgent.CommInit(1);

            if (nRet < 0)
            {
                WriteLog("통신프로그램 실행 오류");
            }
            else
            {
                WriteLog("통신프로그램 실행 성공");
            }
                
            //}
        }

        private void btnDisConnect_Click(object sender, EventArgs e)
        {
            //// 종료시 통신을 해제한다.
            //if (axHDFCommAgent1.GetSafeHwnd())
            //{
                if (m_CommAgent.CommGetConnectState() == 1)
                {
                    if (m_sUserId != "")
                        m_CommAgent.CommLogout(m_sUserId);
                }
            
                m_CommAgent.CommTerminate(1);
                //axHDFCommAgent1.CommTerminate(true);
                //axHDFCommAgent1.DestroyWindow();
            //}

            WriteLog("통신프로그램 종료 성공");
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            // 로그인한다.
            m_sUserId = txtUserID.Text;
            m_sUserPw = mtxtUserPwd.Text;
            m_sAuthPw = mtxtUserCertPwd.Text;

            int nRet = m_CommAgent.CommLogin(m_sUserId, m_sUserPw, m_sAuthPw);
            if (nRet > 0)
            {
                WriteLog("로그인 성공");

                // 로긴후 반드시 호출...
                //m_CommAgent.CommAccInfo();
            }
            else
            {
                string strRet = string.Format("[{0}]", nRet);
                WriteLog("로그인 실패" + strRet);
            }
        }

        private void btnLogout_Click(object sender, EventArgs e)
        {
            // 로그아웃한다.
            int nRet = m_CommAgent.CommLogout(m_sUserId);

            string strRet = string.Format("[{0}]", nRet);
            if (nRet < 0)
            {
                WriteLog("로그아웃 실패" + strRet);
            }
            else
            {
                WriteLog("로그아웃 성공" + strRet);
            }
        }

        public void WriteLog(String strMsg)
        {
            string str;
            str = DateTime.Now.ToString("[tt hh:mm:ss.fff]", new CultureInfo("en-US")) + " [" + strMsg + "]";
            lstLog.Items.Add(str);
            lstLog.SetSelected(lstLog.Items.Count - 1, true);
        }

        private void rdTR_CheckedChanged(object sender, EventArgs e)
        {
            txtTrCode.Enabled = true;
            txtTrInput.Enabled = true;
            txtFidCode.Enabled = false;
            txtFidInput1.Enabled = false;
            txtFidInput2.Enabled = false;
        }

        private void rdFID_CheckedChanged(object sender, EventArgs e)
        {
            txtTrCode.Enabled = false;
            txtTrInput.Enabled = false;
            txtFidCode.Enabled = true;
            txtFidInput1.Enabled = true;
            txtFidInput2.Enabled = true;
        }

        private void rdDomestic_CheckedChanged(object sender, EventArgs e)
        {
            // TR 조회
            txtTrCode.Text = DEF_TR_CODE;
            txtTrInput.Text = "101F";

            // FID 조회
            txtFidCode.Text = DEF_FID_CODE;
            txtFidInput1.Text = "101G300020001";
            txtFidInput2.Text = "000075051057"; //종목코드+호가시간+종가+누적거래량

            // 실시간 선물/옵션 체결/호가 등록
            txtRealInput.Text = "101G3000";
            txtRealType.Text = "65";

            txtAccNo.Text = "05359001   ";

            txtJumunCode.Text = DEF_ORD_CODE_NEW;
            txtJumunInput.Text = "04001001   1234    201JB245                        111110           1";
        }

        private void rdForeign_CheckedChanged(object sender, EventArgs e)
        {
            // TR 조회
            txtTrCode.Text = DEF_HW_JANGO;
            txtTrInput.Text = "0112278520    ";

            // FID조회
            string strSeries = "CLK16                           ";
            txtFidCode.Text = DEF_HW_FID_CODE;
            txtFidInput1.Text = strSeries;
            txtFidInput2.Text = "000028002017";//종목코드+호가시간+종가+누적거래량

            // 실시간 선물/옵션 체결/호가 등록
            txtRealInput.Text = strSeries;
            txtRealType.Text = "82";

            txtAccNo.Text = "011227     ";

            txtJumunCode.Text = DEF_HW_ORD_CODE_NEW;
            txtJumunInput.Text = "0119871234    CLK16                           1200              1         1                                  ";
        }

        private void rdFX_CheckedChanged(object sender, EventArgs e)
        {
            // TR 조회
            txtTrCode.Text = DEF_FX_JANGO;
            txtTrInput.Text = "1sivas99 0119861234                        ";

            // FID조회
            string strSeries = "EUR/USD   ";
            txtFidCode.Text = DEF_FX_FID_CODE;
            txtFidInput1.Text = strSeries;
            txtFidInput2.Text = "000001002010";//종목코드+매도가격+매수가격+호가시간

            // 실시간 선물/옵션 체결/호가 등록
            txtRealInput.Text = strSeries;
            txtRealType.Text = "171";

            txtAccNo.Text = "011986     ";

            txtJumunCode.Text = DEF_FX_ORD_CODE_NEW;
            txtJumunInput.Text = "0119861234    EUR/USD                         11.24633             1              CEO";	//예약주문
        }

        private void btnQuery_Click(object sender, EventArgs e)
        {
            int nResult;

	        // 조회성 데이터를 요청한다.
	        if (rdTR.Checked)
	        {
		        m_sCode = txtTrCode.Text;

		        nResult = m_CommAgent.CommGetConnectState();
		        if ( nResult == 1 )
		        {
			        if ( m_sCode == "g11002.DQ0104&" || m_sCode == "g11002.DQ0107&" )
			        {
				        //string strAcctNo(txtTrInput.Text, 11);
                        string strAcctNo = GetStringByByte(txtTrInput.Text, 0, 11);
				        string strDealNo = m_CommAgent.CommGetDealNo(strAcctNo);
                        string strAcctPw = GetStringByByte(txtTrInput.Text, strAcctNo.Length + strDealNo.Length, 8);
				
				        txtTrInput.Text = strAcctNo + strDealNo + strAcctPw;
			        }
			        nResult = m_CommAgent.CommRqData(m_sCode, txtTrInput.Text, txtTrInput.Text.Length, m_strNextKey);
			        m_strNextKey = "";
		        }
	        }
	        else
	        {
		        m_sCode = txtFidCode.Text;

		        nResult = m_CommAgent.CommFIDRqData(m_sCode, txtFidInput1.Text, txtFidInput2.Text, txtFidInput1.Text.Length, m_strNextKey);
		        m_strNextKey = "";
	        }

            string strMsg = string.Format("요청한 RQID[{0}]", nResult );
	        WriteLog(strMsg);
        }

        // 실시간 등록
        // 국내 : 51:선물호가, 51:옵션호가, 65:선물체결, 66:옵션체결
        // 해외 : 76:선물호가, 82:선물체결
        private void btnRealReg_Click(object sender, EventArgs e)
        {
            int nResult;
            int nRealType = Int32.Parse(txtRealType.Text);

            // 리얼(실시간) 데이타를 요청한다.
            nResult = m_CommAgent.CommSetBroad(txtRealInput.Text, nRealType);

            string strMsg = string.Format("실시간 종목[{0}] 구분[{1}] 등록결과[{2}]", txtRealInput.Text, txtRealType.Text, nResult);
            WriteLog(strMsg);
        }

        private void btnRealUnReg_Click(object sender, EventArgs e)
        {

            int nRealType = Int32.Parse(txtRealType.Text);

            // 실시간 데이터 수신을 해제한다.
            m_CommAgent.CommRemoveBroad(txtRealInput.Text, nRealType);

            // 모든 실시간 데이터 수신을 해제는 종목을 "" 로
            //m_CommAgent.CommRemoveBroad("", nRealType);
        }

        // 주문체결 등록
        private void btnJumunCheReg_Click(object sender, EventArgs e)
        {
            //등록
            m_CommAgent.CommSetJumunChe(txtUserID.Text, txtAccNo.Text);
            WriteLog("실시간 주문체결/미체결 등록");
        }

        // 주문체결 해제
        private void btnJumunCheUnReg_Click(object sender, EventArgs e)
        {
            //해제
            m_CommAgent.CommRemoveJumunChe(txtUserID.Text, txtAccNo.Text);
            WriteLog("실시간 주문체결/미체결 해지");
        }

        // 주문 전송
        private void btnJumun_Click(object sender, EventArgs e)
        {
            int nResult;

            m_sCode = txtJumunCode.Text;

            nResult = m_CommAgent.CommJumunSvr(m_sCode, txtJumunInput.Text);
            string strMsg = string.Format("주문요청 요청번호[{0}]", nResult);
            WriteLog(strMsg);
        }

        private void btnBusinessDay_Click(object sender, EventArgs e)
        {
            WriteLog(m_CommAgent.CommGetBusinessDay(3));
        }

        private void btnFrBusinessDay_Click(object sender, EventArgs e)
        {
            WriteLog(m_CommAgent.CommGetBusinessDay(1));
        }

        private void btnGetSystemTp_Click(object sender, EventArgs e)
        {
            WriteLog(m_CommAgent.CommGetInfo("systemtp", "1"));
        }

        private void m_CommAgent_OnDataRecv(object sender, AxHDFCommAgentLib._DHDFCommAgentEvents_OnDataRecvEvent e)
        {
            if (e.sTrCode == DEF_TR_CODE)
            {
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");
                if (nRepeatCnt > 0)
                {
                    for (int i = 0; i < nRepeatCnt; i++)
                    {
                        string sData = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "종목코드");

                        WriteLog(sData);
                    }

                    // CommGetDataDirect 예제 추가. - 20140416 sivas
                    string strBuff = m_CommAgent.CommGetDataDirect(e.sTrCode, -1, 0, nRepeatCnt * 32, 0, "A");
                    WriteLog(strBuff);
                }
            }
            else if (e.sTrCode == DEF_ORD_CODE_NEW)
            {
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");

                string strExchTp = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "접수구분");
                string strProcTp = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "처리구분");
                string strAcctNo = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "계좌번호");
                string strOrdNo = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "주문번호");
                string strSeries = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "종목코드");

                string strMsg = string.Format("주문응답 번호[{0}][{1}]처리[{2}]계좌번호[{3}]주문번호[{4}]", e.nRqId, strExchTp, strProcTp, strAcctNo, strOrdNo);
                WriteLog(strMsg);
            }
            else if (e.sTrCode == DEF_FID_CODE)
            {
                // FID값 parsing
                //종목코드(8)+호가시간(6)+종가(9)+누적거래량(7)
                string strSeries = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "단축코드");
                string strTime = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "호가수신시간");
                string strCloseP = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "현재가");
                string strVolume = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "누적거래량");

                string strLastP = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "최종결제가");
                string strDate = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "상장일");
                string strHighP = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "고가");

                string strMsg = string.Format("RQID[{0}] FID 종목[{1}]시간[{2}]현재가[{3}]거래량[{4}]", e.nRqId, strSeries, strTime, strCloseP, strVolume);
                WriteLog(strMsg);

                //AfxMessageBox(strLastP + "[" + strHighP + "]");
            }
            else if (e.sTrCode == DEF_CHE_LIST)
            {
                string strMsg = string.Format("국내 체결내역 : 조회[{0}]", DEF_CHE_LIST);
                WriteLog(strMsg);

                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");
                for (int i = 0; i < nRepeatCnt; i++)
                {
                    string strAcctNm = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "계좌번호");
                    string strEntrTot = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "주문번호");
                    string strTrstMgn = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "종목코드");
                    string strMntMgn = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "매매구분");

                    strAcctNm = strAcctNm.TrimEnd();
                    strTrstMgn = strTrstMgn.TrimEnd();
                    strMsg = string.Format("계좌번호[{0}] 주문번호[{1}] 종목[{2}] 매매구분[{3}]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
                    WriteLog(strMsg);
                }
                strMsg = string.Format("체결수량[{0}]", nRepeatCnt);
                WriteLog(strMsg);
            }
            else if (e.sTrCode == DEF_MICHE_LIST)
            {
                string strMsg = string.Format("국내 미체결내역 : 조회[{0}]", DEF_MICHE_LIST);
                WriteLog(strMsg);

                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");
                for (int i = 0; i < nRepeatCnt; i++)
                {
                    string strAcctNm = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "계좌번호");
                    string strEntrTot = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "주문번호");
                    string strTrstMgn = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "종목코드");
                    string strMntMgn = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "매매구분");

                    strAcctNm = strAcctNm.TrimEnd();
                    strTrstMgn = strTrstMgn.TrimEnd();
                    strMsg = string.Format("계좌번호[{0}] 주문번호[{1}] 종목[{2}] 매매구분[{3}]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
                    WriteLog(strMsg);
                }
                strMsg = string.Format("미체결수량[{0}]", nRepeatCnt);
                WriteLog(strMsg);
            }
            else if (e.sTrCode == DEF_HW_JANGO)
            {
                string strMsg = string.Format("해외 예탁자산 및 증거금 응답[{0}]", DEF_HW_JANGO);
                WriteLog(strMsg);

                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");
                for (int i = 0; i < nRepeatCnt; i++)
                {
                    string strAcctNm = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "통화구분");
                    string strEntrTot = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "예탁금총액");
                    string strEntrCh = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "예탁금잔액");

                    strAcctNm.TrimEnd();
                    strMsg = string.Format("통화구분[{0}] 예탁금총액[{1}] 예탁금잔액[{2}]", strAcctNm, strEntrTot, strEntrCh);
                    WriteLog(strMsg);
                }

            }
            else if (e.sTrCode == DEF_HW_FID_CODE)
            {
                string strMsg = string.Format("해외 현재가시세응답[{0}]", DEF_HW_FID_CODE);
                WriteLog(strMsg);

                string strSeries = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "종목코드");
                string strTime = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "한글종목명");
                string strCloseP = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "현재가");
                string strVolume = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "누적거래량");

                //m_edSeriesO.SetWindowText(strSeries);
                //m_edTimeO.SetWindowText(strTime);
                string strFormatPrice = strCloseP;
                strFormatPrice = m_CommAgent.CommGetHWOrdPrice(strSeries, strCloseP, 0);
                //m_edClosePO.SetWindowText(strFormatPrice);
                //m_edVolumeO.SetWindowText(strVolume);

                strMsg = string.Format("FID 종목[{0}]시간[{1}]현재가[{2}]변환가[{3}]거래량[{4}]", strSeries, strTime, strCloseP, strFormatPrice, strVolume);
                WriteLog(strMsg);

                string strType1, strType2, strType3, strType4;
                //strCloseP = strCloseP;//"1241300";
                strType1 = m_CommAgent.CommGetHWOrdPrice(strSeries, strCloseP, 0);
                strType2 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType1, 1);
                strType3 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType2, 2);
                strType4 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType3, 0);
                strMsg = string.Format("FID Recv[{0}]->[{1}]->[{2}]->[{3}]->[{4}]", strCloseP, strType1, strType2, strType3, strType4);

                WriteLog(strMsg);
                strSeries = strSeries.TrimEnd();

                MessageBox.Show(m_CommAgent.CommGetHWInfo(strSeries, 7));
                strSeries = "종목";
                strMsg = string.Format("{0}", strSeries.Length);
                string strOut = m_CommAgent.CommGetDataDirect("o51000", -1, 60, 10, 0, "A");
                MessageBox.Show(strOut);
            }
            else if (e.sTrCode == DEF_HW_ORD_CODE_NEW)
            {
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");

                string strExchTp = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "접수구분");
                string strProcTp = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "처리코드");
                string strAcctNo = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "처리메시지");
                string strOrdNo = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "주문번호");

                string strMsg = string.Format("주문응답 번호[{0}][{1}]처리[{2}]계좌번호[{3}]주문번호[{4}]", e.nRqId, strExchTp, strProcTp, strAcctNo, strOrdNo);
                WriteLog(strMsg);
            }
            else if (e.sTrCode == DEF_HW_MSTINFO)
            {
                // OCX에서 /mst/JMCODE.cod 파일을 생성시킨다.
                WriteLog("해외 종목 서비스 요청 완료!!!");
            }
            else if (e.sTrCode == DEF_FX_JANGO)
            {
                string strMsg = string.Format("FX 계좌정보 : 자산내역조회[{0}]", DEF_FX_JANGO);
                WriteLog(strMsg);

                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");
                for (int i = 0; i < nRepeatCnt; i++)
                {
                    string strAcctNm = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "계좌명");
                    string strEntrTot = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "예탁총액");
                    string strTrstMgn = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "위탁증거금");
                    string strMntMgn = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "유지증거금");

                    strAcctNm = strAcctNm.TrimEnd();
                    strMsg = string.Format("계좌명[{0}] 예탁총액[{1}] 위탁증거금[{2}] 유지증거금[{3}]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
                    WriteLog(strMsg);
                }
            }
            else if (e.sTrCode == DEF_FX_FID_CODE)
            {
                string strMsg = string.Format("FX 마스터데이타[{0}]", DEF_FX_FID_CODE);
                WriteLog(strMsg);

                string strSeries = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "종목코드");
                string strOffer = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "매도가격");
                string strBid = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "매수가격");
                string strTime = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "호가시간");

                strMsg = string.Format("FID 종목[{0}]시간[{1}]매도가격[{2}]매수가격[{3}]", strSeries, strTime, strOffer, strBid);
                WriteLog(strMsg);
            }
            else if (e.sTrCode == DEF_FX_ORD_CODE_NEW)
            {
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");

                string strExchTp = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "접수구분");
                string strProcTp = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "처리코드");
                string strAcctNo = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "처리메시지");
                string strOrdNo = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "주문번호");

                string strMsg;
                strMsg = string.Format("주문응답 번호[{0}][{1}]처리[{2}]계좌번호[{3}]주문번호[{4}]", e.nRqId, strExchTp, strProcTp, strAcctNo, strOrdNo);
                WriteLog(strMsg);
            }
            else if (e.sTrCode == "o44005")
            {
                // 해외 차트 조회
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec2");

                string strMsg = string.Format("응답개수[{0}]", nRepeatCnt);
                WriteLog(strMsg);

                for (int i = 0; i < nRepeatCnt; i++)
                {
                    string sDate = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec2", i, "일자");
                    string sTime = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec2", i, "시간");
                    string sCloseP = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec2", i, "종가");
                    string sVolume = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec2", i, "체결량");

                    strMsg = string.Format("[{0}][{1}][{2}][{3}]", sDate, sTime, sCloseP, sVolume);
                    WriteLog(strMsg);
                }
            }
            // 20140328 계좌정보 수신 부분 추가. - 20140328 sivas
	        else if ( e.sTrCode == DEF_ACCT_INFO )
	        {
		        string strMsg = "계좌정보 수신완료!!!";
		        WriteLog(strMsg);

		        string strData = m_CommAgent.CommGetAccInfo();

                int nCount = Int32.Parse(strData.Substring(0,5));    // Count;
                strData = strData.Substring(5);

                for(int i=0; i<nCount; i++)
                {
                    string strAcctNo = GetStringByByte(strData, 0, 11); strData = GetStringByByte(strData, 11, -1);
                    string strAcctNm = GetStringByByte(strData, 0, 30); strData = GetStringByByte(strData, 30, -1);
                    string strAcctGb = GetStringByByte(strData, 0, 01); strData = GetStringByByte(strData, 01, -1);

                    strMsg = string.Format("[{0}][{1}][{2}]", strAcctNo, strAcctNm, strAcctGb);
			        WriteLog(strMsg);
                }
	        }
            else if ( e.sTrCode == "l41601" )
	        {
		        // input = "1012014051620020"
		        // req = "000001002003004005006007008009"
		        int nRepeatCnt = 20;	// 조회시 요청한 개수
		        if ( nRepeatCnt > 0 )
		        {
			        //typedef struct 
			        //{
				    //    char		baseid[8];		/*  [기초자산ID]기초자산ID     */
				    //    char		date[8];		/*  [일자]일자     */
				    //    char		price[6];		/*  [현재가]현재가     */
				    //    char		sign[1];		/*  [대비구분]대비구분     */
				    //    char		change[6];		/*  [대비]대비     */
				    //    char		open[6];		/*  [시가]시가     */
				    //    char		high[6];		/*  [고가]고가     */
				    //    char		low[6];			/*  [저가]저가     */
				    //    char		volume[15];		/*  [누적거래량]누적거래량     */
				    //    char		jnilclose[6];	/*  [전일종가]     */
                    //
			        //}HDF_I41601;

                    int nBuffSize = 62;   //위의 주석처리된 구조체의 크기.
                    
			        string strBuff = m_CommAgent.CommGetDataDirect(e.sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
                    
			        // Struct만큼 잘라서 사용하세요...
                    
			        WriteLog(strBuff);
		        }
	        }

            else if (e.sTrCode == "g11004.AQ0207%")
            {
                string strProcTp = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "처리코드");
                string strProMsg = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "처리메시지");
                string strAcctNo = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "계좌번호");
                string strAcctNm = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "계좌명");
                string strTp = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "구분");
                string strYN = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "일치여부");

                string strBuff = string.Format("[{0}][{1}][{2}][{3}][{4}][{5}]", strProcTp, strProMsg, strAcctNo, strAcctNm, strTp, strYN);

                WriteLog(strBuff);
            }
            else if (e.sTrCode == "o51100")
            {
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");

                for (int i = 0; i < nRepeatCnt; i++)
                {
                    string sDate = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "종목코드");
                    string sTime = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "종목명");
                    string sCloseP = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "현재가");
                    string sVolume = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "전일대비구분");

                    string strMsg = string.Format("[{0}][{1}][{2}][{3}]", sDate, sTime, sCloseP, sVolume);
                    WriteLog(strMsg);
                }
            }
            else if (e.sTrCode == "o51200")
            {
                string strMsg;
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");

                for (int i = 0; i < nRepeatCnt; i++)
                {
                    string sDate = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "일자");
                    string sTime = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "시간");
                    string sCloseP = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "종가");
                    string sVolume = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "체결량");

                    strMsg = string.Format("[{0}][{1}][{2}][{3}]", sDate, sTime, sCloseP, sVolume);
                    WriteLog(strMsg);
                }

                int nBuffSize = 129 + 16;	// Fid조회는 필드 마지막에 구분자 1자리가 있으므로 각 필드 만큼 더해준다.
                string strBuff = m_CommAgent.CommGetDataDirect(e.sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
                strMsg = string.Format("[{0}]", strBuff);
                WriteLog(strMsg);

                string strNextKey = m_CommAgent.CommGetNextKey(e.nRqId, "");
                if (strNextKey != "")
                {
                    DialogResult nRet = MessageBox.Show("조회를 계속 할까요?", "현대선물", MessageBoxButtons.YesNo);
                    if (nRet == DialogResult.Yes)
                    {
                        m_strNextKey = strNextKey;
                        btnQuery_Click(null, null); //동작하려나?? anigate.
                    }
                }
            }
            else if (e.sTrCode == "g11004.AQ0401%")
            {
                string strMsg;
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");

                for (int i = 0; i < nRepeatCnt; i++)
                {
                    string sDate = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "주문번호");
                    string sTime = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "계좌번호");
                    string sCloseP = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "종목코드");
                    string sVolume = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "주문가격");

                    strMsg = string.Format("[{0}][{1}][{2}][{3}]", sDate, sTime, sCloseP, sVolume);
                    WriteLog(strMsg);
                }

                int nBuffSize = 226;
                string strBuff = m_CommAgent.CommGetDataDirect(e.sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
                strMsg = string.Format("[{0}]", strBuff);
                WriteLog(strMsg);
            }
            else if (e.sTrCode == "g11004.AQ0408%")
            {
                string strMsg;
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");

                for (int i = 0; i < nRepeatCnt; i++)
                {
                    string sData01 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "처리일자");
                    string sData02 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "주문번호");
                    string sData03 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "주문/체결구분");
                    string sData04 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "종목코드");
                    string sData05 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "매매구분");
                    string sData06 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "주문수량");
                    string sData07 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "정정수량");
                    string sData08 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "취소수량");
                    string sData09 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "체결수량");
                    string sData10 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "주문잔량");
                    string sData11 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "주문/체결가격");
                    string sData12 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "가격조건");
                    string sData13 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "통신주문구분");
                    string sData14 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "주문전략구분");
                    string sData15 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "거래소접수/거부구분");
                    string sData16 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "거래소접수/거부처리시간");
                    string sData17 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "거래소접수번호/거부코드");
                    string sData18 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "거래소주문상태");
                    string sData19 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "작업일자");
                    string sData20 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "작업시간");
                    string sData21 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "작업사원");
                    string sData22 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "사설IP");
                    string sData23 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "공인IP");
                    string sData24 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "처리일자");
                    string sData25 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "최초원주문번호");
                    string sData26 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", i, "stop가격");


                    strMsg = string.Format("[{0}][{1}][{2}][{3}]", sData01, sData02, sData03, sData04);
                    WriteLog(strMsg);
                }

                int nBuffSize = 226;
                string strBuff = m_CommAgent.CommGetDataDirect(e.sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
                strMsg = string.Format("[{0}]", strBuff);
                WriteLog(strMsg);


                string strNextKey = m_CommAgent.CommGetNextKey(e.nRqId, "");
                if (strNextKey != "")
                {
                    int nRet = (int)MessageBox.Show("조회를 계속 할까요?", "현대선물", MessageBoxButtons.YesNo);
                    if (nRet == (int)DialogResult.Yes)
                    {
                        m_strNextKey = strNextKey;
                        btnQuery_Click(null, null); //동작하려나?? anigate.
                    }
                }
            }
            else if (e.sTrCode == "o44011")	// 서버 현재시간
            {
                string sDate = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "서버일자");
                string sTime = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "서버시간");

                string strMsg = string.Format("서버날짜[{0}]서버시간[{1}]", sDate, sTime);
                WriteLog(strMsg);
            }
            else if (e.sTrCode == "n51003")
            {
                // 공지사항 조회
                string sTemp1 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "제목");

                string strMsg = string.Format("[{0}]", sTemp1);
                WriteLog(strMsg);

                string strNextKey = m_CommAgent.CommGetNextKey(e.nRqId, "");
                if (strNextKey != "")
                {
                    int nRet = (int)MessageBox.Show("n51003 조회를 계속 할까요?", "현대선물", MessageBoxButtons.YesNo);
                    if (nRet == (int)DialogResult.Yes)
                    {
                        m_strNextKey = strNextKey;
                        btnQuery_Click(null, null); //동작하려나?? anigate.
                    }
                }
            }
            else if (e.sTrCode == "g11002.DQ0321&")
            {
                string sTemp1 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "처리코드");
                string sTemp2 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "처리메시지");
                string sTemp3 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "계좌번호");
                string sTemp4 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "일치여부");
                string sTemp5 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "계좌구분");

                string strMsg = string.Format("처리코드[{0}]처리메시지[{1}]계좌번호[{2}]일치여부[{3}]계좌구분[{4}]", sTemp1, sTemp2, sTemp3, sTemp4, sTemp5);
                WriteLog(strMsg);
            }
            else if (e.sTrCode == "g11002.DQ0110&")
            {
                string sTemp1 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "계좌번호");
                string sTemp2 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "계좌명");
                string sTemp3 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "종목");

                string strMsg = string.Format("계좌번호[{0}]계좌명[{1}]종목[{2]]", sTemp1, sTemp2, sTemp3);
                WriteLog(strMsg);
            }
            else if (e.sTrCode == "n51006")
            {
                string strMsg;
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");

                string sLen = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "전체사이즈");

                int nBuffSize = 10 + 1 + 255 + Int32.Parse(sLen);
                string strBuff = m_CommAgent.CommGetDataDirect(e.sTrCode, -1, 0, nBuffSize, 0, "A");
                strMsg = string.Format("[{0}]", strBuff);
                WriteLog(strMsg);
                MessageBox.Show(strMsg);
            }
            else if (e.sTrCode == "l41600")
            {
                /***
                if ( nRepeatCnt > 0 )
                {
                    typedef struct 
                    {
                        char		baseid[8];		//  [기초자산ID]기초자산ID
                        char		date[8];		//  [일자]일자
                        char		price[6];		//  [현재가]현재가
                        char		sign[1];		//  [대비구분]대비구분
                        char		change[6];		//  [대비]대비
                        char		open[6];		//  [시가]시가
                        char		high[6];		//  [고가]고가
                        char		low[6];			//  [저가]저가
                        char		volume[15];		//  [누적거래량]누적거래량
                        char		jnilclose[6];	//  [전일종가]

                    }HDF_I41601;
                    int nBuffSize = sizeof(HDF_I41601);
                    CString strBuff = m_CommAgent.CommGetDataDirect(sTrCode, -1, 0, nRepeatCnt * nBuffSize, 0, "A");
			
                    // Struct만큼 잘라서 사용하세요...
                    WriteLog(strBuff);
                }
                ***/

                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");
                string sLen1 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "품목명");
                string sLen2 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "체결시간");
                string sLen3 = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "현재가");

                WriteLog(sLen1 + sLen2 + sLen3);
            }
            else if (e.sTrCode == "g11002.CQ0101&")
            {
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec1");
                for (int i = 0; i < nRepeatCnt; i++)
                {
                    string strAcctNm = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "사용자ID");
                    string strEntrTot = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "계좌번호");
                    string strTrstMgn = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "대출구분");
                    string strMntMgn = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "TMP1");

                    strAcctNm = strAcctNm.TrimEnd();
                    string strMsg = string.Format("사용자ID[{0}] 계좌번호[{1}] 대출구분[{2}] TMP1[{3}]", strAcctNm, strEntrTot, strTrstMgn, strMntMgn);
                    WriteLog(strMsg);
                }
            }
            else if (e.sTrCode == "o44010")
            {
                // 해외 차트 조회
                int nRepeatCnt = m_CommAgent.CommGetRepeatCnt(e.sTrCode, -1, "OutRec2");

                string sRcvCnt = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec1", 0, "수신봉개수");

                nRepeatCnt = Int32.Parse(sRcvCnt);

                string strMsg = string.Format("응답개수[{0}]", nRepeatCnt);
                WriteLog(strMsg);

                for (int i = 0; i < nRepeatCnt; i++)
                {
                    string sDate = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec2", i, "시작일자");
                    string sTime = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec2", i, "시작시간");
                    string sCloseP = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec2", i, "종가");
                    string sVolume = m_CommAgent.CommGetData(e.sTrCode, -1, "OutRec2", i, "체결량");

                    strMsg = string.Format("[{0}][{1}][{2}][{3}]", sDate, sTime, sCloseP, sVolume);
                    WriteLog(strMsg);
                }
            }


        }

        private void m_CommAgent_OnGetBroadData(object sender, AxHDFCommAgentLib._DHDFCommAgentEvents_OnGetBroadDataEvent e)
        {
            int nRealType = e.nRealType;
            string strKey = e.sJongmokCode;

            switch (nRealType)
            {
                case 51:	// 선물호가
                    {
                        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
                        WriteLog(strSeries);
                    }
                    break;
                case 52:	// 옵션호가
                    {

                    }
                    break;
                case 58: //상품선물호가실시간
                    {
                        string kfutcode = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "kfutcode");
                        string hotime = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "hotime");
                        string offerho1 = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "offerho1");
                        string bidho1 = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "bidho1");

                        kfutcode = kfutcode.TrimEnd();
                        hotime = hotime.TrimEnd();
                        offerho1 = offerho1.TrimEnd();
                        bidho1 = bidho1.TrimEnd();

                        string strMsg = string.Format("호가실시간 kfutcode[{0}]hotime[{1}]offerho1[{2}]bidho1[{3}]", kfutcode, hotime, offerho1, bidho1);
                        WriteLog(strMsg);
                    }
                    break;
                case 65:	// 선물체결
                case 71:	// 상품선물체결
                    {
                        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
                        string strTime = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "체결시간");
                        string strCloseP = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "현재가");
                        string strVolume = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "누적거래량");

                        strSeries   = strSeries.TrimEnd();
                        strTime     = strTime.TrimEnd();
                        strCloseP   = strCloseP.TrimEnd();
                        strVolume = strVolume.TrimEnd();

                        string strMsg = string.Format("71실시간 종목코드[{0}]체결시간[{1}]현재가[{2}]누적거래량[{3}]", strSeries, strTime, strCloseP, strVolume);
                        WriteLog(strMsg);
                    }
                    break;
                case 66:	// 옵션체결
                    {

                    }
                    break;
                case 181:	// 주문접수
                    {
                        string strOrdNo = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
                        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
                        string strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
                        string strOrdP = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문가격");
                        string strOrdQ = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문수량");

                        strOrdNo = strOrdNo.TrimEnd();
                        strSeries = strSeries.TrimEnd();
                        strOrdP = strOrdP.TrimEnd();
                        strOrdQ = strOrdQ.TrimEnd();

                        string strMsg = string.Format("주문접수 번호[{0}]종목[{1}]매매[{2}]가격[{3}]수량[{4}]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
                        WriteLog(strMsg);

                        m_strOrdNo = strOrdNo;
                    }
                    break;
                case 182:	// 미체결
                    {
                        string strOrdNo = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
                        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
                        string strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
                        string strOrdP = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문가격");
                        string strOrdQ = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문수량");
                        string strRemQ = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "잔량");

                        strOrdNo = strOrdNo.TrimEnd();
                        strSeries = strSeries.TrimEnd();
                        strOrdP = strOrdP.TrimEnd();
                        strOrdQ = strOrdQ.TrimEnd();
                        strRemQ = strRemQ.TrimEnd();

                        if (m_strOrdNo == strOrdNo)
                        {
                            string strMsg = string.Format("미체결 주문번호[{0}]종목[{1}]매매[{2}]가격[{3}]수량[{4}]잔량[{5}]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ, strRemQ);
                            WriteLog(strMsg);
                        }
                    }
                    break;
                case 183:	// 미결제
                    {
                        string strMsg = string.Format("리얼번호[{0}] 수신!", nRealType);
                        WriteLog(strMsg);
                    }
                    break;
                case 185:	// 체결
                    {
                        string strOrdNo = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
                        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
                        string strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
                        string strTrdP = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "체결가격");
                        string strTrdQ = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "체결수량");
                        string strTrdTm = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "체결시간");

                        if (m_strOrdNo == strOrdNo)
                        {
                            string strMsg = string.Format("체결 주문번호[{0}]종목[{1}]매매[{2}]가격[{3}]수량[{4}]체결시간[{5}]", strOrdNo, strSeries, strBySlTp, strTrdP, strTrdQ, strTrdTm);
                            WriteLog(strMsg);
                        }
                    }
                    break;
                case 75 :	//해외호가
			        {

			        }
			        break;
		        case 82 :	//해외체결
			        {
				        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
				        string strTime   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "기준체결시간");
				        string strCloseP = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "체결가");
				        string strVolume = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "누적거래량");

				        string strFormatPrice = strCloseP;
				        strFormatPrice = m_CommAgent.CommGetHWOrdPrice(strSeries, strCloseP, 0);

				        string strType1, strType2, strType3, strType4;
				        //strCloseP = strCloseP;//"1241300";
				        strType1 = m_CommAgent.CommGetHWOrdPrice(strSeries, strCloseP, 0);
				        strType2 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType1, 1);
				        strType3 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType2, 2);
				        strType4 = m_CommAgent.CommGetHWOrdPrice(strSeries, strType3, 0);

				        string strMsg = string.Format("Real 82 Recv[{0}]->[{1}]->[{2}]->[{3}]->[{4}]", strCloseP, strType1, strType2, strType3, strType4);
				        WriteLog(strMsg);

				        string strBuff = m_CommAgent.CommGetDataDirect(strKey, nRealType, 0, 230, 0, "A");
                        string strSeries1 = GetStringByByte(strBuff, 0, 10);
				        strMsg = strSeries1;
				        WriteLog(strMsg);
			        }
			        break;
		        case 196 : //해외주문접수
			        {
				        string strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
				        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
				        string strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
				        string strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문가격");
				        string strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문수량");

                        strOrdNo = strOrdNo.Trim();
                        strSeries = strSeries.Trim();
                        strOrdP = strOrdP.Trim();
                        strOrdQ = strOrdQ.Trim();

				        string strErrCd = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "처리코드");
				        string strErrMsg= m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "처리메세지");
                        strErrCd = strErrCd.Trim();
                        strErrMsg = strErrMsg.Trim();

				        string strMsg = string.Format("[{0}] 주문접수코드[{1}][{2}]", nRealType, strErrCd, strErrMsg);
				        WriteLog(strMsg);

				        strMsg = string.Format("[{0}] 주문접수 번호[{1}]종목[{2}]매매[{3}]가격[{4}]수량[{5}]", nRealType, strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
				        WriteLog(strMsg);

				        m_strOrdNo = strOrdNo;
			        }
			        break;
		        case 186 : //해외미체결내역
			        {
				        string strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
				        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
				        string strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
				        string strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문가격");
				        string strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문수량");

                        strOrdNo = strOrdNo.TrimEnd();
                        strSeries = strSeries.TrimEnd();
                        strOrdP = strOrdP.TrimEnd();
                        strOrdQ = strOrdQ.TrimEnd();

				        if ( m_strOrdNo == strOrdNo )
				        {
					        string strMsg = string.Format("미체결 주문번호{0}]종목[{1}]매매[{2}]가격[{3}]수량[{4}]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
					        //WriteLog(strMsg);
				        }
			        }
			        break;
		        case 187 : //해외미결제내역
			        {

			        }
			        break;
		        case 189 : //해외체결내역
			        {
				        string strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
				        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
				        string strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
				        string strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문가격");
				        string strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문수량");

                        strOrdNo = strOrdNo.TrimEnd();
                        strSeries = strSeries.TrimEnd();
                        strOrdP = strOrdP.TrimEnd();
                        strOrdQ = strOrdQ.TrimEnd();

				        if ( m_strOrdNo == strOrdNo )
				        {
					        string strMsg = string.Format("체결 주문번호[{0}]종목[{1}]매매[{2}]가격[{3}]수량[{4}]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
					        WriteLog(strMsg);
				        }
			        }
			        break;
                case 171 : //FX 시세
			        {
				        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "단축코드");
				        string strTime   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "호가시간");
				        string strOffer = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매도호가");
				        string strBid = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매수호가");

				        string strMsg = string.Format("FX시세  종목[{0}] 호가시간[{1}] 매도호가[{2}]매수호가[{3}]",strSeries, strTime, strOffer, strBid);
				        WriteLog(strMsg);
			        }
			        break;
		        case 191 : //FX미체결내역
			        {
				        string strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
				        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
				        string strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
				        string strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문가격");
				        string strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문수량");

                        strOrdNo = strOrdNo.TrimEnd();
                        strSeries = strSeries.TrimEnd();
                        strOrdP = strOrdP.TrimEnd();
                        strOrdQ = strOrdQ.TrimEnd();

				        if ( m_strOrdNo == strOrdNo )
				        {
					        string strMsg = string.Format("미체결 주문번호[{0}]종목[{1}]매매[{2}]가격[{3}]수량[{4}]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
					        WriteLog(strMsg);
				        }
			        }
			        break;
		        case 192 : //FX미청산내역
			        {
				        string strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
				        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
				        string strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
				        string strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "진입가격");
				        string strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "진입수량");

                        strOrdNo = strOrdNo.TrimEnd();
                        strSeries = strSeries.TrimEnd();
                        strOrdP = strOrdP.TrimEnd();
                        strOrdQ = strOrdQ.TrimEnd();

				        if ( m_strOrdNo == strOrdNo )
				        {
					        string strMsg = string.Format("미청산 주문번호[{0}]종목[{1}]매매[{2}]가격[{3}]수량[{4}]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
					        WriteLog(strMsg);
				        }
			        }
			        break;
		        case 193 : //FX청산내역
			        {
				        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
				        string strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
				        string strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "청산가격");
				        string strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "청산수량");

                        strSeries = strSeries.TrimEnd();
                        strOrdP = strOrdP.TrimEnd();
                        strOrdQ = strOrdQ.TrimEnd();

				        string strMsg = string.Format("청산 종목[{0}]매매[{1}]가격[{2}]수량[{3}]", strSeries, strBySlTp, strOrdP, strOrdQ);
				        WriteLog(strMsg);
			        }
			        break;
		        case 197 : //FX주문접수
			        {
				        string strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
				        string strErrcd = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "처리코드");
				        string strErrMsg = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "처리메세지");

                        strOrdNo = strOrdNo.TrimEnd();
                        strErrcd = strErrcd.TrimEnd();
                        strErrMsg = strErrMsg.TrimEnd();

				        string strMsg = string.Format("주문접수 번호[{0}]처리코드[{1}]처리메시지[{2}]", strOrdNo, strErrcd, strErrMsg);
				        WriteLog(strMsg);

				        m_strOrdNo = strOrdNo;
			        }
			        break;
		        case 101:
			        {
				        string strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "지수");
				        string strErrcd = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "전일비");
				        string strErrMsg = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "등락율");

                        strOrdNo = strOrdNo.TrimEnd();
                        strErrcd = strErrcd.TrimEnd();
                        strErrMsg = strErrMsg.TrimEnd();

				        string strMsg = string.Format("지수[{0}]전일비[{1}]등락율[{2}]", strOrdNo, strErrcd, strErrMsg);
				        WriteLog(strMsg);
			        }
			        break;
		        case 199:
			        {
				        string strOrdNo  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문번호");
				        string strSeries = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "종목코드");
				        string strBySlTp = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "매매구분");
				        string strOrdP   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문가격");
				        string strOrdQ   = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문수량");

                        strOrdNo = strOrdNo.TrimEnd();
                        strSeries = strSeries.TrimEnd();
                        strOrdP = strOrdP.TrimEnd();
                        strOrdQ = strOrdQ.TrimEnd();

				        string strMsg = string.Format("Yes미체결 주문번호[{0}]종목[{1}]매매[{2}]가격[{3}]수량[{4}]", strOrdNo, strSeries, strBySlTp, strOrdP, strOrdQ);
				        WriteLog(strMsg);
			        }
			        break;
		        case 208:
			        {
				        string strBuff = m_CommAgent.CommGetDataDirect("0", nRealType, 0, 15, 0, "A");
				        WriteLog(strBuff);
			        }
			        break;
		        case 184 :	// 국내실시간잔고
		        case 188 :	// 해외실시간잔고
			        {
				        string strAccno  = m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "계좌번호");
				        string strAmt	= m_CommAgent.CommGetData(strKey, nRealType, "OutRec1", 0, "주문가능금액");
				        string strMsg = string.Format("리얼번호[{0}]계좌번호[{1}]주문가능금액[{2}]", nRealType, strAccno, strAmt);
				        WriteLog(strMsg);
			        }
			        break;
		        default:
			        {
				        string strMsg = string.Format("[{0}] 알수없는 실시간 요청값", nRealType);
				        WriteLog(strMsg);
			        }
			        break;
            }
        }

        private void m_CommAgent_OnGetMsg(object sender, AxHDFCommAgentLib._DHDFCommAgentEvents_OnGetMsgEvent e)
        {
            string strLog = string.Format("[{0}][{1}]", e.sCode, e.sMsg);
            WriteLog(strLog);
        }

        private void m_CommAgent_OnGetMsgWithRqId(object sender, AxHDFCommAgentLib._DHDFCommAgentEvents_OnGetMsgWithRqIdEvent e)
        {
            string strLog = string.Format("RQID[{0}][{1}][{2}]", e.nRqId, e.sCode, e.sMsg);
            WriteLog(strLog);
        }

        private string GetStringByByte(string strSrc, int nStartIndex, int nLength)
        {
            Encoding encoding = Encoding.GetEncoding("ks_c_5601-1987");
            byte[] buff = encoding.GetBytes(strSrc);

            if (nLength == -1) //-1로 들어왔을 경우는 끝까지로 한다.
                nLength = buff.Length - nStartIndex;

            return encoding.GetString(buff, nStartIndex, nLength);
        }

        private void btnGetForeignMst_Click(object sender, EventArgs e)
        {
            WriteLog("해외 모든MST파일 정보 서비스 요청");

            //대소문자 구별하지 않음. 내부적으로 변환하여 사용됨 - 20150417 anigate
            /*!!!!중요!!!!*/
            // 전체요청과 단일요청을 함께 사용금지!
            // 혹시라도 사용하게 되면 단일요청이든 전체요청이든 모든 응답값을 수신 후에 호출할 것! - 20150417 anigate
            //m_CommAgent.CommReqMakeCod("aLL");
            //m_CommAgent.CommReqMakeCod("jmCode|cMe");
            //m_CommAgent.CommReqMakeCod("opmCode");

            m_CommAgent.CommReqMakeCod("all", 0);
        }

        private void btnGetForeignMst2_Click_1(object sender, EventArgs e)
        {
            WriteLog("해외 선물MST파일 정보 서비스 요청");

            m_CommAgent.CommReqMakeCod("futures", 0);
        }

        private void btnGetForeignMst3_Click_1(object sender, EventArgs e)
        {
            WriteLog("해외 옵션MST파일 정보 서비스 요청");

            m_CommAgent.CommReqMakeCod("option", 0);
        }
    }
}
