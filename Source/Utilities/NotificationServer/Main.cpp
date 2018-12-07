/*
--12102017 Mr Jay's Review
	- remove unnecessary variables [done]
	- clear vectors after usage / initialize [done]
	- do log using Mr. Jay's library [done]
	- when debugging , log also into file 
	- compare string should be less than 0 [done]
	- don't create thread unnecessarily [done]
	- use sprintf instead of cout [done]
	- in inireader, should not hardcode the memory size .. use sizeof [done]
	- definitions use IRUM_Common.h - D:\IRUM_SVR\SOURCE\IRUM_INC [done]
	- use the functions in D:\IRUM_SVR\SOURCE\IRUM_UTIL - like logging file procedures . chartqueueshm is a good example [done]
	- no need start time end time for running service [done]
	- use volatile for global memory, [done]

--12102017 Mr Jay's Review
	- put its own logs into the ini file
*/

#include "Main.h"


// service main function
void __stdcall ServiceStart(DWORD argc, LPTSTR* argv);

// service control
void __stdcall SCMHandler(DWORD opcode);

// service status
void __stdcall SetStatus(DWORD dwState,
	DWORD dwAccept = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE);
void install();
void uninstall();
std::string GetFileDate(_In_ std::string const&, _In_ size_t const);
void GetDateBeforeNDays(_Out_ char*, _In_ int);
string GetExecutableFilename();
string GetExecutablePath();
string GetExecutableConfig();
string GetExecutableFilenameOnly();
string GetExecutableLogFilenameOnly();
int fn_RandomBetween(int nStart, int nEnd);
int fn_GetSecond();

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
volatile BOOL	g_bContinue = TRUE;	// flag whether continue process or not
char	g_zConfig[_MAX_PATH];

//Following is for LogDeleter globals
char g_sStr[300];//for storing error messages

char g_sLogDirectory[_MAX_PATH];
char g_sPresentDirectory[_MAX_PATH];
int g_nDeleteAfterNDays;
//----

CRITICAL_SECTION	g_Console;
CLogMsg g_log;


//#define __APP_VERSION "v0.1"
#define __APP_VERSION "20181017_LOGNOTIFIER_VERSION"

