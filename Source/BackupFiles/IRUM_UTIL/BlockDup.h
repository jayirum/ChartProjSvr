#pragma once
#pragma warning(disable:4996)
#include <stdio.h>
#include <Windows.h>

/*
	중복실행방지를 위해
*/
class CBlockDup
{
public:
	CBlockDup()
	{
		m_hMutex = NULL;
	}
	virtual ~CBlockDup()
	{
		Close();
	}
	VOID Close()
	{
		if(m_hMutex)	CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

	

	INT  CheckDuplicate(char* zProcessName) {
		sprintf(m_zMutexNm, "Global\\MUTEX_%s", zProcessName);
		m_hMutex = CreateMutex(NULL, FALSE, m_zMutexNm);
		if (m_hMutex == NULL)
		{
			SetErrMsg(GetLastError(), "Mutex Open Error : ");
			Close();
			return 0;
		}
		if (GetLastError() == ERROR_ALREADY_EXISTS)
			return -1;

		return 1;
	}
	void SetErrMsg(int nErrCode, char* msg)
	{
		sprintf_s(m_zMsg, msg);
		LPVOID lpMsgBuf = NULL;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			nErrCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
		);

		strcat_s(m_zMsg, (char*)lpMsgBuf);

		LocalFree(lpMsgBuf);
	};
private:
	char	m_zMutexNm[128];
	HANDLE	m_hMutex;
	char	m_zMsg[1024];
};