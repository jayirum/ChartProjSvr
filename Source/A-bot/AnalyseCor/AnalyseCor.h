#pragma once


#include "../../IRUM_UTIL/TcpClient.h"
#include "../../IRUM_UTIL/BaseThread.h"
#include <map>
#include <string>
#include "TrackDiff.h"

// CAnalyseCor dialog
class CAnalyseCor : public CBaseThread
{
// Construction
public:
	CAnalyseCor();
	~CAnalyseCor();

	// Generated message map functions
	BOOL Initialize();
	VOID Finalize();

private:
	VOID ThreadFunc();


	BOOL	LoadSymbolInfo();
	void	InitApiSocket();
	void	CloseApiSocket();

private:
	CTcpClient		*m_pApiClient;
	CTrackDiff		*m_track;

	char			m_zApiIP[32];
	char			m_zApiPort[32];

	
	char			m_zEur[32], m_zGbp[32];
};
