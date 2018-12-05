#include "../../IRUM_UTIL/ADOFunc.h"
#include "TradeOption.h"
#include "StratHistManager.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/LogMsg.h"

extern CLogMsg g_log;
extern char			g_zConfig[_MAX_PATH];

CStratHistManager::CStratHistManager(char* pzSymbol)
{
	ZeroMemory(&m_symbol, sizeof(m_symbol));
	ZeroMemory(&m_param, sizeof(m_param));
	m_pos.Init(TRUE);

	m_symbol.set_symbol(pzSymbol);

	InitializeCriticalSection(&m_cs);
}


CStratHistManager::~CStratHistManager()
{
	DeleteCriticalSection(&m_cs);
}

VOID CStratHistManager::SetOpenPrc(char *pzOpenPrc)
{
	strcpy(m_param.zOpenPrc, pzOpenPrc);
}

BOOL CStratHistManager::LoadStratInfo()
{
	BOOL bRet = FALSE;;
	__try
	{
		bRet = LoadStratInfoInner();
	}
	__except (ReportException(GetExceptionCode(), "LoadStratInfo", m_zMsg))
	{
		g_log.log(ERR, "[LoadStratInfo ERROR](%s)", m_zMsg);
	}
	return bRet;
}

BOOL CStratHistManager::LoadStratInfoInner()
{
	char ip[32], id[32], pwd[32], name[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", name);
	//CUtil::GetConfig(g_zConfig, "DBINFO", "DB_POOL_CNT", cnt);

	CDBPoolAdo* dbPool = new CDBPoolAdo(ip, id, pwd, name);
	if (!dbPool->Init(1))
	{
		return FALSE;
	}

	//CDBHandlerAdo db(dbPool);
	CADOFunc* db = dbPool->GetAvailableDB();
	char zQ[1024];
	sprintf(zQ, "EXEC ABOT_NO1_LOAD_SYMBOL_INDI '%s' ", m_symbol.symbol());
	if (FALSE == db->ExecQuery(zQ))
	{
		delete dbPool;
		return FALSE;
	}
	else
	{
		int nLen = 0;
		if (db->IsNextRow())
		{
			//Sleep(10);
			char zArtc[512]={ 0, };
			db->GetStr(0L, zArtc);
			if (strlen(zArtc) == 0)
			{
				g_log.log(ERR, "[ADO ERROR](%s)", db->GetError());
				int a = 0;
			}
			m_symbol.set_artc(zArtc);
			char zName[128];	db->GetStr("NAME", zName);		m_symbol.set_name(zName);
			m_symbol.set_tickval(db->GetDouble("TICK_VALUE"));
			m_symbol.set_ticksize(db->GetDouble("TICK_SIZE"));
			m_symbol.set_dotcnt(db->GetLong("DOT_CNT"));

			m_param.nOrdQty = db->GetLong("ORD_QTY");
			db->GetStr("END_TM", m_param.zEndTM);
			db->GetStr("START_TM", m_param.zStartTM);
			m_param.nMaxCntSL = db->GetLong("MAXCNT_SL");
			m_param.nMaxCntPT = db->GetLong("MAXCNT_PT");


			m_param.dEntryTouchPoint = db->GetDouble("ENTRY_TOUCHPOINT_PCT")/100.;	//0.1%
			m_param.dPT50_TouchPoint = db->GetDouble("PT50_CHK_TOUCHPOINT_PCT")/100.;	//0.5%
			m_param.dPT80_TouchPoint = db->GetDouble("PT80_CHK_TOUCHPOINT_PCT")/100.;	//0.8%
			m_param.dPT90_TouchPoint = db->GetDouble("PT90_CHK_TOUCHPOINT_PCT")/100.;	//0.9%

			db->Next();
		}
		db->Close();
	}

	delete dbPool;
	return TRUE;
}
//
//void CStratHistManager::SetInitInfo(double dTickVal, double dTickSize, int nDotCnt,
//	char* pzOpenPrc, int nOrdQty, char* pzStartTM, char* pzEndTM, int nMaxSLCnt, int nMaxPTCnt,
//	double dEntryTouchPoint, double dPT50_TouchPoint, double dPtClrTrigger)
//{
//	lock();
//	m_symbol.set_tickval(dTickVal);
//	m_symbol.set_ticksize(dTickSize);
//	m_symbol.set_dotcnt(nDotCnt);
//	strcpy(m_param.zOpenPrc, pzOpenPrc);
//	strcpy(m_param.zStartTM, pzStartTM);
//	strcpy(m_param.zEndTM, pzEndTM);
//	m_param.nOrdQty = nOrdQty;
//	m_param.nMaxCntSL = nMaxSLCnt;
//	m_param.nMaxCntPT = nMaxPTCnt;
//	m_param.dEntryTouchPoint = dEntryTouchPoint;
//	m_param.dPT50_TouchPoint = dPT50_TouchPoint;
//	m_param.dPtClrTrigger = dPtClrTrigger;
//	unlock();
//}


// 최초진입
/*
char zEntryPrc[32];
char zClrPrc[32];
char zPL[32];
char cBsTp[1];
double dQty;

double dOrdSentQty;			//주문전송한 수량. 0 이 아니면 주문하지 않는다.

EN_STATUS_FLAG	Status;
int				nTotSLCnt;	//당일 누적 sl 횟수
int				nTotPTCnt;	//당일 누적 pt 횟수
//BOOL			bProfitRealized;
char			zMaxPLPrc[LEN_PRC + 1];	//진입 후 최고 수익가격
BOOL			bHitPTPrc;	// 0.5% 수익구간을 찍었나?

BOOL			bFinish;
*/
void CStratHistManager::AcptEntryNewProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	double dCntrQty = atof(lpCntrQty);
	
	ZeroMemory(m_pos.zEntryPrc, sizeof(m_pos.zEntryPrc));
	ZeroMemory(m_pos.zClrPrc, sizeof(m_pos.zEntryPrc));
	ZeroMemory(m_pos.zPL, sizeof(m_pos.zPL));
	m_pos.cBsTp[0] = 0;
	m_pos.Status = FLAG_NONE;
	ZeroMemory(m_pos.zMaxPLPrc, sizeof(m_pos.zMaxPLPrc));
	m_pos.bPT50Touched = m_pos.bPT80Touched = m_pos.bPT90Touched = FALSE;
	m_pos.bFinish = FALSE;

	m_pos.Status = (lpCntrBsTp[0] == CD_BUY) ? FLAG_OPEN_BUY : FLAG_OPEN_SELL;
	sprintf(m_pos.zEntryPrc, "%.*s", LEN_PRC, lpCntrPrc);
	m_pos.cBsTp[0] = lpCntrBsTp[0];
	m_pos.dQty = dCntrQty;

	// 주문전송한 수량 차감
	m_pos.dOrdSentQty -= dCntrQty;

	g_log.log(INFO, "[체결처리]진입체결[%s](BS:%c)(진입가:%s)(수량:%f)",
		m_symbol.symbol(), m_pos.cBsTp[0], lpCntrPrc, dCntrQty);
}


