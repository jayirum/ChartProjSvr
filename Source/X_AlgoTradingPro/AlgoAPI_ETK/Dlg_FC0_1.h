#if !defined(AFX_DLG_FC0_H__02C7D1E4_2C21_46E3_891F_7256BFB82BF2__INCLUDED_)
#define AFX_DLG_FC0_H__02C7D1E4_2C21_46E3_891F_7256BFB82BF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_FC0.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_FC0_1 dialog


/***************************************************************************


  
		해외선물 시세, 호가를 모두 요청한다.



***************************************************************************/

#include "ClientInterface.h"
//#include "../Inc_ETK/common.h"
#include "../Common/Algo_common.h"
#include "../Common/Inc_ETK/ETK_Common.h"
#include "../../IRUM_UTIL/util.h"
#include <list>
#include <map>



#define MSG_RE_CONNECT		WM_USER + 889
//TCPRELAY
#define WM_SISE_DATA	WM_USER + 195

#define DEF_TARGET_CNT	1

class CDlg_FC0_1 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_FC0_1 )
// Construction
public:
	CDlg_FC0_1(CWnd* pParent = NULL);   // standard constructor

	//////////////////////////////////////////////////////////////////////////
	//	USER FUNCTION
	BOOL	LoadSymbols();
	
// Dialog Data
	//{{AFX_DATA(CDlg_FC0_1)
	enum { IDD = IDD_FC0_1 };
	CListBox	m_lstMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_FC0_1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	
	CClientInterface	*m_pClientIF;
	std::list<CString>		m_lstSymbols;

	//////////////////////////////////////////////////////////////////////////////

protected:
	CString				m_strCode, m_strCode_H;
	void				InitCtrls();
	void				AdviseData(char i_cTickYN, CString i_sSymbol);
	void				UnadviseData(char i_cTickYN, CString i_sSymbol);

	// Generated message map functions
	//{{AFX_MSG(CDlg_FC0_1)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonUnadvise();
	
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg void		OnButtonRequest		();
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_FC0_H__02C7D1E4_2C21_46E3_891F_7256BFB82BF2__INCLUDED_)
