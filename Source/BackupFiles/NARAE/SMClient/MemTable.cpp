#include "MemTable.h"


TBaseMemTable::TBaseMemTable()
{
	FDataList = new TRBTree;
	InitializeCriticalSectionAndSpinCount(&FLock,2000);
}

TBaseMemTable::~TBaseMemTable()
{
	TListP itList;
	for (itList = FIndexList.begin(); itList != FIndexList.end(); itList++)
	{
		LIST_DATA* p = (*itList).second;
		delete p->mTree;
		delete p;
	}
	FIndexList.clear();

	TRBNode* IT = FDataList->First;
	if (IT != NULL)
	{
		while (IT != FDataList->Last)
		{
			DataFree(IT->Key);
			FDataList->RBInc(IT);
		}
		DataFree(IT->Key);
	}
	delete FDataList;
	DeleteCriticalSection(&FLock);
}

void TBaseMemTable::AddIndex(char* IndexName)
{
	LIST_DATA* p = new LIST_DATA;
	strcpy(p->sIndex, IndexName);
	p->nIndexLen = strlen(IndexName);
	p->mTree = new TMultiRBTree();

	int nIdx = FIndexList.size();
	FIndexList[nIdx] =  p ;
}

void TBaseMemTable::Delete(void *Data)
{
	int I;
	TRBNode* IT;
	char szKey[MAXLEN_KEY] = { 0, };
	TListP itList;
	for (itList = FIndexList.begin(); itList != FIndexList.end(); itList++)
	{
		I = (*itList).first;
		GetKey(I, Data, szKey);

		TMultiRBTree* p = ((*itList).second)->mTree;
		p->Delete(p->Find(szKey), Data);
		//TMultiRBTree(FIndexList.Items[I]).Delete(TMultiRBTree(FIndexList.Items[I]).Find(GetKey(I, Data)), Data);
	}
	IT = FDataList->Find(Data);
	DataFree(IT->Key);
	FDataList->Delete(IT);
}

void TBaseMemTable::Edit(void* OldData, void* NewData)
{
}


TMultiRBNode* TBaseMemTable::Find(int IndexNo, char* Key)
{
	TMultiRBNode* 	Result;
	TListP it = FIndexList.find(IndexNo);
	if (it == FIndexList.end())
		return NULL;

	Result = ((*it).second)->mTree->Find(Key);
	return Result;
}

void TBaseMemTable::Insert(void* Data)
{
	int I;

	FDataList->Add(Data, Data);

	char szKey[MAXLEN_KEY] = { 0, };
	TListP itList;
	for (itList = FIndexList.begin(); itList != FIndexList.end(); itList++)
	{
		I = (*itList).first;
		GetKey(I, Data, szKey);

		TMultiRBTree* p = ((*itList).second)->mTree;
		p->Add(szKey, Data);
		//TMultiRBTree(FIndexList.Items[I]).Add(GetKey(I, Data), Data);
	}

	//for (I = 0; FIndexList.size() - 1; I++)
	//{
	//	GetKey(I, Data, szKey);
	//	TMultiRBTree(FIndexList.Items[I]).Add(GetKey(I, Data), Data);
	//}
}

void TBaseMemTable::Clear()
{
	TListP itList;
	for (itList = FIndexList.begin(); itList != FIndexList.end(); itList++)
	{
		LIST_DATA *p = (*itList).second;
		delete p->mTree;
		delete p;
	}

	TRBNode* IT = FDataList->First;
	if (IT != NULL)
	{
		while (IT != FDataList->Last)
		{
			DataFree(IT->Key);
			FDataList->RBInc(IT);
		}
		DataFree(IT->Key);
	}
	FDataList->Clear();
}

int TBaseMemTable::Count()
{
	return FDataList->Count;
}

TRBTree* TBaseMemTable::GetDataList()
{
	return FDataList;
}

void TBaseMemTable::Lock()
{
	EnterCriticalSection(&FLock);
}

void TBaseMemTable::UnLock()
{
	LeaveCriticalSection(&FLock);
}