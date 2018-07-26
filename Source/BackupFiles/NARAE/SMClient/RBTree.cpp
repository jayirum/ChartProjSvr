
#include "RBTree.h"
#include <windows.h>

int ComparePointer(void* Item1, void* Item2)
{
	int Result;
	if (DWORD(Item1) < DWORD(Item2))
		Result = -1;
	else if (Item1 == Item2)
		Result = 0;
	else
		Result = 1;
	
	return Result;
}



//TRBTree::TRBTree(int nKeyLen)
//{
//	FKeyLen = nKeyLen;
//	CompareFunc = ComparePointer;
//	Root = NULL;
//	Leftmost = NULL;
//	Rightmost = NULL;
//}


TRBTree::TRBTree()
{
	FKeyLen = 0;
	CompareFunc = ComparePointer;
	Root = NULL;
	Leftmost = NULL;
	Rightmost = NULL;
}


TRBTree::~TRBTree()
{
	Clear();
}


void TRBTree::SetKeyLen(int len)
{
	FKeyLen = len;
}

void TRBTree::RBInc(TRBNode* x)
{
	TRBNode* y;

	if (x->Right != NULL)
	{
		x = x->Right;
		while (x->Left != NULL)
		{
			x = x->Left;
		}
	}
	else
	{
		y = x->Parent;
		while (x == y->Right)
		{
			x = y;
			y = y->Parent;
		}

		if (x->Right != y)
		{
			x = y;
		}
	}
}

void TRBTree::RBDec(TRBNode* x)
{
	TRBNode* y;


	if (x->Left != NULL)
	{
		y = x->Left;
		while (y->Right != NULL)
		{
			y = y->Right;
		}
		x = y;
	}
	else
	{
		y = x->Parent;
		while (x == y->Left)
		{
			x = y;
			y = y->Parent;
		}
		x = y;
	}
}

void TRBTree::fast_erase(TRBNode* x)
{
	if (x->Left != NULL)
		fast_erase(x->Left);
	if (x->Right != NULL)
		fast_erase(x->Right);
	delete x;
}

void TRBTree::Clear()
{
	if (Root != NULL)
		fast_erase(Root);
	Root = NULL;
	Leftmost = NULL;
	Rightmost = NULL;
	FCount = 0;
}

TRBNode* TRBTree::Find(void* Key)
{
	int cmp;

	TRBNode* Result = Root;
	while (Result != NULL)
	{
		cmp = CompareFunc(Result->Key, Key);
		if (cmp < 0) {
			Result = Result->Right;
		}
		else if (cmp > 0) {
			Result = Result->Left;
		}
		else
			break;
	}

	return Result;
}

void TRBTree::RotateLeft(TRBNode* x)
{
	TRBNode* y;

	y = x->Right;
	x->Right = y->Left;

	if (y->Left != NULL)
	{
		y->Left->Parent = x;
	}

	y->Parent = x->Parent;
	if (x == Root) {
		Root = y;
	}
	else if (x == x->Parent->Left) {
		x->Parent->Left = y;
	}
	else {
		x->Parent->Right = y;
	}
	y->Left = x;
	x->Parent = y;
}

void TRBTree::RotateRight(TRBNode* x)
{
	TRBNode* y;

	y = x->Left;
	x->Left = y->Right;
	if (y->Right != NULL) {
		y->Right->Parent = x;
	}

	y->Parent = x->Parent;
	if (x == Root) {
		Root = y;
	}
	else if (x == x->Parent->Right) {
		x->Parent->Right = y;
	}
	else {
		x->Parent->Left = y;
	}

	y->Right = x;
	x->Parent = y;
}

TRBNode* TRBTree::Minimum(TRBNode* x)
{
	TRBNode* Result = x;
	while (Result->Left != NULL)
		Result = Result->Left;

	return Result;
}

TRBNode* TRBTree::Maximum(TRBNode* x)
{
	TRBNode* Result = x;
	while (Result->Right != NULL)
		Result = Result->Right;
	return Result;
}

TRBNode* TRBTree::Add(void* key, void* Data)
{
	TRBNode *x, *y, *z, *zpp, *Result;
	int cmp;

	z = new TRBNode;
	// Initialize fields in new node z }
	z->Key = key;
	z->Left = NULL;
	z->Right = NULL;
	z->Color = clRed;
	z->Data = Data;

	FCount++;
	Result = z;
	
	//{ Maintain leftmost and rightmost nodes }
	if ((Leftmost == NULL) || (CompareFunc(key, Leftmost->Key) < 0))
	{
		Leftmost = z;
	}

	if ((Rightmost == NULL) || (CompareFunc(Rightmost->Key, key) < 0))
	{
		Rightmost = z;
	}

	//{ Insert node z }
	y = NULL;
	x = Root;
	while (x != NULL)
	{
		y = x;
		cmp = CompareFunc(key, x->Key);
		if (cmp < 0) {
			x = x->Left;
		}
		else if (cmp > 0) {
			x = x->Right;
		}
		else {
			// Value already exists in tree. }
			Result = x;
			FCount--;
			delete z;
			return Result;
		}
	}


	z->Parent = y;
	if (y == NULL) {
		Root = z;
	}
	else if (CompareFunc(key, y->Key) < 0) {
		y->Left = z;
	}
	else {
		y->Right = z;
	}

	//{ Rebalance tree }
	while ((z != Root) && (z->Parent->Color == clRed))
	{
		zpp = z->Parent->Parent;
		if (z->Parent == zpp->Left)
		{
			y = zpp->Right;
			if ((y != NULL) && (y->Color == clRed))
			{
				z->Parent->Color = clBlack;
				y->Color = clBlack;
				zpp->Color = clRed;
				z = zpp;
			}
			else
			{
				if (z == z->Parent->Right)
				{
					z = z->Parent;
					RotateLeft(z);
				}
				z->Parent->Color = clBlack;
				zpp->Color = clRed;
				RotateRight(zpp);
			}
		} // if (z->Parent = zpp->Left) 
		else
		{
			y = zpp->Left;
			if ((y != NULL) && (y->Color == clRed))
			{
				z->Parent->Color  = clBlack;
				y->Color = clBlack;
				zpp->Color = clRed;
				z = zpp;
			}
			else
			{
				if (z == z->Parent->Left) 
				{
					z = z->Parent;
					RotateRight(z);
				}
				z->Parent->Color = clBlack;
				zpp->Color = clRed;
				RotateLeft(zpp);
			}
		}
	} // while ((z != Root) && (z->Parent->Color == clRed))

	Root->Color = clBlack;
}

