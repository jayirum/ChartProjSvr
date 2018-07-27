// SharedMem.cpp: implementation of the CQueueShm class.
//
//////////////////////////////////////////////////////////////////////

#include "QueueShm.h"
#include <search.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueueShm::CQueueShm()
{
	m_pShm = NULL;
	m_hShm = NULL;
	m_hMutex = NULL;
	m_pSD = NULL;

	m_pGroup = NULL;
	m_pHeader = NULL;
	m_pStruct = NULL;

	ZeroMemory(&m_InitData, sizeof(m_InitData));	
	m_bAscGroup = TRUE;
	m_bAscData = TRUE;
	m_pFindGroup = NULL;
	m_pFirstStruct = NULL;
	m_pFindData = NULL;

	ZeroMemory(m_szErr, sizeof(m_szErr));
}

CQueueShm::~CQueueShm()
{
	Close();
}


int CQueueShm::Create(HANDLE hFile, INIT_DATA_Q *shmData, LPCTSTR lpShmName, LPCTSTR lpMutexName)
{
	int nIndexSize	= ( shmData->lGroupKeySize + GROUP_POINTER_Q_SIZE ) * shmData->lMaxGroupCnt;
	int nDataSize	= shmData->lMaxGroupCnt * (STRUCT_HEADER_Q_SIZE + (shmData->lStructSize * shmData->lMaxStructCnt)  );
	int nTotSize	= INIT_DATA_Q_SIZE + nIndexSize + nDataSize + shmData->lHeaderSize;

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
	
	ZeroMemory(m_pShm, INIT_DATA_Q_SIZE);
	
	m_hMutex = CreateMutex(&sa, FALSE, lpMutexName);
	if (m_hMutex == NULL )
	{
		SetErrMsg(GetLastError(), "Mutex Open Error : ");
		Close();
		return -1;
	}

	if (!SetInitSize(shmData))
		return -1;
	
	if (!ClearData(TRUE, TRUE, TRUE))
		return -1;

	return nTotSize;
}


BOOL CQueueShm::Open(LPCTSTR lpShmName, LPCTSTR lpMutexName)
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
	
	CopyMemory((void*)&m_InitData, m_pShm, INIT_DATA_Q_SIZE);
	
	if(m_InitData.lMaxGroupCnt <= 0)
		m_pGroup = NULL;
	else
		m_pGroup = m_pShm + INIT_DATA_Q_SIZE;

	if(m_InitData.lHeaderSize <= 0)
		m_pHeader = NULL;
	else
		m_pHeader = m_pShm + INIT_DATA_Q_SIZE + (m_InitData.lGroupIndexSize * m_InitData.lMaxGroupCnt);

	if(m_InitData.lMaxStructCnt <= 0)
		m_pStruct = NULL;
	else
		m_pStruct = m_pShm + INIT_DATA_Q_SIZE + (m_InitData.lGroupIndexSize * m_InitData.lMaxGroupCnt) + m_InitData.lHeaderSize;

	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, lpMutexName);
	if (m_hMutex == NULL )
	{
		SetErrMsg(GetLastError(), "Mutex Open Error : ");
		Close();
		return FALSE;
	}

	return TRUE;
}

void CQueueShm::Close()
{
	if (m_pShm)
	{
		UnmapViewOfFile(m_pShm);
	}

//
////		if (!FlushViewOfFile(m_pShm, 0))
////			SetErrMsg(GetLastError(), "FlushViewOfFile Failed: ");
//
//		for(int i=0; i < 3; i++)
//		{
//			if(UnmapViewOfFile(m_pShm))
//				break;
//
////			SetErrMsg(GetLastError(), "UnmapviewOfFile Failed: ");
//			Sleep(10);
//		}
//	}

	if(m_hShm) CloseHandle(m_hShm);
	if(m_hMutex) CloseHandle(m_hMutex);
	if(m_pSD) LocalFree((HLOCAL) m_pSD);
	m_hShm = NULL;
	m_pShm = NULL;
	m_hMutex = NULL;
	m_pSD = NULL;
}

void CQueueShm::Lock()
{
	DWORD dw = WaitForSingleObject(m_hMutex, INFINITE);
	if( dw == WAIT_ABANDONED ){
		ReleaseMutex(m_hMutex);
		dw = WaitForSingleObject(m_hMutex, INFINITE);
	}
}

void CQueueShm::Unlock()
{
	ReleaseMutex(m_hMutex);
}


