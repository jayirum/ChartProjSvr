#include "FBOrdThread.h"
#include "../../IRUM_UTIL/Log.h"
#include "../../IRUM_UTIL/util.h"


extern CLogMsg g_log;


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
CFBOrdThread::CFBOrdThread(CESApi* api, CRITICAL_SECTION* cs, HWND* hwd)
{
	InitializeCriticalSectionAndSpinCount(&m_csOrd, 2000);
	InitializeCriticalSectionAndSpinCount(&m_csPos, 2000);
	InitializeCriticalSectionAndSpinCount(&m_csProtect, 2000);
	
	m_api = api;
	m_csApi = cs;
	m_hwd = hwd;
}


CFBOrdThread::~CFBOrdThread()
{
	DeleteCriticalSection(&m_csOrd);
	DeleteCriticalSection(&m_csPos);
	DeleteCriticalSection(&m_csProtect);
	//CloseHandle(m_hMutexOrd);
}





VOID CFBOrdThread::ThreadFunc()
{
	__try {
		MainFunc();
	}
	__except (ReportException(GetExceptionCode(), "CFBOrdThread::ThreadFunc", m_szMsg))
	{
		g_log.log(LOGTP_FATAL, m_szMsg);
		exit(0);
	}

}

VOID CFBOrdThread::MainFunc()
{	
	// ������ҵ� �ε�
	if (!Initialize())
	{
		g_log.log(LOGTP_FATAL, "�ֹ��������ʱ�ȭ ����.���μ��� ����");
		exit(0);
	}

	while (TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(1, (HANDLE*)&m_hDie, FALSE, 1, QS_ALLPOSTMESSAGE);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		else if (dwRet == WAIT_ABANDONED_0) {
			g_log.log(LOGTP_ERR, "������ WAIT ERROR(%d)", GetLastError());
			Sleep(1000);
			continue;
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_NOTI_MATCH:
				{
					EnterCriticalSection(&m_csProtect);
					DoProc_RecvAPIReplyWrapper((At0985O_st*)msg.lParam);
					delete ((void*)msg.lParam);
					LeaveCriticalSection(&m_csProtect);

					break;
				} // case WM_NOTI_MATCH:
			case WM_NOTI_REMAINORD:
			{
				lpEs5611O_st p = (lpEs5611O_st)msg.lParam;
				int nCnt = S2N(p->Dtno, sizeof(p->Dtno));
				for (int i = 0; i < nCnt; i++)
				{
					CString sAcnt;	
					sAcnt.Format("%.*s", sizeof(p->Sub[i].Account), p->Sub[i].Account);
					sAcnt.Trim();

					CString sSymbol;
					sSymbol.Format("%.*s", sizeof(p->Sub[i].ShortCode), p->Sub[i].ShortCode);
					sSymbol.TrimRight();

					CString sOrdNo;
					sOrdNo.Format("%.*s", sizeof(p->Sub[i].Ord_No), p->Sub[i].Ord_No);
					sOrdNo.TrimLeft(0x30);	sOrdNo.Trim();
					
					// ��ü�����
					CString sQty;
					sQty.Format("%.*s", sizeof(p->Sub[i].Mcg_q), p->Sub[i].Mcg_q);
					sQty.TrimLeft(0x30);	sQty.Trim();
					
					char cSide;
					switch (p->Sub[i].Bysl_tp[0]) {
					case '1': case '3': case '5':	cSide = DEF_BUY;	break;
					case '2': case '4': case '6':	cSide = DEF_SELL;	break;
					}

					CString sPrc;
					sPrc.Format("%.*s", sizeof(p->Sub[i].Ord_p), p->Sub[i].Ord_p);
					sPrc.Trim();

					int nCnt = OrderRecv_Add(sAcnt, sSymbol, atoi(sOrdNo.GetBuffer()), 0,
						atoi(sQty.GetBuffer()), cSide, sPrc.GetBuffer());

					//char msg[1024];
					//sprintf(msg, "[%s](%s)[�ֹ�TR����]�������ֹ��߰�(�ֹ���ȣ:%d)(SIDE:%c)(����:%10.5f)(�Ѱ���:%d)",
					//	sAcnt.GetBuffer(), sSymbol.GetBuffer(), atoi(sOrdNo.GetBuffer()),
					//	cSide, atof(sPrc.GetBuffer()), nCnt);
					//g_log.log(LOGTP_SUCC, msg);
				}
				delete ((void*)msg.lParam);
				break;
			}
			case WM_NOTI_POSITION:
			{
				lpEs5612O_st p = (lpEs5612O_st)msg.lParam;

				int nCnt = S2N(p->Dtno, sizeof(p->Dtno));
				for (int i = 0; i < nCnt; i++)

				{
					CString sFullCode, sSymbol;
					sFullCode.Format("%.*s", sizeof(p->Sub[i].FullCode), p->Sub[i].FullCode);
					sFullCode.TrimRight();
					sSymbol = m_api->GetShortCode(sFullCode);

					CString sAcnt;
					sAcnt.Format("%.*s", sizeof(p->Sub[i].Account), p->Sub[i].Account);
					sAcnt.Trim();

					// �̰�������
					CString sQty;
					sQty.Format("%.*s", sizeof(p->Sub[i].Open_q), p->Sub[i].Open_q);
					sQty.TrimLeft(0x30);	sQty.Trim();

					// �Ÿű��� (1.�ż� 2.�ŵ�)
					char cSide;
					switch (p->Sub[i].Bysl_tp[0]) {
					case '1':	cSide = DEF_BUY;	break;
					case '2':	cSide = DEF_SELL;	break;
					}

					// ��հ�
					CString sPrc;
					sPrc.Format("%.*s", sizeof(p->Sub[i].Avgt_p), p->Sub[i].Avgt_p);
					sPrc.Trim();

					int nCnt = PosUpdate(sAcnt, sSymbol, cSide, sPrc.GetBuffer(), atoi(sQty.GetBuffer()));

					char msg[1024];
					sprintf(msg, "[�ܰ�TR����][%s][%s](SIDE:%c)(����:%10.5f)(�Ѱ���:%d)",
						sAcnt.GetBuffer(), sSymbol.GetBuffer(), cSide, atof(sPrc.GetBuffer()), nCnt);
					g_log.log(LOGTP_SUCC, msg);


				} /* End of for */
			} // end of case WM_NOTI_POSITION:
				break;
			} // switch (msg.message)

			
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))

		
	} // while (TRUE)
	
}



