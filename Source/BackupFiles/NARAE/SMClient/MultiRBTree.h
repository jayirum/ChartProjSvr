#pragma once

#include "RBTree.h"
#include <stdio.h>


typedef int(*TStrSortCompare)(char* Item1, char* Item2);

//enum TMultiColor { clRed, clBlack };


//#define FORMAT_KEY(key,len,out) { sprintf(out, "-*.*s", len, len, key); }

struct  TMultiRBNode
{
	char	Key[MAXLEN_KEY];
	TMultiRBNode *Left, *Right, *Parent;
	TColor Color;
	TRBTree *Duplicates;
};
 


class TMultiRBTree
{
public:
	//TMultiRBTree(int nKeyLen);
	TMultiRBTree();
	virtual ~TMultiRBTree();

	void SetKeyLen(int len);
	void Clear();
	void Delete(TMultiRBNode* z, void* Data);
	TMultiRBNode* Find(char* pKey);
	TMultiRBNode* Add(char* pKey, void* Data);
	void RBInc(TMultiRBNode* x);
	void RBDec(TMultiRBNode* x);


	TMultiRBNode* FirstGet() { return Leftmost; };
	void FirstSet() { return ; };
	TMultiRBNode* LastGet() { return Rightmost; }
	void LastSet() { return ; };

	property(TMultiRBNode*, FirstGet, FirstSet) First;
	property(TMultiRBNode*, LastGet, LastSet) Last;
private:
	void RotateLeft(TMultiRBNode* x);
	void RotateRight(TMultiRBNode* x);
	TMultiRBNode* Minimum(TMultiRBNode* x);
	TMultiRBNode* Maximum(TMultiRBNode* x);

private:
	TMultiRBNode* Root, *Leftmost, *Rightmost;
	TStrSortCompare CompareFunc;
	int FKeyLen;
};

  