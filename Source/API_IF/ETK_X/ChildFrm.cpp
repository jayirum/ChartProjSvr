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
// 	{ "t1101", RUNTIME_CLASS(CDlg_t1101), IDD_t1101, "�ֽ����簡ȣ����ȸ(t1101)" },
// 	{ "t1102", RUNTIME_CLASS(CDlg_t1102), IDD_t1102, "�ֽ����簡(�ü�)��ȸ(t1102)" },
// 	{ "t1104", RUNTIME_CLASS(CDlg_t1104), IDD_t1104, "�ֽ����簡�ü��޸�(t1104)" },
// 	{ "t1105", RUNTIME_CLASS(CDlg_t1105), IDD_t1105, "�ֽ��Ǹ�/��ũ��ȸ(t1105)" },
// 	{ "t1301", RUNTIME_CLASS(CDlg_t1301), IDD_t1301, "�ֽĽð��뺰ü����ȸ(t1301)" },
// 	{ "t1302", RUNTIME_CLASS(CDlg_t1302), IDD_t1302, "�ֽĺк��ְ���ȸ(t1302)" },
// 	{ "t1305", RUNTIME_CLASS(CDlg_t1305), IDD_t1305, "�Ⱓ���ְ�(t1305)" },
// 	{ "t1308", RUNTIME_CLASS(CDlg_t1308), IDD_t1308, "�ֽĽð��뺰ü����ȸíƮ(t1308)" },
// 	{ "t1444", RUNTIME_CLASS(CDlg_t1444), IDD_t1444, "�ð��Ѿ׻���(t1444)" },
// 	{ "t1449", RUNTIME_CLASS(CDlg_t1449), IDD_t1449, "���ݴ뺰�Ÿź�����ȸ(t1449)" },
// 	{ "t1452", RUNTIME_CLASS(CDlg_t1452), IDD_t1452, "�ŷ�������(t1452)" },
// 	{ "t1516", RUNTIME_CLASS(CDlg_t1516), IDD_t1516, "����������ü�(t1516)" },
// 	{ "t1532", RUNTIME_CLASS(CDlg_t1532), IDD_t1532, "�����׸�(t1532)" },
// 	{ "t1809", RUNTIME_CLASS(CDlg_t1809), IDD_t1809, "��ȣ��ȸ(t1809)" },
// 	{ "t1537", RUNTIME_CLASS(CDlg_t1537), IDD_t1537, "�׸����񺰽ü���ȸ(t1537)" },
// 	{ "t1701", RUNTIME_CLASS(CDlg_t1701), IDD_t1701, "���α�����񺰵���(t1701)" },
// 	{ "t1901", RUNTIME_CLASS(CDlg_t1901), IDD_t1901, "ETF���簡(�ü�)��ȸ(t1901)" },
// 	{ "t1902", RUNTIME_CLASS(CDlg_t1902), IDD_t1902, "ETF�ð�������(t1902)" },
// 	{ "t1903", RUNTIME_CLASS(CDlg_t1903), IDD_t1903, "ETF�Ϻ�����(t1903)" },
// 	{ "t1904", RUNTIME_CLASS(CDlg_t1904), IDD_t1904, "ETF����������ȸ(t1904)" },
// 	{ "t1486", RUNTIME_CLASS(CDlg_t1486), IDD_t1486, "�ð�������ü�ᰡ(t1486)" },
// 	{ "t1488", RUNTIME_CLASS(CDlg_t1488), IDD_t1488, "����ü�ᰡ�����������ȸ(t1488)" },
// 	{ "t1961", RUNTIME_CLASS(CDlg_t1961), IDD_t1961, "ELW�ŷ�������(t1961)" },
// 	{ "t1966", RUNTIME_CLASS(CDlg_t1966), IDD_t1966, "ELW�ŷ���ݻ���(t1966)" },
// 	{ "t5012", RUNTIME_CLASS(CDlg_t5012), IDD_t5012, "�ŷ��� �������ŵ�(t5012)" },
// 	{ "t5024", RUNTIME_CLASS(CDlg_t5024), IDD_t5024, "��ܴ��ָŵ��ֹ�(t5024)" },
// 	{ "t5101", RUNTIME_CLASS(CDlg_t5101), IDD_t5101, "�������������ֹ�" },
 	{ "t5511", RUNTIME_CLASS(CDlg_t5511), IDD_t5511, "���������ֹ�(t5511)" },
