/*
	

*/
#if !defined(AFX_QUEUESHM_H__CF500232_EDF3_42F1_B896_66773D354A3D__INCLUDED_)
#define AFX_QUEUESHM_H__CF500232_EDF3_42F1_B896_66773D354A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma optimize("t", on)

#include <windows.h>
#include <stdio.h>

/*
	m_pShm
*/

// SHM의 맨 위에 초기화 데이터 Structure
typedef struct _INIT_DATA_Q
{
	long lCurrGroupCnt;
	long lMaxGroupCnt;		//	총 group 수
	long lGroupKeySize;		
	long lHeaderSize;
	long lStructSize;		//	한 struct 의 크기
	long lStructKeySize;
	long lMaxStructCnt;		//	한 group 이 갖는 struct(data) 의 수

	// Q
	long lGroupIndexSize;	//	lGroupKeySize + GROUP_POINTER_SIZE;
} INIT_DATA_Q;
#define INIT_DATA_Q_SIZE	sizeof(INIT_DATA_Q)

// 각 Structure 그룹의 맨 처음 데이터
typedef struct _STRUCT_HEADER_DATA_Q
{
	long lLastSavePos;		//	마지막 데이터 저장 위치
	long IsOverwriting;		//	한바퀴 이상 돌아서 overwriting 하고 있는가?
	long IsCopyStarted;		//	copy 를 시작했는가?
} STRUCT_HEADER_DATA_Q;
#define STRUCT_HEADER_Q_SIZE	sizeof(STRUCT_HEADER_DATA_Q)

#define GROUP_POINTER_Q_SIZE	4				// GroupKey의 포인터 저장하는 곳 크기(pointer)

#define CUTOFF_Q 8            /* testing shows that this is good value */
#define STKSIZ_Q (8*sizeof(void*) - 2)

//int compare( char *arg1, char *arg2 );
//long g_lGroupKeySize = 0;

class CQueueShm  
{
public:
	CQueueShm();
	virtual ~CQueueShm();

	int		Create(HANDLE hFile, INIT_DATA_Q *shmData, LPCTSTR lpShmName, LPCTSTR lpMutexName);
	BOOL	Open(LPCTSTR lpShmName, LPCTSTR lpMutexName);
	void	Close();

	//BOOL	SetInitSize(long lMaxGroupCnt, long lGroupKeySize, long lHeaderSize, long lStructSize, long lStructKeySize, long lMaxStructCnt);
	
	BOOL	GroupInsert(char *pGroupKey);
	BOOL	GroupDelete(char *pGroupKey);
	BOOL	GroupFind(char *pGroupKey);
	BOOL	GroupFindReverse(char *pGroupKey);
	int		GroupFindFirst();
	int		GroupFindLast();
	BOOL	GroupGetNext(/*out*/char *pGroupKey);
	int		GroupGetCurrCnt();


	BOOL	DataInsert(char *pGroupKey, char *pStructData);	// ==> save data in circular queue
	//BOOL	InsertDataArray(char *pGroupKey, int nCnt, char *pStructData);
	BOOL	DataIsEnd(long lLastSavePos);	//DONE 이후에 데이터를 저장할 공간이 더 없이 마지막인가?
	BOOL	DataUpdate(char *pGroupKey, char *pStructKey, char *pStructData);
	BOOL	DataUpdate(char *pGroupKey, char *pStructKey, /*in*/int nStartPos, /*in*/int nLen, char *pStructData);// 2004-09-19

	BOOL	DataGet(char *pGroupKey, char *pStructKey, /*out*/char *pStructData);
	BOOL	DataGet(char *pGroupKey, char *pStructKey, /*in*/int nStartPos, /*in*/int nLen, /*out*/char *pStructData);
	int		DataGetArray(char *pGroupKey, BOOL bAll, int nCnt, /*out*/char *pData);
	int		GetCurrDataCnt(char *pGroupKey) { return GetCurrStructCnt(pGroupKey); }
	BOOL	GetCurrStructKey(_In_ char* pGroupKey, _Out_ char* pStructKey);
	BOOL	IsSavedEnoughDataCnt(_In_ char* pGroupKey, _In_ int nBaseCnt);

	//BOOL	DelData(char *pGroupKey, char *pStructKey, BOOL bStructSort = TRUE);
	//BOOL	DelDataArray(char *pGroupKey, BOOL bAll, int nCnt, BOOL bStructSort = TRUE);
	//BOOL	SortData(char *pGroupKey);	/* only ascending */


	//BOOL	DataFind(char *pGroupKey, char *pStructKey);
	int		DataFind(char *pGroupKey, char *pStructKey);
	//int		FindDataReverse(char *pGroupKey, char *pStructKey);
	//int		FindFirstData(char *pGroupKey);
	//int		FindLastData(char *pGroupKey);
	//BOOL	GetNextData(/*out*/char *pStructData);

	BOOL	HeaderGetData(/*out*/char *pHeaderData);
	BOOL	HeaderGetData(/*in*/int nStartPos, /*in*/int nLen, /*out*/char *pHeaderData);
	BOOL	HeaderSetData(/*in*/char *pHeaderData);
	BOOL	HeaderSetData(/*in*/int nStartPos, /*in*/int nLen, /*in*/char *pHeaderData);
	int		GetCurrStructCnt(char *pGroupKey);
	
	void	SetErrMsg(int nErrCode, char* msg);
	inline	char*	GetErr(){return m_szErr;};
	BOOL	ClearData(BOOL bGroup, BOOL bHeader, BOOL bStruct);
protected:
	BOOL	SetInitSize(INIT_DATA_Q* initData);

	char*	SearchGroupPointer(char *pGroupKey);
	char*	SearchStructPointer(char *pGroupKey);
	char*	SearchDataPointer(char *pStruct, char *pFindKey);	// circular queue logic
	int		Compare(const void *arg1, const void *arg2, int nCompareSize);
	void*	Search(register const void *key, const void *base, size_t num, size_t width, bool bFindGroup);
	void	swap(char *a, char *b, size_t width);
	void	shortsort(char *lo, char *hi, size_t width);
	void	QsortGroup(void *base, size_t num, size_t width);

	void	Lock();
	void	Unlock();


	char	*m_pShm;
	char	*m_pGroup;
	char	*m_pHeader;
	char	*m_pStruct;
	HANDLE	m_hShm;
	HANDLE	m_hMutex;

	//Q
	INIT_DATA_Q	m_InitData;
	//long	m_lHeaderSize;
	//long	m_lGroupKeySize;
	//long	m_lGroupIndexSize;
	//long	m_lStructSize;
	//long	m_lStructKeySize;
	//long	m_lMaxGroupCnt;
	//long	m_lMaxStructCnt;
	

	char	m_szErr[512];
	//BOOL	m_bCompareGroup;
	BOOL	m_bAscGroup;
	BOOL	m_bAscData;
	char	*m_pFindGroup;
	char	*m_pFirstStruct;
	char	*m_pFindData;
	PSECURITY_DESCRIPTOR	m_pSD;      // Pointer to SD.
};

#endif // !defined(AFX_SHAREDMEM_H__CF500232_EDF3_42F1_B896_66773D354A3D__INCLUDED_)
