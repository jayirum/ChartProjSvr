#include "StrategyMDB.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include <stdio.h>

extern CLogMsg g_log;

CStrategyMDB::CStrategyMDB(char *pzSymbol):CStrategyMDB(pzSymbol)
{
}


CStrategyMDB::~CStrategyMDB()
{
}


BOOL CStrategyMDB::CreateMDB()
{
	m_p = new ST_MDB;
	ZeroMemory(m_p, sizeof(ST_MDB));

	g_log.log(INFO, "Succeed to create MDB");
	return TRUE;
}


BOOL CStrategyMDB::LoadSymbolAndStrategy()
{
	CDBHandlerAdo db(m_pDBPool->Get());
	char zQ[1024];
	sprintf(zQ, "EXEC MF_LOAD_SYBOL ");
	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(NOTIFY, "Load Symbol Error(%s)", zQ);
		return FALSE;
	}

	while (db->IsNextRow())
	{
		char zSymbol[128];	db->GetStr("SYMBOL", zSymbol);
		if (strcmp(zSymbol, m_zSymbol))
		{
			db->Next();
			continue;
		}

		char zArtc[128];	db->GetStr("ARTC_CD", zArtc);
		char zName[128];	db->GetStr("NAME", zName);
		double dTickVal = db->GetDouble("TICK_VALUE");
		double dTickSize = db->GetDouble("TICK_SIZE");
		int nDotCnt = db->GetLong("DOT_CNT");
		int nMaxCntSL = db->GetLong("MAXCNT_SL");
		int nMaxCntPT = db->GetLong("MAXCNT_PT");
		char zEndTM[128];	db->GetStr("END_TM", zEndTM);
		char zStartTM[128];	db->GetStr("START_TM", zStartTM);
		int nOrdQty = db->GetLong("ORD_QTY");

		strcpy(m_p->Symbol, m_zSymbol);
		strcpy(m_p->symbol.Symbol, m_zSymbol);
		strcpy(m_p->symbol.ArtcCd, zArtc);
		strcpy(m_p->symbol.SymbolName, zName);
		m_p->symbol.TickSize = dTickSize;
		m_p->symbol.TickValue = dTickVal;
		m_p->symbol.DotCnt = nDotCnt;
		m_p->param.MaxCntSL = nMaxCntSL;
		m_p->param.MaxCntPT = nMaxCntPT;
		strcpy(m_p->param.EndTM, zEndTM);
		strcpy(m_p->param.StartTM, zStartTM);
		m_p->param.OrdQty = nOrdQty;

		g_log.log(INFO, "[%s] (TickSize:%.5f)(TickVal:%.2f)(DOTCNT:%d)(MAXCNT_SL:%d)(MAXCNT_PT:%d)(START_TM:%s)(END_TM:%s)(ORD_QTY:%d)"
			, m_zSymbol
			, dTickSize
			, dTickVal
			, nDotCnt
			, nMaxCntSL
			, nMaxCntPT
			, zStartTM
			, zEndTM
			, nOrdQty
		);
		db->Next();

	} // while (db->IsNextRow())

	db->Close();
}



