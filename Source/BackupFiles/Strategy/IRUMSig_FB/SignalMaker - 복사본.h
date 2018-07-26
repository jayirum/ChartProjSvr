/*
	subscribing
	Signal »ý¼º
*/
#pragma once
#include <string>
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/QueueShm.h"
#include "../../IRUM_UTIL/NanoPubSub.h"

#define MAX_CHARTTP_CNT	20

class CSignalMaker : public CBaseThread
{
public:
	CSignalMaker(char* pzSymbol, char* pzArtcCode, CMemPool* pMemPool, unsigned dwMainThreadId);
	virtual ~CSignalMaker();

	VOID	SignalProc(VOID* pIn, int tp);
	
	BOOL	BeginShm();
	VOID	EndShm();

	BOOL	BeginSubscribe();
	VOID	EndSubScribe();
private:
	BOOL	IsMySymbol(char* pSise);
	VOID	ThreadFunc();
private:
	char			m_zShmNm[128], m_zMutexNm[128];
	char			m_zSymbol[128];
	char			m_zArtc[128];
	CMemPool*		m_pMemPool;
	CQueueShm		*m_pShmQ;
	CNanoPubSub		*m_pNanoSub;
	unsigned		m_dwMainThreadId;
};