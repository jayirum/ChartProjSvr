
#include "UserStratOrd.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "Main.h"

extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
CUserStratOrd::CUserStratOrd(char* pzUserID, CMemPool* pMemPool, unsigned dwSaveThread, unsigned dwSendThread)
{
	ZeroMemory(&m_UserInfo, sizeof(m_UserInfo));

	FORMAT_USERID(pzUserID, m_UserInfo.zUserID);

	m_dwSaveThread = dwSaveThread;
	m_dwSendThread = dwSendThread;
	m_pMemPool = pMemPool;
	m_UserInfo.bStratApplied = FALSE;
	
	//strcpy(m_zSymbol, pzSymbol);
	//strcpy(m_zArtc, pzArtcCode);
	//GET_SHM_NM(pzArtcCode, m_zShmNm);
	//GET_SHM_LOCK_NM(pzArtcCode, m_zMutexNm);
	
	m_pShmQ = NULL;
	m_pMDSub = NULL;
	m_pDBPool = NULL;
	m_pStratLog = NULL;
			
	m_bInitCheck = FALSE;
	m_pSignalResult = new char[LEN_BUFF_SIZE];
	m_pClientBuff = new char[LEN_BUFF_SIZE];
	ResumeThread();
}


CUserStratOrd::~CUserStratOrd()
{
	EndSubScribe();
	EndShm();
	//EndSignals();
	EndDB();
	EndStratLog();;
	delete[] m_pSignalResult;
	delete[] m_pClientBuff;
}

BOOL CUserStratOrd::BeginShm()
{
	char szDir[_MAX_PATH], szModule[_MAX_PATH], szConfig[_MAX_PATH];
	CUtil::GetMyModuleAndDir(szDir, szModule, szConfig);

	//	OPEN SHM
	if (!m_pShmQ->Open((LPCTSTR)m_zShmNm, (LPCTSTR)m_zMutexNm))
	{
		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN ����(%s)(symbol:%s)(%s)", m_zShmNm, m_UserInfo.SymbolInfo.zSymbolInfo, m_pShmQ->GetErr());
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "SHM OPEN ����(%s)", m_zMutexNm);

	return TRUE;
}


BOOL CUserStratOrd::BeginSubscribe()
{
	SAFE_DELETE(m_pMDSub);

	m_pMDSub = new CNanoPubSub(TP_SUB);
	return (m_pMDSub->Begin(INNER_CHANNEL_PUBSUB));
}

BOOL CUserStratOrd::BeginDB()
{
	char ip[32], id[32], pwd[32], cnt[32], name[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", name);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_POOL_CNT", cnt);

	m_pDBPool = new CDBPoolAdo(ip, id, pwd, name);
	if (!m_pDBPool->Init(1))
	{
		g_log.log(LOGTP_ERR, "DB OPEN FAIL(MSG:%s)", m_pDBPool->GetMsg());
		g_log.log(LOGTP_ERR, "(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
		return FALSE;
	}
	g_log.log(LOGTP_ERR, "DB OPEN OK(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
	return TRUE;
}

VOID CUserStratOrd::EndDB()
{
	SAFE_DELETE(m_pDBPool);
}

VOID CUserStratOrd::EndSubScribe()
{
	m_pMDSub->End();
	SAFE_DELETE(m_pMDSub);
}

void CUserStratOrd::EndShm()
{
	m_pShmQ->Close();
	SAFE_DELETE(m_pShmQ);
}

//

VOID CUserStratOrd::ThreadFunc()
{
	__try {
		MainFunc();
	}
	__except (ReportException(GetExceptionCode(), "CUserStratOrd::ThreadFunc", m_szMsg))
	{
		g_log.log(LOGTP_FATAL, m_szMsg);
		exit(0);
	}

}


BOOL CUserStratOrd::BeginStratLog()
{
	if (!m_pStratLog)
		m_pStratLog = new CStratLog;

	return m_pStratLog->Begin(m_UserInfo.zUserID);
}

VOID CUserStratOrd::EndStratLog()
{
	SAFE_DELETE(m_pStratLog);
}

VOID CUserStratOrd::MainFunc()
{

	g_log.log(LOGTP_SUCC, "[THREAD ID:%d](%s)Start...", GetMyThreadID(), m_UserInfo.zUserID);
	
	if (!BeginDB())
		return;

	if (!BeginShm())
		return ;


	if (!LoadUserInfo(FALSE))
	{
		return;
	}

	if (!BeginStratLog())
		return;

	if (!BeginSubscribe())
		return;

	//TODO if (!BeginSignals())
	//	return;


	char zSignalDataBuff[LEN_BUFF_SIZE] = { 0, };
	char zClientBuff[LEN_BUFF_SIZE] = { 0, };
	char zMsg[LEN_BUFF_SIZE] = { 0, };

	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, 1, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) 
		{
			g_log.log(LOGTP_ERR, "[THREAD ID:%d]WAIT ERROR(%d)", GetMyThreadID(), GetLastError());
			Sleep(1000);
			continue;
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message== WM_MANAGER_NOTIFY_SOCK)
			{
				// �����ڿ�û ó��. proceed manager's request
				DoManagerNotify((void*)msg.lParam);
				break;
			}
			if (msg.message == WM_RECV_CLIENT)
			{
				ClientDataProc((char*)msg.lParam);
			}
			m_pMemPool->release((char*)msg.lParam);
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))


		memset(zSignalDataBuff, 0x00, sizeof(zSignalDataBuff));
		int nRecvLen = m_pMDSub->Subscribe((void*)zSignalDataBuff);
		
		// Do Make Signal after receiving marketdata;
		if (nRecvLen > 0)
		{
			if (!IsStratApplied())
				continue;

			//g_log.log(LOGTP_SUCC, zSignalDataBuff);
			ZeroMemory(zClientBuff, sizeof(zClientBuff));
			StratOrdWrapper(zSignalDataBuff);

			//TODO. Send2client
		}
		
	} // while (TRUE)
	
}

BOOL CUserStratOrd::IsSameUserID(char* psUserID, int nIDLen)
{
	return (strncmp(m_UserInfo.zUserID, psUserID, nIDLen) == 0);
}

BOOL CUserStratOrd::IsMyStratSymbol(char* pzSymbol)
{
	CUtil::TrimAll(pzSymbol, strlen(pzSymbol));

	return (strncmp(m_UserInfo.zSymbol, pzSymbol, strlen(pzSymbol)) == 0);
}

// client ���� ������ TR, REAL �� ó��
VOID CUserStratOrd::ClientDataProc(char* pRecvData)
{
	PACK_HEADER_C2S* pH = (PACK_HEADER_C2S*)pRecvData;

	// �� ������ �ƴϸ� ������.
	if (!IsSameUserID(pH->UserID, LEN_USERID))
		return;

	char zSymbol[LEN_SYMBOL + 1];
	sprintf(zSymbol, "%.*s", sizeof(pH->Symbol), pH->Symbol);
	if (!IsMyStratSymbol(zSymbol)) {
		m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, " ", "���Ź��� Ŭ���̾�Ʈ��Ŷ ������ �� ��������� �ٸ���(��������:%s)", zSymbol);
		return;
	}

	if (pH->RsltTp[0] != 'S')
	{
		m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, " ", "���Ź��� Ŭ���̾�Ʈ��Ŷ�� ������ �ƴϴ�.(���:%c)", pH->RsltTp[0]);
		return;
	}

	if (strncmp(pH->Code, CD_C_ORDER, sizeof(pH->Code)) == 0)
	{
		PACK_C_ORDCNTR* p = (PACK_C_ORDCNTR*)pRecvData;
		if(p->ReplyTp[0] == 'O')
			ClientDataRealOrd(pRecvData);
		if (p->ReplyTp[0] == 'E')
			ClientDataRealExec(pRecvData);
	}
	if (strncmp(pH->Code, CD_C_NCLR, sizeof(pH->Code)) == 0)
		ClientDataTrPos(pRecvData);
	if (strncmp(pH->Code, CD_C_REMAIN_ORD, sizeof(pH->Code)) == 0)
		ClientDataTrOrd(pRecvData);

}

