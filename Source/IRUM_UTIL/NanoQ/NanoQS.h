#pragma once


#include "NanoQBase.h"


class CNanoQS : public CNanoQ
{
public:
	CNanoQS();
	virtual ~CNanoQS();

	BOOL	Begin(char* pzRemoteChannelNm, int nRecvTimeout);
	int		RecvData(_Out_ char* pData, _In_ int nBuffLen, _Out_ int* pnErrCode);
private:

};
