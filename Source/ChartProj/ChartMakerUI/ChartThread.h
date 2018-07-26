/*
	# 각 종목별로 클래스를 생성한다.
	# 그러나, SHM 은 ARTC 단위 이다.

*/

#pragma once
#include "afxwin.h"
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/SharedMem.h"
#include "Poco/MemoryPool.h"
#include "NanoSocket.h"
#include "../Common/ChartCommon.h"



class CChartThread : public CBaseThread
{
public:
	CChartThread(CString sSymbol, char* pzArtcCode, Poco::MemoryPool* pMemPool, CNanoSocket* sock);
	~CChartThread();

	VOID	ThreadFunc();
	//VOID	Log(BOOL bSucc, char* pMsg, ...);
	VOID	ChartProc(VOID* pIn, CHART_TP tp);
	VOID	ChartProcTick(VOID* pIn, CHART_TP tp);
	VOID	Chart_SMA(CHART_TP tp, char* pzGroupKey, char* pzNowChartNm, _In_ ST_SHM_CHART_UNIT& nowChart);	// simple moving average
	VOID	SendData2Ord(VOID* pIn);
	BOOL	InitChartSHM();
	VOID	CloseChartShm();

	char*	Symbol() { return m_sSymbol.GetBuffer(); }
private:
	char				m_zArtc[32];
	char				m_zShmNm[128], m_zMutexNm[128];
	CString				m_sSymbol;
	CSharedMem			m_shm;
	Poco::MemoryPool*	m_pMemPool;
	CNanoSocket			*m_pSock;
	CString				m_sLastChartNm[CHART_TP_CNT], m_sPrevChartNm[CHART_TP_CNT];

};

