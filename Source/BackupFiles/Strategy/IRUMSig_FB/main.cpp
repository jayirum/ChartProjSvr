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
//#include "../../IRUM_UTIL/NanoPull.h"
//#include "../../IRUM_UTIL/NanoPubSub.h"
//#include "../../IRUM_UTIL/NanoPush.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "../../IRUM_UTIL/QueueShm.h"
#include "SignalMaker.h"
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

volatile BOOL	g_bDebug;
HANDLE	g_hDieEvent;				// ���α׷� ��ü�� ������ �̺�Ʈ
volatile BOOL	g_bContinue = TRUE;	// ���α׷� ��ü�� ������ �÷���
char	g_zConfig[_MAX_PATH];



BOOL	DBOpen();
BOOL	LoadSymbol();
//MCAST	BOOL	InitMarketDataRecv();
BOOL	InitMemPool();
//MCAST	BOOL	InitPublish();
BOOL	InitSigSend();
BOOL	InitShm();

//MCAST	static unsigned WINAPI Thread_RecvMD(LPVOID lp);
static unsigned WINAPI Thread_SendSignal(LPVOID lp);
static unsigned WINAPI Thread_SaveSignal(LPVOID lp);



CRITICAL_SECTION	g_Console;
CLogMsg				g_log;
CDBPoolAdo			*g_pDBPool		= NULL;
//TODO CNanoPull			*g_pRecvMD		= NULL;
//MCAST CNanoPubSub			*g_pRecvMD = NULL;
//CNanoPush			*g_pSendSig2Ord = NULL;
CTcpClient			*g_pSendSig2Clnt = NULL;
CMemPool			*g_pMemPool		= NULL;
//MCAST CNanoPubSub			*g_pPublishMD	= NULL;
//TODO CQueueShm			*g_pQueueShm = NULL;

HANDLE				g_hRecvThread = NULL, g_hSaveThread = NULL, g_hSendThread = NULL;
unsigned int		g_unRecvThread = 0, g_unSaveThread = 0, g_unSendThread = 0;

std::map<std::string, CSignalMaker*>	g_mapSymbol;

#define SEND_TIMEOUT_MILI	500

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


	if (!InitMemPool()) {
		return 0;
	}

	if (!DBOpen())
		return 0;
	
	////MCAST
	//if (!InitPublish())
	//	return 0;

	//if (!InitMarketDataRecv())
	//	return 0;

	if (!InitSigSend())
		return 0;

	if (!InitShm())
		return 0;



	// Thread for saving singal data
	g_hSaveThread = (HANDLE)_beginthreadex(NULL, 0, &Thread_SaveSignal, NULL, 0, &g_unSaveThread);

	// Thread for sending signal data to client
	g_hSendThread = (HANDLE)_beginthreadex(NULL, 0, &Thread_SendSignal, NULL, 0, &g_unSendThread);


	if (!LoadSymbol())
		return 0;

	DWORD ret = WaitForSingleObject(g_hDieEvent, INFINITE);

	g_bContinue = FALSE;

	SAFE_DELETE(g_pDBPool);
	//MCAST SAFE_DELETE(g_pRecvMD);
	SAFE_CLOSEHANDLE(g_hRecvThread);
	SAFE_CLOSEHANDLE(g_hSaveThread);
	SAFE_CLOSEHANDLE(g_hSendThread);
	SAFE_DELETE(g_pMemPool);
	//MCAST SAFE_DELETE(g_pPublishMD);
	SAFE_DELETE(g_pSendSig2Clnt);
	//SAFE_DELETE(g_pSendSig2Ord);
	//TODO SAFE_DELETE(g_pQueueShm);

	CoUninitialize();

	return 0; 
}

BOOL InitShm()
{
	//TODO g_pQueueShm = new CQueueShm();
	return TRUE;
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

////MCAST 
//BOOL	InitPublish()
//{
//	g_pPublishMD = new CNanoPubSub(TP_PUB);
//	return g_pPublishMD->Begin(INNER_CHANNEL_PUBSUB);
//}


////MCAST 
//static unsigned WINAPI Thread_RecvMD(LPVOID lp)
//{
//	ST_PACK2CHART* pMD;
//	char zSymbol[128];
//
//	while (g_bContinue)
//	{
//		Sleep(1);
//		char* pBuf = g_pMemPool->get();
//		//TODO int nLen = g_pRecvMD->RecvData(pBuf);
//		int nLen = g_pRecvMD->Subscribe(pBuf);
//		if (nLen == 0) {
//			g_pMemPool->release(pBuf);
//			continue;
//		}
//		if (nLen < 0)
//		{
//			g_log.log(LOGTP_ERR, "RECV MarketData ERROR:%s", g_pRecvMD->GetMsg());
//			printf("RECV MarketData ERROR:%s", g_pRecvMD->GetMsg());
//			g_pMemPool->release(pBuf);
//			Sleep(1000);
//			continue;
//		}
//		//g_log.log(LOGTP_SUCC, "[RECV](%.80s)", pBuf);
//		pMD = (ST_PACK2CHART*)pBuf;
//		sprintf(zSymbol, "%.*s", sizeof(pMD->ShortCode), pMD->ShortCode);
//		CUtil::TrimAll(zSymbol, strlen(zSymbol));
//		std::string sSymbol = zSymbol;
//
//		std::map<std::string, CSignalMaker*>::iterator it = g_mapSymbol.find(sSymbol);
//		if (it == g_mapSymbol.end())
//		{
//			//g_log.log(LOGTP_ERR, "[%s] ������ ��û�� ������ .", sSymbol.c_str());
//			//printf("[%s] ������ ��û�� ������ �ƴϴ�\n", sSymbol.c_str());
//		}
//		else
//		{
//			g_pPublishMD->Publish(pBuf, nLen);
//			//g_log.log(LOGTP_SUCC, "[RECV](%.80s)", pBuf);
//		}
//	}
//
//	return 0;
//}


static unsigned WINAPI Thread_SaveSignal(LPVOID lp)
{
	char zQ[1024];
	//char temp[1024];
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
				else {
					g_log.log(LOGTP_SUCC, "[SIG SAVE](%s)", zQ);
				}

				g_pMemPool->release(pData);
			} //if (msg.message == WM_SEND_STRATEGY)
		} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	} // while (TRUE)

	return 0;
}

