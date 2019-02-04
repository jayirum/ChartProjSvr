
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
			g_log.log(NOTIFY, "(%s)Thread DB OPEN FAILED.(%s)(%s)(%s)", m_zArtcCd, ip, id, pwd);
			return FALSE;
		}
	}

	if (!InitClientConnect())
		return FALSE;

	m_hUpdateDeal = (HANDLE)_beginthreadex(NULL, 0, &Thread_UpdateDeal, this, 0, &m_unUpdateDeal);
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
	CDBHandlerAdo db(m_pDBPool->Get());
	char zQ[1024];
	sprintf(zQ, "SELECT DEAL_SEQ, TM_ORD_START, TM_WAIT_START, TM_RESULT_START, DEAL_STATUS, 3 AS DURATION WHERE ARTC_CD='%s'", m_zArtcCd);
	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(NOTIFY, "Load DealInfo Error(%s)", zQ);
		return FALSE;
	}
	while (db->IsNextRow())
	{
		_FBI::ST_DEAL_INFO* pInfo = new _FBI::ST_DEAL_INFO;
		pInfo->DealSeq = db->GetLong("DEAL_SEQ");
		db->GetStr("TM_ORD_START", pInfo->tm_order);		// hh:mm:ss
		db->GetStr("TM_WAIT_START", pInfo->tm_wait);
		db->GetStr("TM_RESULT_START", pInfo->tm_result);
		pInfo->DealStatus = db->GetLong("DEAL_STATUS");
		pInfo->DurationMin = db->GetLong("DURATION");
		

		// DURATION *2 ���� ���� �����ʹ� ���� Loading �� �ʿ� ����.
		int nPassedSecs = CUtil::GetPassedSeconds(pInfo->tm_order, TRUE/*bColon*/);
		if (nPassedSecs / 60 > 2)
		{
			delete pInfo;
			continue;
		}

		std::string sOrdTM = std::string(pInfo->tm_order);

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
				//TODO
				break;
			case _FBI::WM_TERMINATE:
				//TODO
				break;
			}
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)
}


VOID CDealManager::DealManage()
{
	EnterCriticalSection(&m_csDeal);
	DealManageInner();
	LeaveCriticalSection(&m_csDeal);
}

/*
	����ð��� MAP �� ����ó�� DEAL STATUS �� ���ؼ� 
	UPDATE DB
	NOTIFY CLIENT

	DEAL_STATUS	0	�̽���
	DEAL_STATUS	1	�ֹ�����
	DEAL_STATUS	2	���
	DEAL_STATUS	3	�������
	DEAL_STATUS	4	�Ϸ�

*/
VOID CDealManager::DealManageInner()
{
	char zNow[32]; SYSTEMTIME st; GetLocalTime(&st);
	sprintf(zNow, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	int nLen = strlen(zNow);

	// ���� ó�� DEAL ������ �����´�.
	std::string sOrdTm;
	_FBI::ST_DEAL_INFO* pInfo;

	std::map<std::string, _FBI::ST_DEAL_INFO*>::iterator it = m_mapDeal.begin();;
	sOrdTm = (*it).first;
	pInfo = (*it).second;

	char status[32];

	// �̹� ó���� �� DEAL �̸� MAP ���� ����
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_DONE)
	{
		g_log.log(INFO, "[DEAL_MANAGE(%s)(����:%s)(DEAL�ֹ��ð�:%s)(����:%s)]�̹� �Ϸ�� DEAL �̶� MAP ���� ����",
			pInfo->Symbol, zNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status));
		delete pInfo;
		m_mapDeal.erase(it);
		return;
	}

	// ����ð��� ��. ����ð����� ���簡 �� �ʴ�.
	if (strncmp(zNow, pInfo->tm_result, nLen) >= 0)
	{
		return DealResult(zNow, sOrdTm, pInfo);
	} // if (strncmp(zNow, pInfo->tm_result, nLen) >= 0)

	// ���ð��� ��. ���ð����� ���簡 �� �ʴ�.
	if (strncmp(zNow, pInfo->tm_wait, nLen) >= 0)
	{
		
		return DealWait(zNow, sOrdTm, pInfo);
	}

	// �ֹ��ð��� ��. �ֹ��ð����� ���簡 �� �ʴ�.
	if (strncmp(zNow, pInfo->tm_order, nLen) >= 0)
	{
		return DealOrd(zNow, sOrdTm, pInfo);
	}
}


