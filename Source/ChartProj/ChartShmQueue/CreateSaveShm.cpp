

#include "../../IRUM_INC/IRUM_Common.h"
#include "CreateSaveShm.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/Util.h"
#include <list>
#include <string>

extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];

CCreateSaveShm::CCreateSaveShm(char *pzArtcSymbol)
{
	strcpy(m_zArtc, pzArtcSymbol);

	GET_SHM_NM(pzArtcSymbol, m_zShmNm);
	GET_SHM_LOCK_NM(pzArtcSymbol, m_zMutexNm);
	m_pDBPool = NULL;
}


CCreateSaveShm::~CCreateSaveShm()
{
	m_bContinue = FALSE;
	StopThread();
	SAFE_DELETE(m_pDBPool);
}

BOOL CCreateSaveShm::Initialize()
{
	if (!Create())
		return FALSE;

	// LASTCHART SHM OPEN
	//if (!m_shmLast.Open(LASTCHART_SHM_NM, LASTCHART_LOCK_NM))
	//{
	//	g_log.log(LOGTP_ERR, "최종차트저장SHM OPEN FAIL(%s)(%s)", LASTCHART_SHM_NM, LASTCHART_LOCK_NM);
	//	return FALSE;
	//}

	char ip[32], id[32], pwd[32], cnt[32], dbname[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", dbname);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_POOL_CNT", cnt);

	m_pDBPool = new CDBPoolAdo(ip, id, pwd, dbname);
	if (!m_pDBPool->Init(atoi(cnt)))
	{
		g_log.log(LOGTP_ERR, "DB OPEN FAIL(IP:%s)(ID:%s)(PWD:%s)(DB:TRADE2010)(MSG:%s)",ip, id, pwd, m_pDBPool->GetMsg());
		SAFE_DELETE(m_pDBPool);
		return FALSE;
	}


	ResumeThread();

	return TRUE;
}

BOOL CCreateSaveShm::Create()
{
	INIT_DATA_Q	shmData;
	shmData.lMaxGroupCnt = MAX_SYMBOL_PER_ARTC * CHART_TP_CNT;			// 차트종류만큼 
	shmData.lGroupKeySize = LEN_SHM_GROUP_KEY;		// SYMBOL + 차트종류
	shmData.lHeaderSize = sizeof(ST_SHM_CHART_HEADER);
	shmData.lStructSize = sizeof(_ST_SHM_CHART_UNIT);
	shmData.lStructKeySize = LEN_SHM_STRUCT_KEY;
	shmData.lMaxStructCnt = MAX_CHART_UNIT_CNT;
	
	//	CREATE SHM
	int nTotSize = m_shmQ.Create(NULL, &shmData, m_zShmNm, m_zMutexNm);
	if (nTotSize<1)
	{
		sprintf(m_zMsg,"[SHM CREATE ERROR](%s)(%s)(%s)", m_zShmNm, m_zMutexNm, m_shmQ.GetErr());
		g_log.log(LOGTP_ERR, "%s", m_zMsg);
		return FALSE;
	}

	// already created
	else if (nTotSize == 0)
	{
		if (!m_shmQ.Open(m_zShmNm, m_zMutexNm))
		{
			sprintf(m_zMsg, "[SHM OPEN ERROR](%s)(%s)(%s)\n", m_zShmNm, m_zMutexNm, m_shmQ.GetErr());
			g_log.log(LOGTP_ERR, "%s", m_zMsg);
			return FALSE;
		}
		//g_log.log(LOGTP_SUCC, "이미 SHM 이 생성되어 있어서 OPEN 한다.(%s)(%s)", m_zShmNm, m_zMutexNm);
		//printf("이미 SHM 이 생성되어 있어서 OPEN 한다.(%s)(%s)\n", m_zShmNm, m_zMutexNm);
	}
	else
	{
		//if (!m_shm.SetInitSize(shmData.lMaxGroupCnt
		//	, shmData.lGroupKeySize
		//	, shmData.lHeaderSize
		//	, shmData.lStructSize
		//	, shmData.lStructKeySize
		//	, shmData.lMaxStructCnt)
		//	)
		//{
		//	sprintf(m_zMsg, "[SHM SetInitSize ERROR](%s)(%s)", m_zShmNm, m_zMutexNm);
		//	g_log.log(LOGTP_ERR, "%s", m_zMsg);
		//	return FALSE;
		//}
	}
	//int Size = shmData.lMaxGroupCnt*CHART_TP_CNT*MAX_CHART_UNIT_CNT * sizeof(ST_SHM_CHART_UNIT);
	sprintf(m_zMsg, "(%s)(%s) succeeded in creating SHM.SHM size = 총그룹갯수(심볼수:%d)*차트수(%d)*차트당UNIT수(%d)*UNIT크기(%d) = %d (%10.2f MB)",
		m_zShmNm, m_zMutexNm,
		MAX_SYMBOL_PER_ARTC, CHART_TP_CNT, MAX_CHART_UNIT_CNT, sizeof(ST_SHM_CHART_UNIT),
		nTotSize, (double)nTotSize /1024./1024.);

	g_log.log(LOGTP_SUCC, m_zMsg);
	printf("%s\n", m_zMsg);

	return TRUE;
}


