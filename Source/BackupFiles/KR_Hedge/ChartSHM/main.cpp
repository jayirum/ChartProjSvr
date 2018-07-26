#pragma warning(disable:4786)
#pragma warning(disable:4503)


#include "main.h"
#include <windows.h>
#include <winsvc.h>
#include <stdio.h>
#include <time.h>
#include "../../IRUM_UTIL/Log.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/SharedMem.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "../Common/KRCommon.h"
#include "SaveChart.h"

//���� ���� ��ü
void __stdcall ServiceStart(DWORD argc, LPTSTR* argv);
//���� ���� 
void __stdcall SCMHandler(DWORD opcode);
//���� ȯ�� ����
void __stdcall SetStatus(DWORD dwState,
						  DWORD dwAccept = SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_PAUSE_CONTINUE);
void install();
void uninstall();

SC_HANDLE hScm ;
SC_HANDLE hSrv ;
SERVICE_STATUS ss;

PSECURITY_DESCRIPTOR pSD;			// Pointer to SD.
SECURITY_ATTRIBUTES sa;
//DWORD	m_dwThrID;					//������ ���̵�

SERVICE_STATUS_HANDLE g_hXSS;		//���� ȯ�� �۷ι� �ڵ�
DWORD	g_XSS;						//���� ���� ���� ���庯��

BOOL	g_bDebug;
HANDLE	g_hDieEvent;				// ���α׷� ��ü�� ������ �̺�Ʈ
BOOL	g_bContinue = TRUE;			// ���α׷� ��ü�� ������ �÷���
char	g_zConfig[_MAX_PATH];

CRITICAL_SECTION	g_Console;
CLogMsg g_log;


CSharedMem	shmChart, shmLast, shmIndi;
char szMaxSymbolCnt[128] = { 0, }, szMaxChartKind[128] = { 0, };

BOOL InitChartShm()
{
	////////////////////////////////////////////////////////////////////////////////////
	// CHART SHM
	
	CUtil::GetConfig(g_zConfig, "SHM_INFO", "MAX_SYMBOL_CNT", szMaxSymbolCnt);
	CUtil::GetConfig(g_zConfig, "SHM_INFO", "MAX_CHART_KIND", szMaxChartKind);

	INIT_DATA	shmData;
	shmData.lMaxGroupCnt = atoi(szMaxSymbolCnt) * atoi(szMaxChartKind);	// 50�� 
	shmData.lGroupKeySize = LEN_SHM_GROUP_KEY;				// SYMBOL + ��Ʈ����
	shmData.lHeaderSize = 0;				//	Header �� ����.
	shmData.lStructSize = sizeof(_ST_SHM_CHART_UNIT);
	shmData.lStructKeySize = LEN_SHM_STRUCT_KEY;
	shmData.lMaxStructCnt = MAX_CHART_UNIT_CNT ;	// 1������ => �����δ� ���� reset
	g_log.log(LOGTP_SUCC, "SHM ũ�� = (�ѱ׷찹��(�ɺ���:%d)*(��Ʈ��:%d)=%d) * (��Ʈ����ũ��:%d * ����Ʈ��(1440*1��):%d) = %d (%d MB)",
		atoi(szMaxSymbolCnt), atoi(szMaxChartKind), shmData.lMaxGroupCnt,
		shmData.lStructSize, shmData.lStructSize*shmData.lMaxStructCnt,
		(shmData.lMaxGroupCnt*shmData.lStructSize*shmData.lMaxStructCnt),
		(shmData.lMaxGroupCnt*shmData.lStructSize*shmData.lMaxStructCnt) / 1024 / 1024);
			

	//	GET SHM NAME
	char szSHMName[128] = { 0, }, szMutexName[128] = { 0, };
	CUtil::GetConfig(g_zConfig, "SHM_INFO", "SHM_NAME", szSHMName);
	CUtil::GetConfig(g_zConfig, "SHM_INFO", "MUTEX_NAME", szMutexName);

	//	CREATE SHM
	int nRet = shmChart.Create(NULL, &shmData, szSHMName, szMutexName);
	if (nRet<1)
	{
		printf("[SHM CREATE ERROR](%s)(%s)(%s)\n", szSHMName, szMutexName, shmChart.GetErr());
		g_log.log(LOGTP_ERR, "[SHM CREATE ERROR](%s)(%s)(%s)", szSHMName, szMutexName, shmChart.GetErr());
		return FALSE;
	}

	// �̹� open
	else if (nRet == 0)
	{
		if (!shmChart.Open(szSHMName, szMutexName))
		{
			printf( "[SHM OPEN ERROR](%s)(%s)(%s)\n", szSHMName, szMutexName, shmChart.GetErr());
			g_log.log(LOGTP_ERR, "[SHM OPEN ERROR](%s)(%s)(%s)", szSHMName, szMutexName, shmChart.GetErr());
			return FALSE;
		}
		g_log.log(LOGTP_SUCC, "�̹� SHM �� �����Ǿ� �־ OPEN �Ѵ�.(%s)(%s)", szSHMName, szMutexName);
		printf("�̹� SHM �� �����Ǿ� �־ OPEN �Ѵ�.(%s)(%s)\n", szSHMName, szMutexName);
	}
	else
	{
		if (!shmChart.SetInitSize(shmData.lMaxGroupCnt
			, shmData.lGroupKeySize
			, shmData.lHeaderSize
			, shmData.lStructSize
			, shmData.lStructKeySize
			, shmData.lMaxStructCnt)
			)
		{
			g_log.log(LOGTP_ERR, "[SHM SetInitSize ERROR](%s)(%s)", szSHMName, szMutexName);
			printf("[SHM SetInitSize ERROR](%s)(%s)\n", szSHMName, szMutexName);
			return FALSE;
		}
	}
	g_log.log(LOGTP_SUCC, "(%s)(%s) SHM ���� ����", szSHMName, szMutexName);
	printf("(%s)(%s) SHM ���� ����\n", szSHMName, szMutexName);
	
	return TRUE;
}

