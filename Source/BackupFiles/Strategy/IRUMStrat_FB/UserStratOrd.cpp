
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
		g_log.log(LOGTP_FATAL, ">>>>>>>CHART SHM OPEN 에러(%s)(symbol:%s)(%s)", m_zShmNm, m_UserInfo.SymbolInfo.zSymbolInfo, m_pShmQ->GetErr());
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "SHM OPEN 성공(%s)", m_zMutexNm);

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
				// 관리자요청 처리. proceed manager's request
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

// client 에서 수신한 TR, REAL 등 처리
VOID CUserStratOrd::ClientDataProc(char* pRecvData)
{
	PACK_HEADER_C2S* pH = (PACK_HEADER_C2S*)pRecvData;

	// 내 정보가 아니면 버린다.
	if (!IsSameUserID(pH->UserID, LEN_USERID))
		return;

	char zSymbol[LEN_SYMBOL + 1];
	sprintf(zSymbol, "%.*s", sizeof(pH->Symbol), pH->Symbol);
	if (!IsMyStratSymbol(zSymbol)) {
		m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, " ", "수신받은 클라이언트패킷 종목이 내 전략종목과 다르다(수신종목:%s)", zSymbol);
		return;
	}

	if (pH->RsltTp[0] != 'S')
	{
		m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, " ", "수신받은 클라이언트패킷이 성공이 아니다.(결과:%c)", pH->RsltTp[0]);
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

// 주문 real 로 수신 및 관리
// mit 는 주문 전송시점에 이미 저장하고 있다. CLINET 는  MIT 에 대해서 리턴하지 않는다.
VOID CUserStratOrd::ClientDataRealOrd(char* pRecvData)
{
	PACK_C_ORDCNTR* p = (PACK_C_ORDCNTR*)pRecvData;
	
	// 거부처리
	if (p->ReplyTp[0] == 'R')
	{
		m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, " ", "거부주문수신.(#:%.*s)(원#:%.*s)(Msg:%.*s)"
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
		m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, " ", "신규주문저장.(#:%.*s)(수량:%.*s)(SIDE:%c)(PRC:%s)"
			, sizeof(p->OrdNo), p->OrdNo, sizeof(p->RemainQty), p->RemainQty,p->Side[0], zOrdPrc);
	}

	// CANCEL ORDER
	if (p->ReplyTp[0] == 'O' && p->OrdProcTp[0] == CD_ORD_PROC_CNCL && p->OrdTp[0]==CD_ORD_TP_LIMIT)
	{
		char zOrigNo[32];
		sprintf(zOrigNo, "%.*s", sizeof(p->OrigNo), p->OrigNo);
		int nCnt = m_UserInfo.OrdInfo.DelLimitOrdInfoByOrdNo(zOrigNo);
		m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, " ", "주문취소.(원#:%s).취소처리갯수:%d", zOrigNo, nCnt);
	}

}

