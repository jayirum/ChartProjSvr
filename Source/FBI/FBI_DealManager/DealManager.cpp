
#include "DealManager.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/Util.h"
#include "../common/FBIInc.h"


extern BOOL		g_bContinue;	// flag whether continue process or not
extern CLogMsg	g_log;
extern char		g_zConfig[_MAX_PATH];
extern CMemPool	g_memPool;


CDealManager::CDealManager(char* pzStkCd) : CBaseThread(pzStkCd)
{
	strcpy(m_zStkCd, pzStkCd);
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
			g_log.log( ERR/*NOTIFY*/, "(%s)Thread DB OPEN FAILED.(%s)(%s)(%s)", m_zStkCd, ip, id, pwd);
			return FALSE;
		}
	}

	if (!InitClientConnect())
		return FALSE;

	if (!LoadChartInfo())
		return FALSE;

	if (!LoadDealInfo())
		return FALSE;

	ResumeThread();
	
	m_hRsltProc = (HANDLE)_beginthreadex(NULL, 0, &Thread_ResultProcByChart, this, 0, &m_unRsltProc);
	
	return TRUE;
}


VOID CDealManager::Finalize()
{
	SAFE_DELETE(m_pDBPool);

	std::map<int, _FBI::ST_DEAL_INFO*>::iterator it;
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
	char z[128];
	char zOrderTime[128], zChartWaitTm[32], zFirstChartNm[32], zNow[32];
	int nDealDateTp;
	SYSTEMTIME st;

	std::string sChartNm;
	if (!m_chartMap.GetFirstChart(&sChartNm))
	{
		g_log.log(ERR, "[%s]��Ʈ �����Ͱ� ��� DealInfo �� Load ���� �ʴ´�.", m_zArtcCd);
		return FALSE;
	}
	strcpy(z, sChartNm.c_str());
	sprintf(zFirstChartNm, "%.2s:%.2s", z + 8, z + 10);

	CDBHandlerAdo db(m_pDBPool->Get());
	
	sprintf(zQ, "SELECT DEAL_SEQ, ARTC_CD, DATE_TP, TM_ORD_START, TM_WAIT_START, TM_CHARTWAIT_START, TM_END, DEAL_STATUS, 3 AS DURATION "
				" FROM AA_DEAL_MST WHERE ARTC_CD=dbo.AA_FN_ARTC_CD('%s') AND TM_CHARTWAIT_START>='%s' ", m_zStkCd, zFirstChartNm);
	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(ERR/*NOTIFY*/, "Load DealInfo Error(%s)", zQ);
		return FALSE;
	}
	while (db->IsNextRow())
	{
		// hh:mm:ss
		db->GetStr("TM_ORD_START", zOrderTime);
		db->GetStr("TM_CHARTWAIT_START", zChartWaitTm);
		nDealDateTp = db->GetLong("DATE_TP");

		//// DURATION *2 ���� ���� �����ʹ� ���� Loading �� �ʿ� ����.
		//int nPassedSecs = CUtil::GetPassedSeconds(zOrderTime, TRUE/*bColon*/);
		//if (nPassedSecs / 60 > 2)
		//{
		//	db->Next();
		//	continue;
		//}

		//// ������Ʈ �ð����� ���� deal �� ������.
		//if (sChartNm.compare(zChartWaitTm) > 0)
		//{
		//	g_log.log(INFO, "[%s]Deal�� ��Ʈ���ð�(%s)�� ���� ��Ʈ�ð�(%s)���� �����̹Ƿ� ������.",
		//		m_zArtcCd, zChartWaitTm, (char*)sChartNm.c_str());

		//	db->Next();
		//	continue;
		//}


		// �ֹ����۽ð��� �̹� ������ ������.
		// ������ ������ ����ؾ� �Ѵ�. 
		// 1���� : 08:00:00 ~ 23:59:59
		// 2���� : 00:00:00 ~ 06:00
		GetLocalTime(&st); sprintf(zNow, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);

		_FBI::EN_DATETP nowDateTp = _FBI::dateTp(zNow);
		// 1 ����
		if(nowDateTp ==_FBI::DATETP_TRADE)
		{
			// 1���� �ΰ͸� ���Ѵ�.
			if (nDealDateTp == (int)_FBI::DATETP_TRADE)
			{
				if (strncmp(zOrderTime, zNow, strlen(zOrderTime)) <= 0)
				{
					db->Next();
					continue;
				}
			}
		}

		// 2 ����
		if (nowDateTp == _FBI::DATETP_NEXT)
		{
			// 1���� �ΰ��� �����.
			if(nDealDateTp ==(int)_FBI::DATETP_TRADE)
			{
				db->Next();
				continue;
			}
			else
			{
				if (strncmp(zOrderTime, zNow, strlen(zOrderTime)) <= 0)
				{
					db->Next();
					continue;
				}
			}
		}

		_FBI::ST_DEAL_INFO* pInfo = new _FBI::ST_DEAL_INFO;
		ZeroMemory(pInfo, sizeof(_FBI::ST_DEAL_INFO));
		pInfo->DealSeq = db->GetLong("DEAL_SEQ");
		db->GetStr("TM_ORD_START", pInfo->tm_order);		// hh:mm:ss
		db->GetStr("ARTC_CD", pInfo->ArtcCd);
		strcpy(m_zArtcCd, pInfo->ArtcCd);
		strcpy(pInfo->tm_order, zOrderTime);
		db->GetStr("TM_WAIT_START", pInfo->tm_wait);
		db->GetStr("TM_CHARTWAIT_START", pInfo->tm_chartwait);
		db->GetStr("TM_END", pInfo->tm_end);
		pInfo->DealStatus = (_FBI::EN_DEAL_SATTUS)db->GetLong("DEAL_STATUS");
		pInfo->DurationMin = db->GetLong("DURATION");
		pInfo->DateTp = nDealDateTp;

		g_log.log(INFO, "[SEQ:%d](%s)(%s)(%s)", pInfo->DealSeq, pInfo->tm_order, pInfo->tm_wait, pInfo->tm_chartwait);
		EnterCriticalSection(&m_csDeal);
		m_mapDeal[pInfo->DealSeq] = pInfo;
		LeaveCriticalSection(&m_csDeal);

		db->Next();
	}
	db->Close();
	g_log.log(INFO, "Succeeded in DealInfo[%s]", m_zArtcCd);
	return TRUE;
}