// �ֹ� real �� ���� �� ����
// mit �� �ֹ� ���۽����� �̹� �����ϰ� �ִ�. CLINET ��  MIT �� ���ؼ� �������� �ʴ´�.
VOID CUserStratOrd::ClientDataRealOrd(char* pRecvData)
{
	PACK_C_ORDCNTR* p = (PACK_C_ORDCNTR*)pRecvData;
	
	// �ź�ó��
	if (p->ReplyTp[0] == 'R')
	{
		m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, " ", "�ź��ֹ�����.(#:%.*s)(��#:%.*s)(Msg:%.*s)"
			, sizeof(p->OrdNo), p->OrdNo, sizeof(p->OrigNo), p->OrigNo, sizeof(p->Msg), p->Msg);
		return;
	}

	char zSymbol[64], zOrdNo[64], zRemainQty[64], zOrdPrc[64], zDT[64], zTM[64];

	sprintf(zSymbol, "%.*s", sizeof(p->Symbol), p->Symbol);
	sprintf(zOrdNo, "%.*s", sizeof(p->OrdNo), p->OrdNo);
	sprintf(zRemainQty, "%.*s", sizeof(p->RemainQty), p->RemainQty);
	sprintf(zOrdPrc, "%.*s", sizeof(p->OrdPrc), p->OrdPrc);
	sprintf(zDT, "%.*s", sizeof(p->DT), p->DT);
	sprintf(zTM, "%.*s", sizeof(p->TM), p->TM);
		
	// NEW ORDER
	if (p->ReplyTp[0] == 'O' && p->OrdProcTp[0] == CD_ORD_PROC_NEW && p->OrdTp[0] == CD_ORD_TP_LIMIT)
	{
		if (atoi(zRemainQty) == 0)
		{
			//TODO. LOG
			return;
		}

		m_UserInfo.OrdInfo.SetOrdInfo(zSymbol, m_UserInfo.zAppliedStratID, p->OrdTp[0], zOrdNo, zOrdNo, zRemainQty, p->Side[0], zOrdPrc, zDT, zTM);
		m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, " ", "�ű��ֹ�����.(#:%.*s)(����:%.*s)(SIDE:%c)(PRC:%s)"
			, sizeof(p->OrdNo), p->OrdNo, sizeof(p->RemainQty), p->RemainQty,p->Side[0], zOrdPrc);
	}

	// CANCEL ORDER
	if (p->ReplyTp[0] == 'O' && p->OrdProcTp[0] == CD_ORD_PROC_CNCL && p->OrdTp[0]==CD_ORD_TP_LIMIT)
	{
		char zOrigNo[32];
		sprintf(zOrigNo, "%.*s", sizeof(p->OrigNo), p->OrigNo);
		int nCnt = m_UserInfo.OrdInfo.DelLimitOrdInfoByOrdNo(zOrigNo);
		m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, " ", "�ֹ����.(��#:%s).���ó������:%d", zOrigNo, nCnt);
	}

}

// ü�� real �� ���� �� ����
VOID CUserStratOrd::ClientDataRealExec(char* pRecvData)
{
	if (!IsStratApplied())
		return;
	PACK_C_ORDCNTR* p = (PACK_C_ORDCNTR*)pRecvData;

	// �ź�ó��
	if (p->ReplyTp[0] == 'R')
	{
		//TODO. LOG
		//g_log.log(LOGTP_ERR, "�ֹ��źμ���[%s][���ֹ���ȣ:%d]", sSymbol.GetBuffer(), nOrigNo);
		return;
	}

	char temp[128];
	char zSymbol[64], zOrdNo[64], zRemainQty[64], zOrdPrc[64], zAvgPrc[64], zDT[64], zTM[64];

	sprintf(zSymbol, "%.*s", sizeof(p->Symbol), p->Symbol);
	sprintf(zOrdNo, "%.*s", sizeof(p->OrdNo), p->OrdNo);
	sprintf(zRemainQty, "%.*s", sizeof(p->RemainQty), p->RemainQty);
	sprintf(zOrdPrc, "%.*s", sizeof(p->OrdPrc), p->OrdPrc);
	sprintf(zAvgPrc, "%.*s", sizeof(p->AvgPrc), p->AvgPrc);
	sprintf(zDT, "%.*s", sizeof(p->DT), p->DT);
	sprintf(zTM, "%.*s", sizeof(p->TM), p->TM);

	// ü������ ���� �ֹ��Ϸῡ ���� ���� ����
	INT nCnt = m_UserInfo.OrdInfo.DelLimitOrdInfoByOrdNo(zOrdNo);
	
	int nOpenQty = S2N(p->OpenQty, sizeof(p->OpenQty));
	char zChartNm[32];
	char zTradeTm[32], zTradeDT[32];
	sprintf(zTradeTm, "%.8s", p->TM);
	sprintf(zTradeDT, "%.8s", p->DT);
	GET_CHART_NM_EX(zTradeDT, zTradeTm, m_UserInfo.ChartTp, zChartNm);

	// ����. ������ ��� 1 �̹Ƿ� �Ϻ�û���� ���ٰ� ����.
	if(nOpenQty>0)
	{
		BOOL bAlreadyOpened = m_UserInfo.PosInfo.IsOpened(zSymbol);
		if (bAlreadyOpened) {
			//TODO. LOGGING. �̹� ���Ե� ���¿��� û���� �ƴ� ü���� ���Դ�.
			// �����ܰ����, ��� ���
			// go on
		}

		// �������Ժ� ����
		strcpy(m_UserInfo.zLastEntryChartNm, zChartNm);

		// ���� ���� ���� ������, MIT �ֹ� ���
		Cncl_LimitOrd(zSymbol, p->Side[0], 0, 'S');
		Cncl_MITOrd(zSymbol, p->Side[0], FALSE);

		// �ð�����
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_OPENPRC, strlen(STRATID_FB_ORD_OPENPRC)) != 0)
		{
			// ������ ADD
			m_UserInfo.PosInfo.SetPosInfoReal(zSymbol, nOpenQty, p->OpenSide[0], zAvgPrc, zTradeDT, zTradeTm, (CHART_TP)m_UserInfo.ChartTp);
			
			// ���� ������(20 TICK)
			double dOrdPrc;
			char cPlusMinus, cOrdSide;
			if (p->Side[0] == 'S') {
				cPlusMinus = '-';
				cOrdSide = 'B';
			}
			else {
				cPlusMinus = '+';
				cOrdSide = 'S';
			}
			dOrdPrc = CUtil::GetPrcByTick(zOrdPrc, 20, m_UserInfo.SymbolInfo.dTickSize, cPlusMinus);
			FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, temp);
			strcpy(zOrdPrc, temp);

			m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, "", "�ð���ġ�� ���� ü�� ���� �� ���������� ����");


			// ���������� �ֹ�����. 
			SendSaveStratOrd(CD_ORD_TP_LIMIT, CD_ORD_PROC_NEW,zOrdPrc, zOrdPrc, cOrdSide, m_UserInfo.nEntryOrdQty, 'N', 0,  'Y', FALSE, "");

			// ���� MIT(20 TICK)
			if (p->Side[0] == 'S')	cPlusMinus = '+';
			if (p->Side[0] == 'B')	cPlusMinus = '-';
			dOrdPrc = CUtil::GetPrcByTick(zOrdPrc, 20, m_UserInfo.SymbolInfo.dTickSize, cPlusMinus);
			FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, temp);
			strcpy(zOrdPrc, temp);

			m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, "", "�ð���ġ�� ���� ü�� ���� �� ����MIT ����");

			// ����MIT �ֹ�����
			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW,zOrdPrc, zOrdPrc, cOrdSide, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, "");
		}
		// ������ǥ����
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_ASSISTPRC, strlen(STRATID_FB_ORD_ASSISTPRC)) != 0)
		{
			//TODO
		}
		// 10���� ����
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_CROSSMA, strlen(STRATID_FB_ORD_CROSSMA)) != 0)
		{
			//TODO
		}

		// Ⱦ������
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_SIDEWAY, strlen(STRATID_FB_ORD_SIDEWAY)) != 0)
		{
			//TODO
		}
	}
	else // û��
	{
		// ����û��� ����
		strcpy(m_UserInfo.zLastClrChartNm, zChartNm);

		// ���� ��� ���� ������, MIT �ֹ� ���
		Cncl_LimitOrd(zSymbol, 0x20, 0, 'A');
		Cncl_MITOrd(zSymbol, 0x20, TRUE);

		// ������ CLEAR
		m_UserInfo.PosInfo.DelPosInfo(zSymbol);

		// �ð�����
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_OPENPRC, strlen(STRATID_FB_ORD_OPENPRC)) != 0)
		{

		}
		// ������ǥ����
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_ASSISTPRC, strlen(STRATID_FB_ORD_ASSISTPRC)) != 0)
		{
			//TODO
		}
		// 10���� ����
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_CROSSMA, strlen(STRATID_FB_ORD_CROSSMA)) != 0)
		{
			//TODO
		}

		// Ⱦ������
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_SIDEWAY, strlen(STRATID_FB_ORD_SIDEWAY)) != 0)
		{
			//TODO
		}
	}

}




