#pragma warning(disable:4786)
#pragma warning(disable:4503)



#include "../../IRUM_UTIL/TcpClient.h"
#include "../../IRUM_UTIL/ADOFunc.h"
#include "main.h"
#include <winsvc.h>
#include <stdio.h>
#include <time.h>
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/NanoPull.h"
#include "../../IRUM_UTIL/NanoPubSub.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "UserStratOrd.h"
#include <list>
#include <map>
#include <string>

#include <Windows.h>


//서비스 메인 본체
void __stdcall ServiceStart(DWORD argc, LPTSTR* argv);
//서비스 제어 
void __stdcall SCMHandler(DWORD opcode);
//서비스 환경 설정
void __stdcall SetStatus(DWORD dwState,
						  DWORD dwAccept = SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_PAUSE_CONTINUE);
void install();
void uninstall();

SC_HANDLE hScm ;
SC_HANDLE hSrv ;
SERVICE_STATUS ss;

PSECURITY_DESCRIPTOR pSD;			// Pointer to SD.
SECURITY_ATTRIBUTES sa;
//DWORD	m_dwThrID;					//쓰레드 아이디

SERVICE_STATUS_HANDLE g_hXSS;		//서비스 환경 글로벌 핸들
DWORD	g_XSS;						//서비스 현재 상태 저장변수

volatile BOOL	g_bDebug;
HANDLE	g_hDieEvent;				// 프로그램 전체를 끝내는 이벤트
volatile BOOL	g_bContinue = TRUE;	// 프로그램 전체를 끝내는 플래그
char	g_zConfig[_MAX_PATH];



BOOL	DBOpen();
//BOOL	LoadSymbol();
BOOL	LoadStratAppliedUser();
BOOL	InitRecvSignal();
BOOL	InitMemPool();
BOOL	InitInnerPublish();
BOOL	InitSigSend();
//TODO : 관리자 통보용

static unsigned WINAPI Thread_RecvSignal(LPVOID lp);
static unsigned WINAPI Thread_SendRecvClient(LPVOID lp);
static unsigned WINAPI Thread_SaveStrat(LPVOID lp);
//TODO : 관리자 통보용


CRITICAL_SECTION	g_Console;
CLogMsg				g_log;
CDBPoolAdo			*g_pDBPool		= NULL;
CNanoPull			*g_pRecvSignal	= NULL;	// receive signal from IRUMSig_FB
//CNanoPush			*g_pSendSig2Ord = NULL;
CTcpClient			*g_pClientSock = NULL;	// Send order to Client
CMemPool			*g_pMemPool		= NULL;
CNanoPubSub			*g_pPublish	= NULL;		// publish to all UserStratOrd thread


HANDLE				g_hRecvThread = NULL, g_hSaveThread = NULL, g_hSendThread = NULL;
unsigned int		g_unRecvThread = 0, g_unSaveThread = 0, g_unSendThread = 0;
std::map<std::string, CUserStratOrd*>	g_mapUser;

#define __APP_VERSION "Ver 1.0"