BOOL CQueueShm::ClearData(BOOL bGroup, BOOL bHeader, BOOL bStruct)
{
	if (!m_pShm) return FALSE;

	try {
		if (bGroup && m_pGroup)
		{
			ZeroMemory(m_pGroup, m_InitData.lGroupIndexSize * m_InitData.lMaxGroupCnt);
			*(long*)m_pShm = 0;
		}

		if (bHeader && m_pHeader)
			ZeroMemory(m_pHeader, m_InitData.lHeaderSize);

		if (bStruct && m_pStruct)
			ZeroMemory(m_pStruct, (m_InitData.lStructSize * m_InitData.lMaxStructCnt + STRUCT_HEADER_Q_SIZE) * m_InitData.lMaxGroupCnt);
	}
	catch (...) {
		lstrcpy(m_szErr, "ClearData try catch error.. maybe size error");
		return FALSE;
	}
	return TRUE;
}

BOOL CQueueShm::SetInitSize(INIT_DATA_Q* initData)
{
	if(!m_pShm)	return false;

	initData->lGroupIndexSize = initData->lGroupKeySize + GROUP_POINTER_Q_SIZE;
	CopyMemory(m_pShm, (void*)initData, INIT_DATA_Q_SIZE);
	CopyMemory(&m_InitData, m_pShm, INIT_DATA_Q_SIZE);

	if(m_InitData.lMaxGroupCnt <= 0)
		m_pGroup = NULL;
	else
		m_pGroup = m_pShm + INIT_DATA_Q_SIZE;

	if(m_InitData.lHeaderSize <= 0)
		m_pHeader = NULL;
	else
		m_pHeader = m_pShm + INIT_DATA_Q_SIZE + (m_InitData.lGroupIndexSize * m_InitData.lMaxGroupCnt);

	if(m_InitData.lMaxStructCnt <= 0)
		m_pStruct = NULL;
	else
		m_pStruct = m_pShm + INIT_DATA_Q_SIZE + (m_InitData.lGroupIndexSize * m_InitData.lMaxGroupCnt) + m_InitData.lHeaderSize;

	if (m_pGroup)
	{
		ZeroMemory(m_pGroup, m_InitData.lGroupIndexSize * m_InitData.lMaxGroupCnt);
		*(long*)m_pShm = 0;
	}

	if (m_pHeader)
		ZeroMemory(m_pHeader, m_InitData.lHeaderSize);

	if (m_pStruct)
		ZeroMemory(m_pStruct, (m_InitData.lStructSize * m_InitData.lMaxStructCnt + STRUCT_HEADER_Q_SIZE) * m_InitData.lMaxGroupCnt);

	return TRUE;
}	


BOOL CQueueShm::GetCurrStructKey(_In_ char* pGroupKey, _Out_ char* pStructKey)
{
	if (!m_pShm) return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if (!pStruct)
		return FALSE;

	STRUCT_HEADER_DATA_Q structHeader;
	CopyMemory(&structHeader, pStruct, sizeof(structHeader));

	// There is no data
	if (structHeader.IsCopyStarted == FALSE)
		return FALSE;

	char* pCurrKey = (char*)(pStruct + STRUCT_HEADER_Q_SIZE + (m_InitData.lStructSize * structHeader.lLastSavePos));

	memcpy(pStructKey, pCurrKey, m_InitData.lStructKeySize);
	
	return TRUE;
}

BOOL CQueueShm::DataGet(char *pGroupKey, char *pStructKey, char *pStructData)
{
	if(!m_pShm) return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) 
		return FALSE;

	char *pData = SearchDataPointer(pStruct, pStructKey);
	if(!pData) 
		return FALSE;

	CopyMemory(pStructData, pData, m_InitData.lStructSize);

	return TRUE;
}

BOOL CQueueShm::DataGet(char *pGroupKey, char *pStructKey, /*in*/int nStartPos, /*in*/int nLen, /*out*/char *pStructData)
{
	if(!m_pShm) return FALSE;

	if(m_InitData.lStructSize < nLen + nStartPos)
		return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return FALSE;

	char *pData = SearchDataPointer(pStruct, pStructKey);
	if(!pData) return FALSE;

	CopyMemory(pStructData, pData + nStartPos, nLen);

	return TRUE;
}

int CQueueShm::DataGetArray(char *pGroupKey, BOOL bAll, int nCnt, char *pData)
{
	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return FALSE;

	long lRetCnt = *(long*)pStruct;

	if(!bAll && lRetCnt > nCnt)	lRetCnt = nCnt;

	CopyMemory(pData, pStruct+STRUCT_HEADER_Q_SIZE, m_InitData.lStructSize * lRetCnt);

	return lRetCnt;
}

BOOL CQueueShm::DataUpdate(char *pGroupKey, char *pStructKey, char *pStructData)
{
	if(!m_pShm) return FALSE;
	
	Lock();
	__try {
		char *pStruct = SearchStructPointer(pGroupKey);
		if (!pStruct) return FALSE;

		char *pData = SearchDataPointer(pStruct, pStructKey);
		if (!pData) return FALSE;

		CopyMemory(pData, pStructData, m_InitData.lStructSize);
	}
	__finally {
		Unlock();
	}
	return TRUE;
}