// 체결 real 로 수신 및 관리
VOID CUserStratOrd::ClientDataRealExec(char* pRecvData)
{
	if (!IsStratApplied())
		return;
	PACK_C_ORDCNTR* p = (PACK_C_ORDCNTR*)pRecvData;

	// 거부처리
	if (p->ReplyTp[0] == 'R')
	{
		//TODO. LOG
		//g_log.log(LOGTP_ERR, "주문거부수신[%s][원주문번호:%d]", sSymbol.GetBuffer(), nOrigNo);
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

	// 체결전에 들어온 주문완료에 의한 정보 삭제
	INT nCnt = m_UserInfo.OrdInfo.DelLimitOrdInfoByOrdNo(zOrdNo);
	
	int nOpenQty = S2N(p->OpenQty, sizeof(p->OpenQty));
	char zChartNm[32];
	char zTradeTm[32], zTradeDT[32];
	sprintf(zTradeTm, "%.8s", p->TM);
	sprintf(zTradeDT, "%.8s", p->DT);
	GET_CHART_NM_EX(zTradeDT, zTradeTm, m_UserInfo.ChartTp, zChartNm);

	// 진입. 수량은 모두 1 이므로 일부청산은 없다고 본다.
	if(nOpenQty>0)
	{
		BOOL bAlreadyOpened = m_UserInfo.PosInfo.IsOpened(zSymbol);
		if (bAlreadyOpened) {
			//TODO. LOGGING. 이미 진입된 상태에서 청산이 아닌 체결이 들어왔다.
			// 기존잔고수량, 평단 기록
			// go on
		}

		// 최종진입봉 저장
		strcpy(m_UserInfo.zLastEntryChartNm, zChartNm);

		// 기존 같은 방향 지정가, MIT 주문 취소
		Cncl_LimitOrd(zSymbol, p->Side[0], 0, 'S');
		Cncl_MITOrd(zSymbol, p->Side[0], FALSE);

		// 시가전략
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_OPENPRC, strlen(STRATID_FB_ORD_OPENPRC)) != 0)
		{
			// 포지션 ADD
			m_UserInfo.PosInfo.SetPosInfoReal(zSymbol, nOpenQty, p->OpenSide[0], zAvgPrc, zTradeDT, zTradeTm, (CHART_TP)m_UserInfo.ChartTp);
			
			// 익절 지정가(20 TICK)
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

			m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, "", "시가일치에 의해 체결 수신 후 익절지정가 전송");


			// 익절지정가 주문전송. 
			SendSaveStratOrd(CD_ORD_TP_LIMIT, CD_ORD_PROC_NEW,zOrdPrc, zOrdPrc, cOrdSide, m_UserInfo.nEntryOrdQty, 'N', 0,  'Y', FALSE, "");

			// 손절 MIT(20 TICK)
			if (p->Side[0] == 'S')	cPlusMinus = '+';
			if (p->Side[0] == 'B')	cPlusMinus = '-';
			dOrdPrc = CUtil::GetPrcByTick(zOrdPrc, 20, m_UserInfo.SymbolInfo.dTickSize, cPlusMinus);
			FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, temp);
			strcpy(zOrdPrc, temp);

			m_pStratLog->logProcClPack(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, "", "시가일치에 의해 체결 수신 후 손절MIT 전송");

			// 손절MIT 주문전송
			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW,zOrdPrc, zOrdPrc, cOrdSide, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, "");
		}
		// 보조지표전략
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_ASSISTPRC, strlen(STRATID_FB_ORD_ASSISTPRC)) != 0)
		{
			//TODO
		}
		// 10이평선 전략
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_CROSSMA, strlen(STRATID_FB_ORD_CROSSMA)) != 0)
		{
			//TODO
		}

		// 횡보전략
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_SIDEWAY, strlen(STRATID_FB_ORD_SIDEWAY)) != 0)
		{
			//TODO
		}
	}
	else // 청산
	{
		// 최종청산봉 저장
		strcpy(m_UserInfo.zLastClrChartNm, zChartNm);

		// 기존 모든 방향 지정가, MIT 주문 취소
		Cncl_LimitOrd(zSymbol, 0x20, 0, 'A');
		Cncl_MITOrd(zSymbol, 0x20, TRUE);

		// 포지션 CLEAR
		m_UserInfo.PosInfo.DelPosInfo(zSymbol);

		// 시가전략
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_OPENPRC, strlen(STRATID_FB_ORD_OPENPRC)) != 0)
		{

		}
		// 보조지표전략
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_ASSISTPRC, strlen(STRATID_FB_ORD_ASSISTPRC)) != 0)
		{
			//TODO
		}
		// 10이평선 전략
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_CROSSMA, strlen(STRATID_FB_ORD_CROSSMA)) != 0)
		{
			//TODO
		}

		// 횡보전략
		if (strncmp(m_UserInfo.zAppliedStratID, STRATID_FB_ORD_SIDEWAY, strlen(STRATID_FB_ORD_SIDEWAY)) != 0)
		{
			//TODO
		}
	}

}




// TR 로 조회한 포지션 정보 저장
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


// TR 로 조회한 미체결 정보 저장
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

	// 내전략심볼이 아니면 나간다.
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
	1) signal 수신
	2) 양봉시가일치 : 매수, 음봉시가일치 : 매도
	3) 데드크로스 조건 점검
	  - 데드크로스 : 매수만 진입가능
	  - 골든크로스 : 매도만 진입가능