int  _Start()
{
	char	msg[512] = { 0, };
	CHAR	szDir[_MAX_PATH];

	//	GET LOG DIR
	CProp prop;
	prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY);
	strcpy(szDir, prop.GetValue("CONFIG_DIR_CHART"));

	CUtil::GetCnfgFileNm(szDir, EXENAME, g_zConfig);
	CUtil::GetConfig(g_zConfig, "DIR", "LOG", szDir);
	g_log.OpenLog(szDir, EXENAME);

	g_log.log(LOGTP_SUCC, "-----------------------------------------------------");
	g_log.log(LOGTP_SUCC, "버전[%s]%s", __DATE__, __APP_VERSION);
	g_log.log(LOGTP_SUCC, "-----------------------------------------------------");
	printf("버전[%s]%s\n", __DATE__, __APP_VERSION);

	//---------------------------------------------
	//	프로그램 전체를 끝내는 이벤트
	//---------------------------------------------
	g_hDieEvent = CreateEvent(&sa, TRUE, FALSE, NULL);

	if (g_bDebug) {
		//CUtil::LogPrint(&g_log, TRUE, "**************************\n");
		//CUtil::LogPrint(&g_log, TRUE, "** 서비스를 시작합니다. **\n");
		//CUtil::LogPrint(&g_log, TRUE, "**************************\n");
	}
	else {
		SetStatus(SERVICE_RUNNING);
		//log.LogEventInf(-1," 서비스를 시작합니다.");
	}

	// Thread for saving singal data
	g_hSaveThread = (HANDLE)_beginthreadex(NULL, 0, &Thread_SaveStrat, NULL, 0, &g_unSaveThread);

	// Thread for sending signal data to client
	g_hSendThread = (HANDLE)_beginthreadex(NULL, 0, &Thread_SendRecvClient, NULL, 0, &g_unSendThread);


	if (!InitMemPool()) {
		return 0;
	}

	if (!DBOpen())
		return 0;

	if(!LoadStratAppliedUser())
		return 0;


	if (!InitInnerPublish())
		return 0;

	// receive signal from IRUMSig_FB
	if (!InitRecvSignal())
		return 0;

	// TODO. InitStratSend	if (!InitSigSend())
		return 0;

	DWORD ret = WaitForSingleObject(g_hDieEvent, INFINITE);

	g_bContinue = FALSE;

	SAFE_DELETE(g_pDBPool);
	SAFE_DELETE(g_pRecvSignal);
	SAFE_CLOSEHANDLE(g_hRecvThread);
	SAFE_CLOSEHANDLE(g_hSaveThread);
	SAFE_CLOSEHANDLE(g_hSendThread);
	SAFE_DELETE(g_pMemPool);
	SAFE_DELETE(g_pPublish);
	SAFE_DELETE(g_pClientSock);
	//SAFE_DELETE(g_pSendSig2Ord);

	CoUninitialize();

	return 0; 
}


BOOL	InitMemPool()
{
	
	g_pMemPool = new CMemPool(MEM_PRE_ALLOC, MEM_MAX_ALLOC, MEM_BLOCK_SIZE);
	
	if(g_pMemPool->available()==0)
	{
		return FALSE;
	}
	return TRUE;
}


BOOL	InitInnerPublish()
{
	g_pPublish = new CNanoPubSub(TP_PUB);
	return g_pPublish->Begin(INNER_CHANNEL_PUBSUB);
}

static unsigned WINAPI Thread_RecvSignal(LPVOID lp)
{
	ST_PACK2CHART* pSise;
	char zSymbol[128];

	while (g_bContinue)
	{
		Sleep(1);
		char* pBuf = g_pMemPool->get();
		int nLen = g_pRecvSignal->RecvData(pBuf);
		if (nLen < 1)
		{
			g_log.log(LOGTP_ERR, "RECV Signal ERROR:%s", g_pRecvSignal->GetMsg());
			printf("SISE RECV ERROR:%s", g_pRecvSignal->GetMsg());
			g_pMemPool->release(pBuf);
			Sleep(1000);
			continue;
		}

		pSise = (ST_PACK2CHART*)pBuf;
		sprintf(zSymbol, "%.*s", sizeof(pSise->ShortCode), pSise->ShortCode);
		CUtil::TrimAll(zSymbol, strlen(zSymbol));
		std::string sSymbol = zSymbol;

		std::map<std::string, CUserStratOrd*>::iterator it = g_mapUser.find(sSymbol);
		if (it == g_mapUser.end())
		{
			//g_log.log(LOGTP_ERR, "[%s] 종목은 요청한 종목이 아니다.", sSymbol.c_str());
		}
		else
		{
			if (g_pPublish->Publish(pBuf, nLen) < 0)
			{
				g_log.log(LOGTP_ERR, "Publish failed(%s)", g_pPublish->GetMsg());
			}
		}
		g_pMemPool->release(pBuf);
	}

	return 0;
}


