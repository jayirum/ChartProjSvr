
#include "../../IRUM_INC/IRUM_Common.h"
#include "../../IRUM_UTIL/ADOFunc.h"
#include "SignalMaker.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include "Main.h"


extern CLogMsg g_log;

CSignalMaker::CSignalMaker(char* pzSymbol, char* pzArtcCode, CMemPool* pMemPool, unsigned dwMainThreadId)
{
	m_dwMainThreadId = dwMainThreadId;
	m_pMemPool = pMemPool;
	strcpy(m_zSymbol, pzSymbol);
	strcpy(m_zArtc, pzArtcCode);
	GET_SHM_NM(pzArtcCode, m_zShmNm);
	GET_SHM_LOCK_NM(pzArtcCode, m_zMutexNm);
	m_pShmQ = NULL;
	m_pNanoSub = NULL;

	ResumeThread();
}

CSignalMaker::~CSignalMaker()
{
	EndShm();
	EndSubScribe();
}

BOOL CSignalMaker::IsMySymbol(char* pSise)
{
	return TRUE;
}


BOOL CSignalMaker::BeginShm()
{
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);

	//	OPEN SHM
	if (!m_pShmQ->Open((LPCTSTR)m_zShmNm, (LPCTSTR)m_zMutexNm))
	{
		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN 에러(%s)(symbol:%s)(%s)", m_zShmNm, m_zSymbol, m_pShmQ->GetErr());
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "SHM OPEN 성공(%s)", m_zMutexNm);


	for (int i = TP_1MIN; i < TP_DAY; i++)
	{
		char zGroupKey[LEN_GROUP_KEY + 1] = { 0, };
		GET_GROUP_KEY(m_zSymbol, i, zGroupKey);

		char zStructKey[LEN_CHART_NM + 1] = { 0, };
		if (m_pShmQ->GetCurrStructKey(zGroupKey, zStructKey))
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




VOID CSignalMaker::ThreadFunc()
{
	printf("thread:%d\n", GetCurrentThreadId());
	if (!BeginShm())
	{
		g_log.log(LOGTP_ERR, ">>>>>> SHM OPEN ERROR <<<<<<<<<");
		return;
	}



	//char zSiseBuff[1024];

	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, INFINITE, QS_ALLPOSTMESSAGE);
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
				int i = 0;
				for (i = (int)TP_1MIN; i < (int)TP_DAY; i++)	//일,주,월은 배치로
				{
					ChartProc((void*)msg.lParam, (int)i);
				}
				m_pMemPool->release((char*)msg.lParam);
				break;
			}
			} // switch (msg.message)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)
}