*/
VOID CUserStratOrd::Strat_OpenPrc()
{
	PACK_STRAT_FB_OPENPRC* p = (PACK_STRAT_FB_OPENPRC*)m_FlowInfo.pSigData;
	
	char cOrdSide = 0x00;
	char zOrdPrc[32];
	//BOOL bSendOrd = FALSE;

	// 최종SIGNAL 가격 저장
	char zPrc[32]; 
	sprintf(zPrc, "%.*s", sizeof(p->StratPrc), p->StratPrc);
	strcpy(m_UserInfo.zLastStratPrc, zPrc);

	// if 이미 진입
	if (m_UserInfo.PosInfo.IsOpened(m_FlowInfo.zSymbol))
	{
		// 진입봉과 현재봉이 같으면 청산하지 않는다.
		if (m_UserInfo.PosInfo.IsSameChart(m_FlowInfo.zSymbol, m_FlowInfo.zChartNm)) {
			m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, FALSE, "이미 진입중이고, 진입봉과 현재봉이 같으므로 청산하지 않는다.");
			return;
		}

		//TODO. ???이전 로직에서는 음봉, 양봉 모두 일치해도 청산은 했었는데 이건???

		// 매수진입이면 매도주문청산, vice versa
		// 음봉은 매도주문, 양봉은 매수주문
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
				"청산주문전송SKIP.현포지션SIDE[%c] 패킷SIG차트구분[%c]", m_UserInfo.PosInfo.PosSide(m_FlowInfo.zSymbol), p->ChartGb[0]);
			return;
		}
		
		// 기존주문 취소처리
		m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE, "청산전 기존모든주문 취소 전송");
		Cncl_LimitOrd(m_UserInfo.zSymbol, 0x20, 0, 'A');
		Cncl_MITOrd(m_UserInfo.zSymbol, 0x20, TRUE);

		m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE, "청산주문전송");

		// 주문가격 : 시가
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
	// 진입되어 있지 않다. 

	// 청산봉에서는 진입하지 않는다.
	if (strncmp(m_FlowInfo.zChartNm, m_UserInfo.zLastClrChartNm, LEN_CHART_NM) == 0) {
		m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, FALSE, "최종청산봉과 현재봉이 같으므로 청산하지 않는다.");
		return;
	}


	// 크로스 점검
	CROSS_TP crossTp = GetCross(m_FlowInfo.ChartPrev);
	char zCross[32];
	CROSS2NAME(crossTp, zCross);
	cOrdSide = 0x00;
	// 데드크로스 => 매수만
	if( crossTp==DEAD_CROSS && p->ChartGb[0] == CD_PLUS_CANDLE)
	{
		cOrdSide = CD_BUY;
	}

	// 골든크로스 => 매도만
	if (crossTp == GOLDEN_CROSS && p->ChartGb[0] == CD_MINUS_CANDLE)
	{
		cOrdSide = CD_SELL;
	}

	if (cOrdSide == 0x00)
	{
		m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, FALSE,
			"진입주문전송SKIP.CROSS(%s) 패킷SIG차트구분[%c]", zCross, p->ChartGb[0]);
		return;
	}

	// 기존주문 취소처리
	m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE, "진입전 기존모든주문 취소 전송");
	Cncl_LimitOrd(m_UserInfo.zSymbol, 0x20, 0, 'A');
	Cncl_MITOrd(m_UserInfo.zSymbol, 0x20, TRUE);
	
	m_pStratLog->logProcSig(STRATID_FB_ORD_OPENPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE, "진입주문전송");

	// 주문가격 : 시가
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


