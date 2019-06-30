#pragma once


#include "NanoQBase.h"
#include "PUBSUB.h"

class CNanoQPub : public CNanoQ
{
public:
	CNanoQPub();
	~CNanoQPub();

	BOOL	Begin(char* pzMyAccNo, int nSendTimeout, int nQueueTp, BOOL bCallbackUse = FALSE);
	VOID	ThreadFunc();
private:

};


class CNanoQSub : public CNanoQ
{
public:
	CNanoQSub();
	~CNanoQSub();

	BOOL	Begin(char* pzMasterAccNo, int nRecvTimeout, int nQueueTp, BOOL bCallbackUse = FALSE);
	VOID	ThreadFunc();
private:
};
