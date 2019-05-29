// BARelayLib.cpp : Defines the exported functions for the DLL application.
//


#define BA_RELAY_EXPORTS

#include "BARelayEA.h"
#include "../Common/NanoQPipe.h"
#include "../Common/NanoQPubSub.h"
#include "./utils/err.c"	//important

CNanoQWriter	g_Sender;
CNanoQSub		g_Subscriber;


BOOL	g_bInitSendChannel = FALSE;
BOOL	g_bInitSlaveChannel = FALSE;;
char	g_zSenderMsg[1024], g_zReceiverMsg[1024];


char* Sender_GetMsg()	{ return g_zSenderMsg;}
char* Receiver_GetMsg(){ return g_zReceiverMsg; }

char* Sender_ChannelNm() { return g_Sender.ChannelNm(); }
char* Receiver_ChannelNm() { return g_Subscriber.ChannelNm(); }

int Sender_InitChannel(int nSendTimeout)
{
	if (!g_Sender.Begin(NULL, nSendTimeout, TP_INTERPROC))
	{
		g_bInitSendChannel = FALSE;
		sprintf(g_zSenderMsg, "[%s]InitSendChannel Failed:%s", 
			g_Sender.ChannelNm(), g_Sender.GetMsg());
		return (int)(_BA_RELAY::Q_ERROR);
	}

	if( !g_Sender.Connect())
	{
		g_bInitSendChannel = FALSE;
		sprintf(g_zSenderMsg, "[%s]Connect Failed:%s",
			g_Sender.ChannelNm(), g_Sender.GetMsg());
		return (int)(_BA_RELAY::Q_ERROR);
	}

	g_bInitSendChannel = TRUE;
	return (int)(_BA_RELAY::Q_SUCCESS);
}


int Sender_DeInitChannel()
{
	g_bInitSendChannel = FALSE;
	g_Sender.End();
	return _BA_RELAY::Q_SUCCESS;
}

int Receiver_InitSlaveChannel(char* pzMasterAccNo, int nSendTimeout)
{
	if (!g_Subscriber.Begin(pzMasterAccNo, nSendTimeout, TP_INTERPROC))
	{
		g_bInitSlaveChannel = FALSE;
		sprintf(g_zReceiverMsg, "[%s]InitSlaveChannel Failed:%s", 
			g_Subscriber.ChannelNm(), g_Subscriber.GetMsg());
		return _BA_RELAY::Q_ERROR;
	}

	if(!g_Subscriber.Connect())
	{
		g_bInitSlaveChannel = FALSE;
		sprintf(g_zReceiverMsg, "[%s]Connect Failed:%s",
			g_Subscriber.ChannelNm(), g_Subscriber.GetMsg());
		return _BA_RELAY::Q_ERROR;
	}

	g_bInitSlaveChannel = TRUE;
	return _BA_RELAY::Q_SUCCESS;
}

int Receiver_DeInitSlaveChannel()
{
	g_bInitSlaveChannel = FALSE;
	g_Subscriber.End();
	return _BA_RELAY::Q_SUCCESS;
}