// 보조지표 생성에 따른 주문
VOID CUserStratOrd::Strat_AssistPrcSet()
{
	PACK_STRAT_FB_ASSIST* p = (PACK_STRAT_FB_ASSIST*)m_FlowInfo.pSigData;
	//PACK_STRAT_ORD* pClient = (PACK_STRAT_ORD*)m_FlowInfo.zClientBuf;

	m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE,"보조지표설정 수신 후 모든MIT 취소");
		
	// 기존 보조신호에 의한 손절익절 취소처리 
	// ==> 그러나, 모든 유저는 단 하나의 전략만 있으므로 그냥 취소처리 한다.
	Cncl_MITOrd(m_UserInfo.zSymbol, 0x20, TRUE);
	

	//double dOrdPrc;
	char zPrc[32];
	char zChartNm[32];
	sprintf(zChartNm, "%.*s", LEN_CHART_NM, p->ChartNm);

	sprintf(zPrc, "%.*s", sizeof(p->StratPrc), p->StratPrc);
	double dAssistPrc = atof(zPrc);

	// 최종 보조지표가격 저장
	strcpy(m_UserInfo.zLastStratPrc, zPrc);

	// 크로스 점검
	CROSS_TP crossTp = GetCross(m_FlowInfo.ChartPrev);

	if (m_UserInfo.bCrossApplied )
	{
		// 데드크로스 => 매수만
		if (crossTp == DEAD_CROSS && p->ChartGb[0] == CD_PLUS_CANDLE)
		{
			// 가격 : 보조지표 + 1틱
			double dOrdPrc = dAssistPrc + (1 * m_UserInfo.SymbolInfo.dTickSize);
			FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);
			// 보조신호 < 시가 인 경우는 주문하지 않는다.
			// TODO. 기존 로직에는 있었다. 적용해야 하나?
			//if (CUtil::CompPrc(zPrc, LEN_PRC, m_FlowInfo.ChartNow.open, LEN_PRC, m_UserInfo.SymbolInfo.nDotCnt, LEN_PRC) < 0)
			//{
			//	//TODO. JUST LOGGING
			//	return;
			//}

			m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE,
				"보조지표SET(%s).매수.데드크로스(직전봉SMA:%.*s < 직전봉LMA:%.*s)",
				p->StratPrc, LEN_PRC, m_FlowInfo.ChartPrev.sma_short, LEN_PRC, m_FlowInfo.ChartPrev.sma_short);

			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_BUY, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
		}

		// 골든크로스 => 매도만
		if (crossTp == GOLDEN_CROSS && p->ChartGb[0] == CD_MINUS_CANDLE)
		{

			// 가격 : 보조지표 - 1틱
			double dOrdPrc = dAssistPrc - (1 * m_UserInfo.SymbolInfo.dTickSize);
			FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);

			// 보조신호 > 시가 인 경우는 주문하지 않는다.
			// TODO. 기존 로직에는 있었다. 적용해야 하나?
			//if (CUtil::CompPrc(zPrc, LEN_PRC, m_FlowInfo.ChartNow.open, LEN_PRC, m_UserInfo.SymbolInfo.nDotCnt, LEN_PRC) > 0)
			//{
			//	//TODO. JUST LOGGING
			//	return;
			//}

			m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE,
				"보조지표SET(%s).매도.골든크로스(직전봉SMA:%.*s > 직전봉LMA:%.*s)",
				p->StratPrc, LEN_PRC, m_FlowInfo.ChartPrev.sma_short, LEN_PRC, m_FlowInfo.ChartPrev.sma_short);

			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_SELL, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
		}
	} // if (m_UserInfo.bCrossApplied )
	else
	{
		if (p->ChartGb[0] == CD_PLUS_CANDLE)
		{
			// 가격 : 보조지표 + 1틱
			double dOrdPrc = dAssistPrc + (1 * m_UserInfo.SymbolInfo.dTickSize);
			FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);
			// 보조신호 < 시가 인 경우는 주문하지 않는다.
			// TODO. 기존 로직에는 있었다. 적용해야 하나?
			//if (CUtil::CompPrc(zPrc, LEN_PRC, m_FlowInfo.ChartNow.open, LEN_PRC, m_UserInfo.SymbolInfo.nDotCnt, LEN_PRC) < 0)
			//{
			//	//TODO. JUST LOGGING
			//	return;
			//}

			m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE,
				"보조지표SET(%s).매수.NOCROSS.양봉", p->StratPrc);

			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_BUY, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
		}

		if (p->ChartGb[0] == CD_MINUS_CANDLE)
		{

			// 가격 : 보조지표 - 1틱
			double dOrdPrc = dAssistPrc - (1 * m_UserInfo.SymbolInfo.dTickSize);
			FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);

			// 보조신호 > 시가 인 경우는 주문하지 않는다.
			// TODO. 기존 로직에는 있었다. 적용해야 하나?
			//if (CUtil::CompPrc(zPrc, LEN_PRC, m_FlowInfo.ChartNow.open, LEN_PRC, m_UserInfo.SymbolInfo.nDotCnt, LEN_PRC) > 0)
			//{
			//	//TODO. JUST LOGGING
			//	return;
			//}

			m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, TRUE,
				"보조지표SET(%s).매도.NOCROSS.음봉", p->StratPrc);

			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_SELL, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
		}
	}


}



