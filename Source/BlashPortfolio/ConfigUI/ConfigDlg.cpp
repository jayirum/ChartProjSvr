// ConfigDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ConfigUI.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"
#include "TabGeneral.h"
#include "TabSymbol.h"

// ConfigDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(ConfigDlg, CDialog)

ConfigDlg::ConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONFIGDLG, pParent)
{

}

ConfigDlg::~ConfigDlg()
{
}

void ConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_TAB_GENERAL, m_tabGeneral);
	DDX_Control(pDX, IDC_TAB_GENERAL, m_tab);
}


BEGIN_MESSAGE_MAP(ConfigDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_GENERAL, &ConfigDlg::OnTcnSelchangeTabGeneral)
END_MESSAGE_MAP()


// ConfigDlg 메시지 처리기입니다.


BOOL ConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CRect rect;
	m_tab.GetWindowRect(&rect);

	m_tab.InsertItem(0, _T("General Config"));
	m_tab.InsertItem(1, _T("Symbols Config"));
	m_tab.SetCurSel(0);

	m_pTabGeneral	= new CTabGeneral;
	m_pTabGeneral->Create(IDD_DIALOG_GENERAL, &m_tab);
	m_pTabGeneral->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pTabGeneral->ShowWindow(SW_SHOW);

	m_pTabSymbol	= new CTabSymbol;
	m_pTabSymbol->Create(IDD_DIALOG_SYMBOL, &m_tab);
	m_pTabSymbol->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pTabSymbol->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void ConfigDlg::OnTcnSelchangeTabGeneral(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int sel = m_tab.GetCurSel();

	switch (sel)
	{
	case 0:
		m_pTabGeneral->ShowWindow(SW_SHOW);
		m_pTabSymbol->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pTabGeneral->ShowWindow(SW_HIDE);
		m_pTabSymbol->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}
