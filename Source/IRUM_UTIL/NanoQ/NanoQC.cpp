#include "NanoQC.h"
#include <stdio.h>

CNanoQC::CNanoQC()
{

}

CNanoQC::~CNanoQC()
{}

BOOL CNanoQC::Begin(char* pzRemoteChannelNm, int nSendTimeout)
{
	m_tp = TP_PUSH;
	
	if (!pzRemoteChannelNm) {
		sprintf(m_zMsg, "Need Remote channel name");
		return FALSE;
	}

	sprintf(m_zRemoteChannel, "ipc://%s", pzRemoteChannelNm);
	m_sock = nn_socket(AF_SP, NN_PUSH);
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed create socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
	}

	//timeout
	m_nTimeout = nSendTimeout;
	int ret = nn_setsockopt(m_sock, NN_SOL_SOCKET, NN_SNDTIMEO, &m_nTimeout, sizeof(m_nTimeout));
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed send time out on socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
	}
	return TRUE;
}


BOOL CNanoQC::Connect()
{
	int rc = nn_connect(m_sock, m_zRemoteChannel);
	if (rc < 0) {
		int err = nn_errno();
		sprintf(m_zMsg,"Failed to connect \"%s\": %s [%d]\n",
			m_zRemoteChannel,
			nn_err_strerror(err),
			(int)err);
		return FALSE;;
	}
	return TRUE;
}


int	CNanoQC::SendData(char* pData, int nSendLen, int* pnErrCode)
{
	*pnErrCode = Q_SUCCESS;
	int rc = nn_send(m_sock, pData, nSendLen, 0);
	if (rc < 0) 
	{
		int err = nn_errno();
		if (err == ETIMEDOUT)
		{
			*pnErrCode = Q_TIMEOUT;
			return Q_ERROR;
		}
		sprintf(m_zMsg, "Failed to send: %s\n", nn_err_strerror(err));
		*pnErrCode = Q_ERROR;
		return Q_ERROR;
	}
	return rc;
}