
#include "ChartMaker.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "../../IRUM_UTIL/ADOFunc.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include "C:\ta-lib\c\include\ta_libc.h"
#include <winbase.h>

extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];

CChartMaker::CChartMaker(char* pzSymbol, char* pzArtcCode, CMemPool* pMemPool, unsigned dwMainThreadId)
{
	m_dwMainThreadId = dwMainThreadId;
	m_pMemPool = pMemPool;
	strcpy(m_zSymbol, pzSymbol);
	strcpy(m_zArtc, pzArtcCode);
	GET_SHM_NM(pzArtcCode, m_zShmNm);
	GET_SHM_LOCK_NM(pzArtcCode, m_zMutexNm);

	ZeroMemory(&m_ChartNmSeq, sizeof(m_ChartNmSeq));
	ZeroMemory(&m_ChartMakeCnt, sizeof(m_ChartMakeCnt));

	ResumeThread();

	// worker thread ����
	m_hWorkDie = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hWorkThread = (HANDLE)_beginthreadex(NULL, 0, &WorkThread, this, 0, &m_dwWorkThreadID);
}

CChartMaker::~CChartMaker()
{
	CloseChartShm();
	EndMcastRecv();

	SetEvent(m_hWorkDie);
	if (WaitForSingleObject(m_hWorkThread, 3000) != WAIT_OBJECT_0) {
		DWORD dwExitCode = 0;
		TerminateThread(m_hWorkThread, dwExitCode);
	}
	CloseHandle(m_hWorkDie);
	CloseHandle(m_hWorkThread);
	m_hDie = m_hThread = NULL;
}

BOOL CChartMaker::IsMySymbol(char* pSise)
{
	return TRUE;
}


BOOL CChartMaker::InitChartSHM()
{
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);

	//	OPEN SHM
	if (!m_shmQ.Open((LPCTSTR)m_zShmNm, (LPCTSTR)m_zMutexNm))
	{
		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN ����(%s)(symbol:%s)(%s)", m_zShmNm, m_zSymbol, m_shmQ.GetErr());
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "SHM OPEN ����(%s)", m_zMutexNm);


	for (int i = TP_TICK2; i < TP_TICK60; i++)
	{
		char zGroupKey[LEN_GROUP_KEY + 1] = { 0, };
		GET_GROUP_KEY(m_zSymbol, i, zGroupKey);

		char zStructKey[LEN_CHART_NM + 1] = { 0, };
		if (m_shmQ.GetCurrStructKey(zGroupKey, zStructKey))
		{
			m_sLastChartNm[ChartIdx(i)] = zStructKey;
			m_ChartNmSeq[ChartIdx(i)] = atoi(zStructKey);
		}
		else
		{
			//
		}
	}
	return TRUE;
}



BOOL CChartMaker::BeginMcastRecv()
{
	m_pMcastRecv = new CMCastRecv;

	char zLocalIP[32], zMcastIP[32], port[32];
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "IP", zLocalIP);
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "IP_MCAST", zMcastIP);
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "PORT", port);

	BOOL bRet = m_pMcastRecv->Begin(zLocalIP, zMcastIP, atoi(port));
	if (bRet)
		g_log.log(LOGTP_SUCC, "[%s]�ü����� Init ����(LOCAL IP:%s)(MCAST IP:%s)(PORT:%s)", m_zSymbol, zLocalIP, zMcastIP, port);
	else
		g_log.log(LOGTP_ERR, "[%s]�ü����� Init ����(LOCAL IP:%s)(MCAST IP:%s)(PORT:%s)", m_zSymbol, zLocalIP, zMcastIP, port);

	return bRet;
}

VOID CChartMaker::EndMcastRecv()
{
	SAFE_DELETE(m_pMcastRecv);

}


