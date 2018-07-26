#pragma once
#include "../Common/KRCommon.h"
#include <afx.h>
#include <map>

#define REPL_TYPE_ACPT_ORD	'1'		// 주문접수 
#define REPL_TYPE_MATCH		'2'		// 체결 
#define	REPL_TYPE_ACPT_MDFY	'3'		// 정정확인 
#define	REPL_TYPE_ACPT_CANCEL	'4'	// 취소확인
#define	REPL_TYPE_RJCT_NEW	'5'		// 신규거부 
#define	REPL_TYPE_RJCT_MDFY	'6'		// 정정거부 
#define	REPL_TYPE_RJCT_CANCEL	'7'	// 취소거부 
#define	REPL_TYPE_ACPT_BOOK		'0'	// 원장접수)


typedef struct _ORDER_INFO
{
	CString sSymbol;
	CString sOrderNo;
	char	cSide;	// 1:매수, 2:매도
	CString sPrc;
	UINT	nRmainQty;
	CString sTM;
}ORDER_INFO;


//typedef struct _POS
//{
//	CString sCntrNo;	// 체결번호
//	char	cSide;	// 1:매수, 2:매도
//	CString sPrc;
//	UINT	nRmainQty;
//
//}POS;

typedef struct _POS_INFO
{
	CString sSymbol;
	char	cSide;		// 1:매수, 2:매도
	CString sAvgPrc;	// 평균단가
	UINT	nPosQty;
}POS_INFO;

class CIndividual
{
public:
	CIndividual();
	virtual ~CIndividual();

	VOID Clear();

public:
	CString m_sAcntNo;
	CString m_sConnPwd;
	CString m_sAcntPwd;
	CString m_sOrdSymbol;		// 자동주문할 종목
	BOOL	m_arrStrategy[STRATEGY_CNT];	// .
	std::map<CString/*주문번호*/, ORDER_INFO*>	m_mapOrd;	// 주문번호, COrder
	std::map<CString/*종목*/, POS_INFO*>	m_mapPos;	// symbol, CPos
};

class CUserInfoManager
{
public:
	CUserInfoManager();
	~CUserInfoManager();

	BOOL ProcOrderApiData(char* pzAcntNo, CString& sSymbol, char* psApiBuff);
	
	BOOL SetAcntInfo(char* pzAcntNo, char* pzConnPwd, char* pzAcntPwd);
	BOOL SetOrderSymbol(char* pzAcntNo, char* pzSymbol);
	BOOL SetStrategy(char* pzAcntNo, STRATEGY_TP tp);
	char* GetMsg() { return m_sMsg.GetBuffer(); };
	
private:
	VOID Clear();
	VOID LOCK() { EnterCriticalSection(&m_cs); };
	VOID UNLOCK() { LeaveCriticalSection(&m_cs); };

	BOOL ProcOrderInfo(char* pzAcntNo, CString& sSymbol, char* psApiBuff);
	BOOL ProcPosInfo(char* pzAcntNo, CString& sSymbol, char* psApiBuff);

	ORDER_INFO* GetOrderPtr(char* pzAcntNo, CString& sOrdNo);
	BOOL DelOrder(CString sAcntNo, CString& sOrdNo);
	BOOL SaveOrder(CString sAcntNo, CString& sOrdNo, ORDER_INFO* pOrd);

	POS_INFO* GetPosPtr(char* pzAcntNo, CString& sSymbol);
	BOOL DelPosPtr(CString& sAcntNo, CString& sSymbol);
	BOOL SavePos(CString sAcntNo, CString& sSymbol, POS_INFO* pPos);

private:
	CRITICAL_SECTION m_cs;
	CString m_sMsg;
	std::map<CString/*계좌*/, CIndividual*>	m_mapUser;	// acnt
};