void CStratHistManager::AcptEntryAddProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	double dCntrQty = atof(lpCntrQty);
	double dAmt1 = atof(m_pos.zEntryPrc) * m_pos.dQty;
	double dAmt2 = atof(lpCntrPrc) * atof(lpCntrQty);
	double dQty = dCntrQty + m_pos.dQty;
	double dAvg = (dAmt1 + dAmt2) / dQty;

	sprintf(m_pos.zEntryPrc, "%*.*f", LEN_PRC, m_symbol.dotcnt(), dAvg);
	m_pos.dQty = dQty;

	// 주문전송한 수량 차감
	m_pos.dOrdSentQty -= dCntrQty;


	g_log.log(INFO, "[체결처리]추가진입[%s](BS:%c)(진입가:%s)(평단:%s)(수량:%f)",
		m_symbol.symbol(), m_pos.cBsTp[0], lpCntrQty, m_pos.zEntryPrc, m_pos.dQty);
}

void CStratHistManager::AcptCntrProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	lock();

	// 현재 zero position
	if (m_pos.Status == FLAG_NONE)
	{
		AcptEntryNewProc(lpCntrBsTp, lpCntrPrc, lpCntrQty);
	}
	else
	{
		// 현재 진입중 - 같은 방향
		if (m_pos.cBsTp[0] == lpCntrBsTp[0])
		{
			AcptEntryAddProc(lpCntrBsTp, lpCntrPrc, lpCntrQty);
		}
		else
		{
			// 현재 진입중 - 다른 방향
			AcptCloseProc(lpCntrBsTp, lpCntrPrc, lpCntrQty);
		}
	}

	unlock();
}

