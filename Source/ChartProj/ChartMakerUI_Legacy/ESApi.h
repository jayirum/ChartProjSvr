//////////////////////////////////////////////////////////////////////
// ESApi.h: ESApi.dll 콜하기 위해 정의된 클래스
//
//////////////////////////////////////////////////////////////////////

#if !defined(__WAPPER_ESAPI_H__)
#define __WAPPER_ESAPI_H__

/*-----------------------------------------------------------------------------*/
//
// 해외 API
//
/*-----------------------------------------------------------------------------*/

typedef void (__cdecl *ESONLYFREECAP)(BOOL bValid);
typedef void (__cdecl *ESONLYSIGNALMAKER)(BOOL bValid);
typedef BOOL (__cdecl *ESISSERVERCONNECT)(HWND hWnd);
typedef void (__cdecl *ESDISCONNECTSERVER)(HWND hWnd);
typedef int  (__cdecl *ESESAPIFILEPATH)(LPCTSTR szApiPath);
typedef int  (__cdecl *ESCONNECTSERVER)(HWND hWnd, LPCTSTR szUserID, LPCTSTR szPass, LPCTSTR szCertPass, int nSvrKind);
typedef BOOL (__cdecl *ESGETFULLCODE)(LPSTR szFullCode, LPCTSTR szShortCode);
typedef BOOL (__cdecl *ESGETSHORTCODE)(LPSTR szShortCode, LPCTSTR szFull);
typedef BOOL (__cdecl *ESGETCODEINDEX)(LPSTR szCodeIndex, LPCTSTR szCode);
typedef void (__cdecl *ESGETCOMMUNICATIONTYPE)(LPSTR lpszData, int nLen);
typedef BOOL (__cdecl *ESGETENCODEPASSWORD)(LPSTR szDesPass, int nLen, LPCTSTR szAcct, LPCTSTR szSrcPass);
typedef int  (__cdecl *ESSENDTRDATA)(int nTrCode, LPSTR lpszData, int nLen);
typedef BOOL (__cdecl *ESSETAUTOUPDATE)(BOOL bSet, BOOL bAccount, LPSTR szAutoKey);

class CESApi
{
public:
	CESApi()
	{
		m_hWnd		= NULL;
		m_hModule	= NULL;
		m_szDllPath	= _T("");
	};

