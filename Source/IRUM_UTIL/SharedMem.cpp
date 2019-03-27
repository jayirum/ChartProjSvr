// SharedMem.cpp: implementation of the CSharedMem class.
//
//////////////////////////////////////////////////////////////////////

#include "SharedMem.h"
#include <search.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSharedMem::CSharedMem()
{
	m_pShm = NULL;
	m_hShm = NULL;
	m_hMutex = NULL;
	m_pSD = NULL;

	m_pGroup = NULL;
	m_pHeader = NULL;
	m_pStruct = NULL;

	m_lHeaderSize = 0;
	m_lGroupKeySize = 0;
	m_lGroupIndexSize = 0;
	m_lStructSize = 0;
	m_lMaxGroupCnt = 0;
	m_lStructKeySize = 0;
	m_lMaxStructCnt = 0;
	m_bCompareGroup = TRUE;
	m_bAscGroup = TRUE;
	m_bAscData = TRUE;
	m_pFindGroup = NULL;
	m_pFirstStruct = NULL;
	m_pFindData = NULL;

	ZeroMemory(m_szErr, sizeof(m_szErr));
}

CSharedMem::~CSharedMem()
{
	Close();
}

int CSharedMem::Create(HANDLE hFile, DWORD dwSize, LPCTSTR lpShmName, LPCTSTR lpMutexName)
{
	m_pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if(m_pSD == NULL)
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}
	if(!InitializeSecurityDescriptor(m_pSD, SECURITY_DESCRIPTOR_REVISION))
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}
	if(!SetSecurityDescriptorDacl(m_pSD, TRUE, (PACL) NULL, FALSE))
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = m_pSD;
	sa.bInheritHandle = TRUE;

	m_hShm = CreateFileMapping(hFile, &sa, PAGE_READWRITE, 0, dwSize, lpShmName);
	if(m_hShm == NULL)
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}

	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return 0;
	}

	m_pShm = (char*)MapViewOfFile(m_hShm,FILE_MAP_ALL_ACCESS,0,0,0);
	if(m_pShm == NULL)
	{
		SetErrMsg(GetLastError(), "MapViewOfFile Error : ");
		Close();
		return -1;
	}

	ZeroMemory(m_pShm, INIT_DATA_SIZE);

	m_hMutex = CreateMutex(&sa, FALSE, lpMutexName);
	if (m_hMutex == NULL )
	{
		SetErrMsg(GetLastError(), "Mutex Open Error : ");
		Close();
		return -1;
	}

	return 1;
}


int CSharedMem::Create(HANDLE hFile, INIT_DATA *shmData, LPCTSTR lpShmName, LPCTSTR lpMutexName)
{
	int nIndexSize	= ( shmData->lGroupKeySize + GROUP_POINTER_SIZE ) * shmData->lMaxGroupCnt;
	int nDataSize	= shmData->lMaxGroupCnt * (INIT_STRUCT_DATA_SIZE + (shmData->lStructSize * shmData->lMaxStructCnt)  );
	int nTotSize	= INIT_DATA_SIZE + nIndexSize + nDataSize + shmData->lHeaderSize;

	m_pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if(m_pSD == NULL)
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}
	if(!InitializeSecurityDescriptor(m_pSD, SECURITY_DESCRIPTOR_REVISION))
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}
	if(!SetSecurityDescriptorDacl(m_pSD, TRUE, (PACL) NULL, FALSE))
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}
	
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = m_pSD;
	sa.bInheritHandle = TRUE;
	
	m_hShm = CreateFileMapping(hFile, &sa, PAGE_READWRITE, 0, nTotSize, lpShmName);
	if(m_hShm == NULL)
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}
	
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return 0;
	}
	
	m_pShm = (char*)MapViewOfFile(m_hShm,FILE_MAP_ALL_ACCESS,0,0,0);
	if(m_pShm == NULL)
	{
		SetErrMsg(GetLastError(), "MapViewOfFile Error : ");
		Close();
		return -1;
	}
	
	ZeroMemory(m_pShm, INIT_DATA_SIZE);
	
	m_hMutex = CreateMutex(&sa, FALSE, lpMutexName);
	if (m_hMutex == NULL )
	{
		SetErrMsg(GetLastError(), "Mutex Open Error : ");
		Close();
		return -1;
	}
	
	return 1;
}


