#include "stdafx.h"
#include "ChartThread.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include "ta_libc.h"

extern CLogMsg g_log;

CChartThread::CChartThread(CString sSymbol, /*CSharedMem& shmChart, CSharedMem& shmLastNm, */
								Poco::MemoryPool* pMemPool, CNanoSocket* sock, char* pChartTp)
{
	m_sSymbol = sSymbol;
	//m_shm[CHART] = shmChart;
	//m_shm[LASTNM] = shmLastNm;
	m_pMemPool = pMemPool;
	m_pSock = sock;
	memcpy(m_chartTp, pChartTp, sizeof(m_chartTp));
	//m_unSaveThreadID = unSaveThreadID;
}


CChartThread::~CChartThread()
{
	CloseChartShm();
}


VOID CChartThread::ThreadFunc()
{
	g_log.log(LOGTP_SUCC, "[THREAD ID:%d]Start...", GetMyThreadID());

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
				
				ChartProc((void*)msg.lParam, TP_1MIN);
				ChartProc((void*)msg.lParam, TP_5MIN); 
				//ChartProc((void*)msg.lParam, TP_10MIN);
				//ChartProc((void*)msg.lParam, TP_30MIN);
				//ChartProc((void*)msg.lParam, TP_60MIN);
				SendData2Ord((void*)msg.lParam);
				m_pMemPool->release((void*)msg.lParam);
				break;
			}
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