// 120분 차트는 6시 부터 시작한다
void fnGET_CHART_NM_EX(char* date, char*time,int tp, char* out)
{	
	int divider; char tmp[32];	char zTm[32];
	sprintf(zTm, "%.8s", time);				
	if(tp ==TP_1MIN) divider=1;		
	if(tp ==TP_3MIN) divider=3;		
	if(tp ==TP_5MIN) divider=5;		
	if(tp ==TP_10MIN) divider=10;		
	if(tp ==TP_15MIN) divider=15;		
	if(tp ==TP_20MIN) divider=20;		
	if(tp ==TP_60MIN) divider=60;		
	if(tp ==TP_120MIN) divider=120;		
	sprintf(tmp, "%.2s", (zTm+3));		
	int ret = (atoi(tmp) / divider); 	
	int min = (ret)*divider;
	if(tp== TP_60MIN){
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

VOID	CSignalMaker::ChartProc(VOID* pIn, int tp)
{
	ST_PACK2CHART* p = (ST_PACK2CHART*)pIn;
	char temp[32];
	BOOL bRet;

	ST_SHM_CHART_UNIT recvUnit;
	memset(&recvUnit, 0x20, sizeof(recvUnit));

	// GROUP KEY (종목+charttp)
	char szGroupKey[LEN_SHM_GROUP_KEY + 1];
	GET_GROUP_KEY(p->ShortCode, tp, szGroupKey);
	//sprintf(szGroupKey, "%-*.*s%02d", LEN_SYMBOL, LEN_SYMBOL, p->shortCode, tp);

	// STRUCT KEY (차트이름) 0000, 0005, 0010
	char szChartNm[LEN_SHM_STRUCT_KEY + 1];
	GET_CHART_NM_EX(p->Date, p->Time, tp, szChartNm);
	//fnGET_CHART_NM_EX(p->Date, p->Time, tp, szChartNm);

	// SEQNO 자리에 패킷시간을 적는다.
	//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

	///////////////////////////////////////////////////////////////////////////////////////
	// chart unit 구성

	// DB에도 아무것도 없는 경우
	if (m_sLastChartNm[tp].empty())
	{
		m_sPrevChartNm[tp] = DEFINE_NO_CHART;
		m_sLastChartNm[tp] = szChartNm;
	}


	// 직전차트 이름과 현재차트 이름이 다르면 저장. 두번째 차트를 처음으로 구성하는 경우
	// 이때 직전차트는 DB 저장할 수 있도록 한다. ==> 매번 하도록 수정(2017.08.23)
	else if (m_sLastChartNm[tp].compare(szChartNm) != 0)
	{
		// DB 저장 코딩
		//char zPrevNm[32]; sprintf(zPrevNm, m_sLastChartNm[tp].c_str());
		//ST_SHM_CHART_UNIT* p = new ST_SHM_CHART_UNIT;
		//if (!m_pShmQ->DataGet(szGroupKey, zPrevNm, (char*)p))
		//{
		//	g_log.log(LOGTP_FATAL, "차트 저장을 위한 조회 오류(%s)(%s)",szGroupKey, zPrevNm);
		//	delete p;
		//}
		//else
		//{
		//	// group key 를 reserve 에 넣는다.
		//	memcpy(p->Reserved, szGroupKey, LEN_GROUP_KEY);
		//	PostThreadMessage(m_dwMainThreadId, WM_SAVE_CHART, 0, (LPARAM)p);
		//}
		m_sPrevChartNm[tp] = m_sLastChartNm[tp];
		m_sLastChartNm[tp] = szChartNm;
	}
	memcpy(recvUnit.Nm, szChartNm, sizeof(recvUnit.Nm));	// hhmm		0000, 0005, 0010
	memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].c_str(), LEN_CHART_NM);
	memcpy(recvUnit.open, p->Close, sizeof(p->Close));
	memcpy(recvUnit.high, p->Close, sizeof(p->Close));
	memcpy(recvUnit.low, p->Close, sizeof(p->Close));
	memcpy(recvUnit.close, p->Close, sizeof(p->Close));
	memcpy(recvUnit.cntr_qty, p->CntrQty, sizeof(p->CntrQty));
	memcpy(recvUnit.dotcnt, p->DecLen, sizeof(p->DecLen));


	// group(SYMBOL+TP) 을 찾아서 없으면 insert. 즉, 최초 저장
	if (!m_pShmQ->GroupFind(szGroupKey))
	{
		//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

		m_pShmQ->GroupInsert(szGroupKey);
		m_pShmQ->DataInsert(szGroupKey, (char*)&recvUnit);

		// DB 저장
		ST_SHM_CHART_UNIT* p = new ST_SHM_CHART_UNIT;
		memcpy(p, &recvUnit, sizeof(ST_SHM_CHART_UNIT));
		memcpy(p->Reserved, szGroupKey, LEN_GROUP_KEY);
		PostThreadMessage(m_dwMainThreadId, WM_SAVE_CHART, 0, (LPARAM)p);

		g_log.log(LOGTP_SUCC, "INSERT GROUP[%s][NM:%.*s][CLOSE:%.20s]",
			szGroupKey, LEN_CHART_NM, recvUnit.Nm, recvUnit.close);

		return;
	}

	// 데이터를 찾는다. 없으면 INSERT. 즉, 시가 입력이라는 얘기이다.
	ST_SHM_CHART_UNIT existUnit;
	if (!m_pShmQ->DataGet(szGroupKey, szChartNm, (char*)&existUnit))
	{
		memcpy(recvUnit.open, p->Close, sizeof(p->Close));
		memcpy(recvUnit.low, p->Close, sizeof(p->Close));
		memcpy(recvUnit.high, p->Close, sizeof(p->Close));
		memcpy(recvUnit.prevNm, m_sPrevChartNm[tp].c_str(), LEN_CHART_NM);

		//memcpy(recvUnit.seq, p->org.Time, sizeof(p->org.Time));

		
		m_pShmQ->DataInsert(szGroupKey, (char*)&recvUnit);

		// DB 저장
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


	// 차트 계산
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

	if (dNow > dOpen)	existUnit.gb[0] = DEF_CANDLE_PLUS;	// 양봉
	if (dNow < dOpen)	existUnit.gb[0] = DEF_CANDLE_MINUS;	// 음봉
	if (dNow == dOpen)	existUnit.gb[0] = '0';

	memcpy(existUnit.close, recvUnit.close, sizeof(recvUnit.close));
	//memcpy(existUnit.seq, recvUnit.seq, sizeof(recvUnit.seq));
	memcpy(existUnit.prevNm, m_sPrevChartNm[tp].c_str(), LEN_CHART_NM);

	int nQty = S2N(existUnit.cntr_qty, sizeof(existUnit.cntr_qty)) + S2N(recvUnit.cntr_qty, sizeof(recvUnit.cntr_qty));
	char zQty[LEN_QTY + 1];
	sprintf(zQty, "%*d", LEN_QTY, nQty);
	memcpy(existUnit.cntr_qty, zQty, strlen(zQty));


	//SHM 반영
	bRet = m_pShmQ->DataUpdate(szGroupKey, szChartNm, (char*)&existUnit);

	if (!bRet) {
		g_log.log(LOGTP_ERR, "CHART 저장 오류(%s)(%s)", szGroupKey, szChartNm);
	}
	else {
		// DB 저장
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


BOOL CSignalMaker::BeginSubscribe()
{
	SAFE_DELETE(m_pNanoSub);

	m_pNanoSub = new CNanoPubSub(TP_SUB);
	return (m_pNanoSub->Begin(INNER_CHANNEL_PUBSUB));
}


VOID CSignalMaker::EndSubScribe()
{
	m_pNanoSub->End();
	SAFE_DELETE(m_pNanoSub);
}

void CSignalMaker::EndShm()
{
	m_pShmQ->Close();
	SAFE_DELETE(m_pShmQ);
}

