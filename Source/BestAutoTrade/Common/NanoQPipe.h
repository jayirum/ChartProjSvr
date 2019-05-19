#pragma once


#include "NanoQBase.h"
#include "pipeline.h"


class CNanoQReader : public CNanoQ
{
public:
	CNanoQReader();
	~CNanoQReader();

	BOOL	Begin(char* pzMyAccNo, int nRecvTimeout, int nQueueTp, BOOL bCallbackUse = FALSE);
	VOID	ThreadFunc();
	
private:

};


class CNanoQWriter : public CNanoQ
{
public:
	CNanoQWriter();
	~CNanoQWriter();

	BOOL	Begin(char* pzRemoteAccNo, int nSendTimeout, int nQueueTp, BOOL bCallbackUse = FALSE);
	VOID	ThreadFunc();
private:

};

