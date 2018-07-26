
#include "../Common/KRCommon.h"
#include "SaveChart.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/Util.h"
#include <list>

extern CLogMsg g_log;

CSaveChart::CSaveChart(int chartTp, char* pzConfig)
{
	m_nChartTp = chartTp;
	strcpy(m_zConfig, pzConfig);
}


CSaveChart::~CSaveChart()
{
	m_bContinue = FALSE;
	StopThread();
	SAFE_DELETE(m_pDBPool);
}

BOOL CSaveChart::Initialize()
{
	char ip[32], id[32], pwd[32];
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_PWD", pwd);

	BOOL bUseODBC = FALSE;
	m_pDBPool = new CDBPool(bUseODBC, ip, id, pwd, "KRHedge");
	if (!m_pDBPool->Init(2))
	{
		g_log.log(LOGTP_FATAL,  "DB OPEN FAIL(MSG:%s)", m_pDBPool->GetMsg());
		g_log.log(LOGTP_FATAL,  "(IP:%s)(ID:%s)(PWD:%s)(DB:KRHedgae)", ip, id, pwd);
		printf("DB OPEN ERROR(IP:%s)(ID:%s)(PWD:%s)(DB:KRHedgae)", ip, id, pwd);
		SAFE_DELETE(m_pDBPool);
		return FALSE;
	}

	char szSHMName[128] = { 0, }, szMutexName[128] = { 0, };
	CUtil::GetConfig(m_zConfig, "SHM_INFO", "SHM_NAME", szSHMName);
	CUtil::GetConfig(m_zConfig, "SHM_INFO", "MUTEX_NAME", szMutexName);

	//	OPEN SHM
	if (!m_shm.Open(szSHMName, szMutexName))
	{
		printf("SHM OPEN FAIL(%s)(%s)\n", szSHMName, szMutexName);
		return FALSE;
	}


	CUtil::GetConfig(m_zConfig, "SHM_LASTCHART_NM", "SHM_NAME", szSHMName);
	CUtil::GetConfig(m_zConfig, "SHM_LASTCHART_NM", "MUTEX_NAME", szMutexName);
	if (!m_lastShm.Open(szSHMName, szMutexName))
	{
		printf("LAST SHM OPEN FAIL(%s)(%s)\n", szSHMName, szMutexName);
		return FALSE;
	}


	ResumeThread();

	m_bContinue = TRUE;
	return TRUE;
}

VOID CSaveChart::ThreadFunc()
{
	// DB READ
	if (!LoadShmWrapper())
		return;

	while (TRUE)
	{
		if (WaitForSingleObject(m_hDie, 10000)==WAIT_OBJECT_0)
			break;

		// DB WRITE
		DBSaveWrapper();
	}
}

VOID	CSaveChart::DBSave()
{
	char zGroupKey[LEN_GROUP_KEY + 1] = { 0, };

	if (m_shm.FindFirstGroup() < 1)
		return;

	ST_SHM_CHART_UNIT *p = new ST_SHM_CHART_UNIT;
	char zChartTp[2];
	sprintf(zChartTp, "%d", (int)m_nChartTp);
	BOOL bNextGroup = TRUE;
	while (bNextGroup && m_bContinue)
	{
		bNextGroup = m_shm.GetNextGroup(zGroupKey);
		if (strlen(zGroupKey) == 0)
			break;

		// CHART TP 점검
		if (zGroupKey[strlen(zGroupKey) - 1] != zChartTp[0])
			continue;

		if (m_shm.FindFirstData(zGroupKey) < 0)
			continue;

		ZeroMemory(p, sizeof(ST_SHM_CHART_UNIT));
		BOOL bNext = TRUE;
		while (bNext && m_bContinue)
		{
			bNext = m_shm.GetNextData((char*)p);
			if (p == NULL)
				break;

			CDBHandler db(m_pDBPool->Get());
			char zQ[1024];
			sprintf(zQ, "EXEC FB_CHART_SAVE "
				"'%.*s', "	//@I_GROUP_KEY	VARCHAR(5)--// CLN71
				"'%.*s', "	//, @I_CHART_NM	VARCHAR(20)
				"'%.*s', "	//, @I_PREV_NM		VARCHAR(20)
				"'%.*s', "	//@I_CHART_GB	CHAR(1)--// +,-, 0
				"'%.*s', "	//@I_OPEN_PRC	VARCHAR(20)
				"'%.*s', "	//@I_HIGH_PRC	VARCHAR(20)
				"'%.*s', "	//@I_LOW_PRC		VARCHAR(20)
				"'%.*s', "	//@I_CLOSE_PRC	VARCHAR(20)
				"'%.*s', "	//@I_DOT_CNT		VARCHAR(20)
				"'%.*s', "	//@I_SMA_SHORT	VARCHAR(20)
				"'%.*s', "	//@I_SMA_LONG	VARCHAR(20)
				"'%.*s' "
				,
				LEN_GROUP_KEY, zGroupKey,
				sizeof(p->Nm), p->Nm,
				sizeof(p->prevNm), p->prevNm,
				sizeof(p->gb), p->gb,
				sizeof(p->open), p->open,
				sizeof(p->high), p->high,
				sizeof(p->low), p->low,
				sizeof(p->close), p->close,
				sizeof(p->dotcnt), p->dotcnt,
				sizeof(p->sma_short), p->sma_short,
				sizeof(p->sma_long), p->sma_long,
				sizeof(p->seq), p->seq
			);
			if (FALSE == db->Cmd(zQ) || FALSE == db->Exec())
			{
				g_log.log(LOGTP_ERR, "CHART DATA Save 에러(%s)(%s)", db->GetLastError(), zQ);
				printf( "CHART DATA Save 에러(%s)(%s)\n", db->GetLastError(), zQ);
				continue;
			}
			//printf("DB SAVE(%s)\n", zQ);
			
		} //while (m_shm.GetNextData((char*)&stShm))
		//printf("DB SAVE(%s)(%s)\n", zGroupKey);
	} // while (m_shm.GetNextGroup(zGroupKey))

	delete p;
}


