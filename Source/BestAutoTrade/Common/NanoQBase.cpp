#include "NanoQBase.h"

CNanoQ::CNanoQ() 
{
	m_sock = -1;
	ZeroMemory(m_zChannelNm, sizeof(m_zChannelNm));
}
CNanoQ::~CNanoQ()
{
	End();
}

VOID CNanoQ::End()
{
	StopThread();
	if (m_sock != -1)
	{
		nn_close(m_sock);
		m_sock = -1;
	}
}

BOOL CNanoQ::Begin(char* pzAccNo, int nTimeout, int nQueueTp, BOOL bCallbackUse)
{
	m_bUseCallback = bCallbackUse;
	m_nTimeout = nTimeout;

	m_tpQueue = (QUEUE_TP)nQueueTp;

	if (m_tpQueue == TP_INPROC)
	{
		switch (m_tpNanomsg)
		{
		case TP_PIPE_READER:
		case TP_PIPE_WRITER:
			sprintf(m_zChannelNm, "inproc://%s", RELAY_CHANNEL_NM);
			break;
		case TP_PUB:
		case TP_SUB:
			sprintf(m_zChannelNm, "inproc://BA_MASTER_%s", pzAccNo);
			break;			
		}		
	}
	if (m_tpQueue == TP_INTERPROC)
	{
		switch (m_tpNanomsg)
		{
		case TP_PIPE_READER:
		case TP_PIPE_WRITER:
			sprintf(m_zChannelNm, "ipc://%s", RELAY_CHANNEL_NM);
			break;
		case TP_PUB:
		case TP_SUB:
			sprintf(m_zChannelNm, "ipc://MASTER_%s", pzAccNo);
			break;
		}
	}

	return TRUE;
}

void CNanoQ::RegisterCallBack(LPVOID lpClassInstancePtr, PRecvCallBack lpCallBack)
{
	m_lpClassInstancePtr = lpClassInstancePtr;
	m_lpCallBackFn = lpCallBack;

	if(m_bUseCallback)
		ResumeThread();
}


int	CNanoQ::SendData(char* pData, int nSendLen, int* pnErrCode)
{
	*pnErrCode = Q_SUCCESS;
	int rc = nn_send(m_sock, pData, nSendLen, 0);
	if (rc < 0)
	{
		int err = nn_errno();
		if (err == ETIMEDOUT)
		{
			*pnErrCode = Q_TIMEOUT;
			return Q_TIMEOUT;
		}
		sprintf(m_zMsg, "Failed to send: %s\n", nn_err_strerror(err));
		*pnErrCode = Q_ERROR;
		return Q_ERROR;
	}
	return rc;
}

char* CNanoQ::ChannelNm()
{ 
	return m_zChannelNm; 
}


int	CNanoQ::RecvData(_Out_ char* pData, _In_ int nBuffLen, _Out_ int* pnErrCode)
{
	*pnErrCode = Q_SUCCESS;
	int rc = nn_recv(m_sock, pData, nBuffLen, 0);
	if (rc < 0)
	{
		int err = nn_errno();
		if (err == ETIMEDOUT)
		{
			*pnErrCode = Q_TIMEOUT;
			//sprintf(m_zMsg, "Failed to recv: %s\n", nn_err_strerror(err));
			return Q_ERROR;
		}
		sprintf(m_zMsg, "Failed to recv: %s\n", nn_err_strerror(err));
		*pnErrCode = Q_ERROR;
		return Q_ERROR;
	}
	return rc;
}

int	CNanoQ::RecvDataEx()
{
	char zRecvBuff[1024];
	int nBuffSize = sizeof(zRecvBuff);
	int nErrCode, nRet;
	while (!Is_TimeOfStop(1))
	{
		ZeroMemory(zRecvBuff, sizeof(zRecvBuff));
		nRet = RecvData(zRecvBuff, nBuffSize, &nErrCode);
		if (nRet > 0)
		{
			m_lpCallBackFn(m_lpClassInstancePtr, zRecvBuff, nRet);
		}
	}
	return 0;
}



BOOL CNanoQ::Connect()
{
	if (!NeedsToCallConnect())
		return TRUE;

	int rc = nn_connect(m_sock, m_zChannelNm);
	if (rc < 0) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed to connect \"%s\": %s [%d]\n",
			m_zChannelNm,
			nn_err_strerror(err),
			(int)err);
		return FALSE;;
	}
	return TRUE;
}


BOOL CNanoQ::NeedsToCallConnect()
{
	BOOL bRet = FALSE;
	switch (m_tpNanomsg)
	{
	case TP_PIPE_WRITER:
	case TP_SUB:
		bRet = TRUE;
		break;
	}

	return bRet;

}