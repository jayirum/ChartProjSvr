// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "XingAPI_Sample.h"

#include "ChildFrm.h"

#include "Dlg_t1101.h"
#include "Dlg_t1301.h"
#include "Dlg_t5511.h"
#include "Dlg_t6011.h"
#include "Dlg_t6024.h"
#include "Dlg_t0431.h"
#include "Dlg_t0433.h"

#include "Dlg_H1_.h"
#include "Dlg_H2_.h"
#include "Dlg_h3_ELW.h"
#include "Dlg_h2_ELW.h"
#include "Dlg_SC0.h"
#include "Dlg_SC1.h"
#include "Dlg_SC2.h"
#include "Dlg_SC3.h"
#include "Dlg_SC4.h"
#include "Dlg_FC0.h"
#include "Dlg_FC0_1.h"
#include "Dlg_OC0.h"
#include "Dlg_BMT.h"
#include "Dlg_BM_.h"
#include "Dlg_t9907.h"
#include "Dlg_t9905.h"
#include "Dlg_t2101.h"

typedef struct
{
	char			szTrCode[ 11];
	CRuntimeClass*	pRuntimeClass;
	UINT			uDlgID;
	char			szDesc	[101];
} TR_WINDOW_TABLE;

static TR_WINDOW_TABLE s_tblTrWindow[] = 
{
// 	{ "t1101", RUNTIME_CLASS(CDlg_t1101), IDD_t1101, "주식현재가호가조회(t1101)" },
// 	{ "t1102", RUNTIME_CLASS(CDlg_t1102), IDD_t1102, "주식현재가(시세)조회(t1102)" },
// 	{ "t1104", RUNTIME_CLASS(CDlg_t1104), IDD_t1104, "주식현재가시세메모(t1104)" },
// 	{ "t1105", RUNTIME_CLASS(CDlg_t1105), IDD_t1105, "주식피못/디마크조회(t1105)" },
// 	{ "t1301", RUNTIME_CLASS(CDlg_t1301), IDD_t1301, "주식시간대별체결조회(t1301)" },
// 	{ "t1302", RUNTIME_CLASS(CDlg_t1302), IDD_t1302, "주식분별주가조회(t1302)" },
// 	{ "t1305", RUNTIME_CLASS(CDlg_t1305), IDD_t1305, "기간별주가(t1305)" },
// 	{ "t1308", RUNTIME_CLASS(CDlg_t1308), IDD_t1308, "주식시간대별체결조회챠트(t1308)" },
// 	{ "t1444", RUNTIME_CLASS(CDlg_t1444), IDD_t1444, "시가총액상위(t1444)" },
// 	{ "t1449", RUNTIME_CLASS(CDlg_t1449), IDD_t1449, "가격대별매매비중조회(t1449)" },
// 	{ "t1452", RUNTIME_CLASS(CDlg_t1452), IDD_t1452, "거래량상위(t1452)" },
// 	{ "t1516", RUNTIME_CLASS(CDlg_t1516), IDD_t1516, "업종별종목시세(t1516)" },
// 	{ "t1532", RUNTIME_CLASS(CDlg_t1532), IDD_t1532, "종목별테마(t1532)" },
// 	{ "t1809", RUNTIME_CLASS(CDlg_t1809), IDD_t1809, "신호조회(t1809)" },
// 	{ "t1537", RUNTIME_CLASS(CDlg_t1537), IDD_t1537, "테마종목별시세조회(t1537)" },
// 	{ "t1701", RUNTIME_CLASS(CDlg_t1701), IDD_t1701, "외인기관종목별동향(t1701)" },
// 	{ "t1901", RUNTIME_CLASS(CDlg_t1901), IDD_t1901, "ETF현재가(시세)조회(t1901)" },
// 	{ "t1902", RUNTIME_CLASS(CDlg_t1902), IDD_t1902, "ETF시간별추이(t1902)" },
// 	{ "t1903", RUNTIME_CLASS(CDlg_t1903), IDD_t1903, "ETF일별추이(t1903)" },
// 	{ "t1904", RUNTIME_CLASS(CDlg_t1904), IDD_t1904, "ETF구성종목조회(t1904)" },
// 	{ "t1486", RUNTIME_CLASS(CDlg_t1486), IDD_t1486, "시간별예상체결가(t1486)" },
// 	{ "t1488", RUNTIME_CLASS(CDlg_t1488), IDD_t1488, "예상체결가등락율상위조회(t1488)" },
// 	{ "t1961", RUNTIME_CLASS(CDlg_t1961), IDD_t1961, "ELW거래량상위(t1961)" },
// 	{ "t1966", RUNTIME_CLASS(CDlg_t1966), IDD_t1966, "ELW거래대금상위(t1966)" },
// 	{ "t5012", RUNTIME_CLASS(CDlg_t5012), IDD_t5012, "거래소 선물대용매도(t5012)" },
// 	{ "t5024", RUNTIME_CLASS(CDlg_t5024), IDD_t5024, "장외단주매도주문(t5024)" },
// 	{ "t5101", RUNTIME_CLASS(CDlg_t5101), IDD_t5101, "현물복수정상주문" },
 	{ "t5511", RUNTIME_CLASS(CDlg_t5511), IDD_t5511, "선옵정상주문(t5511)" },
// 	{ "t5502", RUNTIME_CLASS(CDlg_t5502), IDD_t5502, "현물정정주문(t5502)" },
// 	{ "t5503", RUNTIME_CLASS(CDlg_t5503), IDD_t5503, "현물취소주문(t5503)" },
// 	{ "t0424", RUNTIME_CLASS(CDlg_t0424), IDD_t0424, "주식잔고2(t0424)" },
// 	{ "t0425", RUNTIME_CLASS(CDlg_t0425), IDD_t0425, "주식체결/미체결(t0425)" },
// 	{ "t6005", RUNTIME_CLASS(CDlg_t6005), IDD_t6005, "증거금율별주문가능금액조회(t6005)" },
 	{ "t6011", RUNTIME_CLASS(CDlg_t6011), IDD_t6011, "계좌별예수금조회(t6011)" },
 	{ "t6024", RUNTIME_CLASS(CDlg_t6024), IDD_t6024, "주문가능수량/증거금(t6024)" },
// 	{ "t5026", RUNTIME_CLASS(CDlg_t5026), IDD_t5026, "일괄청산주문/가능여부조회(t5026)" },
 	{ "t2101", RUNTIME_CLASS(CDlg_t2101), IDD_t2101, "선물/옵션현재가(시세)조회(t2101)" },
// 	{ "t2105", RUNTIME_CLASS(CDlg_t2105), IDD_t2105, "선물/옵션현재가호가조회(t2105)" },
// 	{ "t2106", RUNTIME_CLASS(CDlg_t2106), IDD_t2106, "선물/옵션현재가시세메모(t2106)" },
// 	{ "t2201", RUNTIME_CLASS(CDlg_t2201), IDD_t2201, "선물옵션시간대별체결조회(t2201)" },
// 	{ "t2203", RUNTIME_CLASS(CDlg_t2203), IDD_t2203, "기간별주가(t2203)" },
// 	{ "t2209", RUNTIME_CLASS(CDlg_t2209), IDD_t2209, "선물옵션틱분별체결조회챠트(t2209)" },
// 	{ "t2210", RUNTIME_CLASS(CDlg_t2210), IDD_t2210, "선물옵션시간대별체결조회(단일출력용)" },
// 	{ "t2301", RUNTIME_CLASS(CDlg_t2301), IDD_t2301, "옵션전광판(t2301)" },
// 	{ "t2405", RUNTIME_CLASS(CDlg_t2405), IDD_t2405, "선물옵션호가잔량비율챠트(t2405)" },
// 	{ "t2421", RUNTIME_CLASS(CDlg_t2421), IDD_t2421, "미결제약정추이(t2421)" },
// 	{ "t5511", RUNTIME_CLASS(CDlg_t5511), IDD_t5511, "선옵정상주문(t5511)" },
// 	{ "t5512", RUNTIME_CLASS(CDlg_t5512), IDD_t5512, "선옵정정주문(t5512)" },
// 	{ "t5513", RUNTIME_CLASS(CDlg_t5513), IDD_t5513, "선옵취소주문(t5513)" },
// 	{ "t5009", RUNTIME_CLASS(CDlg_t5009), IDD_t5009, "선옵복수계좌복수종목정상주문 " },
// 	{ "t5010", RUNTIME_CLASS(CDlg_t5010), IDD_t5010, "선옵복수계좌복수종목정정주문" },
// 	{ "t5011", RUNTIME_CLASS(CDlg_t5011), IDD_t5011, "선옵복수계좌복수종목취소주문 " },
 	{ "t0431", RUNTIME_CLASS(CDlg_t0431), IDD_t0431, "선물/옵션잔고평가(주문팝업)(t0431)" },
 	{ "t0433", RUNTIME_CLASS(CDlg_t0433), IDD_t0433, "선물/옵션미결제잔고(t0433)" },
// 	{ "t0434", RUNTIME_CLASS(CDlg_t0434), IDD_t0434, "선물/옵션체결/미체결(t0434)" },
// 	{ "t6034", RUNTIME_CLASS(CDlg_t6034), IDD_t6034, "계좌예탁현황(선물/옵션)(t6034)" },
// 	{ "t1511", RUNTIME_CLASS(CDlg_t1511), IDD_t1511, "업종현재가(t1511)" },
// 	{ "t1485", RUNTIME_CLASS(CDlg_t1485), IDD_t1485, "예상지수(t1485)" },
// 	{ "t1601", RUNTIME_CLASS(CDlg_t1601), IDD_t1601, "투자자별종합(t1601)" },
// 	{ "t1602", RUNTIME_CLASS(CDlg_t1602), IDD_t1602, "시간대별투자자매매추이(t1602)" },
// 	{ "t1603", RUNTIME_CLASS(CDlg_t1603), IDD_t1603, "시간대별투자자매매추이상세(t1603)" },
// 	{ "t1615", RUNTIME_CLASS(CDlg_t1615), IDD_t1615, "투자자매매종합1(t1615)" },
// 	{ "t1617", RUNTIME_CLASS(CDlg_t1617), IDD_t1617, "투자자매매종합2(t1617)" },
// 	{ "t1664", RUNTIME_CLASS(CDlg_t1664), IDD_t1664, "투자자매매종합(챠트)" },
// 	{ "t1621", RUNTIME_CLASS(CDlg_t1621), IDD_t1621, "업종별분별투자자매매동향(챠트용)" },
// 	{ "t1631", RUNTIME_CLASS(CDlg_t1631), IDD_t1631, "프로그램매매종합조회(t1631)" },
// 	{ "t1632", RUNTIME_CLASS(CDlg_t1632), IDD_t1632, "시간대별프로그램매매추이(t1632)" },
// 	{ "t1633", RUNTIME_CLASS(CDlg_t1633), IDD_t1633, "기간별프로그램매매추이(t1633)" },
// 	{ "t1640", RUNTIME_CLASS(CDlg_t1640), IDD_t1640, "프로그램매매종합조회(미니)(t1640)" },
// 	{ "t1662", RUNTIME_CLASS(CDlg_t1662), IDD_t1662, "시간대별프로그램매매추이(차트)(t1662)" },
// 	{ "t3102", RUNTIME_CLASS(CDlg_t3102), IDD_t3102, "뉴스본문(t3102)" },
// 	{ "t3202", RUNTIME_CLASS(CDlg_t3202), IDD_t3202, "종목별증시일정(t3202)" },
// 	{ "t3341", RUNTIME_CLASS(CDlg_t3341), IDD_t3341, "재무순위종합(t3341)" },
// 	{ "t3401", RUNTIME_CLASS(CDlg_t3401), IDD_t3401, "투자의견(t3401)" },
// 	{ "t1404", RUNTIME_CLASS(CDlg_t1404), IDD_t1404, "관리/불성실/투자유의조회(t1404)" },
// 	{ "t1405", RUNTIME_CLASS(CDlg_t1405), IDD_t1405, "투자경고/매매정지/정리매매조회(t1405)" },
// 	{ "t4201", RUNTIME_CLASS(CDlg_t4201), IDD_t4201, "주식챠트(종합)(t4201)" },
// 	{ "t4203", RUNTIME_CLASS(CDlg_t4203), IDD_t4203, "업종챠트(종합)(t4203)" },
 	{ "t9905", RUNTIME_CLASS(CDlg_t9905), IDD_t9905, "기초자산리스트조회(t9905)" },
// 	{ "t9907", RUNTIME_CLASS(CDlg_t9907), IDD_t9907, "만기월조회(t9907)" },
// 
// 	{ "S2_"  , RUNTIME_CLASS(CDlg_S2_), IDD_S2_, "KOSPI우선호가" },
// 	{ "S3_"  , RUNTIME_CLASS(CDlg_S3_), IDD_S3_, "KOSPI 체결" },
// 	{ "S4_"  , RUNTIME_CLASS(CDlg_S4_), IDD_S4_, "KOSPI 기세" },
	{ "H1_"  , RUNTIME_CLASS(CDlg_H1_), IDD_H1_, "KOSPI 호가잔량" },
	{ "H2_"  , RUNTIME_CLASS(CDlg_H2_), IDD_H2_, "KOSPI 시간외호가잔량" },
// 	{ "YS3"  , RUNTIME_CLASS(CDlg_YS3), IDD_YS3, "KOSPI 예상체결" },
// 	{ "KS_"  , RUNTIME_CLASS(CDlg_KS_), IDD_KS_, "KOSDAQ 우선호가" },
// 	{ "K3_"  , RUNTIME_CLASS(CDlg_K3_), IDD_K3_, "KOSDAQ 체결" },
// 	{ "HA_"  , RUNTIME_CLASS(CDlg_HA_), IDD_HA_, "KOSDAQ 호가잔량" },
// 	{ "HB_"  , RUNTIME_CLASS(CDlg_HB_), IDD_HB_, "KOSDAQ시간외호가잔량" },
// 	{ "YK3"  , RUNTIME_CLASS(CDlg_YK3), IDD_YK3, "KOSDAQ예상체결" },
// 	{ "s2_"  , RUNTIME_CLASS(CDlg_s2_), IDD_s2_, "ELW 우선호가" },
// 	{ "s3_"  , RUNTIME_CLASS(CDlg_s3_), IDD_s3_, "ELW 체결" },
// 	{ "s4_"  , RUNTIME_CLASS(CDlg_s4_), IDD_s4_, "ELW 기세" },
//	{ "h3_"  , RUNTIME_CLASS(CDlg_h3_ELW), IDD_h3_ELW, "ELW 호가잔량" },
//	{ "h2_"  , RUNTIME_CLASS(CDlg_h2_ELW), IDD_h2_ELW, "ELW 시간외호가잔량" },
// 	{ "TH_"  , RUNTIME_CLASS(CDlg_TH_), IDD_TH_, "OTCBB호가" },
// 	{ "TS_"  , RUNTIME_CLASS(CDlg_TS_), IDD_TS_, "OTCBB체결" },
//  	{ "SC0"  , RUNTIME_CLASS(CDlg_SC0), IDD_SC0, "주식주문접수" },
// 	{ "SC1"  , RUNTIME_CLASS(CDlg_SC1), IDD_SC1, "주식주문체결" },
// 	{ "SC2"  , RUNTIME_CLASS(CDlg_SC2), IDD_SC2, "주식주문정정" },
// 	{ "SC3"  , RUNTIME_CLASS(CDlg_SC3), IDD_SC3, "주식주문취소" },
// 	{ "SC4"  , RUNTIME_CLASS(CDlg_SC4), IDD_SC4, "주식주문거부" },
	{ "FC0"  , RUNTIME_CLASS(CDlg_FC0), IDD_FC0, "선물체결(시세)/호가" },
	{ "FC0_1", RUNTIME_CLASS(CDlg_FC0_1), IDD_FC0_1, "선물체결(해외시세)/호가" },
// 	{ "FH0"  , RUNTIME_CLASS(CDlg_FH0), IDD_FH0, "선물호가" },
// 	{ "OC0"  , RUNTIME_CLASS(CDlg_OC0), IDD_OC0, "옵션체결" },
// 	{ "OH0"  , RUNTIME_CLASS(CDlg_OH0), IDD_OH0, "옵션호가" },
// 	{ "O01"  , RUNTIME_CLASS(CDlg_O01), IDD_O01, "선물주문접수" },
// 	{ "H01"  , RUNTIME_CLASS(CDlg_H01), IDD_H01, "선물주문정정취소" },
// 	{ "C01"  , RUNTIME_CLASS(CDlg_C01), IDD_C01, "선물주문체결" },
// 	{ "IJ_"  , RUNTIME_CLASS(CDlg_IJ_), IDD_IJ_, "지수" },
// 	{ "YJ_"  , RUNTIME_CLASS(CDlg_YJ_), IDD_YJ_, "예상지수" },
// 	{ "PM_"  , RUNTIME_CLASS(CDlg_PM_), IDD_PM_, "KOSPI프로그램매매전체집계" },
// 	{ "KM_"  , RUNTIME_CLASS(CDlg_KM_), IDD_KM_, "KOSDAQ프로그램매매전체집계" },
//	{ "BMT"  , RUNTIME_CLASS(CDlg_BMT), IDD_BMT, "시간대별 투자자매매추이" },
//	{ "BM_"  , RUNTIME_CLASS(CDlg_BM_), IDD_BM_, "업종별 투자자별 매매현황" },
// 	{ "NWS"  , RUNTIME_CLASS(CDlg_NWS), IDD_NWS, "실시간뉴스제목패킷" },
// 	{ "JIF"  , RUNTIME_CLASS(CDlg_JIF), IDD_JIF, "장운영정보" },
};

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
		ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
		ON_WM_SETFOCUS()
		ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
void CChildFrame::OnFileClose() 
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.

	SendMessage(WM_CLOSE);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	return 0;
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	m_wndView.SetFocus();
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	// otherwise, do default handling
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CChildFrame::SetTR( LPCTSTR pszTrCode )
{
	int nCount = sizeof( s_tblTrWindow ) / sizeof( TR_WINDOW_TABLE );
	for( int i=0; i<nCount; i++ )
	{
		if( strcmp( s_tblTrWindow[i].szTrCode, pszTrCode ) == 0 )
		{
			SetWindowText( pszTrCode );
			m_wndView.SetTR( s_tblTrWindow[i].pRuntimeClass, s_tblTrWindow[i].uDlgID );
			return TRUE;
		}
	}

	CString strMsg;
	strMsg.Format( "%s 의 Sample을 찾을 수 없습니다.", pszTrCode );
	
	MessageBox( strMsg, "에러", MB_ICONSTOP );

	SendMessage( WM_CLOSE );
	
	return FALSE;
}