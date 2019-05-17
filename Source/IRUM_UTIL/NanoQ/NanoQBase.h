#pragma once

// project option > VC++ Directories > Include Directories : C:\nanomsg-1.1.5\src;
// project option > VC++ Directories > Library Directories : diretory including nanomsg.lib
// C:\ChartProjSvr\Source\_Bin;
// project option > C/C++ > Preprocessor : NN_HAVE_WINDOWS;

#include "nn.h"
#include "pipeline.h"
#include "./utils/attr.h"
#include "./utils/err.h"
#include <Windows.h>


#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "nanomsg.lib")

enum _NANOQ_TP {TP_PULL, TP_PUSH};
enum {Q_ERROR=-1, Q_SUCCESS, Q_TIMEOUT};


class CNanoQ
{
public:
	CNanoQ();
	~CNanoQ();

	virtual BOOL	Begin(char* pzChannelNm, int nTimeout) = 0;
	VOID	End();

	inline char*	GetMsg() { return m_zMsg; }
protected:
	_NANOQ_TP	m_tp;
	int		m_nTimeout;
	int		m_sock;
	char	m_zChannel[NN_SOCKADDR_MAX];
	char	m_zMsg[1024];
};