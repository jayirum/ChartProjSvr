#include "stdafx.h"
#include "CopyOrder.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_INC/IRUM_Common.h"

extern CLogMsg g_log;
VOID IR_PrintMsg(BOOL bSucc, char* pMsg, ...)
{

	char buff[DEF_LOG_LEN];
	va_list argptr;

	va_start(argptr, pMsg);
	vsprintf_s(buff, pMsg, argptr);
	va_end(argptr);

	if (bSucc)
		g_log.log(LOGTP_SUCC, buff);
	else
		g_log.log(LOGTP_ERR, buff);

}


CCopyOrder::CCopyOrder(char* zAcntNo, char* zAcntPwd, CESApi *Api, CRITICAL_SECTION* csApi, 
						Poco::MemoryPool* pMemPool, CDBPool *pDBPool, HWND hWnd, UINT unMultiple)
{
	strcpy(m_zAcntNo, zAcntNo);
	strcpy(m_zAcntPwd, zAcntPwd);
	m_Api = Api;
	m_csApi = csApi;
	m_pMemPool = pMemPool;
	m_pDBPool = pDBPool;
	m_hWnd = hWnd;
	m_unMultiple = unMultiple;
}


CCopyOrder::~CCopyOrder()
{
}

VOID CCopyOrder::ThreadFunc()
{
	char temp[1024];
	int iRslt;

	IR_PrintMsg(LOGTP_SUCC, "[스레드id:%d][%d][%s] 계좌 Thread start", ThrId(),m_dwThreadID, Acnt());
	
	while(TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, 1, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) {
			IR_PrintMsg(LOGTP_ERR, "[THREAD 계좌:%s]WAIT ERROR(%d)", Acnt(), GetLastError());
			Sleep(1000);
			continue;
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
		switch (msg.message)
		{
		case WM_COPYTHREAD_DIE:
		{
			return;
		}
		case WM_COPYTHREAD_RECV:
			COPYORD_TARGET_CNTR* p = (COPYORD_TARGET_CNTR*)msg.lParam;

			char			szBuff[4096];
			lpEs5601I_st	lpTrIn = (lpEs5601I_st)szBuff;

			memset(szBuff, 0x00, 4096);
			memset(szBuff, 0x20, 4096);

			sprintf(temp, "%010.0f", (double)(unsigned long)m_hWnd);
			memcpy(lpTrIn->Head.WinId, temp, strlen(temp));

			// 계좌번호
			memcpy(lpTrIn->Account, Acnt(), strlen(Acnt()));

			// 비밀번호
			m_Api->GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), Acnt(), Pwd());

			// 주문구분	(1.신규주문 2.정정주문 3.취소주문)
			lpTrIn->Order_Type[0] = '1';

			// 종목코드
			CString sCode;
			if (ir_isbrokerHD(p->Broker))
			{
				CString sHd;
				char zKr[64];
				sHd.Format("%.*s", sizeof(p->symbol), p->symbol);
				sHd.Trim();
				sCode = ir_cvtcode_HD_KR(sHd.GetBuffer(), zKr);
				memcpy(lpTrIn->ShortCode, zKr, strlen(zKr));
			}
			else
			{
				sprintf(temp, "%.*s", sizeof(p->symbol), p->symbol);
				sCode = m_Api->GetShortCode(temp);
				memcpy(lpTrIn->ShortCode, sCode, sCode.GetLength());
			}


			// 매도매수구분 (1.매수 2.매도). 
			// copytrading 은 그대로, hedge trading 은 반대로 한다.
			if (p->TradeTp[0] == DEF_TRADEGB_COPY) {
				lpTrIn->BuySell_Type[0] = p->side[0];
			}
			else {
				if (p->side[0] == DEF_BUY)	lpTrIn->BuySell_Type[0] = DEF_SELL;
				if (p->side[0] == DEF_SELL)	lpTrIn->BuySell_Type[0] = DEF_BUY;
			}

			// 가격조건	(1.지정가 2.시장가). 무조건 시장가
			lpTrIn->Price_Type[0] = '2';

			// 체결조건 (시장가일경우(3) 지정가일경우(1))
			lpTrIn->Trace_Type[0] = '3';

			// 주문수량
			sprintf(temp, "%.*s", sizeof(p->qty), p->qty);
			int nQty = atoi(temp)*m_unMultiple;
			sprintf(temp, "%0*d", sizeof(lpTrIn->Order_Volume), nQty);
			memcpy(lpTrIn->Order_Volume, temp, sizeof(lpTrIn->Order_Volume));

			// 주문가격
			//memcpy (lpTrIn->Order_Price, m_szOrdP, m_szOrdP.GetLength());


			// 통신주문구분
			m_Api->GetCommunicationType(lpTrIn->Order_Comm_Type, sizeof(lpTrIn->Order_Comm_Type));

			// 주문전략구분 (1.일반주문 2.STOP주문)
			lpTrIn->Stop_Type[0] = '1';

			// STOP주문가격 (STOP주문이면 입력 아니면 0 셋팅)
			//lpTrIn->Stop_Price;

			// 주문조작구분 (항상 1로)
			lpTrIn->Oper_Type[0] = '1';

			/*
			IR_PrintMsg(TRUE, TRUE, "[대상체결](ACC:%.*s)(종목:%.*s)(방향:%.*s)(수량:%.*s)",
			sizeof(p->Account), p->Account, sizeof(p->FullCode), p->FullCode, sizeof(p->Side), p->Side, sizeof(p->Qty), p->Qty);
			*/
			
			EnterCriticalSection(m_csApi);
			iRslt = m_Api->SendTrData(ESID_5601, szBuff, SZ_Es5601I);
			LeaveCriticalSection(m_csApi);
			IR_PrintMsg(TRUE, "[COPY체결전송:%d](ACC:%.*s)(종목:%.*s)(방향:%.*s)(수량:%.*s)(비번:%s)",
				iRslt,
				sizeof(lpTrIn->Account), lpTrIn->Account,
				sizeof(lpTrIn->ShortCode), lpTrIn->ShortCode,
				sizeof(lpTrIn->BuySell_Type), lpTrIn->BuySell_Type,
				sizeof(lpTrIn->Order_Volume), lpTrIn->Order_Volume,
				Pwd()
			);

			// DB 저장
			SaveTradeInfo(sCode, (char*)p, szBuff);
		}// end of while
		}
	}
}


