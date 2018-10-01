
// MudFishOrdDlg.h : header file
//

#pragma once


#include "../../IRUM_UTIL/adofunc.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/BlockDup.h"
#include "../../IRUM_UTIL/TcpClient.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "StratHistManager.h"
#include "StratMaker.h"
#include <map>
#define EXENAME		"MudFishOrd.exe"

enum {API_ORD=0, API_TICK};
typedef struct _ST_STRAT
{
	CStratHistManager*	h;
	CStratMaker*		m;
	BOOL				bFirstFeed;
}ST_STRAT;

typedef std::map<std::string, ST_STRAT*>			MAP_STRAT;
typedef std::map<std::string, ST_STRAT*>::iterator	ITMAP_STRAT;


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

	static unsigned WINAPI Thread_ApiTick(LPVOID lp);
	static unsigned WINAPI Thread_ApiOrd(LPVOID lp);
	static unsigned WINAPI Thread_SaveData(LPVOID lp);

	BOOL	LoadSymbolInfo(BOOL bCreateStrat);
	void	ReSetSymbolInfo();
	VOID	End();
	VOID	ClearStratMap();
	void	showMsg(BOOL bSucc, char* pMsg, ...);
	void	InitSymbolList();
	void	InitRealPLList();
	void	InitApiSocket(int Idx);
	void	CloseApiSocket(int Idx);

	void	DatafeedProc(std::string sSymbol, char* pPacket, BOOL bFirstFeedSaved);

	//void	CalcEstmPL();
	VOID	ApiOrd_Err(char* pPacket);
	VOID	ApiOrd_Acpt(char* pPacket);
	VOID	ApiOrd_RealOrd(char* pPacket);
	VOID	ApiOrd_RealCntr(char* pPacket);

	BOOL	GetSymbolInfo(CString& symbol, double* pTickVal, double* pTickSize, double* pCurrPrc, int* pDotCnt);
private:
	CBlockDup		m_Dup;
	CMemPool		*m_pMemPool;
	CTcpClient		*m_pApiClient[2];
	CDBPoolAdo		*m_pDBPool;

	std::string		m_sApiIP[2];
	int				m_nApiPort[2];
		
	HANDLE			m_hApiTick, m_hApiOrd, m_hSaveData;
	unsigned int	m_unApiTick, m_unApiOrd, m_unSaveData;

	int m_nItem;
	int m_nSubitem;

	MAP_STRAT				m_mapStrat;
	CString m_sSymbol;
	CString m_sOpenPrc;
	CString m_sCloseTime;
	CString m_sMaxSLCnt;

	BOOL	m_bContinue;
public:
	CListBox m_lstMsg;
	
	CListCtrl m_ctlSymbol;
	std::map<CString, UINT>	m_mapIdxSymbol;
	
	CListCtrl m_ctlRealPl;
	std::map<CString, UINT>	m_mapIdxPl;

	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnDblclkListSymbol(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonStratStart();
	afx_msg void OnBnClickedButtonStratStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