// Save the Max Price after open position
// and check the profit taking price is touched.
void CStrategyMDB::SetMaxPLPrc(char* pzCurrPrc)
{
	if (!IsOpenSrategyExist())
		return;

	if (MaxPLPrc() <= 0)
	{
		strcpy(m_p->tx.zMaxPLPrc, pzCurrPrc);
		return;
	}

	int nComp = 0;
	if (IsLong())	// BUY POS. MAXPL -> HIGHER
	{
		nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, m_p->tx.zMaxPLPrc, LEN_PRC, dotcnt(), LEN_PRC);
		if (nComp <= 0)
		{
			return;
		}
	}
	if (IsShort())	// BUY POS. MAXPL -> HIGHER
	{
		nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, m_p->tx.zMaxPLPrc, LEN_PRC, dotcnt(), LEN_PRC);
		if (nComp >= 0)
		{
			return;
		}
	}

	// save Max Price
	strcpy(m_p->tx.zMaxPLPrc, pzCurrPrc);

	//익절기준을 시가에서 진입가로 변경한다.20181207
	double dEntryPrc = PosPrcD();

	double dCurrPrc = atof(pzCurrPrc);

	double dTouchedPoint = 0, dTouchedPrc = 0;

	BOOL bUpdated = FALSE;

	double dSign = (IsLong()) ? 1.0 : -1.0;
	double dTouchPrc_50 = dEntryPrc * (1.0 + (dSign * m_p->param.dPT_TouchPoint_50));
	double dTouchPrc_80 = dEntryPrc * (1.0 + (dSign * m_p->param.dPT_TouchPoint_80));
	double dTouchPrc_90 = dEntryPrc * (1.0 + (dSign * m_p->param.dPT_TouchPoint_90));


	if (!IsPT_Touched_50())
	{
		if (
			(IsLong() && CUtil::CompPrc(dCurrPrc, dTouchPrc_50, dotcnt(), LEN_PRC) >= 0)
			||
			(IsShort() && CUtil::CompPrc(dCurrPrc, dTouchPrc_50, dotcnt(), LEN_PRC) <= 0)
			)
		{
			SetPT_Touched_50();
			dTouchedPoint = m_p->param.dPT_TouchPoint_50;
			dTouchedPrc = dTouchPrc_50;
			bUpdated = TRUE;
		}
	}

	if (!IsPT_Touched_80())
	{
		if (
			(IsLong() && CUtil::CompPrc(dCurrPrc, dTouchPrc_80, dotcnt(), LEN_PRC) >= 0)
			||
			(IsShort() && CUtil::CompPrc(dCurrPrc, dTouchPrc_80, dotcnt(), LEN_PRC) <= 0)
			)
		{
			SetPT_Touched_80();
			dTouchedPoint = m_p->param.dPT_TouchPoint_80;
			dTouchedPrc = dTouchPrc_80;
			bUpdated = TRUE;
		}
	}
	if (!IsPT_Touched_90())
	{
		if (
			(IsLong() && CUtil::CompPrc(dCurrPrc, dTouchPrc_90, dotcnt(), LEN_PRC) >= 0)
			||
			(IsShort() && CUtil::CompPrc(dCurrPrc, dTouchPrc_90, dotcnt(), LEN_PRC) <= 0)
			)
		{
			SetPT_Touched_90();
			dTouchedPoint = m_p->param.dPT_TouchPoint_90;
			dTouchedPrc = dTouchPrc_90;
			bUpdated = TRUE;
		}
	}

	if (bUpdated)
	{
		g_log.log(INFO, "[%s][FIRE CHECK] Touched PT Point %.2f Percent. Start PT check."
						"(CurrPrc:%s)(TouchPointPrc:%.*f)(EntryPrc:%s)",
			m_p->Symbol,
			dTouchedPoint*100.,
			pzCurrPrc,
			dotcnt(), dTouchedPrc,
			PosPrc()
		);
	}
	return;
}



void CStrategyMDB::AcptCntrProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	// 현재 zero position
	if (IsOpenSrategyExist())
	{
		AcptEntryNewProc(lpCntrBsTp, lpCntrPrc, lpCntrQty);
	}
	else
	{
		// 현재 진입중 - 같은 방향
		if (m_p->tx.PosBsTp[0] == lpCntrBsTp[0])
		{
			AcptEntryAddProc(lpCntrBsTp, lpCntrPrc, lpCntrQty);
		}
		else
		{
			// 현재 진입중 - 다른 방향
			AcptCloseProc(lpCntrBsTp, lpCntrPrc, lpCntrQty);
		}
	}

}



void CStrategyMDB::AcptCloseProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	int nCntrQty = atoi(lpCntrQty);
	double dPLQty;
	double dPrcGap;
	if (lpCntrBsTp[0] == CD_SELL)
		dPrcGap = atof(lpCntrPrc) - PosPrcD();
	else
		dPrcGap = PosPrcD() - atof(lpCntrPrc);

	double dPlTick = dPrcGap / TickSize();
	double dPlMoney = 0;


	//1. 전부체결
	if (PosQty() == nCntrQty)
	{
		dPLQty = (double)nCntrQty;

		if (dPrcGap == 0)
		{
			g_log.log(INFO, "[%s][CNTR PROC]Clear.Prfit is 0 ", Symbol());
		}
		else if (dPrcGap > 0)
		{
			IncPTCnt();
			g_log.log(INFO, "[%s][CNTR PROC]Clear.PROFIT.Inc PTCnt(%d)", Symbol(), PTCnt());
			
			if (PTCnt() >= MaxPTCnt())
			{
				SetFinished();
				g_log.log(INFO, "[%s][CNTR PROC]PT Count is (%d). Stop Strategy.",Symbol(), PTCnt());
			}
		}
		else if (dPrcGap < 0) 
		{
			IncSLCnt(); 
			g_log.log(INFO, "[%s][CNTR PROC]Clear.LOSS.Inc SLCnt(%d)", Symbol(), SLCnt());
			if (SLCnt() >= MaxSLCnt())
			{
				SetFinished();
				g_log.log(INFO, "[%s][CNTR PROC]SL Count is (%d). Stop Strategy.", Symbol(), SLCnt());
			}
		}
		m_p->tx.PosStatus = POS_NONE;
		ZeroMemory(m_p->tx.PosPrc, LEN_PRC);
		m_p->tx.PosQty = 0;
		ZeroMemory(m_p->tx.zMaxPLPrc, sizeof(m_p->tx.zMaxPLPrc));
		m_p->tx.bPT_Touched_50 = m_p->tx.bPT_Touched_80 = m_p->tx.bPT_Touched_90 = FALSE;
	}
	//2. 일부체결
	else if (PosQty() > nCntrQty)
	{
		g_log.log(INFO, "[%s][CNTR PROC]Clear Partly(PosQty:%d)(CntrQty:%d)", Symbol(), PosQty(), nCntrQty);
		dPLQty = (double)nCntrQty;
		m_p->tx.PosQty -= nCntrQty;

		// 주문전송한 수량 차감
		ReleaseSentOrdQty(nCntrQty);
	}

	//3. 역전
	else if (PosQty() < nCntrQty)
	{
		g_log.log(INFO, "[%s][CNTR PROC]Reverse(PosQty:%d)(CntrQty:%d)", Symbol(), PosQty(), nCntrQty);

		
		dPLQty =(double)PosQty();
		m_p->tx.PosQty = nCntrQty - m_p->tx.PosQty;
		m_p->tx.PosBsTp[0] = lpCntrBsTp[0];
		m_p->tx.PosStatus = POS_OPEN;

		// 주문전송한 수량 차감
		ReleaseSentOrdQty(nCntrQty);

		ZeroMemory(m_p->tx.zMaxPLPrc, sizeof(m_p->tx.zMaxPLPrc));
		m_p->tx.bPT_Touched_50 = m_p->tx.bPT_Touched_80 = m_p->tx.bPT_Touched_90 = FALSE;
	}

	dPlMoney = dPlTick * TickValue() * dPLQty;
	g_log.log(INFO, "[%s][Calc Cntr PL] Prc Gap(%f) CalcQty(%f) PL Tick(%f) ==> PL(%f)",
		Symbol(), dPrcGap, dPLQty, dPlTick, dPlMoney);
}