int CSharedMem::CreateEx(HANDLE hFile, INIT_DATA *shmData, LPCTSTR lpShmName, LPCTSTR lpMutexName)
{
	int nIndexSize = (shmData->lGroupKeySize + GROUP_POINTER_SIZE) * shmData->lMaxGroupCnt;
	int nDataSize = shmData->lMaxGroupCnt * (INIT_STRUCT_DATA_SIZE + (shmData->lStructSize * shmData->lMaxStructCnt));
	int nTotSize = INIT_DATA_SIZE + nIndexSize + nDataSize + shmData->lHeaderSize;

	m_pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (m_pSD == NULL)
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}
	if (!InitializeSecurityDescriptor(m_pSD, SECURITY_DESCRIPTOR_REVISION))
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}
	if (!SetSecurityDescriptorDacl(m_pSD, TRUE, (PACL)NULL, FALSE))
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = m_pSD;
	sa.bInheritHandle = TRUE;

	m_hShm = CreateFileMapping(hFile, &sa, PAGE_READWRITE, 0, nTotSize, lpShmName);
	if (m_hShm == NULL)
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return -1;
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		SetErrMsg(GetLastError(), "Create SharedMem Error : ");
		Close();
		return 0;
	}

	m_pShm = (char*)MapViewOfFile(m_hShm, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (m_pShm == NULL)
	{
		SetErrMsg(GetLastError(), "MapViewOfFile Error : ");
		Close();
		return -1;
	}

	ZeroMemory(m_pShm, INIT_DATA_SIZE);

	m_hMutex = CreateMutex(&sa, FALSE, lpMutexName);
	if (m_hMutex == NULL)
	{
		SetErrMsg(GetLastError(), "Mutex Open Error : ");
		Close();
		return -1;
	}

	if (!SetInitSizeEx(shmData))
		return -1;

	if (!ClearData(TRUE, TRUE, TRUE))
		return -1;

	return nTotSize;
}



BOOL CSharedMem::Open(LPCTSTR lpShmName, LPCTSTR lpMutexName)
{
	m_hShm = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, lpShmName);
	if (m_hShm == NULL ) 
	{
		SetErrMsg(GetLastError(), "Open SharedMem Error : ");
		Close();
		return FALSE;
	}

	m_pShm = (char*)MapViewOfFile(m_hShm,FILE_MAP_ALL_ACCESS,0,0,0);
	if(m_pShm == NULL)
	{
		SetErrMsg(GetLastError(), "MapViewOfFile Error : ");
		Close();
		return FALSE;
	}

	INIT_DATA initData;
	CopyMemory((void*)&initData, m_pShm, INIT_DATA_SIZE);
	m_lMaxGroupCnt = initData.lMaxGroupCnt;
	m_lGroupKeySize = initData.lGroupKeySize;
	m_lGroupIndexSize = m_lGroupKeySize + GROUP_POINTER_SIZE;
	m_lHeaderSize = initData.lHeaderSize;
	m_lStructSize = initData.lStructSize;
	m_lStructKeySize = initData.lStructKeySize;
	m_lMaxStructCnt = initData.lMaxStructCnt;
	
	if(m_lMaxGroupCnt <= 0)
		m_pGroup = NULL;
	else
		m_pGroup = m_pShm + INIT_DATA_SIZE;

	if(m_lHeaderSize <= 0)
		m_pHeader = NULL;
	else
		m_pHeader = m_pShm + INIT_DATA_SIZE + (m_lGroupIndexSize * m_lMaxGroupCnt);

	if(m_lMaxStructCnt <= 0)
		m_pStruct = NULL;
	else
		m_pStruct = m_pShm + INIT_DATA_SIZE + (m_lGroupIndexSize * m_lMaxGroupCnt) + m_lHeaderSize;

	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, lpMutexName);
	if (m_hMutex == NULL )
	{
		SetErrMsg(GetLastError(), "Mutex Open Error : ");
		Close();
		return FALSE;
	}

	return TRUE;
}

void CSharedMem::Close()
{
	if(m_pShm)
	{
//		if (!FlushViewOfFile(m_pShm, 0))
//			SetErrMsg(GetLastError(), "FlushViewOfFile Failed: ");

		for(int i=0; i < 3; i++)
		{
			if(UnmapViewOfFile(m_pShm))
				break;

//			SetErrMsg(GetLastError(), "UnmapviewOfFile Failed: ");
			Sleep(10);
		}
	}

	if(m_hShm) CloseHandle(m_hShm);
	if(m_hMutex) CloseHandle(m_hMutex);
	if(m_pSD) LocalFree((HLOCAL) m_pSD);
	m_hShm = NULL;
	m_pShm = NULL;
	m_hMutex = NULL;
	m_pSD = NULL;
}

void CSharedMem::Lock()
{
	DWORD dw = WaitForSingleObject(m_hMutex, INFINITE);
	if( dw == WAIT_ABANDONED ){
		ReleaseMutex(m_hMutex);
		dw = WaitForSingleObject(m_hMutex, INFINITE);
	}
}

void CSharedMem::Unlock()
{
	ReleaseMutex(m_hMutex);
}

