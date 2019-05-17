#include "NanoQS.h"
#include <stdio.h>

CNanoQS::CNanoQS()
{

}

CNanoQS::~CNanoQS()
{}

BOOL CNanoQS::Begin(char* pzMyChannelNm, int nRecvTimeout)
{
	m_tp = TP_PULL;

	if (!pzMyChannelNm) {
		sprintf(m_zMsg, "Need my channel name");
		return FALSE;
	}
	sprintf(m_zMyChannel, "ipc://%s", pzMyChannelNm);

	m_sock = nn_socket(AF_SP, NN_PULL);
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed create socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
	}

	int rc = nn_bind(m_sock, m_zMyChannel);
	if (rc < 0) 
	{
		int err = nn_errno();
		printf("Failed to bind to \"%s\": %s [%d]\n",
			m_zMyChannel,
			nn_err_strerror(err),
			(int)err);
		return -1;
	}

	// timeout
	//timeout
	m_nTimeout = nRecvTimeout;
	int ret = nn_setsockopt(m_sock, NN_SOL_SOCKET, NN_RCVTIMEO, &m_nTimeout, sizeof(m_nTimeout));
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed recv time out on socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
	}

	return TRUE;
}


int	CNanoQS::RecvData(_Out_ char* pData, _In_ int nBuffLen, _Out_ int* pnErrCode)
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