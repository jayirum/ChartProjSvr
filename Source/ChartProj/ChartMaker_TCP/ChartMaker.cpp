
#include "ChartMaker.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "../../IRUM_UTIL/ADOFunc.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include "C:\ta-lib\c\include\ta_libc.h"


extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];
extern CMemPool	*g_pMemPool;

// 120�� ��Ʈ�� 6�� ���� �����Ѵ�
void fnGET_CHART_NM_EX(char* date, char*time, int tp, char* out)
{
	int divider; char tmp[32];	char zTm[32];
	sprintf(zTm, "%.8s", time);
	if (tp == TP_1MIN) divider = 1;
	if (tp == TP_3MIN) divider = 3;
	if (tp == TP_5MIN) divider = 5;
	if (tp == TP_10MIN) divider = 10;
	if (tp == TP_15MIN) divider = 15;
	if (tp == TP_20MIN) divider = 20;
	if (tp == TP_60MIN) divider = 60;
	if (tp == TP_120MIN) divider = 120;
	sprintf(tmp, "%.2s", (zTm + 3));
	int ret = (atoi(tmp) / divider);
	int min = (ret)*divider;
	if (tp == TP_60MIN) {
		int h = S2N(zTm, 2); if (h == 24) h = 0; sprintf(out, "%.8s%02d00", date, h);
	}
	else if (tp == TP_120MIN) {
		int h = S2N(zTm, 2);
		int hRemain = h % 2;
		if (hRemain == 1)	h -= 1;
		if (h == 24) h = 0; sprintf(out, "%.8s%02d00", date, h);
	}
	else
		sprintf(out, "%.8s%.2s%02d", date, zTm, min);
	//if(min==60){ int h = S2N(zTm,2)+1; if(h==24) h=0; sprintf(out, "%.8s%02d00",date,h);}	
	//else if(min==120){ int h = S2N(zTm,2)+2; if(h==24) h=0; sprintf(out, "%.8s%02d00",date,h);}	
	//else sprintf(out, "%.8s%.2s%02d",date, zTm, min);
}


CChartMaker::CChartMaker(char* pzSymbol, char* pzArtcCode, /*CMemPool* pMemPool,*/ unsigned dwMainThreadId)
{
	m_dwMainThreadId = dwMainThreadId;
	//m_pMemPool = pMemPool;
	strcpy(m_zSymbol, pzSymbol);
	strcpy(m_zArtc, pzArtcCode);
	GET_SHM_NM(pzArtcCode, m_zShmNm);
	GET_SHM_LOCK_NM(pzArtcCode, m_zMutexNm);

	ResumeThread();

	// worker thread ����
	m_hWorkDie = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hWorkThread = (HANDLE)_beginthreadex(NULL, 0, &WorkThread, this, 0, &m_dwWorkThreadID);
}

CChartMaker::~CChartMaker()
{
	CloseChartShm();
	//EndMcastRecv();
	CloseMemPool();

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


	for (int i = TP_1MIN; i < TP_DAY; i++)
	{
		char zGroupKey[LEN_GROUP_KEY + 1] = { 0, };
		GET_GROUP_KEY(m_zSymbol, i, zGroupKey);

		char zStructKey[LEN_CHART_NM + 1] = { 0, };
		if (m_shmQ.GetCurrStructKey(zGroupKey, zStructKey))
		{
			m_sLastChartNm[i] = zStructKey;
		}
		else
		{
			//
		}
	}
	return TRUE;
}