BOOL CSharedMem::SetInitSize(long lMaxGroupCnt, long lGroupKeySize, long lHeaderSize, long lStructSize, long lStructKeySize, long lMaxStructCnt)
{
	if(!m_pShm)	return false;

	INIT_DATA initData;
	CopyMemory((void*)&initData, m_pShm, INIT_DATA_SIZE);
	initData.lMaxGroupCnt = lMaxGroupCnt;
	initData.lGroupKeySize = lGroupKeySize;
	initData.lHeaderSize = lHeaderSize;
	initData.lStructSize = lStructSize;
	initData.lStructKeySize = lStructKeySize;
	initData.lMaxStructCnt = lMaxStructCnt;
	CopyMemory(m_pShm, (void*)&initData, INIT_DATA_SIZE);

	m_lMaxGroupCnt = lMaxGroupCnt;
	m_lGroupKeySize = lGroupKeySize;
	m_lGroupIndexSize = m_lGroupKeySize + GROUP_POINTER_SIZE;
	m_lHeaderSize = lHeaderSize;
	m_lStructSize = lStructSize;
	m_lStructKeySize = lStructKeySize;
	m_lMaxStructCnt = lMaxStructCnt;

	if(m_lMaxGroupCnt <= 0)
		m_pGroup = NULL;
	else
		m_pGroup = m_pShm + INIT_DATA_SIZE;

	if(m_lHeaderSize <= 0)
		m_pHeader = NULL;
	else
		m_pHeader = m_pShm + INIT_DATA_SIZE + (m_lGroupIndexSize * m_lMaxGroupCnt);

	if(m_lMaxStructCnt <= 0)
		m_pStruct = NULL;
	else
		m_pStruct = m_pShm + INIT_DATA_SIZE + (m_lGroupIndexSize * m_lMaxGroupCnt) + m_lHeaderSize;

	return TRUE;
}



BOOL CSharedMem::SetInitSizeEx(INIT_DATA* pInitData)
{
	if (!m_pShm)	return false;

	INIT_DATA initData;
	CopyMemory((void*)&initData, m_pShm, INIT_DATA_SIZE);

	initData.lMaxGroupCnt = pInitData->lMaxGroupCnt;
	initData.lGroupKeySize = pInitData->lGroupKeySize;
	initData.lHeaderSize = pInitData->lHeaderSize;
	initData.lStructSize = pInitData->lStructSize;
	initData.lStructKeySize = pInitData->lStructKeySize;
	initData.lMaxStructCnt = pInitData->lMaxStructCnt;
	CopyMemory(m_pShm, (void*)&initData, INIT_DATA_SIZE);

	m_lMaxGroupCnt = pInitData->lMaxGroupCnt;
	m_lGroupKeySize = pInitData->lGroupKeySize;
	m_lGroupIndexSize = m_lGroupKeySize + GROUP_POINTER_SIZE;
	m_lHeaderSize = pInitData->lHeaderSize;
	m_lStructSize = pInitData->lStructSize;
	m_lStructKeySize = pInitData->lStructKeySize;
	m_lMaxStructCnt = pInitData->lMaxStructCnt;

	if (m_lMaxGroupCnt <= 0)
		m_pGroup = NULL;
	else
		m_pGroup = m_pShm + INIT_DATA_SIZE;

	if (m_lHeaderSize <= 0)
		m_pHeader = NULL;
	else
		m_pHeader = m_pShm + INIT_DATA_SIZE + (m_lGroupIndexSize * m_lMaxGroupCnt);

	if (m_lMaxStructCnt <= 0)
		m_pStruct = NULL;
	else
		m_pStruct = m_pShm + INIT_DATA_SIZE + (m_lGroupIndexSize * m_lMaxGroupCnt) + m_lHeaderSize;

	return TRUE;
}

BOOL CSharedMem::GetData(char *pGroupKey, char *pStructKey, char *pStructData)
{
	if(!m_pShm) return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) 
		return FALSE;

	char *pData = SearchDataPointer(pStruct, pStructKey);
	if(!pData) 
		return FALSE;

	CopyMemory(pStructData, pData, m_lStructSize);

	return TRUE;
}

BOOL CSharedMem::DataGet(char *pGroupKey, char *pStructKey, /*out*/char *pStructData)
{
	int nErrCode;
	char pErrMsg[128];
	return DataGet(pGroupKey, pStructKey, pStructData, &nErrCode, pErrMsg);
}

BOOL CSharedMem::DataGet(char *pGroupKey, char *pStructKey, /*out*/char *pStructData, int *pErrCode, char* pErrMsg)
{
	if (!m_pShm) {
		*pErrCode = 1;
		sprintf(pErrMsg, "SHM is not set up");
		return FALSE;
	}

	char *pStruct = SearchStructPointer(pGroupKey);
	if (!pStruct) {
		*pErrCode = 2;
		sprintf(pErrMsg, "SearchStructPointer Error(GROUP_KEY:%s)", pGroupKey);
		return FALSE;
	}

	char *pData = SearchDataPointer(pStruct, pStructKey);
	if (!pData) {
		*pErrCode = 3;
		sprintf(pErrMsg, "SearchDataPointer Error(STRUCT_KEY:%s)", pStructKey);
		return FALSE;
	}

	CopyMemory(pStructData, pData, m_lStructSize);

	return TRUE;
}


BOOL CSharedMem::GetData(char *pGroupKey, char *pStructKey, /*in*/int nStartPos, /*in*/int nLen, /*out*/char *pStructData)
{
	if(!m_pShm) return FALSE;

	if(m_lStructSize < nLen + nStartPos)
		return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return FALSE;

	char *pData = SearchDataPointer(pStruct, pStructKey);
	if(!pData) return FALSE;

	CopyMemory(pStructData, pData + nStartPos, nLen);

	return TRUE;
}

