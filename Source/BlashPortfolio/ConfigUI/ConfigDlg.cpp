// ConfigDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ConfigUI.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"
#include "TabGeneral.h"
#include "TabSymbol.h"

// ConfigDlg ��ȭ �����Դϴ�.

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


// ConfigDlg �޽��� ó�����Դϴ�.


BOOL ConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void ConfigDlg::OnTcnSelchangeTabGeneral(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
