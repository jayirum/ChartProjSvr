#pragma once

#include "MultiRBTree.h"
#include "RBTree.h"
#include <stdio.h>
#include <windows.h>
#include <map>

typedef struct _LIST_DATA
{
	char sIndex[MAXLEN_KEY];
	int nIndexLen;
	TMultiRBTree* mTree;
}LIST_DATA;

typedef std::map<int, LIST_DATA*> TList ;			// index, 
typedef std::map<int, LIST_DATA*>::iterator		TListP;

class  TBaseMemTable
{
public:
	TBaseMemTable();
	virtual ~TBaseMemTable();

protected:
	virtual void DataFree(void* Data) = 0;
	virtual char* GetKey(int Index, void* Data, char* oKey) = 0;
	TRBTree* GetDataList();
	void Insert(void* Data);
	virtual void* CreateRecord() = 0;

public:
	void Lock();
	void UnLock();

public:
	void Clear();
	void Delete(void* Data);
	void Edit(void* OldData, void* NewData);
	void AddIndex(char* IndexName);
	TMultiRBNode* Find(int IndexNo, char* Key);
	int Count();

private:
	CRITICAL_SECTION FLock;
	TList	FIndexList;
	TRBTree	*FDataList;
};

/*

FDestinationList.AddIndex('DEST_MSG_HANDLE_INDEX_UNIQ');
FDestinationList.AddIndex('DEST');
FDestinationList.AddIndex('DEST_MSG');

위 각 index 에 TMultiRBTree 를 이용하여 여러 데이터가 달라붙어 있는 형태
*/