
// HDFCommClient.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "HDFCommClient.h"
#include "HDFCommClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHDFCommClientApp

BEGIN_MESSAGE_MAP(CHDFCommClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHDFCommClientApp 생성

CHDFCommClientApp::CHDFCommClientApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CHDFCommClientApp 개체입니다.

CHDFCommClientApp theApp;


// CHDFCommClientApp 초기화
BOOL CHDFCommClientApp::RegisterWindowClass()
{
	WNDCLASS wndcls;

	// Get the info for this class.
	// #32770 is the default class name for dialogs boxes.
	::GetClassInfo(AfxGetInstanceHandle(), "#32770", &wndcls);

	// Change the name of the class.
	wndcls.lpszClassName = CLASSNAME_APPINSTANCE;

	// Register this class so that MFC can use it.
	return AfxRegisterClass(&wndcls);
}

BOOL CHDFCommClientApp::InitInstance()
{
	if ( RegisterWindowClass() == FALSE )
	{
		AfxMessageBox("등록 실패!!!");
	}

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	
	// 챠트 OCX 파일 등록 처리
	TCHAR iniFileName[500];

	GetModuleFileName(NULL, iniFileName, MAX_PATH); 
	CString strFileName;
	CString strExeName = "HDFCommClient.exe";
	strFileName.Format("%s", iniFileName);
	int nDel = strExeName.GetLength();
	int nFind = strFileName.Find("HDFCommClient.exe");
	strFileName.Delete(nFind, nDel);
	strFileName = strFileName += "HDFCommAgent.ocx";
	RegisterOCX(strFileName);

	//afxAmbientActCtx = FALSE;

	CHDFCommClientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal(); 
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}

int CHDFCommClientApp::RegisterOCX(CString strFileName)
{
#ifdef LOGGING
	CommsDbgLog(0, "RegisterOCX() - start");
#endif
	int			iReturn=1;
	CString		szErrorMsg;

	strFileName.Replace("'\'", "\\");
	// Initialize OLE.
	if (FAILED(OleInitialize(NULL))) {
		AfxMessageBox("DLLRegister OleInitialize 실패");
		return 1;
	}

	SetErrorMode(SEM_FAILCRITICALERRORS);       // Make sure LoadLib fails.
	// Load the library.
	HINSTANCE hLib = LoadLibraryEx ( strFileName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
	if (hLib == NULL) {
		szErrorMsg.Format("File Name=%s, GetLastError() NO = 0x%08lx\n", strFileName, GetLastError());
		AfxMessageBox(szErrorMsg);
		iReturn = 0;
		goto CleanupOle;
	}

	HRESULT (STDAPICALLTYPE * lpDllEntryPoint)(void);
	// Find the entry point.
	(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, "DllRegisterServer");
	if (lpDllEntryPoint == NULL) {
		// 		TCHAR szExt[_MAX_EXT];
		// 		_tsplitpath(strFileName, NULL, NULL, NULL, szExt);

		TCHAR drive[255];
		TCHAR szExt[255];
		TCHAR path[MAX_PATH];
		TCHAR filename[MAX_PATH];
		_tsplitpath_s((LPTSTR)(LPCTSTR)strFileName, drive,_countof(drive), path, _countof(path),filename,_countof(filename), szExt, _countof(szExt) );

		if ((_stricmp(szExt, ".dll") != 0) && (_stricmp(szExt, ".ocx") != 0)) {
			szErrorMsg.Format("File Name=%s, GetProcAddress Fail\n", strFileName);
			AfxMessageBox(szErrorMsg);
		}

		iReturn = 0;
		goto CleanupLibrary;
	}

	// Call the entry point.
	if (FAILED((*lpDllEntryPoint)())) {
		szErrorMsg.Format("File Name=%s, lpDllEntryPoint Fail\n", strFileName);
		AfxMessageBox(szErrorMsg);
		iReturn = 0;
		goto CleanupLibrary;
	}
return iReturn;

CleanupLibrary:
	FreeLibrary(hLib);

CleanupOle:
	OleUninitialize();

#ifdef LOGGING
	CommsDbgLog(0, "RegisterOCX() - end");
#endif

	return iReturn;
}
