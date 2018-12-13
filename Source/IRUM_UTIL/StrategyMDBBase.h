#pragma once

#include "SharedMem.h"
#include "IRUM_Common.h"
#include "ADOFunc.h"

//#define MAX_SYMBOL_CNT			30
//
//
//#define FMT_SYMBOL(in,out){ sprintf(out, "%-*.*s", LEN_SYMBOL,LEN_SYMBOL,in);}
//#define FMT_ARTC(in,out){ sprintf(out, "%-*.*s", LEN_ARTC,LEN_ARTC,in);}
//#define FMT_SYMBOL_NM(in,out){ sprintf(out, "%-*.*s", LEN_SYMBOL_NM,LEN_SYMBOL_NM,in);}
//#define FMT_TICKSIZE(dIn,nDotcnt,out){ sprintf(out, "%0*.*f", LEN_TICKSIZE,nDotcnt,dIn);}
//#define FMT_TICKVALUE(dIn,out){ sprintf(out, "%0*.2f",LEN_PL,dIn);}
//#define FMT_DOTCNT(nIn,out){ sprintf(out, "%0*d", LEN_DOTCNT,nIn);}


typedef struct _SYMBOL_INFO
{
	char	Symbol[32];
	char	ArtcCd[32];
	char	SymbolName[64];
	double	TickSize;	//0.00001
	double	TickValue;
	int 	DotCnt;
}SYMBOL_INFO;


class CStrategyMDBBase
{
public:
	CStrategyMDBBase(char *pzSymbol);
	virtual ~CStrategyMDBBase();

	BOOL	Initialize(char* pConfig);
	virtual VOID	Finalize();

	
	char*	Symbol() { return m_zSymbol; }
	BOOL	IsInitialized() { return m_bInit; }
	char*	getmsg() { return m_zMsg; }

protected:
	virtual BOOL	CreateMDB() = 0;
	virtual BOOL	LoadSymbolAndStrategy() = 0;
	BOOL			DBOpen();

protected:
	char m_zSymbol[32];
	//char m_zShmName[32], m_zMutexName[32];
	//CSharedMem	*m_shm;
	CDBPoolAdo	*m_pDBPool;
	char		m_zMsg[1024];
	char		m_zConfig[512];
	BOOL		m_bInit;
};