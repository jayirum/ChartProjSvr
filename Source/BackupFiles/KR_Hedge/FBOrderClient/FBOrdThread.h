#pragma once

#include "stdafx.h"
#include "ESApi.h"
#include "../Common/KRCommon.h"
#include "../../IRUM_UTIL/BaseThread.h"
//#include "../../IRUM_UTIL/sharedmem.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include <map>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �ֹ�����
typedef struct _ORD_INFO
{
	CString sSymbol;
	int nOrdNo;
	int nOrigNo;
	int nRemainQty;
	char cSide[1];
	char zPrc[LEN_PRC + 1];
}ORD_INFO;


// ����������
typedef struct _POS_INFO
{
	CString sSymbol;
	int		nOpenQty;
	char	cSide[1];
	char	zPosPrc[LEN_PRC + 1];
	int		nCntrNo;
}POS_INFO;

typedef std::map<int, ORD_INFO*>			MAP_ORDSUB;		// �ֹ���ȣ, ORDINFO
typedef std::map<int, ORD_INFO*>::iterator	MAPIT_ORDSUB;
typedef std::map<CString, MAP_ORDSUB>			MAP_ORD;	// ���¹�ȣ, 
typedef std::map<CString, MAP_ORDSUB>::iterator	MAPIT_ORD;

typedef std::map<CString, POS_INFO*>			MAP_POSSUB;		// ����, POS_INFO
typedef std::map<CString, POS_INFO*>::iterator	MAPIT_POSSUB;
typedef std::map<CString, MAP_POSSUB>			MAP_POS;		// ���¹�ȣ, 
typedef std::map<CString, MAP_POSSUB>::iterator	MAPIT_POS;


enum ORDSEND_TYPE {
	ORDSEND_CNCL_ORDNO, ORDSEND_CNCL_SYMBOL, ORDSEND_CNCL_ACNT,
	ORDSEND_CLR_SYMBOL, ORDSEND_CLR_ACNT};

class CFBOrdThread : public CBaseThread
{ 
public:
	CFBOrdThread(CESApi* api, CRITICAL_SECTION* cs, HWND* hwd);
	~CFBOrdThread();


	BOOL	Initialize();
	BOOL	OrderSend(ORDSEND_TYPE tp, _In_ CString& sAcnt, _In_ CString& sSymbol, _In_ CString sAcntPwd, int nOrdNo);
	////////////////////////////////////////////////////////////////////////
	//	�Ϲ��ֹ� ����
	VOID OrdSend_General(_In_ CString& sAcnt, _In_ CString& sSymbol, _In_ CString sAcntPwd
		, _In_ int nQty, char* zPrc, char cSide);


	VOID	APIRecvReal(At0985O_st* p);
	VOID	APIRecvTr(int nGb,void* p);

private:
	VOID	MainFunc();
	VOID	ThreadFunc();


	////////////////////////////////////////////////////////////////////////
	//	����ֹ� ����
	BOOL OrderSend_Cncl_Symbol(_In_ CString& sAcnt, _In_ CString& sSymbol, _In_ CString sAcntPwd);
	BOOL OrderSend_Cncl_OrdNo(_In_ CString& sAcnt, _In_ int nOrdNo, _In_ CString sAcntPwd);
	BOOL OrderSend_Cncl_Acnt(_In_ CString& sAcnt, _In_ CString sAcntPwd);

	////////////////////////////////////////////////////////////////////////
	//	û���ֹ� ����
	BOOL OrderSend_Clr_Symbol(_In_ CString& sAcnt, _In_ CString& sSymbol, _In_ CString sAcntPwd);
	BOOL OrderSend_Clr_Acnt(_In_ CString& sAcnt, _In_ CString sAcntPwd);


	////////////////////////////////////////////////////////////////////////
	// API �� ���� �ֹ�ü�� ������ ���� ��� 
	
	VOID	DoProc_RecvAPIReplyWrapper(At0985O_st* p);
	VOID	DoProc_RecvAPIReply(At0985O_st* p);

	int	OrderRecv_Add(_In_ CString& sAcnt, CString sSymbol, int nOrdNo, int nOrigNo, int nRemainQty, char cSide, char *pzPrc);
	int	OrderRecv_Modify(_In_ CString& sAcnt, CString sSymbol, int nOrdNo, int nOrigNo, int nRemainQty, char cSide, char *pzPrc);
	int	OrderRecv_Cncl(_In_ CString& sAcnt, _In_  int nOrigNo);

	int PosUpdate(_In_ CString& sAcnt, _In_ CString sSymbol, _In_ char cSide, _In_ char* pzPrc, _In_ int nOpenQty);
	int PosDelete(_In_ CString& sAcnt, _In_ CString sSymbol);

	// �ֹ� API �� ����
	VOID	DoOrder(_In_ CString& sAcnt,const char cOrdType, const char* zPrc, const char cSide, 
						const int nQty, _In_ CString& sSymbol, const int nOrigNo, _In_ CString sAcntPwd);


	VOID	APILOCK() { EnterCriticalSection(m_csApi); }
	VOID	APIUNLOCK() { LeaveCriticalSection(m_csApi); }

	VOID	LOCK_ORD() { EnterCriticalSection(&m_csOrd); }
	VOID	UNLOCK_ORD(){LeaveCriticalSection(&m_csOrd); }
	VOID	LOCK_POS() { EnterCriticalSection(&m_csPos); };
	VOID	UNLOCK_POS() { LeaveCriticalSection(&m_csPos); };

private:
	CESApi				*m_api;
	CRITICAL_SECTION	*m_csApi;
	HWND				*m_hwd;

	MAP_ORD			m_mapOrd;
	MAP_POS			m_mapPos;
	CRITICAL_SECTION	m_csOrd, m_csPos;

	HANDLE			m_hMutexOrd;

	char			m_szMsg[1024];
	CRITICAL_SECTION	m_csProtect;	// �ٸ� �۾� ���� �� MANAGE_NOTIFY ���� �ȵǰ�.
};