int CSharedMem::GetDataArray(char *pGroupKey, BOOL bAll, int nCnt, char *pData)
{
	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return FALSE;

	long lRetCnt = *(long*)pStruct;

	if(!bAll && lRetCnt > nCnt)	lRetCnt = nCnt;

	CopyMemory(pData, pStruct+INIT_STRUCT_DATA_SIZE, m_lStructSize * lRetCnt);

	return lRetCnt;
}

BOOL CSharedMem::UpdateData(char *pGroupKey, char *pStructKey, char *pStructData)
{
	return SetData(pGroupKey, pStructKey, pStructData);
}
BOOL CSharedMem::SetData(char *pGroupKey, char *pStructKey, char *pStructData)
{
	if(!m_pShm) return FALSE;
	
	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return FALSE;
	
	char *pData = SearchDataPointer(pStruct, pStructKey);
	if(!pData) return FALSE;

	CopyMemory(pData, pStructData, m_lStructSize);

	return TRUE;
}

BOOL CSharedMem::UpdateData(char *pGroupKey, char *pStructKey, 
							/*in*/int nStartPos, /*in*/int nLen, char *pStructData, BOOL bStructSort)
{
	return SetData(pGroupKey, pStructKey, nStartPos, nLen, pStructData, bStructSort);
}
BOOL CSharedMem::SetData(char *pGroupKey, char *pStructKey, 
						 /*in*/int nStartPos, /*in*/int nLen, char *pStructData, BOOL bStructSort)
{
	if(!m_pShm) return FALSE;

	if(m_lStructSize < nLen + nStartPos)
		return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return FALSE;

	long lCurrStructCnt = *(long*)pStruct;	// 2004-09-19

	char *pData = SearchDataPointer(pStruct, pStructKey);
	if(!pData) return FALSE;

	CopyMemory(pData + nStartPos, pStructData, nLen);

	if(bStructSort && lCurrStructCnt > 1)
	{
		m_bCompareGroup = FALSE;
		Qsort(pStruct+INIT_STRUCT_DATA_SIZE, lCurrStructCnt, m_lStructSize);
	}

	return TRUE;
}

BOOL CSharedMem::InsertData(char *pGroupKey, char *pStructData, BOOL bStructSort)
{
	return AddData(pGroupKey, pStructData, bStructSort);
}
BOOL CSharedMem::AddData(char *pGroupKey, char *pStructData, BOOL bStructSort)
{
	if(!m_pShm) return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) 
		return FALSE;

	long lCurrStructCnt = *(long*)pStruct;
	if(lCurrStructCnt >= m_lMaxStructCnt) 
		return FALSE;

	char *pAddData = (char*)(pStruct + INIT_STRUCT_DATA_SIZE + (m_lStructSize * lCurrStructCnt));
	CopyMemory(pAddData, pStructData, m_lStructSize);

	*(long*)pStruct = ++lCurrStructCnt;

	if(bStructSort && lCurrStructCnt > 1)
	{
		m_bCompareGroup = FALSE;
		Qsort(pStruct+INIT_STRUCT_DATA_SIZE, lCurrStructCnt, m_lStructSize);
	}

	return TRUE;
}

// 가장 최근 데이터 반환
int CSharedMem::GetCurrData(char *pGroupKey,/*out*/char *pStructData)
{
	if (!m_pShm) return -1;

	char *pStruct = SearchStructPointer(pGroupKey);
	if (!pStruct)
		return -2;

	long lCurrStructCnt = *(long*)pStruct;
	if (lCurrStructCnt == 0)
		return -3;

	char *pCurrData = (char*)(pStruct + INIT_STRUCT_DATA_SIZE + (m_lStructSize * lCurrStructCnt-1));
	CopyMemory(pStructData, pCurrData, m_lStructSize);
	return 1;
}

BOOL CSharedMem::InsertDataArray(char *pGroupKey, int nCnt, char *pStructData, BOOL bStructSort)
{
	return AddDataArray(pGroupKey, nCnt, pStructData, bStructSort);
}

BOOL CSharedMem::AddDataArray(char *pGroupKey, int nCnt, char *pStructData, BOOL bStructSort)
{
	if(!m_pShm) return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return FALSE;

	long lCurrStructCnt = *(long*)pStruct;
	char *pAddData = (char*)(pStruct + INIT_STRUCT_DATA_SIZE + (m_lStructSize * lCurrStructCnt));

	long lAddCnt = 0;
	if(nCnt + lCurrStructCnt > m_lMaxStructCnt)	lAddCnt = m_lMaxStructCnt - lCurrStructCnt;
	else										lAddCnt = nCnt;

	CopyMemory(pAddData, pStructData, m_lStructSize*lAddCnt);

	lCurrStructCnt += lAddCnt;
	*(long*)pStruct = lCurrStructCnt;

	if(bStructSort && lCurrStructCnt > 1)
	{
		m_bCompareGroup = FALSE;
		Qsort(pStruct+INIT_STRUCT_DATA_SIZE, lCurrStructCnt, m_lStructSize);
	}

	return TRUE;
}