int  _Start()
{
	char	msg[512] = { 0, };
	CHAR	szDir[_MAX_PATH];
	CHAR	szNotificationServer[_MAX_PATH];
	CHAR	szNotificationPort[_MAX_PATH];
	int		nNotificationPort;
	string szConfigFilename;


	//	GET LOG DIR
	CProp prop;

	printf("Path = %s\n", GetExecutablePath().c_str());
	sprintf(szDir,"%s",GetExecutablePath().c_str());
	szConfigFilename = GetExecutableConfig();
	sprintf(g_zConfig, "%s", szConfigFilename.c_str());
	//prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY);
	//sprintf(g_sStr, "%s\\%s", prop.GetValue("CONFIG_DIR_LOGDELETER"), SERVICECONFIG); //reads from registry the location of .ini file.
	//sprintf(g_sPresentDirectory, "%s", prop.GetValue("CONFIG_DIR_LOGDELETER"));

	CUtil::GetConfig(g_zConfig, "DIR", "LOG", szDir);

	// 
	CUtil::GetConfig(g_zConfig, "NOTIFICATION", "NOTIFICATION_SERVER_IP", szNotificationServer);
	CUtil::GetConfig(g_zConfig, "NOTIFICATION", "NOTIFICATION_SERVER_PORT", szNotificationPort);
	nNotificationPort = stoi(string(szNotificationPort));



	char szchFilename[MAX_PATH];
	sprintf(szchFilename, "%s", GetExecutableLogFilenameOnly().c_str());
	sprintf(g_sLogDirectory, "%s", szDir);
	g_log.OpenLogEx(g_sLogDirectory, szchFilename, szNotificationServer, nNotificationPort, __APP_VERSION) ;

	g_log.log(LOGTP_SUCC, "-----------------------------------------------------");
	g_log.log(LOGTP_SUCC, "Version[%s] %s", __DATE__, __APP_VERSION);
	g_log.log(LOGTP_SUCC, "-----------------------------------------------------");
	printf("Version[%s] %s\n", __DATE__, __APP_VERSION);
	g_log.log(LOGTP_SUCC,"Notification Server : %s \t Notification Port : %d \n", szNotificationServer, nNotificationPort);
	g_log.log(LOGTP_SUCC, "ApplicationName : %s \n", __APP_VERSION);

	//---------------------------------------------
	//---------------------------------------------
	g_hDieEvent = CreateEvent(&sa, TRUE, FALSE, NULL);

	if (g_bDebug) {
		//CUtil::LogPrint(&g_log, TRUE, "**************************\n");
		//CUtil::LogPrint(&g_log, TRUE, "** Start the service. **\n");
		//CUtil::LogPrint(&g_log, TRUE, "**************************\n");
	}
	else {
		SetStatus(SERVICE_RUNNING);
		//log.LogEventInf(-1," Start the service.");
	}

	/*
	* Perform main service function here
	*/
	while (g_bContinue == TRUE)
	{

		//workerthread

		//test
		int nTemp = fn_GetSecond();//fn_RandomBetween(1, 30);
		//g_log.log(LOGTP_SUCC, "Number is %d", nTemp);
		if (nTemp%20 == 0)
		{
			g_log.log(NOTIFY,"Notification Test from Irumnet Server");
			//g_log.log(NOTIFY, "Number is %d",nTemp);
		}
		//test
		
		Sleep(SERVICEWAITMILLISECONDS);
	}
	DWORD ret = WaitForSingleObject(g_hDieEvent, INFINITE); 
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


int main(int argc, LPSTR *argv)
{
	g_bDebug = FALSE;

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
	SERVICE_TABLE_ENTRYA stbl[] =
	{
		{ SERVICENAME, (LPSERVICE_MAIN_FUNCTIONA)ServiceStart },
		{ NULL, NULL }
	};

	if (!StartServiceCtrlDispatcherA(stbl))
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
	g_XSS = dwState;			//Keep current status

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
	g_hXSS = RegisterServiceCtrlHandlerA(SERVICENAME,
		(LPHANDLER_FUNCTION)SCMHandler);
	if (g_hXSS == 0)
	{
		//log.LogEventErr(-1,"Unable to register service control handler.");
		return;
	}

	//Notify service is starting
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
	_Start();												//Service execution

															//log.LogEventInf(-1,"Service is closed successfully.");
	SetStatus(SERVICE_STOPPED, 0);					///Stop service
	return;
}

// https://docs.microsoft.com/en-us/windows/desktop/services/querying-a-service-s-configuration
// Purpose: 
//   Retrieves and displays the current service configuration.
//
// Parameters:
//   None
// 
// Return value:
//   None
//
VOID __stdcall DoQuerySvc()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	LPQUERY_SERVICE_CONFIG lpsc = NULL;
	LPSERVICE_DESCRIPTION lpsd = NULL;
	DWORD dwBytesNeeded, cbBufSize, dwError;

	// Get a handle to the SCM database. 

	schSCManager = OpenSCManager(
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager)
	{
		printf("OpenSCManager failed (%d)\n", GetLastError());
		return;
	}

	// Get a handle to the service.

	schService = OpenService(
		schSCManager,          // SCM database 
		SERVICENAME,             // name of service 
		SERVICE_QUERY_CONFIG); // need query config access 

	if (schService == NULL)
	{
		printf("OpenService failed (%d)\n", GetLastError());
		CloseServiceHandle(schSCManager);
		return;
	}

	// Get the configuration information.

	if (!QueryServiceConfig(
		schService,
		NULL,
		0,
		&dwBytesNeeded))
	{
		dwError = GetLastError();
		if (ERROR_INSUFFICIENT_BUFFER == dwError)
		{
			cbBufSize = dwBytesNeeded;
			lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, cbBufSize);
		}
		else
		{
			printf("QueryServiceConfig failed (%d)", dwError);
			goto cleanup;
		}
	}

	if (!QueryServiceConfig(
		schService,
		lpsc,
		cbBufSize,
		&dwBytesNeeded))
	{
		printf("QueryServiceConfig failed (%d)", GetLastError());
		goto cleanup;
	}

	if (!QueryServiceConfig2(
		schService,
		SERVICE_CONFIG_DESCRIPTION,
		NULL,
		0,
		&dwBytesNeeded))
	{
		dwError = GetLastError();
		if (ERROR_INSUFFICIENT_BUFFER == dwError)
		{
			cbBufSize = dwBytesNeeded;
			lpsd = (LPSERVICE_DESCRIPTION)LocalAlloc(LMEM_FIXED, cbBufSize);
		}
		else
		{
			printf("QueryServiceConfig2 failed (%d)", dwError);
			goto cleanup;
		}
	}

	if (!QueryServiceConfig2(
		schService,
		SERVICE_CONFIG_DESCRIPTION,
		(LPBYTE)lpsd,
		cbBufSize,
		&dwBytesNeeded))
	{
		printf("QueryServiceConfig2 failed (%d)", GetLastError());
		goto cleanup;
	}

	// Print the configuration information.

	_tprintf(TEXT("%s configuration: \n"), SERVICENAME);
	_tprintf(TEXT("  Type: 0x%x\n"), lpsc->dwServiceType);
	_tprintf(TEXT("  Start Type: 0x%x\n"), lpsc->dwStartType);
	_tprintf(TEXT("  Error Control: 0x%x\n"), lpsc->dwErrorControl);
	_tprintf(TEXT("  Binary path: %s\n"), lpsc->lpBinaryPathName);
	_tprintf(TEXT("  Account: %s\n"), lpsc->lpServiceStartName);

	if (lpsd->lpDescription != NULL && lstrcmp(lpsd->lpDescription, TEXT("")) != 0)
		_tprintf(TEXT("  Description: %s\n"), lpsd->lpDescription);
	if (lpsc->lpLoadOrderGroup != NULL && lstrcmp(lpsc->lpLoadOrderGroup, TEXT("")) != 0)
		_tprintf(TEXT("  Load order group: %s\n"), lpsc->lpLoadOrderGroup);
	if (lpsc->dwTagId != 0)
		_tprintf(TEXT("  Tag ID: %d\n"), lpsc->dwTagId);
	if (lpsc->lpDependencies != NULL && lstrcmp(lpsc->lpDependencies, TEXT("")) != 0)
		_tprintf(TEXT("  Dependencies: %s\n"), lpsc->lpDependencies);

	LocalFree(lpsc);
	LocalFree(lpsd);