	~CESApi()
	{
		if (m_hModule != NULL) {
			FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
	};


protected:
	LPVOID GetMyProcAddress(LPCTSTR lpProcName)
	{
		if (m_hModule == NULL) {
			if (m_szDllPath.IsEmpty())
				return NULL;

			m_hModule = LoadLibrary(m_szDllPath);

			if (m_hModule == NULL)
				return NULL;
		}

		return (LPVOID)GetProcAddress(m_hModule, lpProcName);
	};

public:
	BOOL LoadDll(LPCTSTR szDllName)
	{
		if(m_hModule == NULL)
		{
			m_szDllPath	= szDllName;
			m_hModule	= LoadLibrary(m_szDllPath);

			if (m_hModule == NULL) {
				CString szMsg;
				szMsg.Format("%s 파일을 로드하는데 실패하였습니다.", m_szDllPath);
				::MessageBox(NULL, szMsg, "오류", MB_OK);
				return FALSE;
			}
		}

		return TRUE;
	};

	BOOL LoadDll(LPCTSTR szApiPath, LPCTSTR szDllName)
	{
		m_szDllPath.Format("%s/%s", szApiPath, szDllName);

		m_hModule	= LoadLibrary(m_szDllPath);

		if (m_hModule == NULL) {
			CString szMsg;
			szMsg.Format("%s 파일을 로드하는데 실패하였습니다.(%d)", m_szDllPath, GetLastError());
			::MessageBox(NULL, szMsg, "오류", MB_OK);
			return FALSE;
		}

		ESApiFilePath(szApiPath);

		return TRUE;
	};

	int ConnectServer(HWND hWnd, LPCTSTR szUserID, LPCTSTR szPass, LPCTSTR szCertPass, int nSvrKind)
	{
		ESCONNECTSERVER lpConnect = (ESCONNECTSERVER)GetMyProcAddress("ESConnectServer");

		if (lpConnect)
			return lpConnect(hWnd, szUserID, szPass, szCertPass, nSvrKind);
		else
			::MessageBox(hWnd, "Error Server Connect!!", "Error", MB_OK);

		return 0;
	};

	void OnlyFreeCap(BOOL bValid)
	{
		ESONLYFREECAP lpFreeCap = (ESONLYFREECAP)GetMyProcAddress("ESOnlyFreeCap");

		if (lpFreeCap)
			lpFreeCap(bValid);
	};

	void OnlySignalMaker(BOOL bValid)
	{
		ESONLYSIGNALMAKER lpSignalMaker = (ESONLYSIGNALMAKER)GetMyProcAddress("ESOnlySignalMaker");

		if (lpSignalMaker)
			lpSignalMaker(bValid);
	};

	BOOL IsServerConnect(HWND hWnd)
	{
		ESISSERVERCONNECT lpIsConnect = (ESISSERVERCONNECT)GetMyProcAddress("ESIsServerConnect");

		if (lpIsConnect)
			return lpIsConnect(hWnd);

		return FALSE;
	};

	void DisConnectServer(HWND hWnd)
	{
		ESDISCONNECTSERVER lpDisConnect = (ESDISCONNECTSERVER)GetMyProcAddress("ESDisConnectServer");

		if (lpDisConnect)
			lpDisConnect(hWnd);
	}

	int ESApiFilePath(LPCTSTR szApiPath)
	{
		ESESAPIFILEPATH lpApiPath = (ESESAPIFILEPATH)GetMyProcAddress("ESESApiFilePath");

		if (lpApiPath)
			return lpApiPath(szApiPath);

		return 0;
	}

	// HTS에서 사용 하는 정보 가져오기
	CString GetShortCode(LPCTSTR szFull)
	{
		CString			szRet;
		char			szBuff[5];
		ESGETSHORTCODE	lpShortCode = (ESGETSHORTCODE)GetMyProcAddress("ESGetShortCode");

		if (lpShortCode) {
			memset (szBuff, 0x20, sizeof(szBuff));
			if (lpShortCode(szBuff, szFull)) {
				szRet.Format("%.*s", sizeof(szBuff), szBuff);
				szRet.TrimRight();
			}
		}

		return szRet;
	}

	CString GetFullCode(LPCTSTR szShortCode)
	{
		CString			szRet;
		char			szBuff[32];
		ESGETFULLCODE	lpFullCode = (ESGETFULLCODE)GetMyProcAddress("ESGetFullCode");

		if (lpFullCode) {
			memset (szBuff, 0x20, sizeof(szBuff));
			if (lpFullCode(szBuff, szShortCode)) {
				szRet.Format("%.*s", sizeof(szBuff), szBuff);
				szRet.TrimRight();
			}
		}

		return szRet;
	}

	CString GetCodeIndex(LPCTSTR szCode)
	{
		CString			szRet;
		char			szBuff[4];
		ESGETCODEINDEX	lpCodeIndex = (ESGETCODEINDEX)GetMyProcAddress("ESGetCodeIndex");

		if (lpCodeIndex) {
			memset (szBuff, 0x20, sizeof(szBuff));
			if (lpCodeIndex(szBuff, szCode)) {
				szRet.Format("%.*s", sizeof(szBuff), szBuff);
				szRet.TrimRight();
			}
		}

		return szRet;
	}
	
	void GetCommunicationType(LPSTR lpszData, int nLen)
	{
		ESGETCOMMUNICATIONTYPE lpCommunication = (ESGETCOMMUNICATIONTYPE)GetMyProcAddress("ESGetCommunicationType");

		if (lpCommunication)
			lpCommunication(lpszData, nLen);
	}

	BOOL GetEncodePassword(LPSTR szDesPass, int nLen, LPCTSTR szAcct, LPCTSTR szSrcPass)
	{
		ESGETENCODEPASSWORD lpEncode = (ESGETENCODEPASSWORD)GetMyProcAddress("ESGetEncodePassword");

		if (lpEncode)
			return lpEncode(szDesPass, nLen, szAcct, szSrcPass);

		return FALSE;
	}

	// 데이터 전송 관련
	int SendTrData(int nTrCode, LPSTR lpszData, int nLen)
	{
		ESSENDTRDATA lpSend = (ESSENDTRDATA)GetMyProcAddress("ESSendTrData");

		if (lpSend)
			return lpSend(nTrCode, lpszData, nLen);

		return FALSE;
	}

	int SetAutoUpdate(BOOL bSet, BOOL bAccount, LPSTR szAutoKey)
	{
		ESSETAUTOUPDATE lpAuto = (ESSETAUTOUPDATE)GetMyProcAddress("ESSetAutoUpdate");

		if (lpAuto)
			return lpAuto(bSet, bAccount, szAutoKey);

		return FALSE;
	}

protected:
	HWND	m_hWnd;
	HMODULE	m_hModule;

	CString	m_szDllPath;
};

#endif	//__WAPPER_ESAPI_H__