// TR �� ��ȸ�� ������ ���� ����
VOID CUserStratOrd::ClientDataTrPos(char* pRecvData)
{
	PACK_C_MULTIRECORDS* pRecv = (PACK_C_MULTIRECORDS*)pRecvData;
	int nCnt = S2N(pRecv->Header.ArrayCnt, sizeof(pRecv->Header.ArrayCnt));
	char* pIn = pRecv->Body;
	char zSymbol[128], zOpenQty[128], zPosPrc[128], zOpenTM[128];
	char cSide;
	
	for (int i = 0; i < nCnt; i++)
	{
		PACK_C_INNERNCLR* p = (PACK_C_INNERNCLR*)(pIn + (sizeof(PACK_C_INNERNCLR)*i));
		sprintf(zSymbol, "%.*s", sizeof(p->Symbol), p->Symbol);
		sprintf(zOpenQty, "%.*s", sizeof(p->Qty), p->Qty);
		sprintf(zPosPrc, "%.*s", sizeof(p->AvgPrc), p->AvgPrc);
		sprintf(zOpenTM, "%.*s", sizeof(pRecv->Header.TM), pRecv->Header.TM);
		cSide = p->Side[0];
		//SetPosInfoTR(char* pzSymbol, char* pzOpenQty, char cSide, char* pzPosPrc, char* pzOpenTM, CHART_TP tp);
		m_UserInfo.PosInfo.SetPosInfoTR(zSymbol, zOpenQty, cSide, zPosPrc, zOpenTM, m_UserInfo.ChartTp);
	}
}


// TR �� ��ȸ�� ��ü�� ���� ����
VOID CUserStratOrd::ClientDataTrOrd(char* pRecvData)
{
	PACK_C_MULTIRECORDS* pRecv = (PACK_C_MULTIRECORDS*)pRecvData;
	int nCnt = S2N(pRecv->Header.ArrayCnt, sizeof(pRecv->Header.ArrayCnt));
	char* pIn = pRecv->Body;
	char zSymbol[128], zOrdNo[128], zOrigNo[128], zQty[128], zPrc[128], zDT[128], zTM[128];
	char cSide;

	for (int i = 0; i < nCnt; i++)
	{
		PACK_C_INNERORD* p = (PACK_C_INNERORD*)(pIn + (sizeof(PACK_C_INNERORD)*i));
		sprintf(zSymbol, "%.*s", sizeof(p->Symbol), p->Symbol);
		sprintf(zOrdNo, "%.*s", sizeof(p->OrdNo), p->OrdNo);
		sprintf(zOrigNo, "%.*s", sizeof(p->OrigNo), p->OrigNo);
		sprintf(zQty, "%.*s", sizeof(p->Qty), p->Qty);
		cSide = p->Side[0];
		sprintf(zPrc, "%.*s", sizeof(p->OrdPrc), p->OrdPrc);
		sprintf(zDT, "%.*s", sizeof(p->OrdDT), p->OrdDT);
		sprintf(zTM, "%.*s", sizeof(p->OrdTM), p->OrdTM);
		
		m_UserInfo.OrdInfo.SetOrdInfo(zSymbol, m_UserInfo.zAppliedStratID, p->OrdTp[0], zOrdNo, zOrigNo, zQty, cSide, zPrc, zDT, zTM);
	}
}

VOID CUserStratOrd::StratOrdWrapper(char* pSignalData)
{
	__try
	{
		__try
		{

			StratOrd(pSignalData);
		}
		__except (ReportException(GetExceptionCode(), "CUserStratOrd::StratOrdWrapper", m_szMsg))
		{
			g_log.log(LOGTP_FATAL, m_szMsg);
		}

	}
	__finally
	{ }
}


VOID CUserStratOrd::StratOrd(char* pSignalData)
{
	if (!IsStratApplied())
		return;

	PACK_HEADER_S2C* p = (PACK_HEADER_S2C*)pSignalData;

	ZeroMemory(&m_FlowInfo, sizeof(m_FlowInfo));

	m_FlowInfo.pSigData = pSignalData;
	sprintf(m_FlowInfo.zSymbol, "%.*s", sizeof(p->Symbol), p->Symbol);
	FORMAT_SYMBOL(m_FlowInfo.zSymbol);
	sprintf(m_FlowInfo.zGroupKey, "%.*s", LEN_SHM_GROUP_KEY, p->GroupKey);

	// �������ɺ��� �ƴϸ� ������.
	if (strcmp(m_UserInfo.zSymbol, m_FlowInfo.zSymbol) != 0) {
		//TODO. LOG
		return;
	}


	if (GetLastChartShm(m_FlowInfo.zGroupKey, m_FlowInfo.zChartNm, m_FlowInfo.ChartPrev, m_FlowInfo.ChartNow) < 0)
	{
		//TODO. LOG
		return;
	}

	if (strncmp(p->Code, CD_SIG_OPENPRC, sizeof(p->Code)) == 0) {
		Strat_OpenPrc();
		Strat_Multi_OpenAssist();
		Strat_Multi_OpenAssistClrOrd();
	}
	if (strncmp(p->Code, CD_SIG_ASSIST, sizeof(p->Code)) == 0) {
		Strat_AssistPrc();
		Strat_Multi_OpenAssist();
		Strat_Multi_OpenAssistClrOrd();
	}
	if (strncmp(p->Code, CD_SIG_MACROSS, sizeof(p->Code)) == 0)
		Strat_CrossMA();
	if (strncmp(p->Code, CD_SIG_SIDEWAY, sizeof(p->Code)) == 0)
		Strat_SideWay();



}