VOID CDealManager::DealOrd(char* pzNow, std::string sOrdTm, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_NONSTART)
	{
		g_log.log(INFO, "[DEAL_MANAGE(%s)(����:%s)(DEAL�ֹ��ð�:%s)(����:%s)]�ֹ��ð�(%s)������ ���� <�̽���>�̹Ƿ� <�ֹ�>���� UPDATE",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status), pInfo->tm_order);

		// update.
		pInfo->DealStatus = _FBI::DEAL_STATUS_ORD;

		char* pData = g_memPool.get();
		memcpy(pData, pInfo, sizeof(_FBI::ST_DEAL_INFO));
		PostThreadMessage(m_unUpdateDeal, _FBI::WM_DEAL_STATUS, 0, (LPARAM)pData);

		m_mapDeal[sOrdTm] = pInfo;

	}
	else
	{
		//���°� �̻��ϴ�. ���� ó��.
		g_log.log(NOTIFY, "[DEAL_MANAGE(%s)(����:%s)(DEAL�ֹ��ð�:%s)(����:%s)]�ֹ��ð� �����µ� ���°� <�̽���>�� �ƴϴ�. ",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status));
		delete pInfo;
		m_mapDeal.erase(sOrdTm);
	}
}



VOID CDealManager::DealWait(char* pzNow, std::string sOrdTm, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_ORD)
	{
		g_log.log(INFO, "[DEAL_MANAGE(%s)(����:%s)(DEAL�ֹ��ð�:%s)(����:%s)]���ð�(%s)������ ���� <�ֹ�>�̹Ƿ� <���>�� UPDATE",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status), pInfo->tm_wait);

		// update.
		pInfo->DealStatus = _FBI::DEAL_STATUS_WAIT;

		char* pData = g_memPool.get();
		memcpy(pData, pInfo, sizeof(_FBI::ST_DEAL_INFO));
		PostThreadMessage(m_unUpdateDeal, _FBI::WM_DEAL_STATUS, 0, (LPARAM)pData);

		m_mapDeal[sOrdTm] = pInfo;

	}
	else
	{
		//���°� �̻��ϴ�. ���� ó��.
		g_log.log(NOTIFY, "[DEAL_MANAGE(%s)(����:%s)(DEAL�ֹ��ð�:%s)(����:%s)]���ð� �����µ� ���°� <�ֹ�>�� �ƴϴ�. ",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status));
		delete pInfo;
		m_mapDeal.erase(sOrdTm);
	}
}


VOID CDealManager::DealResult(char* pzNow, std::string sOrdTm, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_WAIT)
	{
		// update.
		g_log.log(INFO, "[DEAL_MANAGE(%s)(����:%s)(DEAL�ֹ��ð�:%s)(����:%s)]����ð�(%s)������ ���� <���>�̹Ƿ� <���>�� UPDATE",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status), pInfo->tm_result);

		pInfo->DealStatus = _FBI::DEAL_STATUS_RESULT;

		char* pData = g_memPool.get();
		memcpy(pData, pInfo, sizeof(_FBI::ST_DEAL_INFO));
		PostThreadMessage(m_unUpdateDeal, _FBI::WM_DEAL_STATUS, 0, (LPARAM)pData);

		m_mapDeal[sOrdTm] = pInfo;
	}
	else
	{
		//���°� �̻��ϴ�. ���� ó��.
		g_log.log(NOTIFY, "[DEAL_MANAGE(%s)(����:%s)(DEAL�ֹ��ð�:%s)(����:%s)]����ð� �����µ� ���°� <���>�� �ƴϴ�. ",
			pInfo->Symbol, pzNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status));
		delete pInfo;
		m_mapDeal.erase(sOrdTm);
	}
}



unsigned WINAPI CDealManager::Thread_UpdateDeal(LPVOID lp)
{
	CDealManager* pThis = (CDealManager*)lp;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		switch (msg.message)
		{
		case _FBI::WM_DEAL_STATUS:
			pThis->UpdateDeal((_FBI::ST_DEAL_INFO*)msg.lParam);
			g_memPool.release((char*)msg.lParam);
			break;
		case _FBI::WM_TERMINATE:
			return 0;
		}
	}
	return 0;
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
char Time[8];				// �� status ���۽ð�
};
*/
void CDealManager::UpdateDeal(_FBI::ST_DEAL_INFO* pInfo)
{
	// send to the client
	char z[128];
	char packet[1024] = { 0, };
	_FBI::ST_DEAL_STATUS* p = (_FBI::ST_DEAL_STATUS*)packet;

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
		g_log.log(NOTIFY, "[%s](Status:%s)Update �õ��� �߸��ƴ�", pInfo->Symbol, _FBI::dealstatus(pInfo->DealStatus, z));
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
			g_log.log(NOTIFY, "Update DEAL_MST error(%s)", zQ);
			Sleep(1000);
			db->Close();
			continue;
		}
		else
		{
			g_log.log(INFO, "[%s][%d][%s]Succeeded in updating DEAL_MST", m_zArtcCd, pInfo->DealSeq, _FBI::dealstatus(pInfo->DealStatus, z));
			break;
		}
	}
}

//TODO
BOOL CDealManager::InitClientConnect()
{
	return TRUE;
}