/*
struct PT_API_CHART
{
char	STX[1];
char	Len[4];
char	StkCd[FBILEN_SYMBOL];
char	Date[8];
char	ChartType[1];	// m:minute, h:hour, d:day
char	TimeFrame[3];	// 001: 1min, 005
char	ChartTime[5];	// hh:mm
char	Open[FBILEN_PRC];
char	High[FBILEN_PRC];
char	Low[FBILEN_PRC];
char	Close[FBILEN_PRC];
char	Volume[FBILEN_VOLUME];
char	ETX[1];
};
*/
BOOL CDealManager::LoadChartInfo()
{
	char zQ[1024];
	char z[128];
	char zDt[32], zTm[32];

	CDBHandlerAdo db(m_pDBPool->Get());
	sprintf(zQ, "SELECT TRADE_DT, CHART_TP, CHART_NM, OPEN_PRC, HIGH_PRC, LOW_PRC, CLOSE_PRC, CNTR_QTY "
		" FROM AA_CHART_DATA_MIN_01 WHERE SYMBOL='%s' ORDER BY CHART_NM", m_zStkCd);
	if (FALSE == db->ExecQuery(zQ))
	{
		g_log.log(ERR/*NOTIFY*/, "Load Chart Error(%s)(%s)",db->GetError(), zQ);
		return FALSE;
	}
	while (db->IsNextRow())
	{
		_FBI::PT_API_CHART* pChart = new _FBI::PT_API_CHART;
		memset(pChart, 0x20, sizeof(_FBI::PT_API_CHART));
		
		db->GetStr("TRADE_DT", z);
		memcpy(pChart->StkCd, m_zStkCd, min(sizeof(pChart->StkCd),strlen(z)));

		// yyyymmddhhss
		db->GetStr("CHART_NM", z);
		sprintf(zDt, "%.8s", z);
		sprintf(zTm, "%.2s:%.2s", z + 8, z + 10);
		
		memcpy(pChart->Date, zDt, sizeof(pChart->Date));
		pChart->ChartType[0] = 'M';
		memcpy(pChart->TimeFrame, "001", 3);
		memcpy(pChart->ChartTime, zTm, sizeof(pChart->ChartTime));

		sprintf(z,"%.5f", db->GetDbl("OPEN_PRC"));
		memcpy(pChart->Open, z, min(sizeof(pChart->Open),strlen(z)));

		sprintf(z, "%.5f", db->GetDbl("HIGH_PRC"));
		memcpy(pChart->High, z, min(sizeof(pChart->High), strlen(z)));
		
		sprintf(z, "%.5f", db->GetDbl("LOW_PRC"));
		memcpy(pChart->Low, z, min(sizeof(pChart->Low), strlen(z)));
		
		sprintf(z, "%.5f", db->GetDbl("CLOSE_PRC"));
		memcpy(pChart->Close, z, min(sizeof(pChart->Close), strlen(z)));
		
		m_chartMap.Save((char*)pChart);

		db->Next();
	}
	db->Close();
	g_log.log(INFO, "Succeeded in DealInfo[%s]", m_zArtcCd);
	return TRUE;
}