/*
	1) signal ����
	2) ����ð���ġ : �ż�, �����ð���ġ : �ŵ�
	3) ����ũ�ν� ���� ����
	  - ����ũ�ν� : �ż��� ���԰���
	  - ���ũ�ν� : �ŵ��� ���԰���
*/
VOID CUserStratOrd::Strat_OpenPrc()
{
	PACK_STRAT_FB_OPENPRC* p = (PACK_STRAT_FB_OPENPRC*)m_FlowInfo.pSigData;
	
	char cOrdSide = 0x00;
	char zOrdPrc[32];
	//BOOL bSendOrd = FALSE;

	// ����SIGNAL ���� ����
	char zPrc[32]; 
	sprintf(zPrc, "%.*s", sizeof(p->StratPrc), p->StratPrc);
	strcpy(m_UserInfo.zLastStratPrc, zPrc);

	// if �̹� ����
	if (m_UserInfo.PosInfo.IsOpened(m_FlowInfo.zSymbol))
	{
		// ���Ժ��� ������� ������ û������ �ʴ´�.
		if (m_UserInfo.PosInfo.IsSameChart(m_FlowInfo.zSymbol, m_FlowInfo.zChartNm)) {
			m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, FALSE, "�̹� �������̰�, ���Ժ��� ������� �����Ƿ� û������ �ʴ´�.");
			return;
		}

		//TODO. ???���� ���������� ����, ��� ��� ��ġ�ص� û���� �߾��µ� �̰�???

		// �ż������̸� �ŵ��ֹ�û��, vice versa
		// ������ �ŵ��ֹ�, ����� �ż��ֹ�
		if (m_UserInfo.PosInfo.PosSide(m_FlowInfo.zSymbol) == CD_BUY && p->ChartGb[0] == CD_MINUS_CANDLE)
		{
			cOrdSide = CD_SELL;
		}
		if (m_UserInfo.PosInfo.PosSide(m_FlowInfo.zSymbol) == CD_SELL && p->ChartGb[0] == CD_PLUS_CANDLE)
		{
			cOrdSide = CD_BUY;
		}
		if (cOrdSide == 0x00)
		{
			m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, FALSE,
				"û���ֹ�����SKIP.��������SIDE[%c] ��ŶSIG��Ʈ����[%c]", m_UserInfo.PosInfo.PosSide(m_FlowInfo.zSymbol), p->ChartGb[0]);
			return;
		}
		
		// �����ֹ� ���ó��
		m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE, "û���� ��������ֹ� ��� ����");
		Cncl_LimitOrd(m_UserInfo.zSymbol, 0x20, 0, 'A');
		Cncl_MITOrd(m_UserInfo.zSymbol, 0x20, TRUE);

		m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE, "û���ֹ�����");

		// �ֹ����� : �ð�
		sprintf(zOrdPrc, "%.*s", sizeof(m_FlowInfo.ChartNow.open), m_FlowInfo.ChartNow.open);

		SendSaveStratOrd(
			CD_ORD_TP_MIT,
			CD_ORD_PROC_NEW,
			zOrdPrc,	// char* pzPrc, 
			zOrdPrc,	// char* pzOrgPrc,
			cOrdSide,
			m_UserInfo.nEntryOrdQty,
			'Y',		//char cClrYN,
			0,			//int nOrigNo,
			'N',		//char cApiYN,
			FALSE,		//BOOL bCnclAllQty
			m_FlowInfo.ChartNow.Nm
		);

		return;
	} // if( m_UserInfo.PosInfo.IsOpened(m_FlowInfo.zSymbol))


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ԵǾ� ���� �ʴ�. 

	// û��������� �������� �ʴ´�.
	if (strncmp(m_FlowInfo.zChartNm, m_UserInfo.zLastClrChartNm, LEN_CHART_NM) == 0) {
		m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, FALSE, "����û����� ������� �����Ƿ� û������ �ʴ´�.");
		return;
	}


	// ũ�ν� ����
	CROSS_TP crossTp = GetCross(m_FlowInfo.ChartPrev);
	char zCross[32];
	CROSS2NAME(crossTp, zCross);
	cOrdSide = 0x00;
	// ����ũ�ν� => �ż���
	if( crossTp==DEAD_CROSS && p->ChartGb[0] == CD_PLUS_CANDLE)
	{
		cOrdSide = CD_BUY;
	}

	// ���ũ�ν� => �ŵ���
	if (crossTp == GOLDEN_CROSS && p->ChartGb[0] == CD_MINUS_CANDLE)
	{
		cOrdSide = CD_SELL;
	}

	if (cOrdSide == 0x00)
	{
		m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, FALSE,
			"�����ֹ�����SKIP.CROSS(%s) ��ŶSIG��Ʈ����[%c]", zCross, p->ChartGb[0]);
		return;
	}

	// �����ֹ� ���ó��
	m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE, "������ ��������ֹ� ��� ����");
	Cncl_LimitOrd(m_UserInfo.zSymbol, 0x20, 0, 'A');
	Cncl_MITOrd(m_UserInfo.zSymbol, 0x20, TRUE);
	
	m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE, "�����ֹ�����");

	// �ֹ����� : �ð�
	sprintf(zOrdPrc, "%.*s", sizeof(m_FlowInfo.ChartNow.open), m_FlowInfo.ChartNow.open);

	SendSaveStratOrd(
		CD_ORD_TP_MIT,
		CD_ORD_PROC_NEW,
		zOrdPrc,	// char* pzPrc, 
		zOrdPrc,	// char* pzOrgPrc,
		cOrdSide,
		m_UserInfo.nEntryOrdQty,
		'N',		//char cClrYN,
		0,			//int nOrigNo,
		'N',		//char cApiYN,
		FALSE,		//BOOL bCnclAllQty
		m_FlowInfo.ChartNow.Nm
	);
}


VOID CUserStratOrd::Strat_AssistPrc()
{
	PACK_STRAT_FB_ASSIST* p = (PACK_STRAT_FB_ASSIST*)m_FlowInfo.pSigData;

	//memcpy(&pClient->Header, &p->Header, sizeof(pClient->Header));
	//memcpy(pClient->Header.Code, CD_STRAT_ORD, sizeof(pClient->Header.Code));
	//pClient->Header.StratTp[0] = 'O';
	//pClient->Header.API_YN[0] = 'Y';
	//char temp[128];
	//NOWTIME_NONE_DOT(temp);
	//memcpy(p->Header.TM, temp, strlen(temp));

	if (p->ClrYN[0] == 'N')
		Strat_AssistPrcSet();
	else
		Strat_AssistPrcClr();
}


