
#include "DealManager.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/Util.h"
#include "../common/FBIInc.h"


extern BOOL		g_bContinue;	// flag whether continue process or not
extern CLogMsg	g_log;
extern char		g_zConfig[_MAX_PATH];
extern CMemPool	g_memPool;


CDealManager::CDealManager(char* pzArtcCd) : CBaseThread(pzArtcCd)
{
	strcpy(m_zArtcCd, pzArtcCd);
}


CDealManager::~CDealManager()
{
}



BOOL CDealManager::Initialize()
{
	InitializeCriticalSection(&m_csDeal);

	char ip[32], id[32], pwd[32], name[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", name);

	if (!m_pDBPool)
	{
		m_pDBPool = new CDBPoolAdo(ip, id, pwd, name);
		if (!m_pDBPool->Init(2))
		{
			g_log.log( ERR/*NOTIFY*/, "(%s)Thread DB OPEN FAILED.(%s)(%s)(%s)", m_zArtcCd, ip, id, pwd);
			return FALSE;
		}
	}

	if (!InitClientConnect())
		return FALSE;

	ResumeThread();
	//m_hUpdateDeal = (HANDLE)_beginthreadex(NULL, 0, &Thread_UpdateDeal, this, 0, &m_unUpdateDeal);
	return TRUE;
}


VOID CDealManager::Finalize()
{
	SAFE_DELETE(m_pDBPool);

	std::map<std::string, _FBI::ST_DEAL_INFO*>::iterator it;
	EnterCriticalSection(&m_csDeal);
	for (it = m_mapDeal.begin(); it != m_mapDeal.end(); ++it)
	{
		_FBI::ST_DEAL_INFO* s = (*it).second;
		delete s;
	}
	m_mapDeal.clear();
	DeleteCriticalSection(&m_csDeal);
}

BOOL CDealManager::LoadDealInfo()
{
	char zQ[1024];
	char zTime[128], zNow[32];
	SYSTEMTIME st;

	CDBHandlerAdo db(m_pDBPool->Get());
	
	sprintf(zQ, "SELECT DEAL_SEQ, ARTC_CD, TM_ORD_START, TM_WAIT_START, TM_RESULT_START, DEAL_STATUS, 3 AS DURATION "
				" FROM AA_DEAL_MST WHERE ARTC_CD='%s'", m_zArtcCd);
	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(ERR/*NOTIFY*/, "Load DealInfo Error(%s)", zQ);
		return FALSE;
	}
	while (db->IsNextRow())
	{
		// hh:mm:ss
		db->GetStr("TM_ORD_START", zTime);
											
		//// DURATION *2 보다 이전 데이터는 굳이 Loading 할 필요 없다.
		//int nPassedSecs = CUtil::GetPassedSeconds(zTime, TRUE/*bColon*/);
		//if (nPassedSecs / 60 > 2)
		//{
		//	db->Next();
		//	continue;
		//}

		// 주문시작시간이 이미 지난건 버린다.
		GetLocalTime(&st); sprintf(zNow, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
		if (strncmp(zTime, zNow, strlen(zTime)) <= 0)
		{
			db->Next();
			continue;
		}

		_FBI::ST_DEAL_INFO* pInfo = new _FBI::ST_DEAL_INFO;
		ZeroMemory(pInfo, sizeof(_FBI::ST_DEAL_INFO));
		pInfo->DealSeq = db->GetLong("DEAL_SEQ");
		db->GetStr("TM_ORD_START", pInfo->tm_order);		// hh:mm:ss
		db->GetStr("ARTC_CD", pInfo->Symbol);
		strcpy(pInfo->tm_order, zTime);
		db->GetStr("TM_WAIT_START", pInfo->tm_wait);
		db->GetStr("TM_RESULT_START", pInfo->tm_result);
		pInfo->DealStatus = db->GetLong("DEAL_STATUS");
		pInfo->DurationMin = db->GetLong("DURATION");
		
		std::string sOrdTM = std::string(zTime);

		//g_log.log(INFO, "_[%s]_(%s)(%s)(%s)", sOrdTM.c_str(), pInfo->tm_order, pInfo->tm_wait, pInfo->tm_result);
		EnterCriticalSection(&m_csDeal);
		m_mapDeal[sOrdTM] = pInfo;
		LeaveCriticalSection(&m_csDeal);

		db->Next();
	}
	db->Close();
	g_log.log(INFO, "Succeeded in DealInfo[%s]", m_zArtcCd);
	return TRUE;
}

VOID CDealManager::ThreadFunc()
{
	if (!LoadDealInfo())
		return;

	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, _FBI::TIMEOUT_CHECK_DEAL, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) {
			g_log.log(LOGTP_ERR, "[THREAD ID:%d]WAIT ERROR(%d)", GetMyThreadID(), GetLastError());
			Sleep(1000);
			continue;
		}
		else if (dwRet == WAIT_TIMEOUT)
		{
			DealManage();
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case _FBI::WM_RECV_API_CHART:
				ResultProcByChart((char*)msg.lParam);
				g_memPool.release((char*)msg.lParam);
				break;
			case _FBI::WM_TERMINATE:
				//TODO
				return;
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)
}