VOID CDealManager::ThreadFunc()
{
	//if (!LoadDealInfo())
	//	return;

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
				m_chartMap.Save((char*)msg.lParam);
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
	chart �����͸� �̿��� ü��ó��
	struct PT_API_CHART
	{
	char	STX[1];
	char	Len[4];
	char	Symbol[FBILEN_SYMBOL];
	char	Date[8];
	char	ChartType[1];	// m:minute, h:hour, d:day
	char	TimeFrame[3];	// 001: 1min, 005
	char	ChartTime[5];	// hh:mm
	char	Open[FBILEN_PRC];
	char	High[FBILEN_PRC];
	char	Low[FBILEN_PRC];
	char	Close[FBILEN_PRC];
	char	Volume[FBILEN_VOLUME];
	char	ETX[1];
	};

	[AA_ORD_RESULT]
	@I_STK_CD	VARCHAR(10)
	,@I_CHART_TP	INT
	,@I_OPEN_PRC_S	VARCHAR(20)
	,@I_CLOSE_PRC_S	VARCHAR(20)
*/
unsigned WINAPI CDealManager::Thread_ResultProcByChart(LPVOID lp)
{
	CDealManager* pThis = (CDealManager*)lp;
	_FBI::PT_API_CHART* pChart;
	std::string sChartNm;

	// Deal ����
	std::map<int, _FBI::ST_DEAL_INFO*>::iterator itDeal;
	_FBI::ST_DEAL_INFO* pDeal;
	int nDealSeq;
	BOOL bFind;

	while (pThis->m_bContinue)
	{
		Sleep(_FBI::TIMEOUT_CHECK_DEAL);

		sChartNm = "";
		pChart = NULL;
		
		// ���� ��Ʈ ������ ���Ѵ�.
		pChart = pThis->m_chartMap.GetFirstChart(&sChartNm);
		if (pChart==NULL)
			continue;

		
		// Deal ������ ���Ѵ�.		
		for (itDeal = pThis->m_mapDeal.begin(); itDeal != pThis->m_mapDeal.end(); )
		{
			bFind = FALSE;

			nDealSeq = (*itDeal).first;
			pDeal = (*itDeal).second;

			// ��Ʈ�ð��� deal ��Ʈ��� �ð� ��
			int nComp = strncmp(pChart->ChartTime, pDeal->tm_chartwait, 5);


			// ������Ʈ ������ Deal ������ �����. (hh:mm ��)
			if (nComp > 0)
			{
				g_log.log(INFO, "[%s][SEQ:%d]������Ʈ�ð�(%s) ���� ���� DEAL��Ʈ���ð�(%s) �̶� �����.", 
					pThis->m_zArtcCd,nDealSeq, pChart->ChartTime, pDeal->tm_chartwait);

				delete pDeal;
				itDeal = pThis->m_mapDeal.erase(itDeal);
				continue;
			}

			if (nComp < 0) {
				g_log.log(ERR, "[%s][SEQ:%d]������Ʈ�ð�(%s) ���� ���� DEAL��Ʈ���ð�(%s) �̶� ������..",
					pThis->m_zArtcCd, nDealSeq, pChart->ChartTime, pDeal->tm_chartwait);
				break;
			}

			// ���� ������ ���� �̴�.
			if (pDeal->DealStatus == _FBI::DEAL_STATUS_RESULTING)
			{
				bFind = TRUE;
				break;
			}
			else
			{
				bFind = FALSE;
				break;
			}
		}//for (itDeal = pThis->m_mapDeal.begin(); itDeal != pThis->m_mapDeal.end(); )

		if (!bFind)
		{
			continue;
		}

		int nChartTp = 0;	//1����Ʈ
		char zChartNm[32];
		sprintf(zChartNm, "%.8s%.2s%.2s", pChart->Date, pChart->ChartTime, pChart->ChartTime + 3);


		//DB UPDATE
		for (int i = 0; i < 3; i++)
		{
			CDBHandlerAdo db(pThis->m_pDBPool->Get());
			char zQ[1024];

			sprintf(zQ, "EXEC AA_ORD_RESULT "
				"'%.*s'"
				",%d"
				",%d"
				",'%.*s'"
				",'%.*s'"
				, sizeof(pChart->StkCd), pChart->StkCd
				, nDealSeq
				, nChartTp
				, sizeof(pChart->Open), pChart->Open
				, sizeof(pChart->Close), pChart->Close
			);

			if (FALSE == db->ExecQuery(zQ))
			{
				g_log.log(ERR/*NOTIFY*/, "�ֹ����ó��(AA_ORD_RESULT) error(%s)", zQ);
				Sleep(1000);
				db->Close();
				continue;
			}
			else
			{
				g_log.log(INFO, "[�ֹ�ó�� ����][%s][SEQ:%d][CHART_NM:%s][OPEN:%.10s][CLOSE:%.10s]",
					pThis->m_zArtcCd, nDealSeq, (char*)sChartNm.c_str(), pChart->Open, pChart->Close);

				// ������ ��Ʈ ����
				pThis->m_chartMap.DeleteAfterOrerProc(sChartNm);

				// DealInfo ����
				pThis->DealErase(nDealSeq);
				break;
			}
		}

	} // while(pThis->m_bContinue)

	return 0;
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
	 CLIENT

	enum EN_DEAL_SATTUS {
		DEAL_STATUS_NONSTART = 0//	�̽���
		, DEAL_STATUS_ORD		//	�ֹ����� 1
		, DEAL_STATUS_WAIT		//	��� 2
		, DEAL_STATUS_CHARTWAIT	//	��Ʈ��� 3
		, DEAL_STATUS_RESULTING	//	������� 4
		, DEAL_STATUS_DONE		//	�Ϸ� 5
	};

*/
VOID CDealManager::DealManageInner()
{
	char zNow[32] = { 0, }; 
	SYSTEMTIME st;
	int nLen = strlen(zNow);

	// �ִ� 2���� deal �� �������̴�.
	// ��� ó�� / deal ����
	int nDealSeq;
	std::string sOrdTm, sRsltTm;
	_FBI::ST_DEAL_INFO* pInfo;
	_FBI::EN_DATETP nowDataTp;
	BOOL bMatched = FALSE;
	std::map<int, _FBI::ST_DEAL_INFO*>::iterator it;
	for (it = m_mapDeal.begin(); it != m_mapDeal.end(); it++)
	{
		sOrdTm.clear();
		pInfo = NULL;
		nDealSeq = (*it).first;
		pInfo = (*it).second;

		if (pInfo->DealStatus == _FBI::DEAL_STATUS_RESULTING)
			continue;

		GetLocalTime(&st);
		sprintf(zNow, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
		nLen = strlen(zNow);
		nowDataTp = _FBI::dateTp(zNow);

		// HH:MM ������ �񱳸� �ؼ� �Ҽӵ� DEAL �� ������.
		//if (strncmp(pInfo->tm_order, zNow, 5) <= 0 &&
		//	strncmp(zNow, pInfo->tm_end, 5) <= 0
		//	)
		//{
		//	bMatched = TRUE;
		//	break;
		//}
		if (strncmp(pInfo->tm_order, zNow, 5) <= 0)
		{
			bMatched = TRUE;
			break;
		}
	}

	if (!bMatched)	return;	//----------------------------------

	char status[32];

	// �̹� ó���� �� DEAL �̸� MAP ���� ����
	// ==> ���ó�� �� �ű⼭ ������ �ϱ� ������ �� �ڵ�� ���� �ȵ� ���̴�.
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_DONE)
	{
		g_log.log(INFO, "[DEAL_MANAGE(%s)(����:%s)(DEAL�ֹ��ð�:%s)(����:%s)]�̹� �Ϸ�� DEAL �̶� MAP ���� ����",
			pInfo->ArtcCd, zNow, sOrdTm.c_str(), _FBI::dealstatus(pInfo->DealStatus, status));
		delete pInfo;
		m_mapDeal.erase(it);
		return;	//----------------------------------
	}
	
	// ������ð� �� ��. ������ð� ���� ���簡 �� �ʴ�.
	if (strncmp(zNow, pInfo->tm_end, nLen) > 0)
	{
		return DealResulting(zNow, pInfo);
	} // if (strncmp(zNow, pInfo->tm_result, nLen) >= 0)

	// ��Ʈ���ð� �� ��. ��Ʈ���ð� ���� ���簡 �� �ʴ�.
	if (strncmp(zNow, pInfo->tm_chartwait, nLen) >= 0)
	{
		return DealChartWait(zNow, pInfo);
	} // if (strncmp(zNow, pInfo->tm_result, nLen) >= 0)

	// ���ð��� ��. ���ð����� ���簡 �� �ʴ�.
	if (strncmp(zNow, pInfo->tm_wait, nLen) >= 0)
	{
		return DealWait(zNow, pInfo);
	}

	// �ֹ��ð��� ��. �ֹ��ð����� ���簡 �� �ʴ�.
	if (strncmp(zNow, pInfo->tm_order, nLen) >= 0)
	{
		return DealOrd(zNow, pInfo);
	}
}


VOID CDealManager::DealOrd(char* pzNow, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_ORD)
	{
		return;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_NONSTART)
	{
		g_log.log(INFO, "[DEAL_MANAGE(%s)(SEQ:%d)(����:%s)(DEAL�ֹ��ð�:%s)(����:%s)]�ֹ��ð� ������ ���� <�̽���>�̹Ƿ� <�ֹ�>���� UPDATE",
			pInfo->ArtcCd, pInfo->DealSeq, pzNow, pInfo->tm_order, _FBI::dealstatus(pInfo->DealStatus, status));

		// update.
		pInfo->DealStatus = _FBI::DEAL_STATUS_ORD;
		UpdateDeal(pInfo);
		m_mapDeal[pInfo->DealSeq] = pInfo;

		//char* pData = g_memPool.get();
		//memcpy(pData, pInfo, sizeof(_FBI::ST_DEAL_INFO));
		//PostThreadMessage(m_unUpdateDeal, _FBI::WM_DEAL_STATUS, 0, (LPARAM)pData);
	}
	else
	{
		//���°� �̻��ϴ�. ���� ó��.
		g_log.log(ERR/*NOTIFY*/, "[DEAL_MANAGE(%s)(SEQ:%d)(����:%s)(DEAL�ֹ��ð�:%s)(����:%s)]�ֹ��ð� �����µ� ���°� <�̽���>�� �ƴϴ�. ",
			pInfo->ArtcCd, pInfo->DealSeq, pzNow, pInfo->tm_order, _FBI::dealstatus(pInfo->DealStatus, status));
		delete pInfo;
		m_mapDeal.erase(pInfo->DealSeq);
	}
}



VOID CDealManager::DealWait(char* pzNow, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_WAIT)
	{
		return;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_ORD)
	{
		g_log.log(INFO, "[DEAL_MANAGE(%s)(SEQ:%d)(����:%s)(DEAL���ð�:%s)(����:%s)]���ð� ������ ���� <�ֹ�>�̹Ƿ� <���>�� UPDATE",
			pInfo->ArtcCd, pInfo->DealSeq, pzNow, pInfo->tm_wait, _FBI::dealstatus(pInfo->DealStatus, status));

		// update.
		pInfo->DealStatus = _FBI::DEAL_STATUS_WAIT;
		UpdateDeal(pInfo);
		m_mapDeal[pInfo->DealSeq] = pInfo;
	}
	else
	{
		//���°� �̻��ϴ�. ���� ó��.
		g_log.log(ERR/*NOTIFY*/, "[DEAL_MANAGE(%s)(SEQ:%d)(����:%s)(DEAL���ð�:%s)(����:%s)]���ð� �����µ� ���°� <�ֹ�>�� �ƴϴ�. ",
			pInfo->ArtcCd, pInfo->DealSeq, pzNow, pInfo->tm_wait, _FBI::dealstatus(pInfo->DealStatus, status));

		delete pInfo;
		m_mapDeal.erase(pInfo->DealSeq);
	}
}


