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

VOID CStratHistManager::SetOpenPrc(char *pzOpenPrc)
{
	strcpy(m_param.zOpenPrc, pzOpenPrc);
}



void CStratHistManager::SetInitInfo(double dTickVal, double dTickSize, int nDotCnt,
	char* pzOpenPrc, int nOrdQty, char* pzEndTM, int nMaxSLCnt, int nMaxPTCnt,
	double dEntryTrigger, double dClrCheckTrigger, double dPtClrTrigger)
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
	m_param.dEntryTrigger = dEntryTrigger;
	m_param.dClrCheckTrigger = dClrCheckTrigger;
	m_param.dPtClrTrigger = dPtClrTrigger;
	unlock();
}


// ��������
/*
char zEntryPrc[32];
char zClrPrc[32];
char zPL[32];
char cBsTp[1];
double dQty;

double dOrdSentQty;			//�ֹ������� ����. 0 �� �ƴϸ� �ֹ����� �ʴ´�.

EN_STATUS_FLAG	Status;
int				nTotSLCnt;	//���� ���� sl Ƚ��
int				nTotPTCnt;	//���� ���� pt Ƚ��
//BOOL			bProfitRealized;
char			zMaxPLPrc[LEN_PRC + 1];	//���� �� �ְ� ���Ͱ���
BOOL			bHitPTPrc;	// 0.5% ���ͱ����� �����?

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
	m_pos.bHitPTPrc = FALSE;
	m_pos.bFinish = FALSE;

	m_pos.Status = (lpCntrBsTp[0] == CD_BUY) ? FLAG_OPEN_BUY : FLAG_OPEN_SELL;
	sprintf(m_pos.zEntryPrc, "%.*s", LEN_PRC, lpCntrPrc);
	m_pos.cBsTp[0] = lpCntrBsTp[0];
	m_pos.dQty = dCntrQty;

	// �ֹ������� ���� ����
	m_pos.dOrdSentQty -= dCntrQty;

	g_log.log(INFO, "[ü��ó��]����ü��[%s](BS:%c)(���԰�:%s)(����:%f)",
		m_symbol.zSymbol, m_pos.cBsTp[0], lpCntrPrc, dCntrQty);
}


void CStratHistManager::AcptEntryAddProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	double dCntrQty = atof(lpCntrQty);
	double dAmt1 = atof(m_pos.zEntryPrc) * m_pos.dQty;
	double dAmt2 = atof(lpCntrPrc) * atof(lpCntrQty);
	double dQty = dCntrQty + m_pos.dQty;
	double dAvg = (dAmt1 + dAmt2) / dQty;

	sprintf(m_pos.zEntryPrc, "%*.*f", LEN_PRC, m_symbol.nDotCnt, dAvg);
	m_pos.dQty = dQty;

	// �ֹ������� ���� ����
	m_pos.dOrdSentQty -= dCntrQty;


	g_log.log(INFO, "[ü��ó��]�߰�����[%s](BS:%c)(���԰�:%s)(���:%s)(����:%f)",
		m_symbol.zSymbol, m_pos.cBsTp[0], lpCntrQty, m_pos.zEntryPrc, m_pos.dQty);
}

void CStratHistManager::AcptCntrProc(LPCSTR lpCntrBsTp, LPCSTR lpCntrPrc, LPCSTR lpCntrQty)
{
	lock();

	// ���� zero position
	if (m_pos.Status == FLAG_NONE)
	{
		AcptEntryNewProc(lpCntrBsTp, lpCntrPrc, lpCntrQty);
	}
	else
	{
		// ���� ������ - ���� ����
		if (m_pos.cBsTp[0] == lpCntrBsTp[0])
		{
			AcptEntryAddProc(lpCntrBsTp, lpCntrPrc, lpCntrQty);
		}
		else
		{
			// ���� ������ - �ٸ� ����
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

	double dPlTick = dPrcGap / m_symbol.dTickSize;
	double dPlMoney = 0;

	//1. ����ü��
	if (m_pos.dQty == dCntrQty)
	{
		g_log.log(INFO, "[ü��ó��]����û�����[%s]", m_symbol.zSymbol);

		dPLQty = dCntrQty;

		if (dPrcGap == 0)
		{
			g_log.log(INFO, "���� 0");
		}
		else if (dPrcGap > 0)
		{
			m_pos.IncPTCnt();
			g_log.log(INFO, "[ü��ó��]��������û��.����ī��Ʈ����(%d)[%s]", m_pos.nTotPTCnt, m_symbol.zSymbol);

			// ������ ��� open ���� �����Ѵ�.
			sprintf(m_param.zOpenPrc, lpCntrPrc);

			g_log.log(INFO, "[ü��ó��]����û���� ����.Open Price�� ���簡(%s)�� ����[%s]", lpCntrPrc, m_symbol.zSymbol);
		}
		else if (dPrcGap < 0) {
			m_pos.IncSLCnt();
			g_log.log(INFO, "[ü��ó��]��������û��.����ī��Ʈ����(%d)[%s]", m_pos.nTotSLCnt, m_symbol.zSymbol);
		}

		if (m_pos.nTotPTCnt >= m_param.nMaxCntPT)
		{
			m_pos.bFinish = TRUE;
			g_log.log(INFO, "[ü��ó��]����û��.����(%d)ȸ �������� �ŷ��ߴ�[%s]", m_pos.nTotPTCnt, m_symbol.zSymbol);
		}
		if (m_pos.nTotSLCnt >= m_param.nMaxCntSL)
		{
			m_pos.bFinish = TRUE;
			g_log.log(INFO, "[ü��ó��]����û��.����(%d)ȸ �������� �ŷ��ߴ�[%s]", m_pos.nTotSLCnt, m_symbol.zSymbol);
		}

		// �ʱ�ȭ
		m_pos.Init(FALSE);
	}
	//2. �Ϻ�ü��
	else if (m_pos.dQty > dCntrQty)
	{
		g_log.log(INFO, "[ü��ó��]�Ϻ�û��[%s]", m_symbol.zSymbol);
		dPLQty = dCntrQty;
		m_pos.dQty -= dCntrQty;

		// �ֹ������� ���� ����
		m_pos.dOrdSentQty -= dCntrQty;
	}

	//3. ����
	else if (m_pos.dQty < dCntrQty)
	{
		dPLQty = m_pos.dQty;
		m_pos.cBsTp[0] = lpCntrBsTp[0];
		m_pos.Status = (m_pos.cBsTp[0] == CD_BUY) ? FLAG_OPEN_BUY : FLAG_OPEN_SELL;

		g_log.log(INFO, "[ü��ó��] �����ǿ��� �߻�.��������(%f) ü�����(%f)[%s]",
			m_symbol.zSymbol, m_pos.dQty, dCntrQty);

		// �ֹ������� ���� ����
		m_pos.dOrdSentQty -= dCntrQty;
	}

	dPlMoney = dPlTick * m_symbol.dTickValue * dPLQty;
	g_log.log(INFO, "[ü��ó��]ü����� ���[%s]: ������(%f) ���ͼ���(%f) ����TICK(%f) ����(%f)",
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

     MAX PRC  ---------------------------
 �ð���� 0.5% ----------------------------  �������˽�����
 �ð���� 0.1% ----------------------------  ����

 ����, MAX - ���԰����� �ּ� 0.4% ���̰� ���� �Ѵ�.
 (MAX - ���԰�) / ���԰� >= 0.3%

*/
BOOL CStratHistManager::IsPTCondition(char* pzCurrPrc, _Out_ char* pMsg)
{
	BOOL bResult = FALSE;
	char zGapMax[32], zGapCurr[32];

	int nComp = CUtil::CompPrc(m_pos.zMaxPLPrc, LEN_PRC, m_pos.zEntryPrc, LEN_PRC, m_symbol.nDotCnt, LEN_PRC);
	if (nComp == 0)
		return FALSE;

	// ���� LONG
	if (IsLong() && m_pos.bHitPTPrc)
	{
		double dMaxPrc = atof(m_pos.zMaxPLPrc);
		double dEntryPrc = atof(m_pos.zEntryPrc);
		double dCurrPrc = atof(pzCurrPrc);

		// (MAX - ���԰�) / ���԰� >= 0.4%
		//double dGapPrc = (dMaxPrc - dEntryPrc) / dEntryPrc;
		//if (dGapPrc < (m_param.dClrCheckTrigger - m_param.dEntryTrigger)/2.)
		//	return FALSE;

		// (1)�ְ��϶� ������ 50%����.
		sprintf(zGapMax, "%.2f", (dMaxPrc - dEntryPrc)*m_param.dPtClrTrigger);

		// (2)���簡�� ���� ����(�������)
		sprintf(zGapCurr, "%.2f", dCurrPrc - dEntryPrc);

		// ������� < (1) �̸� �ٷ� �����Ѵ�.
		int nComp = CUtil::CompPrc(zGapCurr, LEN_PRC, zGapMax, LEN_PRC, m_symbol.nDotCnt, LEN_PRC);
		if (nComp<0)
		{
			bResult = TRUE;
			sprintf(pMsg, "[�����ߵ�][LONG��������]\n"
				"[1]( �ְ�(%s)-���԰�(%s) ) * ��������Percentage(%f)=%s\n"
				"[2]( ���簡(%s)-���԰�(%s) = %s)\n"
				"�������[2]�� �ְ����*(%s) ���� �����Ƿ� ��� ����"
				,
				m_pos.zMaxPLPrc, m_pos.zEntryPrc, m_param.dPtClrTrigger, zGapMax,
				pzCurrPrc, m_pos.zEntryPrc, zGapCurr,
				zGapMax
			);

		}
	}

	// ���� Short
	if (IsShort() && m_pos.bHitPTPrc)
	{
		double dMaxPrc = atof(m_pos.zMaxPLPrc);
		double dEntryPrc = atof(m_pos.zEntryPrc);
		double dCurrPrc = atof(pzCurrPrc);

		// (���԰� - MAX ) / ���԰� >= 0.4%
		//double dGapPrc = (dEntryPrc - dMaxPrc) / dMaxPrc;
		//if (dGapPrc < (m_param.dClrCheckTrigger - m_param.dEntryTrigger)/2.)
		//	return FALSE;

		sprintf(zGapMax, "%.2f", (dEntryPrc - dMaxPrc)*m_param.dPtClrTrigger);
		sprintf(zGapCurr, "%.2f", dEntryPrc - dCurrPrc);
		int nComp = CUtil::CompPrc(zGapCurr, LEN_PRC, zGapMax, LEN_PRC, 2, LEN_PRC);
		if (nComp<0)
		{
			bResult = TRUE;
			sprintf(pMsg, "[�����ߵ�][SHORT��������]\n"
				"[1]( ���԰�(%s)-������(%s) ) * ��������Percentage(%f)=%s\n"
				"[2]( ���԰�(%s)-���簡(%s) = %s)\n"
				"�������[2]�� �ְ����*(%s) ���� �����Ƿ� ��� ����\n"
				,
				m_pos.zEntryPrc, m_pos.zMaxPLPrc, m_param.dPtClrTrigger, zGapMax,
				m_pos.zEntryPrc, pzCurrPrc, zGapCurr,
				zGapMax
			);
		}
	}

	return bResult;
}