/*
	chart 데이터를 이용한 체결처리


	[AA_ORD_RESULT]
	@I_STK_CD	VARCHAR(10)
	,@I_CHART_TP	INT
	,@I_OPEN_PRC_S	VARCHAR(20)
	,@I_CLOSE_PRC_S	VARCHAR(20)
*/
VOID CDealManager::ResultProcByChart(char* pChartData)
{
	_FBI::PT_API_CHART* p = (_FBI::PT_API_CHART*)pChartData;
	int nChartTp = 0;
	char zChartNm[32];

	//DB UPDATE
	for (int i = 0; i < 3; i++)
	{
		CDBHandlerAdo db(m_pDBPool->Get());
		char zQ[1024];

		sprintf(zQ, "EXEC AA_ORD_RESULT "
					"'%.*s'"
					",%d"
					",'%.*s'"
					",'%.*s'"
			, sizeof(p->Symbol), p->Symbol
			, nChartTp
			, sizeof(p->Open), p->Open
			, sizeof(p->Close), p->Close
		);

		if (FALSE == db->ExecQuery(zQ))
		{
			g_log.log(ERR/*NOTIFY*/, "주문결과처리(AA_ORD_RESULT) error(%s)", zQ);
			Sleep(1000);
			db->Close();
			continue;
		}
		else
		{
			g_log.log(INFO, "[%s][CHART_NM:%s][OPEN:%.10s][CLOSE:%.10s]주문처리 성공",
				m_zArtcCd, zChartNm, p->Open, p->Close);
			break;
		}
	}
}


VOID CDealManager::DealManage()
{
	EnterCriticalSection(&m_csDeal);
	DealManageInner();
	LeaveCriticalSection(&m_csDeal);
}

/*
	현재시간과 MAP 의 가장처음 DEAL STATUS 를 비교해서 
	UPDATE DB
	 CLIENT

	DEAL_STATUS	0	미시작
	DEAL_STATUS	1	주문가능
	DEAL_STATUS	2	대기
	DEAL_STATUS	3	결과진행
	DEAL_STATUS	4	완료

*/
VOID CDealManager::DealManageInner()
{
	char zNow[32] = { 0, }; 
	SYSTEMTIME st;
	int nLen = strlen(zNow);

	// 제일 처음 DEAL 정보를 가져온다.
	std::string sOrdTm, sRsltTm;
	_FBI::ST_DEAL_INFO* pInfo;

	std::map<std::string, _FBI::ST_DEAL_INFO*>::iterator it;
	for (it = m_mapDeal.begin(); it != m_mapDeal.end(); it++)
	{
		sOrdTm.clear();
		pInfo = NULL;
		sOrdTm = (*it).first;		
		pInfo = (*it).second;

		GetLocalTime(&st);
		sprintf(zNow, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
		nLen = strlen(zNow);

		// HH:MM 까지만 비교를 한다.
		if (strncmp(pInfo->tm_order, zNow, 5) <= 0 &&
			strncmp(zNow, pInfo->tm_result, 5) <= 0
			)
		{
			break;
		}
	}
	char status[32];

	// 이미 처리가 된 DEAL 이면 MAP 에서 제거
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_DONE)
	{
		g_log.log(INFO, "[DEAL_MANAGE(%s)(현재:%s)(DEAL주문시간:%s)(상태:%s)]이미 완료된 DEAL 이라서 MAP 에서 제거",
			pInfo->Symbol, zNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status));
		delete pInfo;
		m_mapDeal.erase(it);
		return;
	}

	// 종료시간과 비교. 종료시간보다 현재가 더 늦다.
	if (strncmp(zNow, pInfo->tm_result, nLen) >= 0)
	{
		return DealResult(zNow, sOrdTm, pInfo);
	} // if (strncmp(zNow, pInfo->tm_result, nLen) >= 0)

	// 대기시간과 비교. 대기시간보다 현재가 더 늦다.
	if (strncmp(zNow, pInfo->tm_wait, nLen) >= 0)
	{
		return DealWait(zNow, sOrdTm, pInfo);
	}

	// 주문시간과 비교. 주문시간보다 현재가 더 늦다.
	if (strncmp(zNow, pInfo->tm_order, nLen) >= 0)
	{
		return DealOrd(zNow, sOrdTm, pInfo);
	}
}