VOID CCreateSaveShm::ThreadFunc()
{
	BOOL bInit = FALSE;

	//for (int i = TP_1MIN; i < TP_DAY; i++)
	//{
	//	if (!m_bContinue)
	//		return;

	//	if (!LoadShmWrapper(i)) {
	//		printf("LoadSymbolError(%s)(%d)\n", m_zArtc, i);
	//		return;
	//	}
	//}

	while (m_bContinue)
	{
		if (WaitForSingleObject(m_hDie, INFINITE) == WAIT_OBJECT_0) {
			break;
		}


	}
}
//

BOOL	CCreateSaveShm::LoadShmWrapper(/*CHART_TP*/int  tp)
{
	__try
	{
		return LoadShm(tp);
	}
	__except (ReportException(GetExceptionCode(), "CCreateSaveShm::LoadShmWrapper", m_zMsg))
	{
		g_log.log(LOGTP_FATAL, m_zMsg);
	}
	return FALSE;
}


BOOL	CCreateSaveShm::LoadShm(/*CHART_TP*/int  tp)
{
	CDBHandlerAdo db(m_pDBPool->Get());
	char zQ[1024];
	std::list<std::string> listGroupKey;

	sprintf(zQ, "EXEC CHART_GET_RECENTCHART_KEY '%s', %d", m_zArtc, (INT)tp);
	if (FALSE == db->QrySelect(zQ))
	{
		g_log.log(LOGTP_ERR, "CHART DATA Load-1 Fail(%s)(%s)", db->GetError(), zQ);
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "Load Initial Chart Data(%s)", m_zArtc);

	while (db->IsNextRow())
	{
		char zGroup[128];
		db->GetStr("GROUP_KEY", zGroup);
		listGroupKey.push_back(std::string(zGroup));
		db->Next();
	}

	db->Close();

	std::list<std::string>::iterator it;
	for (it = listGroupKey.begin(); it != listGroupKey.end(); it++)
	{
		std::string sKey = *it;
		sprintf(zQ, "EXEC CHART_GET_RECENTCHART_DATA %d, '%s' ", (INT)tp, sKey.c_str());

		if (FALSE == db->QrySelect(zQ))
		{
			g_log.log(LOGTP_ERR, "CHART DATA Load error(%s)(%s)", db->GetError(), zQ);
			printf("CHART DATA Load Error(%s)(%s)\n", db->GetError(), zQ);
			return FALSE;
		}

		
		int nLoop = 0;
		char zTemp[128];
		CHAR zGroupKey[LEN_GROUP_KEY + 1];
		char zChartNm[LEN_CHART_NM + 1], zPrevNm[LEN_CHART_NM + 1];
		ST_SHM_CHART_UNIT stShm;
		while (db->IsNextRow())
		{
			sprintf(zGroupKey, "%.*s", LEN_GROUP_KEY, db->GetStr("GROUP_KEY", zTemp));
			sprintf(zChartNm, "%.*s", LEN_CHART_NM, db->GetStr("CHART_NM", zTemp));
			sprintf(zPrevNm, "%.*s", LEN_CHART_NM, db->GetStr("PREV_NM", zTemp));

			ZeroMemory(&stShm, sizeof(stShm));

			sprintf(stShm.Nm, "%.*s", LEN_CHART_NM, zChartNm);
			sprintf(stShm.prevNm, "%.*s", LEN_CHART_NM, zPrevNm);
			sprintf(stShm.gb, "%.1s", db->GetStr("CHART_GB", zTemp));
			sprintf(stShm.open, "%*.5f", LEN_PRC, db->GetDouble("OPEN_PRC"));
			sprintf(stShm.high, "%*.5f", LEN_PRC, db->GetDouble("HIGH_PRC"));
			sprintf(stShm.low, "%*.5f", LEN_PRC, db->GetDouble("LOW_PRC"));
			sprintf(stShm.close, "%*.5f", LEN_PRC, db->GetDouble("CLOSE_PRC"));
			sprintf(stShm.sma_short, "%*.5f", LEN_PRC, db->GetDouble("SMA_SHORT"));
			sprintf(stShm.sma_long, "%*.5f", LEN_PRC, db->GetDouble("SMA_LONG"));
			//sprintf(stShm.seq, "%.*s", sizeof(stShm.seq), db->Get("PACK_TM"));
			sprintf(stShm.cntr_qty, "%.*s", sizeof(stShm.cntr_qty), db->GetStr("CNTR_QTY", zTemp));


			//g_log.log(LOGTP_SUCC, "[DBREAD][GROUP_KEY:%s][CHART_NM:%s][PREV_NM:%s]", zGroupKey, zChartNm, zPrevNm);
			//printf("[DBREAD][GROUP_KEY:%s][CHART_NM:%s][PREV_NM:%s]", zGroupKey, zChartNm, zPrevNm);

			// DESC 이므로 첫번째를 LAST CHART_NM 으로 한다.
			if (nLoop == 0)
			{
				m_shmQ.HeaderSetData(0, LEN_CHART_NM, zChartNm);
			}

			// GROUP KEY 
			if (!m_shmQ.GroupFind(zGroupKey))
			{
				m_shmQ.GroupInsert(zGroupKey);
				//g_log.log(LOGTP_SUCC, "[GROUP INSERT][%s]", zGroupKey);
				m_shmQ.DataInsert(zGroupKey, (char*)&stShm);
				//g_log.log(LOGTP_SUCC, "[DATA INSERT][%s][%.*s][%.*s]", zGroupKey, LEN_CHART_NM, stShm.Nm, LEN_CHART_NM, stShm.prevNm);
				//printf("[DATA INSERT][%s][%.*s][%.*s]\n", zGroupKey, LEN_CHART_NM, stShm.Nm, LEN_CHART_NM, stShm.prevNm);
			}
			else
			{
				if (m_shmQ.DataFind(zGroupKey, zChartNm)<1)
				{
					m_shmQ.DataInsert(zGroupKey, (char*)&stShm);
					//g_log.log(LOGTP_SUCC, "[DATA INSERT][%s][%.*s][%.*s]", zGroupKey, LEN_CHART_NM, stShm.Nm, LEN_CHART_NM, stShm.prevNm);
					//printf("[DATA INSERT][%s][%.*s][%.*s]\n", zGroupKey, LEN_CHART_NM, stShm.Nm, LEN_CHART_NM, stShm.prevNm);

					
					//int ret = m_shmQ.DataFind(zGroupKey, "201708151604");
					//BOOL bRet = m_shmQ.DataGet(zGroupKey, "201708151604",(char*) &debug);
					// ret = m_shmQ.DataFind(zGroupKey, debug.prevNm);
				}
			}

			// LAST SHM
			//ST_SHM_LAST_CHART last; ZeroMemory(&last, sizeof(last));
			//memcpy(last.Nm, zChartNm, LEN_CHART_NM);
			//if (!m_shmLast.FindData(zGroupKey, zGroupKey))
			//{	
			//	m_shmLast.InsertGroup(zGroupKey);
			//	m_shmLast.InsertData(zGroupKey, (char*)&last);
			//}
			//else
			//{
			//	m_shmLast.UpdateData(zGroupKey, zGroupKey, (char*)&last);
			//}
			nLoop++;
			db->Next();
		} // while (db->NextRow())

		db->Close();


	} // for (it = listGroupKey.begin(); it != listGroupKey.end(); it++)
	return TRUE;
}


