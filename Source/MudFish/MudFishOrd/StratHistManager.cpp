#include "StratHistManager.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/LogMsg.h"

extern CLogMsg g_log;
CStratHistManager::CStratHistManager(char* pzSymbol)
{
	ZeroMemory(&m_symbol, sizeof(m_symbol));
	ZeroMemory(&m_param, sizeof(m_param));
	m_pos.Init(TRUE);

	lstrcpy(m_symbol.zSymbol, pzSymbol);
	
	InitializeCriticalSection(&m_cs);
}


CStratHistManager::~CStratHistManager()
{
	DeleteCriticalSection(&m_cs);
}

void CStratHistManager::SetInitInfo(double dTickVal, double dTickSize, int nDotCnt,
	char* pzOpenPrc, int nOrdQty, char* pzEndTM, int nMaxSLCnt, int nMaxPTCnt,
	double dEntrySpread, double dClrSpread, double dPtPoint)
{
	lock();
	m_symbol.dTickValue = dTickVal;
	m_symbol.dTickSize = dTickSize;
	m_symbol.nDotCnt = nDotCnt;
	strcpy(m_param.zOpenPrc, pzOpenPrc);
	strcpy(m_param.zEndTM, pzEndTM);
	m_param.nOrdQty = nOrdQty;
	m_param.nMaxCntSL = nMaxSLCnt;
	m_param.nMaxCntPT = nMaxPTCnt;
	m_param.dEntrySpread = dEntrySpread;
	m_param.dClrSpread = dClrSpread;
	m_param.dPtPoint = dPtPoint;
	unlock();
}

void CStratHistManager::SetCntrInfo(LPCSTR lpBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	lock();

	// 현재 zero position
	if (m_pos.Status == FLAG_NONE)
	{
		ZeroMemory(&m_pos, sizeof(m_pos));
		m_pos.Status = (lpBsTp[0] == CD_BUY) ? FLAG_OPEN_BUY : FLAG_OPEN_SELL;
		sprintf(m_pos.zEntryPrc, "%.*s", LEN_PRC, lpCntrPrc);
		m_pos.cBsTp[0] = lpBsTp[0];
		m_pos.dQty = atof(lpCntrQty);
		
	}
	else
	{
		// 현재 진입중 - 같은 방향
		if (m_pos.cBsTp[0] == lpBsTp[0])
		{
			double dAmt1 = atof(m_pos.zEntryPrc) * m_pos.dQty;
			double dAmt2 = atof(lpCntrPrc) * atof(lpCntrQty);
			double dQty = atof(lpCntrQty) + m_pos.dQty;
			double dAvg = (dAmt1 + dAmt2) / dQty;

			sprintf(m_pos.zEntryPrc, "%*.*f", LEN_PRC, m_symbol.nDotCnt, dAvg);
			m_pos.dQty = dQty;
		}
		else
		{
			// 현재 진입중 - 다른 방향
			CalcClrCntr(lpBsTp, lpCntrPrc, lpCntrQty);
		}
	}

	// 주문전송한 수량 차감
	m_pos.dOrdSentQty -= atof(lpCntrQty);

	unlock();
}

void CStratHistManager::CalcClrCntr(LPCSTR lpBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	double dCntrQty = atof(lpCntrQty);
	double dPLQty;
	double dPrcGap;
	if (lpBsTp[0] == CD_BUY)
		dPrcGap = atof(m_pos.zEntryPrc) - atof(lpCntrPrc);
	else
		dPrcGap = atof(m_pos.zEntryPrc) - atof(lpCntrPrc);

	double dPlTick = dPrcGap / m_symbol.dTickSize;
	double dPlMoney = 0;

	//1. 전부체결
	if (m_pos.dQty == dCntrQty)
	{
		dPLQty = dCntrQty;

		if (dPrcGap > 0) 
		{
			m_pos.nTotPTCnt++;

			// 익절인 경우 open 가를 변경한다.
			sprintf(m_param.zOpenPrc, lpCntrPrc);
		}
		if (dPrcGap < 0)	m_pos.nTotSLCnt++;

		if (m_pos.nTotPTCnt >= m_param.nMaxCntPT)
		{
			m_pos.bFinish = TRUE;
			g_log.log(LOGTP_SUCC, "[%s]익절(%d)회 수행으로 거래중단.", m_symbol.zSymbol, m_pos.nTotPTCnt);

		}
		if (m_pos.nTotSLCnt >= m_param.nMaxCntSL)
		{
			m_pos.bFinish = TRUE;
			g_log.log(LOGTP_SUCC, "[%s]손절(%d)회 수행으로 거래중단.", m_symbol.zSymbol, m_pos.nTotPTCnt);
		}
		m_pos.Init(FALSE);
		
	}

	//2. 일부체결
	if (m_pos.dQty > dCntrQty)
	{
		dPLQty = dCntrQty;
		m_pos.dQty -= dCntrQty;
	}

	//3. 역전
	if (m_pos.dQty < dCntrQty)
	{
		dPLQty = m_pos.dQty;
		m_pos.cBsTp[0] = lpBsTp[0];
		m_pos.Status = (m_pos.cBsTp[0] == CD_BUY) ? FLAG_OPEN_BUY : FLAG_OPEN_SELL;

		g_log.log(LOGTP_ERR, "[%s] 포지션역전 발생.기존수량(%f) 체결수량(%f)",
			m_symbol.zSymbol, m_pos.dQty, dCntrQty);
	}
	dPlMoney = dPlTick * m_symbol.dTickValue * dPLQty;
	g_log.log(LOGTP_SUCC, "[%s] 가격차(%f) 손익수량(%f) 손익TICK(%f) 손익(%f)",
		m_symbol.zSymbol, dPrcGap, dPLQty, dPlTick, dPlMoney);
}



