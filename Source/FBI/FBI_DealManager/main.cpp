
#pragma warning(disable:4786)
#pragma warning(disable:4503)


//#include "AlgoFront.h"

#include "main.h"
#include "FBIMainProc.h"
#include <Windows.h>
#include <stdio.h>
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_util/IRUM_Common.h"
#include "../../IRUM_UTIL/MemPool.h"

// service main function
void __stdcall ServiceStart(DWORD argc, LPTSTR* argv);

// service control
void __stdcall SCMHandler(DWORD opcode);

// service status
void __stdcall SetStatus(DWORD dwState,
	DWORD dwAccept = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE);
void install();
void uninstall();

SC_HANDLE hScm;
SC_HANDLE hSrv;
SERVICE_STATUS ss;

PSECURITY_DESCRIPTOR pSD;			// Pointer to SD.
SECURITY_ATTRIBUTES sa;
//DWORD	m_dwThrID;					

SERVICE_STATUS_HANDLE g_hXSS;		// service env global handle
DWORD	g_XSS;

BOOL	g_bDebug;
HANDLE	g_hDieEvent;				// event for terminating process
CRITICAL_SECTION	g_Console;

/// global variables shared with all classes
BOOL		g_bContinue = TRUE;	// flag whether continue process or not
CLogMsg		g_log;
char		g_zConfig[_MAX_PATH];
CMemPool	g_memPool(_FBI::MEM_PRE_ALLOC, _FBI::MEM_MAX_ALLOC, _FBI::MEM_BLOCK_SIZE);
char		g_zMyName[128];

char SERVICENAME[128], DISPNAME[128], DESC[128];

int  _Start()
{
	char	msg[512] = { 0, };
	CHAR	szDir[_MAX_PATH] = { 0 };

	char szNotificationServer[32], szNotificationPort[32];
	CUtil::GetConfig(g_zConfig, "DIR", "LOG", szDir);
	CUtil::GetConfig(g_zConfig, "NOTIFICATION", "NOTIFICATION_SERVER_IP", szNotificationServer);
	CUtil::GetConfig(g_zConfig, "NOTIFICATION", "NOTIFICATION_SERVER_PORT", szNotificationPort);

	g_log.OpenLogEx(szDir, EXENAME, szNotificationServer, atoi(szNotificationPort), SERVICENAME);

	g_log.log(LOGTP_SUCC, "-----------------------------------------------------");
	g_log.log(LOGTP_SUCC, "Version[%s] %s", __DATE__, __APP_VERSION);
	g_log.log(LOGTP_SUCC, "-----------------------------------------------------");



	//---------------------------------------------
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


	CFBIMainProc main;
	if (main.Initialize())
	{
		DWORD ret = WaitForSingleObject(g_hDieEvent, INFINITE);
	}

	CoUninitialize();
	return 0;
}



BOOL WINAPI ControlHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
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



/*
usage
filename servername
*/
int main(int argc, LPSTR *argv)
{
	g_bDebug = FALSE;

	char	msg[512] = { 0, };
	CHAR	szDir[_MAX_PATH];

	//	GET LOG DIR
	CProp prop;
	prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY);
	strcpy(szDir, prop.GetValue("CONFIG_DIR_CHART"));

	CUtil::GetCnfgFileNm(szDir, EXENAME, g_zConfig);

	CUtil::GetConfig(g_zConfig, "SERVICE", "SERVICE_NAME", SERVICENAME);
	CUtil::GetConfig(g_zConfig, "SERVICE", "DISP_NAME", DISPNAME);
	CUtil::GetConfig(g_zConfig, "SERVICE", "DESC", DESC);



	if ((argc > 1) &&
		((*argv[1] == '-') || (*argv[1] == '/')))
	{
		if (_stricmp("install", argv[1] + 1) == 0)
		{
			install();
		}
		else if (_stricmp("remove", argv[1] + 1) == 0 || _stricmp("delete", argv[1] + 1) == 0)
		{
			uninstall();
		}
		else if (_stricmp("debug", argv[1] + 1) == 0)
		{
			g_bDebug = TRUE;
			SetConsoleCtrlHandler(ControlHandler, TRUE);
			InitializeCriticalSection(&g_Console);

			_Start();

			DeleteCriticalSection(&g_Console);
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
		{ SERVICENAME, (LPSERVICE_MAIN_FUNCTION)ServiceStart },
		{ NULL, NULL }
	};

	if (!StartServiceCtrlDispatcher(stbl))
	{
		return  -1;
	}

	return 0;
}