//VOID	CCreateSaveShm::DBSave(/*CHART_TP*/int  tp)
//{
//	char zGroupKey[LEN_GROUP_KEY + 1] = { 0, };
//
//	if (m_shmQ.FindFirstGroup() < 1)
//		return;
//
//	char zData[1024] = { 0, };
//	ST_SHM_CHART_UNIT *p = (ST_SHM_CHART_UNIT*)zData;
//	char zShmChartTp[32], zChartTp[32];
//	sprintf(zChartTp, "%02d", (int)tp);
//
//	char zChartNm[32];
//
//
//	BOOL bNextGroup = TRUE;
//	while (bNextGroup && m_bContinue)
//	{
//		bNextGroup = m_shm.GetNextGroup(zGroupKey);
//		if (strlen(zGroupKey) == 0)
//			break;
//
//		// CHART TP 점검
//		sprintf(zShmChartTp, "%.2s", zGroupKey + 5);
//		if (strcmp(zShmChartTp, zChartTp)!=0)
//			continue;
//
//		if (m_shm.FindFirstData(zGroupKey) < 0)
//			continue;
//
//		ZeroMemory(p, sizeof(ST_SHM_CHART_UNIT));
//		BOOL bNext = TRUE;
//		while (bNext && m_bContinue)
//		{
//			bNext = m_shm.GetNextData((char*)p);
//			if (p == NULL)
//				break;
//
//			sprintf(zChartNm, "%.*s", LEN_CHART_NM, p->Nm);
//
//			// 이미 저장한 데이터이면 pass
//			if (m_sLastSaveNm[tp].compare(zChartNm) >= 0)
//				continue;
//
//			CDBHandlerAdo db(m_pDBPool->Get());
//			char zQ[1024];
//			sprintf(zQ, "EXEC CHART_SAVE "
//				"'%.*s', "	//@I_GROUP_KEY	VARCHAR(5)--// CLN71
//				"'%.*s', "	//, @I_CHART_NM	VARCHAR(20)
//				"'%.*s', "	//, @I_PREV_NM		VARCHAR(20)
//				"'%.*s', "	//@I_CHART_GB	CHAR(1)--// +,-, 0
//				"'%.*s', "	//@I_OPEN_PRC	VARCHAR(20)
//				"'%.*s', "	//@I_HIGH_PRC	VARCHAR(20)
//				"'%.*s', "	//@I_LOW_PRC		VARCHAR(20)
//				"'%.*s', "	//@I_CLOSE_PRC	VARCHAR(20)
//				"'%.*s', "	//@I_CNTR_QTY	VARCHAR(20)
//				"'%.*s', "	//@I_DOT_CNT		VARCHAR(20)
//				"'%.*s', "	//@I_SMA_SHORT	VARCHAR(20)
//				"'%.*s' "	//@I_SMA_LONG	VARCHAR(20)
//				//"'%.*s' "
//				,
//				LEN_GROUP_KEY, zGroupKey,
//				sizeof(p->Nm), p->Nm,
//				sizeof(p->prevNm), p->prevNm,
//				sizeof(p->gb), p->gb,
//				sizeof(p->open), p->open,
//				sizeof(p->high), p->high,
//				sizeof(p->low), p->low,
//				sizeof(p->close), p->close,
//				sizeof(p->cntr_qty), p->cntr_qty,
//				sizeof(p->dotcnt), p->dotcnt,
//				sizeof(p->sma_short), p->sma_short,
//				sizeof(p->sma_long), p->sma_long
//				//sizeof(p->seq), p->seq
//			);
//			if (FALSE == db->ExecQuery(zQ))
//			{
//				g_log.log(LOGTP_ERR, "CHART DATA Save 에러(%s)(%s)", db->GetError(), zQ);
//				printf( "CHART DATA Save 에러(%s)(%s)\n", db->GetError(), zQ);
//				continue;
//			}
//
//			m_sLastSaveNm[tp] = zChartNm;
//			//if(strncmp(zGroupKey, "GCQ7", 4)!=0)
//				printf("DB SAVE(%s)\n", zQ);
//			
//		} //while (m_shm.GetNextData((char*)&stShm))
//
//		//printf("DB SAVE(%s)(%s)\n", zGroupKey);
//	} // while (m_shm.GetNextGroup(zGroupKey))
//
//}
//
//
//VOID	CCreateSaveShm::DBSaveWrapper(/*CHART_TP*/int  tp)
//{
//	__try
//	{
//		DBSave(tp);
//	}
//	__except (ReportException(GetExceptionCode(), "CCreateSaveShm::DBSaveWrapper", m_zMsg))
//	{
//		g_log.log(LOGTP_FATAL, m_zMsg);
//	}
//}