// 보조지표 청산 주문
VOID CUserStratOrd::Strat_AssistPrcClr()
{
	PACK_STRAT_FB_ASSIST* p = (PACK_STRAT_FB_ASSIST*)m_FlowInfo.pSigData;

	// 포지션이 없으면 나간다.
	if (!m_UserInfo.PosInfo.IsOpened(m_UserInfo.zSymbol))
	{
		m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol,m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, FALSE, "청산주문전송SKIP.포지션없음");
		return;
	}

	// 이미 설정된 보조지표 가격이 없으면 나간다.
	if (m_UserInfo.zLastStratPrc[0] == 0x00)
	{
		m_pStratLog->logProcSig(STRATID_FB_SIG_ASSISTPRC, m_FlowInfo.zSymbol, m_UserInfo.zUserID, m_FlowInfo.ChartNow.Nm, FALSE, 
			"청산주문전송SKIP.설정된 보조지표 가격이 없다.");
		return;
	}

	char zPrc[64];
	char zAssistPrc[128];
	sprintf(zAssistPrc, "%.*s", LEN_PRC, p->StratPrc);

	char cPosSide = m_UserInfo.PosInfo.PosSide(m_FlowInfo.zSymbol);

	// 매수포지션 & 붕괴 => 매도청산 주문
	if (cPosSide == CD_BUY && p->ClrCrossTp[0] == 'D')
	{
		// 기존MIT 취소
		Cncl_MITOrd(m_UserInfo.zSymbol, 0x20, TRUE);

		// TODO.지정가도 취소해야 하나?

		// 가격 : 보조지표 - 1틱
		double dOrdPrc = atof(zAssistPrc) - (1 * m_UserInfo.SymbolInfo.dTickSize);
		FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);

		// 청산주문전송.MIT.
		SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_SELL, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
	}

	// 매도포지션 & 돌괴 => 매수청산 주문
	if (cPosSide == CD_SELL && p->ClrCrossTp[0] == 'U')
	{
		// 기존MIT 취소
		Cncl_MITOrd(m_UserInfo.zSymbol, 0x20, TRUE);

		// TODO.지정가도 취소해야 하나?

		// 가격 : 보조지표 + 1틱
		double dOrdPrc = atof(zAssistPrc) + (1 * m_UserInfo.SymbolInfo.dTickSize);
		FORMAT_PRC(dOrdPrc, m_UserInfo.SymbolInfo.nDotCnt, zPrc);

		// 청산주문전송.MIT.
		SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_NEW, zPrc, zPrc, CD_BUY, m_UserInfo.nEntryOrdQty, 'N', 0, 'N', FALSE, m_FlowInfo.ChartNow.Nm);
	}
}