VOID CChartMaker::ThreadFunc()
{
	printf("thread:%d\n", GetCurrentThreadId());
	if (!InitChartSHM())
	{
		g_log.log(LOGTP_ERR, ">>>>>> SHM OPEN ERROR <<<<<<<<<");
		return;
	}

	// MCAST RECV INIT
	if (!BeginMcastRecv())
	{
		return;
	}


	char zMarketDataBuff[1024] = { 0, };
	int nSymbolLen = strlen(m_zSymbol);

	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, 1, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) {
			//g_log.log(LOGTP_ERR, "[THREAD ID:%d]WAIT ERROR(%d)", GetMyThreadID(), GetLastError());
			Sleep(1000);
		}
		memset(zMarketDataBuff, 0x00, sizeof(zMarketDataBuff));
		//MCAST int nRecvLen = m_pMDSub->Subscribe((void*)zMarketDataBuff);
		int nRecvLen = m_pMcastRecv->RecvData(zMarketDataBuff, sizeof(zMarketDataBuff));

		// Do Make Signal after receiving marketdata;
		if (nRecvLen > 0)
		{
			ST_PACK2CHART* p = (ST_PACK2CHART*)zMarketDataBuff;
			if (strncmp(m_zSymbol, p->ShortCode, nSymbolLen) != 0)
				continue;

			char* pData = m_pMemPool->get();
			strcpy(pData, zMarketDataBuff);
			int nLen = strlen(pData);

			PostThreadMessage(m_dwWorkThreadID, WM_RECV_API_MD, (WPARAM)nLen, (LPARAM)pData);

		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_CHART_ALL_KILL:
				g_log.log(LOGTP_SUCC, "[THREAD ID:%d] Recv Kill Msg", GetMyThreadID());
				break;
			//case WM_CHART_DATA:
			//{
			//	int i = 0;
			//	//for (i = (int)TP_TICK1; i < (int)TP_TICK60; i++)
			//	//{
			//	//	ChartProcTick((void*)msg.lParam, (CHART_TP)i);
			//	//}
			//	for (i = (int)TP_TICK2; i < (int)TP_TICK60+1; i++)
			//	{
			//		ChartProc((void*)msg.lParam, (int)i);
			//		//ChartProc((void*)msg.lParam, (int)TP_TICK2);
			//	}
			//	m_pMemPool->release((char*)msg.lParam);
			//	break;
			//}
			} // switch (msg.message)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)
}

int		CChartMaker::ChartIdx(int chartTp)
{
	int idx;
	if (chartTp == TP_TICK2)	idx = 0;
	if (chartTp == TP_TICK5)	idx = 1;
	if (chartTp == TP_TICK10)	idx = 2;
	if (chartTp == TP_TICK20)	idx = 3;
	if (chartTp == TP_TICK30)	idx = 4;
	if (chartTp == TP_TICK40)	idx = 5;
	if (chartTp == TP_TICK60)	idx = 6;
	return idx;
}

BOOL	CChartMaker::IsMaxChartMakeCnt(int chartTp, int lCnt)
{
	BOOL bRet = FALSE;

	if (chartTp == TP_TICK2 && lCnt == 2)	bRet = TRUE;
	if (chartTp == TP_TICK5 && lCnt == 5)	bRet = TRUE;
	if (chartTp == TP_TICK10 && lCnt == 10)	bRet = TRUE;
	if (chartTp == TP_TICK20 && lCnt == 20)	bRet = TRUE;
	if (chartTp == TP_TICK30 && lCnt == 30)	bRet = TRUE;
	if (chartTp == TP_TICK40 && lCnt == 40)	bRet = TRUE;
	if (chartTp == TP_TICK60 && lCnt == 60)	bRet = TRUE;
	
	return bRet;
}


unsigned WINAPI CChartMaker::WorkThread(LPVOID lp)
{
	CChartMaker* p = (CChartMaker*)lp;
	while (WaitForSingleObject(p->m_hWorkDie, 1) != WAIT_OBJECT_0)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_RECV_API_MD)
			{
				char* pData = (char*)msg.lParam;
				//printf("work recv(%.40s)\n", pData);
				int nLen = (int)msg.wParam;

				int i = 0;
				for (i = (int)TP_TICK2; i < (int)TP_TICK60+1; i++)
				{
					p->ChartProc((void*)msg.lParam, (int)i);
					//ChartProc((void*)msg.lParam, (int)TP_TICK2);
				}
				p->m_pMemPool->release(pData);
			} //if (msg.message == WM_SEND_STRATEGY)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)
	return 0;
}



