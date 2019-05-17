#pragma warning(disable:4786)
#pragma warning(disable:4503)



#include "../../IRUM_UTIL/MCastRecv.h"
#include "../../IRUM_UTIL/ADOFunc.h"
#include "main.h"
#include "ChartMaker.h"
#include <winsvc.h>
#include <stdio.h>
#include <time.h>
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/MemPool.h"
#include "../../IRUM_UTIL/LogMsg.h"
//#include "../../IRUM_UTIL/NanoPubSub.h"
#include "../../IRUM_INC/IRUM_Common.h"

#include "ChartMaker.h"
#include <list>
#include <map>
#include <string>
#include <Windows.h>

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
volatile BOOL	g_bContinue = TRUE;			// ���α׷� ��ü�� ������ �÷���
char	g_zConfig[_MAX_PATH];



BOOL	DBOpen();
BOOL	LoadSymbol();
//BOOL	InitSiseRecv();
BOOL	InitMemPool();
//static unsigned WINAPI SiseRecvThread(LPVOID lp);
static unsigned WINAPI ChartSaveThread(LPVOID lp);



CRITICAL_SECTION	g_Console;
CLogMsg				g_log;
CDBPoolAdo			*g_ado		= NULL;
//TODO CSiseRecv			*g_pMDSub = NULL;
//CNanoPubSub			*g_pMDSub = NULL;
//CMCastRecv			*g_pMcastRecv = NULL;
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
	g_log.log(LOGTP_SUCC, "����[%s]%s", __DATE__, __APP_VERSION);
	g_log.log(LOGTP_SUCC, "-----------------------------------------------------");
	printf("����[%s]%s\n", __DATE__, __APP_VERSION);

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

	// ��Ʈ ���� ������
	g_hSaveThread = (HANDLE)_beginthreadex(NULL, 0, &ChartSaveThread, NULL, 0, &g_unSaveThread);


	if (!InitMemPool()) {
		return 0;
	}

	if (!DBOpen())
		return 0;

	//if (!InitSiseRecv())
	//	return 0;

	if (!LoadSymbol())
		return 0;

	

	DWORD ret = WaitForSingleObject(g_hDieEvent, INFINITE);

	
	SAFE_DELETE(g_ado);
	//SAFE_DELETE(g_pMcastRecv);
	SAFE_CLOSEHANDLE(g_hRecvThread);
	SAFE_CLOSEHANDLE(g_hSaveThread);
	SAFE_DELETE(g_pMemPool);
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
//
//static unsigned WINAPI SiseRecvThread(LPVOID lp)
//{
//	ST_PACK2CHART* pSise;
//	char zSymbol[128];
//
//	while (g_bContinue)
//	{
//		Sleep(1);
//		char* pBuf = g_pMemPool->get();
//		//TODO int nLen = g_pMDSub->RecvData(pBuf);
//		//int nLen = g_pMDSub->Subscribe(pBuf);
//		//if (nLen == 0) {
//		//	g_pMemPool->release(pBuf);
//		//	continue;
//		//}
//		int nLen = g_pMcastRecv->RecvData(pBuf, MEM_BLOCK_SIZE);
//		if (nLen < 0)
//		{
//			g_log.log(LOGTP_ERR, "SISE RECV ERROR:%s", g_pMcastRecv->GetMsg());
//			printf("SISE RECV ERROR:%s", g_pMcastRecv->GetMsg());
//			g_pMemPool->release(pBuf);
//			Sleep(1000);
//			continue;
//		}
//		else
//			printf("[RECV](%.*s)\n", nLen, pBuf);
//
//		pSise = (ST_PACK2CHART*)pBuf;
//		sprintf(zSymbol, "%.*s", sizeof(pSise->ShortCode), pSise->ShortCode);
//		CUtil::TrimAll(zSymbol, strlen(zSymbol));
//		std::string sSymbol = zSymbol;
//
//		std::map<std::string, CChartMaker*>::iterator it = g_mapSymbol.find(sSymbol);
//		if (it == g_mapSymbol.end())
//		{
//			//g_log.log(LOGTP_ERR, "[%s] ������ ��û�� ������ �ƴϴ�.", sSymbol.c_str());
//		}
//		else
//		{
//			CChartMaker* p = (*it).second;
//			PostThreadMessage(p->GetMyThreadID(), WM_CHART_DATA, 0, (LPARAM)pBuf);
//			//printf("SISE RECV(%s)\n", pBuf);
//		}
//	}
//
//	return 0;
//}


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
					g_log.log(LOGTP_ERR, "CHART DATA Save ����(%s)(%s)", db->GetError(), zQ);
					printf("CHART DATA Save ����(%s)(%s)\n", db->GetError(), zQ);
				}
				//else
					//g_log.log(LOGTP_SUCC, "DB SAVE(%s)", zQ);
				delete p;
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
	g_log.log(LOGTP_ERR, "DB OPEN OK(IP:%s)(ID:%s)(PWD:%s)(DB:%s)", ip, id, pwd, name);
	return TRUE;
}


BOOL LoadSymbol()
{

	CDBHandlerAdo db(g_ado->Get());
	char zQ[1024];
	sprintf(zQ, "EXEC CHART_GET_SYMBOL");

	if (!db->ExecQuery(zQ))
	{
		g_log.log(LOGTP_ERR, "�������� ����(%s)(%s)", zQ, db->GetError());
		return FALSE;
	}

	char yearLen[32], euroDollar[32];
	CUtil::GetConfig(g_zConfig, "SYMBOL_TYPE", "YEAR_LENGTH", yearLen);
	CUtil::GetConfig(g_zConfig, "SYMBOL_TYPE", "EURO_DOLLAR", euroDollar);


	char zTemp[32], zSymbol[32], zArtc[32];
	while (db->IsNextRow())
	{
		db->GetStr("ARTC_CODE", zTemp);
		ir_cvtcode_uro_6e(zTemp, zArtc);


		db->GetStr("SYMBOL", zTemp);
		ir_cvtcode_uro_6e(zTemp, zSymbol);

		// KR �� CLQ7, �ٸ����� CLQ17 
		ir_cvtcode_HD_KR(zSymbol, zTemp);
		std::string symbol = zSymbol;


		CChartMaker* p = new CChartMaker(zSymbol, zArtc, g_pMemPool, g_unSaveThread);

		g_mapSymbol[symbol] = p;
		g_log.log(LOGTP_SUCC, "[%s][%s] ��Ʈ��������", zArtc, zSymbol);
		printf("[%s][%s] ��Ʈ��������\n", zArtc, zSymbol);

		db->Next();
	}
	db->Close();

	return TRUE;
}


BOOL	InitSiseRecv()
{
	// ChartApiKR �� ���� �ü� ����
	char zLocalIP[32], zMcastIP[32], port[32];
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "IP", zLocalIP);
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "IP_MCAST", zMcastIP);
	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "PORT", port);

	//g_pMcastRecv = new CMCastRecv();
	//if (!g_pMcastRecv->Begin(zLocalIP, zMcastIP, atoi(port)))
	//{
	//	g_log.log(LOGTP_FATAL, "%s", g_pMcastRecv->GetMsg());
	//	return FALSE;/
	//}
	//g_log.log(LOGTP_SUCC, "��Ƽĳ��Ʈ���� �ʱ�ȭ ����(LOCAL:%s)(MCAST:%s)(PORT:%s)", zLocalIP, zMcastIP, port);

	//// ���� ������
	//g_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, &SiseRecvThread, NULL, 0, &g_unRecvThread);


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
		"IRUM_ChartShmQueue",	//NULL, //szDependency,
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