/*
	신호를 받은 후 현재 포지션 없으면 진입로직, 있으면 청산로직
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

	// 크로스 점검
	CROSS_TP crossTp = GetCross(m_FlowInfo.ChartPrev);


	// 포지션 없음. 진입
	if (!m_UserInfo.PosInfo.IsOpened(m_FlowInfo.zSymbol))
	{
		//memcpy(pClient->OrdTp, CD_ORD_TP_MIT, strlen(CD_ORD_TP_MIT));
		//memcpy(pClient->OrdProcTp, CD_ORD_PROC_NEW, strlen(CD_ORD_PROC_NEW));
		//pClient->ClrYN[0] = 'N';
		//sprintf(temp, "%-*.*s", LEN_QTY, LEN_QTY, "1");
		//memcpy(pClient->OrdQty, temp, strlen(temp));

		// 데드크로스 + 돌파 => 매수
		if (crossTp==DEAD_CROSS && p->Tp[0] == 'U')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_BUY;
			//double dPrc = atof(zMAPrc) + (1 * m_SymbolInfo.dTickSize);	// MA +  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. 전송
		}

		// 골든크로스 + 붕괴 => 매도
		if (crossTp == GOLDEN_CROSS && p->Tp[0] == 'D')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_SELL;
			//double dPrc = atof(zMAPrc) - (1 * m_SymbolInfo.dTickSize);	// MA -  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. 전송
		}
	}

	// 청산
	else if (m_UserInfo.PosInfo.IsOpened(m_FlowInfo.zSymbol))
	{
		//memcpy(pClient->OrdTp, CD_ORD_TP_MIT, strlen(CD_ORD_TP_MIT));
		//memcpy(pClient->OrdProcTp, CD_ORD_PROC_NEW, strlen(CD_ORD_PROC_NEW));
		//pClient->ClrYN[0] = 'Y';
		//sprintf(temp, "%-*.*s", LEN_QTY, LEN_QTY, "1");
		//memcpy(pClient->OrdQty, temp, strlen(temp));

		char cPosSide = m_UserInfo.PosInfo.PosSide(m_FlowInfo.zSymbol);

		// SHORT + 돌파 => 매수청산
		if (cPosSide ==CD_SELL && p->Tp[0] == 'U')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_BUY;
			//double dPrc = atof(zMAPrc) + (1 * m_SymbolInfo.dTickSize);	// MA +  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. 전송
		}

		// LONG + 붕괴 => 매도청산
		if (cPosSide == CD_BUY && p->Tp[0] == 'D')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_SELL;
			//double dPrc = atof(zMAPrc) - (1 * m_SymbolInfo.dTickSize);	// MA -  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. 전송
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


	// 포지션 없음. 진입
	if (!m_UserInfo.PosInfo.IsOpened(m_FlowInfo.zSymbol))
	{
		//memcpy(pClient->OrdTp, CD_ORD_TP_MIT, strlen(CD_ORD_TP_MIT));
		//memcpy(pClient->OrdProcTp, CD_ORD_PROC_NEW, strlen(CD_ORD_PROC_NEW));
		//pClient->ClrYN[0] = 'N';
		//sprintf(temp, "%-*.*s", LEN_QTY, LEN_QTY, "1");
		//memcpy(pClient->OrdQty, temp, strlen(temp));

		// 돌파 => 매수
		if (p->Tp[0] == 'U')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_BUY;
			//double dPrc = atof(zStratPrc) + (1 * m_SymbolInfo.dTickSize);	// MA +  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. 전송
		}

		// 붕괴 => 매도
		if (p->Tp[0] == 'D')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_SELL;
			//double dPrc = atof(zStratPrc) - (1 * m_SymbolInfo.dTickSize);	// MA -  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. 전송
		}
	}

	// 청산
	else if (m_UserInfo.PosInfo.IsOpened(m_FlowInfo.zSymbol))
	{
		//memcpy(pClient->OrdTp, CD_ORD_TP_MIT, strlen(CD_ORD_TP_MIT));
		//memcpy(pClient->OrdProcTp, CD_ORD_PROC_NEW, strlen(CD_ORD_PROC_NEW));
		//pClient->ClrYN[0] = 'Y';
		//sprintf(temp, "%-*.*s", LEN_QTY, LEN_QTY, "1");
		//memcpy(pClient->OrdQty, temp, strlen(temp));

		char cPosSide = m_UserInfo.PosInfo.PosSide(m_FlowInfo.zSymbol);

		// SHORT + 돌파 => 매수청산
		if (cPosSide == CD_SELL && p->Tp[0] == 'U')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_BUY;
			//double dPrc = atof(zStratPrc) + (1 * m_SymbolInfo.dTickSize);	// MA +  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. 전송
		}

		// LONG + 붕괴 => 매도청산
		if (cPosSide == CD_BUY && p->Tp[0] == 'D')
		{
			//m_FlowInfo.bRsltSend = TRUE;
			//pClient->Side[0] = DEF_SELL;
			//double dPrc = atof(zStratPrc) - (1 * m_SymbolInfo.dTickSize);	// MA -  1 TICK
			//sprintf(temp, "%.*f", sizeof(pClient->OrdPrc), dPrc);
			//memcpy(pClient->OrdPrc, temp, strlen(temp));

			//TODO. 전송
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
	p->OrdTp[0] = cOrdTp;	// 1-지정가, 2-시장가, 3-손절, 4-익절
	p->OrdProcTp[0] = cOrdProcTp;	// 1-신규, 2-정정, 3-취소, 4-거부, A - 종목취소, B - 전종목취소, C - 종목청산, D - 전종목청산
	p->ClrYN[0] = cClrYN;		// 청산주문여부
	memcpy(p->OrdPrc, pzPrc, strlen(pzPrc));

	char zOrdQty[32];
	sprintf(zOrdQty, "%d", nQty);
	memcpy(p->OrdQty, zOrdQty, strlen(zOrdQty));

	sprintf(temp, "%d", nOrigNo);
	memcpy(p->OrigOrdNo, temp, strlen(temp));
	memcpy(p->OrigPrcCncl, pzOrigPrc,strlen(pzOrigPrc));	//손절,익절 취소시 원주문 가격
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

	// MIT 인 경우는 자체 저장
	if (cOrdTp == CD_ORD_TP_MIT)
	{
		m_UserInfo.OrdInfo.SetOrdInfo(m_UserInfo.zSymbol, m_UserInfo.zAppliedStratID, CD_ORD_TP_MIT, "", "",
			zOrdQty, cSide, pzPrc, "", "");
	}
	
	// LOGGING
	sprintf(temp, "%.*s", LEN_CHART_NM, psChartNm);
	//(char* psStratCd, char* pzSymbol, char* pzChartNm, char cOrdTp, char cOrdProcTp, char* pzPrc, char* pzOrgPrc, char* pMsg, ...);
	m_pStratLog->logOrd(m_UserInfo.zAppliedStratID, m_UserInfo.zSymbol, m_UserInfo.zUserID, temp, cOrdTp, cOrdProcTp, pzPrc, pzOrigPrc,
		"주문전송(%s)", pData);
}


// 현재 기준 현재 차트와 직전 차트 가져오기
INT CUserStratOrd::GetLastChartShm(char* pzGroupKey, char* pzChartNm, ST_SHM_CHART_UNIT& shmPrev, ST_SHM_CHART_UNIT& shmNow)
{
	char szChart[LEN_CHART_NM + 1];

	// 현재 차트 정보 가져오기
	BOOL bExist = m_pShmQ->DataGet(pzGroupKey, pzChartNm, (char*)&shmNow);

	// 1. 현재 차트가 없는 경우 ==> 나간다.
	if (FALSE == bExist)
	{
		// 지금시간 차트가 없는데, 지금시간 체결가는 들어온 상황이라면 차트 이상이다.
		g_log.log(LOGTP_ERR, "[%s][%s] 현재가수신했는데 현재차트가 없다.", pzGroupKey, pzChartNm);
		return -1;
	}


	// 2. 현재 차트가 있는 경우 직전 차트 가져오기	  
	sprintf(szChart, "%.*s", LEN_CHART_NM, shmNow.prevNm);
	if (strcmp(szChart, "NONE") == 0)
		return -3;

	if (FALSE == m_pShmQ->DataGet(pzGroupKey, szChart, (char*)&shmPrev))
	{
		g_log.log(LOGTP_ERR, "(%s)(%.*s) 직전 차트가 SHM 에 없다", pzGroupKey, LEN_CHART_NM, szChart);
		return -2;
	}
	//g_log.log(LOGTP_SUCC, "직전차트(%.4s)", chart[0].Nm);


	return 1;
}



/*

typedef struct _USER_STRAT_INFO
{
char	zCompID[LEN_STRAT_COMP_ID+1];
char	zUserID[LEN_USERID + 1];
BOOL	bStratApplied;
char	zSymbol[LEN_SYMBOL + 1];			// 전략심볼
char	zAppliedStratID[32];
CHART_TP ChartTp;
BOOL	bCrossApplied;
char	zLastClrChartNm[LEN_CHART_NM + 1];		// 최종청산봉
char	zLastEntryChartNm[LEN_CHART_NM + 1];	// 최종진입봉
int		nEntryOrdQty;	// 진입기본수량
char	zLastStratPrc[LEN_PRC + 1];				// 전략최종가격
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
	JAY	FB	11	시가일치	Y	00701000001	CLZ7	CL	CLZ7	0.01000	2	1
	BOOL	bStratApplied;
	CHART_TP ChartTp;
	char	zLastClrChartNm[LEN_CHART_NM + 1];		// 최종청산봉
	char	zLastEntryChartNm[LEN_CHART_NM + 1];	// 최종진입봉
	int		nEntryOrdQty;	// 진입기본수량
	char	zLastStratPrc[LEN_PRC + 1];				// 전략최종가격
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
	case MNG_FB_ORDSYMBOL_ADD:	//7)FB 자동주문종목 설정
	case MGN_FB_STRAT_SET:		//10) FB특정계좌 전략설
		//sprintf(zMsg, "%s%s전략설정/종목설정(%s)(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT, AcntNo(), p->zWorkVal);
		//g_log.log(LOGTP_SUCC, zMsg);
		//m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT, zMsg);

		//EnterCriticalSection(&m_csProtect);
		//ReleaseStrategy(FALSE/*DBSAVE*/, 'N', "관리자 전략설정 선작업");
		//Initialize();
		//LeaveCriticalSection(&m_csProtect);

		break;
	case MNG_FB_ORDSYMBOL_RELEASE:	// 8)FB 자동주문종목 해제
		//sprintf(zMsg, "%s%s종목해제(%s)(%s)", LOG1_ADMIN, LOG2_ADMIN_SYMBOL, AcntNo(), p->zWorkVal);
		//g_log.log(LOGTP_SUCC, zMsg);
		//m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_SYMBOL, zMsg);

		//EnterCriticalSection(&m_csProtect);
		//ReleaseSymbol(CString(p->zWorkVal));
		//LeaveCriticalSection(&m_csProtect);
		break;
	case MGN_FB_STRAT_RELEASE:	// 11) FB특정계좌 전략해제
		//sprintf(zMsg, "%s%s 계좌전략해제(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT, AcntNo());
		//g_log.log(LOGTP_SUCC, zMsg);
		//m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT, zMsg);

		//EnterCriticalSection(&m_csProtect);
		//ReleaseStrategy(TRUE/*DB SAVE*/, 'N', "관리자 해제");
		//LeaveCriticalSection(&m_csProtect);
		break;
	case MGN_FB_CROSS_CHECK:
		//bCrossCheck = TRUE;
		//sprintf(zMsg, "%s%s크로스적용 설정(%s)", LOG1_ADMIN, LOG2_ADMIN_CROSS, AcntNo());
		//g_log.log(LOGTP_SUCC, zMsg);
		//m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_CROSS, zMsg);
		break;
	//case MNG_FB_CROSS_UNCHECK:
	//	sprintf(zMsg, "%s%s크로스적용 해제(%s)", LOG1_ADMIN, LOG2_ADMIN_CROSS, AcntNo());
	//	g_log.log(LOGTP_SUCC, zMsg);
	//	m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_CROSS, zMsg);
	//	bCrossCheck = FALSE;
		break;
	case MNG_FB_PROFIT_TICK:
	case MNG_FB_LOSS_TICK:
		//sprintf(zMsg, "%s%s전략해제 틱수 재 설정(%s)", LOG1_ADMIN, LOG2_ADMIN_STRAT_TICK, AcntNo());
		//g_log.log(LOGTP_SUCC, zMsg);
		//m_pStratLog->LoggingEx(AcntNo(), " ", (CHART_TP)0, LOG1_ADMIN, LOG2_ADMIN_STRAT_TICK, zMsg);
		//LoadAcntTradeInfo();
		break;
	default:
		g_log.log(LOGTP_ERR, "[잘못된 작업TP:%d]", p->nWorkTp);
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
		sprintf(msg, "[GetCross]<NONE_CROSS-데이터이상> (short:%10.5f)(long:%10.5f) ", dShort, dLong);
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

