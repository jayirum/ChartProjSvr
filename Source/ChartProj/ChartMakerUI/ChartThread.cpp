#include "stdafx.h"
#include "ChartThread.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include "C:\ta-lib\c\include\ta_libc.h"

extern CLogMsg g_log;
#define DBPoolPtr ((CDBPoolAdo*)m_pDBPool)

CChartThread::CChartThread(CString sSymbol, char* pzArtcCode, Poco::MemoryPool* pMemPool, CNanoSocket* sock)
{
	m_sSymbol = sSymbol;
	m_pMemPool = pMemPool;
	m_pSock = sock;

	GET_SHM_NM(pzArtcCode, m_zShmNm);
	GET_SHM_LOCK_NM(pzArtcCode, m_zMutexNm);
	strcpy(m_zArtc, pzArtcCode);
}


CChartThread::~CChartThread()
{
	CloseChartShm();
}


VOID CChartThread::ThreadFunc()
{
	g_log.log(LOGTP_SUCC, "[%s] Chart Thread Start...", Symbol());
	
	if (!InitChartSHM())
	{
		g_log.log(LOGTP_ERR, ">>>>>> SHM OPEN ERROR <<<<<<<<<");
		return;
	}

	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, INFINITE, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) {
			g_log.log(LOGTP_ERR,  "[THREAD ID:%d]WAIT ERROR(%d)", GetMyThreadID(), GetLastError());
			Sleep(1000);
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_CHART_ALL_KILL:
				g_log.log(LOGTP_SUCC, "[THREAD ID:%d] Recv Kill Msg", GetMyThreadID());
				break;
			case WM_CHART_DATA:			
				{
					int i = 0;
					//for (i = (int)TP_TICK1; i < (int)TP_TICK60; i++)
					//{
					//	ChartProcTick((void*)msg.lParam, (CHART_TP)i);
					//}
					for (i = (int)TP_1MIN; i < (int)TP_DAY; i++)
					{
						ChartProc((void*)msg.lParam, (CHART_TP)i);
					}
					//SendData2Ord((void*)msg.lParam);			// ���� ������ ���� ����.
					//m_pMemPool->release((void*)msg.lParam);	// DB ������ ���� �� �����͸� ���. DB ���� �� Release �Ѵ�.
					break;
				}
			} // switch (msg.message)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)
	
}


VOID	CChartThread::SendData2Ord(VOID* pIn)
{
	int size = sizeof(ST_PACK2ORD);
	void *msg = nn_allocmsg(size, 0);
	ST_PACK2ORD* p = (ST_PACK2ORD*)msg;
	At0933O_st_Ex* pp =  (At0933O_st_Ex*)pIn;
	
	memcpy(p->Date, pp->org.Date, sizeof(p->Date));
	memcpy(p->Time, pp->org.Time, sizeof(p->Time));
	memcpy(p->shortCode, pp->shortCode, sizeof(p->shortCode));
	memcpy(p->close, pp->org.ClosePrice, sizeof(p->close));
	char tmp[32];
	FORMAT_SEQNO(pp->seq, tmp);
	memcpy(p->seq, tmp, LEN_SEQNO);
	
	int nLen = m_pSock->SendData(msg, size);
	if (nLen < 0)
		g_log.log(LOGTP_ERR, "[SEND ERROR:%s]", m_pSock->GetMsg());
	//else if (nLen > 0)
	//	Log(TRUE, ">>>>SEND(%.100s)<<<", msg);
	
}