VOID CDealManager::DealChartWait(char* pzNow, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_CHARTWAIT)
	{
		return;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_WAIT)
	{
		// update.
		g_log.log(INFO, "[DEAL_MANAGE(%s)(SEQ:%d)(����:%s)(DEAL��Ʈ���ð�:%s)(����:%s)]��Ʈ���ð� ������ ���� <���>�̹Ƿ� <��Ʈ���>�� UPDATE",
			pInfo->ArtcCd, pInfo->DealSeq, pzNow, pInfo->tm_chartwait, _FBI::dealstatus(pInfo->DealStatus, status));

		pInfo->DealStatus = _FBI::DEAL_STATUS_CHARTWAIT;
		UpdateDeal(pInfo);
		m_mapDeal[pInfo->DealSeq] = pInfo;

		//char* pData = g_memPool.get();
		//memcpy(pData, pInfo, sizeof(_FBI::ST_DEAL_INFO));
		//PostThreadMessage(m_unUpdateDeal, _FBI::WM_DEAL_STATUS, 0, (LPARAM)pData);
	}
	else
	{
		//���°� �̻��ϴ�. ���� ó��.
		g_log.log(ERR/*NOTIFY*/, "[DEAL_MANAGE(%s)(SEQ:%d)(����:%s)(DEAL��Ʈ���ð�:%s)(����:%s)]��Ʈ���ð� �����µ� ���°� <���>�� �ƴϴ�. ",
			pInfo->ArtcCd, pInfo->DealSeq, pzNow, pInfo->tm_chartwait, _FBI::dealstatus(pInfo->DealStatus, status));

		delete pInfo;
		m_mapDeal.erase(pInfo->DealSeq);
	}
}

