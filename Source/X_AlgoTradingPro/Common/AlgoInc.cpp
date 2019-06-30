#include "AlgoInc.h"
#include <stdio.h>


char* _ALGOINC::packlen(int len, char* out)
{
	sprintf(out, "%0*d", LEN_PACKET_SIZE, len);
	return out;
}


char* _ALGOINC::rsltCode(int code, char* out)
{
	sprintf(out, "%0*d", _ALGOINC::LEN_RSLT_CODE, code);
	return out;
}

char* _ALGOINC::now(char* out)
{
	SYSTEMTIME st; GetLocalTime(&st);
	sprintf(out, "%04d%02d%02d"
		"%02d%02d%02d%03d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return out;
}


BOOL _ALGOINC::RequestSendIO(SOCKET sock, char sysTp, char* pSendBuf, int nSendLen, char* pIoBuffer)
{
	DWORD dwOutBytes = 0;
	DWORD dwFlags = 0;
	_ALGOINC::IO_CONTEXT* pSend = NULL;

	try {
		pSend = (_ALGOINC::IO_CONTEXT*)pIoBuffer;

		ZeroMemory(pSend, sizeof(_ALGOINC::IO_CONTEXT));
		CopyMemory(pSend->buf, pSendBuf, nSendLen);
		//	pSend->sock	= sock;
		pSend->wsaBuf.buf = pSend->buf;
		pSend->wsaBuf.len = nSendLen;
		pSend->context = CTX_RQST_SEND;
		pSend->sysTp = sysTp;

		int nRet = WSASend(sock
			, &pSend->wsaBuf	// wsaBuf 배열의 포인터
			, 1					// wsaBuf 포인터 갯수
			, &dwOutBytes		// 전송된 바이트 수
			, dwFlags
			, &pSend->overLapped	// overlapped 포인터
			, NULL);
		if (nRet == SOCKET_ERROR) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				sprintf(pIoBuffer, "WSASend error : %d", WSAGetLastError());
				return FALSE;
			}
		}
	}
	catch (...) {
		sprintf(pIoBuffer, "WSASend error - Exception");
		return FALSE;
	}
	
	return TRUE;
}


BOOL _ALGOINC::RequestRecvIO(SOCKET sock, char sysTp, char* pIoBuffer)
{
	_ALGOINC::IO_CONTEXT* pRecv = NULL;
	DWORD dwNumberOfBytesRecvd = 0;
	DWORD dwFlags = 0;

	BOOL bRet = TRUE;
	try {
		pRecv = (_ALGOINC::IO_CONTEXT*)pIoBuffer;

		ZeroMemory(pRecv, sizeof(_ALGOINC::IO_CONTEXT));
		//ZeroMemory( &(pRecv->overLapped), sizeof(WSAOVERLAPPED));
		pRecv->wsaBuf.buf = pRecv->buf;
		pRecv->wsaBuf.len = _ALGOINC::LEN_BUFFER;
		pRecv->context = _ALGOINC::CTX_RQST_RECV;
		pRecv->sysTp = sysTp;


		int nRet = WSARecv(sock
			, &(pRecv->wsaBuf)
			, 1, &dwNumberOfBytesRecvd, &dwFlags
			, &(pRecv->overLapped)
			, NULL);
		if (nRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING) {
				sprintf(pIoBuffer, "WSARecv error : %d", WSAGetLastError());
				return FALSE;
			}
		}
	}
	catch (...) {
		sprintf(pIoBuffer, "WSARecv error - Exception");
		return FALSE;
	}

	return TRUE;
}
