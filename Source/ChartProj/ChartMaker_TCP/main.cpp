#pragma warning(disable:4786)
#pragma warning(disable:4503)


#include "../../IRUM_UTIL/TcpClient.h"
#include "ChartMaker.h"
#include "../../IRUM_UTIL/ADOFunc.h"
#include "main.h"
#include <winsvc.h>
#include <stdio.h>
#include <time.h>
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/LogMsg.h"
//#include "../../IRUM_UTIL/NanoPubSub.h"
#include "../../IRUM_INC/IRUM_Common.h"
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

BOOL	g_bDebug;
HANDLE	g_hDieEvent;				// 프로그램 전체를 끝내는 이벤트
volatile BOOL	g_bContinue = TRUE;			// 프로그램 전체를 끝내는 플래그
char	g_zConfig[_MAX_PATH];
char	g_zMsg[1024];


BOOL	DBOpen();
BOOL	LoadSymbol();
BOOL	InitApiClient();
BOOL	InitMemPool();
static unsigned WINAPI RecvMDThread(LPVOID lp);
void RecvMDThreadFn();
static unsigned WINAPI ChartSaveThread(LPVOID lp);



CRITICAL_SECTION	g_Console;
CLogMsg				g_log;
CDBPoolAdo			*g_ado		= NULL;
//TODO CSiseRecv			*g_pMDSub = NULL;
//CNanoPubSub			*g_pMDSub = NULL;
//CMCastRecv			*g_pMcastRecv = NULL;
CTcpClient			*g_pApiRecv = NULL;
CMemPool			*g_pMemPool = NULL;

HANDLE				g_hRecvThread = NULL, g_hSaveThread = NULL;
unsigned int		g_unRecvThread = 0, g_unSaveThread = 0;
std::map<std::string, CChartMaker*>	g_mapSymbol;



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
	g_log.log(LOGTP_SUCC, "Version[%s]%s", __DATE__, __APP_VERSION);
	g_log.log(LOGTP_SUCC, "-----------------------------------------------------");
	printf("Version[%s]%s\n", __DATE__, __APP_VERSION);

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

	if (!InitMemPool()) {
		return 0;
	}

	if (!DBOpen())
		return 0;


	InitApiClient();

	// 차트 저장 스레드
	g_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, &RecvMDThread, NULL, 0, &g_unRecvThread);
	g_hSaveThread = (HANDLE)_beginthreadex(NULL, 0, &ChartSaveThread, NULL, 0, &g_unSaveThread);


	if (!LoadSymbol())
		return 0;
	

	DWORD ret = WaitForSingleObject(g_hDieEvent, INFINITE);

	std::map<std::string, CChartMaker*>::iterator it;
	for (it = g_mapSymbol.begin(); it != g_mapSymbol.end();it++)
	{
		CChartMaker* p = (*it).second;
		delete p;
	}
	//printf("----------------1\n");
	//SAFE_DELETE(g_ado);
	//printf("----------------2\n");
	//SAFE_DELETE(g_pApiRecv);
	//printf("----------------3\n");
	//SAFE_CLOSEHANDLE(g_hRecvThread);
	//printf("----------------4\n");
	//SAFE_CLOSEHANDLE(g_hSaveThread);
	//printf("----------------5\n");
	//SAFE_DELETE(g_pMemPool);
	//printf("----------------6\n");
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


static unsigned WINAPI RecvMDThread(LPVOID lp)
{
	__try {
		RecvMDThreadFn();
	}
	__except (ReportException(GetExceptionCode(), "RecvMDThread", g_zMsg))
	{
		g_log.log(LOGTP_FATAL, g_zMsg);
		exit(0);
	}

	return 0;
}