void CStratHistManager::AcptCloseProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	double dCntrQty = atof(lpCntrQty);
	double dPLQty;
	double dPrcGap;
	if (lpCntrBsTp[0] == CD_SELL)
		dPrcGap = atof(lpCntrPrc) - atof(m_pos.zEntryPrc);
	else
		dPrcGap = atof(m_pos.zEntryPrc) - atof(lpCntrPrc);

	double dPlTick = dPrcGap / m_symbol.ticksize();
	double dPlMoney = 0;

	//1. 전부체결
	if (m_pos.dQty == dCntrQty)
	{
		g_log.log(INFO, "[체결처리]전부청산시작[%s]", m_symbol.symbol());

		dPLQty = dCntrQty;

		if (dPrcGap == 0)
		{
			g_log.log(INFO, "손익 0");
		}
		else if (dPrcGap > 0)
		{
			m_pos.IncPTCnt();
			g_log.log(INFO, "[체결처리]익절전부청산.익절카운트증가(%d)[%s]", m_pos.nTotPTCnt, m_symbol.symbol());

			// 익절인 경우 open 가를 변경한다.
			sprintf(m_param.zOpenPrc, lpCntrPrc);

			g_log.log(INFO, "[체결처리]전부청산이 익절.Open Price를 현재가(%s)로 변경[%s]", lpCntrPrc, m_symbol.symbol());
		}
		else if (dPrcGap < 0) {
			m_pos.IncSLCnt();
			g_log.log(INFO, "[체결처리]손절전부청산.손절카운트증가(%d)[%s]", m_pos.nTotSLCnt, m_symbol.symbol());
		}

		if (m_pos.nTotPTCnt >= m_param.nMaxCntPT)
		{
			m_pos.bFinish = TRUE;
			g_log.log(INFO, "[체결처리]전부청산.익절(%d)회 수행으로 거래중단[%s]", m_pos.nTotPTCnt, m_symbol.symbol());
		}
		if (m_pos.nTotSLCnt >= m_param.nMaxCntSL)
		{
			m_pos.bFinish = TRUE;
			g_log.log(INFO, "[체결처리]전부청산.손절(%d)회 수행으로 거래중단[%s]", m_pos.nTotSLCnt, m_symbol.symbol());
		}

		// 초기화
		m_pos.Init(FALSE);
	}
	//2. 일부체결
	else if (m_pos.dQty > dCntrQty)
	{
		g_log.log(INFO, "[체결처리]일부청산[%s]", m_symbol.symbol());
		dPLQty = dCntrQty;
		m_pos.dQty -= dCntrQty;

		// 주문전송한 수량 차감
		m_pos.dOrdSentQty -= dCntrQty;
	}

	//3. 역전
	else if (m_pos.dQty < dCntrQty)
	{
		dPLQty = m_pos.dQty;
		m_pos.cBsTp[0] = lpCntrBsTp[0];
		m_pos.Status = (m_pos.cBsTp[0] == CD_BUY) ? FLAG_OPEN_BUY : FLAG_OPEN_SELL;

		g_log.log(INFO, "[체결처리] 포지션역전 발생.기존수량(%f) 체결수량(%f)[%s]",
			m_symbol.symbol(), m_pos.dQty, dCntrQty);

		// 주문전송한 수량 차감
		m_pos.dOrdSentQty -= dCntrQty;
	}

	dPlMoney = dPlTick * m_symbol.tickval() * dPLQty;
	g_log.log(INFO, "[체결처리]체결손익 계산[%s]: 가격차(%f) 손익수량(%f) 손익TICK(%f) 손익(%f)",
		m_symbol.symbol(), dPrcGap, dPLQty, dPlTick, dPlMoney);
}


double	CStratHistManager::GetPTLevelTouched()
{
	if (m_pos.bPT90Touched)
		return PTRATE_90;
	if (m_pos.bPT80Touched)
		return PTRATE_80;
	if (m_pos.bPT50Touched)
		return PTRATE_50;

	return 0;
}