VOID CDealManager::DealResulting(char* pzNow, _FBI::ST_DEAL_INFO* pInfo)
{
	char status[128];
	if (pInfo->DealStatus == _FBI::DEAL_STATUS_RESULTING)
	{
		return;
	}
	else if (pInfo->DealStatus == _FBI::DEAL_STATUS_CHARTWAIT)
	{
		// update.
		g_log.log(INFO, "[DEAL_MANAGE(%s)(SEQ:%d)(����:%s)(DEAL�������ð�:%s)(����:%s)]�������ð� ������ ���� <��Ʈ���>�̹Ƿ� <�������>�� UPDATE",
			pInfo->ArtcCd, pInfo->DealSeq, pzNow, pInfo->tm_end, _FBI::dealstatus(pInfo->DealStatus, status));

		pInfo->DealStatus = _FBI::DEAL_STATUS_RESULTING;
		UpdateDeal(pInfo);
		m_mapDeal[pInfo->DealSeq] = pInfo;

		//char* pData = g_memPool.get();
		//memcpy(pData, pInfo, sizeof(_FBI::ST_DEAL_INFO));
		//PostThreadMessage(m_unUpdateDeal, _FBI::WM_DEAL_STATUS, 0, (LPARAM)pData);
	}
	else
	{
		//���°� �̻��ϴ�. ���� ó��.
		g_log.log(ERR/*NOTIFY*/, "[DEAL_MANAGE(%s)(SEQ:%d)(����:%s)(DEAL�������ð�:%s)(����:%s)]�������ð� �����µ� ���°� <��Ʈ���>�� �ƴϴ�. ",
			pInfo->ArtcCd, pInfo->DealSeq, pzNow, pInfo->tm_end, _FBI::dealstatus(pInfo->DealStatus, status));

		delete pInfo;
		m_mapDeal.erase(pInfo->DealSeq);
	}
}

