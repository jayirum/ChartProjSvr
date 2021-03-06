#pragma once

#include "PktHandler.h" //todo after completion - remove ../
#include "BaseThread.h" //todo after completion - remove ../
#pragma warning(disable:4996)

#pragma comment(lib, "ws2_32.lib")

#define DEF_SEND_RETRY 3

/*
	Use blocking mode socket
	- set timeout for send and recv
*/
class CTcpClient : public CBaseThread
{
public:
	
	CTcpClient(char* pzName);
	virtual ~CTcpClient();

	BOOL	Begin( char* pRemoteIP, int nPort, int nTimeOut);
	VOID	StartRecvData() { ResumeThread(); }
	VOID	SetNagle(BOOL bOn);
	VOID	End();
	INT		SendData(char* pInBuf, int nBufLen, int *o_ErrCode);
	char*	GetMsg() { return m_zMsg; };
	VOID	SetIP_Port(char* psIP, int nPort);
	int		GetOneRecvedPacket(char* pOutBuf);
	BOOL	HappenedRecvError() { return m_bRecvErr; }
	BOOL	IsConnected();

	BOOL	Connect();
private:
	int		RecvData(char* pOutBuf, int nBufLen, int *o_ErrCode);
	virtual VOID	ThreadFunc();
	VOID	RecvThread();
	VOID	Disconnect();
	//BOOL	Send(char* pInBuf, int nBufLen, int nTimeOut);
	//BOOL	Recv(char* pOutBuf, int nBufLen);
	//BOOL	SendRecv(/*in,out*/ char* pBuf, int nBufLen );	
	VOID	DumpErr( char* pSrc, int nErr );
//protected:
	char			m_zMyName[32];
	char			m_zRemoteIP[128];
	int				m_nRemotePort;
	char			m_zMsg[512];
	SOCKET			m_sock;
	SOCKADDR_IN		m_sin;
	//WSAEVENT		m_hwsa;
	BOOL			m_bConn;
	int				m_nTimeout;
	BOOL			m_bRecvErr;
	CPktHandler		m_pktHandler;

};

