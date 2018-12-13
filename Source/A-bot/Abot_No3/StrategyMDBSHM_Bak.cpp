#include "StrategyMDB.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include <stdio.h>

extern CLogMsg g_log;

CStrategyMDB::CStrategyMDB(char *pzMDBName):CStrategyMDB(pzMDBName)
{
	m_bAlreadRead = FALSE;
}


CStrategyMDB::~CStrategyMDB()
{
}


BOOL CStrategyMDB::CreateMDB()
{
	INIT_DATA	shmData;
	shmData.lMaxGroupCnt = MAX_SYMBOL_CNT;	// SYMBOL ¼ö 
	shmData.lGroupKeySize = LEN_SYMBOL;
	shmData.lHeaderSize = 0;
	shmData.lStructSize = sizeof(ST_MDB);
	shmData.lStructKeySize = LEN_SYMBOL;
	shmData.lMaxStructCnt = 1;

	//	CREATE SHM
	m_shm = new CSharedMem;
	int nRet = m_shm->Create(NULL, &shmData, m_zShmName, m_zMutexName);
	if (nRet < 1)
	{
		sprintf(m_zMsg, "[MDB CREATE ERROR](%s)(%s)(%s)", m_zShmName, m_zMutexName, m_shm->GetErr());
		g_log.log(NOTIFY, "%s", m_zMsg);
		return FALSE;
	}
	if (!m_shm->SetInitSize(shmData.lMaxGroupCnt
		, shmData.lGroupKeySize
		, shmData.lHeaderSize
		, shmData.lStructSize
		, shmData.lStructKeySize
		, shmData.lMaxStructCnt)
		)
	{
		sprintf(m_zMsg, "[SHM SetInitSize ERROR](%s)(%s)", m_zShmName, m_zMutexName);
		g_log.log(NOTIFY, "%s", m_zMsg);
		return FALSE;
	}


	g_log.log(INFO, "Succeed to create MDB");
	m_bAlreadRead = FALSE;
	return TRUE;
}

BOOL CStrategyMDB::Read(_In_ char* pzSymbol)
{
	char zSymbol[32];
	FMT_SYMBOL(pzSymbol, zSymbol);
	if (!m_shm->GetData(zSymbol, zSymbol, (char *)&m_stCurrMdb)) {
		m_bAlreadRead = FALSE;
		return FALSE;
	}
	m_bAlreadRead = TRUE;
	m_nDotCnt = S2N(m_stCurrMdb.symbol.DotCnt, sizeof(m_stCurrMdb.symbol.DotCnt));
	return TRUE;
}

BOOL CStrategyMDB::IsFinished()
{
	return (m_stCurrMdb.tx.FinishedYN[0] == 'Y') ? TRUE : FALSE;
}



EN_MKT_STATUS CStrategyMDB::MktStatus()
{
	char zStatus[2];
	sprintf(zStatus, "%c", m_stCurrMdb.tx.MktStatus[0]);
	return (EN_MKT_STATUS)atoi(zStatus);
}

BOOL CStrategyMDB::OrderAlreadySent()
{
	int n = S2N(m_stCurrMdb.tx.OrdSentQty, sizeof(m_stCurrMdb.tx.OrdSentQty));
	return (n > 0);
}


BOOL CStrategyMDB::IsOpenSrategyExist()
{
	return (m_stCurrMdb.tx.PosStatus[0] == DEF_POS_STATUS_OPEN);
}



BOOL CStrategyMDB::Save(_In_ char* pzSymbol, _In_ ST_MDB* pMDB)
{
	char zSymbol[32];
	FMT_SYMBOL(pzSymbol, zSymbol);
	m_bAlreadRead = FALSE;
	if (!m_shm->UpdateData(zSymbol, zSymbol, (char *)pMDB))
		return FALSE;
	if (!m_shm->GetData(zSymbol, zSymbol, (char *)&m_stCurrMdb))
		return FALSE;
	
	m_nDotCnt = S2N(m_stCurrMdb.symbol.DotCnt, sizeof(m_stCurrMdb.symbol.DotCnt));
	m_bAlreadRead = TRUE;
	return TRUE;
}

BOOL CStrategyMDB::AddSentOrdQty()
{
	int nCurrQty = S2N(m_stCurrMdb.tx.OrdSentQty, sizeof(m_stCurrMdb.tx.OrdSentQty));
	int nAddQty = S2N(m_stCurrMdb.param.OrdQty, sizeof(m_stCurrMdb.param.OrdQty));
	sprintf(t, "%0*d", sizeof(m_stCurrMdb.tx.OrdSentQty), nCurrQty + nAddQty);
	memcpy(m_stCurrMdb.tx.OrdSentQty, t, sizeof(m_stCurrMdb.tx.OrdSentQty));
	return Save(m_stCurrMdb.Symbol, &m_stCurrMdb);
}

BOOL CStrategyMDB::SaveMktStatus(EN_MKT_STATUS status)
{
	char zStatus[2];
	sprintf(zStatus, "%d", (int)status);
	m_stCurrMdb.tx.MktStatus[0] = zStatus[0];
	return Save(m_stCurrMdb.Symbol, &m_stCurrMdb);
}