BOOL InitLastShm()
{

	////////////////////////////////////////////////////////////////////////////////////
	// LAST CHART NAME SHM
	INIT_DATA	shmData;

	// GROUPKEY (����+CHART TP) ���� 1���� ������Ʈ �̸��� �ִ�.
	shmData.lMaxGroupCnt = atoi(szMaxSymbolCnt) * atoi(szMaxChartKind);	// 500�� 
	shmData.lGroupKeySize = LEN_SHM_GROUP_KEY;				// SYMBOL + ��Ʈ����
	shmData.lHeaderSize = 0;				//	Header �� ����.
	shmData.lStructSize = sizeof(ST_SHM_LASTCHART);
	shmData.lStructKeySize = LEN_SHM_GROUP_KEY;
	shmData.lMaxStructCnt = 1;

	//	GET SHM NAME
	char szSHMName[128] = { 0, }, szMutexName[128] = { 0, };
	CUtil::GetConfig(g_zConfig, "SHM_LASTCHART_NM", "SHM_NAME", szSHMName);
	CUtil::GetConfig(g_zConfig, "SHM_LASTCHART_NM", "MUTEX_NAME", szMutexName);

	//	CREATE SHM
	int nRet = shmLast.Create(NULL, &shmData, szSHMName, szMutexName);
	if (nRet<1)
	{
		g_log.log(LOGTP_ERR, "[SHM CREATE ERROR](%s)(%s)(%s)", szSHMName, szMutexName, shmLast.GetErr());
		printf( "[SHM CREATE ERROR](%s)(%s)(%s)\n", szSHMName, szMutexName, shmLast.GetErr());
		return FALSE;
	}

	// �̹� open
	else if (nRet == 0)
	{
		if (!shmLast.Open(szSHMName, szMutexName))
		{
			g_log.log(LOGTP_ERR, "[SHM OPEN ERROR](%s)(%s)(%s)", szSHMName, szMutexName, shmLast.GetErr());
			printf( "[SHM OPEN ERROR](%s)(%s)(%s)\n", szSHMName, szMutexName, shmLast.GetErr());
			return FALSE;
		}

		printf( "�̹� SHM �� �����Ǿ� �־ OPEN �Ѵ�.(%s)(%s)\n", szSHMName, szMutexName);
		g_log.log(LOGTP_SUCC, "�̹� SHM �� �����Ǿ� �־ OPEN �Ѵ�.(%s)(%s)", szSHMName, szMutexName);
	}
	else
	{
		if (!shmLast.SetInitSize(shmData.lMaxGroupCnt
			, shmData.lGroupKeySize
			, shmData.lHeaderSize
			, shmData.lStructSize
			, shmData.lStructKeySize
			, shmData.lMaxStructCnt)
			)
		{
			g_log.log(LOGTP_ERR, "[SHM SetInitSize ERROR](%s)(%s)\n", szSHMName, szMutexName);
			printf("[SHM SetInitSize ERROR](%s)(%s)", szSHMName, szMutexName);
			return FALSE;
		}
		printf( "(%s)(%s) SHM ���� ����\n", szSHMName, szMutexName);
		g_log.log(LOGTP_SUCC, "(%s)(%s) SHM ���� ����", szSHMName, szMutexName);
	}
	return TRUE;
}



