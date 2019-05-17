#pragma once


#include "NanoQBase.h"


class CNanoQReader : public CNanoQ
{
public:
	CNanoQReader();
	~CNanoQReader();

	BOOL	Begin(char* pzChannelNm, int nRecvTimeout);
	int		RecvData(_Out_ char* pData, _In_ int nBuffLen, _Out_ int* pnErrCode);
private:

};