// ������ǥ ������ ���� �ֹ�
VOID CUserStratOrd::Strat_AssistPrcSet()
{
	PACK_STRAT_FB_ASSIST* p = (PACK_STRAT_FB_ASSIST*)m_FlowInfo.pSigData;
	//PACK_STRAT_ORD* pClient = (PACK_STRAT_ORD*)m_FlowInfo.zClientBuf;

	m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE,"������ǥ���� ���� �� ���MIT ���");
		
	// ���� ������ȣ�� ���� �������� ���ó�� 
	// ==> �׷���, ��� ������ �� �ϳ��� ������ �����Ƿ� �׳� ���ó�� �Ѵ�.
	Cncl_MITOrd(m_UserInfo.zSymbol, 0x20, TRUE);
	

	//double dOrdPrc;
	char zPrc[32];
	char zChartNm[32];
	sprintf(zChartNm, "%.*s", LEN_CHART_NM, p->ChartNm);

	sprintf(zPrc, "%.*s", sizeof(p->StratPrc), p->StratPrc);
	double dAssistPrc = atof(zPrc);

	// ���� ������ǥ���� ����
	strcpy(m_UserInfo.zLastStratPrc, zPrc);

	// ũ�ν� ����
	CROSS_TP crossTp = GetCross(m_FlowInfo.ChartPrev);

	if (m_UserInfo.bCrossApplied )
	{
		// ����ũ�ν� => �ż���
		if (crossTp == DEAD_CROSS && p->ChartGb[0] == CD_PLUS_CANDLE)
		{
			// ���� : ������ǥ + 1ƽ
			double dOrdPrc = dAssistPrc + (1 * m_UserInfo.SymbolInfo.dTickSize);
			FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);
			// ������ȣ < �ð� �� ���� �ֹ����� �ʴ´�.
			// TODO. ���� �������� �־���. �����ؾ� �ϳ�?
			//if (CUtil::CompPrc(zPrc, LEN_PRC, m_FlowInfo.ChartNow.open, LEN_PRC, m_UserInfo.SymbolInfo.nDotCnt, LEN_PRC) < 0)
			//{
			//	//TODO. JUST LOGGING
			//	return;
			//}

			m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE,
				"������ǥSET(%s).�ż�.����ũ�ν�(������SMA:%.*s < ������LMA:%.*s)",
				p->StratPrc, LEN_PRC, m_FlowInfo.ChartPrev.sma_short, LEN_PRC, m_FlowInfo.ChartPrev.sma_short);

			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_BUY, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
		}

		// ���ũ�ν� => �ŵ���
		if (crossTp == GOLDEN_CROSS && p->ChartGb[0] == CD_MINUS_CANDLE)
		{

			// ���� : ������ǥ - 1ƽ
			double dOrdPrc = dAssistPrc - (1 * m_UserInfo.SymbolInfo.dTickSize);
			FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);

			// ������ȣ > �ð� �� ���� �ֹ����� �ʴ´�.
			// TODO. ���� �������� �־���. �����ؾ� �ϳ�?
			//if (CUtil::CompPrc(zPrc, LEN_PRC, m_FlowInfo.ChartNow.open, LEN_PRC, m_UserInfo.SymbolInfo.nDotCnt, LEN_PRC) > 0)
			//{
			//	//TODO. JUST LOGGING
			//	return;
			//}

			m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE,
				"������ǥSET(%s).�ŵ�.���ũ�ν�(������SMA:%.*s > ������LMA:%.*s)",
				p->StratPrc, LEN_PRC, m_FlowInfo.ChartPrev.sma_short, LEN_PRC, m_FlowInfo.ChartPrev.sma_short);

			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_SELL, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
		}
	} // if (m_UserInfo.bCrossApplied )
	else
	{
		if (p->ChartGb[0] == CD_PLUS_CANDLE)
		{
			// ���� : ������ǥ + 1ƽ
			double dOrdPrc = dAssistPrc + (1 * m_UserInfo.SymbolInfo.dTickSize);
			FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);
			// ������ȣ < �ð� �� ���� �ֹ����� �ʴ´�.
			// TODO. ���� �������� �־���. �����ؾ� �ϳ�?
			//if (CUtil::CompPrc(zPrc, LEN_PRC, m_FlowInfo.ChartNow.open, LEN_PRC, m_UserInfo.SymbolInfo.nDotCnt, LEN_PRC) < 0)
			//{
			//	//TODO. JUST LOGGING
			//	return;
			//}

			m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE,
				"������ǥSET(%s).�ż�.NOCROSS.���", p->StratPrc);

			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_BUY, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
		}

		if (p->ChartGb[0] == CD_MINUS_CANDLE)
		{

			// ���� : ������ǥ - 1ƽ
			double dOrdPrc = dAssistPrc - (1 * m_UserInfo.SymbolInfo.dTickSize);
			FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);

			// ������ȣ > �ð� �� ���� �ֹ����� �ʴ´�.
			// TODO. ���� �������� �־���. �����ؾ� �ϳ�?
			//if (CUtil::CompPrc(zPrc, LEN_PRC, m_FlowInfo.ChartNow.open, LEN_PRC, m_UserInfo.SymbolInfo.nDotCnt, LEN_PRC) > 0)
			//{
			//	//TODO. JUST LOGGING
			//	return;
			//}

			m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE,
				"������ǥSET(%s).�ŵ�.NOCROSS.����", p->StratPrc);

			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_SELL, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
		}
	}


}



// ������ǥ û�� �ֹ�
VOID CUserStratOrd::Strat_AssistPrcClr()
{
	PACK_STRAT_FB_ASSIST* p = (PACK_STRAT_FB_ASSIST*)m_FlowInfo.pSigData;

	// �������� ������ ������.
	if (!m_UserInfo.PosInfo.IsOpened(m_UserInfo.zSymbol))
	{
		m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol,m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, FALSE, "û���ֹ�����SKIP.�����Ǿ���");
		return;
	}

	// �̹� ������ ������ǥ ������ ������ ������.
	if (m_UserInfo.zLastStratPrc[0] == 0x00)
	{
		m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, FALSE, 
			"û���ֹ�����SKIP.������ ������ǥ ������ ����.");
		return;
	}

	char zPrc[64];
	char zAssistPrc[128];
	sprintf(zAssistPrc, "%.*s", LEN_PRC, p->StratPrc);

	char cPosSide = m_UserInfo.PosInfo.PosSide(m_FlowInfo.zSymbol);

	// �ż������� & �ر� => �ŵ�û�� �ֹ�
	if (cPosSide == CD_BUY && p->ClrCrossTp[0] == 'D')
	{
		// ����MIT ���
		Cncl_MITOrd(m_UserInfo.zSymbol, 0x20, TRUE);

		// TODO.�������� ����ؾ� �ϳ�?

		// ���� : ������ǥ - 1ƽ
		double dOrdPrc = atof(zAssistPrc) - (1 * m_UserInfo.SymbolInfo.dTickSize);
		FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);

		// û���ֹ�����.MIT.
		SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_SELL, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
	}

	// �ŵ������� & ���� => �ż�û�� �ֹ�
	if (cPosSide == CD_SELL && p->ClrCrossTp[0] == 'U')
	{
		// ����MIT ���
		Cncl_MITOrd(m_UserInfo.zSymbol, 0x20, TRUE);

		// TODO.�������� ����ؾ� �ϳ�?

		// ���� : ������ǥ + 1ƽ
		double dOrdPrc = atof(zAssistPrc) + (1 * m_UserInfo.SymbolInfo.dTickSize);
		FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);

		// û���ֹ�����.MIT.
		SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_BUY, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
	}
}

