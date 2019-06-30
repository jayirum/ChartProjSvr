#if !defined(AFX_DLG_FC0_1_H__02C7D1E4_2C21_46E3_891F_7256BFB82BF2__INCLUDED_)
#define AFX_DLG_FC0_1_H__02C7D1E4_2C21_46E3_891F_7256BFB82BF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_FC0.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_FC0 dialog


/***************************************************************************


  
		KS200 선물, 옵션의 시세, 호가를 모두 요청한다.



***************************************************************************/

#include "../Inc_ETK/COMMON.H"
#include "../Inc_ETK/Packet.h"
//#include "../../include/UtilLibInc.h"

#include <set>
#include <list>
#include <map>


#define ETK_REAL_SISE_F			"FC0"
#define ETK_REAL_SISE_O			"OC0"
#define ETK_REAL_HOGA_F			"FH0"
#define ETK_REAL_HOGA_O			"OH0"

//CME
#define ETK_REAL_SISE_F_CME		"NC0"
#define ETK_REAL_HOGA_F_CME		"NH0"

#define MSG_RE_CONNECT		WM_USER + 889

//TCPRELAY
#define WM_SISE_DATA	WM_USER + 195

#define DEF_TARGET_CNT	1

class CDlg_FC0 : public CDialog
{
	DECLARE_DYNCREATE( CDlg_FC0 )
// Construction
public:
	CDlg_FC0(CWnd* pParent = NULL);   // standard constructor

	//////////////////////////////////////////////////////////////////////////
	//	USER FUNCTION
	BOOL	SockInit();
	VOID	SockClose();
	BOOL	GetStkCodeList_KS(char* psGdsCode);
	BOOL	GetNowPrc( CString i_sStkCode );
	VOID	Send_Sise_KSF( char* psOrgData );
	VOID	Send_Sise_KSO( char* psOrgData );
	VOID	Send_Hoga_KSF( char* psOrgData );
	VOID	Send_Hoga_KSO( char* psOrgData );
	
	//////////////////////////////////////////////////////////////////////////
	//static	DWORD WINAPI	ConnectThread(LPVOID lp);
	//static	DWORD WINAPI	SendThread_SHM(LPVOID lp);

	//CUdpSocket	*m_sockM;
	//CTcpSock	*m_pSock	[DEF_TARGET_CNT];
	DWORD		m_dwThreadID;
	HANDLE		m_hThread	;
	HANDLE		m_hDie		;
	int			m_nUdpPortLC	;
	int			m_nUdpPortReal	;

	std::list<CString>		m_lstData;
	CRITICAL_SECTION	m_cs;

	///////////////////////////////////////////////////////////////////////////
	//	시세를 SHM 에 직접 저장
	VOID SaveSHM( char* pSise );

	//시세중계
	//TCPRELAY CSiseClntSend	*m_clntSend;

// Dialog Data
	//{{AFX_DATA(CDlg_FC0)
	enum { IDD = IDD_FC0 };
	CListBox	m_lstMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_FC0)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	
	//TODO CLog	m_logF, m_logInner;

	//TODO CUdpSend	*m_sockFront;	//	FRONT SERVER 에게 전송할 UDP SOCKET
	//TODO CUdpSend	*m_sockLosscut;	//	LOSSCUT 에게 전송할 UDP SOCKET

	//TODO ScrewDB		m_db;
	
	char szPath[_MAX_PATH];
	char		m_szMcastIP[32];
	char		m_szLocalIP[32];
	int			m_nPortFront;
	int			m_nPortFront_O;
	int			m_nPortSHM;
	int			m_nPortMT;
	std::list<CString>		m_sStkList_KSF;
	std::list<CString>		m_sStkList_KSO;
	std::map<CString, CString>	m_mapNowPrc;	//	종목별 현재가 저장

	
	//CSharedMem		m_shm;

	HANDLE					m_hThdDummySise;				// skeo 2011-01-17
	static DWORD WINAPI		ProcDummySiseTest(LPVOID lp);	// skeo 2011-01-17

	//SMILOR.2스레드.
	static	DWORD WINAPI SiseThread(LPVOID lp);
	VOID	SiseProc(LPRECV_REAL_PACKET p);
	HANDLE				m_hSiseThr;
	CRITICAL_SECTION	m_csSise;

	
	//////////////////////////////////////////////////////////////////////////////
	//TCPRELAY
	BOOL	Relay_Begin();
	VOID	Relay_End();
	static	DWORD WINAPI  Relay_SendDThread(LPVOID lp);
	VOID	PostRelayData(LPSTR lpData, INT nLen);
	VOID	SendRelay(LPSTR lpData, INT nLen);
	BOOL	RequestSendIO( SOCKET sock, char* pSendBuf, int nSendLen );

	SOCKET		m_sockListen;
	SOCKADDR_IN	m_localsin;
	WSAEVENT	m_wsaAccept;				//	accept event
	WSAEVENT	m_wsaSend;
	std::set<SOCKET>	m_setRelaySock;
	CRITICAL_SECTION	m_csRelaySock;
	BOOL				m_bRelayContinue;
	HANDLE				m_hRelayThread;
	DWORD				m_dwRelayThread;

	std::list<CString>		m_lstRelayData;
	CRITICAL_SECTION	m_csRelayData;
	//////////////////////////////////////////////////////////////////////////////

protected:
	CString				m_strCode, m_strCode_H;
	void				InitCtrls();
	void				AdviseData(char i_cSiseYN, CString sStk, char* i_psGDS_CODE);
	void				UnadviseData(char i_cSiseYN, CString sStk, char* i_psGDS_CODE);

	// Generated message map functions
	//{{AFX_MSG(CDlg_FC0)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonUnadvise();
	afx_msg void OnButtonTest_1();
	//}}AFX_MSG
	afx_msg void		OnDestroy();
	afx_msg void		OnButtonRequest		();
	afx_msg	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_FC0_H__02C7D1E4_2C21_46E3_891F_7256BFB82BF2__INCLUDED_)