BOOL CQueueShm::DataUpdate(char *pGroupKey, char *pStructKey, /*in*/int nStartPos, /*in*/int nLen, char *pStructData)
{
	if(!m_pShm) return FALSE;

	if(m_InitData.lStructSize < nLen + nStartPos)
		return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return FALSE;

	long lCurrStructCnt = *(long*)pStruct;	// 2004-09-19

	char *pData = SearchDataPointer(pStruct, pStructKey);
	if(!pData) return FALSE;

	CopyMemory(pData + nStartPos, pStructData, nLen);

	return TRUE;
}


/*
	현재 데이터 끝인가?
	Is End of data?
*/
BOOL CQueueShm::DataIsEnd(long lLastSavePos)
{
	if (lLastSavePos+1 == m_InitData.lMaxStructCnt)
		return TRUE;

	return FALSE;
}


/*
	lLastSavePos 는 마지막 데이터가 저장된 위치 이므로
	하나 증가시켜서 신규 데이터를 저장해야 한다.

	lLastSavePos is the place where last data is saved, 
	so increase one position and save a new data on there.
*/
BOOL CQueueShm::DataInsert(char *pGroupKey, char *pStructData)
{
	if(!m_pShm) return FALSE;
	char *pAddData = NULL;

	Lock();
	__try {
		char *pStruct = SearchStructPointer(pGroupKey);
		if (!pStruct)
			return FALSE;

		// 현재 COUNT 정보를 가져온다. (Get current count)
		STRUCT_HEADER_DATA_Q structHeader;
		ZeroMemory(&structHeader, sizeof(structHeader));
		memcpy(&structHeader, pStruct, sizeof(structHeader));

		// 제일 처음인 경우 (In case of start)
		if (structHeader.IsCopyStarted == FALSE )
		{
			structHeader.IsCopyStarted = TRUE;	//
		}
		else
		{
			// 마지막 저장한 위치가 끝인 경우 처음부터 다시 => 환형큐
			// (If here is the last place, start from at first again ==> circular queue)
			if (DataIsEnd(structHeader.lLastSavePos))
			{
				structHeader.lLastSavePos = 0;
				structHeader.IsOverwriting = TRUE;
			}
			else
				structHeader.lLastSavePos++;
		}

		// 데이터 저장할 위치 (the place on where data is saved)
		pAddData = (char*)(pStruct + STRUCT_HEADER_Q_SIZE + (m_InitData.lStructSize * structHeader.lLastSavePos));
		CopyMemory(pAddData, pStructData, m_InitData.lStructSize);		

		// header 다시 저장 (save header back)
		CopyMemory(pStruct, &structHeader, sizeof(structHeader));
	}
	__finally
	{
		Unlock();
	}
	return TRUE;
}

BOOL CQueueShm::GroupInsert(char *pGroupKey)
{
	if(!m_pShm || !m_pGroup || !m_pStruct) return FALSE;

	Lock();
	__try {
		long lCurrGroupCnt = *(long*)m_pShm;
		if (lCurrGroupCnt >= m_InitData.lMaxGroupCnt) {
			return FALSE;
		}

		char *pAddGroup = m_pGroup + (m_InitData.lGroupIndexSize * lCurrGroupCnt);
		CopyMemory(pAddGroup, pGroupKey, m_InitData.lGroupKeySize);

		char *pAddStruct = m_pStruct + ((m_InitData.lStructSize * m_InitData.lMaxStructCnt + STRUCT_HEADER_Q_SIZE) * lCurrGroupCnt);
		char **ppStruct = (char**)(pAddGroup + m_InitData.lGroupKeySize);
		*ppStruct = (char*)((long)pAddStruct - (long)m_pShm);	// 공유 메모리의 Start Pointer로부터 Offset 계산

		*(long*)pAddStruct = 0;
		*(long*)m_pShm = ++lCurrGroupCnt;

		if (lCurrGroupCnt > 1)
		{
			QsortGroup(m_pGroup, lCurrGroupCnt, m_InitData.lGroupIndexSize);
		}
	}
	__finally
	{
		Unlock();
	}

	return TRUE;
}