void RecvMDThreadFn()
{
	ST_PACK2CHART_EX* pSise;
	char zSymbol[128];

	while (g_bContinue)
	{
		Sleep(1);
		if (g_pApiRecv->HappenedRecvError())
		{
			g_log.log(LOGTP_ERR, "TCP RECV ERROR:%s", g_pApiRecv->GetMsg());
			printf("TCP RECV ERROR:%s\n", g_pApiRecv->GetMsg()); 
			continue;
		}
		char* pBuf = NULL;;
		if (!g_pMemPool->get(&pBuf))
		{
			g_log.log(LOGTP_ERR, "memory pool get error");
			printf("memory pool get error\n");
			continue;
		}
		int nLen = g_pApiRecv->GetOneRecvedPacket(pBuf);
		if (nLen == 0){
			g_pMemPool->release(pBuf);
			continue;
		}
		if (nLen < 0)
		{
			g_log.log(LOGTP_ERR, "PAKCET Error(%s)(%s)", pBuf, g_pApiRecv->GetMsg());
			//printf("PAKCET Error(%s)\n", g_pApiRecv->GetMsg());
			g_pMemPool->release(pBuf);
			continue;
		}

		if (nLen > 0)
		{
			pSise = (ST_PACK2CHART_EX*)pBuf;
			char tm[9];
			sprintf(tm, "%.2s:%.2s:%.2s", pSise->Time, pSise->Time + 2, pSise->Time + 4);
			memcpy(pSise->Time, tm, sizeof(pSise->Time));
			sprintf(zSymbol, "%.*s", sizeof(pSise->ShortCode), pSise->ShortCode);
			CUtil::TrimAll(zSymbol, strlen(zSymbol));
			std::string sSymbol = zSymbol;

			std::map<std::string, CChartMaker*>::iterator it = g_mapSymbol.find(sSymbol);
			if (it == g_mapSymbol.end())
			{
				g_pMemPool->release(pBuf);
			}
			else
			{
				CChartMaker* p = (*it).second;
				PostThreadMessage(p->GetChartThreadId(), WM_RECV_API_MD, 0, (LPARAM)pBuf);
			}
		}
	}

	return ;
}