BOOL CStrategyMDB::ReleaseSentOrdQty(int nReleaseQty)
{
	int nCurrQty = S2N(m_stCurrMdb.tx.OrdSentQty, sizeof(m_stCurrMdb.tx.OrdSentQty));
	nCurrQty -= nReleaseQty;
	if (nCurrQty < 0)
		nCurrQty = 0;

	sprintf(t, "%0*d", sizeof(m_stCurrMdb.tx.OrdSentQty), nCurrQty);
	memcpy(m_stCurrMdb.tx.OrdSentQty, t, sizeof(m_stCurrMdb.tx.OrdSentQty));
	return Save(m_stCurrMdb.Symbol, &m_stCurrMdb);
	return TRUE;
}

char* CStrategyMDB::PosPrc()
{
	std::string prc = std::string(m_stCurrMdb.tx.PosPrc, LEN_PRC);
	return (char*)prc.c_str();
}


char* CStrategyMDB::StartTM()
{
	std::string tm = std::string(m_stCurrMdb.param.StartTM, sizeof(m_stCurrMdb.param.StartTM));
	return (char*)tm.c_str();
}

char* CStrategyMDB::EndTM()
{
	std::string tm = std::string(m_stCurrMdb.param.EndTM, sizeof(m_stCurrMdb.param.EndTM));
	return (char*)tm.c_str();
}



BOOL CStrategyMDB::LoadSymbolAndStrategy()
{
	CDBHandlerAdo db(m_pDBPool->Get());
	char zQ[1024];
	sprintf(zQ, "EXEC MF_LOAD_SYBOL ");
	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(NOTIFY, "Load Symbol Error(%s)", zQ);
		return FALSE;
	}

	char t[128],  zSymbolKey[32];
	ST_MDB mdb;
	while (db->IsNextRow())
	{
		char zArtc[128];	db->GetStr("ARTC_CD", zArtc);
		char zSymbol[128];	db->GetStr("SYMBOL", zSymbol);
		char zName[128];	db->GetStr("NAME", zName);
		double dTickVal = db->GetDouble("TICK_VALUE");
		double dTickSize = db->GetDouble("TICK_SIZE");
		int nDotCnt = db->GetLong("DOT_CNT");

		FMT_SYMBOL(zSymbol, zSymbolKey);
		if (!m_shm->FindGroup(zSymbolKey))
		{
			if (!m_shm->InsertGroup(zSymbolKey))
			{
				g_log.log(NOTIFY, "MDB GroupKey Add ERROR!!!(%s)", t);
				return FALSE;
			}

			FillMemory(&mdb, sizeof(mdb), '0');
			memcpy(mdb.Symbol, zSymbolKey, sizeof(mdb.symbol));
			memcpy(mdb.symbol.Symbol, zSymbolKey, sizeof(mdb.symbol.Symbol));

			FMT_ARTC(zArtc, t);
			memcpy(mdb.symbol.ArtcCd, t, sizeof(mdb.symbol.ArtcCd));
		
			FMT_SYMBOL_NM(zName, t);
			memcpy(mdb.symbol.SymbolName, t, sizeof(mdb.symbol.SymbolName));

			FMT_TICKSIZE(dTickSize, nDotCnt, t);
			memcpy(mdb.symbol.TickSize, t, sizeof(mdb.symbol.TickSize));
			
			FMT_TICKVALUE(dTickVal, t);
			memcpy(mdb.symbol.TickValue, t, sizeof(mdb.symbol.TickValue));

			FMT_DOTCNT( nDotCnt, t);
			memcpy(mdb.symbol.DotCnt, t, sizeof(mdb.symbol.DotCnt));

			int nMaxCntSL = db->GetLong("MAXCNT_SL");
			sprintf(t, "%0*d", sizeof(mdb.param.MaxCntSL), nMaxCntSL);
			memcpy(mdb.param.MaxCntSL, t, sizeof(mdb.param.MaxCntSL));

			int nMaxCntPT = db->GetLong("MAXCNT_PT");
			sprintf(t, "%0*d", sizeof(mdb.param.MaxCntPT), nMaxCntPT);
			memcpy(mdb.param.MaxCntPT, t, sizeof(mdb.param.MaxCntPT));

			char zEndTM[128];	db->GetStr("END_TM", zEndTM);
			memcpy(mdb.param.EndTM, zEndTM, sizeof(mdb.param.EndTM));

			char zStartTM[128];	db->GetStr("START_TM", zStartTM);
			memcpy(mdb.param.StartTM, zEndTM, sizeof(mdb.param.StartTM));
				
			int nOrdQty = db->GetLong("ORD_QTY");
			sprintf(t, "%0*d", sizeof(mdb.param.OrdQty), nOrdQty);
			memcpy(mdb.param.OrdQty, t, sizeof(mdb.param.OrdQty));
			
			if (!m_shm->InsertData(zSymbolKey, (char*)&mdb))
			{
				g_log.log(NOTIFY, "Insert MDB data Error(%s)", zSymbolKey);
				return FALSE;
			}
			printf("Insert MDB(%s)\n", zSymbolKey);

		} // if (!m_shm->FindGroup(zSymbolKey))

		db->Next();

	} // while (db->IsNextRow())

	db->Close();
}