VOID CDealManager::DealErase(int nDealSeq)
{
	EnterCriticalSection(&m_csDeal);
	m_mapDeal.erase(nDealSeq);
	LeaveCriticalSection(&m_csDeal);
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
	_FBI::PT_DEAL_STATUS* p = (_FBI::PT_DEAL_STATUS*)packet;

	memcpy(p->ArtcCd, m_zStkCd, min(strlen(m_zArtcCd),_FBI::FBILEN_SYMBOL));
	memcpy(p->StkCd, m_zStkCd, min(strlen(m_zStkCd), _FBI::FBILEN_SYMBOL));

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
	case _FBI::DEAL_STATUS_CHARTWAIT:
		sprintf(p->Time, pInfo->tm_chartwait);
		break;
	case _FBI::DEAL_STATUS_RESULTING:
		sprintf(p->Time, pInfo->tm_end);
		break;
	default:
		g_log.log(ERR/*NOTIFY*/, "[%s](Status:%s)Update �õ��� �߸��ƴ�", pInfo->ArtcCd, _FBI::dealstatus(pInfo->DealStatus, z));
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













//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CChartMap
//////////////////////////////////////////////////////////////////////////////////////////////
CChartMap::CChartMap()
{
	InitializeCriticalSection(&m_csChart);
}

CChartMap::~CChartMap()
{
	EnterCriticalSection(&m_csChart);
	std::map<std::string, _FBI::PT_API_CHART*>::iterator it2;
	for (it2 = m_mapChart.begin(); it2 != m_mapChart.end(); ++it2)
	{
		_FBI::PT_API_CHART* p = (*it2).second;
		delete p;
	}
	m_mapChart.clear();
	DeleteCriticalSection(&m_csChart);
}


VOID CChartMap::Save(char* pChartData)
{
	_FBI::PT_API_CHART* p = (_FBI::PT_API_CHART*)pChartData;

	_FBI::PT_API_CHART* pNew = new _FBI::PT_API_CHART;
	memcpy(pNew, p, sizeof(_FBI::PT_API_CHART));

	char zChartNm[32] = { 0, };
	_FBI::chartName(p->Date, p->ChartTime, zChartNm);

	EnterCriticalSection(&m_csChart);
	m_mapChart[zChartNm] = pNew;
	LeaveCriticalSection(&m_csChart);

	//if (m_sFirstChartNm.compare(zChartNm) > 0)
	//	m_sFirstChartNm = std::string(zChartNm);
}

VOID CChartMap::DeleteAfterOrerProc(_In_ std::string sChartNm)
{
	EnterCriticalSection(&m_csChart);
	m_mapChart.erase(sChartNm);
	LeaveCriticalSection(&m_csChart);
}


// return : chart name
_FBI::PT_API_CHART* CChartMap::GetFirstChart(_Out_ std::string *psChartNm)
{
	_FBI::PT_API_CHART* p = NULL;

	if (m_mapChart.empty())
		return NULL;

	EnterCriticalSection(&m_csChart);
	*psChartNm = (*m_mapChart.begin()).first;
	p = (*m_mapChart.begin()).second;
	LeaveCriticalSection(&m_csChart);
	return p;
}

_FBI::PT_API_CHART* CChartMap::GetNextChart(_In_ std::string sPrevChartNm, _Out_ std::string *psChartNm)
{
	_FBI::PT_API_CHART* p = NULL;

	EnterCriticalSection(&m_csChart);
	std::map<std::string, _FBI::PT_API_CHART*>::iterator it = m_mapChart.find(sPrevChartNm);
	if (it == m_mapChart.end())
	{
		LeaveCriticalSection(&m_csChart);
		return NULL;
	}

	it++;

	if (it == m_mapChart.end())
	{
		LeaveCriticalSection(&m_csChart);
		return NULL;
	}

	*psChartNm = (*it).first;
	p = (*it).second;
	LeaveCriticalSection(&m_csChart);
	return p;
}