VOID CDealManager::DealOrd(char* pzNow, std::string sOrdTm, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_ORD)
	{
		return;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_NONSTART)
	{
		g_log.log(INFO, "[DEAL_MANAGE(%s)(현재:%s)(DEAL주문시간:%s)(상태:%s)]주문시간(%s)지나고 현재 <미시작>이므로 <주문>으로 UPDATE",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status), pInfo->tm_order);

		// update.
		pInfo->DealStatus = _FBI::DEAL_STATUS_ORD;
		UpdateDeal(pInfo);
		m_mapDeal[sOrdTm] = pInfo;

		//char* pData = g_memPool.get();
		//memcpy(pData, pInfo, sizeof(_FBI::ST_DEAL_INFO));
		//PostThreadMessage(m_unUpdateDeal, _FBI::WM_DEAL_STATUS, 0, (LPARAM)pData);
	}
	else
	{
		//상태가 이상하다. 에러 처리.
		g_log.log(ERR/*NOTIFY*/, "[DEAL_MANAGE(%s)(현재:%s)(DEAL주문시간:%s)(상태:%s)]주문시간 지났는데 상태가 <미시작>이 아니다. ",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status));
		delete pInfo;
		m_mapDeal.erase(sOrdTm);
	}
}



VOID CDealManager::DealWait(char* pzNow, std::string sOrdTm, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_WAIT)
	{
		return;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_ORD)
	{
		g_log.log(INFO, "[DEAL_MANAGE(%s)(현재:%s)(DEAL주문시간:%s)(상태:%s)]대기시간(%s)지나고 현재 <주문>이므로 <대기>로 UPDATE",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status), pInfo->tm_wait);

		// update.
		pInfo->DealStatus = _FBI::DEAL_STATUS_WAIT;
		UpdateDeal(pInfo);
		m_mapDeal[sOrdTm] = pInfo;

		//char* pData = g_memPool.get();
		//memcpy(pData, pInfo, sizeof(_FBI::ST_DEAL_INFO));
		//PostThreadMessage(m_unUpdateDeal, _FBI::WM_DEAL_STATUS, 0, (LPARAM)pData);

	}
	else
	{
		//상태가 이상하다. 에러 처리.
		g_log.log(ERR/*NOTIFY*/, "[DEAL_MANAGE(%s)(현재:%s)(DEAL주문시간:%s)(상태:%s)]대기시간 지났는데 상태가 <주문>이 아니다. ",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status));
		delete pInfo;
		m_mapDeal.erase(sOrdTm);
	}
}