BOOL CCopyOrder::SaveTradeInfo(CString& sShortCode, char* pTarget, char* pHedge)
{
	CDBHandler db(m_pDBPool->Get());

	COPYORD_TARGET_CNTR* t = (COPYORD_TARGET_CNTR*)pTarget;
	lpEs5601I_st h = (lpEs5601I_st)pHedge;

	char symbol[64];
	char trade_qty[64];
	char h_acnt[64], h_side[64], h_dt[64], h_tm[64];

	sprintf(symbol, sShortCode.GetBuffer());
	sprintf(trade_qty, "%.*s", sizeof(t->qty), t->qty);
	sprintf(h_acnt, "%.*s", sizeof(h->Account), h->Account);
	sprintf(h_side, "%.*s", sizeof(h->BuySell_Type), h->BuySell_Type);

	SYSTEMTIME st;
	//char temp[32];
	GetLocalTime(&st);

	sprintf(h_dt, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	sprintf(h_tm, "%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	CString sQ;


	sQ.Format("INSERT INTO TRADE_INFO("
		"SYMBOL, "
		"TRADE_QTY, "
		"TARGET_ACNT_NO, "
		"TARGET_TRADE_NO, "
		"TARGET_TRADE_SIDE, "
		"TARGET_ORD_TP, "
		"TARGET_PRICE, "
		"TARGET_TRADE_DT, "
		"TARGET_TRADE_TM, "
		"TARGET_CMSN, "
		"HEDGE_ACNT_NO, "
		"HEDGE_TRADE_SIDE, "
		"HEDGE_TRADE_DT, "
		"HEDGE_TRADE_TM, "
		"HEDGE_ORD_TP, "
		"HEDGE_PRICE )"
		" VALUES("
		"'%s',"	//SYMBOL
		"%d,"	//TRADE_QTY
		"'%.*s',"	//TARGET_ACNT_NO
		"'%.*s',"	//TARGET_TRADE_NO
		"'%.*s',"	//TARGET_TRADE_SIDE
		"'%.*s',"	//TARGET_ORD_TP
		"%f,"	//TARGET_PRICE
		"'%.*s',"	//TARGET_TRADE_DT
		"'%.*s',"	//TARGET_TRADE_TM
		"%f,"	//TARGET_CMSN
		"'%s',"	//HEDGE_ACNT_NO
		"'%s',"	//HEDGE_TRADE_SIDE
		"'%s',"	//HEDGE_TRADE_DT
		"'%s',"	//HEDGE_TRADE_TM
		"'%c',"	//HEDGE_ORD_TP
		"%f )"	//HEDGE_PRICE
		, symbol
		, S2N(t->qty,sizeof(t->qty))
		, sizeof(t->TargetAcntNo), t->TargetAcntNo
		, sizeof(t->TargetOrdNo), t->TargetOrdNo
		, sizeof(t->side), t->side
		, sizeof(t->TargetOrdTp), t->TargetOrdTp
		, S2D(t->prc, sizeof(t->prc))
		, sizeof(t->TargetDT), t->TargetDT
		, sizeof(t->TargetTM), t->TargetTM
		, S2D(t->TargetCmsn, sizeof(t->TargetCmsn))
		, h_acnt
		, h_side
		, h_dt
		, h_tm
		, h->Price_Type[0]		//(1.지정가 2.시장가)
		, S2D(h->Order_Price,sizeof(h->Order_Price))
	);

	if (!db->Cmd(sQ.GetBuffer()) || !db->Exec())
	{
		IR_PrintMsg(FALSE, "체결정보 저장 오류<%s>(MSG:%s)", sQ.GetBuffer(), db->GetLastError());
		return FALSE;
	}
	IR_PrintMsg(TRUE,  "[DB저장](ACC:%.*s)(종목:%.*s)(방향:%c)(수량:%.*s)",		
		sizeof(h->Account), h->Account,
		sizeof(h->ShortCode), h->ShortCode,
		h->BuySell_Type[0], sizeof(h->Order_Volume), h->Order_Volume);
	return TRUE;
}