void  __stdcall SetStatus(DWORD dwState, DWORD dwAccept)
{
	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState = dwState;
	ss.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	ss.dwWin32ExitCode = 0;
	ss.dwServiceSpecificExitCode = 0;
	ss.dwCheckPoint = 0;
	ss.dwWaitHint = 0;
	g_XSS = dwState;			//현재 상태 보관

	SetServiceStatus(g_hXSS, &ss);
	return;
}

void __stdcall SCMHandler(DWORD opcode)
{
	if (opcode == g_XSS)
		return;

	switch (opcode)
	{
	case SERVICE_CONTROL_PAUSE:
		SetStatus(SERVICE_PAUSE_PENDING, 0);
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
	if (g_hXSS == 0)
	{
		//log.LogEventErr(-1,"서비스 컨트롤 핸들러를 등록할 수 없습니다.");
		return;
	}

	//서비스가 시작 중임을 알린다
	SetStatus(SERVICE_START_PENDING);

	//// Allocate memory for the security descriptor.	
	pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(
		LPTR		//Specifies how to allocate memory
					// Allocates fixed memory && Initializes memory contents to zero.
		, SECURITY_DESCRIPTOR_MIN_LENGTH	//number of bytes to allocate
	);


	//// Initialize the new security descriptor.
	InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);


	//// Add a NULL descriptor ACL to the security descriptor.
	SetSecurityDescriptorDacl(pSD, TRUE, (PACL)NULL, FALSE);

	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = TRUE;


	g_bDebug = FALSE;
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
	if (hScm == NULL)
	{
		//log.LogEventErr(-1, "서비스 메니져와 연결할 수 없습니다");
		return;
	}
	GetCurrentDirectory(MAX_PATH, SrvPath);
	strcat(SrvPath, "\\");
	strcat(SrvPath, EXENAME);
	//sprintf(SrvPath, "D:\\IBSSERVER(BACKUPSYSTEM)\\RealShmUP\\Release\\RealShmUP.exe");

	if (_access(SrvPath, 0) != 0)
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

	if (hSrv == NULL)
	{
		printf("Failed to install the service : %d\n", GetLastError());
	}
	else
	{
		lpDes.lpDescription = Desc;
		ChangeServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, &lpDes);
		printf("Succeeded in installing the service.(dependency:%s)\n", szDependency);
		CloseServiceHandle(hSrv);
	}
	CloseServiceHandle(hScm);
}

void uninstall()
{
	hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (hScm == NULL)
	{
		//log.LogEventErr(-1, "서비스 메니져와 연결할 수 없습니다");
		printf("서비스 메니져와 연결할 수 없습니다\n");
		printf("Fail to connect service manager\n");
		return;
	}

	hSrv = OpenService(hScm, SERVICENAME, SERVICE_ALL_ACCESS);
	if (hSrv == NULL)
	{
		CloseServiceHandle(hScm);
		//log.LogEventErr(-1, "서비스가 설치되어 있지 않습니다");
		printf("서비스가 설치되어 있지 않습니다 : %d\n", GetLastError());
		printf("Service is not installed: %d\n", GetLastError());
		return;
	}

	//서비스 중지
	QueryServiceStatus(hSrv, &ss);
	if (ss.dwCurrentState != SERVICE_STOPPED)
	{
		ControlService(hSrv, SERVICE_CONTROL_STOP, &ss);
		Sleep(2000);
	}

	//서비스 제거
	if (DeleteService(hSrv))
	{
		//log.LogEventInf(-1, "성공적으로 서비스를 제거했습니다");
		printf("성공적으로 서비스를 제거했습니다\n");
		printf("succeeded in removing service\n");
	}
	else {
		printf("서비스를 제거하지 못했습니다.\n");
		printf("failed to remove service\n");
	}
	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);
}