/*
	��ȣ�� ���� �� ���� ������ ������ ���Է���, ������ û�����
*/
VOID CUserStratOrd::Strat_CrossMA()
{
	PACK_STRAT_FB_CROSSMA* p = (PACK_STRAT_FB_CROSSMA*)m_FlowInfo.pSigData;
	//PACK_STRAT_ORD* pClient = (PACK_STRAT_ORD*)m_FlowInfo.zClientBuf;

	//memcpy(&pClient->Header, &p->Header, sizeof(pClient->Header));
	//memcpy(pClient->Header.Code, CD_STRAT_ORD, sizeof(pClient->Header.Code));
	//pClient->Header.StratTp[0] = 'O';
	//pClient->Header.API_YN[0] = 'N';
	//char temp[128];
	//NOWTIME_NONE_DOT(temp);
	//memcpy(p->Header.TM, temp, strlen(temp));

	char zMAPrc[32];
	sprintf(zMAPrc, "%.*s", sizeof(p->MAPrice), p->MAPrice);

	// ũ�ν� ����
	CROSS_TP crossTp = GetCross(m_FlowInfo.ChartPrev);


	// ������ ����. ����
	if (!m_UserInfo.PosInfo.IsOpened(m_FlowInfo.zSymbol))
	{
		//memcpy(pClient->OrdTp, CD_ORD_TP_MIT, strlen(CD_ORD_TP_MIT));
		//memcpy(pClient->OrdProcTp, CD_ORD_PROC_NEW, strlen(CD_ORD_PROC_NEW));
		//pClient->ClrYN[0] = 'N';
		//sprintf(temp, "%-*.*s", LEN_QTY, LEN_QTY, "1");
		//memcpy(pClient->OrdQty, temp, strlen(temp));

		// ����ũ�ν� + ���� => �ż�
		if (crossTp==DEAD_CROSS && p->Tp[0] == 'U')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_BUY;
			//double dPrc = atof(zMAPrc) + (1 * m_SymbolInfo.dTickSize);	// MA +  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. ����
		}

		// ���ũ�ν� + �ر� => �ŵ�
		if (crossTp == GOLDEN_CROSS && p->Tp[0] == 'D')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_SELL;
			//double dPrc = atof(zMAPrc) - (1 * m_SymbolInfo.dTickSize);	// MA -  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. ����
		}
	}

	// û��
	else if (m_UserInfo.PosInfo.IsOpened(m_FlowInfo.zSymbol))
	{
		//memcpy(pClient->OrdTp, CD_ORD_TP_MIT, strlen(CD_ORD_TP_MIT));
		//memcpy(pClient->OrdProcTp, CD_ORD_PROC_NEW, strlen(CD_ORD_PROC_NEW));
		//pClient->ClrYN[0] = 'Y';
		//sprintf(temp, "%-*.*s", LEN_QTY, LEN_QTY, "1");
		//memcpy(pClient->OrdQty, temp, strlen(temp));

		char cPosSide = m_UserInfo.PosInfo.PosSide(m_FlowInfo.zSymbol);

		// SHORT + ���� => �ż�û��
		if (cPosSide ==CD_SELL && p->Tp[0] == 'U')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_BUY;
			//double dPrc = atof(zMAPrc) + (1 * m_SymbolInfo.dTickSize);	// MA +  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. ����
		}

		// LONG + �ر� => �ŵ�û��
		if (cPosSide == CD_BUY && p->Tp[0] == 'D')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_SELL;
			//double dPrc = atof(zMAPrc) - (1 * m_SymbolInfo.dTickSize);	// MA -  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. ����
		}
	}
}


VOID CUserStratOrd::Strat_SideWay()
{
	PACK_STRAT_FB_SIDEWAY* p = (PACK_STRAT_FB_SIDEWAY*)m_FlowInfo.pSigData;
	//PACK_STRAT_ORD* pClient = (PACK_STRAT_ORD*)m_FlowInfo.zClientBuf;

	//memcpy(&pClient->Header, &p->Header, sizeof(pClient->Header));
	//memcpy(pClient->Header.Code, CD_STRAT_ORD, sizeof(pClient->Header.Code));
	//pClient->Header.StratTp[0] = 'O';
	//pClient->Header.API_YN[0] = 'N';
	//char temp[128];
	//NOWTIME_NONE_DOT(temp);
	//memcpy(p->Header.TM, temp, strlen(temp));

	char zStratPrc[32];
	sprintf(zStratPrc, "%.*s", sizeof(p->StratPrc), p->StratPrc);


	// ������ ����. ����
	if (!m_UserInfo.PosInfo.IsOpened(m_FlowInfo.zSymbol))
	{
		//memcpy(pClient->OrdTp, CD_ORD_TP_MIT, strlen(CD_ORD_TP_MIT));
		//memcpy(pClient->OrdProcTp, CD_ORD_PROC_NEW, strlen(CD_ORD_PROC_NEW));
		//pClient->ClrYN[0] = 'N';
		//sprintf(temp, "%-*.*s", LEN_QTY, LEN_QTY, "1");
		//memcpy(pClient->OrdQty, temp, strlen(temp));

		// ���� => �ż�
		if (p->Tp[0] == 'U')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_BUY;
			//double dPrc = atof(zStratPrc) + (1 * m_SymbolInfo.dTickSize);	// MA +  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. ����
		}

		// �ر� => �ŵ�
		if (p->Tp[0] == 'D')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_SELL;
			//double dPrc = atof(zStratPrc) - (1 * m_SymbolInfo.dTickSize);	// MA -  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. ����
		}
	}

	// û��
	else if (m_UserInfo.PosInfo.IsOpened(m_FlowInfo.zSymbol))
	{
		//memcpy(pClient->OrdTp, CD_ORD_TP_MIT, strlen(CD_ORD_TP_MIT));
		//memcpy(pClient->OrdProcTp, CD_ORD_PROC_NEW, strlen(CD_ORD_PROC_NEW));
		//pClient->ClrYN[0] = 'Y';
		//sprintf(temp, "%-*.*s", LEN_QTY, LEN_QTY, "1");
		//memcpy(pClient->OrdQty, temp, strlen(temp));

		char cPosSide = m_UserInfo.PosInfo.PosSide(m_FlowInfo.zSymbol);

		// SHORT + ���� => �ż�û��
		if (cPosSide == CD_SELL && p->Tp[0] == 'U')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_BUY;
			//double dPrc = atof(zStratPrc) + (1 * m_SymbolInfo.dTickSize);	// MA +  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. ����
		}

		// LONG + �ر� => �ŵ�û��
		if (cPosSide == CD_BUY && p->Tp[0] == 'D')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_SELL;
			//double dPrc = atof(zStratPrc) - (1 * m_SymbolInfo.dTickSize);	// MA -  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. ����
		}
	}

}


VOID CUserStratOrd::Strat_Multi_OpenAssist()
{
	if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_OPEN_ASSIST, strlen(STRATID_FB_ORD_OPEN_ASSIST)) != 0)
		return;
}


VOID CUserStratOrd::Strat_Multi_OpenAssistClrOrd()
{
	if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_OPEN_ASSIST_CLRORD, strlen(STRATID_FB_ORD_OPEN_ASSIST_CLRORD)) != 0)
		return;
}