/* try catch �� ����*/
void CFBOrdThread::DoProc_RecvAPIReplyWrapper( At0985O_st* p)
{
	__try
	{
		__try
		{
			DoProc_RecvAPIReply(p);
		}
		__except (ReportException(GetExceptionCode(), "CFBOrdThread::DoProc_RecvAPIReplyWrapper", m_szMsg))
		{
			g_log.log(LOGTP_FATAL, m_szMsg);
		}
	}
	__finally
	{

	}
}

/*
	API �� ���� �ֹ�/ü�� ��� ���� ���� �� ó��

	- ESApiDllDlgDlg::OnRecvData
*/

VOID CFBOrdThread::DoProc_RecvAPIReply( At0985O_st* p)
{
	CString sAcnt;
	sAcnt.Format("%.*s", sizeof(p->Account), p->Account);
	sAcnt.Trim();

	CString sFullCode;
	sFullCode.Format("%.*s", sizeof(p->FullCode), p->FullCode);
	CString sSymbol = m_api->GetShortCode(sFullCode);


	int nOrigNo = S2N((char*)p->Orig_ord_no, sizeof(p->Orig_ord_no));
	int nOrdNo = S2N((char*)p->Ord_no, sizeof(p->Ord_no));
	int nRemainQty = S2N((char*)p->RemainQty, sizeof(p->RemainQty));
	
	char msg[1024];
	/*
		�����Ǳ���:������������ ����. SIDE:ü��SIDE 
			-> ADD : ���� ����.
			-> �Ϻ�û�� : ���� �ٸ���.
		�ܰ����:���������Ǽ���
		û�갡�ɼ��� : �ܰ����-��ü�����
		[ü�����][API_����](GCQ7)(�����Ǳ���:1)(SIDE:1)(û�갡�ɼ���:0000000001)(�ܰ����:0000000001)(ü�ᰡ��:1254.90000)(ü�����:1                   )(ü���ȣ:00047)
		[ü�����][API_����](GCQ7)(�����Ǳ���:1)(SIDE:1)(û�갡�ɼ���:0000000001)(�ܰ����:0000000002)(ü�ᰡ��:1255.00000)(ü�����:1                   )(ü���ȣ:00048)
		[ü�����][API_����](GCQ7)(�����Ǳ���:1)(SIDE:1)(û�갡�ɼ���:0000000002)(�ܰ����:0000000003)(ü�ᰡ��:1255.00000)(ü�����:1                   )(ü���ȣ:00049)
		[ü�����][API_����](GCQ7)(�����Ǳ���:1)(SIDE:2)(û�갡�ɼ���:0000000001)(�ܰ����:0000000002)(ü�ᰡ��:1254.90000)(ü�����:1                   )(ü���ȣ:00050)
		[ü�����][API_����](GCQ7)(�����Ǳ���:1)(SIDE:2)(û�갡�ɼ���:0000000000)(�ܰ����:0000000001)(ü�ᰡ��:1254.90000)(ü�����:1                   )(ü���ȣ:00051)
		[ü�����][API_����](GCQ7)(�����Ǳ���:2)(SIDE:2)(û�갡�ɼ���:0000000000)(�ܰ����:0000000000)(ü�ᰡ��:1254.90000)(ü�����:1                   )(ü���ȣ:00052)
	*/
	if (p->ReplyType[0] == DEF_RSLT_MATCH) 
	{
		//sprintf(msg,  "[%s](%s)(�����Ǳ���:%c)(SIDE:%c)(û�갡�ɼ���:%.*s)(�ܰ����:%.*s)(ü�ᰡ��:%10.5f)(ü�����:%.*s)(ü���ȣ:%.*s)"
		//	, sAcnt.GetBuffer()
		//	, sSymbol.GetBuffer()
		//	, p->Open_tp[0]
		//	, p->Side[0]
		//	, sizeof(p->Rsrb_q), p->Rsrb_q
		//	, sizeof(p->Open_q), p->Open_q
		//	, S2D((char*)p->ExecPrice,sizeof(p->ExecPrice))
		//	, sizeof(p->ExecQty), p->ExecQty
		//	, sizeof(p->Trd_no), p->Trd_no
		//);
		//g_log.log(LOGTP_SUCC,  msg);
	}
	else {
		//sprintf(msg, "[%s](%s)(��������:%c)����(%c)(�ֹ�����:%.*s)(�ֹ��ܷ�:%.*s)(�ֹ�����:%10.5f)(�ֹ���ȣ:%d)(���ֹ���ȣ:%d)"
		//	, sAcnt.GetBuffer()
		//	, sSymbol.GetBuffer()
		//	, p->ReplyType[0]
		//	, p->Side[0]
		//	, sizeof(p->Qty), p->Qty
		//	, sizeof(p->RemainQty), p->RemainQty
		//	, S2D((CHAR*)p->Price,sizeof(p->Price))
		//	, nOrdNo
		//	, nOrigNo
		//);
		//g_log.log(LOGTP_SUCC, "%s", msg);
	}


	///////////////////////////////////////////////////////////
	// �ź��ֹ� ó��, 
	if (p->ReplyType[0] == DEF_RSLT_RJCT_NEW ||
		p->ReplyType[0] == DEF_RSLT_RJCT_MDFY ||
		p->ReplyType[0] == DEF_RSLT_RJCT_CNCL
		)
	{
		g_log.log(LOGTP_ERR, "�ֹ��źμ���[%s][���ֹ���ȣ:%d]", sSymbol.GetBuffer(), nOrigNo);
		return;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ű� / ���� / ��� �ֹ�
	int nIdx = -1;
	if (p->ReplyType[0] == DEF_RSLT_NEW || p->ReplyType[0] == DEF_RSLT_MDFY || p->ReplyType[0] == DEF_RSLT_CNCL)
	{
		CString sPrc;
		sPrc.Format("%.*s", sizeof(p->Price), p->Price);
		sPrc.Trim();

		if (p->ReplyType[0] == DEF_RSLT_NEW)
		{
			int nCnt = OrderRecv_Add(sAcnt, sSymbol, nOrdNo, nOrigNo, nRemainQty, p->Side[0], sPrc.GetBuffer());

			sprintf(msg, "[�ű��ֹ�����][%s][%s](�ֹ���ȣ:%d)(SIDE:%c)(����:%10.5f)(�Ѱ���:%d)",
				sAcnt.GetBuffer(),sSymbol.GetBuffer(), nOrdNo, p->Side[0], atof(sPrc.GetBuffer()), nCnt);
			g_log.log(LOGTP_SUCC, msg);
		}
		else if (p->ReplyType[0] == DEF_RSLT_MDFY)
		{
			int nCnt = OrderRecv_Modify(sAcnt, sSymbol, nOrdNo, nOrigNo, nRemainQty, p->Side[0], sPrc.GetBuffer());

			sprintf(msg, "[�����ֹ�����][%s][%s](�ֹ���ȣ:%d)(���ֹ�:%d)(SIDE:%c)(����:%10.5f)(�Ѱ���:%d)",
				sAcnt.GetBuffer(), sSymbol.GetBuffer(), nOrdNo, nOrigNo, p->Side[0], atof(sPrc.GetBuffer()), nCnt);
			g_log.log(LOGTP_SUCC, msg);
		}
		else if (p->ReplyType[0] == DEF_RSLT_CNCL)
		{
			int nCnt = OrderRecv_Cncl(sAcnt, nOrigNo);
		
			sprintf(msg, "[����ֹ�����][%s][%s](�ֹ���ȣ:%d)(���ֹ�:%d)(SIDE:%c)(�Ѱ���:%d)",
				sAcnt.GetBuffer(), sSymbol.GetBuffer(), nOrdNo, nOrigNo, p->Side[0], nCnt);
			g_log.log(LOGTP_SUCC, msg);
		}

	} // if (p->ReplyType[0] == DEF_RSLT_NEW || p->ReplyType[0] == DEF_RSLT_MDFY || p->ReplyType[0] == DEF_RSLT_CNCL)

	if (p->ReplyType[0] == DEF_RSLT_MATCH)
	{
		// ü������ ���� �ֹ��Ϸῡ ���� ���� ����
		int nOrdNo = S2N((char*)p->Ord_no, sizeof(p->Ord_no));		
		int nCnt = OrderRecv_Cncl(sAcnt, nOrdNo);

		//sprintf(msg, "[%s](%s)[ü�����]ü������Ͽ� �ֹ���������(�ֹ���ȣ:%d)(�����ֹ�����:%d)",
		//	sAcnt.GetBuffer(), sSymbol.GetBuffer(), nOrdNo, nCnt);
		//g_log.log(LOGTP_SUCC, msg);

		int nOpenQty = S2N((char*)p->Open_q, sizeof(p->Open_q));
		char zExecPrc[128]; sprintf(zExecPrc, "%.*s", sizeof(p->ExecPrice), p->ExecPrice);


		// �����ֹ� or �Ϻ�û�� ���������� ����;
		if (nOpenQty > 0)
		{
			int nCnt = PosUpdate(sAcnt, sSymbol, p->Side[0], zExecPrc, nOpenQty);
			sprintf(msg, "[ü�����-�ܰ�����][%s][%s](��������������:%d)(�ܰ����:%d)(����:%c)(����:%10.5f)",
				sAcnt.GetBuffer(), sSymbol.GetBuffer(), nCnt, nOpenQty, p->Side[0], atof(zExecPrc));
			g_log.log(LOGTP_SUCC, msg);
		}

		// ����û��. �ش� ���� �ܰ� ���� ����
		else
		{
			// ���԰��ر��� û���ֹ� �ߺ����� flag �ʱ�ȭ
			int nCnt = PosDelete(sAcnt, sSymbol);
			sprintf(msg, "[ü�����-����û��][%s][%s](��������������:%d)",sAcnt.GetBuffer(), sSymbol.GetBuffer(), nCnt);
			g_log.log(LOGTP_SUCC, msg);
		}
	} // if (p->ReplyType[0] == DEF_RSLT_MATCH)
}


/*
m_mapOrd : acnt-sub, acnt-sub
sub : ordno-ord, ordno-pos

CString sSymbol;
int nOrdNo;
int nOrigNo;
int nRemainQty;
char cSide[1];
char zPrc[LEN_PRC + 1];
*/
int CFBOrdThread::OrderRecv_Add(_In_ CString& sAcnt, CString sSymbol, int nOrdNo, int nOrigNo, 
							int nRemainQty, char cSide, char *pzPrc)
{
	int nCnt = 0;
	ORD_INFO* pOrd = new ORD_INFO;
	pOrd->sSymbol = sSymbol;
	pOrd->nOrdNo = nOrdNo;
	pOrd->nOrigNo = nOrigNo;
	pOrd->nRemainQty = nRemainQty;
	pOrd->cSide[0] = cSide;
	strcpy(pOrd->zPrc, pzPrc);

	LOCK_ORD();

	MAPIT_ORD itOrd = m_mapOrd.find(sAcnt);
	

	// �ش���������� ������ add
	if (itOrd == m_mapOrd.end())
	{
		MAP_ORDSUB sub;
		sub[nOrdNo] = pOrd;
		m_mapOrd[sAcnt] = sub;
		nCnt = 1;
	}
	else
	{
		MAP_ORDSUB sub = (*itOrd).second;
		sub[nOrdNo] = pOrd;
		m_mapOrd[sAcnt] = sub;
		nCnt = sub.size();
	}

	UNLOCK_ORD();
	return nCnt;
}


/*
m_mapOrd : acnt-sub, acnt-sub
sub : ordno-ord, ordno-pos
*/
int CFBOrdThread::OrderRecv_Modify(_In_ CString& sAcnt, CString sSymbol, int nOrdNo, int nOrigNo, int nRemainQty, char cSide, char *pzPrc)
{
	int nCnt = 0;

	ORD_INFO* pOrd = new ORD_INFO;
	pOrd->sSymbol = sSymbol;
	pOrd->nOrdNo = nOrdNo;
	pOrd->nOrigNo = nOrigNo;
	pOrd->nRemainQty = nRemainQty;
	pOrd->cSide[0] = cSide;
	strcpy(pOrd->zPrc, pzPrc);

	LOCK_ORD();
	MAPIT_ORD itOrd = m_mapOrd.find(sAcnt);

	// �ش� ������ �ֹ��� ������ �����ֹ����׳� add �Ѵ�.
	if (itOrd == m_mapOrd.end())
	{
		MAP_ORDSUB sub;
		sub[pOrd->nOrdNo] = pOrd;
		m_mapOrd[sAcnt] = sub;
		nCnt = 1;
	}
	else
	{
		MAP_ORDSUB sub = (*itOrd).second;
		MAPIT_ORDSUB itSub = sub.find(pOrd->nOrigNo);

		// �������ֹ��� ������ ���� �� �����ֹ� add
		if (itSub != sub.end()) 
		{
			delete (*itSub).second;	// ORD_INFO*	
			sub.erase(itSub);
		}
		// ���ֹ��� ������ �����ֹ��� �׳� add �Ѵ�.
		sub[pOrd->nOrdNo] = pOrd;	
		m_mapOrd[sAcnt] = sub;
		
		nCnt = sub.size();
	}

	UNLOCK_ORD();

	return nCnt;
}


/*
// Ư���ֹ���ȣ ���
m_mapOrd : acnt-sub, acnt-sub
sub : ordno-ord, ordno-ord
*/
BOOL CFBOrdThread::OrderSend_Cncl_OrdNo(_In_ CString& sAcnt, _In_ int nOrdNo, _In_ CString sAcntPwd)
{
	MAPIT_ORD itOrd = m_mapOrd.find(sAcnt);
	if (itOrd != m_mapOrd.end())
	{
		MAP_ORDSUB sub = (*itOrd).second;
		MAPIT_ORDSUB itSub = sub.find(nOrdNo);

		// ���ֹ��� ������
		if (itSub != sub.end())
		{
			ORD_INFO* p = (*itSub).second;
			DoOrder(sAcnt, ORD_TYPE_CNCL, NULL, p->cSide[0], p->nRemainQty, p->sSymbol, p->nOrdNo, sAcntPwd);
		}
	}

	return TRUE;
}

BOOL CFBOrdThread::OrderSend(ORDSEND_TYPE tp, _In_ CString& sAcnt, _In_ CString& sSymbol, _In_ CString sAcntPwd, int nOrdNo)
{
	BOOL bRet;
	LOCK_ORD();
	switch (tp)
	{
	case ORDSEND_CNCL_ORDNO:
		bRet = OrderSend_Cncl_OrdNo(sAcnt, nOrdNo, sAcntPwd);
		break;
	case ORDSEND_CNCL_SYMBOL:
		bRet = OrderSend_Cncl_Symbol(sAcnt, sSymbol, sAcntPwd);
		break;
	case ORDSEND_CNCL_ACNT:
		bRet = OrderSend_Cncl_Acnt(sAcnt, sAcntPwd);
		break;
	case ORDSEND_CLR_SYMBOL:
		LOCK_POS();
		bRet = OrderSend_Clr_Symbol(sAcnt, sSymbol, sAcntPwd);
		UNLOCK_POS();
		break;
	case ORDSEND_CLR_ACNT:
		LOCK_POS();
		bRet = OrderSend_Clr_Acnt(sAcnt, sAcntPwd);
		UNLOCK_POS();
		break;
	}
	UNLOCK_ORD();
	return bRet;
}

/*
// Ư������ ���
m_mapOrd : acnt-sub, acnt-sub
sub : ordno-ord, ordno-ord
*/
BOOL CFBOrdThread::OrderSend_Cncl_Symbol(_In_ CString& sAcnt, _In_ CString& sSymbol, _In_ CString sAcntPwd)
{
	MAPIT_ORD itOrd = m_mapOrd.find(sAcnt);
	if (itOrd == m_mapOrd.end())
	{
		return FALSE;
	}
	MAP_ORDSUB sub = (*itOrd).second;
	if (sub.size() == 0)
	{
		m_mapOrd.erase(sAcnt);
		return FALSE;
	}

	BOOL bFind = FALSE;
	MAPIT_ORDSUB itSub;
	for (itSub = sub.begin(); itSub != sub.end(); itSub++)
	{
		ORD_INFO* p = (*itSub).second;
		if (p->sSymbol.Compare(sSymbol) != 0)
		{
			continue;
		}
		DoOrder(sAcnt, ORD_TYPE_CNCL, NULL, p->cSide[0], p->nRemainQty, p->sSymbol, p->nOrdNo, sAcntPwd);
		bFind = TRUE;
	}

	int nCnt = sub.size();
	//if(nCnt==0)	m_mapOrd.erase(sAcnt);
	
	return bFind;
}

/*
// Ư������ ���
m_mapOrd : acnt-sub, acnt-sub
sub : ordno-ord, ordno-ord
*/
BOOL CFBOrdThread::OrderSend_Cncl_Acnt(_In_ CString& sAcnt, _In_ CString sAcntPwd)
{
	MAPIT_ORD itOrd = m_mapOrd.find(sAcnt);
	if (itOrd == m_mapOrd.end())
	{
		return FALSE;
	}
	MAP_ORDSUB sub = (*itOrd).second;
	if (sub.size() == 0)
	{
		//m_mapOrd.erase(sAcnt);
		return FALSE;
	}

	BOOL bFind = FALSE;
	MAPIT_ORDSUB itSub;
	for (itSub = sub.begin(); itSub != sub.end();itSub++)
	{
		ORD_INFO* p = (*itSub).second;
		CString symbol = p->sSymbol;
		DoOrder(sAcnt, ORD_TYPE_CNCL, NULL, p->cSide[0], p->nRemainQty,symbol , p->nOrdNo, sAcntPwd);
		bFind = TRUE;
	}

	//m_mapOrd.erase(sAcnt);

	return bFind;
}

/*
m_mapOrd : acnt-sub, acnt-sub
sub : ordno-ord, ordno-ord
*/
int CFBOrdThread::OrderRecv_Cncl(_In_ CString& sAcnt, _In_ int nOrigNo)
{
	int nCnt = 0;
	LOCK_ORD();
	MAPIT_ORD itOrd = m_mapOrd.find(sAcnt);

	if (itOrd != m_mapOrd.end())
	{
		MAP_ORDSUB sub = (*itOrd).second;
		MAPIT_ORDSUB itSub = sub.find(nOrigNo);

		// ���ֹ��� ������ ����
		if (itSub != sub.end())
		{
			delete (*itSub).second;	// ORD_INFO*
			sub.erase(itSub);
		}

		nCnt = sub.size();
		// �ش������ ��ü���ֹ��� ������
		if (nCnt == 0)
		{
			m_mapOrd.erase(sAcnt);
		}
		else
		{
			m_mapOrd[sAcnt] = sub;
		}
	}
	UNLOCK_ORD();
	return nCnt;
}

/*
	m_mapPos : acnt-sub, acnt-sub
	sub : symbol-pos, symbol-pos
*/
int CFBOrdThread::PosDelete(_In_ CString& sAcnt, _In_ CString sSymbol)
{
	LOCK_POS();
	MAPIT_POS itPos = m_mapPos.find(sAcnt);

	// �ش� ������ �������� ������ �׳� ������.
	if (itPos == m_mapPos.end())
	{
		UNLOCK_POS();
		return 0;
	}
	MAP_POSSUB sub = (*itPos).second;
	MAPIT_POSSUB itSub = sub.find(sSymbol);

	// �����ڷᰡ ������ �����.
	if (itSub != sub.end())
	{
		delete (*itSub).second;
		sub.erase(sSymbol);
	}

	// �ش� ������ �������� �ϳ��� ������ ���� ����
	int nCnt = sub.size();
	if (nCnt == 0)
	{
		m_mapPos.erase(sAcnt);
	}
	else
	{
		m_mapPos[sAcnt] = sub;
	}
	
	UNLOCK_POS();

	return nCnt;
}

/*
m_mapPos : acnt-sub, acnt-sub
sub : symbol-pos, symbol-pos
*/
int CFBOrdThread::PosUpdate(_In_ CString& sAcnt, _In_ CString sSymbol, _In_ char cSide, _In_ char* pzPrc, _In_ int nOpenQty)
{
	int nCnt = 0;
	LOCK_POS();
	MAPIT_POS itPos = m_mapPos.find(sAcnt);

	POS_INFO* p = new POS_INFO;
	p->sSymbol = sSymbol;
	p->nOpenQty = nOpenQty;
	p->cSide[0] = cSide;
	strcpy(p->zPosPrc, pzPrc);


	// �ش� ������ �������� ������ �׳� add �Ѵ�.
	if (itPos == m_mapPos.end())
	{
		MAP_POSSUB sub;
		sub[sSymbol] = p;
		m_mapPos[sAcnt] = sub;
		nCnt = sub.size();
	}
	else
	{
		MAP_POSSUB sub = (*itPos).second;		
		MAPIT_POSSUB itSub = sub.find(sSymbol);

		// �����ڷᰡ ������ ������ ����� add
		if (itSub != sub.end())
		{
			delete (*itSub).second;
			sub.erase(itSub);
		}
		// add �Ѵ�.
		sub[sSymbol] = p;
		m_mapPos[sAcnt] = sub;
		nCnt = sub.size();
	}
	 
	UNLOCK_POS();

	return nCnt;
}

VOID CFBOrdThread::OrdSend_General(_In_ CString& sAcnt, _In_ CString& sSymbol, _In_ CString sAcntPwd
	, _In_ int nQty, char* zPrc, char cSide)
{
	
	DoOrder(sAcnt, ORD_TYPE_NEW, zPrc, cSide, nQty, sSymbol, 0, sAcntPwd);
}


/*
	Ư������ û��
*/
BOOL CFBOrdThread::OrderSend_Clr_Acnt(_In_ CString& sAcnt, _In_ CString sAcntPwd)
{
	BOOL bFind = FALSE;
	MAPIT_POS itPos = m_mapPos.find(sAcnt);
	if (itPos != m_mapPos.end())
	{
		MAP_POSSUB sub = (*itPos).second;
		if (sub.size() > 0)
		{
			MAPIT_POSSUB itSub;
			for (itSub = sub.begin(); itSub != sub.end(); itSub++)
			{
				POS_INFO* p = (*itSub).second;
				
				char cSide = (p->cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
				DoOrder(sAcnt, ORD_TYPE_NEW, NULL/*���尡*/, cSide, p->nOpenQty, p->sSymbol, 0, sAcntPwd);
				bFind = TRUE;
			}
		}
	}
	//m_mapPos.erase(sAcnt);


	return bFind;
}


/*
Ư������ û��
*/
BOOL CFBOrdThread::OrderSend_Clr_Symbol(_In_ CString& sAcnt, _In_ CString& sSymbol, _In_ CString sAcntPwd)
{
	BOOL bFind = FALSE;

	MAPIT_POS itPos = m_mapPos.find(sAcnt);
	if (itPos != m_mapPos.end())
	{
		MAP_POSSUB sub = (*itPos).second;
		if (sub.size() > 0)
		{
			MAPIT_POSSUB itSub;
			for (itSub = sub.begin(); itSub != sub.end(); itSub++)
			{
				POS_INFO* p = (*itSub).second;
				if (p->sSymbol.Compare(sSymbol) != 0)
				{
					continue;
				}

				char cSide = (p->cSide[0] == DEF_BUY) ? DEF_SELL : DEF_BUY;
				DoOrder(sAcnt, ORD_TYPE_NEW, NULL/*���尡*/, cSide, p->nOpenQty, p->sSymbol, 0, sAcntPwd);
				bFind = TRUE;
			}

		}
	}
	return bFind;
}



VOID	CFBOrdThread::DoOrder(_In_ CString& sAcnt, const char cOrdType, const char* zPrc, 
							const char cSide, const int nQty, _In_ CString& sSymbol, const int nOrigNo, _In_ CString sAcntPwd)
{
	char			szBuff[4096];
	lpEs5601I_st	lpTrIn = (lpEs5601I_st)szBuff;

	memset(szBuff, 0x20, 4096);

	char temp[128];
	sprintf(temp, "%010.0f", (double)(unsigned long)m_hwd);
	memcpy(lpTrIn->Head.WinId, temp, strlen(temp));

	// ���¹�ȣ
	memcpy(lpTrIn->Account, sAcnt, sAcnt.GetLength());

	// ��й�ȣ
	m_api->GetEncodePassword(lpTrIn->Pass, sizeof(lpTrIn->Pass), sAcnt, sAcntPwd);

	// �ֹ�����	(1.�ű��ֹ� 2.�����ֹ� 3.����ֹ�)
	lpTrIn->Order_Type[0] = cOrdType;

	if (cOrdType == ORD_TYPE_CNCL) {
		sprintf(temp, "%d", nOrigNo);
		memcpy(lpTrIn->Order_Org_No, temp, min(sizeof(lpTrIn->Order_Org_No), strlen(temp)));
	}
		

	// �����ڵ�
	memcpy(lpTrIn->ShortCode, sSymbol.GetBuffer(), sSymbol.GetLength());

	// �ŵ��ż����� (1.�ż� 2.�ŵ�). 
	lpTrIn->BuySell_Type[0] = cSide;

	// ��������	(1.������ 2.���尡). 
	// ü������ (���尡�ϰ��(3) �������ϰ��(1))
	if (NULL == zPrc || atof(zPrc)==0)
	{
		if (cOrdType == ORD_TYPE_NEW) 
		{
			lpTrIn->Price_Type[0] = '2';
			lpTrIn->Trace_Type[0] = '3';
			
		}
		//if (cOrdType == ORD_TYPE_NEW) 
		//{
		//	lpTrIn->Price_Type[0] = '1';
		//	lpTrIn->Trace_Type[0] = '1';
		//}
		memset(lpTrIn->Order_Price, 0x20, sizeof(lpTrIn->Order_Price));
	}
	else 
	{
		lpTrIn->Price_Type[0] = '1';
		lpTrIn->Trace_Type[0] = '1';
		if (cOrdType == ORD_TYPE_CNCL)
			memset(lpTrIn->Order_Price, 0x20, LEN_PRC);
		else
			memcpy(lpTrIn->Order_Price, zPrc, LEN_PRC);
	}
	// �ֹ�����
	sprintf(temp, "%*d", sizeof(lpTrIn->Order_Volume), nQty);
	memcpy(lpTrIn->Order_Volume, temp, sizeof(lpTrIn->Order_Volume));
	
	// ����ֹ�����
	m_api->GetCommunicationType(lpTrIn->Order_Comm_Type, sizeof(lpTrIn->Order_Comm_Type));

	// �ֹ��������� (1.�Ϲ��ֹ� 2.STOP�ֹ�)
	lpTrIn->Stop_Type[0] = '1';


	// �ֹ����۱��� 
	if (cOrdType == ORD_TYPE_NEW)		lpTrIn->Oper_Type[0] = '1';
	else if (cOrdType == ORD_TYPE_CNCL)	lpTrIn->Oper_Type[0] = '3';

	APILOCK();
	if(cOrdType==ORD_TYPE_NEW)
		m_api->SendTrData(ESID_5601, szBuff, SZ_Es5601I);
	else if (cOrdType == ORD_TYPE_CNCL)
		m_api->SendTrData(ESID_5603, szBuff, SZ_Es5601I);
	APIUNLOCK();

	char msg[1024];
	sprintf(msg, "[�ֹ�����][%s][%s](�ֹ�����:%c)(����:%.*s)(����:%.*s)(PRC:%.*s)(���ֹ�:%.*s)",
		sAcnt.GetBuffer(), sSymbol.GetBuffer(), 
		lpTrIn->Oper_Type[0],
		sizeof(lpTrIn->BuySell_Type), lpTrIn->BuySell_Type,
		sizeof(lpTrIn->Order_Volume), lpTrIn->Order_Volume,
		sizeof(lpTrIn->Order_Price), lpTrIn->Order_Price,
		sizeof(lpTrIn->Order_Org_No), lpTrIn->Order_Org_No
	);
	g_log.log(LOGTP_SUCC, "%s", msg);
}


VOID CFBOrdThread::APIRecvReal(At0985O_st* p)
{
	PostThreadMessage(GetMyThreadID(), WM_NOTI_MATCH, 0, (LPARAM)p);
}

VOID CFBOrdThread::APIRecvTr(int nGb, void* p)
{
	if(nGb==1)
		PostThreadMessage(GetMyThreadID(), WM_NOTI_REMAINORD, 0, (LPARAM)p);
	else
		PostThreadMessage(GetMyThreadID(), WM_NOTI_POSITION, 0, (LPARAM)p);
}

BOOL CFBOrdThread::Initialize()
{
	return TRUE;
}