/*
	// 익절 자리 touch 하면 주문
*/
BOOL CStratHistManager::IsProfitTakingCondition(
	char* pzCurrPrc, void* pOption, _Out_ char* pMsg, _Out_ char* pzStratID
)
{
	// 50%, 80%, 90%
	double dPTRate = GetPTLevelTouched();
	if (dPTRate == 0)
		return FALSE;

	// MAXPLPrc == EntryPrc ==> skip
	int nComp = CUtil::CompPrc(m_pos.zMaxPLPrc, LEN_PRC, m_pos.zEntryPrc, LEN_PRC, m_symbol.dotcnt(), LEN_PRC);
	if (nComp == 0)
		return FALSE;

	double dPrcMax = atof(m_pos.zMaxPLPrc);
	double dPrcEntry = atof(m_pos.zEntryPrc);
	double dPrcCurr = atof(pzCurrPrc);
	double dPrcFire = 0;
	double dPrcGap = 0;

	BOOL bResult = FALSE;

	// LONG 일때, 현재가 < FIRE PRC 이면 익절
	if (IsLong())
	{
		dPrcGap = (dPrcMax - dPrcEntry) * dPTRate;
		dPrcFire = dPrcEntry + dPrcGap;
		nComp = CUtil::CompPrc(dPrcCurr, dPrcFire, m_symbol.dotcnt(), LEN_PRC);
	}

	// SHORT 일때, 현재가 > FIRE PRC 이면 익절
	if (IsShort())
	{
		dPrcGap = (dPrcEntry - dPrcMax) * dPTRate;
		dPrcFire = dPrcEntry - dPrcGap;
		nComp = CUtil::CompPrc(dPrcFire, dPrcCurr, m_symbol.dotcnt(), LEN_PRC);
	}
	if (nComp <= 0)
	{
		if (IsLong())	strcpy(pzStratID, STRATID_SELL_PT);
		else			strcpy(pzStratID, STRATID_BUY_PT);

		bResult = TRUE;
		sprintf(pMsg, "[전략발동][익절조건]"
			"(최고대비 %.2f Percent = %.*f) >= 현재가(%.*f)"
			,
			dPTRate*100., m_symbol.dotcnt(), dPrcFire, m_symbol.dotcnt(), dPrcCurr
		);

		g_log.log(INFO, pMsg);
	}


	return bResult;
}


//VOID CStratHistManager::SetConfigInfo(LPCSTR pzOpenPrc, LPCSTR pzOrdQty, LPCSTR pzEndTM, LPCSTR sMaxSL,
//	LPCSTR sMaxPT, LPCSTR sEntrySpread, LPCSTR sClrSpread, LPCSTR sPtPoint)
//{
//	strcpy(m_param.zOpenPrc, pzOpenPrc);
//	m_param.nMaxCntSL = atoi(sMaxSL);	//당일 누적 sl 제한횟수
//	m_param.nMaxCntPT = atoi(sMaxPT);	//당일 누적 pt 제한횟수
//	strcpy(m_param.zEndTM, pzEndTM);
//	m_param.nOrdQty = atoi(pzOrdQty);
//	m_param.dEntryTouchPoint = atof(sEntrySpread);
//	m_param.dPT50_TouchPoint = atof(sClrSpread);
//	m_param.dPtClrTrigger = atof(sPtPoint);
//}



VOID CStratHistManager::SetOrderSent(double dSentQty)
{
	m_pos.dOrdSentQty += dSentQty;
}