// 	{ "t5502", RUNTIME_CLASS(CDlg_t5502), IDD_t5502, "���������ֹ�(t5502)" },
// 	{ "t5503", RUNTIME_CLASS(CDlg_t5503), IDD_t5503, "��������ֹ�(t5503)" },
// 	{ "t0424", RUNTIME_CLASS(CDlg_t0424), IDD_t0424, "�ֽ��ܰ�2(t0424)" },
// 	{ "t0425", RUNTIME_CLASS(CDlg_t0425), IDD_t0425, "�ֽ�ü��/��ü��(t0425)" },
// 	{ "t6005", RUNTIME_CLASS(CDlg_t6005), IDD_t6005, "���ű������ֹ����ɱݾ���ȸ(t6005)" },
 	{ "t6011", RUNTIME_CLASS(CDlg_t6011), IDD_t6011, "���º���������ȸ(t6011)" },
 	{ "t6024", RUNTIME_CLASS(CDlg_t6024), IDD_t6024, "�ֹ����ɼ���/���ű�(t6024)" },
// 	{ "t5026", RUNTIME_CLASS(CDlg_t5026), IDD_t5026, "�ϰ�û���ֹ�/���ɿ�����ȸ(t5026)" },
 	{ "t2101", RUNTIME_CLASS(CDlg_t2101), IDD_t2101, "����/�ɼ����簡(�ü�)��ȸ(t2101)" },
// 	{ "t2105", RUNTIME_CLASS(CDlg_t2105), IDD_t2105, "����/�ɼ����簡ȣ����ȸ(t2105)" },
// 	{ "t2106", RUNTIME_CLASS(CDlg_t2106), IDD_t2106, "����/�ɼ����簡�ü��޸�(t2106)" },
// 	{ "t2201", RUNTIME_CLASS(CDlg_t2201), IDD_t2201, "�����ɼǽð��뺰ü����ȸ(t2201)" },
// 	{ "t2203", RUNTIME_CLASS(CDlg_t2203), IDD_t2203, "�Ⱓ���ְ�(t2203)" },
// 	{ "t2209", RUNTIME_CLASS(CDlg_t2209), IDD_t2209, "�����ɼ�ƽ�к�ü����ȸíƮ(t2209)" },
// 	{ "t2210", RUNTIME_CLASS(CDlg_t2210), IDD_t2210, "�����ɼǽð��뺰ü����ȸ(������¿�)" },
// 	{ "t2301", RUNTIME_CLASS(CDlg_t2301), IDD_t2301, "�ɼ�������(t2301)" },
// 	{ "t2405", RUNTIME_CLASS(CDlg_t2405), IDD_t2405, "�����ɼ�ȣ���ܷ�����íƮ(t2405)" },
// 	{ "t2421", RUNTIME_CLASS(CDlg_t2421), IDD_t2421, "�̰�����������(t2421)" },
// 	{ "t5511", RUNTIME_CLASS(CDlg_t5511), IDD_t5511, "���������ֹ�(t5511)" },
// 	{ "t5512", RUNTIME_CLASS(CDlg_t5512), IDD_t5512, "���������ֹ�(t5512)" },
// 	{ "t5513", RUNTIME_CLASS(CDlg_t5513), IDD_t5513, "��������ֹ�(t5513)" },
// 	{ "t5009", RUNTIME_CLASS(CDlg_t5009), IDD_t5009, "���ɺ������º������������ֹ� " },
// 	{ "t5010", RUNTIME_CLASS(CDlg_t5010), IDD_t5010, "���ɺ������º������������ֹ�" },
// 	{ "t5011", RUNTIME_CLASS(CDlg_t5011), IDD_t5011, "���ɺ������º�����������ֹ� " },
 	{ "t0431", RUNTIME_CLASS(CDlg_t0431), IDD_t0431, "����/�ɼ��ܰ���(�ֹ��˾�)(t0431)" },
 	{ "t0433", RUNTIME_CLASS(CDlg_t0433), IDD_t0433, "����/�ɼǹ̰����ܰ�(t0433)" },
