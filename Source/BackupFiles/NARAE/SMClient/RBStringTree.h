#pragma once
#include <stdio.h>
#include "Common.h"
#include "../../IRUM_UTIL/Util.h"
typedef int(*TStrSortCompare)(char* Item1, char* Item2);



struct TRBStringNode
{
	char Key[MAXLEN_KEY];
	TRBStringNode *Left;
	TRBStringNode *Right;
	TRBStringNode *Parent;
	TColor Color;
	void* Data;
};

class TRBStringTree
{
public:
	TRBStringTree();
	virtual ~TRBStringTree();

	void Clear();
	TRBStringNode* Find(char* Key);
	TRBStringNode* Add(char* Key, void* Data);

	void Delete(TRBStringNode* z);
	void RBInc(TRBStringNode* x);
	void RBDec(TRBStringNode* x);
public:
	TRBStringNode* FirstGet() { return Leftmost; };
	void FirstSet() { return ; };
	TRBStringNode* LastGet() { return Rightmost; };
	void LastSet() { return ; };
	int CountGet() { return FCount; };
	void CountSet() { return ; };

	property(TRBStringNode*, FirstGet, FirstSet) First;
	property(TRBStringNode*, LastGet, LastSet) Last;
	property(INT, CountGet, CountSet) Count;
private:
	void RotateLeft(TRBStringNode* x);
	void RotateRight(TRBStringNode* x);
	TRBStringNode* Minimum(TRBStringNode* x);
	TRBStringNode* Maximum(TRBStringNode* x);
	void fast_erase(TRBStringNode* x);
private:
	int FCount;
	TRBStringNode *Root, *Leftmost, *Rightmost;
	//CompareFunc: TListSortCompare;
};
