#if !defined(AFX_REAL_ORDCNTR_H__54302942_704F_4E51_8425_47428123667F__INCLUDED_)
#define AFX_REAL_ORDCNTR_H__54302942_704F_4E51_8425_47428123667F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Real_OrdCntr.h : header file
//

/***************************************************************************


  
		KS200 선물, 옵션의 주문 및 체결 실시간 TR 을 등록한다.



***************************************************************************/

#include "../../include/common.h"
#include "../../include/Packet.h"
#include "../../include/UtilLibInc.h"


#define ETK_REAL_ORD		"O01"
#define ETK_REAL_MDFYCNCL	"H01"
#define ETK_REAL_CNTR		"C01"

class CReal_OrdCntr : public CDialog
{
// Construction
public:
	CReal_OrdCntr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReal_OrdCntr)
	enum { IDD = IDD_O01 };
	CListBox	m_lstMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReal_OrdCntr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


	//////////////////////////////////////////////////////////////////////////
	//	member variables
	private:
		
	ScrewDB		m_db;
	CLog		m_logF;
	//CMQBase		m_MQRslt_Ord;
	//CMQBase		m_MQRslt_Cntr;
	
	list<CString>		m_sStkList_KSF;
	list<CString>		m_sStkList_KSO;
	
	char		m_szAPI_UserID[64];
	char		m_szAPI_AcntNo[64];

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReal_OrdCntr)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg void		OnButtonRequest		();
	BOOL				GetStkCodeList_KS(char* psGdsCode);
// 	void				AdviseData(CString sStk);
// 	void				UnadviseData(CString sStk);
	void				RegistRealData(BOOL bAdvise, CString sStk);
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REAL_ORDCNTR_H__54302942_704F_4E51_8425_47428123667F__INCLUDED_)