BOOL CQueueShm::GroupDelete(char *pGroupKey)
{
	if(!m_pShm || !m_pGroup || !m_pStruct) return FALSE;

	Lock();
	__try {
		char *pDelGroup = SearchGroupPointer(pGroupKey);
		if (!pDelGroup) return FALSE;

		char *pStruct = SearchStructPointer(pGroupKey);
		if (!pStruct) return FALSE;

		ZeroMemory(pStruct + STRUCT_HEADER_Q_SIZE, m_InitData.lStructSize * m_InitData.lMaxStructCnt);
		*(long*)pStruct = 0;

		long lCurrGroupCnt = *(long*)m_pShm;
		char *pStart = pDelGroup + m_InitData.lGroupIndexSize;
		char *pLast = m_pGroup + (m_InitData.lGroupIndexSize * lCurrGroupCnt);
		MoveMemory(pDelGroup, pStart, pLast - pStart);
		ZeroMemory(pLast - m_InitData.lGroupIndexSize, m_InitData.lGroupIndexSize);

		*(long*)m_pShm = --lCurrGroupCnt;

		QsortGroup(m_pGroup, lCurrGroupCnt, m_InitData.lGroupIndexSize);

		if (m_pFindGroup && lCurrGroupCnt > 0)
		{
			if (m_bAscGroup)
			{
				m_pFindGroup -= m_InitData.lGroupIndexSize;
				if (m_pFindGroup < pStart) m_pFindGroup = NULL;
			}
			else
			{
				pLast -= m_InitData.lGroupIndexSize;
				if (m_pFindGroup >= pLast) m_pFindGroup = NULL;
			}
		}
	}
	__finally {
		Unlock();
	}
	return TRUE;
}


BOOL CQueueShm::GroupFind(char *pGroupKey)
{
	m_pFindGroup = NULL;

	if(!m_pShm || !pGroupKey) return FALSE;

	char *pGroup = SearchGroupPointer(pGroupKey);
	if(!pGroup) return FALSE;

	m_bAscGroup = TRUE;

	return TRUE;
}

BOOL CQueueShm::GroupFindReverse(char *pGroupKey)
{
	BOOL bRet = GroupFind(pGroupKey);

	m_bAscGroup = FALSE;

	return bRet;
}

int CQueueShm::GroupFindFirst()
{
	m_pFindGroup = NULL;

	if(!m_pShm) return -1;

	long lCurrGroupCnt = *(long*)m_pShm;
	if(lCurrGroupCnt < 1)
		return 0;

	m_bAscGroup = TRUE;

	return 1;
}

int CQueueShm::GroupFindLast()
{
	int nRet = GroupFindFirst();

	m_bAscGroup = FALSE;

	return nRet;
}

BOOL CQueueShm::GroupGetNext(/*out*/char *pGroupKey)
{
	long lCurrGroupCnt = *(long*)m_pShm;
	char *pStart = m_pGroup; 
	char *pLast = pStart + (m_InitData.lGroupIndexSize * lCurrGroupCnt);

	Lock();
	__try {
		if (m_bAscGroup)
		{
			if (m_pFindGroup == NULL)
				m_pFindGroup = pStart;
			else
				m_pFindGroup += m_InitData.lGroupIndexSize;

			if (m_pFindGroup >= pLast)
			{
				m_pFindGroup = NULL;
				*pGroupKey = 0x00;
				return FALSE;
			}

			CopyMemory(pGroupKey, m_pFindGroup, m_InitData.lGroupKeySize);

			if (m_pFindGroup + m_InitData.lGroupIndexSize >= pLast)
				return FALSE;
		}
		else
		{
			if (m_pFindGroup == NULL)
				m_pFindGroup = pLast - m_InitData.lGroupIndexSize;
			else
				m_pFindGroup -= m_InitData.lGroupIndexSize;

			if (m_pFindGroup < pStart)
			{
				m_pFindGroup = NULL;
				*pGroupKey = 0x00;
				return FALSE;
			}

			CopyMemory(pGroupKey, m_pFindGroup, m_InitData.lGroupKeySize);

			if (m_pFindGroup - m_InitData.lGroupIndexSize < pStart)
				return FALSE;
		}
	}
	__finally
	{
		Unlock();
	}

	return TRUE;
}

//BOOL CQueueShm::DataFind(char *pGroupKey, char *pStructKey)
int CQueueShm::DataFind(char *pGroupKey, char *pStructKey)
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

//

BOOL CQueueShm::HeaderGetData(char *pData)
{
	if(!m_pShm || !m_pHeader) return FALSE;

	CopyMemory(pData, m_pHeader, m_InitData.lHeaderSize);

	return TRUE;
}

BOOL CQueueShm::HeaderGetData(/*in*/int nStartPos, /*in*/int nLen, /*out*/char *pData)
{
	if(!m_pShm || !m_pHeader) return FALSE;

	if(m_InitData.lHeaderSize < nLen + nStartPos)
		return FALSE;

	CopyMemory(pData, m_pHeader + nStartPos, nLen);

	return TRUE;
}

