// SharedMem.h: interface for the CSharedMem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREDMEM_H__CF500232_EDF3_42F1_B896_66773D354A3D__INCLUDED_)
#define AFX_SHAREDMEM_H__CF500232_EDF3_42F1_B896_66773D354A3D__INCLUDED_

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
typedef struct _INIT_DATA
{
	long lCurrGroupCnt;
	long lMaxGroupCnt;
	long lGroupKeySize;
	long lHeaderSize;
	long lStructSize;
	long lStructKeySize;
	long lMaxStructCnt;
} INIT_DATA;
#define INIT_DATA_SIZE	sizeof(INIT_DATA)

// 각 Structure 그룹의 맨 처음 데이터
typedef struct _INIT_STRUCT_DATA
{
	long lCurrStructCnt;
} INIT_STRUCT_DATA;
#define INIT_STRUCT_DATA_SIZE	sizeof(INIT_STRUCT_DATA)

#define GROUP_POINTER_SIZE	4				// GroupKey의 포인터 저장하는 곳 크기(pointer)

#define CUTOFF 8            /* testing shows that this is good value */
#define STKSIZ (8*sizeof(void*) - 2)

//int compare( char *arg1, char *arg2 );
//long g_lGroupKeySize = 0;

class CSharedMem  
{
public:
	CSharedMem();
	virtual ~CSharedMem();

	int		Create(HANDLE hFile, DWORD dwSize, LPCTSTR lpShmName, LPCTSTR lpMutexName);
	int		Create(HANDLE hFile, INIT_DATA *shmData, LPCTSTR lpShmName, LPCTSTR lpMutexName);
	BOOL	Open(LPCTSTR lpShmName, LPCTSTR lpMutexName);
	void	Close();

	BOOL	SetInitSize(long lMaxGroupCnt, long lGroupKeySize, long lHeaderSize, long lStructSize, long lStructKeySize, long lMaxStructCnt);
	
	BOOL	GetData(char *pGroupKey, char *pStructKey, /*out*/char *pStructData);
	BOOL	GetData(char *pGroupKey, char *pStructKey, /*in*/int nStartPos, /*in*/int nLen, /*out*/char *pStructData);
	int		GetDataArray(char *pGroupKey, BOOL bAll, int nCnt, /*out*/char *pData);
	BOOL	UpdateData(char *pGroupKey, char *pStructKey, char *pStructData);
	BOOL	UpdateData(char *pGroupKey, char *pStructKey, 
			/*in*/int nStartPos, /*in*/int nLen, char *pStructData, BOOL bStructSort=FALSE);// 2004-09-19
	BOOL	SetData(char *pGroupKey, char *pStructKey, char *pStructData);
	BOOL	SetData(char *pGroupKey, char *pStructKey, 
			/*in*/int nStartPos, /*in*/int nLen, char *pStructData, BOOL bStructSort=FALSE);// 2004-09-19
	BOOL	InsertData(char *pGroupKey, char *pStructData, BOOL bStructSort = TRUE);
	BOOL	AddData(char *pGroupKey, char *pStructData, BOOL bStructSort = TRUE);
	BOOL	InsertDataArray(char *pGroupKey, int nCnt, char *pStructData, BOOL bStructSort = TRUE);
	BOOL	AddDataArray(char *pGroupKey, int nCnt, char *pStructData, BOOL bStructSort = TRUE);
	BOOL	DelData(char *pGroupKey, char *pStructKey, BOOL bStructSort = TRUE);
	BOOL	DelDataArray(char *pGroupKey, BOOL bAll, int nCnt, BOOL bStructSort = TRUE);
	BOOL	SortData(char *pGroupKey);	/* only ascending */

	BOOL	InsertGroup(char *pGroupKey, BOOL bGroupSort = TRUE);
	BOOL	AddGroup(char *pGroupKey, BOOL bGroupSort = TRUE);
	BOOL	DelGroup(char *pGroupKey, BOOL bGroupSort = TRUE);
	BOOL	SortGroup(); /* only ascending */
	
	BOOL	FindGroup(char *pGroupKey);
	BOOL	FindGroupReverse(char *pGroupKey);
	int		FindFirstGroup();
	int		FindLastGroup();
	BOOL	GetNextGroup(/*out*/char *pGroupKey);
	//BOOL	FindData(char *pGroupKey, char *pStructKey);
	int		FindData(char *pGroupKey, char *pStructKey);
	int		FindDataReverse(char *pGroupKey, char *pStructKey);
	int		FindFirstData(char *pGroupKey);
	int		FindLastData(char *pGroupKey);
	BOOL	GetNextData(/*out*/char *pStructData);

	BOOL	GetHeaderData(/*out*/char *pHeaderData);
	BOOL	GetHeaderData(/*in*/int nStartPos, /*in*/int nLen, /*out*/char *pHeaderData);
	BOOL	SetHeaderData(/*in*/char *pHeaderData);
	BOOL	SetHeaderData(/*in*/int nStartPos, /*in*/int nLen, /*in*/char *pHeaderData);
	BOOL	ClearData(BOOL bGroup = TRUE, BOOL bHeader = TRUE, BOOL bStruct = TRUE);
	int		GetCurrGroupCnt();
	int		GetCurrStructCnt(char *pGroupKey);
	
	void	Lock();
	void	Unlock();
	void	SetErrMsg(int nErrCode, char* msg);
	inline	char*	GetErr(){return m_szErr;};

//	BOOL	SetTransaction( CShmTransaction* pTrans, char* pGroupKey );

protected:
	char*	SearchGroupPointer(char *pGroupKey);
	char*	SearchStructPointer(char *pGroupKey);
	char*	SearchDataPointer(char *pStruct, char *pStructKey);
	int		Compare(const void *arg1, const void *arg2);
	void*	Search(register const void *key, const void *base, size_t num, size_t width);
	void	swap(char *a, char *b, size_t width);
	void	shortsort(char *lo, char *hi, size_t width);
	void	Qsort(void *base, size_t num, size_t width);

	char	*m_pShm;
	char	*m_pGroup;
	char	*m_pHeader;
	char	*m_pStruct;
	HANDLE	m_hShm;
	HANDLE	m_hMutex;
	long	m_lHeaderSize;
	long	m_lGroupKeySize;
	long	m_lGroupIndexSize;
	long	m_lStructSize;
	long	m_lStructKeySize;
	long	m_lMaxGroupCnt;
	long	m_lMaxStructCnt;
	char	m_szErr[512];
	BOOL	m_bCompareGroup;
	BOOL	m_bAscGroup;
	BOOL	m_bAscData;
	char	*m_pFindGroup;
	char	*m_pFirstStruct;
	char	*m_pFindData;
	PSECURITY_DESCRIPTOR	m_pSD;      // Pointer to SD.
};

#endif // !defined(AFX_SHAREDMEM_H__CF500232_EDF3_42F1_B896_66773D354A3D__INCLUDED_)