static unsigned WINAPI Thread_SaveStrat(LPVOID lp)
{
	char zQ[1024];
	CDBHandlerAdo db(g_pDBPool->Get());

	while (g_bContinue)
	{
		Sleep(1);
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_SEND_STRATEGY)
			{
				char* pData = (char*)msg.lParam;
				int nLen = (int)msg.wParam;

				// save data
				PACK_HEADER_S2C* pHd = (PACK_HEADER_S2C*)pData;
				int nRemainLen = nLen - sizeof(PACK_HEADER_S2C);

				char zStratID[32]; pack_header_fields(pData, "STRAT_ID", zStratID);
				char zChartTp[32]; pack_header_fields(pData, "CHART_TP", zChartTp);
				char zChartNm[32]; pack_header_fields(pData, "CHART_NM", zChartNm);
				char zStratPrc[32]; pack_header_fields(pData, "STRAT_PRC", zStratPrc);

				sprintf(zQ, "EXEC STRAT_SAVE "
					"'%.*s', "	//@I_COMP_ID	VARCHAR(10)
					"'%s', "	//@I_STRAT_ID	VARCHAR(10)
					"'%.*s', "	//@I_START_TP	CHAR(1)
					"'%.*s', "	//@I_SYMBOL		VARCHAR(10)
					"'%s', "	//@I_CHART_TP	INT
					"'%s', "	//@I_CHART_NM	CHAR(12)
					"'%s', "	//@I_PRICE		VARCHAR(15)
					"'%.*s' "	//@I_DATA		VARCHAR(200)
					,
					sizeof(pHd->StratCompID), pHd->StratCompID
					, zStratID
					, sizeof(pHd->StratTp), pHd->StratTp
					, sizeof(pHd->Symbol), pHd->Symbol
					, zChartTp
					, zChartNm
					, zStratPrc
					, nRemainLen, &pData[sizeof(PACK_HEADER_S2C)]
				);

				if (FALSE == db->ExecQuery(zQ))
				{
					g_log.log(LOGTP_ERR, "Save Strategy Failed (%s)(%s)", db->GetError(), zQ);
					printf("Save Strategy Failed(%s)(%s)\n", db->GetError(), zQ);
				}

				g_pMemPool->release(pData);
			} //if (msg.message == WM_SEND_STRATEGY)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)

	return 0;
}



#define SEND_TIMEOUT_MILI	500
static unsigned WINAPI Thread_SendRecvClient(LPVOID lp)
{
	int nErrCode;
	int nSentLen, nRecvLen;
	char zRecvData[LEN_BUFF_SIZE];
	while (g_bContinue)
	{
		Sleep(1);
		MSG msg;

		// get data from UserStratOrd thread to send to client
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_SEND_STRATEGY)
			{
				char* pData = (char*)msg.lParam;
				int nLen = (int)msg.wParam;
				// send data
				nSentLen = g_pClientSock->SendData(pData, nLen, &nErrCode);
				if (nSentLen< 0){
					g_log.log(LOGTP_FATAL, "Send Strategy to Client Failed(%s)", g_pClientSock->GetMsg());
				}
				else if (nSentLen > 0) {
					//g_log.log(LOGTP_SUCC, "[Send Client](%.*s)", nLen, pData);
				}

				g_pMemPool->release(pData);
			} //if (msg.message == WM_SEND_STRATEGY)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))

		// get data from client
		ZeroMemory(zRecvData, sizeof(zRecvData));
		nRecvLen = g_pClientSock->RecvData(zRecvData, sizeof(zRecvData), &nErrCode);
		if (nRecvLen > 0)
		{
			// Post to All threads
			std::map<std::string, CUserStratOrd*>::iterator it;
			for(it= g_mapUser.begin();it!=g_mapUser.end();it++)
			{
				char* pBuf = g_pMemPool->get();
				memcpy(pBuf, zRecvData, nRecvLen);
				PostThreadMessage(((*it).second)->GetMyThreadID(), WM_RECV_CLIENT, (WPARAM)0, (LPARAM)pBuf);
			}
		}
		else if (nRecvLen == 0)
			continue;
		else {
			g_log.log(LOGTP_ERR, "recv data from client failed(%s)", g_pClientSock->GetMsg());
		}
	} // while (TRUE)

	return 0;
}