//
//BOOL CChartMaker::InitChartSHM()
//{
//	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
//	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);
//
//	//	LAST SHM OPEN
//	//if (!m_shmLast.Open(LASTCHART_SHM_NM, LASTCHART_LOCK_NM))
//	//{
//	//	g_log.log(LOGTP_FATAL, ">>>>>>>LAST CHART SHM OPEN ����(%s)(symbol:%s)(%s)", LASTCHART_SHM_NM, LASTCHART_LOCK_NM, m_shmLast.GetErr());
//	//	return FALSE;
//	//}
//	
//	//	OPEN SHM
//	if (!m_shmQ.Open((LPCTSTR)m_zShmNm, (LPCTSTR)m_zMutexNm))
//	{
//		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN ����(%s)(symbol:%s)(%s)",m_zShmNm, m_zSymbol, m_shmQ.GetErr());
//		return FALSE;
//	}
//	g_log.log(LOGTP_SUCC, "SHM OPEN ����(%s)", m_zMutexNm);
//
//
//	// DB ���� ������ ��Ʈ �̸��� �����´�.
//	char ip[32], id[32], pwd[32], name[32];
//	CUtil::GetConfig(szConfig, "DBINFO", "DB_IP", ip);
//	CUtil::GetConfig(szConfig, "DBINFO", "DB_ID", id);
//	CUtil::GetConfig(szConfig, "DBINFO", "DB_PWD", pwd);
//	CUtil::GetConfig(szConfig, "DBINFO", "DB_NAME", name);
//
//	CDBPoolAdo *pDBPool;
//	pDBPool = new CDBPoolAdo(ip, id, pwd, name);
//	if (!pDBPool->Init(1))
//	{
//		SAFE_DELETE(pDBPool);
//		return 0;
//	}
//	CDBHandlerAdo db(pDBPool->Get());
//	char zQ[1024];
//
//
//	for (int i = TP_1MIN; i < TP_DAY; i++)
//	{
//		sprintf(zQ, "EXEC CHART_GET_LASTCHART_NM '%s', %d ", m_zSymbol, i);
//		if (FALSE == db->QrySelect(zQ))
//		{
//			g_log.log(LOGTP_ERR, "��������Ʈ ��ȸ ����(%s)(%s)", db->GetError(), zQ);
//			return 0;
//		}
//
//
//		while (db->IsNextRow())
//		{
//			char temp[128];
//			int nTp = db->GetLong("CHART_TP");
//			m_sLastChartNm[nTp] = db->GetStr("CHART_NM", temp);
//			db->Next();
//		}
//	}
//	db->Close();
//
//	return TRUE;
//}
//




BOOL CChartMaker::InitMemPool()
{
	m_pMemPool = new CMemPool(MEMPOOL_PRE_ALLOC, MEMPOOL_MAX_ALLOC, MEMPOOL_BLOCK_SIZE);

	if (m_pMemPool->available() == 0)
	{
		return FALSE;
	}
	return TRUE;
}
VOID CChartMaker::CloseMemPool()
{
	SAFE_DELETE(m_pMemPool);
}

VOID CChartMaker::ThreadFunc()
{
	//printf("thread:%d\n", GetCurrentThreadId());
	if (!InitChartSHM())
	{
		g_log.log(LOGTP_ERR, ">>>>>> SHM OPEN ERROR <<<<<<<<<");
		return;
	}

	if( !InitMemPool())
	{
		g_log.log(LOGTP_ERR, ">>>>>> Mempool OPEN ERROR <<<<<<<<<");
		return;
	}

	

	//char zMarketDataBuff[1024] = { 0, };
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
				ST_PACK2CHART_EX* p = (ST_PACK2CHART_EX*)msg.lParam;		// MEM POOL
				if (strncmp(m_zSymbol, p->ShortCode, nSymbolLen) != 0) {
					g_pMemPool->release((char*)msg.lParam); // main ���� �Ѿ�� �޸�
					continue;
				}
				char* pData = m_pMemPool->get();
				strcpy(pData, (char*)msg.lParam);
				int nLen = strlen(pData);

				PostThreadMessage(m_dwWorkThreadID, WM_RECV_API_MD, (WPARAM)nLen, (LPARAM)pData);

				// main ���� �Ѿ�� �޸�
				g_pMemPool->release((char*)msg.lParam);
				break;
			}
			} // switch (msg.message)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)
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
				//TODO
				//g_log.log(LOGTP_ERR, "RECV(%s)", pData);
				int nLen = (int)msg.wParam;

				int i = 0;
				for (i = (int)TP_1MIN; i < (int)TP_DAY; i++)	//��,��,���� ��ġ��
				{
					//TODO 
					//i = (int)TP_1MIN;
					p->ChartProc((void*)msg.lParam, (int)i);
					//break;
				}
				p->m_pMemPool->release(pData);
			} //if (msg.message == WM_SEND_STRATEGY)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)
	return 0;
}