VOID CStratHistManager::SetConfigInfo(LPCSTR pzOpenPrc, LPCSTR pzOrdQty, LPCSTR pzEndTM, LPCSTR sMaxSL,
	LPCSTR sMaxPT, LPCSTR sEntrySpread, LPCSTR sClrSpread, LPCSTR sPtPoint)
{
	strcpy(m_param.zOpenPrc, pzOpenPrc);
	m_param.nMaxCntSL = atoi(sMaxSL);	//���� ���� sl ����Ƚ��
	m_param.nMaxCntPT = atoi(sMaxPT);	//���� ���� pt ����Ƚ��
	strcpy(m_param.zEndTM, pzEndTM);
	m_param.nOrdQty = atoi(pzOrdQty);
	m_param.dEntryTrigger = atof(sEntrySpread);
	m_param.dClrCheckTrigger = atof(sClrSpread);
	m_param.dPtClrTrigger = atof(sPtPoint);
}



VOID CStratHistManager::SetOrderSent(double dSentQty)
{
	m_pos.dOrdSentQty += dSentQty;
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
			if (!m_pos.bHitPTPrc)
			{
				int nComp = CUtil::CompPrc(atof(pzCurrPrc), atof(m_param.zOpenPrc)*(1.0 + m_param.dClrCheckTrigger)
					, m_symbol.nDotCnt, LEN_PRC);

				if (nComp >= 0) {
					m_pos.bHitPTPrc = TRUE;
					g_log.log(INFO, "[�����ߵ�]���°���� 50 Percentage ��ġ.���ݺ��� LONG�������� ����(���簡:%s)(���°�:%s)[%s]",
						pzCurrPrc, m_param.zOpenPrc, m_symbol.zSymbol);
				}
			}
		}
	}
	if (IsShort())
	{
		// SELL POS. MAXPL -> LOWER
		nComp = CUtil::CompPrc(pzCurrPrc, LEN_PRC, m_pos.zMaxPLPrc, LEN_PRC, m_symbol.nDotCnt, LEN_PRC);
		if (nComp < 0)
		{
			strcpy(m_pos.zMaxPLPrc, pzCurrPrc);

			if (!m_pos.bHitPTPrc)
			{
				int nComp = CUtil::CompPrc(atof(pzCurrPrc), atof(m_param.zOpenPrc)*(1 - m_param.dClrCheckTrigger)
					, m_symbol.nDotCnt, LEN_PRC);

				if (nComp <= 0) {
					m_pos.bHitPTPrc = TRUE;
					g_log.log(INFO, "[�����ߵ�]���°���� 50 Percentage ��ġ.���ݺ��� SHORT�������� ����(���簡:%s)(���°�:%s)[%s]", 
						pzCurrPrc, m_param.zOpenPrc, m_symbol.zSymbol);
				}
			}
		}
	}
}



void ST_POSITION::Init(BOOL bFirst)
{
	if (bFirst)
	{
		nTotSLCnt=0;	//���� ���� sl Ƚ��
		nTotPTCnt=0;	//���� ���� pt Ƚ��
		ZeroMemory(zMaxPLPrc, sizeof(zMaxPLPrc));
		bHitPTPrc = FALSE;
		bFinish = FALSE;

		g_log.log(DEBUG_, "COUNT INIT");
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