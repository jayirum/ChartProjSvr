#if !defined(AFX_DLG_T5511_H__AC77E66E_89A9_4F96_9ACC_A0E4F74A0EE9__INCLUDED_)
#define AFX_DLG_t5511_H__AC77E66E_89A9_4F96_9ACC_A0E4F74A0EE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_t5511.h : header file
//

/****************************************************************************


1.	�ֹ��� MQ ���� ������ ���ǻ�� �����Ѵ�.

	- �Ϲ��ֹ�, �����ֹ�, ����ֹ� ���

2.	�ֹ�TR �� ���� ����

3.	�ֹ�/ü���� REAL ���� �� ����
		

****************************************************************************/

//#include "../../include/UtilLibInc.h"


#define ETK_TR_ORD_NEW		"t5511"
#define ETK_TR_ORD_MDFY		"t5512"
#define ETK_TR_ORD_CNCL		"t5513"


#define ETK_REAL_ACPT_ORD		"O01"
#define ETK_REAL_ACPT_MDFYCNCL	"H01"
#define ETK_REAL_ACPT_CNTR		"C01"


#include <map>
#include <list>
using namespace std;
//
//typedef struct _MAP_ORD_INFO
//{
//	char		TRCode[32];
//	char		GDS_CODE[2];
//	char		CLORD_NO[MAX_ORD_NO+1];
//}MAP_ORD_INFO;

/////////////////////////////////////////////////////////////////////////////
// CDlg_t5511 dialog

class CDlg_t5511 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_t5511 )

// Construction
public:
	CDlg_t5511(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_t5511)
	enum { IDD = IDD_t5511 };
	CListBox	m_lstTR;
	CListBox	m_lstR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_t5511)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


	//////////////////////////////////////////////////////////////////////////
	//	member variables
private:
	
	/*-------------------------------------
		�ֹ�TR ��/���ſ� ���� ���
	-------------------------------------*/
	VOID	ReturnRjct(int i_nErrCode, char* i_psGDS_CODE, char* i_psCLORD_NO, char* i_psErrMsg);
	
	//	MQ READ �� ���� ������
	static	DWORD WINAPI  ThreadProc(LPVOID lp);
	VOID	StopThread();

	BOOL	GetStkCodeList_KS(char* psGdsCode);
	BOOL	RegistRealData(BOOL bAdvise, char* i_psTRCode);

private:
	//CLog	m_logF;

	HANDLE		m_hDie;
	HANDLE		m_hThread;

// 	CMQBase		m_MQReq_Ord;	//	�ֹ�TR ���� MQ
// 	CMQBase		m_MQRslt_Ord;	//	�ֹ���� ������ MQ
// 	CMQBase		m_MQRslt_Cntr;	//	ü���� ������ MQ

	//map<int, MAP_ORD_INFO>	m_mapRqID;	//	RqID, TR CODE

	/*-------------------------------------------------
		�ֹ�ü���� REAL ���� �� ������ ���� ���
	--------------------------------------------------*/	
	//ScrewDB		m_db;
	
	list<CString>		m_sStkList_KSF;
	list<CString>		m_sStkList_KSO;

	
	char		m_szAPI_UserID[64];
	char		m_szAPI_AcntNo[64];
	int			m_nOrdAliveMin;	//	�ֹ���ȿ�ð�
	CString		m_sMsg;
	char		m_szMsg[1024];


// Implementation
protected:
	void				RequestData( char* i_psLabel, char* i_psBody );

	// Generated message map functions
	//{{AFX_MSG(CDlg_t5511)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void		OnButtonRequest		();
	afx_msg	LRESULT		OnXMReceiveData		( WPARAM wParam, LPARAM lParam );	//	�ֹ�TR ���� ����
	afx_msg	LRESULT		OnXMTimeoutData		( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );	//	�ֹ�/ü�� REAL ����
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_T5501_H__AC77E66E_89A9_4F96_9ACC_A0E4F74A0EE9__INCLUDED_)