BOOL CSharedMem::DelData(char *pGroupKey, char *pStructKey, BOOL bStructSort)
{
	if(!m_pShm) return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) 
		return FALSE;

	char *pData = SearchDataPointer(pStruct, pStructKey);
	if(!pData) 
		return FALSE;

	long lCurrStructCnt = *(long*)pStruct;
	char *pStart = pData + m_lStructSize; 
	char *pLast = pStruct + INIT_STRUCT_DATA_SIZE + (m_lStructSize * lCurrStructCnt);
	MoveMemory(pData, pStart, pLast - pStart);
	ZeroMemory(pLast - m_lStructSize, m_lStructSize);

	*(long*)pStruct = --lCurrStructCnt;

	if(bStructSort)
	{
		m_bCompareGroup = FALSE;
		Qsort(pStruct+INIT_STRUCT_DATA_SIZE, lCurrStructCnt, m_lStructSize);
	}

	if(m_pFindData)
	{
		if(m_bAscData)
		{
			m_pFindData -= m_lStructSize;
			if(m_pFindData < pStart) m_pFindData = NULL;
		}
		else
		{
			pLast -= m_lStructSize;
			if(m_pFindData >= pLast) m_pFindData = NULL;
		}
	}

	return TRUE;
}

BOOL CSharedMem::DelDataArray(char *pGroupKey, BOOL bAll, int nCnt, BOOL bStructSort)
{
	if(!m_pShm) return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return FALSE;

	long lCurrStructCnt = *(long*)pStruct;
	long lDelCnt = 0;

	if(!bAll && lCurrStructCnt > nCnt)	lDelCnt = nCnt;
	else								lDelCnt = lCurrStructCnt;

	char *pStart = pStruct + INIT_STRUCT_DATA_SIZE + (m_lStructSize * lDelCnt);
	long lCopySize = (lCurrStructCnt-lDelCnt) * m_lStructSize;
	MoveMemory(pStruct + INIT_STRUCT_DATA_SIZE, pStart, lCopySize);
	ZeroMemory(pStruct + INIT_STRUCT_DATA_SIZE + lCopySize, m_lStructSize * lDelCnt);

	lCurrStructCnt -= lDelCnt;
	*(long*)pStruct = lCurrStructCnt;

	if(bStructSort && lCurrStructCnt > 1)
	{
		m_bCompareGroup = FALSE;
		Qsort(pStruct+INIT_STRUCT_DATA_SIZE, lCurrStructCnt, m_lStructSize);
	}

	return TRUE;
}

BOOL CSharedMem::SortData(char *pGroupKey)
{
	if(!m_pShm) return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return FALSE;

	long lCurrStructCnt = *(long*)pStruct;

	m_bCompareGroup = FALSE;
	Qsort(pStruct+INIT_STRUCT_DATA_SIZE, lCurrStructCnt, m_lStructSize);

	return TRUE;
}

BOOL CSharedMem::InsertGroup(char *pGroupKey, BOOL bGroupSort)
{
	return AddGroup(pGroupKey, bGroupSort);
}
BOOL CSharedMem::AddGroup(char *pGroupKey, BOOL bGroupSort)
{
	if(!m_pShm || !m_pGroup || !m_pStruct) return FALSE;

	long lCurrGroupCnt = *(long*)m_pShm;
	if(lCurrGroupCnt >= m_lMaxGroupCnt){
		return FALSE;
	}

	char *pAddGroup = m_pGroup + (m_lGroupIndexSize * lCurrGroupCnt);
	CopyMemory(pAddGroup, pGroupKey, m_lGroupKeySize);

	char *pAddStruct = m_pStruct + ((m_lStructSize * m_lMaxStructCnt + INIT_STRUCT_DATA_SIZE) * lCurrGroupCnt);
	char **ppStruct = (char**)(pAddGroup + m_lGroupKeySize);
	*ppStruct = (char*)((long)pAddStruct - (long)m_pShm);	// 공유 메모리의 Start Pointer로부터 Offset 계산

	*(long*)pAddStruct = 0;
	*(long*)m_pShm = ++lCurrGroupCnt;

	if(bGroupSort && lCurrGroupCnt > 1)
	{
		m_bCompareGroup = TRUE;
		Qsort(m_pGroup, lCurrGroupCnt, m_lGroupIndexSize);
	}

	return TRUE;
}

BOOL CSharedMem::DelGroup(char *pGroupKey, BOOL bGroupSort)
{
	if(!m_pShm || !m_pGroup || !m_pStruct) return FALSE;

	char *pDelGroup = SearchGroupPointer(pGroupKey);
	if(!pDelGroup) return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return FALSE;

	ZeroMemory(pStruct + INIT_STRUCT_DATA_SIZE, m_lStructSize * m_lMaxStructCnt);
	*(long*)pStruct = 0;

	long lCurrGroupCnt = *(long*)m_pShm;
	char *pStart = pDelGroup + m_lGroupIndexSize; 
	char *pLast = m_pGroup + (m_lGroupIndexSize * lCurrGroupCnt);
	MoveMemory(pDelGroup, pStart, pLast - pStart);
	ZeroMemory(pLast - m_lGroupIndexSize, m_lGroupIndexSize);

	*(long*)m_pShm = --lCurrGroupCnt;

	if(bGroupSort && lCurrGroupCnt > 1)
	{
		m_bCompareGroup = TRUE;
		Qsort(m_pGroup, lCurrGroupCnt, m_lGroupIndexSize);
	}

	if(m_pFindGroup && lCurrGroupCnt > 0)
	{
		if(m_bAscGroup)
		{
			m_pFindGroup -= m_lGroupIndexSize;
			if(m_pFindGroup < pStart) m_pFindGroup = NULL;
		}
		else
		{
			pLast -= m_lGroupIndexSize;
			if(m_pFindGroup >= pLast) m_pFindGroup = NULL;
		}
	}

	return TRUE;
}