// Save the Max Price after open position
// and check the profit taking price is touched.
void CStratHistManager::SetMaxPLPrc(char* pzCurrPrc)
{
	if (m_pos.Status == FLAG_NONE)
		return;

	if (m_pos.zMaxPLPrc[0] == 0x00)
	{
		strcpy(m_pos.zMaxPLPrc, pzCurrPrc);
		return;
	}

	int nComp = 0;
	if (IsLong())	// BUY POS. MAXPL -> HIGHER
	{		
		nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, m_pos.zMaxPLPrc, LEN_PRC, m_symbol.dotcnt(), LEN_PRC);
		if (nComp <= 0)
		{
			return;
		}
	}
	if (IsShort())	// BUY POS. MAXPL -> HIGHER
	{		
		nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, m_pos.zMaxPLPrc, LEN_PRC, m_symbol.dotcnt(), LEN_PRC);
		if (nComp >= 0)
		{
			return;
		}
	}

	// save Max Price
	strcpy(m_pos.zMaxPLPrc, pzCurrPrc);

	double dTouchPrc = 0;
	double dOpenPrc = atof(m_param.zOpenPrc);
	double dTouchPoint = 0;
	int nLevel = 0;
	if (!m_pos.bPT50Touched)
	{
		dTouchPoint = m_param.dPT50_TouchPoint;
		nLevel = 1;
	}
	else
	{
		if (!m_pos.bPT80Touched)
		{
			dTouchPoint = m_param.dPT80_TouchPoint;
			nLevel = 2;
		}
		else if (!m_pos.bPT90Touched)
		{
			dTouchPoint = m_param.dPT90_TouchPoint;
			nLevel = 3;
		}
	}

	if (IsLong())
	{
		// 현재가 : 1.2720,  시가:1.2718  0.1%:1.2731  0.5%:1.2781   
		dTouchPrc = dOpenPrc * (1.0 + dTouchPoint);
		nComp = CUtil::CompPrc(atof(pzCurrPrc), dTouchPrc, m_symbol.dotcnt(), LEN_PRC);
		if (nComp < 0)
			return;
	}
	if (IsShort())
	{
		// 현재가 : 1.2707,  시가:1.2718  0.1%:1.2705  0.5%:1.2667  
		dTouchPrc = dOpenPrc * (1.0 - dTouchPoint);
		nComp = CUtil::CompPrc(atof(pzCurrPrc), dTouchPrc, m_symbol.dotcnt(), LEN_PRC);
		if (nComp > 0)
			return;
	}

	
	
	if (nLevel == 1)	m_pos.bPT50Touched = TRUE;
	if (nLevel == 2)	m_pos.bPT80Touched = TRUE;
	if (nLevel == 3)	m_pos.bPT90Touched = TRUE;

	g_log.log(INFO, "[전략발동]오픈가대비 %.*f Percent 터치.지금부터 익절조건 점검(현재가:%s)(기준가:%.*f)(오픈가:%s)[%s]",
			m_symbol.dotcnt(),dTouchPoint*100., pzCurrPrc, m_symbol.dotcnt(), dTouchPrc, m_param.zOpenPrc, m_symbol.symbol());

	return;
}



