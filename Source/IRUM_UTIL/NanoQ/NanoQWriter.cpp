#include "NanoQWriter.h"
#include <stdio.h>

CNanoQWriter::CNanoQWriter()
{

}

CNanoQWriter::~CNanoQWriter()
{
}

BOOL CNanoQWriter::Begin(char* pzChannelNm, int nSendTimeout)
{
	m_tp = TP_PUSH;
	
	if (!pzChannelNm) {
		sprintf(m_zMsg, "Need Remote channel name");
		return FALSE;
	}

	sprintf(m_zChannel, "ipc://%s", pzChannelNm);
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


BOOL CNanoQWriter::Connect()
{
	int rc = nn_connect(m_sock, m_zChannel);
	if (rc < 0) {
		int err = nn_errno();
		sprintf(m_zMsg,"Failed to connect \"%s\": %s [%d]\n",
			m_zChannel,
			nn_err_strerror(err),
			(int)err);
		return FALSE;;
	}
	return TRUE;
}


int	CNanoQWriter::SendData(char* pData, int nSendLen, int* pnErrCode)
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