BOOL CQueueShm::HeaderSetData(char *pData)
{
	if(!m_pShm || !m_pHeader) return FALSE;

	CopyMemory(m_pHeader, pData, m_InitData.lHeaderSize);

	return TRUE;
}

BOOL CQueueShm::HeaderSetData(/*in*/int nStartPos, /*in*/int nLen, /*in*/char *pData)
{
	if(!m_pShm || !m_pHeader) return FALSE;

	if(m_InitData.lHeaderSize < nLen + nStartPos)
		return FALSE;

	CopyMemory(m_pHeader + nStartPos, pData, nLen);

	return TRUE;
}



int	CQueueShm::GroupGetCurrCnt()
{
	if(!m_pShm) return -1;

	return *(long*)m_pShm;
}

int	CQueueShm::GetCurrStructCnt(char *pGroupKey)
{
	if(!m_pShm || !pGroupKey) return -1;

	char *pStruct = SearchStructPointer(pGroupKey);
	if(!pStruct) return -1;

	STRUCT_HEADER_DATA_Q* p = (STRUCT_HEADER_DATA_Q*)pStruct;
	return p->lLastSavePos+1;
}

// 파라미터의 숫자 이상의 데이터가 저장되었는가?
// The number of data which saved on SHM is more than nBaseCnt
BOOL CQueueShm::IsSavedEnoughDataCnt(_In_ char* pGroupKey, _In_ int nBaseCnt)
{
	if (!m_pShm || !pGroupKey) return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if (!pStruct) return FALSE;

	BOOL bRet = FALSE;
	STRUCT_HEADER_DATA_Q* p = (STRUCT_HEADER_DATA_Q*)pStruct;
	if (p->IsOverwriting == FALSE)
	{
		bRet = (nBaseCnt <= p->lLastSavePos + 1) ? TRUE : FALSE;
	}
	else
	{
		if (nBaseCnt <= m_InitData.lMaxStructCnt)
			bRet = TRUE;
		else
		{
			if (nBaseCnt <= m_InitData.lMaxStructCnt + p->lLastSavePos + 1)
				bRet = TRUE;
		}
	}
	return bRet;
}

char* CQueueShm::SearchGroupPointer(char *pGroupKey)
{
	if(!m_pGroup) return NULL;

	long lCurrGroupCnt = *(long*)m_pShm;
	char *pGroup = (char*)Search(pGroupKey, m_pGroup, lCurrGroupCnt, m_InitData.lGroupIndexSize, TRUE);
	return pGroup;
}

char* CQueueShm::SearchStructPointer(char *pGroupKey)
{
	char *pGroup = SearchGroupPointer(pGroupKey);
	if(!pGroup) 
		return NULL;

	char **ppStruct = (char**)(pGroup + m_InitData.lGroupKeySize);
	char *pStruct = (char*)((long)m_pShm + (long)*ppStruct);
	return pStruct;
}