VOID	CChartMaker::ChartProc(VOID* pIn, int tp)
{

	ST_PACK2CHART_EX* p = (ST_PACK2CHART_EX*)pIn;
	char temp[32];
	BOOL bRet;

	ST_SHM_CHART_UNIT recvUnit;
	memset(&recvUnit, 0x20, sizeof(recvUnit));

	// GROUP KEY (symbol+charttp)
	char szGroupKey[LEN_SHM_GROUP_KEY + 1];
	GET_GROUP_KEY(p->ShortCode, tp, szGroupKey);

	// STRUCT KEY (chart name) 0000, 0005, 0010
	char szChartNm[LEN_SHM_STRUCT_KEY + 1];
	GET_CHART_NM_EX(p->Date, p->Time, tp, szChartNm);

	// SEQNO �ڸ��� ��Ŷ�ð��� ���´�.
	//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

	///////////////////////////////////////////////////////////////////////////////////////
	// chart unit ����

	// DB���� �ƹ��͵� ���� ��� (If there is no data from DB)
	if (m_sLastChartNm[tp].empty())
	{
		m_sPrevChartNm[tp] = DEFINE_NO_CHART;
		m_sLastChartNm[tp] = szChartNm;
	}


	/*
		������Ʈ�̸��� ���簡 �Ҽ� ��Ʈ�̸��� �ٸ� ��� ==> �ι�° ��Ʈ�� ó������ �����ϴ� ����̴�.
		- �� ��� ������Ʈ�̸��� ������Ʈ�̸��� �����ϰ�, ������Ʈ�̸����� ������Ʈ�̸��� �����Ѵ�.

		��, ������Ʈ�̸��� ������Ʈ�̸��� ������ ��� ���� ��Ʈ�� ��� �׷����� �ִ� ����̹Ƿ�
		������Ʈ�̸��� ������Ʈ�̸��� �������� �ʴ´�.
	*/
	else if (m_sLastChartNm[tp].compare(szChartNm) != 0)
	{
		m_sPrevChartNm[tp] = m_sLastChartNm[tp];
		m_sLastChartNm[tp] = szChartNm;
	}

	// recvUnit �� packet ������ ����
	memcpy(recvUnit.Nm, szChartNm, sizeof(recvUnit.Nm));	// hhmm		0000, 0005, 0010
	memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].c_str(), min(sizeof(recvUnit.prevNm), m_sPrevChartNm[tp].size()));
	memcpy(recvUnit.open, p->Close, sizeof(p->Close));
	memcpy(recvUnit.high, p->Close, sizeof(p->Close));
	memcpy(recvUnit.low, p->Close, sizeof(p->Close));
	memcpy(recvUnit.close, p->Close, sizeof(p->Close));
	memcpy(recvUnit.cntr_qty, p->CntrQty, sizeof(p->CntrQty));
	memcpy(recvUnit.dotcnt, p->DecLen, sizeof(p->DecLen));


	// group(SYMBOL+TP) �� ã�Ƽ� ������ insert. ��, ���� ���� (Find Group)
	if (!m_shmQ.GroupFind(szGroupKey))	
	{
		if (!m_shmQ.GroupInsert(szGroupKey))
		{
			g_log.log(LOGTP_ERR, "GroupInsert ERROR!!!(%s)(%s)", m_zSymbol, szGroupKey);
			return;
		}
		printf("GroupInsert(%s)(%s)\n", m_zSymbol, szGroupKey);
		if(!m_shmQ.DataInsert(szGroupKey, (char*)&recvUnit))
		{
			g_log.log(LOGTP_ERR, "DataInsert ERROR-1!!!(%s)(%s)(%s)", m_zSymbol, szGroupKey, szChartNm);
			return;
		}
		printf("DataInsert-1(%s)(%s)(%s)\n", m_zSymbol, szGroupKey, szChartNm);
		//if (strcmp(m_zSymbol, "6BZ7") == 0) {
		//	g_log.log(LOGTP_SUCC, "Insert Group/Data[%s][NM:%.*s][O:%.20s][H:%.20s][L:%.20s][C:%.20s](Q:%.20s)",
		//		szGroupKey, LEN_CHART_NM, recvUnit.Nm, recvUnit.open, recvUnit.high, recvUnit.low, recvUnit.close, recvUnit.cntr_qty);
		//}
		return;
	}

	// �����͸� ã�´�. 
	ST_SHM_CHART_UNIT existUnit;
	bRet = m_shmQ.DataGet(szGroupKey, szChartNm, (char*)&existUnit); // Get data using chart name (struct key)
	
	// �����Ͱ� ����. insert. ��, �ð� �Է��̶�� ����̴�.
	if (!bRet)
	{
		memcpy(recvUnit.open, p->Close, sizeof(p->Close));
		memcpy(recvUnit.low, p->Close, sizeof(p->Close));
		memcpy(recvUnit.high, p->Close, sizeof(p->Close));
		//memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].c_str(), LEN_CHART_NM);
		memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].c_str(), min(sizeof(recvUnit.prevNm), m_sPrevChartNm[tp].size()));

		//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

		// �ð��� ���� ��� ������Ʈ�� SMA �� ���ؼ� �����Ѵ�.
		Chart_SMA(tp, szGroupKey, szChartNm, (char*)&recvUnit);

		if (!m_shmQ.DataInsert(szGroupKey, (char*)&recvUnit))
		{
			g_log.log(LOGTP_ERR, "DataInsert ERROR-2!!!(%s)(%s)(%s)", m_zSymbol, szGroupKey, szChartNm);
			return;
		}
		printf("DataInsert-2(%s)(%s)(%s)\n", m_zSymbol, szGroupKey, szChartNm);

		// DB ����
		ST_SHM_CHART_UNIT* pChart = new ST_SHM_CHART_UNIT;
		memcpy(pChart, &recvUnit, sizeof(ST_SHM_CHART_UNIT));
		memcpy(pChart->Reserved, szGroupKey, LEN_GROUP_KEY);
		PostThreadMessage(m_dwMainThreadId, WM_SAVE_CHART, 0, (LPARAM)pChart);
		//if (strcmp(m_zSymbol, "6BZ7") == 0) {
		//	g_log.log(LOGTP_SUCC, "DB INSERT Data[%s][NM:%.*s][O:%.20s][H:%.20s][L:%.20s][C:%.20s](Q:%.20s)",
		//		szGroupKey, LEN_CHART_NM, pChart->Nm, pChart->open, pChart->high, pChart->low, pChart->close, pChart->cntr_qty);
		//}
		return;
	}

	// �����Ͱ� �ִ�.

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
	//memcpy(existUnit.prevNm, m_sPrevChartNm[tp].c_str(), LEN_CHART_NM);
	memcpy(existUnit.prevNm, m_sPrevChartNm[tp].c_str(), min(sizeof(existUnit.prevNm), m_sPrevChartNm[tp].size()));

	int nQty = S2N(existUnit.cntr_qty, sizeof(existUnit.cntr_qty)) + S2N(recvUnit.cntr_qty, sizeof(recvUnit.cntr_qty));
	char zQty[LEN_QTY + 1];
	sprintf(zQty, "%*d", LEN_QTY, nQty);
	memcpy(existUnit.cntr_qty, zQty, strlen(zQty));


	//SHM �ݿ�
	bRet = m_shmQ.DataUpdate(szGroupKey, szChartNm, (char*)&existUnit);

	if (!bRet) {
		g_log.log(LOGTP_ERR, "DataUpdate ERROR!!!(%s)(%s)(%s)", m_zSymbol, szGroupKey, szChartNm);
		return;
	}
	else {
		printf("DataUpdate(%s)(%s)(%s)\n", m_zSymbol, szGroupKey, szChartNm);
		// DB ����
		ST_SHM_CHART_UNIT* pChart = new ST_SHM_CHART_UNIT;
		memcpy(pChart, &existUnit, sizeof(ST_SHM_CHART_UNIT));
		memcpy(pChart->Reserved, szGroupKey, LEN_GROUP_KEY);
		PostThreadMessage(m_dwMainThreadId, WM_SAVE_CHART, 0, (LPARAM)pChart);
		//if (strcmp(m_zSymbol, "6BZ7") == 0) {
		//	g_log.log(LOGTP_SUCC, "UPDATE Data[%s][NM:%.*s][O:%.20s][H:%.20s][L:%.20s][C:%.20s](Q:%.20s)",
		//		szGroupKey, LEN_CHART_NM, pChart->Nm, pChart->open, pChart->high, pChart->low, pChart->close, pChart->cntr_qty);
		//}
		//g_log.log(LOGTP_SUCC, "UPDATE DATA[%s][NM:%.*s][CLOSE:%.20s](%.*s)",
		//	szGroupKey, LEN_CHART_NM, pChart->Nm, pChart->close, sizeof(ST_SHM_CHART_UNIT), (char*)pChart);

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
	if(!m_shmQ.IsSavedEnoughDataCnt(pzGroupKey, SMA_LONG_CNT + 1))
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

