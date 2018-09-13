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

	// ���� zero position
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
		// ���� ������ - ���� ����
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
			// ���� ������ - �ٸ� ����
			CalcClrCntr(lpBsTp, lpCntrPrc, lpCntrQty);
		}
	}

	// �ֹ������� ���� ����
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

	//1. ����ü��
	if (m_pos.dQty == dCntrQty)
	{
		dPLQty = dCntrQty;

		if (dPrcGap > 0) 
		{
			m_pos.nTotPTCnt++;

			// ������ ��� open ���� �����Ѵ�.
			sprintf(m_param.zOpenPrc, lpCntrPrc);
		}
		if (dPrcGap < 0)	m_pos.nTotSLCnt++;

		if (m_pos.nTotPTCnt >= m_param.nMaxCntPT)
		{
			m_pos.bFinish = TRUE;
			g_log.log(LOGTP_SUCC, "[%s]����(%d)ȸ �������� �ŷ��ߴ�.", m_symbol.zSymbol, m_pos.nTotPTCnt);

		}
		if (m_pos.nTotSLCnt >= m_param.nMaxCntSL)
		{
			m_pos.bFinish = TRUE;
			g_log.log(LOGTP_SUCC, "[%s]����(%d)ȸ �������� �ŷ��ߴ�.", m_symbol.zSymbol, m_pos.nTotPTCnt);
		}
		m_pos.Init(FALSE);
		
	}

	//2. �Ϻ�ü��
	if (m_pos.dQty > dCntrQty)
	{
		dPLQty = dCntrQty;
		m_pos.dQty -= dCntrQty;
	}

	//3. ����
	if (m_pos.dQty < dCntrQty)
	{
		dPLQty = m_pos.dQty;
		m_pos.cBsTp[0] = lpBsTp[0];
		m_pos.Status = (m_pos.cBsTp[0] == CD_BUY) ? FLAG_OPEN_BUY : FLAG_OPEN_SELL;

		g_log.log(LOGTP_ERR, "[%s] �����ǿ��� �߻�.��������(%f) ü�����(%f)",
			m_symbol.zSymbol, m_pos.dQty, dCntrQty);
	}
	dPlMoney = dPlTick * m_symbol.dTickValue * dPLQty;
	g_log.log(LOGTP_SUCC, "[%s] ������(%f) ���ͼ���(%f) ����TICK(%f) ����(%f)",
		m_symbol.zSymbol, dPrcGap, dPLQty, dPlTick, dPlMoney);
}



/*
	���� �� �ְ������ 50% ���Ϸ� �������� ���� �� �����ΰ�?

ex)
	�ż����� 100�϶� (�ְ� - ���԰�)
		�ְ� 120  ==> gap : 20
		���簡 115  ==> gap : 15 -> ���� ���Ѵ�.
		���簡 109  ==> gap : 9  -> �����Ѵ�.
		���簡 99   ==> gap : -1 -> �����Ѵ�.

	�ŵ����� 100�϶� (���԰� - �ְ�)
		�ְ� 80  ==> gap : 20
		���簡 85  ==> gap : 15 -> ���� ���Ѵ�.
		���簡 95  ==> gap : 5  -> �����Ѵ�.
		���簡 101   ==> gap : -1 -> �����Ѵ�.
*/
BOOL CStratHistManager::IsPTCondition(char* pzCurrPrc,_Out_ char* pMsg)
{
	BOOL bResult = FALSE;
	char zGapMax[32], zGapCurr[32];

	int nComp = CUtil::CompPrc(m_pos.zMaxPLPrc, LEN_PRC, m_pos.zEntryPrc, LEN_PRC, m_symbol.nDotCnt, LEN_PRC);
	if (nComp == 0)
		return FALSE;
	
	// ���� LONG
	if (IsLong())
	{
		sprintf(zGapMax, "%.2f", (atof(m_pos.zMaxPLPrc) - atof(m_pos.zEntryPrc))*m_param.dClrSpread);
		sprintf(zGapCurr, "%.2f", atof(pzCurrPrc) - atof(m_pos.zEntryPrc));
		int nComp = CUtil::CompPrc(zGapCurr, LEN_PRC, zGapMax, LEN_PRC, m_symbol.nDotCnt, LEN_PRC);
		if (nComp<0)
		{
			bResult = TRUE;
			sprintf(pMsg, "LONG��������.(���԰�:%s, �ְ�:%s, ���簡:%s) (�ְ�GAP*%f:%s, ���簡Gap:%s)",
				m_pos.zEntryPrc, m_pos.zMaxPLPrc, pzCurrPrc, m_param.dClrSpread, zGapMax, zGapCurr);
		}
	}

	// ���� Short
	if (IsShort())
	{
		sprintf(zGapMax, "%.2f", (atof(m_pos.zEntryPrc) - atof(m_pos.zMaxPLPrc))*m_param.dClrSpread);
		sprintf(zGapCurr, "%.2f", atof(m_pos.zEntryPrc) - atof(pzCurrPrc));
		int nComp = CUtil::CompPrc(zGapCurr, LEN_PRC, zGapMax, LEN_PRC, 2, LEN_PRC);
		if (nComp<0)
		{
			bResult = TRUE;
			sprintf(pMsg, "SHORT��������.(���԰�:%s, �ְ�:%s, ���簡:%s) (�ְ�GAP*%f:%s, ���簡Gap:%s)",
				m_pos.zEntryPrc, m_pos.zMaxPLPrc, pzCurrPrc, m_param.dClrSpread, zGapMax,  zGapCurr);
		}
	}

	return bResult;
}


// ���� �� �ְ� ���Ͱ����� �����ϱ� ����
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

			// ���°� ��� 0.5% ������ �ǵ�ȴ°�?
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
		nTotSLCnt;	//���� ���� sl Ƚ��
		nTotPTCnt;	//���� ���� pt Ƚ��
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