/*
	환형큐에서 원하는 데이터를 찾는다.
	lLastSavePos 는 마지막 데이터가 저장된 위치이므로
	저장된 데이터 count 는 lLastSavePos+1 이어야 한다.

	Find the data in SHM.
	lLastSavePos is the position where contain the last data,
	so the count of saved data is lLastSavePos+1
*/
char* CQueueShm::SearchDataPointer(char *pStruct, char *pFindKey)
{
	char *pData = NULL;
	int nSearchCnt = 0;
	STRUCT_HEADER_DATA_Q structHeader;
	CopyMemory(&structHeader, pStruct, sizeof(structHeader));

	// 아직 copy 된게 없다. (There is nothing to be copied)
	if (structHeader.IsCopyStarted == FALSE)
	{
		return NULL;
	}

	// 제일 첫 위치 (the first position)
	char* pFirstKey = (char*)(pStruct + STRUCT_HEADER_Q_SIZE + (m_InitData.lStructSize * 0));

	/*
	// 제일 첫 바퀴. 아직 overwriting 이 안된 상황 (The first run. There is no overwritten data yet)
	// -> 현재 위치까지를 search 한다. (scan until current position)
	*/
	if (structHeader.IsOverwriting == FALSE)
	{
		nSearchCnt = structHeader.lLastSavePos + 1;
		pData = (char*)Search(pFindKey, pFirstKey, nSearchCnt, m_InitData.lStructSize, FALSE);
		return pData;
	}

	/*
	// 이미 overwriting 이 시작된 상황. (Overwriting started already)
	*/

	// 마지막 저장위치가 끝이라면 전체에 대해 search 한다. (If the last saved position is end of the queue, scan the full queueu)
	if (DataIsEnd(structHeader.lLastSavePos))
	{
		pData = (char*)Search(pFindKey, pFirstKey, structHeader.lLastSavePos + 1, m_InitData.lStructSize, FALSE);
		return pData;
	}
	
	// 마지막 저장위치가 끝이 아니고 (If the last saved position is not end of the queue)
	// FIRST KEY=====================현재위치KEY(current key)--------찾는KEY(searching key)----------------END  
	// FIRST KEY=======찾는KEY(searching key)==============현재위치KEY(current key)------------------------END
	
	char* pCurrKey = (char*)(pStruct + STRUCT_HEADER_Q_SIZE + (m_InitData.lStructSize * structHeader.lLastSavePos));
	
	// 최초key(the first key) = 찾는key(searching key) ?
	int nComp = Compare(pFirstKey, pFindKey, m_InitData.lStructKeySize);
	if(nComp==0)	return pFirstKey;

	// 최초key(the first key) = 현재key(searching key)
	nComp = Compare(pFirstKey, pCurrKey, m_InitData.lStructKeySize);
	if (nComp == 0)
	{
		char* pBase = pStruct + STRUCT_HEADER_Q_SIZE + (m_InitData.lStructSize*(structHeader.lLastSavePos + 1));
		int nCnt = m_InitData.lMaxStructCnt - structHeader.lLastSavePos - 1;
		pData = (char*)Search(pFindKey, pBase, nCnt, m_InitData.lStructSize, FALSE);
		return pData;
	}


	// 현재key(current key) = 찾는key(searching key) 
	nComp = Compare(pCurrKey, pFindKey, m_InitData.lStructKeySize);
	if (nComp == 0)	return pCurrKey;

	
	// 현재key(current key) < 찾는key(searching key)
	if (nComp < 0)
	{
		char* pBase = pStruct + STRUCT_HEADER_Q_SIZE + (m_InitData.lStructSize*(structHeader.lLastSavePos+1));
		int nCnt = m_InitData.lMaxStructCnt - structHeader.lLastSavePos-1;
		pData = (char*)Search(pFindKey, pBase, nCnt, m_InitData.lStructSize, FALSE);
	}

	// 현재key(current key) > 찾는key(searching key)
	if (nComp > 0)
	{
		char* pBase = pFirstKey;
		int nCnt = structHeader.lLastSavePos + 1;
		pData = (char*)Search(pFindKey, pBase, nCnt, m_InitData.lStructSize, FALSE);
	}

	return pData;
}

void CQueueShm::SetErrMsg(int nErrCode, char* msg)
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

// BOOL CQueueShm::SetTransaction( CShmTransaction* pTrans, char* pGroupKey )
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
// 	long lSize = STRUCT_HEADER_Q_SIZE + (m_InitData.lStructSize * lRetCnt);
// 
// 	pTrans->AddList( pStruct, lSize );
// 
// 	return TRUE;
// }

int CQueueShm::Compare(const void *arg1,const void *arg2, int nCompareSize)
{
	return memcmp( arg1, arg2, nCompareSize);
}