BOOL DBOpen()
{

	char ip[32], id[32], pwd[32], cnt[32], name[32];
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_IP", ip);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_ID", id);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_PWD", pwd);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_NAME", name);
	CUtil::GetConfig(g_zConfig, "DBINFO", "DB_POOL_CNT", cnt);

	g_pDBPool = new CDBPoolAdo(ip, id, pwd, name);
	if (!g_pDBPool->Init(atoi(cnt)))
	{
		g_log.log(LOGTP_ERR, "DB OPEN FAIL(MSG:%s)", g_pDBPool->GetMsg());
		g_log.log(LOGTP_ERR, "(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
		SAFE_DELETE(g_pDBPool);
		return FALSE;
	}
	g_log.log(LOGTP_ERR, "DB OPEN OK(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
	return TRUE;
}

/*
	Load User ID
*/
BOOL LoadStratAppliedUser()
{

	CDBHandlerAdo db(g_pDBPool->Get());
	char zQ[1024];
	sprintf(zQ, "SELECT USER_ID FROM STRAT_MAPPING WHERE USE_YN='Y' ");

	if (!db->ExecQuery(zQ))
	{
		g_log.log(LOGTP_ERR, "Select User Info error(%s)(%s)", zQ, db->GetError());
		return FALSE;
	}

	char zUserID[128], temp[128];

	while (db->IsNextRow())
	{
		db->GetStr("USER_ID", temp);
		FORMAT_USERID(temp, zUserID);
		std::string sID = zUserID;

		CUserStratOrd* p = new CUserStratOrd(zUserID, g_pMemPool, g_unSaveThread, g_unSendThread);

		g_mapUser[sID] = p;
		g_log.log(LOGTP_SUCC, "[%s]", zUserID);
		printf("[%s]", zUserID);

		db->Next();
	}
	db->Close();

	return TRUE;
}


BOOL	InitRecvSignal()
{
	// ChartApiKR 로 부터 시세 수신
	g_pRecvSignal = new CNanoPull;
	char ip[32], port[32];
	CUtil::GetConfig(g_zConfig, "STRATEGY_ORDER_SOCKET_INFO", "IP", ip);
	CUtil::GetConfig(g_zConfig, "STRATEGY_ORDER_SOCKET_INFO", "PORT", port);
	if (!g_pRecvSignal->Begin(ip, port))
	{
		g_log.log(LOGTP_FATAL, "%s", g_pRecvSignal->GetMsg());
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, g_pRecvSignal->GetMsg());
	printf("%s\n",g_pRecvSignal->GetMsg());

	//// 수신 스레드
	g_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, &Thread_RecvSignal, NULL, 0, &g_unRecvThread);


	return TRUE;
}


#define SEND_TIMEOUT_MILI	500
BOOL	InitSigSend()
{
	// SEND 2 CLIENT
	g_pClientSock = new CTcpClient;

	char ip[32], port[32];
	CUtil::GetConfig(g_zConfig, "STRATEGY_RELAY_SOCKET_INFO", "IP", ip);
	CUtil::GetConfig(g_zConfig, "STRATEGY_RELAY_SOCKET_INFO", "PORT", port);
	if (!g_pClientSock->Begin(ip, atoi(port), SEND_TIMEOUT_MILI))
	{
		g_log.log(LOGTP_FATAL, "%s", g_pClientSock->GetMsg());
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, g_pClientSock->GetMsg());
	printf("%s\n", g_pClientSock->GetMsg());

	// SEND 2 STRATEGY ORDER
	//g_pSendSig2Ord = new CNanoPush;

	//CUtil::GetConfig(g_zConfig, "STRATEGY_ORDER_SOCKET_INFO", "IP", ip);
	//CUtil::GetConfig(g_zConfig, "STRATEGY_ORDER_SOCKET_INFO", "PORT", port);
	//if (!g_pSendSig2Ord->Begin(ip, port))
	//{
	//	g_log.log(LOGTP_FATAL, "%s", g_pSendSig2Ord->GetMsg());
	//	return FALSE;
	//}
	//g_log.log(LOGTP_SUCC, g_pSendSig2Ord->GetMsg());
	//printf("%s\n", g_pSendSig2Ord->GetMsg());


	return TRUE;
}

