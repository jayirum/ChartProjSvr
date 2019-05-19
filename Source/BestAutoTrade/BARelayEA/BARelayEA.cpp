// BARelayLib.cpp : Defines the exported functions for the DLL application.
//


#define BARELAYEZ_EXPORTS

#include "BARelayEA.h"
#include "../Common/NanoQPipe.h"
#include "../Common/NanoQPubSub.h"
#include "./utils/err.c"	//important

CNanoQWriter	g_Sender;
CNanoQSub		g_Subscriber;


BOOL	g_bInitSendChannel = FALSE;
BOOL	g_bInitSlaveChannel = FALSE;;
char	g_zMsg[1024];


int InitSendChannel(int nSendTimeout)
{
	if (!g_Sender.Begin(NULL, nSendTimeout, TP_INTERPROC))
	{
		g_bInitSendChannel = FALSE;
		sprintf("InitSendChannel Failed:%s", g_Sender.GetMsg());
		return (int)(_BARELAYEA::Q_ERROR);
	}
	g_bInitSendChannel = TRUE;
	return (int)(_BARELAYEA::Q_SUCCESS);
}


int DeInitSendChannel()
{
	g_bInitSendChannel = FALSE;
	g_Sender.End();
	return _BARELAYEA::Q_SUCCESS;
}

int InitSlaveChannel(char* pzMasterAccNo, int nSendTimeout)
{
	if (!g_Subscriber.Begin(pzMasterAccNo, nSendTimeout, TP_INTERPROC))
	{
		g_bInitSlaveChannel = FALSE;
		sprintf("InitSlaveChannel Failed:%s", g_Subscriber.GetMsg());
		return _BARELAYEA::Q_ERROR;
	}
	g_bInitSlaveChannel = TRUE;
	return _BARELAYEA::Q_SUCCESS;
}

int DeInitSlaveChannel()
{
	g_bInitSlaveChannel = FALSE;
	g_Subscriber.End();
	return _BARELAYEA::Q_SUCCESS;
}


int SendData(char* pData, int nSendLen)
{
	int nRet, nErrCode;
	nRet = g_Sender.SendData(pData, nSendLen, &nErrCode);
	if (nRet == _BARELAYEA::Q_ERROR)
	{
		if (nErrCode == _BARELAYEA::Q_ERROR)
		{
			//sprintf("SendData Failed:%s", g_Sender.GetMsg());
			return _BARELAYEA::Q_ERROR;
		}
		else if (nErrCode == _BARELAYEA::Q_TIMEOUT)
		{
			return _BARELAYEA::Q_TIMEOUT;
		}
	}
	return nRet;	// receive length
}

/*
		char Type[1];
		char System[5];
		char Broker[5];
		char AccNo[20];*/
int RegisterAsMaster(char* pzMyAccNo)
{
	char zBuffer[1024] = { 0, };
	int nStructLen = sizeof(_BARELAYEA::PACK_REG_MASTER);
	_BARELAYEA::PACK_REG_MASTER * p = (_BARELAYEA::PACK_REG_MASTER *)zBuffer;
	memset(p, 0x20, nStructLen);
	memcpy(p->header.Code, _BARELAYEA::CODE_REG_MASTER, strlen(_BARELAYEA::CODE_REG_MASTER));
	p->header.Type[0] = _BARELAYEA::TP_COMMAND;
	memcpy(p->header.AccNo, pzMyAccNo, strlen(pzMyAccNo));

	int nRet = SendData(zBuffer, nStructLen);
	if (nRet == _BARELAYEA::Q_ERROR)
		sprintf("RegisterAsMaster failed(%s)", g_Sender.GetMsg());
	return nRet;
}