int Sender_SendData(char* pData, int nSendLen)
{
	SYSTEMTIME st;
	char zTm[32];
	GetLocalTime(&st);
	sprintf(zTm, "%04d%02d%02d_%02d%02d%02d%03d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	_BA_RELAY::Header* pH = (_BA_RELAY::Header*)pData;
	memcpy(pH->Tm, zTm, sizeof(pH->Tm));

	int nRet, nErrCode;
	nRet = g_Sender.SendData(pData, nSendLen, &nErrCode);
	if (nRet == _BA_RELAY::Q_ERROR)
	{
		if (nErrCode == _BA_RELAY::Q_ERROR)
		{
			//sprintf("SendData Failed:%s", g_Sender.GetMsg());
			return _BA_RELAY::Q_ERROR;
		}
		else if (nErrCode == _BA_RELAY::Q_TIMEOUT)
		{
			return _BA_RELAY::Q_TIMEOUT;
		}
	}
	return nRet;	// SEND length
}

/*
		char Type[1];
		char System[5];
		char Broker[5];
		char AccNo[20];*/
int Sender_RegisterAsMaster(char* pzMyAccNo, /*out*/char* pSendBuf, int nBufLen)
{
	ZeroMemory(pSendBuf, nBufLen);
	int nStructLen = sizeof(_BA_RELAY::PT_REG_MASTER);
	_BA_RELAY::PT_REG_MASTER * p = (_BA_RELAY::PT_REG_MASTER *)pSendBuf;
	memset(p, 0x20, nStructLen);
	memcpy(p->header.Code, _BA_RELAY::CODE_REG_MASTER, strlen(_BA_RELAY::CODE_REG_MASTER));
	p->header.Type[0] = _BA_RELAY::TP_COMMAND;
	memcpy(p->header.AccNo, pzMyAccNo, strlen(pzMyAccNo));
	memcpy(p->MasterAccNo, pzMyAccNo, strlen(pzMyAccNo));

	p->Action[0] = TP_REG;

	int nRet = Sender_SendData(pSendBuf, nStructLen);
	if (nRet == _BA_RELAY::Q_ERROR) {
		sprintf(g_zSenderMsg, "[%s]RegisterAsMaster failed(%s)", g_Sender.ChannelNm(), g_Sender.GetMsg());
	}
	return nRet;
}

int Sender_RegisterAsMasterEA(char* pzMyAccNo, /*out*/char* pSendBuf)
{
	//ZeroMemory(pSendBuf, nBufLen);
	int nStructLen = sizeof(_BA_RELAY::PT_REG_MASTER);
	_BA_RELAY::PT_REG_MASTER * p = (_BA_RELAY::PT_REG_MASTER *)pSendBuf;
	memset(p, 0x20, nStructLen);
	memcpy(p->header.Code, _BA_RELAY::CODE_REG_MASTER, strlen(_BA_RELAY::CODE_REG_MASTER));
	p->header.Type[0] = _BA_RELAY::TP_COMMAND;
	memcpy(p->header.AccNo, pzMyAccNo, strlen(pzMyAccNo));
	memcpy(p->MasterAccNo, pzMyAccNo, strlen(pzMyAccNo));

	p->Action[0] = TP_REG;

	int nRet = Sender_SendData(pSendBuf, nStructLen);
	if (nRet == _BA_RELAY::Q_ERROR) {
		sprintf(g_zSenderMsg, "[%s]RegisterAsMaster failed(%s)", g_Sender.ChannelNm(), g_Sender.GetMsg());
	}
	return nRet;
}



int Sender_UnRegisterAsMaster(char* pzMyAccNo, /*out*/char* pSendBuf, int nBufLen)
{
	ZeroMemory(pSendBuf, nBufLen);
	int nStructLen = sizeof(_BA_RELAY::PT_REG_MASTER);
	_BA_RELAY::PT_REG_MASTER * p = (_BA_RELAY::PT_REG_MASTER *)pSendBuf;
	memset(p, 0x20, nStructLen);
	memcpy(p->header.Code, _BA_RELAY::CODE_REG_MASTER, strlen(_BA_RELAY::CODE_REG_MASTER));
	p->header.Type[0] = _BA_RELAY::TP_COMMAND;
	memcpy(p->header.AccNo, pzMyAccNo, strlen(pzMyAccNo));
	memcpy(p->MasterAccNo, pzMyAccNo, strlen(pzMyAccNo));

	p->Action[0] = TP_UNREG;

	int nRet = Sender_SendData(pSendBuf, nStructLen);
	if (nRet == _BA_RELAY::Q_ERROR) {
		sprintf(g_zSenderMsg, "[%s]UnRegisterAsMaster failed(%s)",
			g_Sender.ChannelNm(), g_Sender.GetMsg());
	}
	return nRet;
}

int Sender_RegisterAsSlave(char* pzMyAccNo, char* pzMasterAccNo, /*out*/char* pSendBuf, int nBufLen)
{
	ZeroMemory(pSendBuf, nBufLen);
	int nStructLen = sizeof(_BA_RELAY::PT_REG_SLAVE);
	_BA_RELAY::PT_REG_SLAVE * p = (_BA_RELAY::PT_REG_SLAVE *)pSendBuf;
	memset(p, 0x20, nStructLen);
	memcpy(p->header.Code, _BA_RELAY::CODE_REG_SLAVE, strlen(_BA_RELAY::CODE_REG_SLAVE));
	p->header.Type[0] = _BA_RELAY::TP_COMMAND;
	memcpy(p->header.AccNo, pzMyAccNo, strlen(pzMyAccNo));
	memcpy(p->MasterAccNo, pzMasterAccNo, strlen(pzMasterAccNo));

	p->Action[0] = TP_REG;

	int nRet = Sender_SendData(pSendBuf, nStructLen);
	if (nRet == _BA_RELAY::Q_ERROR) {
		sprintf(g_zSenderMsg, "[%s]RegisterAsSlave failed(%s)",
			g_Sender.ChannelNm(), g_Sender.GetMsg());
	}
	return nRet;
}

int Sender_UnRegisterAsSlave(char* pzMyAccNo, char* pzMasterAccNo, /*out*/char* pSendBuf, int nBufLen)
{
	ZeroMemory(pSendBuf, nBufLen);
	int nStructLen = sizeof(_BA_RELAY::PT_REG_SLAVE);
	_BA_RELAY::PT_REG_SLAVE * p = (_BA_RELAY::PT_REG_SLAVE *)pSendBuf;
	memset(p, 0x20, nStructLen);
	memcpy(p->header.Code, _BA_RELAY::CODE_REG_SLAVE, strlen(_BA_RELAY::CODE_REG_SLAVE));
	p->header.Type[0] = _BA_RELAY::TP_COMMAND;
	memcpy(p->header.AccNo, pzMyAccNo, strlen(pzMyAccNo));
	memcpy(p->MasterAccNo, pzMasterAccNo, strlen(pzMasterAccNo));

	p->Action[0] = TP_UNREG;

	int nRet = Sender_SendData(pSendBuf, nStructLen);
	if (nRet == _BA_RELAY::Q_ERROR) {
		sprintf(g_zSenderMsg, "[%s]UnRegisterAsSlave failed(%s)",
			g_Sender.ChannelNm(), g_Sender.GetMsg());
	}
	return nRet;
}



int Sender_SendOrder(
	char* pzMyAccNo
	, char* pzMasterAccNo
	, int nOrdNo
	, char* pzSymbol
	, char* pAction		// O:Open, C:Close
	, char* pSide		// B:Buy, S:Sell
	, int nDecimalCnt
	, double dOrdPrc
	, double dSLPrc
	, double dPTPrc
	, double dLots
	, char* psOrdTp		// MK(Market), PD(Pending), SL, TP
	, char* psOrdTm
	, double dOpendedPrc
	, double dOpenedLots
	, char* psOpenedTm
	, double dProfit
	, double dSwap
	, /*out*/char* pSendBuf
	, int nBufLen
)
{
	char temp[128];
	ZeroMemory(pSendBuf, nBufLen);
	int nStructLen = sizeof(_BA_RELAY::PT_MASTER_ORD);
	_BA_RELAY::PT_MASTER_ORD * p = (_BA_RELAY::PT_MASTER_ORD *)pSendBuf;
	memset(p, 0x20, nStructLen);
	memcpy(p->header.Code, _BA_RELAY::CODE_MASTER_ORDER, strlen(_BA_RELAY::CODE_MASTER_ORDER));
	p->header.Type[0] = _BA_RELAY::TP_ORDER;
	memcpy(p->header.AccNo, pzMyAccNo, strlen(pzMyAccNo));
	memcpy(p->MasterAccNo, pzMasterAccNo, strlen(pzMasterAccNo));
	
	sprintf(temp, "%d", nOrdNo);
	memcpy(p->OrdNo, temp, strlen(temp));
	memcpy(p->Symbol, pzSymbol, strlen(pzSymbol));
	memcpy(p->Action, pAction, 1);
	memcpy(p->Side, pSide, 1);
		
	sprintf(temp, "%.*f", nDecimalCnt, dOrdPrc);
	memcpy(p->OrdPrc, temp, strlen(temp));

	sprintf(temp, "%.*f", nDecimalCnt, dSLPrc);
	memcpy(p->SLPrc, temp, strlen(temp));

	sprintf(temp, "%.*f", nDecimalCnt, dPTPrc);
	memcpy(p->PTPrc, temp, strlen(temp));

	sprintf(temp, "%.*f", VOL_DECIMAL_CNT, dLots);
	memcpy(p->Lots, temp, strlen(temp));
	
	memcpy(p->OrdTp, psOrdTp, min(sizeof(p->OrdTp),strlen(psOrdTp)));

	memcpy(p->OrdTm, psOrdTm, strlen(psOrdTm));
	
	sprintf(temp, "%.*f", nDecimalCnt, dOpendedPrc);
	memcpy(p->OpendedPrc, temp, strlen(temp));

	sprintf(temp, "%.*f", nDecimalCnt, dOpendedPrc);
	memcpy(p->OpenedLots, temp, strlen(temp));

	if(psOpenedTm)
		memcpy(p->OpenedTm, psOpenedTm, strlen(psOpenedTm));

	sprintf(temp, "%.2f", dProfit);
	memcpy(p->Profit, temp, strlen(temp));

	sprintf(temp, "%.2f", dSwap);
	memcpy(p->Swap, temp, strlen(temp));


	int nRet = Sender_SendData(pSendBuf, nStructLen);
	if (nRet == _BA_RELAY::Q_ERROR) {
		sprintf(g_zSenderMsg, "[%s]SendOrder failed(%s)",
			g_Sender.ChannelNm(), g_Sender.GetMsg());
	}
	return nRet;
}


int Receiver_RecvData(/*out*/ char* pData, int nBuffLen)
{
	int nRet, nErrCode;
	ZeroMemory(pData, nBuffLen);
	nRet = g_Subscriber.RecvData(pData, nBuffLen, &nErrCode);
	if (nRet == Q_ERROR)
	{
		if (nErrCode == Q_TIMEOUT)
			nRet = Q_TIMEOUT;
		else {
			sprintf(g_zReceiverMsg, "[%s]SendOrder failed(%s)",
				g_Subscriber.ChannelNm(), g_Subscriber.GetMsg());
		}
	}
	return nRet;
}