BOOL WINAPI ControlHandler ( DWORD dwCtrlType )  
{  
	switch( dwCtrlType )  
	{  
	case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate  
	case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode  
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
			printf("Stopping %s...\n", DISPNAME);  
		SetEvent(g_hDieEvent);
		g_bContinue = FALSE;
		return TRUE;  
		break;  
		
	}  
	return FALSE;  
}  


int main(int argc, LPSTR *argv)
{
	g_bDebug = FALSE;
  
	if ( (argc > 1) &&
         ((*argv[1] == '-') || (*argv[1] == '/')) )   
    {  
        if ( _stricmp( "install", argv[1]+1 ) == 0 )  
        {  
            install();  
        }  
        else if ( _stricmp( "remove", argv[1]+1 ) == 0 || _stricmp( "delete", argv[1]+1 ) == 0 )  
        {  
            uninstall();  
        }  
        else if ( _stricmp( "debug", argv[1]+1 ) == 0 )  
        {  
            g_bDebug = TRUE;
				SetConsoleCtrlHandler( ControlHandler, TRUE ); 
				InitializeCriticalSection(&g_Console);
				
				_Start();

				DeleteCriticalSection (&g_Console);
				printf("Stopped.\n"); 
				return 0;  
        }  
        else  
        { 
			  return 0;
        } 
    } 
	SERVICE_TABLE_ENTRY stbl[] = 
	{
		{SERVICENAME, (LPSERVICE_MAIN_FUNCTION)ServiceStart },
		{NULL, NULL}
	};

	if(!StartServiceCtrlDispatcher(stbl))
	{
		return  -1;
	}

	return 0;
}

void  __stdcall SetStatus(DWORD dwState, DWORD dwAccept)
{
	ss.dwServiceType					= SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState					= dwState;
	ss.dwControlsAccepted			= SERVICE_ACCEPT_STOP;
	ss.dwWin32ExitCode				= 0;
	ss.dwServiceSpecificExitCode	= 0;
	ss.dwCheckPoint					= 0;
	ss.dwWaitHint						= 0;
	g_XSS = dwState;			//현재 상태 보관

	SetServiceStatus(g_hXSS, &ss);
	return ;
}

void __stdcall SCMHandler(DWORD opcode)
{
	if(opcode == g_XSS)     
		return;
	
	switch(opcode)
	{
	case SERVICE_CONTROL_PAUSE:
		SetStatus(SERVICE_PAUSE_PENDING,0);
		SetStatus(SERVICE_PAUSED);
		break;

	case SERVICE_CONTROL_CONTINUE:
		SetStatus(SERVICE_CONTINUE_PENDING, 0);
		break;

	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		SetStatus(SERVICE_STOP_PENDING, 0);
		SetEvent(g_hDieEvent);
		g_bContinue = FALSE;
		break;

	case SERVICE_CONTROL_INTERROGATE:
		break;

	default:
		SetStatus(g_XSS);
		break;
	}
}

void __stdcall ServiceStart(DWORD argc, LPTSTR* argv)
{
	g_hXSS = RegisterServiceCtrlHandler(SERVICENAME,
		(LPHANDLER_FUNCTION)SCMHandler); 
	if(g_hXSS ==0)
	{
		//log.LogEventErr(-1,"서비스 컨트롤 핸들러를 등록할 수 없습니다.");
		return ;
	}

	//서비스가 시작 중임을 알린다
	SetStatus(SERVICE_START_PENDING);

	//// Allocate memory for the security descriptor.	
	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(
							LPTR		//Specifies how to allocate memory
										// Allocates fixed memory && Initializes memory contents to zero.
							,SECURITY_DESCRIPTOR_MIN_LENGTH	//number of bytes to allocate
							);
	
	
	//// Initialize the new security descriptor.
	InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);

	
	//// Add a NULL descriptor ACL to the security descriptor.
	SetSecurityDescriptorDacl(pSD, TRUE, (PACL) NULL, FALSE);

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = TRUE;

	
	g_bDebug  = FALSE;
	_Start();												//서비스 실행

	//log.LogEventInf(-1,"서비스가 정상적으로 종료했습니다.");
	SetStatus(SERVICE_STOPPED, 0);					///서비스 멈춤
	return;
}