BOOL InitIndiShm()
{

	////////////////////////////////////////////////////////////////////////////////////
	// LAST CHART NAME SHM
	INIT_DATA	shmData;

	// GROUPKEY (����+CHART TP) ���� 1���� ������Ʈ �̸��� �ִ�.
	shmData.lMaxGroupCnt = atoi(szMaxSymbolCnt) * atoi(szMaxChartKind);	// 500�� 
	shmData.lGroupKeySize = LEN_SHM_GROUP_KEY;				// SYMBOL + ��Ʈ����
	shmData.lHeaderSize = 0;				//	Header �� ����.
	shmData.lStructSize = sizeof(ST_SHM_INDICATOR);
	shmData.lStructKeySize = LEN_INDICHART_NM;
	shmData.lMaxStructCnt = 1;

	//	GET SHM NAME
	char szSHMName[128] = { 0, }, szMutexName[128] = { 0, };
	CUtil::GetConfig(g_zConfig, "SHM_INDICATOR", "SHM_NAME", szSHMName);
	CUtil::GetConfig(g_zConfig, "SHM_INDICATOR", "MUTEX_NAME", szMutexName);

	//	CREATE SHM
	int nRet = shmIndi.Create(NULL, &shmData, szSHMName, szMutexName);
	if (nRet<1)
	{
		g_log.log(LOGTP_ERR, "[SHM CREATE ERROR](%s)(%s)(%s)\n", szSHMName, szMutexName, shmIndi.GetErr());
		return FALSE;
	}

	// �̹� open
	else if (nRet == 0)
	{
		if (!shmIndi.Open(szSHMName, szMutexName))
		{
			g_log.log(LOGTP_ERR, "[SHM OPEN ERROR](%s)(%s)(%s)\n", szSHMName, szMutexName, shmIndi.GetErr());
			return FALSE;
		}

		g_log.log(LOGTP_SUCC, "�̹� SHM �� �����Ǿ� �־ OPEN �Ѵ�.(%s)(%s)\n", szSHMName, szMutexName);
	}
	else
	{
		if (!shmIndi.SetInitSize(shmData.lMaxGroupCnt
			, shmData.lGroupKeySize
			, shmData.lHeaderSize
			, shmData.lStructSize
			, shmData.lStructKeySize
			, shmData.lMaxStructCnt)
			)
		{
			g_log.log(LOGTP_ERR, "[SHM SetInitSize ERROR](%s)(%s)\n", szSHMName, szMutexName);
			return FALSE;
		}
		g_log.log(LOGTP_SUCC, "(%s)(%s) SHM ���� ����\n", szSHMName, szMutexName);
	}
	return TRUE;
}


#define __APP_VERSION "v1.0"

