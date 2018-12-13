#include "TradeOption.h"
#include "../../IRUM_UTIL/Util.h"
#include "../../IRUM_UTIL/LogMsg.h"


extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];

COptBase::COptBase(char* pzConfig)
{
	strcpy(m_zConfig, pzConfig);
	m_bOn = FALSE;
	ZeroMemory(m_zMsg, sizeof(m_zMsg));
	m_pDBPool = NULL;
}
COptBase::~COptBase()
{
	Finalize();
}

void COptBase::Finalize()
{
	SAFE_DELETE(m_pDBPool);
}

BOOL COptBase::ConnectDB()
{
	char ip[32], id[32], pwd[32], cnt[32], name[32];
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_NAME", name);
	CUtil::GetConfig(m_zConfig, "DBINFO", "DB_POOL_CNT", cnt);

	if (!m_pDBPool)
	{
		m_pDBPool = new CDBPoolAdo(ip, id, pwd, name);
		if (!m_pDBPool->Init(atoi(cnt)))
		{
			sprintf(m_zMsg, "DB OPEN FAILED.(%s)(%s)(%s)(%s)", ip, id, pwd, m_pDBPool->GetMsg());
			return FALSE;
		}
	}
	return TRUE;
}



CCrossOpt::CCrossOpt(char* pzConfig):COptBase(pzConfig)
{

}

CCrossOpt::~CCrossOpt()
{}

BOOL CCrossOpt::TurnOn(void* p1)
{
	char zCrossApplied[32];
	CUtil::GetConfig(m_zConfig, "TRADE_OPTION", "CROSS_APPLIED", zCrossApplied);
	if (zCrossApplied[0] != 'Y')
	{
		m_bOn = FALSE;
		return TRUE;
	}

	char zCrossAppliedMode[32], zCandleMin[32], zStrongYN[32];
	CUtil::GetConfig(m_zConfig, "TRADE_OPTION", "CROSS_APPLIED_MODE", zCrossAppliedMode);
	if (strcmp(zCrossAppliedMode, "ALL_SYMBOL") == 0)
	{
		m_bOn = TRUE;
		CUtil::GetConfig(m_zConfig, "TRADE_OPTION", "CROSS_CANDLE_MIN", zCandleMin);
		m_nCandleMin = atoi(zCandleMin);

		CUtil::GetConfig(m_zConfig, "TRADE_OPTION", "CROSS_SMA_MIN", zCandleMin);
		m_nSMA_Min = atoi(zCandleMin);

		CUtil::GetConfig(m_zConfig, "TRADE_OPTION", "CROSS_STRONG_YN", zStrongYN);
		m_bStrongCross = (zStrongYN[0] == 'Y') ? TRUE : FALSE;

		g_log.log(INFO, "TRADE OPTION.(Candle Min:%d) (SMA Min:%d) (StrongYN:%c)", m_nCandleMin, m_nSMA_Min, zStrongYN[0]);
	}

	if (strcmp(zCrossAppliedMode, "BY_SYMBOL") == 0)
	{
		if (!ConnectDB())
			return FALSE;

		CDBHandlerAdo db(m_pDBPool->Get());
		char zQ[1024];
		sprintf(zQ, "SELECT * FROM ABOT_NO1_STRAT_CONFIG WHERE SYMBOL = '%s'", (char*)p1);
		if (FALSE == db->ExecQuery(zQ))
		{
			sprintf(m_zMsg, "failed in querying options(%s)", zQ);
			return FALSE;
		}
		db->GetStr("CROSS_APPLIED", zCrossApplied);
		m_nCandleMin = db->GetLong("CROSS_CANDLE_MIN");
		db->Close();

		m_bOn = (zCrossApplied[0] == 'Y') ? TRUE : FALSE;
	}

	return TRUE;
}

//
//
//CProfitSecured::CProfitSecured(char* pzConfig) :COptBase(pzConfig)
//{
//
//}
//
//CProfitSecured::~CProfitSecured()
//{}
//
//BOOL CProfitSecured::TurnOn(void* p1)
//{
//	char zPSecuredApplied[32];
//	CUtil::GetConfig(m_zConfig, "TRADE_OPTION", "PROFIT_SECURED_APPLIED", zPSecuredApplied);
//	if (zPSecuredApplied[0] != 'Y')
//	{
//		m_bOn = FALSE;
//		return TRUE;
//	}
//
//	CUtil::GetConfig(m_zConfig, "TRADE_OPTION", "PROFIT_SECURED_AMT", m_zSecuredAmt);
//
//	if (!ConnectDB())
//		return FALSE;
//
//	CDBHandlerAdo db(m_pDBPool->Get());
//	char zQ[1024];
//	sprintf(zQ, "EXEC ABOT_NO1_PROFITSECURED_TICK_PRC '%s', %f", (char*)p1, atof(m_zSecuredAmt));
//	if (FALSE == db->ExecQuery(zQ))
//	{
//		sprintf(m_zMsg, "failed in querying options(%s)", zQ);
//		return FALSE;
//	}
//	m_dSecuredPrcGap = db->GetDbl("PRC_GAP");
//
//	db->Close();
//
//	m_bOn = (m_dSecuredPrcGap > 0) ? TRUE : FALSE;
//
//	return TRUE;
//}


CTradeOption::CTradeOption(char* pzConfig)
{
	m_Cross = new CCrossOpt(pzConfig);
	//m_PSecured = new CProfitSecured(pzConfig);
}


CTradeOption::~CTradeOption()
{
	SAFE_DELETE(m_Cross);
	//SAFE_DELETE(m_PSecured);
}


BOOL CTradeOption::TurnOn_Cross(char* pzSymbol)
{
	BOOL bRet = m_Cross->TurnOn(pzSymbol);
	if (!bRet)
		strcpy(m_zMsg, m_Cross->getmsg());

	return bRet;
}


//
//BOOL CTradeOption::TurnOn_PSecured(char* pzSymbol)
//{
//	BOOL bRet = m_PSecured->TurnOn(pzSymbol);
//	if (!bRet)
//		strcpy(m_zMsg, m_PSecured->getmsg());
//
//	return bRet;
//}