void ST_POSITION::Init(BOOL bFirst)
{
	if (bFirst)
	{
		nTotSLCnt=0;	//당일 누적 sl 횟수
		nTotPTCnt=0;	//당일 누적 pt 횟수
		ZeroMemory(zMaxPLPrc, sizeof(zMaxPLPrc));
		bPT50Touched = bPT80Touched = bPT90Touched = FALSE;
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

void ST_POSITION::IncSLCnt()
{
	nTotSLCnt++;
}
void ST_POSITION::IncPTCnt()
{
	nTotPTCnt++;
}





//
//
///*
//진입 후 최고수익의 50% 이하로 떨어져서 익절 할 조건인가?
//
//ex)
//매수진입 100일때 (최고가 - 진입가)
//최고가 120  ==> gap : 20
//현재가 115  ==> gap : 15 -> 익절 안한다.
//현재가 109  ==> gap : 9  -> 익절한다.
//현재가 99   ==> gap : -1 -> 익절한다.
//
//매도진입 100일때 (진입가 - 최고가)
//최고가 80  ==> gap : 20
//현재가 85  ==> gap : 15 -> 익절 안한다.
//현재가 95  ==> gap : 5  -> 익절한다.
//현재가 101   ==> gap : -1 -> 익절한다.
//
//MAX PRC  ---------------------------
//시가대비 0.5% ----------------------------  익절점검시작점
//시가대비 0.1% ----------------------------  진입
//
//따라서, MAX - 진입가격은 최소 0.4% 차이가 나야 한다.
//(MAX - 진입가) / 진입가 >= 0.3%
//
//*/
//BOOL CStratHistManager::IsPTCondition(char* pzCurrPrc, _Out_ char* pMsg, _Out_ char* pDbLog)
//{
//	BOOL bResult = FALSE;
//	char zGapMax[32], zGapCurr[32];
//
//	ABOTLOG_NO1 *dblog = (ABOTLOG_NO1*)pDbLog;
//
//	int nComp = CUtil::CompPrc(m_pos.zMaxPLPrc, LEN_PRC, m_pos.zEntryPrc, LEN_PRC, m_symbol.dotcnt(), LEN_PRC);
//	if (nComp == 0)
//		return FALSE;
//
//	// 현재 LONG
//	if (IsLong() && m_pos.bHitPTPrc)
//	{
//		double dMaxPrc = atof(m_pos.zMaxPLPrc);
//		double dEntryPrc = atof(m_pos.zEntryPrc);
//		double dCurrPrc = atof(pzCurrPrc);
//
//		// (MAX - 진입가) / 진입가 >= 0.4%
//		//double dGapPrc = (dMaxPrc - dEntryPrc) / dEntryPrc;
//		//if (dGapPrc < (m_param.dPT50_TouchPoint - m_param.dEntryTouchPoint)/2.)
//		//	return FALSE;
//
//		// (1)최고가일때 수익의 50%수준.
//		sprintf(zGapMax, "%.2f", (dMaxPrc - dEntryPrc)*m_param.dPtClrTrigger);
//
//		// (2)현재가에 의한 수익(현재수익)
//		sprintf(zGapCurr, "%.2f", dCurrPrc - dEntryPrc);
//
//		// 현재수익 < (1) 이면 바로 익절한다.
//		int nComp = CUtil::CompPrc(zGapCurr, LEN_PRC, zGapMax, LEN_PRC, m_symbol.dotcnt(), LEN_PRC);
//		if (nComp<0)
//		{
//			bResult = TRUE;
//			sprintf(pMsg, "[전략발동][LONG익절조건]\n"
//				"[1]( 최고가(%s)-진입가(%s) ) * 익절조건Percentage(%f)=%s\n"
//				"[2]( 현재가(%s)-진입가(%s) = %s)\n"
//				"현재수익[2]이 최고수익*(%s) 보다 작으므로 즉시 익절"
//				,
//				m_pos.zMaxPLPrc, m_pos.zEntryPrc, m_param.dPtClrTrigger, zGapMax,
//				pzCurrPrc, m_pos.zEntryPrc, zGapCurr,
//				zGapMax
//			);
//
//			//DB LOG
//			sprintf(dblog->zMaxPrc, m_pos.zMaxPLPrc);
//			sprintf(dblog->zPtClrTriggerPercent, "%.2f", m_param.dPtClrTrigger * 100);
//		}
//	}
//
//	// 현재 Short
//	if (IsShort() && m_pos.bHitPTPrc)
//	{
//		double dMaxPrc = atof(m_pos.zMaxPLPrc);
//		double dEntryPrc = atof(m_pos.zEntryPrc);
//		double dCurrPrc = atof(pzCurrPrc);
//
//		// (진입가 - MAX ) / 진입가 >= 0.4%
//		//double dGapPrc = (dEntryPrc - dMaxPrc) / dMaxPrc;
//		//if (dGapPrc < (m_param.dPT50_TouchPoint - m_param.dEntryTouchPoint)/2.)
//		//	return FALSE;
//
//		sprintf(zGapMax, "%.2f", (dEntryPrc - dMaxPrc)*m_param.dPtClrTrigger);
//		sprintf(zGapCurr, "%.2f", dEntryPrc - dCurrPrc);
//		int nComp = CUtil::CompPrc(zGapCurr, LEN_PRC, zGapMax, LEN_PRC, dotcnt(), LEN_PRC);
//		if (nComp<0)
//		{
//			bResult = TRUE;
//			sprintf(pMsg, "[전략발동][SHORT익절조건]\n"
//				"[1]( 진입가(%s)-최저가(%s) ) * 익절조건Percentage(%f)=%s\n"
//				"[2]( 진입가(%s)-현재가(%s) = %s)\n"
//				"현재수익[2]이 최고수익*(%s) 보다 작으므로 즉시 익절\n"
//				,
//				m_pos.zEntryPrc, m_pos.zMaxPLPrc, m_param.dPtClrTrigger, zGapMax,
//				m_pos.zEntryPrc, pzCurrPrc, zGapCurr,
//				zGapMax
//			);
//
//			//DB LOG
//			sprintf(dblog->zMaxPrc, m_pos.zMaxPLPrc);
//			sprintf(dblog->zPtClrTriggerPercent, "%.2f", m_param.dPtClrTrigger * 100);
//		}
//	}
//
//	return bResult;
//}
