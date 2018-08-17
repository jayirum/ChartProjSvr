
// Sample_xingACEDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "../../IRUM_INC/eBest/IXingAPI.h"
#include "afxwin.h"

// CSample_xingACEDlg 대화 상자
class CSample_xingACEDlg : public CDialogEx
{
// 생성입니다.
public:
	CSample_xingACEDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SAMPLE_XINGACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	IXingAPI	m_iXingAPI;
	void		InitCtrls();

	void		RequestCSPAT00600();	//현물 정상주문
	void		RequestCIDBT00100();	//해외선물 신규주문
	void		RequestCSPAT00700();	//현물 정정주문
	void		RequestCSPAT00800();	//현물 취소주문
	void		RequestCSPAQ03700();	//현물계좌 주문체결내역 조회 
	void		RequestCSPAQ02300();	// 현물계좌 잔고내역 조회 

	void		ReceiveCSPAT00600( LPVOID pData, int nLen );//현물 정상주문
	void		ReceiveCSPAT00700( LPVOID pData, int nLen );//현물 정정주문
	void		ReceiveCSPAT00800( LPVOID pData, int nLen );//현물 취소주문
	void		ReceiveCSPAQ03700( LPVOID pData, int nLen );//현물계좌 주문체결내역 조회 
	void		ReceiveCSPAQ02300( LPVOID pData, int nLen );// 현물계좌 잔고내역 조회 

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctrlHoga;
	CListCtrl m_ctrlChe;
	CComboBox m_ctrlAccount;
	CComboBox m_ctrlMeme;
	CListCtrl m_ctrlOrderList;
	CListCtrl m_ctrlJangoList;

	CString		m_strRegCode;

	int			GetSelectedItem		( CListCtrl& ctrl );
	void		ProcessDetailLog	( LPCTSTR pszTrCode, LPCTSTR pszData );
	void		SetDetailLogData	( LPCTSTR pszDesc, LPCTSTR pszData, int nDataSize, int nDataType, int nDataDotPos );

	void		DetailLogSC0		( LPCTSTR pszData );
	void		DetailLogSC1		( LPCTSTR pszData );
	void		DetailLogSC2		( LPCTSTR pszData );
	void		DetailLogSC3		( LPCTSTR pszData );
	void		DetailLogSC4		( LPCTSTR pszData );
	void		DetailLogS3_		( LPCTSTR pszData );
	void		DetailLogH1_		( LPCTSTR pszData );
	void		DetailLogK3_		( LPCTSTR pszData );
	void		DetailLogHA_		( LPCTSTR pszData );

	void		ReceiveRealS3_		( LPCTSTR pszData );
	void		ReceiveRealH1_		( LPCTSTR pszData );
	void		ReceiveRealK3_		( LPCTSTR pszData );
	void		ReceiveRealHA_		( LPCTSTR pszData );

	LRESULT		OnXMLogin			( WPARAM wParam, LPARAM lParam );
	LRESULT		OnXMDisconnect		( WPARAM wParam, LPARAM lParam );
	LRESULT		OnXMReceiveData		( WPARAM wParam, LPARAM lParam );
	LRESULT		OnXMReceiveRealData	( WPARAM wParam, LPARAM lParam );
	afx_msg void OnSelchangeComboMeme();
	afx_msg void OnClickedButtonOrder();
	afx_msg void OnClickedButtonListOrder();
	afx_msg void OnClickedButtonListJango();
	afx_msg void OnClickedButtonLogin();
	CListCtrl m_ctrlLog;
	CListCtrl m_ctrlDetailLog;
	afx_msg void OnItemchangedListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListHoga(NMHDR *pNMHDR, LRESULT *pResult);
};
