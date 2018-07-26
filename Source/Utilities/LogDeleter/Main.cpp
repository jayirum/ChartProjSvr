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
void read_directory(_In_ const std::string&, _Out_ stringvec&);
void DeleteLogsFrom(std::string);
bool ReadINISettings();

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


//#define __APP_VERSION "v1.1"
#define __APP_VERSION "20171017_LOGDELETER_VERSION"

int  _Start()
{
	char	msg[512] = { 0, };
	CHAR	szDir[_MAX_PATH];


	//	GET LOG DIR
	CProp prop;

	prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY);
	sprintf(g_sStr, "%s\\%s", prop.GetValue("CONFIG_DIR_LOGDELETER"), SERVICECONFIG); //reads from registry the location of .ini file.
	sprintf(g_sPresentDirectory, "%s", prop.GetValue("CONFIG_DIR_LOGDELETER"));

	CUtil::GetCnfgFileNm(szDir, EXENAME, g_zConfig);
	CUtil::GetConfig(g_zConfig, "DIR", "LOG", szDir);
	g_log.OpenLog(g_sLogDirectory, EXENAME);

	g_log.log(LOGTP_SUCC, "-----------------------------------------------------");
	g_log.log(LOGTP_SUCC, "Version[%s] %s", __DATE__, __APP_VERSION);
	g_log.log(LOGTP_SUCC, "-----------------------------------------------------");
	printf("Version[%s] %s\n", __DATE__, __APP_VERSION);

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
		if (!ReadINISettings())
		{
			g_log.log(LOGTP_SUCC, "INI File cannot be read.");
			return 0;
		}
		
		//g_log.log(LOGTP_SUCC, "%s", g_sLogDirectory);
		//g_log.log(LOGTP_SUCC, "%s", g_sPresentDirectory);
		DeleteLogsFrom(g_sLogDirectory);  
		//DeleteLogsFrom(g_sPresentDirectory); 
		
		Sleep(SERVICEWAITMILLISECONDS);
	}
	DWORD ret = WaitForSingleObject(g_hDieEvent, INFINITE);

	CoUninitialize();
	return 0;
}

void DeleteLogsFrom(std::string i_sLocation)
{
	char	msg[512] = { 0, };
	char	zFileName[_MAX_PATH];
	stringvec svVector;
	string sFileDate;
	string sFilenameAndLocation="";

	svVector.clear();

	read_directory(i_sLocation, svVector);
	
	for (unsigned int t = 0; t < svVector.size(); ++t)
	{
		sFileDate = GetFileDate(svVector.at(t).c_str(), 12);
		ZeroMemory(zFileName, sizeof(zFileName));

		GetDateBeforeNDays(msg, g_nDeleteAfterNDays);
		string sDateBefore(msg);

		if (sFileDate.compare(sDateBefore) < 0)// if the file sFileDate is indeed before sDateBefore ...
		{
			sprintf(zFileName, "%s\\%s", i_sLocation.c_str(), svVector.at(t).c_str());
			std::remove(zFileName); //delete the file
			g_log.log(LOGTP_SUCC, "File Deleted : %s", zFileName); //log the deleted filename
		}
	}

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
	GetCurrentDirectoryA(MAX_PATH, SrvPath);
	strcat(SrvPath, "\\");
	strcat(SrvPath, EXENAME);

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

void read_directory(const std::string& i_sName, stringvec& o_vVector) // retrieves all .log files and pass by reference to o_vVector
{
	std::string pattern(i_sName);
	pattern.append(SERVICEPATTERN);
	WIN32_FIND_DATAA data;
	HANDLE hFind;

	o_vVector.clear();
	if ((hFind = FindFirstFileA(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			o_vVector.push_back(data.cFileName);
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}
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

bool ReadINISettings()
{
	//	GET LOG DIR
	CProp prop;

	prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY);
	sprintf(g_zConfig, "%s\\%s", prop.GetValue("CONFIG_DIR_LOGDELETER"), SERVICECONFIG); //reads from registry the location of .ini file.
	CUtil::GetConfig(g_zConfig, SERVICESECTION, LOGDIRECTORYKEY, g_sLogDirectory);
	CUtil::GetConfig(g_zConfig, SERVICESECTION, DELETENUMBERKEY, &g_sStr[0]);
	//g_log.OpenLog(prop.GetValue("CONFIG_DIR_LOGDELETER"), EXENAME);//v1.0 for opening new log filel at running exe location 
	g_log.OpenLog(g_sLogDirectory,EXENAME); //v1.1
	g_nDeleteAfterNDays = CUtil::Str2N(g_sStr);

	if (strlen(g_sLogDirectory)==0 || g_nDeleteAfterNDays == 0)
		/*
		g_log.log(LOGTP_SUCC, "Please ensure LogDeleter.ini file is in the same folder of this executable and the settings are according to specifications below.");
		g_log.log(LOGTP_SUCC, "[Setting]");
		g_log.log(LOGTP_SUCC, "LOGDIRECTORY=location");
		g_log.log(LOGTP_SUCC, "DeleteAfterNDays=dd");
		*/
		return false;
	return true;
}