int UnRegisterAsMaster(char* pzMyAccNo)
{
	char zBuffer[1024] = { 0, };
	int nStructLen = sizeof(_BARELAYEA::PACK_UNREG_MASTER);
	_BARELAYEA::PACK_UNREG_MASTER * p = (_BARELAYEA::PACK_UNREG_MASTER *)zBuffer;
	memset(p, 0x20, nStructLen);
	memcpy(p->header.Code, _BARELAYEA::CODE_UNREG_MASTER, strlen(_BARELAYEA::CODE_UNREG_MASTER));
	p->header.Type[0] = _BARELAYEA::TP_COMMAND;
	memcpy(p->header.AccNo, pzMyAccNo, strlen(pzMyAccNo));

	int nRet = SendData(zBuffer, nStructLen);
	if (nRet == _BARELAYEA::Q_ERROR)
		sprintf("UnRegisterAsMaster failed(%s)", g_Sender.GetMsg());
	return nRet;
}

int RegisterAsSlave(char* pzMyAccNo, char* pzMasterAccNo)
{
	char zBuffer[1024] = { 0, };
	int nStructLen = sizeof(_BARELAYEA::PACK_REG_SLAVE);
	_BARELAYEA::PACK_REG_SLAVE * p = (_BARELAYEA::PACK_REG_SLAVE *)zBuffer;
	memset(p, 0x20, nStructLen);
	memcpy(p->header.Code, _BARELAYEA::CODE_REG_SLAVE, strlen(_BARELAYEA::CODE_REG_SLAVE));
	p->header.Type[0] = _BARELAYEA::TP_COMMAND;
	memcpy(p->header.AccNo, pzMyAccNo, strlen(pzMyAccNo));
	memcpy(p->MasterAccNo, pzMasterAccNo, strlen(pzMasterAccNo));

	int nRet = SendData(zBuffer, nStructLen);
	if (nRet == _BARELAYEA::Q_ERROR)
		sprintf("RegisterAsSlave failed(%s)", g_Sender.GetMsg());
	return nRet;
}

int UnRegisterAsSlave(char* pzMyAccNo, char* pzMasterAccNo)
{
	char zBuffer[1024] = { 0, };
	int nStructLen = sizeof(_BARELAYEA::PACK_UNREG_SLAVE);
	_BARELAYEA::PACK_UNREG_SLAVE * p = (_BARELAYEA::PACK_UNREG_SLAVE *)zBuffer;
	memset(p, 0x20, nStructLen);
	memcpy(p->header.Code, _BARELAYEA::CODE_UNREG_SLAVE, strlen(_BARELAYEA::CODE_UNREG_SLAVE));
	p->header.Type[0] = _BARELAYEA::TP_COMMAND;
	memcpy(p->header.AccNo, pzMyAccNo, strlen(pzMyAccNo));
	memcpy(p->MasterAccNo, pzMasterAccNo, strlen(pzMasterAccNo));

	int nRet = SendData(zBuffer, nStructLen);
	if (nRet == _BARELAYEA::Q_ERROR)
		sprintf("UnRegisterAsSlave failed(%s)", g_Sender.GetMsg());
	return nRet;
}


void GetMsg(/*out*/char* pMsg)
{
	strcpy(g_zMsg, pMsg);
}

int SendOrder(
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
)
{
	char temp[128];
	char zBuffer[1024] = { 0, };
	int nStructLen = sizeof(_BARELAYEA::PACK_MASTER_ORD);
	_BARELAYEA::PACK_MASTER_ORD * p = (_BARELAYEA::PACK_MASTER_ORD *)zBuffer;
	memset(p, 0x20, nStructLen);
	memcpy(p->header.Code, _BARELAYEA::CODE_MASTER_ORDER, strlen(_BARELAYEA::CODE_MASTER_ORDER));
	p->header.Type[0] = _BARELAYEA::TP_ORDER;
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

	sprintf(temp, "%.*f", VOL_DECIMAL_CNT, dOpendedPrc);
	memcpy(p->OpenedLots, temp, strlen(temp));

	memcpy(p->OpenedTm, psOpenedTm, strlen(psOpenedTm));

	sprintf(temp, "%.2f", dProfit);
	memcpy(p->Profit, temp, strlen(temp));

	sprintf(temp, "%.2f", dSwap);
	memcpy(p->Swap, temp, strlen(temp));


	int nRet = SendData(zBuffer, nStructLen);
	if (nRet == _BARELAYEA::Q_ERROR)
		sprintf("UnRegisterAsSlave failed(%s)", g_Sender.GetMsg());

	return nRet;
}