cleanup:
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
}

string GetExecutableFilename()
{
	vector<char> v_wch_Buffer;
	DWORD dwCopied = 0;
	do
	{
		v_wch_Buffer.resize(v_wch_Buffer.size() + MAX_PATH);
		dwCopied = GetModuleFileName(0, &v_wch_Buffer.at(0), v_wch_Buffer.size());
	} while (dwCopied >= v_wch_Buffer.size());

	v_wch_Buffer.resize(dwCopied);

	string szPath(v_wch_Buffer.begin(), v_wch_Buffer.end());

	return szPath;
}

string GetExecutableFilenameOnly()
{
	string szFilename,szFilenameOnly;
	szFilename = GetExecutableFilename();
	szFilenameOnly = szFilename.replace(szFilename.find(GetExecutablePath()+"\\"), strlen((GetExecutablePath()+"\\").c_str()), "");
	return szFilenameOnly;
}

string GetExecutablePath()
{
	string szBuffer;
	szBuffer = GetExecutableFilename();
	size_t size_t_Found = szBuffer.find_last_of("/\\");
	return szBuffer.substr(0, size_t_Found);
}

string GetExecutableConfig()
{
	string szBuffer;
	szBuffer = GetExecutableFilename();
	szBuffer = szBuffer.replace(szBuffer.find(".exe"), strlen(".exe"), ".ini");
	return szBuffer;
}