void* CQueueShm::Search(const void *key, const void *base, size_t num, size_t width, bool bFindGroup)
{
	char *lo = (char *)base;
	char *hi = (char *)base + (num - 1) * width;
	char *mid;
	unsigned int half;
	int result;
	int nKeySize = (bFindGroup)? m_InitData.lGroupKeySize : m_InitData.lStructKeySize;

	while (lo <= hi)
	{
		if (half = num / 2)
		{
			mid = lo + (num & 1 ? half : (half - 1)) * width;
			if (!(result = Compare(key,mid, nKeySize)))
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
			return(Compare(key,lo, nKeySize) ? NULL : lo);
		else
			break;
	}

	return(NULL);
}

void CQueueShm::QsortGroup(void *base, size_t num, size_t width)
{
    char *lo, *hi;              /* ends of sub-array currently sorting */
    char *mid;                  /* points to middle of subarray */
    char *loguy, *higuy;        /* traveling pointers for partition step */
    size_t size;                /* size of the sub-array */
    char *lostk[STKSIZ_Q], *histk[STKSIZ_Q];
    int stkptr;                 /* stack for saving sub-array to be processed */

    if (num < 2 || width == 0)
        return;                 /* nothing to do */

    stkptr = 0;                 /* initialize stack */

    lo = (char *)base;
    hi = (char *)base + width * (num-1);        /* initialize limits */

recurse:

    size = (hi - lo) / width + 1;        /* number of el's to sort */

    if (size <= CUTOFF_Q) {
        shortsort(lo, hi, width);
    }
    else {
        mid = lo + (size / 2) * width;      /* find middle element */

        if (Compare(lo, mid, m_InitData.lGroupKeySize) > 0) {
            swap(lo, mid, width);
        }
        if (Compare(lo, hi, m_InitData.lGroupKeySize) > 0) {
            swap(lo, hi, width);
        }
        if (Compare(mid, hi, m_InitData.lGroupKeySize) > 0) {
            swap(mid, hi, width);
        }

        loguy = lo;
        higuy = hi;

        for (;;) {
            if (mid > loguy) {
                do  {
                    loguy += width;
                } while (loguy < mid && Compare(loguy, mid, m_InitData.lGroupKeySize) <= 0);
            }
            if (mid <= loguy) {
                do  {
                    loguy += width;
                } while (loguy <= hi && Compare(loguy, mid, m_InitData.lGroupKeySize) <= 0);
            }

            do  {
                higuy -= width;
            } while (higuy > mid && Compare(higuy, mid, m_InitData.lGroupKeySize) > 0);

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
            } while (higuy > mid && Compare(higuy, mid, m_InitData.lGroupKeySize) == 0);
        }
        if (mid >= higuy) {
            do  {
                higuy -= width;
            } while (higuy > lo && Compare(higuy, mid, m_InitData.lGroupKeySize) == 0);
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


void CQueueShm::shortsort(char *lo, char *hi, size_t width)
{
    char *p, *max;

    while (hi > lo) {
        max = lo;
        for (p = lo+width; p <= hi; p += width) {
            if (Compare(p, max, m_InitData.lGroupKeySize) > 0) {
                max = p;
            }
        }
        swap(max, hi, width);
        hi -= width;
    }
}

void CQueueShm::swap(char *a, char *b, size_t width)
{
    char tmp;

    if ( a != b )
        while ( width-- ) {
            tmp = *a;
            *a++ = *b;
            *b++ = tmp;
        }
}




//int CQueueShm::FindDataReverse(char *pGroupKey, char *pStructKey)
//{
//	int nRet = DataFind(pGroupKey, pStructKey);
//
//	m_bAscData = FALSE;
//
//	return nRet;TRUE;
//}
//
//int CQueueShm::FindFirstData(char *pGroupKey)
//{
//	m_pFirstStruct = NULL;
//	m_pFindData = NULL;
//
//	if(!m_pShm || !pGroupKey) return -1;
//
//	m_pFirstStruct = SearchStructPointer(pGroupKey);
//	if(!m_pFirstStruct) return -1;
//
//	if( *(long*)m_pFirstStruct < 1)
//		return 0;
//
//	m_bAscData = TRUE;
//
//	return 1;
//}
//
//int CQueueShm::FindLastData(char *pGroupKey)
//{
//	int nRet = FindFirstData(pGroupKey);
//
//	m_bAscData = FALSE;
//
//	return nRet;
//}
//
///*
//	다음에 데이터가 없으면 return FALSE;
//*/
//BOOL CQueueShm::GetNextData(/*out*/char *pStructData)
//{
//	if(!m_pFirstStruct)
//	{
//		m_pFindData = NULL;
//		*pStructData = 0x00;
//		return FALSE;
//	}
//
//	long lCurrStructCnt = *(long*)m_pFirstStruct;
//	char *pStart = m_pFirstStruct + STRUCT_HEADER_Q_SIZE;
//	char *pLast =  pStart + (m_InitData.lStructSize * lCurrStructCnt);
//
//	if(m_bAscData)
//	{
//		if(m_pFindData == NULL)
//			m_pFindData = pStart;
//		else
//			m_pFindData += m_InitData.lStructSize;
//		
//		if(m_pFindData >= pLast)
//		{
//			m_pFirstStruct = NULL;
//			m_pFindData = NULL;
//			*pStructData = 0x00;
//			return FALSE;
//		}
//		
//		CopyMemory(pStructData, m_pFindData, m_InitData.lStructSize);
//		
//		if(m_pFindData + m_InitData.lStructSize >= pLast)
//			return FALSE;
//	}
//	else
//	{
//		if(m_pFindData == NULL)
//			m_pFindData = pLast - m_InitData.lStructSize;
//		else
//			m_pFindData -= m_InitData.lStructSize;
//		
//		if(m_pFindData < pStart)
//		{
//			m_pFirstStruct = NULL;
//			m_pFindData = NULL;
//			*pStructData = 0x00;
//			return FALSE;
//		}
//		
//		CopyMemory(pStructData, m_pFindData, m_InitData.lStructSize);
//		
//		if(m_pFindData - m_InitData.lStructSize < pStart)
//			return FALSE;
//	}
//
//	return TRUE;
//}


//BOOL CQueueShm::InsertDataArray(char *pGroupKey, int nCnt, char *pStructData)
//{
//	if(!m_pShm) return FALSE;
//
//	char *pStruct = SearchStructPointer(pGroupKey);
//	if(!pStruct) return FALSE;
//
//	long lCurrStructCnt = *(long*)pStruct;
//	char *pAddData = (char*)(pStruct + STRUCT_HEADER_Q_SIZE + (m_InitData.lStructSize * lCurrStructCnt));
//
//	long lAddCnt = 0;
//	if(nCnt + lCurrStructCnt > m_InitData.lMaxStructCnt)	
//		lAddCnt = m_InitData.lMaxStructCnt - lCurrStructCnt;
//	else
//		lAddCnt = nCnt;
//
//	CopyMemory(pAddData, pStructData, m_InitData.lStructSize*lAddCnt);
//
//	lCurrStructCnt += lAddCnt;
//	*(long*)pStruct = lCurrStructCnt;
//
//	if(lCurrStructCnt > 1)
//	{
//		m_bCompareGroup = FALSE;
//		Qsort(pStruct+STRUCT_HEADER_Q_SIZE, lCurrStructCnt, m_InitData.lStructSize);
//	}
//
//	return TRUE;
//}


//BOOL CQueueShm::DelData(char *pGroupKey, char *pStructKey, BOOL bStructSort)
//{
//	if(!m_pShm) return FALSE;
//
//	char *pStruct = SearchStructPointer(pGroupKey);
//	if(!pStruct) 
//		return FALSE;
//
//	char *pData = SearchDataPointer(pStruct, pStructKey);
//	if(!pData) 
//		return FALSE;
//
//	long lCurrStructCnt = *(long*)pStruct;
//	char *pStart = pData + m_InitData.lStructSize; 
//	char *pLast = pStruct + STRUCT_HEADER_Q_SIZE + (m_InitData.lStructSize * lCurrStructCnt);
//	MoveMemory(pData, pStart, pLast - pStart);
//	ZeroMemory(pLast - m_InitData.lStructSize, m_InitData.lStructSize);
//
//	*(long*)pStruct = --lCurrStructCnt;
//
//	if(bStructSort)
//	{
//		m_bCompareGroup = FALSE;
//		Qsort(pStruct+STRUCT_HEADER_Q_SIZE, lCurrStructCnt, m_InitData.lStructSize);
//	}
//
//	if(m_pFindData)
//	{
//		if(m_bAscData)
//		{
//			m_pFindData -= m_InitData.lStructSize;
//			if(m_pFindData < pStart) m_pFindData = NULL;
//		}
//		else
//		{
//			pLast -= m_InitData.lStructSize;
//			if(m_pFindData >= pLast) m_pFindData = NULL;
//		}
//	}
//
//	return TRUE;
//}
//
//BOOL CQueueShm::DelDataArray(char *pGroupKey, BOOL bAll, int nCnt, BOOL bStructSort)
//{
//	if(!m_pShm) return FALSE;
//
//	char *pStruct = SearchStructPointer(pGroupKey);
//	if(!pStruct) return FALSE;
//
//	long lCurrStructCnt = *(long*)pStruct;
//	long lDelCnt = 0;
//
//	if(!bAll && lCurrStructCnt > nCnt)	lDelCnt = nCnt;
//	else								lDelCnt = lCurrStructCnt;
//
//	char *pStart = pStruct + STRUCT_HEADER_Q_SIZE + (m_InitData.lStructSize * lDelCnt);
//	long lCopySize = (lCurrStructCnt-lDelCnt) * m_InitData.lStructSize;
//	MoveMemory(pStruct + STRUCT_HEADER_Q_SIZE, pStart, lCopySize);
//	ZeroMemory(pStruct + STRUCT_HEADER_Q_SIZE + lCopySize, m_InitData.lStructSize * lDelCnt);
//
//	lCurrStructCnt -= lDelCnt;
//	*(long*)pStruct = lCurrStructCnt;
//
//	if(bStructSort && lCurrStructCnt > 1)
//	{
//		m_bCompareGroup = FALSE;
//		Qsort(pStruct+STRUCT_HEADER_Q_SIZE, lCurrStructCnt, m_InitData.lStructSize);
//	}
//
//	return TRUE;
//}
//
//BOOL CQueueShm::SortData(char *pGroupKey)
//{
//	if(!m_pShm) return FALSE;
//
//	char *pStruct = SearchStructPointer(pGroupKey);
//	if(!pStruct) return FALSE;
//
//	long lCurrStructCnt = *(long*)pStruct;
//
//	m_bCompareGroup = FALSE;
//	Qsort(pStruct+STRUCT_HEADER_Q_SIZE, lCurrStructCnt, m_InitData.lStructSize);
//
//	return TRUE;
//}

//BOOL CQueueShm::GroupSort()
//{
//	if(!m_pShm || !m_pGroup) return FALSE;
//
//	long lCurrGroupCnt = *(long*)m_pShm;
//
//	QsortGroup(m_pGroup, lCurrGroupCnt, m_InitData.lGroupIndexSize);
//
//	return TRUE;
//}