BOOL CSharedMem::SortGroup()
{
	if(!m_pShm || !m_pGroup) return FALSE;

	long lCurrGroupCnt = *(long*)m_pShm;

	m_bCompareGroup = TRUE;
	Qsort(m_pGroup, lCurrGroupCnt, m_lGroupIndexSize);

	return TRUE;
}

BOOL CSharedMem::FindGroup(char *pGroupKey)
{
	m_pFindGroup = NULL;

	if(!m_pShm || !pGroupKey) return FALSE;

	char *pGroup = SearchGroupPointer(pGroupKey);
	if(!pGroup) return FALSE;

	m_bAscGroup = TRUE;

	return TRUE;
}

BOOL CSharedMem::FindGroupReverse(char *pGroupKey)
{
	BOOL bRet = FindGroup(pGroupKey);

	m_bAscGroup = FALSE;

	return bRet;
}

int CSharedMem::FindFirstGroup()
{
	m_pFindGroup = NULL;

	if(!m_pShm) return -1;

	long lCurrGroupCnt = *(long*)m_pShm;
	if(lCurrGroupCnt < 1)
		return 0;

	m_bAscGroup = TRUE;

	return 1;
}

int CSharedMem::FindLastGroup()
{
	int nRet = FindFirstGroup();

	m_bAscGroup = FALSE;

	return nRet;
}

BOOL CSharedMem::GetNextGroup(/*out*/char *pGroupKey)
{
	long lCurrGroupCnt = *(long*)m_pShm;
	char *pStart = m_pGroup; 
	char *pLast = pStart + (m_lGroupIndexSize * lCurrGroupCnt);

	if(m_bAscGroup)
	{
		if(m_pFindGroup == NULL)
			m_pFindGroup = pStart;
		else
			m_pFindGroup += m_lGroupIndexSize;

		if(m_pFindGroup >= pLast)
		{
			m_pFindGroup = NULL;
			*pGroupKey = 0x00;
			return FALSE;
		}

		CopyMemory(pGroupKey, m_pFindGroup, m_lGroupKeySize);

		if(m_pFindGroup + m_lGroupIndexSize >= pLast)
			return FALSE;
	}
	else
	{
		if(m_pFindGroup == NULL)
			m_pFindGroup = pLast - m_lGroupIndexSize;
		else
			m_pFindGroup -= m_lGroupIndexSize;

		if(m_pFindGroup < pStart)
		{
			m_pFindGroup = NULL;
			*pGroupKey = 0x00;
			return FALSE;
		}

		CopyMemory(pGroupKey, m_pFindGroup, m_lGroupKeySize);

		if(m_pFindGroup - m_lGroupIndexSize < pStart)
			return FALSE;
	}

	return TRUE;
}

//BOOL CSharedMem::FindData(char *pGroupKey, char *pStructKey)
int CSharedMem::FindData(char *pGroupKey, char *pStructKey)
{
	m_pFirstStruct = NULL;
	m_pFindData = NULL;

	if(!m_pShm || !pGroupKey || !pStructKey) return -1;//FALSE;

	m_pFirstStruct = SearchStructPointer(pGroupKey);
	if(!m_pFirstStruct) return -2;//FALSE;

	char *pData = SearchDataPointer(m_pFirstStruct, pStructKey);
	if(!pData)
	{
		m_pFirstStruct = NULL;
		return -3;//FALSE;
	}

	m_bAscData = TRUE;

	return 1;
}

int CSharedMem::FindDataReverse(char *pGroupKey, char *pStructKey)
{
	int nRet = FindData(pGroupKey, pStructKey);

	m_bAscData = FALSE;

	return nRet;TRUE;
}

int CSharedMem::FindFirstData(char *pGroupKey)
{
	m_pFirstStruct = NULL;
	m_pFindData = NULL;

	if(!m_pShm || !pGroupKey) return -1;

	m_pFirstStruct = SearchStructPointer(pGroupKey);
	if(!m_pFirstStruct) return -1;

	if( *(long*)m_pFirstStruct < 1)
		return 0;

	m_bAscData = TRUE;

	return 1;
}

int CSharedMem::FindLastData(char *pGroupKey)
{
	int nRet = FindFirstData(pGroupKey);

	m_bAscData = FALSE;

	return nRet;
}