void CStrategyMDB::AcptEntryAddProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	int nCntrQty = atoi(lpCntrQty);
	double dAmt1 = PosPrcD() * (double)PosQty();
	double dAmt2 = atof(lpCntrPrc) * atof(lpCntrQty);
	double dAvg = (dAmt1 + dAmt2) / (atof(lpCntrQty)+(double)PosQty());

	sprintf(m_p->tx.PosPrc, "%*.*f", LEN_PRC, dotcnt(), dAvg);
	m_p->tx.PosQty += nCntrQty;

	// 주문전송한 수량 차감
	ReleaseSentOrdQty(nCntrQty);

	g_log.log(INFO, "[%s][CNTR PROC][ADD][%s](BS:%c)(CntrPrc:%s)(평단:%s)(수량:%f)",
		m_p->Symbol, lpCntrBsTp[0], lpCntrPrc, PosPrc(), lpCntrQty);
}

void CStrategyMDB::AcptEntryNewProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	// Init
	ZeroMemory(m_p->tx.PosPrc, sizeof(m_p->tx.PosPrc));
	ZeroMemory(m_p->tx.ClrPrc, sizeof(m_p->tx.ClrPrc));
	ZeroMemory(m_p->tx.PL, sizeof(m_p->tx.PL));
	m_p->tx.PosBsTp[0] = 0;
	m_p->tx.PosStatus = POS_NONE;
	ZeroMemory(m_p->tx.zMaxPLPrc, sizeof(m_p->tx.zMaxPLPrc));
	m_p->tx.bPT_Touched_50 = m_p->tx.bPT_Touched_80 = m_p->tx.bPT_Touched_90 = FALSE;
	m_p->tx.FinishedYN[0] = 'N';

	m_p->tx.PosStatus = POS_OPEN;
	m_p->tx.PosBsTp[0] = lpCntrBsTp[0];
	sprintf(m_p->tx.PosPrc, "%.*s", LEN_PRC, lpCntrPrc);
	m_p->tx.PosQty = atoi(lpCntrQty);

	// 주문전송한 수량 차감
	ReleaseSentOrdQty(atoi(lpCntrQty));

	g_log.log(INFO, "[%s][CNTR PROC][OPEN](BS:%c)(Prc:%s)(Qty:%s)",
		m_p->Symbol, lpCntrBsTp[0], lpCntrPrc, lpCntrQty);
}

//
//BOOL CStrategyMDB::Read(_In_ char* pzSymbol)
//{
//	char zSymbol[32];
//	FMT_SYMBOL(pzSymbol, zSymbol);
//	if (!m_shm->GetData(zSymbol, zSymbol, (char *)&m_p)) {
//		m_bAlreadRead = FALSE;
//		return FALSE;
//	}
//	m_bAlreadRead = TRUE;
//	m_nDotCnt = S2N(m_p.symbol.DotCnt, sizeof(m_p.symbol.DotCnt));
//	return TRUE;
//}