/*
	Post message to main thread to send data to client
*/
VOID CUserStratOrd::SendSaveStratOrd(
	char cOrdTp, 
	char cOrdProcTp, 
	char* pzPrc, 
	char* pzOrigPrc,
	char cSide, 
	int nQty, 
	char cClrYN, 
	int nOrigNo,
	char cApiYN,
	BOOL bCnclAllQty,
	char* psChartNm
)
{
	char* pData = m_pMemPool->get();
	PACK_STRAT_ORD* p = (PACK_STRAT_ORD*)pData;
	memset(p, 0x20, sizeof(PACK_STRAT_ORD));
	*(pData + sizeof(PACK_STRAT_ORD)) = 0x00;

	char temp[128];

	// HEADER
	memcpy(p->Header.Code, CD_STRAT_ORD, strlen(CD_STRAT_ORD));
	memcpy(p->Header.UserID, m_UserInfo.zUserID, strlen(m_UserInfo.zUserID));
	memcpy(p->Header.StratCompID, m_UserInfo.zCompID, strlen(m_UserInfo.zCompID));
	p->Header.StratTp[0] = 'O'; // S:SIGNAL, O:ORDER, B:BOTH
	memcpy(p->Header.Symbol, m_UserInfo.zSymbol, strlen(m_UserInfo.zSymbol));
	//skip.GroupKey[LEN_GROUP_KEY];
	p->Header.API_YN[0] = cApiYN;

	NOWTIME_NONE_DOT(temp);
	memcpy(p->Header.TM, temp, strlen(temp));// HHMMSSmmm

	// BODY
	p->Side[0] = cSide;	// B, S
	p->OrdTp[0] = cOrdTp;	// 1-������, 2-���尡, 3-����, 4-����
	p->OrdProcTp[0] = cOrdProcTp;	// 1-�ű�, 2-����, 3-���, 4-�ź�, A - �������, B - ���������, C - ����û��, D - ������û��
	p->ClrYN[0] = cClrYN;		// û���ֹ�����
	memcpy(p->OrdPrc, pzPrc, strlen(pzPrc));

	char zOrdQty[32];
	sprintf(zOrdQty, "%d", nQty);
	memcpy(p->OrdQty, zOrdQty, strlen(zOrdQty));

	sprintf(temp, "%d", nOrigNo);
	memcpy(p->OrigOrdNo, temp, strlen(temp));
	memcpy(p->OrigPrcCncl, pzOrigPrc,strlen(pzOrigPrc));	//����,���� ��ҽ� ���ֹ� ����
	if (cOrdProcTp == CD_ORD_PROC_CNCL)
	{
		if (bCnclAllQty)	p->CnclAllQtyYN[0] = 'Y';
		else				p->CnclAllQtyYN[0] = 'N';
	}
	int nLen = sizeof(PACK_STRAT_ORD);
	PostThreadMessage(m_dwSendThread, WM_SEND_STRATEGY, (WPARAM)nLen, (LPARAM)pData);

	char *pData2 = m_pMemPool->get();
	strcpy(pData2, pData);
	PostThreadMessage(m_dwSaveThread, WM_SEND_STRATEGY, (WPARAM)nLen, (LPARAM)pData2);

	// MIT �� ���� ��ü ����
	if (cOrdTp == CD_ORD_TP_MIT)
	{
		m_UserInfo.OrdInfo.SetOrdInfo(m_UserInfo.zSymbol, m_UserInfo.zAppliedStratID, CD_ORD_TP_MIT, "", "",
			zOrdQty, cSide, pzPrc, "", "");
	}
	
	// LOGGING
	sprintf(temp, "%.*s", LEN_CHART_NM, psChartNm);
	//(char* psStratCd, char* pzSymbol, char* pzChartNm, char cOrdTp, char cOrdProcTp, char* pzPrc, char* pzOrgPrc, char* pMsg, ...);
	m_pStratLog->logOrd(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, temp, cOrdTp, cOrdProcTp, pzPrc, pzOrigPrc,
		"�ֹ�����(%s)", pData);
}


// ���� ���� ���� ��Ʈ�� ���� ��Ʈ ��������
INT CUserStratOrd::GetLastChartShm(char* pzGroupKey, char* pzChartNm, ST_SHM_CHART_UNIT& shmPrev, ST_SHM_CHART_UNIT& shmNow)
{
	char szChart[LEN_CHART_NM + 1];

	// ���� ��Ʈ ���� ��������
	BOOL bExist = m_pShmQ->DataGet(pzGroupKey, pzChartNm, (char*)&shmNow);

	// 1. ���� ��Ʈ�� ���� ��� ==> ������.
	if (FALSE == bExist)
	{
		// ���ݽð� ��Ʈ�� ���µ�, ���ݽð� ü�ᰡ�� ���� ��Ȳ�̶�� ��Ʈ �̻��̴�.
		g_log.log(LOGTP_ERR, "[%s][%s] ���簡�����ߴµ� ������Ʈ�� ����.", pzGroupKey, pzChartNm);
		return -1;
	}


	// 2. ���� ��Ʈ�� �ִ� ��� ���� ��Ʈ ��������	  
	sprintf(szChart, "%.*s", LEN_CHART_NM, shmNow.prevNm);
	if (strcmp(szChart, "NONE") == 0)
		return -3;

	if (FALSE == m_pShmQ->DataGet(pzGroupKey, szChart, (char*)&shmPrev))
	{
		g_log.log(LOGTP_ERR, "(%s)(%.*s) ���� ��Ʈ�� SHM �� ����", pzGroupKey, LEN_CHART_NM, szChart);
		return -2;
	}
	//g_log.log(LOGTP_SUCC, "������Ʈ(%.4s)", chart[0].Nm);


	return 1;
}



/*

typedef struct _USER_STRAT_INFO
{
char	zCompID[LEN_STRAT_COMP_ID+1];
char	zUserID[LEN_USERID + 1];
BOOL	bStratApplied;
char	zSymbol[LEN_SYMBOL + 1];			// �����ɺ�
char	zAppliedStratID[32];
CHART_TP ChartTp;
BOOL	bCrossApplied;
char	zLastClrChartNm[LEN_CHART_NM + 1];		// ����û���
char	zLastEntryChartNm[LEN_CHART_NM + 1];	// �������Ժ�
int		nEntryOrdQty;	// ���Ա⺻����
char	zLastStratPrc[LEN_PRC + 1];				// ������������
CPosInfo	PosInfo;
COrdInfo	OrdInfo;
SYMBOL_INFO	SymbolInfo;
}USER_STRAT_INFO;
*/
BOOL	CUserStratOrd::LoadUserInfo(BOOL bReleaseStrat)
{
	ZeroMemory(&m_UserInfo, sizeof(m_UserInfo));
	//char sApplyYN[2];
	CDBHandlerAdo db(m_pDBPool->Get());

	char tmp[512];
	char sQ[1024];
	sprintf(sQ, "EXEC STRAT_GET_USERINFO_FB '%s'", m_UserInfo.zUserID );
	if (FALSE == db->ExecQuery(sQ))
	{
		g_log.log(LOGTP_ERR, "Load User Info failed(%s)(%s)", sQ, db->GetError());
		return FALSE;
	}

	/*
	USER_ID	STRAT_COMP_ID	STRAT_ID	STRAT_NM	USE_YN	ACCOUNT_NO	SYMBOL	ARTC_CODE	SYMBOL	TICK_SIZE	DOT_CNT	ENTRY_ORD_QTY
	JAY	FB	11	�ð���ġ	Y	00701000001	CLZ7	CL	CLZ7	0.01000	2	1
	BOOL	bStratApplied;
	CHART_TP ChartTp;
	char	zLastClrChartNm[LEN_CHART_NM + 1];		// ����û���
	char	zLastEntryChartNm[LEN_CHART_NM + 1];	// �������Ժ�
	int		nEntryOrdQty;	// ���Ա⺻����
	char	zLastStratPrc[LEN_PRC + 1];				// ������������
	CPosInfo	PosInfo;
	COrdInfo	OrdInfo;
	SYMBOL_INFO	SymbolInfo;
	*/
	if (db->IsNextRow())
	{
		db->GetStr("STRAT_COMP_ID", tmp);
		strcpy(m_UserInfo.zCompID, tmp);

		db->GetStr("STRAT_ID", tmp);
		strcpy(m_UserInfo.zAppliedStratID, tmp);
		
		db->GetStr("CROSS_APPLY_YN", tmp);
		m_UserInfo.bCrossApplied = (tmp[0]=='Y')?TRUE:FALSE;

		m_UserInfo.nEntryOrdQty = db->GetLong("ENTRY_ORD_QTY");


		db->GetStr("SYMBOL", tmp);
		strcpy(m_UserInfo.zSymbol, tmp);
		
		m_UserInfo.SymbolInfo.nDotCnt = db->GetLong("DOT_CNT");
		
		m_UserInfo.SymbolInfo.dTickSize = db->GetDouble("TICK_SIZE");
		
	} // if (db->NextRow())

	m_UserInfo.bStratApplied = (m_UserInfo.zAppliedStratID[0] == 0x00) ? FALSE : TRUE;

	m_UserInfo.ChartTp = TP_5MIN;

	return TRUE;
}