VOID	CSaveChart::DBSaveWrapper()
{
	__try
	{
		DBSave();
	}
	__except (ReportException(GetExceptionCode(), "CSaveChart::DBSaveWrapper", m_zMsg))
	{
		g_log.log(LOGTP_FATAL, m_zMsg);
	}
}

BOOL	CSaveChart::LoadShmWrapper()
{
	__try
	{
		return LoadShm();
	}
	__except (ReportException(GetExceptionCode(), "CSaveChart::LoadShmWrapper", m_zMsg))
	{
		g_log.log(LOGTP_FATAL, m_zMsg);
	}
	return FALSE;
}


/*
SELECT [SYS_DT]
,[SYMBOL]
,[CHART_TP]
,[GROUP_KEY]
,[CHART_NM]
,[PREV_NM]
,[CHART_GB]
,[OPEN_PRC]
,[HIGH_PRC]
,[LOW_PRC]
,[CLOSE_PRC]
,[DOT_CNT]
,[SMA_SHORT]
,[SMA_LONG]
FROM [KRHEDGE].[dbo].[FB_CHART_DATA]
GO




*/
BOOL	CSaveChart::LoadShm()
{
	CDBHandler db(m_pDBPool->Get());
	char zQ[1024];
	std::list<CString> listGroupKey;

	sprintf(zQ, "SELECT DISTINCT GROUP_KEY FROM FB_CHART_DATA WHERE CHART_TP=%d", m_nChartTp);
	if (FALSE == db->Cmd(zQ) || FALSE == db->Exec())
	{
		g_log.log(LOGTP_ERR, "CHART DATA Load-1 에러(%s)(%s)", db->GetLastError(), zQ);
		return FALSE;
	}
	while (db->NextRow())
	{
		CString sGroup;
		sGroup = db->Get("GROUP_KEY");
		listGroupKey.push_back(sGroup);
	}

	db->Close();

	std::list<CString>::iterator it;
	for (it = listGroupKey.begin(); it != listGroupKey.end(); it++)
	{
		CString sKey = *it;

		sprintf(zQ,
			"SELECT TOP 20 GROUP_KEY, CHART_NM, PREV_NM, CHART_GB, "
			"OPEN_PRC, HIGH_PRC, LOW_PRC, CLOSE_PRC, "
			"DOT_CNT, SMA_SHORT, SMA_LONG "
			"FROM FB_CHART_DATA WHERE GROUP_KEY='%s' ORDER BY SYS_DT DESC , CHART_NM DESC",
			sKey.GetBuffer()
		);

		if (FALSE == db->Cmd(zQ) || FALSE == db->Exec())
		{
			g_log.log(LOGTP_ERR, "CHART DATA Load 에러(%s)(%s)", db->GetLastError(), zQ);
			printf("CHART DATA Load 에러(%s)(%s)\n", db->GetLastError(), zQ);
			return FALSE;
		}

		int nLoop = 0;
		CHAR zGroupKey[LEN_GROUP_KEY + 1];
		char zChartNm[LEN_CHART_NM + 1], zPrevNm[LEN_CHART_NM + 1];
		ST_SHM_CHART_UNIT stShm;
		ST_SHM_LASTCHART	lastNm;

		while (db->NextRow())
		{
			sprintf(zGroupKey, "%.*s", LEN_GROUP_KEY, db->Get("GROUP_KEY"));
			sprintf(zChartNm, "%.*s", LEN_CHART_NM, db->Get("CHART_NM"));
			sprintf(zPrevNm, "%.*s", LEN_CHART_NM, db->Get("PREV_NM"));

			ZeroMemory(&stShm, sizeof(stShm));

			sprintf(stShm.Nm, "%.*s", LEN_CHART_NM, zChartNm);
			sprintf(stShm.prevNm, "%.*s", LEN_CHART_NM, zPrevNm);
			sprintf(stShm.gb, "%.1s", db->Get("CHART_GB"));
			sprintf(stShm.open, "%*.5f", LEN_PRC, db->GetDouble("OPEN_PRC"));
			sprintf(stShm.high, "%*.5f", LEN_PRC, db->GetDouble("HIGH_PRC"));
			sprintf(stShm.low, "%*.5f", LEN_PRC, db->GetDouble("LOW_PRC"));
			sprintf(stShm.close, "%*.5f", LEN_PRC, db->GetDouble("CLOSE_PRC"));
			sprintf(stShm.sma_short, "%*.5f", LEN_PRC, db->GetDouble("SMA_SHORT"));
			sprintf(stShm.sma_short, "%*.5f", LEN_PRC, db->GetDouble("SMA_LONG"));
			sprintf(stShm.seq, "%.*s", sizeof(stShm.seq), db->Get("PACK_TM"));

			//g_log.log(LOGTP_SUCC, "[DBREAD][GROUP_KEY:%s][CHART_NM:%s][PREV_NM:%s]", zGroupKey, zChartNm, zPrevNm);
			//printf("[DBREAD][GROUP_KEY:%s][CHART_NM:%s][PREV_NM:%s]", zGroupKey, zChartNm, zPrevNm);

			memset(&lastNm, 0x20, sizeof(lastNm));
			memcpy(lastNm.GroupKey, zGroupKey, LEN_SHM_GROUP_KEY);
			memcpy(lastNm.LastChartNm, zChartNm, LEN_CHART_NM);

			// DESC 이므로 첫번째를 LAST SHM 으로 한다.
			if(nLoop==0)
			{ 
				m_lastShm.InsertGroup(zGroupKey);
				m_lastShm.InsertData(zGroupKey, (char*)&lastNm);
				g_log.log(LOGTP_SUCC, "[LAST SHM SAVE][%s][%.*s]", zGroupKey, LEN_CHART_NM, lastNm.LastChartNm);
				printf("[LAST SHM SAVE][%s][%.*s]\n", zGroupKey, LEN_CHART_NM, lastNm.LastChartNm);
			}
			
			// GROUP KEY 
			if (!m_shm.FindGroup(zGroupKey))
			{
				m_shm.InsertGroup(zGroupKey);
				g_log.log(LOGTP_SUCC, "[GROUP INSERT][%s]", zGroupKey);
				m_shm.InsertData(zGroupKey, (char*)&stShm);
				g_log.log(LOGTP_SUCC, "[DATA INSERT][%s][%.*s][%.*s]", zGroupKey, LEN_CHART_NM, stShm.Nm, LEN_CHART_NM, stShm.prevNm);

				printf("[DATA INSERT][%s][%.*s][%.*s]\n", zGroupKey, LEN_CHART_NM, stShm.Nm, LEN_CHART_NM, stShm.prevNm);

				//m_lastShm.InsertGroup(zGroupKey);
				//m_lastShm.InsertData(zGroupKey, (char*)&lastNm);
			}
			else
			{
				if (m_shm.FindData(zGroupKey, zChartNm)<1)
				{
					m_shm.InsertData(zGroupKey, (char*)&stShm);
					g_log.log(LOGTP_SUCC, "[DATA INSERT][%s][%.*s][%.*s]", zGroupKey, LEN_CHART_NM, stShm.Nm, LEN_CHART_NM, stShm.prevNm);
					printf( "[DATA INSERT][%s][%.*s][%.*s]\n", zGroupKey, LEN_CHART_NM, stShm.Nm, LEN_CHART_NM, stShm.prevNm);
				}

				//m_lastShm.SetData(zGroupKey, zGroupKey, (char*)&lastNm);
			}

			nLoop++;

		} // while (db->NextRow())

		db->Close();

		

	} // for (it = listGroupKey.begin(); it != listGroupKey.end(); it++)

	return TRUE;
}