/*
	다음에 데이터가 없으면 return FALSE;
*/
BOOL CSharedMem::GetNextData(/*out*/char *pStructData)
{
	if(!m_pFirstStruct)
	{
		m_pFindData = NULL;
		*pStructData = 0x00;
		return FALSE;
	}

	long lCurrStructCnt = *(long*)m_pFirstStruct;
	char *pStart = m_pFirstStruct + INIT_STRUCT_DATA_SIZE;
	char *pLast =  pStart + (m_lStructSize * lCurrStructCnt);

	if(m_bAscData)
	{
		if(m_pFindData == NULL)
			m_pFindData = pStart;
		else
			m_pFindData += m_lStructSize;
		
		if(m_pFindData >= pLast)
		{
			m_pFirstStruct = NULL;
			m_pFindData = NULL;
			*pStructData = 0x00;
			return FALSE;
		}
		
		CopyMemory(pStructData, m_pFindData, m_lStructSize);
		
		if(m_pFindData + m_lStructSize >= pLast)
			return FALSE;
	}
	else
	{
		if(m_pFindData == NULL)
			m_pFindData = pLast - m_lStructSize;
		else
			m_pFindData -= m_lStructSize;
		
		if(m_pFindData < pStart)
		{
			m_pFirstStruct = NULL;
			m_pFindData = NULL;
			*pStructData = 0x00;
			return FALSE;
		}
		
		CopyMemory(pStructData, m_pFindData, m_lStructSize);
		
		if(m_pFindData - m_lStructSize < pStart)
			return FALSE;
	}

	return TRUE;
}

BOOL CSharedMem::GetHeaderData(char *pData)
{
	if(!m_pShm || !m_pHeader) return FALSE;

	CopyMemory(pData, m_pHeader, m_lHeaderSize);

	return TRUE;
}

BOOL CSharedMem::GetHeaderData(/*in*/int nStartPos, /*in*/int nLen, /*out*/char *pData)
{
	if(!m_pShm || !m_pHeader) return FALSE;

	if(m_lHeaderSize < nLen + nStartPos)
		return FALSE;

	CopyMemory(pData, m_pHeader + nStartPos, nLen);

	return TRUE;
}

BOOL CSharedMem::SetHeaderData(char *pData)
{
	if(!m_pShm || !m_pHeader) return FALSE;

	CopyMemory(m_pHeader, pData, m_lHeaderSize);

	return TRUE;
}

BOOL CSharedMem::SetHeaderData(/*in*/int nStartPos, /*in*/int nLen, /*in*/char *pData)
{
	if(!m_pShm || !m_pHeader) return FALSE;

	if(m_lHeaderSize < nLen + nStartPos)
		return FALSE;

	CopyMemory(m_pHeader + nStartPos, pData, nLen);

	return TRUE;
}

BOOL CSharedMem::ClearData(BOOL bGroup, BOOL bHeader, BOOL bStruct)
{
	if(!m_pShm) return FALSE;

	try{
		if(bGroup && m_pGroup)
		{
			ZeroMemory(m_pGroup, m_lGroupIndexSize * m_lMaxGroupCnt);
			*(long*)m_pShm = 0;
		}

		if(bHeader && m_pHeader)
			ZeroMemory(m_pHeader, m_lHeaderSize);
		
		if(bStruct && m_pStruct)
			ZeroMemory(m_pStruct, (m_lStructSize * m_lMaxStructCnt + INIT_STRUCT_DATA_SIZE) * m_lMaxGroupCnt);
	}
	catch(...){
		lstrcpy(m_szErr, "ClearData try catch error.. maybe size error");
		return FALSE;
	}
	return TRUE;
}

int	CSharedMem::GetCurrGroupCnt()
{
	if(!m_pShm) return -1;

	return *(long*)m_pShm;
}

int	CSharedMem::GetCurrStructCnt(char *pGroupKey)
{
	if(!m_pShm || !pGroupKey) return -1;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return -1;

	return *(long*)pStruct;
}

char* CSharedMem::SearchGroupPointer(char *pGroupKey)
{
	if(!m_pGroup) return NULL;

	m_bCompareGroup = TRUE;
	long lCurrGroupCnt = *(long*)m_pShm;
	char *pGroup = (char*)Search(pGroupKey, m_pGroup, lCurrGroupCnt, m_lGroupIndexSize);
	return pGroup;
}

char* CSharedMem::SearchStructPointer(char *pGroupKey)
{
	char *pGroup = SearchGroupPointer(pGroupKey);
	if(!pGroup) 
		return NULL;

	char **ppStruct = (char**)(pGroup + m_lGroupKeySize);
	char *pStruct = (char*)((long)m_pShm + (long)*ppStruct);
	return pStruct;
}

char* CSharedMem::SearchDataPointer(char *pStruct, char *pStructKey)
{
	m_bCompareGroup = FALSE;
	long lCurrStructCnt = *(long*)pStruct;
	char *pData = (char*)Search(pStructKey, pStruct + INIT_STRUCT_DATA_SIZE, lCurrStructCnt, m_lStructSize);
	return pData;
}