int  _Start()
{
	char	msg[512] = { 0, };
	CHAR	szDir[_MAX_PATH];

	//	GET LOG DIR
	CProp prop;
	prop.SetBaseKey(HKEY_LOCAL_MACHINE, IRUM_DIRECTORY);
	strcpy(szDir, prop.GetValue("CONFIG_DIR"));



	CUtil::GetCnfgFileNm(szDir, EXENAME, g_zConfig);
	CUtil::GetConfig(g_zConfig, "DIR", "LOG", szDir);
	g_log.OpenLog(szDir, EXENAME);

	g_log.log(LOGTP_SUCC, "[%s](%s)", SERVICENAME, __APP_VERSION);
	printf("[%s](%s)\n", SERVICENAME, __APP_VERSION);

	//---------------------------------------------
	//	���α׷� ��ü�� ������ �̺�Ʈ
	//---------------------------------------------
	g_hDieEvent = CreateEvent(&sa, TRUE, FALSE, NULL);

	if (g_bDebug) {
		//CUtil::LogPrint(&g_log, TRUE, "**************************\n");
		//CUtil::LogPrint(&g_log, TRUE, "** ���񽺸� �����մϴ�. **\n");
		//CUtil::LogPrint(&g_log, TRUE, "**************************\n");
	}
	else {
		SetStatus(SERVICE_RUNNING);
		//log.LogEventInf(-1," ���񽺸� �����մϴ�.");
	}


	if (!InitChartShm() ||
		!InitLastShm() //||
		//!InitIndiShm()
		)
		return 0;
	

	printf("shm create ok\n");

	Sleep(1000);

	CSaveChart* p[2];	// 1��, 5��
	for (int i = 1; i < 3; i++)
	{
		p[i-1] = new CSaveChart((CHART_TP)i, g_zConfig);
		p[i-1]->Initialize();
	}
	DWORD ret = WaitForSingleObject(g_hDieEvent, INFINITE);

	for (int i = 1; i < 3; i++)
	{
		delete p[i - 1];
	}
	
	CoUninitialize();
	return 0;
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
	g_XSS = dwState;			//���� ���� ����

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
		//log.LogEventErr(-1,"���� ��Ʈ�� �ڵ鷯�� ����� �� �����ϴ�.");
		return ;
	}

	//���񽺰� ���� ������ �˸���
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
	_Start();												//���� ����

	//log.LogEventInf(-1,"���񽺰� ���������� �����߽��ϴ�.");
	SetStatus(SERVICE_STOPPED, 0);					///���� ����
	return;
}

void install()
{
	char SrvPath[MAX_PATH];
	SERVICE_DESCRIPTION lpDes;
	char Desc[64];
	strcpy(Desc, SERVICENAME);
	strcat(Desc, " ����!");

	hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if(hScm == NULL)
	{
		//log.LogEventErr(-1, "���� �޴����� ������ �� �����ϴ�");
		return ;
	}
	GetCurrentDirectory(MAX_PATH, SrvPath);
	strcat(SrvPath, "\\");
	strcat(SrvPath, EXENAME);
	//sprintf(SrvPath, "D:\\IBSSERVER(BACKUPSYSTEM)\\RealShmUP\\Release\\RealShmUP.exe");
	
	if(_access(SrvPath, 0) != 0)
	{
		CloseServiceHandle(hScm);
		//log.LogEventErr(-1, "���� ���α׷��� ���� ���丮�� �����ϴ�");
		printf("���� ���α׷��� ���� ���丮�� �����ϴ�\n");
		return;
	}

	////	���� ����
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
		NULL, //szDependency,
		NULL,
		NULL);

	if(hSrv == NULL)
	{
		printf("���񽺸� ��ġ���� �� �߽��ϴ� : %d\n", GetLastError());
	}
	else
	{
		lpDes.lpDescription = Desc;
		ChangeServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, &lpDes);
		//log.LogEventInf(-1, "���񽺸� ���������� ��ġ�߽��ϴ�.");
		printf("���񽺸� ���������� ��ġ�߽��ϴ�.\n");
		CloseServiceHandle(hSrv);
	}
	CloseServiceHandle(hScm);
}

void uninstall()
{
	hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if(hScm == NULL)
	{
		//log.LogEventErr(-1, "���� �޴����� ������ �� �����ϴ�");
		printf("���� �޴����� ������ �� �����ϴ�\n");
		return;
	}

	hSrv = OpenService(hScm, SERVICENAME, SERVICE_ALL_ACCESS);	
	if(hSrv == NULL)
	{
		CloseServiceHandle(hScm);
		//log.LogEventErr(-1, "���񽺰� ��ġ�Ǿ� ���� �ʽ��ϴ�");
		printf("���񽺰� ��ġ�Ǿ� ���� �ʽ��ϴ� : %d\n", GetLastError());
		return ;
	}	

	//���� ����
	QueryServiceStatus(hSrv, &ss);
	if(ss.dwCurrentState != SERVICE_STOPPED)
	{
		ControlService(hSrv, SERVICE_CONTROL_STOP, &ss);
		Sleep(2000);
	}

	//���� ����
	if(DeleteService(hSrv))
	{
		//log.LogEventInf(-1, "���������� ���񽺸� �����߽��ϴ�");
		printf("���������� ���񽺸� �����߽��ϴ�\n");
	}
	else{
		printf("���񽺸� �������� ���߽��ϴ�.\n");
	}
	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);
}
