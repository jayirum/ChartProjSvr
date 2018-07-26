#ifndef _RBTREE_HEADER_20170424
#define _RBTREE_HEADER_20170424
/*
[KEY]
if Index = 0 then
Result := StrPas(PDestinationRecord(Data)^.Destination) + ';' + StrPas(PDestinationRecord(Data)^.Msg) + ';' +
IntTostr(PDestinationRecord(Data)^.RcvHandle)
else if Index = 1 then
Result := PDestinationRecord(Data)^.Destination
else if Index = 2 then
Result := StrPas(PDestinationRecord(Data)^.Destination) + ';' + StrPas(PDestinationRecord(Data)^.Msg);

	[DATA]
	PDestinationRecord = ^TDestinationRecord;
	TDestinationRecord = Record
	Destination: PChar;
	Msg: PChar;
	RcvHandle: HWND;
	end;

의 형태가 보관된다.
*/

#include "Common.h"
#include "../../IRUM_UTIL/Util.h"

typedef int(*TListSortCompare)(void* Item1, void* Item2);

//enum TColor { clRed, clBlack };


struct TRBNode
{
	void*	Key;	//: Pointer;
	TRBNode* Left;
	TRBNode*	Right;
	TRBNode*	Parent;
	TColor	Color;
	void*	Data;
};

class TRBTree
{

public:
	//TRBTree(int nKeyLen);
	TRBTree();
	virtual ~TRBTree();

	void Clear();
	void SetKeyLen(int len);
	TRBNode* Find(void* Key) ;
	TRBNode* Add(void* Key, void* Data);
	void Delete(TRBNode* z);
	void RBInc(TRBNode* x);
	void RBDec(TRBNode* x);


	TRBNode* FirstGet() { return Leftmost; };
	void FirstSet(TRBNode* p) { return; };
	TRBNode* LastGet() { return Rightmost; };
	void LastSet() { return ; };
	int CountGet() { return FCount; };
	void CountSet() { return ; };

	property(TRBNode*, FirstGet, FirstSet) First;
	property(TRBNode*, LastGet, LastSet) Last;
	property(INT, CountGet, CountSet) Count;

protected:
	void RotateLeft(TRBNode* x);
	void RotateRight(TRBNode* x);
	TRBNode* Minimum(TRBNode* x);
	TRBNode* Maximum(TRBNode* x);
	void fast_erase(TRBNode* x);


private:
	int FKeyLen;
	int FCount;
	TRBNode* Root, *Leftmost, *Rightmost;
	TListSortCompare	CompareFunc;
};

#endif
