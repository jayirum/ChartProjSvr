#pragma once


#include "NanoQBase.h"


class CNanoQWriter : public CNanoQ
{
public:
	CNanoQWriter();
	~CNanoQWriter();

	BOOL	Begin(char* pzChannelNm, int nSendTimeout);
	BOOL	Connect();
	int		SendData(char* pData, int nSendLen, int* pnErrCode);
private:

};