/*
	TICK ��Ʈ�� ������ ��Ʈ�� �����.
	���� 5ƽ ��Ʈ�� ���� 5���� ��� �ϳ��� ĵ���� ����� ���̴�.
	���� 0���� ��Ʈ�̸��� ����� 5���� �ϳ��� ���´�.
*/
VOID	CChartMaker::ChartProc(VOID* pIn, int tp)
{
	ST_PACK2CHART* p = (ST_PACK2CHART*)pIn;
	char temp[32];
	BOOL bRet;

	ST_SHM_CHART_UNIT recvUnit;
	memset(&recvUnit, 0x20, sizeof(recvUnit));

	// GROUP KEY (����+charttp)
	char szGroupKey[LEN_SHM_GROUP_KEY + 1];
	GET_GROUP_KEY(p->ShortCode, tp, szGroupKey);
	//sprintf(szGroupKey, "%-*.*s%02d", LEN_SYMBOL, LEN_SYMBOL, p->shortCode, tp);

	//////////////////////////////////////////////////////////////////////////
	// STRUCT KEY (��Ʈ�̸�)
	// - ƽ��Ʈ ī��Ʈ (2ƽ�� 2, 60ƽ�� 60) �� ����Ѵ�.
	char szChartNm[LEN_SHM_STRUCT_KEY + 1];
	
	// ������ ���. ��Ʈ���� �����.
	if (m_ChartMakeCnt[ChartIdx(tp)] == 0)
	{
		m_ChartMakeCnt[ChartIdx(tp)]++;

		m_ChartNmSeq[ChartIdx(tp)]++;
		GET_TICKCHART_NM(m_ChartNmSeq[ChartIdx(tp)], szChartNm);

		//g_log.log(LOGTP_SUCC, "���ο� ��Ʈ:%s", szChartNm);
		
		if (m_sLastChartNm[ChartIdx(tp)].empty())
		{
			m_sPrevChartNm[ChartIdx(tp)] = DEFINE_NO_CHART;
			m_sLastChartNm[ChartIdx(tp)] = szChartNm;
		}
		else
		{
			// ������Ʈ DB ���� ==> �Ź� �ϵ��� ����(2017.08.23)
			//char zPrevNm[32]; sprintf(zPrevNm, m_sLastChartNm[ChartIdx(tp)].c_str());
			//ST_SHM_CHART_UNIT* p = new ST_SHM_CHART_UNIT;
			//if (!m_shmQ.DataGet(szGroupKey, zPrevNm, (char*)p))
			//{
			//	g_log.log(LOGTP_FATAL, "��Ʈ ������ ���� ��ȸ ����(%s)(%s)", szGroupKey, zPrevNm);
			//	delete p;
			//}
			//else
			//{
			//	// group key �� reserve �� �ִ´�.
			//	memcpy(p->Reserved, szGroupKey, LEN_GROUP_KEY);
			//	PostThreadMessage(m_dwMainThreadId, WM_SAVE_CHART, 0, (LPARAM)p);
			//}
		}
		m_sPrevChartNm[ChartIdx(tp)] = m_sLastChartNm[ChartIdx(tp)];
		m_sLastChartNm[ChartIdx(tp)] = szChartNm;
		
	}
	// ���ʰ� �ƴ� ��� ���� �������� ��Ʈ���� ����Ѵ�.
	else
	{
		strcpy(szChartNm, m_sLastChartNm[ChartIdx(tp)].c_str());
		m_ChartMakeCnt[ChartIdx(tp)]++;

		//g_log.log(LOGTP_SUCC, "�������� ��Ʈ:%s, cnt:%d", szChartNm, m_ChartMakeCnt[ChartIdx(tp)]);

		// �̹��� �������̸� ������ ���ο� ��Ʈ�� ���鵵�� 0
		if (IsMaxChartMakeCnt(tp, m_ChartMakeCnt[ChartIdx(tp)])) {
			m_ChartMakeCnt[ChartIdx(tp)] = 0;
			//g_log.log(LOGTP_SUCC, "��Ʈ ������ cnt:%s", szChartNm);
		}
	}


	memcpy(recvUnit.Nm, szChartNm, sizeof(recvUnit.Nm));	// hhmm		0000, 0005, 0010
	memcpy(recvUnit.prevNm, m_sPrevChartNm[ChartIdx(tp)].c_str(), LEN_CHART_NM);
	memcpy(recvUnit.open, p->Close, sizeof(p->Close));
	memcpy(recvUnit.high, p->Close, sizeof(p->Close));
	memcpy(recvUnit.low, p->Close, sizeof(p->Close));
	memcpy(recvUnit.close, p->Close, sizeof(p->Close));
	memcpy(recvUnit.cntr_qty, p->CntrQty, sizeof(p->CntrQty));
	memcpy(recvUnit.dotcnt, p->DecLen, sizeof(p->DecLen));


	// group(SYMBOL+TP) �� ã�Ƽ� ������ insert. ��, ���� ����
	if (!m_shmQ.GroupFind(szGroupKey))
	{
		//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

		m_shmQ.GroupInsert(szGroupKey);
		m_shmQ.DataInsert(szGroupKey, (char*)&recvUnit);

		// DB ����
		//ST_SHM_CHART_UNIT* p = new ST_SHM_CHART_UNIT;
		//memcpy(p, &recvUnit, sizeof(ST_SHM_CHART_UNIT));
		//memcpy(p->Reserved, szGroupKey, LEN_GROUP_KEY);
		//PostThreadMessage(m_dwMainThreadId, WM_SAVE_CHART, 0, (LPARAM)p);

		g_log.log(LOGTP_SUCC, "INSERT GROUP[%s][NM:%.*s][CLOSE:%.20s]",
			szGroupKey, LEN_CHART_NM, recvUnit.Nm, recvUnit.close);

		return;
	}

	// �����͸� ã�´�. ������ INSERT. ��, �ð� �Է��̶�� ����̴�.
	ST_SHM_CHART_UNIT existUnit;
	if (!m_shmQ.DataGet(szGroupKey, szChartNm, (char*)&existUnit))
	{
		memcpy(recvUnit.open, p->Close, sizeof(p->Close));
		memcpy(recvUnit.low, p->Close, sizeof(p->Close));
		memcpy(recvUnit.high, p->Close, sizeof(p->Close));
		memcpy(recvUnit.prevNm, m_sPrevChartNm[ChartIdx(tp)].c_str(), LEN_CHART_NM);

		//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

		// �ð��� ���� ��� ������Ʈ�� SMA �� ���ؼ� �����Ѵ�.
		Chart_SMA(tp, szGroupKey, szChartNm, (char*)&recvUnit);

		m_shmQ.DataInsert(szGroupKey, (char*)&recvUnit);

		// DB ����
		ST_SHM_CHART_UNIT* p = new ST_SHM_CHART_UNIT;
		memcpy(p, &recvUnit, sizeof(ST_SHM_CHART_UNIT));
		memcpy(p->Reserved, szGroupKey, LEN_GROUP_KEY);
		PostThreadMessage(m_dwMainThreadId, WM_SAVE_CHART, 0, (LPARAM)p);

		//g_log.log(LOGTP_SUCC, "INSERT DATA[%s][NM:%.*s][CLOSE:%.20s][QTY:%d]",
		//	szGroupKey, LEN_CHART_NM, recvUnit.Nm, recvUnit.close, S2N(recvUnit.cntr_qty, sizeof(recvUnit.cntr_qty)));
		//printf("INSERT DATA[%s][NM:%.*s][CLOSE:%.20s][QTY:%d]\n",
		//	szGroupKey, LEN_CHART_NM, recvUnit.Nm, recvUnit.close, S2N(recvUnit.cntr_qty, sizeof(recvUnit.cntr_qty)));
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
	memcpy(existUnit.prevNm, m_sPrevChartNm[ChartIdx(tp)].c_str(), LEN_CHART_NM);

	int nQty = S2N(existUnit.cntr_qty, sizeof(existUnit.cntr_qty)) + S2N(recvUnit.cntr_qty, sizeof(recvUnit.cntr_qty));
	char zQty[LEN_QTY + 1];
	sprintf(zQty, "%*d", LEN_QTY, nQty);
	memcpy(existUnit.cntr_qty, zQty, strlen(zQty));


	//SHM �ݿ�
	bRet = m_shmQ.DataUpdate(szGroupKey, szChartNm, (char*)&existUnit);

	if (!bRet) {
		g_log.log(LOGTP_ERR, "CHART ���� ����(%s)(%s)", szGroupKey, szChartNm);
	}
	else {
		// DB ����
		ST_SHM_CHART_UNIT* p = new ST_SHM_CHART_UNIT;
		memcpy(p, &existUnit, sizeof(ST_SHM_CHART_UNIT));
		memcpy(p->Reserved, szGroupKey, LEN_GROUP_KEY);
		PostThreadMessage(m_dwMainThreadId, WM_SAVE_CHART, 0, (LPARAM)p);

		//Log(TRUE, "NOW CHART[%s][NM:%.4s][NEW_SEQ:%.10s][OPEN:%.20s][NOW_H:%.20s][NOW_L:%.20s][NOW_C:%.20s][%.1s] LAST_NM[%.4s]",
		//	szGroupKey, recvUnit.Nm, recvUnit.seq, existUnit.open, existUnit.high, existUnit.low, recvUnit.close, existUnit.gb, lastNm.LastChartNm);
		//printf("CHART[%s][%.12s][%.12s][O:%7.5f][C:%7.5f][%.1s]\n",
		//	szGroupKey, existUnit.Nm, existUnit.prevNm, atof(existUnit.open), atof(existUnit.close), existUnit.gb);

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
VOID CChartMaker::Chart_SMA(int tp, char* pzGroupKey, char* pzNowChartNm, _Out_ char* i_pNowChart)
{
	ST_SHM_CHART_UNIT* pNowChart = (ST_SHM_CHART_UNIT*)i_pNowChart;

	// �� ���� ��Ʈ ���� �����´�.
	if (!m_shmQ.IsSavedEnoughDataCnt(pzGroupKey, SMA_LONG_CNT + 1))
		return;

	char zChartNm[32], zCloseChartNm[32];
	ST_SHM_CHART_UNIT chartShm;//, stLastChartShm;
	TA_Real stSMA[SMA_LONG_CNT] = { 0, };

	int nLoop = SMA_LONG_CNT;
	sprintf(zChartNm, "%.*s", LEN_CHART_NM, pNowChart->prevNm);
	int nCnt = 1;

	/*
	ARRAY �� �Ųٷ� ��ƾ� �Ѵ�.
	*/
	for (int i = nLoop - 1; i > -1; i--)
	{
		if (strcmp(DEFINE_NO_CHART, zChartNm) == 0)	break;

		if (FALSE == m_shmQ.DataGet(pzGroupKey, zChartNm, (char*)&chartShm))
		{
			g_log.log(LOGTP_ERR, "[Chart_SMA][%s][%s]��Ʈ�� SHM �� ����", pzGroupKey, zChartNm);
			continue;
		}

		// ���� ù��° ��Ʈ ����.
		if (i == nLoop - 1) {
			strcpy(zCloseChartNm, zChartNm);
			//memcpy(&stLastChartShm, &chartShm, sizeof(chartShm));
		}

		sprintf(zChartNm, "%.*s", LEN_CHART_NM, chartShm.prevNm);

		//Log(TRUE, "[Chart_SMA][%d][NOW:%.4s](%.20s)", i, chartShm.Nm, chartShm.close);

		stSMA[i] = atof(chartShm.close);
	}


	TA_Real		outShort[10], outLong[20];
	TA_Integer outBeg, outNbElement;
	TA_Integer outBeg2, outNbElement2;
	TA_RetCode retCode;
	retCode = TA_MA(0, 19, &stSMA[0], 20, TA_MAType_SMA, &outBeg, &outNbElement, &outLong[0]);
	retCode = TA_MA(0, 9, &stSMA[10], 10, TA_MAType_SMA, &outBeg2, &outNbElement2, &outShort[0]);

	char zPrc[32], zDotCnt[32];
	sprintf(zDotCnt, "%.*s", sizeof(chartShm.dotcnt), chartShm.dotcnt);
	int nDotCnt = atoi(zDotCnt);

	double dLong = CUtil::roundoff(outLong[0], nDotCnt);	// �ݿø�
	double dShort = CUtil::roundoff(outShort[0], nDotCnt);	// �ݿø�

	FORMAT_PRC(dLong, nDotCnt, zPrc);
	//memcpy(stLastChartShm.sma_long, zPrc, strlen(zPrc));
	memcpy(pNowChart->sma_long, zPrc, strlen(zPrc));

	FORMAT_PRC(dShort, nDotCnt, zPrc);
	//memcpy(stLastChartShm.sma_short, zPrc, strlen(zPrc));
	memcpy(pNowChart->sma_short, zPrc, strlen(zPrc));

	//g_log.log(LOGTP_SUCC, "[Chart_SMA][LMA:%.*s][SMA:%.*s]", LEN_PRC, stLastChartShm.sma_long, LEN_PRC, stLastChartShm.sma_short);

	//SHM �ݿ�
	//m_shmQ.DataUpdate(pzGroupKey, zCloseChartNm, 0, sizeof(ST_SHM_CHART_UNIT), (char*)&stLastChartShm);
}

void CChartMaker::CloseChartShm()
{
	m_shmQ.Close();

}