VOID	CChartThread::ChartProcTick(VOID* pIn, CHART_TP tp)
{
	At0933O_st_Ex* p = (At0933O_st_Ex*)pIn;
	char temp[32];
	BOOL bRet;

	ST_SHM_CHART_UNIT recvUnit;
	memset(&recvUnit, 0x20, sizeof(recvUnit));

	// GROUP KEY (����+charttp)
	char szGroupKey[LEN_SHM_GROUP_KEY + 1];
	GET_GROUP_KEY(p->shortCode, tp, szGroupKey);

	// STRUCT KEY (��Ʈ�̸�) 0000, 0005, 0010
	char szChartNm[LEN_SHM_STRUCT_KEY + 1];
	GET_CHART_NM(p->org.Time, tp, szChartNm);	

	// SEQNO �ڸ��� ��Ŷ�ð��� ���´�.
	//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

	///////////////////////////////////////////////////////////////////////////////////////
	// chart unit ����
	
	// DB���� �ƹ��͵� ���� ���
	if (m_sLastChartNm[tp].IsEmpty())
	{
		m_sPrevChartNm[tp] = DEFINE_NO_CHART;
		m_sLastChartNm[tp] = szChartNm;
	}
	// ������Ʈ �̸��� ������Ʈ �̸��� �ٸ��� ����. �ι�° ��Ʈ�� ó������ �����ϴ� ���
	else if (m_sLastChartNm[tp].Compare(szChartNm) != 0)
	{
		m_sPrevChartNm[tp] = m_sLastChartNm[tp];
		m_sLastChartNm[tp] = szChartNm;
	}
	memcpy(recvUnit.Nm, szChartNm, sizeof(recvUnit.Nm));	// hhmm		0000, 0005, 0010
	memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].GetBuffer(), LEN_CHART_NM);
	memcpy(recvUnit.open, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	memcpy(recvUnit.high, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	memcpy(recvUnit.low, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	memcpy(recvUnit.close, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	memcpy(recvUnit.cntr_qty, p->org.ContQty, sizeof(p->org.ContQty));
	memcpy(recvUnit.dotcnt, p->org.DecLen, sizeof(p->org.DecLen));

	
	// group(SYMBOL+TP) �� ã�Ƽ� ������ insert. ��, ���� ����
	if (!m_shm.FindGroup(szGroupKey))
	{
		//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

		m_shm.InsertGroup(szGroupKey);
		m_shm.InsertData(szGroupKey, (char*)&recvUnit);
		
		g_log.log(LOGTP_SUCC, "INSERT GROUP[%s][NM:%.*s][CLOSE:%.20s]",
			szGroupKey, LEN_CHART_NM, recvUnit.Nm, recvUnit.close);

		return;
	}

	// �����͸� ã�´�. ������ INSERT. ��, �ð� �Է��̶�� ����̴�.
	ST_SHM_CHART_UNIT existUnit;
	if (!m_shm.GetData(szGroupKey, szChartNm, (char*)&existUnit))
	{
		memcpy(recvUnit.open, p->org.ClosePrice, sizeof(p->org.ClosePrice));
		memcpy(recvUnit.low, p->org.ClosePrice, sizeof(p->org.ClosePrice));
		memcpy(recvUnit.high, p->org.ClosePrice, sizeof(p->org.ClosePrice));
		memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].GetBuffer(), LEN_CHART_NM);

		//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

		// �ð��� ���� ��� ������Ʈ�� SMA �� ���ؼ� �����Ѵ�.
		//if (bOpenPrc)
		//Chart_SMA(tp, szGroupKey, szChartNm, recvUnit);

		m_shm.InsertData(szGroupKey, (char*)&recvUnit);

		g_log.log(LOGTP_SUCC, "INSERT DATA[%s][NM:%.*s][CLOSE:%.20s][QTY:%d]",
			szGroupKey, LEN_CHART_NM, recvUnit.Nm, recvUnit.close,S2N(recvUnit.cntr_qty,sizeof(recvUnit.cntr_qty)));
		return;
	}
	//char prevSeq[11]; sprintf(prevSeq, "%.10s", existUnit.seq);
	//Log(TRUE, "PREV CHART[%s][NM:%.4s][PRE_SEQ:%.10s][OPEN:%.20s][PRE_H:%.20s][PRE_L:%.20s][PRE_C:%.20s][%.1s]",
	//	szGroupKey, existUnit.Nm, prevSeq, existUnit.open,existUnit.high, existUnit.low, existUnit.close, existUnit.gb);


	// ��Ʈ ���
	double dOpen = S2D(existUnit.open, sizeof(existUnit.open));
	double dHigh = S2D(existUnit.high, sizeof(existUnit.high));
	double dLow = S2D(existUnit.low, sizeof(existUnit.low));
	double dNow = S2D(recvUnit.close, sizeof(recvUnit.close));

	dHigh = (dHigh > dNow) ? dHigh : dNow;
	dLow = (dLow < dNow) ? dLow : dNow;

	FORMAT_PRC(dHigh, S2N(existUnit.dotcnt, sizeof(existUnit.dotcnt)), temp);
	memcpy(existUnit.high, temp, min(sizeof(existUnit.high), strlen(temp)));

	FORMAT_PRC(dLow, S2N(existUnit.dotcnt, sizeof(existUnit.dotcnt)), temp);
	memcpy(existUnit.low, temp, min(sizeof(existUnit.low), strlen(temp)));

	if (dNow > dOpen)	existUnit.gb[0] = DEF_CANDLE_PLUS;	// ���
	if (dNow < dOpen)	existUnit.gb[0] = DEF_CANDLE_MINUS;	// ����
	if (dNow == dOpen)	existUnit.gb[0] = '0';

	memcpy(existUnit.close, recvUnit.close, sizeof(recvUnit.close));	
	//memcpy(existUnit.seq, recvUnit.seq, sizeof(recvUnit.seq));
	memcpy(existUnit.prevNm, m_sPrevChartNm[tp].GetBuffer(), LEN_CHART_NM);
	
	int nQty = S2N(existUnit.cntr_qty, sizeof(existUnit.cntr_qty))+ S2N(recvUnit.cntr_qty, sizeof(recvUnit.cntr_qty));
	char zQty[LEN_QTY + 1];
	sprintf(zQty, "%*d", LEN_QTY, nQty);
	memcpy(existUnit.cntr_qty, zQty, strlen(zQty));


	//SHM �ݿ�
	bRet = m_shm.SetData(szGroupKey, szChartNm, (char*)&existUnit);

	if (!bRet) {
		g_log.log(LOGTP_ERR, "CHART ���� ����(%s)(%s)", szGroupKey, szChartNm);
	}
	else {
		//Log(TRUE, "NOW CHART[%s][NM:%.4s][NEW_SEQ:%.10s][OPEN:%.20s][NOW_H:%.20s][NOW_L:%.20s][NOW_C:%.20s][%.1s] LAST_NM[%.4s]",
		//	szGroupKey, recvUnit.Nm, recvUnit.seq, existUnit.open, existUnit.high, existUnit.low, recvUnit.close, existUnit.gb, lastNm.LastChartNm);
		//Log(TRUE, "CHART[%s][%.4s][%.4s][%6d][O:%7.5f][C:%7.5f][%.1s]",
		//	szGroupKey, existUnit.Nm, existUnit.prevNm, atoi(existUnit.seq), atof(existUnit.open), atof(existUnit.close), existUnit.gb);

		//ST_SHM_CHART_UNIT debug;
		//m_shm[CHART].GetData(szGroupKey, szChartNm, (char*)&debug);
		//Log(TRUE, "DEBUG[%s][%.4s][PREV:%.4s][%5d][O:%7.5f][C:%7.5f][%.1s]",
		//	szGroupKey, debug.Nm, debug.prevNm, atoi(debug.seq), atof(debug.open), atof(debug.close), debug.gb);
	}
}


VOID	CChartThread::ChartProc(VOID* pIn, CHART_TP tp)
{
	At0933O_st_Ex* p = (At0933O_st_Ex*)pIn;
	char temp[32];
	BOOL bRet;

	ST_SHM_CHART_UNIT recvUnit;
	memset(&recvUnit, 0x20, sizeof(recvUnit));

	// GROUP KEY (����+charttp)
	char szGroupKey[LEN_SHM_GROUP_KEY + 1];
	GET_GROUP_KEY(p->shortCode, tp, szGroupKey);

	// STRUCT KEY (��Ʈ�̸�) 0000, 0005, 0010
	char szChartNm[LEN_SHM_STRUCT_KEY + 1];
	GET_CHART_NM(p->org.Time, tp, szChartNm);

	// SEQNO �ڸ��� ��Ŷ�ð��� ���´�.
	//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

	///////////////////////////////////////////////////////////////////////////////////////
	// chart unit ����

	// DB���� �ƹ��͵� ���� ���
	if (m_sLastChartNm[tp].IsEmpty())
	{
		m_sPrevChartNm[tp] = DEFINE_NO_CHART;
		m_sLastChartNm[tp] = szChartNm;
	}
	// ������Ʈ �̸��� ������Ʈ �̸��� �ٸ��� ����. �ι�° ��Ʈ�� ó������ �����ϴ� ���
	else if (m_sLastChartNm[tp].Compare(szChartNm) != 0)
	{
		m_sPrevChartNm[tp] = m_sLastChartNm[tp];
		m_sLastChartNm[tp] = szChartNm;
	}
	memcpy(recvUnit.Nm, szChartNm, sizeof(recvUnit.Nm));	// hhmm		0000, 0005, 0010
	memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].GetBuffer(), LEN_CHART_NM);
	memcpy(recvUnit.open, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	memcpy(recvUnit.high, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	memcpy(recvUnit.low, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	memcpy(recvUnit.close, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	memcpy(recvUnit.cntr_qty, p->org.ContQty, sizeof(p->org.ContQty));
	memcpy(recvUnit.dotcnt, p->org.DecLen, sizeof(p->org.DecLen));


	// group(SYMBOL+TP) �� ã�Ƽ� ������ insert. ��, ���� ����
	if (!m_shm.FindGroup(szGroupKey))
	{
		//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

		m_shm.InsertGroup(szGroupKey);
		m_shm.InsertData(szGroupKey, (char*)&recvUnit);

		g_log.log(LOGTP_SUCC, "INSERT GROUP[%s][NM:%.*s][CLOSE:%.20s]",
			szGroupKey, LEN_CHART_NM, recvUnit.Nm, recvUnit.close);

		return;
	}

	// �����͸� ã�´�. ������ INSERT. ��, �ð� �Է��̶�� ����̴�.
	ST_SHM_CHART_UNIT existUnit;
	if (!m_shm.GetData(szGroupKey, szChartNm, (char*)&existUnit))
	{
		memcpy(recvUnit.open, p->org.ClosePrice, sizeof(p->org.ClosePrice));
		memcpy(recvUnit.low, p->org.ClosePrice, sizeof(p->org.ClosePrice));
		memcpy(recvUnit.high, p->org.ClosePrice, sizeof(p->org.ClosePrice));
		memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].GetBuffer(), LEN_CHART_NM);

		//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

		// �ð��� ���� ��� ������Ʈ�� SMA �� ���ؼ� �����Ѵ�.
		//if (bOpenPrc)
		//Chart_SMA(tp, szGroupKey, szChartNm, recvUnit);

		m_shm.InsertData(szGroupKey, (char*)&recvUnit);

		g_log.log(LOGTP_SUCC, "INSERT DATA[%s][NM:%.*s][CLOSE:%.20s][QTY:%d]",
			szGroupKey, LEN_CHART_NM, recvUnit.Nm, recvUnit.close, S2N(recvUnit.cntr_qty, sizeof(recvUnit.cntr_qty)));
		return;
	}
	//char prevSeq[11]; sprintf(prevSeq, "%.10s", existUnit.seq);
	//Log(TRUE, "PREV CHART[%s][NM:%.4s][PRE_SEQ:%.10s][OPEN:%.20s][PRE_H:%.20s][PRE_L:%.20s][PRE_C:%.20s][%.1s]",
	//	szGroupKey, existUnit.Nm, prevSeq, existUnit.open,existUnit.high, existUnit.low, existUnit.close, existUnit.gb);


	// ��Ʈ ���
	double dOpen = S2D(existUnit.open, sizeof(existUnit.open));
	double dHigh = S2D(existUnit.high, sizeof(existUnit.high));
	double dLow = S2D(existUnit.low, sizeof(existUnit.low));
	double dNow = S2D(recvUnit.close, sizeof(recvUnit.close));

	dHigh = (dHigh > dNow) ? dHigh : dNow;
	dLow = (dLow < dNow) ? dLow : dNow;

	FORMAT_PRC(dHigh, S2N(existUnit.dotcnt, sizeof(existUnit.dotcnt)), temp);
	memcpy(existUnit.high, temp, min(sizeof(existUnit.high), strlen(temp)));

	FORMAT_PRC(dLow, S2N(existUnit.dotcnt, sizeof(existUnit.dotcnt)), temp);
	memcpy(existUnit.low, temp, min(sizeof(existUnit.low), strlen(temp)));

	if (dNow > dOpen)	existUnit.gb[0] = DEF_CANDLE_PLUS;	// ���
	if (dNow < dOpen)	existUnit.gb[0] = DEF_CANDLE_MINUS;	// ����
	if (dNow == dOpen)	existUnit.gb[0] = '0';

	memcpy(existUnit.close, recvUnit.close, sizeof(recvUnit.close));
	//memcpy(existUnit.seq, recvUnit.seq, sizeof(recvUnit.seq));
	memcpy(existUnit.prevNm, m_sPrevChartNm[tp].GetBuffer(), LEN_CHART_NM);

	int nQty = S2N(existUnit.cntr_qty, sizeof(existUnit.cntr_qty)) + S2N(recvUnit.cntr_qty, sizeof(recvUnit.cntr_qty));
	char zQty[LEN_QTY + 1];
	sprintf(zQty, "%*d", LEN_QTY, nQty);
	memcpy(existUnit.cntr_qty, zQty, strlen(zQty));


	//SHM �ݿ�
	bRet = m_shm.SetData(szGroupKey, szChartNm, (char*)&existUnit);

	if (!bRet) {
		g_log.log(LOGTP_ERR, "CHART ���� ����(%s)(%s)", szGroupKey, szChartNm);
	}
	else {
		//Log(TRUE, "NOW CHART[%s][NM:%.4s][NEW_SEQ:%.10s][OPEN:%.20s][NOW_H:%.20s][NOW_L:%.20s][NOW_C:%.20s][%.1s] LAST_NM[%.4s]",
		//	szGroupKey, recvUnit.Nm, recvUnit.seq, existUnit.open, existUnit.high, existUnit.low, recvUnit.close, existUnit.gb, lastNm.LastChartNm);
		//Log(TRUE, "CHART[%s][%.4s][%.4s][%6d][O:%7.5f][C:%7.5f][%.1s]",
		//	szGroupKey, existUnit.Nm, existUnit.prevNm, atoi(existUnit.seq), atof(existUnit.open), atof(existUnit.close), existUnit.gb);

		//ST_SHM_CHART_UNIT debug;
		//m_shm[CHART].GetData(szGroupKey, szChartNm, (char*)&debug);
		//Log(TRUE, "DEBUG[%s][%.4s][PREV:%.4s][%5d][O:%7.5f][C:%7.5f][%.1s]",
		//	szGroupKey, debug.Nm, debug.prevNm, atoi(debug.seq), atof(debug.open), atof(debug.close), debug.gb);
	}
}


/*
// simple moving average
	- �ð��� ���� ��� �� ���� ��Ʈ�� SMA �� ���Ѵ�.
*/
VOID CChartThread::Chart_SMA(CHART_TP tp, char* pzGroupKey, char* pzNowChartNm, _In_ ST_SHM_CHART_UNIT& nowChart)
{
	// �� ���� ��Ʈ ���� �����´�.
	int nChartCnt = m_shm.GetCurrStructCnt(pzGroupKey);
	if (nChartCnt < SMA_LONG_CNT+1)
		return;

	if (tp == TP_5MIN)
		INT A = 0;
	char zChartNm[32], zCloseChartNm[32];
	ST_SHM_CHART_UNIT chartShm, closeChartShm;
	TA_Real stSMA[SMA_LONG_CNT] = { 0, };
	
	int nLoop = SMA_LONG_CNT;
	sprintf(zChartNm,"%.*s", LEN_CHART_NM, nowChart.prevNm);
	int nCnt = 1;

	/*
		ARRAY �� �Ųٷ� ��ƾ� �Ѵ�.
	*/
	for (int i = nLoop-1; i > -1; i--)
	{
		if (strcmp(DEFINE_NO_CHART, zChartNm) == 0)	break;

		if (FALSE == m_shm.GetData(pzGroupKey, zChartNm, (char*)&chartShm))
		{
			g_log.log(LOGTP_ERR, "[Chart_SMA][%s][%s]��Ʈ�� SHM �� ����", pzGroupKey, zChartNm);
			continue;
		}
		
		// ���� ù��° ��Ʈ ����.
		if (i == nLoop - 1) {
			strcpy(zCloseChartNm, zChartNm);
			memcpy(&closeChartShm, &chartShm, sizeof(chartShm));
		}

		sprintf(zChartNm, "%.*s", LEN_CHART_NM, chartShm.prevNm);

		//Log(TRUE, "[Chart_SMA][%d][NOW:%.4s](%.20s)", i, chartShm.Nm, chartShm.close);

		stSMA[i] = atof(chartShm.close);
	}


	TA_Real		outShort[10], outLong[20];
	TA_Integer outBeg, outNbElement;
	TA_Integer outBeg2, outNbElement2;
	TA_RetCode retCode = TA_MA(0, 19, &stSMA[0],20, TA_MAType_SMA, &outBeg, &outNbElement, &outLong[0]);
	retCode = TA_MA(0, 9, &stSMA[10],10, TA_MAType_SMA, &outBeg2, &outNbElement2, &outShort[0]);
	
	char zPrc[32], zDotCnt[32];
	sprintf(zDotCnt, "%.*s", sizeof(chartShm.dotcnt), chartShm.dotcnt);
	int nDotCnt = atoi(zDotCnt);

	double dLong = CUtil::roundoff(outLong[0], nDotCnt);	// �ݿø�
	double dShort= CUtil::roundoff(outShort[0], nDotCnt);	// �ݿø�

	FORMAT_PRC(dLong, nDotCnt, zPrc);
	memcpy(closeChartShm.sma_long, zPrc, strlen(zPrc));

	FORMAT_PRC(dShort, nDotCnt, zPrc);
	memcpy(closeChartShm.sma_short, zPrc, strlen(zPrc));

	g_log.log(LOGTP_SUCC, "[Chart_SMA][LMA:%.*s][SMA:%.*s]", LEN_PRC, closeChartShm.sma_long, LEN_PRC, closeChartShm.sma_short);

	//SHM �ݿ�
	m_shm.UpdateData(pzGroupKey, zCloseChartNm, 0, sizeof(ST_SHM_CHART_UNIT), (char*)&closeChartShm);
}

void CChartThread::CloseChartShm()
{
	m_shm.Close();

}

BOOL CChartThread::InitChartSHM()
{
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);

	//	OPEN SHM
	if (!m_shm.Open(m_zShmNm, m_zMutexNm))
	{
		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN ����(%s)(symbol:%s)(%s)", 
			m_zShmNm, m_sSymbol.GetBuffer(), m_shm.GetErr());
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "SHM OPEN ����(%s)", m_zMutexNm);


	// DB ���� ������ ��Ʈ �̸��� �����´�.
	char ip[32], id[32], pwd[32], name[32];
	CUtil::GetConfig(szConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(szConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(szConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(szConfig, "DBINFO", "DB_NAME", name);

	CDBPoolAdo *pDBPool;
	pDBPool = new CDBPoolAdo(ip, id, pwd, name);
	if (!pDBPool->Init(1))
	{
		SAFE_DELETE(pDBPool);
		return 0;
	}
	CDBHandlerAdo db(pDBPool->Get());
	char zQ[1024];


	for (int i = TP_1MIN; i < TP_DAY; i++)
	{
		sprintf(zQ, "EXEC CHART_GET_LASTCHART_NM '%s', %d ", Symbol(), i);
		if (FALSE == db->QrySelect(zQ))
		{
			g_log.log(LOGTP_ERR, "��������Ʈ ��ȸ ����(%s)(%s)", db->GetError(), zQ);
			return 0;
		}


		while (db->IsNextRow())
		{
			char temp[128];
			int nTp = db->GetLong("CHART_TP");
			m_sLastChartNm[nTp] = db->GetStr("CHART_NM", temp);
			db->Next();
		}
	}
	db->Close();

	return TRUE;
}