// 	{ "t0434", RUNTIME_CLASS(CDlg_t0434), IDD_t0434, "����/�ɼ�ü��/��ü��(t0434)" },
// 	{ "t6034", RUNTIME_CLASS(CDlg_t6034), IDD_t6034, "���¿�Ź��Ȳ(����/�ɼ�)(t6034)" },
// 	{ "t1511", RUNTIME_CLASS(CDlg_t1511), IDD_t1511, "�������簡(t1511)" },
// 	{ "t1485", RUNTIME_CLASS(CDlg_t1485), IDD_t1485, "��������(t1485)" },
// 	{ "t1601", RUNTIME_CLASS(CDlg_t1601), IDD_t1601, "�����ں�����(t1601)" },
// 	{ "t1602", RUNTIME_CLASS(CDlg_t1602), IDD_t1602, "�ð��뺰�����ڸŸ�����(t1602)" },
// 	{ "t1603", RUNTIME_CLASS(CDlg_t1603), IDD_t1603, "�ð��뺰�����ڸŸ����̻�(t1603)" },
// 	{ "t1615", RUNTIME_CLASS(CDlg_t1615), IDD_t1615, "�����ڸŸ�����1(t1615)" },
// 	{ "t1617", RUNTIME_CLASS(CDlg_t1617), IDD_t1617, "�����ڸŸ�����2(t1617)" },
// 	{ "t1664", RUNTIME_CLASS(CDlg_t1664), IDD_t1664, "�����ڸŸ�����(íƮ)" },
// 	{ "t1621", RUNTIME_CLASS(CDlg_t1621), IDD_t1621, "�������к������ڸŸŵ���(íƮ��)" },
// 	{ "t1631", RUNTIME_CLASS(CDlg_t1631), IDD_t1631, "���α׷��Ÿ�������ȸ(t1631)" },
// 	{ "t1632", RUNTIME_CLASS(CDlg_t1632), IDD_t1632, "�ð��뺰���α׷��Ÿ�����(t1632)" },
// 	{ "t1633", RUNTIME_CLASS(CDlg_t1633), IDD_t1633, "�Ⱓ�����α׷��Ÿ�����(t1633)" },
// 	{ "t1640", RUNTIME_CLASS(CDlg_t1640), IDD_t1640, "���α׷��Ÿ�������ȸ(�̴�)(t1640)" },
// 	{ "t1662", RUNTIME_CLASS(CDlg_t1662), IDD_t1662, "�ð��뺰���α׷��Ÿ�����(��Ʈ)(t1662)" },
// 	{ "t3102", RUNTIME_CLASS(CDlg_t3102), IDD_t3102, "��������(t3102)" },
// 	{ "t3202", RUNTIME_CLASS(CDlg_t3202), IDD_t3202, "������������(t3202)" },
// 	{ "t3341", RUNTIME_CLASS(CDlg_t3341), IDD_t3341, "�繫��������(t3341)" },
// 	{ "t3401", RUNTIME_CLASS(CDlg_t3401), IDD_t3401, "�����ǰ�(t3401)" },
// 	{ "t1404", RUNTIME_CLASS(CDlg_t1404), IDD_t1404, "����/�Ҽ���/����������ȸ(t1404)" },
// 	{ "t1405", RUNTIME_CLASS(CDlg_t1405), IDD_t1405, "���ڰ��/�Ÿ�����/�����Ÿ���ȸ(t1405)" },
// 	{ "t4201", RUNTIME_CLASS(CDlg_t4201), IDD_t4201, "�ֽ�íƮ(����)(t4201)" },
// 	{ "t4203", RUNTIME_CLASS(CDlg_t4203), IDD_t4203, "����íƮ(����)(t4203)" },
 	{ "t9905", RUNTIME_CLASS(CDlg_t9905), IDD_t9905, "�����ڻ긮��Ʈ��ȸ(t9905)" },
// 	{ "t9907", RUNTIME_CLASS(CDlg_t9907), IDD_t9907, "�������ȸ(t9907)" },
// 
// 	{ "S2_"  , RUNTIME_CLASS(CDlg_S2_), IDD_S2_, "KOSPI�켱ȣ��" },
// 	{ "S3_"  , RUNTIME_CLASS(CDlg_S3_), IDD_S3_, "KOSPI ü��" },
// 	{ "S4_"  , RUNTIME_CLASS(CDlg_S4_), IDD_S4_, "KOSPI �⼼" },
	{ "H1_"  , RUNTIME_CLASS(CDlg_H1_), IDD_H1_, "KOSPI ȣ���ܷ�" },
	{ "H2_"  , RUNTIME_CLASS(CDlg_H2_), IDD_H2_, "KOSPI �ð���ȣ���ܷ�" },