VOID	CChartThread::ChartProc(VOID* pIn, CHART_TP tp)
{
	if (m_chartTp[tp - 1] != 'Y')	return;

	At0933O_st_Ex* p = (At0933O_st_Ex*)pIn;
	char temp[32];
	BOOL bRet;
	//BOOL bOpenPrc = FALSE;

	ST_SHM_LASTCHART	lastNm;
	ST_SHM_CHART_UNIT recvUnit;
	memset(&lastNm, 0x20, sizeof(lastNm));
	memset(&recvUnit, 0x20, sizeof(recvUnit));

	// GROUP KEY (����+charttp)
	char szGroupKey[LEN_SHM_GROUP_KEY + 1];
	GET_GROUP_KEY(p->shortCode, tp, szGroupKey);

	// STRUCT KEY (��Ʈ�̸�) 0000, 0005, 0010
	char szChartNm[LEN_SHM_STRUCT_KEY + 1];
	GET_CHART_NM(p->org.Time, tp, szChartNm);	

	// SEQNO �ڸ��� ��Ŷ�ð��� ���´�.
	memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

	///////////////////////////////////////////////////////////////////////////////////////
	// chart unit ����
	memcpy(recvUnit.Nm, szChartNm, sizeof(recvUnit.Nm));	// hhmm		0000, 0005, 0010
	
	// ���� chart �̸��� �����Ѵ�. ���� ��Ʈ�� ���
	if (m_sLastChartNm[tp-1].IsEmpty() || m_sLastChartNm[tp - 1].Compare(DEFINE_NO_CHART)==0) 
	{
		char zName[LEN_CHART_NM + 1];
		// ���ʽ����ΰ��
		ST_SHM_LASTCHART	shmLast;
		if (m_shm[LASTNM].GetData(szGroupKey, szGroupKey, (char*)&shmLast))
		{
			sprintf(zName, "%.*s", LEN_CHART_NM, shmLast.LastChartNm);
			ST_SHM_CHART_UNIT shmUnit;
			if (m_shm[CHART].GetData(szGroupKey, zName, (char*)&shmUnit))
			{
				sprintf(zName, "%.*s", LEN_CHART_NM, shmUnit.prevNm);
				m_sLastChartNm[tp - 1] = szChartNm;
				m_sPrevChartNm[tp - 1] = zName;
			}
		}
		else
		{
			m_sLastChartNm[tp - 1] = szChartNm;
			m_sPrevChartNm[tp - 1] = DEFINE_NO_CHART;
		}
		
	}
	else
	{
		// ������Ʈ �̸��� ������Ʈ �̸��� �ٸ��� ����. �ι�° ��Ʈ�� ó������ �����ϴ� ���
		if (m_sLastChartNm[tp - 1].Compare(szChartNm) != 0)
		{
			m_sPrevChartNm[tp - 1] = m_sLastChartNm[tp - 1];
			m_sLastChartNm[tp - 1] = szChartNm;
			//bOpenPrc = TRUE; // ������Ʈ�� �̸��� �ٸ��Ƿ� �ð�
		}
		else
		{
			//strcpy(szPrevChartNm, m_sLastChartNm);
		}
	}

	memcpy(recvUnit.dotcnt, p->org.DecLen, sizeof(p->org.DecLen));
	memcpy(recvUnit.close, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	memcpy(recvUnit.open, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	memcpy(recvUnit.low, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	memcpy(recvUnit.high, p->org.ClosePrice, sizeof(p->org.ClosePrice));
	//todo FORMAT_SEQNO(p->seq, temp);
	//todo memcpy(recvUnit.seq, temp, sizeof(recvUnit.seq));
	memcpy(recvUnit.prevNm, m_sPrevChartNm[tp - 1].GetBuffer(), LEN_CHART_NM);
	
	///////////////////////////////////////////////////////////////////////////////////////
	// last chart name 
	memcpy(lastNm.GroupKey, szGroupKey, LEN_SHM_GROUP_KEY);
	memcpy(lastNm.LastChartNm, szChartNm, LEN_CHART_NM);

	// group �� ã�Ƽ� ������ insert
	if (!m_shm[CHART].FindGroup(szGroupKey))
	{
		memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

		//m_shm[CHART].Lock();
		m_shm[CHART].InsertGroup(szGroupKey);
		m_shm[CHART].InsertData(szGroupKey, (char*)&recvUnit);
		//m_shm[CHART].Unlock();
		g_log.log(LOGTP_SUCC, "INSERT GROUP[%s][NM:%.*s][��Ŷ�ð�:%.8s][LAST:%.*s][SEQ:%.10s][CLOSE:%.20s]",
			szGroupKey, LEN_CHART_NM, recvUnit.Nm, p->org.Time, LEN_CHART_NM, recvUnit.prevNm, recvUnit.seq, recvUnit.close);

		// last chart name
		//m_shm[LASTNM].Lock();
		m_shm[LASTNM].InsertGroup(szGroupKey);
		m_shm[LASTNM].InsertData(szGroupKey, (char*)&lastNm);
		//m_shm[LASTNM].Unlock();

		return;
	}

	// LAST CHART NAME ����
	//m_shm[LASTNM].Lock();
	bRet = m_shm[LASTNM].UpdateData(szGroupKey, szGroupKey, (char*)&lastNm);
	//m_shm[LASTNM].Unlock();
	if (!bRet) {
		g_log.log(LOGTP_ERR, "LAST CHART NAME ���� ����(%s)", szGroupKey);
	}

	// �����͸� ã�´�. ������ INSERT. ��, �ð� �Է��̶�� ����̴�.
	ST_SHM_CHART_UNIT existUnit;
	if (!m_shm[CHART].GetData(szGroupKey, szChartNm, (char*)&existUnit))
	{
		memcpy(recvUnit.open, p->org.ClosePrice, sizeof(p->org.ClosePrice));
		memcpy(recvUnit.low, p->org.ClosePrice, sizeof(p->org.ClosePrice));
		memcpy(recvUnit.high, p->org.ClosePrice, sizeof(p->org.ClosePrice));
		memcpy(recvUnit.prevNm, m_sPrevChartNm[tp-1].GetBuffer(), LEN_CHART_NM);

		memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));


		// �ð��� ���� ��� ������Ʈ�� SMA �� ���ؼ� �����Ѵ�.
		//if (bOpenPrc)
		Chart_SMA(tp, szGroupKey, szChartNm, recvUnit);

		//m_shm[CHART].Lock();
		m_shm[CHART].InsertData(szGroupKey, (char*)&recvUnit);
		//m_shm[CHART].Unlock();

		g_log.log(LOGTP_SUCC, "INSERT DATA[%s][NM:%.*s][��Ŷ�ð�:%.10s][CLOSE:%.20s]",
			szGroupKey, LEN_CHART_NM, recvUnit.Nm, p->org.Time, recvUnit.close);
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
	memcpy(existUnit.prevNm, m_sPrevChartNm[tp-1].GetBuffer(), LEN_CHART_NM);
	
	//SHM �ݿ�
	//m_shm[CHART].Lock();
	bRet = m_shm[CHART].SetData(szGroupKey, szChartNm, (char*)&existUnit);
	//m_shm[CHART].Unlock();

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
	int nChartCnt = m_shm[CHART].GetCurrStructCnt(pzGroupKey);
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

		if (FALSE == m_shm[CHART].GetData(pzGroupKey, zChartNm, (char*)&chartShm))
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
	m_shm[CHART].UpdateData(pzGroupKey, zCloseChartNm, 0, sizeof(ST_SHM_CHART_UNIT), (char*)&closeChartShm);
}

void CChartThread::CloseChartShm()
{
	m_shm[CHART].Close();
	m_shm[LASTNM].Close();

}

BOOL CChartThread::InitChartSHM()
{
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);

	char szSHMName[128] = { 0, }, szMutexName[128] = { 0, };
	CUtil::GetConfig(szConfig, "SHM_INFO", "SHM_NAME", szSHMName);
	CUtil::GetConfig(szConfig, "SHM_INFO", "MUTEX_NAME", szMutexName);

	//	OPEN SHM
	//if (!m_shm[CHART].Open(szSHMName, szMutexName))
	if (!m_shm[CHART].Open(szSHMName, szMutexName))
	{
		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN ����(%s)(symbol:%s)(%s)", 
			szSHMName, m_sSymbol.GetBuffer(), m_shm[CHART].GetErr());
		return FALSE;
	}


	CUtil::GetConfig(szConfig, "SHM_LASTCHART_NM", "SHM_NAME", szSHMName);
	CUtil::GetConfig(szConfig, "SHM_LASTCHART_NM", "MUTEX_NAME", szMutexName);
	if (!m_shm[LASTNM].Open(szSHMName, szMutexName))
	{
		g_log.log(LOGTP_ERR, "LASTNM SHM OPEN ����(%s)", szSHMName);
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "SHM OPEN ����");

	return TRUE;
}