void CSharedMem::SetErrMsg(int nErrCode, char* msg)
{
	sprintf_s(m_szErr,msg);
	LPVOID lpMsgBuf=NULL;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		nErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	strcat_s(m_szErr, (char*)lpMsgBuf);

	LocalFree(lpMsgBuf );
}

// BOOL CSharedMem::SetTransaction( CShmTransaction* pTrans, char* pGroupKey )
// {
// 	if(!m_pShm || !pGroupKey) return FALSE;
// 
// 	char *pGroup = SearchGroupPointer(pGroupKey);
// 	if(!pGroup) return FALSE;
// 
// 	char *pStruct = SearchStructPointer(pGroupKey);
// 	if(!pStruct) return FALSE;
// 
// 	long lRetCnt = *(long*)pStruct;
// 	if(lRetCnt<=0)	return FALSE;
// 
// 	long lSize = INIT_STRUCT_DATA_SIZE + (m_lStructSize * lRetCnt);
// 
// 	pTrans->AddList( pStruct, lSize );
// 
// 	return TRUE;
// }

int CSharedMem::Compare(const void *arg1,const void *arg2)
{
	long lCmpSize;

	if(m_bCompareGroup)
		lCmpSize = m_lGroupKeySize;
	else
		lCmpSize = m_lStructKeySize;

	return memcmp( arg1, arg2, lCmpSize );
}

void* CSharedMem::Search(const void *key, const void *base, size_t num, size_t width)
{
	char *lo = (char *)base;
	char *hi = (char *)base + (num - 1) * width;
	char *mid;
	unsigned int half;
	int result;
	
	while (lo <= hi)
	{
		if (half = num / 2)
		{
			mid = lo + (num & 1 ? half : (half - 1)) * width;
			if (!(result = Compare(key,mid)))
				return(mid);
			else if (result < 0)
			{
				hi = mid - width;
				num = num & 1 ? half : half-1;
			}
			else    {
				lo = mid + width;
				num = half;
			}
		}
		else if (num)
			return(Compare(key,lo) ? NULL : lo);
		else
			break;
	}

	return(NULL);
}

void CSharedMem::Qsort(void *base, size_t num, size_t width)
{
    char *lo, *hi;              /* ends of sub-array currently sorting */
    char *mid;                  /* points to middle of subarray */
    char *loguy, *higuy;        /* traveling pointers for partition step */
    size_t size;                /* size of the sub-array */
    char *lostk[STKSIZ], *histk[STKSIZ];
    int stkptr;                 /* stack for saving sub-array to be processed */

    if (num < 2 || width == 0)
        return;                 /* nothing to do */

    stkptr = 0;                 /* initialize stack */

    lo = (char *)base;
    hi = (char *)base + width * (num-1);        /* initialize limits */

recurse:

    size = (hi - lo) / width + 1;        /* number of el's to sort */

    if (size <= CUTOFF) {
        shortsort(lo, hi, width);
    }
    else {
        mid = lo + (size / 2) * width;      /* find middle element */

        if (Compare(lo, mid) > 0) {
            swap(lo, mid, width);
        }
        if (Compare(lo, hi) > 0) {
            swap(lo, hi, width);
        }
        if (Compare(mid, hi) > 0) {
            swap(mid, hi, width);
        }

        loguy = lo;
        higuy = hi;

        for (;;) {
            if (mid > loguy) {
                do  {
                    loguy += width;
                } while (loguy < mid && Compare(loguy, mid) <= 0);
            }
            if (mid <= loguy) {
                do  {
                    loguy += width;
                } while (loguy <= hi && Compare(loguy, mid) <= 0);
            }

            do  {
                higuy -= width;
            } while (higuy > mid && Compare(higuy, mid) > 0);

            if (higuy < loguy)
                break;

            swap(loguy, higuy, width);

            if (mid == higuy)
                mid = loguy;
        }

        higuy += width;
        if (mid < higuy) {
            do  {
                higuy -= width;
            } while (higuy > mid && Compare(higuy, mid) == 0);
        }
        if (mid >= higuy) {
            do  {
                higuy -= width;
            } while (higuy > lo && Compare(higuy, mid) == 0);
        }

        if ( higuy - lo >= hi - loguy ) {
            if (lo < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy;
                ++stkptr;
            }                           /* save big recursion for later */

            if (loguy < hi) {
                lo = loguy;
                goto recurse;           /* do small recursion */
            }
        }
        else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;               /* save big recursion for later */
            }

            if (lo < higuy) {
                hi = higuy;
                goto recurse;           /* do small recursion */
            }
        }
    }

    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;           /* pop subarray from stack */
    }
    else
        return;                 /* all subarrays done */
}


void CSharedMem::shortsort(char *lo, char *hi, size_t width)
{
    char *p, *max;

    while (hi > lo) {
        max = lo;
        for (p = lo+width; p <= hi; p += width) {
            if (Compare(p, max) > 0) {
                max = p;
            }
        }
        swap(max, hi, width);
        hi -= width;
    }
}

void CSharedMem::swap(char *a, char *b, size_t width)
{
    char tmp;

    if ( a != b )
        while ( width-- ) {
            tmp = *a;
            *a++ = *b;
            *b++ = tmp;
        }
}