// 	{ "YS3"  , RUNTIME_CLASS(CDlg_YS3), IDD_YS3, "KOSPI ����ü��" },
// 	{ "KS_"  , RUNTIME_CLASS(CDlg_KS_), IDD_KS_, "KOSDAQ �켱ȣ��" },
// 	{ "K3_"  , RUNTIME_CLASS(CDlg_K3_), IDD_K3_, "KOSDAQ ü��" },
// 	{ "HA_"  , RUNTIME_CLASS(CDlg_HA_), IDD_HA_, "KOSDAQ ȣ���ܷ�" },
// 	{ "HB_"  , RUNTIME_CLASS(CDlg_HB_), IDD_HB_, "KOSDAQ�ð���ȣ���ܷ�" },
// 	{ "YK3"  , RUNTIME_CLASS(CDlg_YK3), IDD_YK3, "KOSDAQ����ü��" },
// 	{ "s2_"  , RUNTIME_CLASS(CDlg_s2_), IDD_s2_, "ELW �켱ȣ��" },
// 	{ "s3_"  , RUNTIME_CLASS(CDlg_s3_), IDD_s3_, "ELW ü��" },
// 	{ "s4_"  , RUNTIME_CLASS(CDlg_s4_), IDD_s4_, "ELW �⼼" },
//	{ "h3_"  , RUNTIME_CLASS(CDlg_h3_ELW), IDD_h3_ELW, "ELW ȣ���ܷ�" },
//	{ "h2_"  , RUNTIME_CLASS(CDlg_h2_ELW), IDD_h2_ELW, "ELW �ð���ȣ���ܷ�" },
// 	{ "TH_"  , RUNTIME_CLASS(CDlg_TH_), IDD_TH_, "OTCBBȣ��" },
// 	{ "TS_"  , RUNTIME_CLASS(CDlg_TS_), IDD_TS_, "OTCBBü��" },
//  	{ "SC0"  , RUNTIME_CLASS(CDlg_SC0), IDD_SC0, "�ֽ��ֹ�����" },
// 	{ "SC1"  , RUNTIME_CLASS(CDlg_SC1), IDD_SC1, "�ֽ��ֹ�ü��" },
// 	{ "SC2"  , RUNTIME_CLASS(CDlg_SC2), IDD_SC2, "�ֽ��ֹ�����" },
// 	{ "SC3"  , RUNTIME_CLASS(CDlg_SC3), IDD_SC3, "�ֽ��ֹ����" },
// 	{ "SC4"  , RUNTIME_CLASS(CDlg_SC4), IDD_SC4, "�ֽ��ֹ��ź�" },
	{ "FC0"  , RUNTIME_CLASS(CDlg_FC0), IDD_FC0, "����ü��(�ü�)/ȣ��" },
	{ "FC0_1", RUNTIME_CLASS(CDlg_FC0_1), IDD_FC0_1, "����ü��(�ؿܽü�)/ȣ��" },
// 	{ "FH0"  , RUNTIME_CLASS(CDlg_FH0), IDD_FH0, "����ȣ��" },
// 	{ "OC0"  , RUNTIME_CLASS(CDlg_OC0), IDD_OC0, "�ɼ�ü��" },
// 	{ "OH0"  , RUNTIME_CLASS(CDlg_OH0), IDD_OH0, "�ɼ�ȣ��" },
// 	{ "O01"  , RUNTIME_CLASS(CDlg_O01), IDD_O01, "�����ֹ�����" },
// 	{ "H01"  , RUNTIME_CLASS(CDlg_H01), IDD_H01, "�����ֹ��������" },
// 	{ "C01"  , RUNTIME_CLASS(CDlg_C01), IDD_C01, "�����ֹ�ü��" },
// 	{ "IJ_"  , RUNTIME_CLASS(CDlg_IJ_), IDD_IJ_, "����" },
// 	{ "YJ_"  , RUNTIME_CLASS(CDlg_YJ_), IDD_YJ_, "��������" },
// 	{ "PM_"  , RUNTIME_CLASS(CDlg_PM_), IDD_PM_, "KOSPI���α׷��Ÿ���ü����" },
// 	{ "KM_"  , RUNTIME_CLASS(CDlg_KM_), IDD_KM_, "KOSDAQ���α׷��Ÿ���ü����" },
//	{ "BMT"  , RUNTIME_CLASS(CDlg_BMT), IDD_BMT, "�ð��뺰 �����ڸŸ�����" },
//	{ "BM_"  , RUNTIME_CLASS(CDlg_BM_), IDD_BM_, "������ �����ں� �Ÿ���Ȳ" },
// 	{ "NWS"  , RUNTIME_CLASS(CDlg_NWS), IDD_NWS, "�ǽð�����������Ŷ" },
// 	{ "JIF"  , RUNTIME_CLASS(CDlg_JIF), IDD_JIF, "������" },
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
	strMsg.Format( "%s �� Sample�� ã�� �� �����ϴ�.", pszTrCode );
	
	MessageBox( strMsg, "����", MB_ICONSTOP );

	SendMessage( WM_CLOSE );
	
	return FALSE;
}