#pragma once


#include "NanoQBase.h"


class CNanoQC : public CNanoQ
{
public:
	CNanoQC();
	virtual ~CNanoQC();

	BOOL	Begin(char* pzRemoteChannelNm, int nSendTimeout);
	BOOL	Connect();
	int		SendData(char* pData, int nSendLen, int* pnErrCode);
private:

};