static unsigned WINAPI Thread_SendSignal(LPVOID lp)
{
	int nErrCode;
	int nSentLen;
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
				// send data
				nSentLen = g_pSendSig2Clnt->SendData(pData, nLen, &nErrCode);
				if (nSentLen< 0){
					g_log.log(LOGTP_FATAL, "Send Strategy to Client Failed(%s)", g_pSendSig2Clnt->GetMsg());
				}
				else if (nSentLen > 0) {
					g_log.log(LOGTP_SUCC, "[Send Client:%d,](%.*s)", nLen, nLen, pData);
				}


				// IRUMStrat_FB ���� ����
				//nSentLen = g_pSendSig2Ord->SendData(pData, nLen);
				//if (nErrCode < 0) {
				//	g_log.log(LOGTP_FATAL, "Send Strategy to Ord Failed(%s)", g_pSendSig2Clnt->GetMsg());
				//}
				//else if (nSentLen > 0) {
				//	//g_log.log(LOGTP_SUCC, "Send Strategy to Ord OK");
				//}

				g_pMemPool->release(pData);
			} //if (msg.message == WM_SEND_STRATEGY)
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
	Load Symbol info
*/
BOOL LoadSymbol()
{

	CDBHandlerAdo db(g_pDBPool->Get());
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

		//TODO
	/*	if (strncmp(zSymbol, "CLZ7", 4) != 0 
			) {
			db->Next();
			continue;
		}*/
		
		// KR �� CLQ7, �ٸ����� CLQ17 
		ir_cvtcode_HD_KR(zSymbol, zTemp);
		std::string symbol = zSymbol;


		CSignalMaker* p = new CSignalMaker(zSymbol, zArtc, g_pMemPool,/*g_pQueueShm,*/ g_unSaveThread, g_unSendThread);

		g_mapSymbol[symbol] = p;
		g_log.log(LOGTP_SUCC, "[%s][%s] ��Ʈ��������", zArtc, zSymbol);
		printf("[%s][%s] ��Ʈ��������\n", zArtc, zSymbol);

		db->Next();
	}
	db->Close();

	return TRUE;
}

////MCAST 
//BOOL	InitMarketDataRecv()
//{
//	// ChartApiKR �� ���� �ü� ���� (receive market data from ChartApiKR.exe)
//	char ip[32], port[32];
//	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "IP", ip);
//	CUtil::GetConfig(g_zConfig, "CHART_SOCKET_INFO", "PORT", port);
//
//	g_pRecvMD = new CNanoPubSub(TP_SUB);
//	if (!g_pRecvMD->BeginTcp(ip, port))
//	{
//		g_log.log(LOGTP_FATAL, "%s", g_pRecvMD->GetMsg());
//		return FALSE;
//	}
//
//
//	g_log.log(LOGTP_SUCC, "API�ü� SUBSCRIBE �ʱ�ȭ����(%s)", g_pRecvMD->GetMsg());
//	printf("%s\n",g_pRecvMD->GetMsg());
//
//	//// ���� ������ (thread for receiving)
//	g_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, &Thread_RecvMD, NULL, 0, &g_unRecvThread);
//
//
//	return TRUE;
//}
//
//

BOOL	InitSigSend()
{
	// SEND 2 CLIENT
	g_pSendSig2Clnt = new CTcpClient;

	char ip[32], port[32];
	CUtil::GetConfig(g_zConfig, "STRATEGY_RELAY_SOCKET_INFO", "IP", ip);
	CUtil::GetConfig(g_zConfig, "STRATEGY_RELAY_SOCKET_INFO", "PORT", port);
	if (!g_pSendSig2Clnt->Begin(ip, atoi(port), SEND_TIMEOUT_MILI))
	{
		g_log.log(LOGTP_FATAL, "[InitSigSend-ToClient] error:%s", g_pSendSig2Clnt->GetMsg());
		//return FALSE;
	}
	else
	{
		//Nagle Off
		g_pSendSig2Clnt->SetNagle(FALSE);

		g_log.log(LOGTP_SUCC, "CLIENT ���ۿ� connect ok(%s)",g_pSendSig2Clnt->GetMsg());
		printf("%s\n", g_pSendSig2Clnt->GetMsg());
	}


	// SEND 2 STRATEGY ORDER
	//g_pSendSig2Ord = new CNanoPush;

	//CUtil::GetConfig(g_zConfig, "STRATEGY_ORDER_SOCKET_INFO", "IP", ip);
	//CUtil::GetConfig(g_zConfig, "STRATEGY_ORDER_SOCKET_INFO", "PORT", port);
	//if (!g_pSendSig2Ord->Begin(ip, port))
	//{
	//	g_log.log(LOGTP_FATAL, "[InitSigSend-ToOrd]error : %s", g_pSendSig2Ord->GetMsg());
	//	//return FALSE;
	//}
	//else {
	//	g_log.log(LOGTP_SUCC, g_pSendSig2Ord->GetMsg());
	//	printf("%s\n", g_pSendSig2Ord->GetMsg());
	//}

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
