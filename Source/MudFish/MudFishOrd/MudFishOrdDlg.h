
// MudFishOrdDlg.h : header file
//

#pragma once


#include "../../IRUM_UTIL/adofunc.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/BlockDup.h"
#include "../../IRUM_UTIL/TcpClient.h"
#include "SymbolPosition.h"
#include "afxwin.h"
#include "afxcmn.h"

#define EXENAME		"MudFishOrd.exe"


// CMudFishOrdDlg dialog
class CMudFishOrdDlg : public CDialogEx
{
// Construction
public:
	CMudFishOrdDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUDFISHORD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:

	static unsigned WINAPI Thread_TickDataRecv(LPVOID lp);
	static unsigned WINAPI Thread_SendOrd(LPVOID lp);

	BOOL	Begin();
	BOOL	LoadSymbolInfo(BOOL bCreateStrat);
	VOID	End();
	void	showMsg(BOOL bSucc, char* pMsg, ...);
	void	InitSymbolList();
	void	InitRealPLList();

private:
	CBlockDup		m_Dup;
	CMemPool		*m_pMemPool;
	CTcpClient		*m_pApiRecv;
	CDBPoolAdo		*m_pDBPool;
	CSymbolPosition	*m_symPos;

	char			m_zApiIP[32];
	int				m_nApiPort;
		
	HANDLE			m_hRecvThread, m_hOrdSendThread;
	unsigned int	m_unRecvThread, m_unOrdSendThread;

	int m_nItem;
	int m_nSubitem;

	//todo std::map<std::string, CStratMaker*>		m_mapStrat;
	//CStratMaker* m_pStrat;
	CString m_sSymbol;
	CString m_sOpenPrc;
	CString m_sCloseTime;
	CString m_sMaxSLCnt;
public:
	CListBox m_lstMsg;
	CListCtrl m_ctlSymbol;
	CListCtrl m_ctlRealPl;
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnDblclkListSymbol(NMHDR *pNMHDR, LRESULT *pResult);
};