VOID CDealManager::DealResult(char* pzNow, std::string sOrdTm, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_RESULT)
	{
		return;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_WAIT)
	{
		// update.
		g_log.log(INFO, "[DEAL_MANAGE(%s)(현재:%s)(DEAL주문시간:%s)(상태:%s)]결과시간(%s)지나고 현재 <대기>이므로 <결과>로 UPDATE",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status), pInfo->tm_result);

		pInfo->DealStatus = _FBI::DEAL_STATUS_RESULT;
		UpdateDeal(pInfo);
		m_mapDeal[sOrdTm] = pInfo;

		//char* pData = g_memPool.get();
		//memcpy(pData, pInfo, sizeof(_FBI::ST_DEAL_INFO));
		//PostThreadMessage(m_unUpdateDeal, _FBI::WM_DEAL_STATUS, 0, (LPARAM)pData);
	}
	else
	{
		//상태가 이상하다. 에러 처리.
		g_log.log(ERR/*NOTIFY*/, "[DEAL_MANAGE(%s)(현재:%s)(DEAL주문시간:%s)(상태:%s)]결과시간 지났는데 상태가 <대기>가 아니다. ",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status));
		delete pInfo;
		m_mapDeal.erase(sOrdTm);
	}
}




/*
struct ST_DEAL_INFO
{
char	Symbol[LEN_SYMBOL];
int		DealSeq;
char	SysDt[8];
char	tm_order[8];	//hh:mm:ss
char	tm_wait[8];		//hh:mm:ss
char	tm_result[8];	//hh:mm:ss
int		DealStatus;
int		DurationMin;
};

struct ST_DEAL_STATUS
{
char Symbol[LEN_SYMBOL];
char DealSeq[LEN_DEAL_SEQ];
char DealStatus[2];
char Time[8];				// 각 status 시작시간
};
*/
void CDealManager::UpdateDeal(_FBI::ST_DEAL_INFO* pInfo)
{
	// send to the client
	char z[128];
	char packet[1024] = { 0, };
	_FBI::PT_DEAL_STATUS* p = (_FBI::PT_DEAL_STATUS*)packet;

	memcpy(p->Symbol, pInfo->Symbol, _FBI::FBILEN_SYMBOL);
	sprintf(p->DealSeq, "%0*d", _FBI::FBILEN_DEAL_SEQ, pInfo->DealSeq);
	sprintf(p->DealStatus, "%02d", pInfo->DealStatus);
	switch (pInfo->DealStatus)
	{
	case _FBI::DEAL_STATUS_ORD:
		sprintf(p->Time, pInfo->tm_order);
		break;
	case _FBI::DEAL_STATUS_WAIT:
		sprintf(p->Time, pInfo->tm_wait);
		break;
	case _FBI::DEAL_STATUS_RESULT:
		sprintf(p->Time, pInfo->tm_result);
		break;
	default:
		g_log.log(ERR/*NOTIFY*/, "[%s](Status:%s)Update 시도가 잘못됐다", pInfo->Symbol, _FBI::dealstatus(pInfo->DealStatus, z));
		return;
	}
	

	//todo. send to client

	//DB UPDATE
	for (int i = 0; i < 3; i++)
	{
		CDBHandlerAdo db(m_pDBPool->Get());
		char zQ[1024];

		sprintf(zQ, "UPDATE	AA_DEAL_MST "
			"SET	DEAL_STATUS = %d "
			"WHERE	DEAL_SEQ = %d "
			"AND	ARTC_CD = '%s' "
			, pInfo->DealStatus
			, pInfo->DealSeq
			, m_zArtcCd
		);
		if (FALSE == db->ExecQuery(zQ))
		{
			g_log.log(ERR/*NOTIFY*/, "Update DEAL_MST error(%s)", zQ);
			Sleep(1000);
			db->Close();
			continue;
		}
		else
		{
			g_log.log(INFO, "Update DB DEAL_MST OK[%s][SEQ:%d][%s]", m_zArtcCd, pInfo->DealSeq, _FBI::dealstatus(pInfo->DealStatus, z));
			break;
		}
	}
}

//TODO
BOOL CDealManager::InitClientConnect()
{
	return TRUE;
}

//unsigned WINAPI CDealManager::Thread_UpdateDeal(LPVOID lp)
//{
//	CDealManager* pThis = (CDealManager*)lp;
//
//	MSG msg;
//	while (GetMessage(&msg, NULL, 0, 0))
//	{
//		switch (msg.message)
//		{
//		case _FBI::WM_DEAL_STATUS:
//			pThis->UpdateDeal((_FBI::ST_DEAL_INFO*)msg.lParam);
//			g_memPool.release((char*)msg.lParam);
//			break;
//		case _FBI::WM_TERMINATE:
//			return 0;
//		}
//	}
//	return 0;
//}