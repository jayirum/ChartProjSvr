#include "NanoQPipe.h"
#include <stdio.h>

CNanoQWriter::CNanoQWriter()
{
	m_tpNanomsg = TP_PIPE_WRITER;
}

CNanoQWriter::~CNanoQWriter()
{
}

BOOL CNanoQWriter::Begin(char* pzChannelNm, int nSendTimeout, int nQueueTp, BOOL bCallbackUse)
{
	m_tpNanomsg = TP_PIPE_WRITER;
	CNanoQ::Begin(pzChannelNm, nSendTimeout, nQueueTp, bCallbackUse);

	m_sock = nn_socket(AF_SP, NN_PUSH);
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed create socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
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




void CNanoQWriter::ThreadFunc()
{
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//			NanoQReader
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

CNanoQReader::CNanoQReader()
{
	m_tpNanomsg = TP_PIPE_READER;
}

CNanoQReader::~CNanoQReader()
{}

VOID CNanoQReader::ThreadFunc()
{
	RecvDataEx();
	return;
}

BOOL CNanoQReader::Begin(char* pzChannelNm, int nRecvTimeout, int nQueueTp, BOOL bCallbackUse)
{
	m_tpNanomsg = TP_PIPE_READER;
	CNanoQ::Begin(pzChannelNm, nRecvTimeout, nQueueTp, bCallbackUse);

	m_sock = nn_socket(AF_SP, NN_PULL);
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed create socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
	}

	int rc = nn_bind(m_sock, m_zChannelNm);
	if (rc < 0)
	{
		int err = nn_errno();
		printf("Failed to bind to \"%s\": %s [%d]\n",
			m_zChannelNm,
			nn_err_strerror(err),
			(int)err);
		return -1;
	}

	//timeout
	int ret = nn_setsockopt(m_sock, NN_SOL_SOCKET, NN_RCVTIMEO, &m_nTimeout, sizeof(m_nTimeout));
	if (m_sock == -1) {
		int err = nn_errno();
		sprintf(m_zMsg, "Failed recv time out on socket: %s [%d]\n", nn_err_strerror(err), (int)err);
		return FALSE;
	}

	return TRUE;
}

