#pragma once
#include "afxwin.h"
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/SharedMem.h"
#include "Poco/MemoryPool.h"
#include "NanoSocket.h"
#include "../Common/KRCommon.h"



class CChartThread : public CBaseThread
{
public:
	CChartThread(CString sSymbol, /*CSharedMem& shmChart, CSharedMem& shmLastNm , */
					Poco::MemoryPool* pMemPool, CNanoSocket* sock, char* pChartTp);
	~CChartThread();

	VOID	ThreadFunc();
	//VOID	Log(BOOL bSucc, char* pMsg, ...);
	VOID	ChartProc(VOID* pIn, CHART_TP tp);
	VOID	Chart_SMA(CHART_TP tp, char* pzGroupKey, char* pzNowChartNm, _In_ ST_SHM_CHART_UNIT& nowChart);	// simple moving average
	VOID	SendData2Ord(VOID* pIn);
	BOOL	InitChartSHM();
	VOID	CloseChartShm();
private:
	CString				m_sSymbol;
	CSharedMem			m_shm[2];
	Poco::MemoryPool*	m_pMemPool;
	CNanoSocket			*m_pSock;
	CString				m_sLastChartNm[CHART_TP_CNT], m_sPrevChartNm[CHART_TP_CNT];
	char				m_chartTp[CHART_TP_CNT];
	//UINT				m_unSaveThreadID;
};

