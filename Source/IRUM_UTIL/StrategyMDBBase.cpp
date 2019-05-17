#include "StrategyMDBBase.h"
#include "Util.h"

CStrategyMDBBase::CStrategyMDBBase(char *pzSymbol)
{
	strcpy(m_zSymbol, pzSymbol);
	//GET_SHM_NM(m_zMDBName, m_zShmName);
	//GET_SHM_LOCK_NM(m_zMDBName, m_zMutexName);
	//m_shm = NULL;
	m_pDBPool = NULL;
	m_bInit = FALSE;
}


CStrategyMDBBase::~CStrategyMDBBase()
{
	Finalize();
}

VOID CStrategyMDBBase::Finalize()
{
	//SAFE_DELETE(m_shm);
	SAFE_DELETE(m_pDBPool);
}

//BOOL CStrategyMDBBase::OpenMDB()
//{
//	return TRUE;
//	//return m_shm->Open(m_zShmName, m_zMutexName);
//}

BOOL CStrategyMDBBase::DBOpen()
{
	char ip[32], id[32], pwd[32], dbname[32], cnt[32];
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_NAME", dbname);
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_POOL_CNT", cnt);

	m_pDBPool = new CDBPoolAdo(ip, id, pwd, dbname);
	if (!m_pDBPool->Init(atoi(cnt)))
	{
		sprintf(m_zMsg, "DB OPEN FAIL(IP:%s)(ID:%s)(PWD:%s)(DB:%s)(MSG:%s)", ip, id, pwd, dbname, m_pDBPool->GetMsg());
		SAFE_DELETE(m_pDBPool);
		return FALSE;
	}
	return TRUE;
}

BOOL CStrategyMDBBase::Initialize(char* pConfig)
{
	strcpy(m_zConfig, pConfig);

	if (!CreateMDB())
		return FALSE;

	if (!DBOpen())
		return FALSE;

	if (!LoadSymbolAndStrategy())
		return FALSE;

	m_bInit = TRUE;
	return TRUE;
}
