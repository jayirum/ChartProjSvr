#include "NanoQPubSub.h"
#include <stdio.h>

CNanoQPub::CNanoQPub()
{
	m_tpNanomsg = TP_PUB;
}

CNanoQPub::~CNanoQPub()
{
}

BOOL CNanoQPub::Begin(char* pzMyAccNo, int nSendTimeout, int nQueueTp, BOOL bCallbackUse )
{
	if (!pzMyAccNo)
	{
		sprintf(m_zMsg, "Need MyAccount No\n");
		return FALSE;
	}

	m_tpNanomsg = TP_PUB;

	CNanoQ::Begin(pzMyAccNo, nSendTimeout, nQueueTp, bCallbackUse);

	m_sock = nn_socket(AF_SP, NN_PUB);
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed create socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
	}
	
	int rc = nn_bind(m_sock, m_zChannelNm);
	if (rc < 0	)
	{
		int err = nn_errno();
		printf("Failed to bind to \"%s\": %s [%d]\n",
			m_zChannelNm,
			nn_err_strerror(err),
			(int)err);
		return -1;
	}

	//timeout
	int ret = nn_setsockopt(m_sock, NN_SOL_SOCKET, NN_SNDTIMEO, &m_nTimeout, sizeof(m_nTimeout));
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed send time out on socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
	}
	return TRUE;
}

VOID CNanoQPub::ThreadFunc()
{
	return;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//			NanoQSub
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

CNanoQSub::CNanoQSub()
{
	m_tpNanomsg = TP_SUB;
}

CNanoQSub::~CNanoQSub()
{
	
}

BOOL CNanoQSub::Begin(char* pzMasterAccNo, int nRecvTimeout, int nQueueTp, BOOL bCallbackUse )
{
	if (!pzMasterAccNo)
	{
		sprintf(m_zMsg, "Need MyAccount No\n");
		return FALSE;
	}


	m_tpNanomsg = TP_SUB;
	CNanoQ::Begin(pzMasterAccNo, nRecvTimeout, nQueueTp, bCallbackUse);


	m_sock = nn_socket(AF_SP, NN_SUB);
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed create socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
	}

	int ret = nn_setsockopt(m_sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed nn_setsockopt on socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
	}

	//timeout
	ret = nn_setsockopt(m_sock, NN_SOL_SOCKET, NN_RCVTIMEO, &m_nTimeout, sizeof(m_nTimeout));
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed recv time out on socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
	}

	return TRUE;
}



VOID CNanoQSub::ThreadFunc()
{
	RecvDataEx();
}