void install()
{
	char SrvPath[MAX_PATH];
	SERVICE_DESCRIPTION lpDes;
	char Desc[64];
	strcpy(Desc, SERVICENAME);
	strcat(Desc, " 서비스!");

	hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if(hScm == NULL)
	{
		//log.LogEventErr(-1, "서비스 메니져와 연결할 수 없습니다");
		return ;
	}
	GetCurrentDirectory(MAX_PATH, SrvPath);
	strcat(SrvPath, "\\");
	strcat(SrvPath, EXENAME);
	//sprintf(SrvPath, "D:\\IBSSERVER(BACKUPSYSTEM)\\RealShmUP\\Release\\RealShmUP.exe");
	
	if(_access(SrvPath, 0) != 0)
	{
		CloseServiceHandle(hScm);
		//log.LogEventErr(-1, "서비스 프로그램이 같은 디렉토리에 없습니다");
		printf("서비스 프로그램이 같은 디렉토리에 없습니다\n");
		return;
	}

	////	종속 서비스
//	char szDependency[64];
// 	memset(szDependency, 0x00, sizeof(szDependency));	
// 	CProp prop;
// 	prop.SetBaseKey(HKEY_LOCAL_MACHINE, REG_FRONTSERVER);
// 	strcpy(szDependency, prop.GetValue("Dependency"));
// 	prop.Close();

	hSrv = CreateService(hScm, SERVICENAME, DISPNAME,
        SERVICE_ALL_ACCESS, 
		SERVICE_WIN32_OWN_PROCESS, 
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		SrvPath,
		NULL,
		NULL,
		"IRUM_ChartShmQueue",	//NULL, //szDependency,
		NULL,
		NULL);

	if(hSrv == NULL)
	{
		printf("서비스를 설치하지 못 했습니다 : %d\n", GetLastError());
	}
	else
	{
		lpDes.lpDescription = Desc;
		ChangeServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, &lpDes);
		//log.LogEventInf(-1, "서비스를 성공적으로 설치했습니다.");
		printf("서비스를 성공적으로 설치했습니다.\n");
		CloseServiceHandle(hSrv);
	}
	CloseServiceHandle(hScm);
}

void uninstall()
{
	hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if(hScm == NULL)
	{
		//log.LogEventErr(-1, "서비스 메니져와 연결할 수 없습니다");
		printf("서비스 메니져와 연결할 수 없습니다\n");
		return;
	}

	hSrv = OpenService(hScm, SERVICENAME, SERVICE_ALL_ACCESS);	
	if(hSrv == NULL)
	{
		CloseServiceHandle(hScm);
		//log.LogEventErr(-1, "서비스가 설치되어 있지 않습니다");
		printf("서비스가 설치되어 있지 않습니다 : %d\n", GetLastError());
		return ;
	}	

	//서비스 중지
	QueryServiceStatus(hSrv, &ss);
	if(ss.dwCurrentState != SERVICE_STOPPED)
	{
		ControlService(hSrv, SERVICE_CONTROL_STOP, &ss);
		Sleep(2000);
	}

	//서비스 제거
	if(DeleteService(hSrv))
	{
		//log.LogEventInf(-1, "성공적으로 서비스를 제거했습니다");
		printf("성공적으로 서비스를 제거했습니다\n");
	}
	else{
		printf("서비스를 제거하지 못했습니다.\n");
	}
	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);
}