/*
	진입 후 최고수익의 50% 이하로 떨어져서 익절 할 조건인가?

ex)
	매수진입 100일때 (최고가 - 진입가)
		최고가 120  ==> gap : 20
		현재가 115  ==> gap : 15 -> 익절 안한다.
		현재가 109  ==> gap : 9  -> 익절한다.
		현재가 99   ==> gap : -1 -> 익절한다.

	매도진입 100일때 (진입가 - 최고가)
		최고가 80  ==> gap : 20
		현재가 85  ==> gap : 15 -> 익절 안한다.
		현재가 95  ==> gap : 5  -> 익절한다.
		현재가 101   ==> gap : -1 -> 익절한다.
*/
BOOL CStratHistManager::IsPTCondition(char* pzCurrPrc,_Out_ char* pMsg)
{
	BOOL bResult = FALSE;
	char zGapMax[32], zGapCurr[32];

	int nComp = CUtil::CompPrc(m_pos.zMaxPLPrc, LEN_PRC, m_pos.zEntryPrc, LEN_PRC, m_symbol.nDotCnt, LEN_PRC);
	if (nComp == 0)
		return FALSE;
	
	// 현재 LONG
	if (IsLong())
	{
		sprintf(zGapMax, "%.2f", (atof(m_pos.zMaxPLPrc) - atof(m_pos.zEntryPrc))*m_param.dClrSpread);
		sprintf(zGapCurr, "%.2f", atof(pzCurrPrc) - atof(m_pos.zEntryPrc));
		int nComp = CUtil::CompPrc(zGapCurr, LEN_PRC, zGapMax, LEN_PRC, m_symbol.nDotCnt, LEN_PRC);
		if (nComp<0)
		{
			bResult = TRUE;
			sprintf(pMsg, "LONG익절조건.(진입가:%s, 최고가:%s, 현재가:%s) (최고가GAP*%f:%s, 현재가Gap:%s)",
				m_pos.zEntryPrc, m_pos.zMaxPLPrc, pzCurrPrc, m_param.dClrSpread, zGapMax, zGapCurr);
		}
	}

	// 현재 Short
	if (IsShort())
	{
		sprintf(zGapMax, "%.2f", (atof(m_pos.zEntryPrc) - atof(m_pos.zMaxPLPrc))*m_param.dClrSpread);
		sprintf(zGapCurr, "%.2f", atof(m_pos.zEntryPrc) - atof(pzCurrPrc));
		int nComp = CUtil::CompPrc(zGapCurr, LEN_PRC, zGapMax, LEN_PRC, 2, LEN_PRC);
		if (nComp<0)
		{
			bResult = TRUE;
			sprintf(pMsg, "SHORT익절조건.(진입가:%s, 최고가:%s, 현재가:%s) (최고가GAP*%f:%s, 현재가Gap:%s)",
				m_pos.zEntryPrc, m_pos.zMaxPLPrc, pzCurrPrc, m_param.dClrSpread, zGapMax,  zGapCurr);
		}
	}

	return bResult;
}


// 진입 후 최고 수익가격을 저장하기 위해
void CStratHistManager::SetMaxPLPrc(char* pzCurrPrc)
{
	if (m_pos.zMaxPLPrc[0] == 0x00)
	{
		strcpy(m_pos.zMaxPLPrc, pzCurrPrc);
		return;
	}

	int nComp = 0;
	if (IsLong())
	{		// BUY POS. MAXPL -> HIGHER
		nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, m_pos.zMaxPLPrc, LEN_PRC, m_symbol.nDotCnt, LEN_PRC);
		if (nComp > 0)
		{
			strcpy(m_pos.zMaxPLPrc, pzCurrPrc);

			// 오픈가 대비 0.5% 조건을 건드렸는가?
			int nComp = CUtil::CompPrc(atof(pzCurrPrc), atof(m_param.zOpenPrc)*(1.0 + m_param.dEntrySpread)
				, m_symbol.nDotCnt, LEN_PRC);

			if (nComp >= 0)	m_pos.bHitPTPrc = TRUE;
		}
	}
	if (IsShort())
	{
		// SELL POS. MAXPL -> LOWER
		nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, m_pos.zMaxPLPrc, LEN_PRC, m_symbol.nDotCnt, LEN_PRC);
		if (nComp < 0)
		{
			strcpy(m_pos.zMaxPLPrc, pzCurrPrc);

			int nComp = CUtil::CompPrc(atof(pzCurrPrc), atof(m_param.zOpenPrc)*(1 - m_param.dEntrySpread)
				, m_symbol.nDotCnt, LEN_PRC);

			if (nComp <= 0)	m_pos.bHitPTPrc = TRUE;
		}
	}
}



void ST_POSITION::Init(BOOL bFirst)
{
	if (bFirst)
	{
		nTotSLCnt;	//당일 누적 sl 횟수
		nTotPTCnt;	//당일 누적 pt 횟수
		ZeroMemory(zMaxPLPrc, sizeof(zMaxPLPrc));
		bHitPTPrc = FALSE;
		bFinish = FALSE;
	}
	ZeroMemory(zEntryPrc, sizeof(zEntryPrc));
	ZeroMemory(zClrPrc, sizeof(zClrPrc));
	ZeroMemory(zPL, sizeof(zPL));
	ZeroMemory(cBsTp, sizeof(cBsTp));
	dQty = 0;
	dOrdSentQty = 0;
	Status = FLAG_NONE;
}