static unsigned WINAPI ChartSaveThread(LPVOID lp)
{
	char zQ[1024];
	char zGroupKey[LEN_GROUP_KEY + 1];

	while (g_bContinue)
	{
		Sleep(1);
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_SAVE_CHART)
			{
				ST_SHM_CHART_UNIT* p = (ST_SHM_CHART_UNIT*)msg.lParam;
				sprintf(zGroupKey, "%.*s", LEN_GROUP_KEY, p->Reserved);
				CDBHandlerAdo db(g_ado->Get());
				
				sprintf(zQ, "EXEC CHART_SAVE "
					"'%.*s', "	//@I_GROUP_KEY	VARCHAR(5)--// CLN71
					"'%.*s', "	//, @I_CHART_NM	VARCHAR(20)
					"'%.*s', "	//, @I_PREV_NM		VARCHAR(20)
					"'%.*s', "	//@I_CHART_GB	CHAR(1)--// +,-, 0
					"'%.*s', "	//@I_OPEN_PRC	VARCHAR(20)
					"'%.*s', "	//@I_HIGH_PRC	VARCHAR(20)
					"'%.*s', "	//@I_LOW_PRC		VARCHAR(20)
					"'%.*s', "	//@I_CLOSE_PRC	VARCHAR(20)
					"'%.*s', "	//@I_CNTR_QTY	VARCHAR(20)
					"'%.*s', "	//@I_DOT_CNT		VARCHAR(20)
					"'%.*s', "	//@I_SMA_SHORT	VARCHAR(20)
					"'%.*s' "	//@I_SMA_LONG	VARCHAR(20)
					//"'%.*s' "
					,
					LEN_GROUP_KEY, zGroupKey,
					sizeof(p->Nm), p->Nm,
					sizeof(p->prevNm), p->prevNm,
					sizeof(p->gb), p->gb,
					sizeof(p->open), p->open,
					sizeof(p->high), p->high,
					sizeof(p->low), p->low,
					sizeof(p->close), p->close,
					sizeof(p->cntr_qty), p->cntr_qty,
					sizeof(p->dotcnt), p->dotcnt,
					sizeof(p->sma_short), p->sma_short,
					sizeof(p->sma_long), p->sma_long
					//sizeof(p->seq), p->seq
				);
				if (FALSE == db->ExecQuery(zQ))
				{
					g_log.log(LOGTP_ERR, "CHART DATA Save err(%s)(%s)", db->GetError(), zQ);
					//printf("CHART DATA Save 에러(%s)(%s)\n", db->GetError(), zQ);
				}
				else
				{
					//g_log.log(LOGTP_SUCC, "DB SAVE(%s)", zQ);
				}
					
				delete p;
				db->Close();
			} //if (msg.message == WM_SAVE_CHART)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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

	g_ado = new CDBPoolAdo(ip, id, pwd, name);
	if (!g_ado->Init(atoi(cnt)))
	{
		g_log.log(LOGTP_ERR, "DB OPEN FAIL(MSG:%s)", g_ado->GetMsg());
		g_log.log(LOGTP_ERR, "(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
		SAFE_DELETE(g_ado);
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "DB OPEN OK(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
	return TRUE;
}


BOOL LoadSymbol()
{

	CDBHandlerAdo db(g_ado->Get());
	char zQ[1024];
	sprintf(zQ, "EXEC CHART_GET_SYMBOL");

	if (!db->ExecQuery(zQ))
	{
		g_log.log(LOGTP_ERR, "GET SYMBOL ERROR()%s)(%s)", zQ, db->GetError());
		return FALSE;
	}

	char saveYn[32], yearLen[32], euroDollar[32];
	CUtil::GetConfig(g_zConfig, "CHART_SAVE", "SAVEONDB", saveYn);
	CUtil::GetConfig(g_zConfig, "SYMBOL_TYPE", "YEAR_LENGTH", yearLen);
	CUtil::GetConfig(g_zConfig, "SYMBOL_TYPE", "EURO_DOLLAR", euroDollar);

	char zTemp[32], zSymbol[32], zArtc[32];
	int nDotCnt = 0;
	while (db->IsNextRow())
	{
		db->GetStr("ARTC_CODE", zTemp);
		ir_cvtcode_uro_6e(zTemp, zArtc);


		db->GetStr("SYMBOL", zTemp);
		ir_cvtcode_uro_6e(zTemp, zSymbol);

		nDotCnt = db->GetLong("DOT_CNT");

		//TODO
		/*if (strncmp(zSymbol, "CL", 2) != 0
			) {
			db->Next();
			continue;
		}*/
		
		// KR 은 CLQ7, 다른곳은 CLQ17 
		ir_cvtcode_HD_KR(zSymbol, zTemp);
		std::string symbol = zSymbol;

		CChartMaker* p = new CChartMaker(zSymbol, zArtc, nDotCnt, g_unSaveThread, (saveYn[0]=='Y'));

		g_mapSymbol[symbol] = p;
		g_log.log(LOGTP_SUCC, "[%s][%s] Chart Symbol", zArtc, zSymbol);
		//printf("[%s][%s] 차트구성종목\n", zArtc, zSymbol);

		db->Next();
	}
	db->Close();

	return TRUE;
}


BOOL	InitApiClient()
{
	// API 로 부터 시세 수신
	char zIP[32], port[32];
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "IP", zIP);
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "PORT", port);
	printf("API IP(%s)PORT(%s)", zIP, port);
	g_pApiRecv = new CTcpClient("ChartMaker");
	if (!g_pApiRecv->Begin(zIP, atoi(port), 10))
	{
		g_log.log(LOGTP_FATAL, "%s", g_pApiRecv->GetMsg());
	}
	else
		g_log.log(LOGTP_SUCC, "TCP CLIENT Initialize and connect OK(IP:%s)(PORT:%s)", zIP,  port);
	
	g_pApiRecv->StartRecvData();

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
	strcat(Desc, " Service");

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
		printf("There is no service process in same directory");
		return;
	}

	////	종속 서비스
	CHAR	szDir[_MAX_PATH];
	char szDependency[64] = { 0, };

	CProp prop;
	prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY);
	strcpy(szDir, prop.GetValue("CONFIG_DIR_CHART"));

	CUtil::GetCnfgFileNm(szDir, EXENAME, g_zConfig);
	CUtil::GetConfig(g_zConfig, "SERVICE", "DEPENDENCY", szDependency);

	hSrv = CreateService(hScm, SERVICENAME, DISPNAME,
        SERVICE_ALL_ACCESS, 
		SERVICE_WIN32_OWN_PROCESS, 
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		SrvPath,
		NULL,
		NULL,
		szDependency,
		NULL,
		NULL);

	if(hSrv == NULL)
	{
		printf("Failed to install the service : %d\n", GetLastError());
	}
	else
	{
		lpDes.lpDescription = Desc;
		ChangeServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, &lpDes);
		//log.LogEventInf(-1, "서비스를 성공적으로 설치했습니다.");
		printf("Succeeded in installing the service.(dependency:%s)\n", szDependency);
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
		printf("Can't connect to SCM\n");
		return;
	}

	hSrv = OpenService(hScm, SERVICENAME, SERVICE_ALL_ACCESS);	
	if(hSrv == NULL)
	{
		CloseServiceHandle(hScm);
		//log.LogEventErr(-1, "서비스가 설치되어 있지 않습니다");
		printf("Service is not installed: %d\n", GetLastError());
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
		printf("Succeeded in removing the service \n");
	}
	else{
		printf("Failed in removing the service\n");
	}
	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);
}