string GetExecutableLog()
{
	string szBuffer;
	szBuffer = GetExecutablePath();
	szBuffer = szBuffer + "\LOG";
	return szBuffer;
}

string GetExecutableLogFilenameOnly()
{
	string szBuffer;
	szBuffer = GetExecutableFilenameOnly();
	szBuffer = szBuffer.replace(szBuffer.find(".exe"), strlen(".exe"), ".log");
	return szBuffer;
}

void install()
{
	char SrvPath[MAX_PATH];
	SERVICE_DESCRIPTIONA lpDes;
	char sDesc[130];
	strcpy(sDesc, DESC);

	hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (hScm == NULL)
	{
		//log.LogEventErr(-1, "Can not connect with service manager");
		return;
	}
	strcpy(SrvPath, GetExecutableFilename().c_str());

	if (_access(SrvPath, 0) != 0)
	{
		CloseServiceHandle(hScm);
		//log.LogEventErr(-1, "The service program is not in the same directory");
		printf("There is no service process in same directory\n");
		return;
	}

	////	Dependent service
	//	char szDependency[64];
	// 	memset(szDependency, 0x00, sizeof(szDependency));	
	// 	CProp prop;
	// 	prop.SetBaseKey(HKEY_LOCAL_MACHINE, REG_FRONTSERVER);
	// 	strcpy(szDependency, prop.GetValue("Dependency"));
	// 	prop.Close();

	hSrv = CreateServiceA(hScm, SERVICENAME, DISPNAME,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		SrvPath,
		NULL,
		NULL,
		NULL, //szDependency,
		NULL,
		NULL);

	if (hSrv == NULL)
	{
		printf("Failed to install service : %d\n", GetLastError());
	}
	else
	{
		lpDes.lpDescription = sDesc;
		ChangeServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, &lpDes);
		//log.LogEventInf(-1, "succeeded in installing service.");
		printf("Succeeded in installing service.\n");
		CloseServiceHandle(hSrv);
	}
	CloseServiceHandle(hScm);
}

void uninstall()
{
	hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (hScm == NULL)
	{
		//log.LogEventErr(-1, "Fail to connect service manager");
		printf("Fail to connect service manager\n");
		return;
	}

	hSrv = OpenServiceA(hScm, SERVICENAME, SERVICE_ALL_ACCESS);
	if (hSrv == NULL)
	{
		CloseServiceHandle(hScm);
		//log.LogEventErr(-1, "Service is not installed");
		printf("Service is not installed: %d\n", GetLastError());
		return;
	}

	//Stop Service
	QueryServiceStatus(hSrv, &ss);
	if (ss.dwCurrentState != SERVICE_STOPPED)
	{
		ControlService(hSrv, SERVICE_CONTROL_STOP, &ss);
		Sleep(2000);
	}

	//Remove Service
	if (DeleteService(hSrv))
	{
		//log.LogEventInf(-1, "succeeded in removing service");
		printf("Succeeded in removing service\n");
	}
	else {
		printf("Failed to remove service\n");
	}
	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);
}


void GetDateBeforeNDays(char* o_pcDateBefore, int i_nDays)
{
	time_t timer;
	struct tm *t;

	timer = time(NULL) - (24 * 60 * 60 * i_nDays);
	t = localtime(&timer);

	sprintf(o_pcDateBefore, "%04d%02d%02d",
		t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
}

std::string GetFileDate(std::string const& i_sSource, size_t const i_nLength) //get the last 8 digits before extension
{
	if (i_nLength >= i_sSource.size()) { return i_sSource; }
	return i_sSource.substr(i_sSource.size() - i_nLength, 8);
}



int fn_RandomBetween(int nStart, int nEnd)
{
	srand(time(NULL));
	auto nRandomInteger = (rand() % nEnd) + nStart;
	return nRandomInteger;
}


int fn_GetSecond()
{
	time_t timer;

	struct tm *t;

	timer = time(NULL);
	t = localtime(&timer);

	return t->tm_sec;
}