void TRBTree::Delete(TRBNode* z)
{
	TRBNode* w, *x, *y, *x_parent;
	TColor tmpcol;

	if (z == NULL)
		return;

	y = z;
	x = NULL;
	x_parent = NULL;

	if (y->Left == NULL)
	{
		//{ z has at most one non - null child.y = z. }
		x = y->Right; //{ x might be null. }
	}
	else
	{
		if (y->Right == NULL)
		{
			//{ z has exactly one non - null child.y = z. }
			x = y->Left; //{ x is not null. }
		}
		else
		{
			//{ z has two non - null children.Set y to }
			y = y->Right; //{   z's successor.  x might be null. }
			while (y->Left != NULL)
			{
				y = y->Left;
			}
			x = y->Right;
		}
	}

	if (y != z)
	{
		//{ "copy y's sattelite data into z" }
		//{ relink y in place of z.y is z's successor }
		z->Left->Parent = y;
		y->Left = z->Left;

		if (y != z->Right)
		{
			x_parent = y->Parent;
			if (x != NULL)
				x->Parent = y->Parent;

			y->Parent->Left = x; //{ y must be a child of left }
			y->Right = z->Right;
			z->Right->Parent = y;
		}
		else {
			x_parent = y;
		}

		if (Root == z) {
			Root = y;
		}
		else if (z->Parent->Left == z) {
			z->Parent->Left = y;
		}
		else
		{
			z->Parent->Right = y;
		}
		y->Parent = z->Parent;
		tmpcol = y->Color;
		y->Color = z->Color;
		z->Color = tmpcol;
		y = z;
		//{ y now points to node to be actually deleted }

	} // if (y != z)
	else
	{
		// y = z }
		x_parent = y->Parent;
		if (x != NULL)
		{
			x->Parent = y->Parent;
		}

		if (Root == z)
		{
			Root = x;
		}
		else
		{
			if (z->Parent->Left == z)
			{
				z->Parent->Left = x;
			}
			else
			{
				z->Parent->Right = x;
			}
		}

		if (Leftmost == z)
		{
			if (z->Right == NULL)
			{   //{ z->left must be null also }
				Leftmost = z->Parent;
			}
			else
			{
				Leftmost = Minimum(x);
			}
		}

		if (Rightmost == z)
		{
			if (z->Left == NULL)
			{
				//{ z->right must be null also }
				Rightmost = z->Parent;
			}
			else
			{
				//{ x == z->left }
				Rightmost = Maximum(x);
			}
		}
	} // else of if (y != z)

	//{ Rebalance tree }
	if (y->Color == clBlack)
	{
		while ((x != Root) && ((x == NULL) || (x->Color == clBlack)))
		{
			if (x == x_parent->Left)
			{
				w = x_parent->Right;
				if (w->Color == clRed)
				{
					w->Color = clBlack;
					x_parent->Color = clRed;
					RotateLeft(x_parent);
					w = x_parent->Right;
				}
				if (((w->Left == NULL) || (w->Left->Color == clBlack)) && ((w->Right == NULL) || (w->Right->Color == clBlack)))
				{
					w->Color = clRed;
					x = x_parent;
					x_parent = x_parent->Parent;
				}
				else
				{
					if ((w->Right == NULL) || (w->Right->Color == clBlack))
					{
						w->Left->Color = clBlack;
						w->Color = clRed;
						RotateRight(w);
						w = x_parent->Right;
					}
					w->Color = x_parent->Color;
					x_parent->Color = clBlack;
					if (w->Right != NULL)
					{
						w->Right->Color = clBlack;
					}
					RotateLeft(x_parent);
					x = Root; //{ break; }
				}
			} // if (x == x_parent->Left)
			else
			{
				//{ same as above, with right <->left. }
				w = x_parent->Left;
				if (w->Color == clRed)
				{
					w->Color = clBlack;
					x_parent->Color = clRed;
					RotateRight(x_parent);
					w = x_parent->Left;
				}
				if (((w->Right == NULL) || (w->Right->Color == clBlack)) && ((w->Left == NULL) || (w->Left->Color == clBlack)))
				{
					w->Color = clRed;
					x = x_parent;
					x_parent = x_parent->Parent;
				}
				else
				{
					if ((w->Left == NULL) || (w->Left->Color == clBlack))
					{
						w->Right->Color = clBlack;
						w->Color = clRed;
						RotateLeft(w);
						w = x_parent->Left;
					}
					w->Color = x_parent->Color;
					x_parent->Color = clBlack;
					if (w->Left != NULL)
					{
						w->Left->Color = clBlack;
					}
					RotateRight(x_parent);
					x = Root; //{ break; }
				}
			} // end of else

		} // while ((x != Root) && ((x == NULL) || (x->Color == clBlack)))

		if (x != NULL)
		{
			x->Color = clBlack;
		}
	} // if (y->Color == clBlack)

	FCount--;
	delete y;
}