VOID CUserStratOrd::DoManagerNotify(void* pNotify)
{
	//char zTemp[1024];
	std::string sSymbol;

	ST_MANAGER_NOTIFY* p = (ST_MANAGER_NOTIFY*)pNotify;

	switch (p->nWorkTp)
	{
	case MNG_FB_ORDSYMBOL_ADD:	//7)FB �ڵ��ֹ����� ����
	case MGN_FB_STRAT_SET:		//10) FBƯ������ ������
		//sprintf(zMsg, "%s%s��������/������(%s)(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT, AcntNo(), p->zWorkVal);
		//g_log.log(LOGTP_SUCC, zMsg);
		//m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT, zMsg);

		//EnterCriticalSection(&m_csProtect);
		//ReleaseStrategy(FALSE/*DBSAVE*/, 'N', "������ �������� ���۾�");
		//Initialize();
		//LeaveCriticalSection(&m_csProtect);

		break;
	case MNG_FB_ORDSYMBOL_RELEASE:	// 8)FB �ڵ��ֹ����� ����
		//sprintf(zMsg, "%s%s��������(%s)(%s)", LOG1_ADMIN, LOG2_ADMIN_SYMBOL, AcntNo(), p->zWorkVal);
		//g_log.log(LOGTP_SUCC, zMsg);
		//m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_SYMBOL, zMsg);

		//EnterCriticalSection(&m_csProtect);
		//ReleaseSymbol(CString(p->zWorkVal));
		//LeaveCriticalSection(&m_csProtect);
		break;
	case MGN_FB_STRAT_RELEASE:	// 11) FBƯ������ ��������
		//sprintf(zMsg, "%s%s ������������(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT, AcntNo());
		//g_log.log(LOGTP_SUCC, zMsg);
		//m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT, zMsg);

		//EnterCriticalSection(&m_csProtect);
		//ReleaseStrategy(TRUE/*DB SAVE*/, 'N', "������ ����");
		//LeaveCriticalSection(&m_csProtect);
		break;
	case MGN_FB_CROSS_CHECK:
		//bCrossCheck = TRUE;
		//sprintf(zMsg, "%s%sũ�ν����� ����(%s)", LOG1_ADMIN, LOG2_ADMIN_CROSS, AcntNo());
		//g_log.log(LOGTP_SUCC, zMsg);
		//m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_CROSS, zMsg);
		break;
	//case MNG_FB_CROSS_UNCHECK:
	//	sprintf(zMsg, "%s%sũ�ν����� ����(%s)", LOG1_ADMIN, LOG2_ADMIN_CROSS, AcntNo());
	//	g_log.log(LOGTP_SUCC, zMsg);
	//	m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_CROSS, zMsg);
	//	bCrossCheck = FALSE;
		break;
	case MNG_FB_PROFIT_TICK:
	case MNG_FB_LOSS_TICK:
		//sprintf(zMsg, "%s%s�������� ƽ�� �� ����(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT_TICK, AcntNo());
		//g_log.log(LOGTP_SUCC, zMsg);
		//m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT_TICK, zMsg);
		//LoadAcntTradeInfo();
		break;
	default:
		g_log.log(LOGTP_ERR, "[�߸��� �۾�TP:%d]", p->nWorkTp);
		break;
	}

}



CROSS_TP CUserStratOrd::GetCross(const ST_SHM_CHART_UNIT& shmPrev)
{
	if (!IsCrossApplied())
		return NONE_CROSS;

	int nRet = CUtil::CompPrc(shmPrev.sma_short, sizeof(shmPrev.sma_short),
		shmPrev.sma_long, sizeof(shmPrev.sma_long), m_UserInfo.SymbolInfo.nDotCnt, LEN_PRC);
	char msg[512];
	CROSS_TP tp;

	double dShort = S2D((char*)shmPrev.sma_short, sizeof(shmPrev.sma_short));
	double dLong = S2D((char*)shmPrev.sma_long, sizeof(shmPrev.sma_long));
	if (dShort <= 0 || dLong <= 0)
	{
		sprintf(msg, "[GetCross]<NONE_CROSS-�������̻�> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
		tp = NONE_CROSS;
	}

	if (nRet == 0) {
		tp = NONE_CROSS;
		sprintf(msg, "[GetCross]<NONE_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
	}
	else if (nRet > 0) {
		tp = GOLDEN_CROSS;
		sprintf(msg, "[GetCross]<GOLDEN_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
	}
	else {
		tp = DEAD_CROSS;
		sprintf(msg, "[GetCross]<DEAD_CROSS> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
	}

	//g_log.log(LOGTP_SUCC,"%s(%s)", msg, AcntNo());
	return tp;
}




//BOOL CUserStratOrd::BeginSignals()
//{
//	m_pSigOpenPrc = new CSignalOpenPrc(m_pShmQ, (void*)m_pDBPool);
//	m_pSigOpenPrc->Begin();
//
//	m_pSigAssist = new CSignalAssist(m_pShmQ, (void*)m_pDBPool);
//	m_pSigAssist->Begin();
//
//	m_pSigCrossMA = new CSignalCrossMA(m_pShmQ, (void*)m_pDBPool);
//	m_pSigCrossMA->Begin();
//
//	m_pSigSideWay = new CSignalSideWay(m_pShmQ, (void*)m_pDBPool);
//	m_pSigSideWay->Begin();
//
//	return TRUE;
//}
//
//VOID CUserStratOrd::EndSignals()
//{
//	SAFE_DELETE(m_pSigOpenPrc);
//	SAFE_DELETE(m_pSigAssist);
//	SAFE_DELETE(m_pSigCrossMA);
//	SAFE_DELETE(m_pSigSideWay);
//}

