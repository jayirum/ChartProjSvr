#include "MultiRBTree.h"
#include  <string.h>


int CompareStr(char* Item1, char* Item2)
{

	//return (strncmp(Item1, Item2, len));
	return (strcmp(Item1, Item2));
}

//TMultiRBTree::TMultiRBTree(int nKeyLen)
//{
//	CompareFunc = CompareStr;
//	Root = NULL;
//	Leftmost = NULL;
//	Rightmost = NULL;
//	FKeyLen = nKeyLen;
//}


TMultiRBTree::TMultiRBTree()
{
	CompareFunc = CompareStr;
	Root = NULL;
	Leftmost = NULL;
	Rightmost = NULL;
	FKeyLen = 0;
}



TMultiRBTree::~TMultiRBTree()
{
	Clear();
}

void TMultiRBTree::SetKeyLen(int len)
{
	FKeyLen = len;
}


void TMultiRBTree::RBInc(TMultiRBNode* x)
{
	TMultiRBNode* y;

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
			x = y;
	}
}

//{ Pre: x != first }
void TMultiRBTree::RBDec(TMultiRBNode* x)
{
	TMultiRBNode* y;

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

void fast_erase(TMultiRBNode* x)
{
	if (x->Left != NULL)
		fast_erase(x->Left);
	if (x->Right != NULL)
		fast_erase(x->Right);

	delete x;
}

void TMultiRBTree::Clear()
{
	if (Root != NULL)
		fast_erase(Root);
	Root = NULL;
	Leftmost =  NULL;
	Rightmost = NULL;
}

TMultiRBNode* TMultiRBTree::Find(char* pKey)
{
	char Key[MAXLEN_KEY];
	//FORMAT_KEY(pKey, FKeyLen, Key);

	int cmp;
	TMultiRBNode* Result = Root;
	while (Result != NULL)
	{
		//cmp = CompareFunc(Result->Key, Key, FKeyLen);
		cmp = CompareFunc(Result->Key, Key);
		if (cmp < 0) {
		Result = Result->Right;
		}
		else if (cmp > 0) {
			Result = Result->Left;
		}
		else
		{
			break;
		}
	}
	return Result;
}

void TMultiRBTree::RotateLeft(TMultiRBNode* x)
{
	TMultiRBNode* y;

	y = x->Right;
	x->Right = y->Left;

	if (y->Left != NULL)
		y->Left->Parent = x;

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

void TMultiRBTree::RotateRight(TMultiRBNode* x)
{
	TMultiRBNode* y;

	y = x->Left;
	x->Left = y->Right;
	
	if (y->Right != NULL)
		y->Right->Parent = x;
	
	y->Parent = x->Parent;
	if (x = Root) {
		Root = y;
	}
	else if (x = x->Parent->Right) {
		x->Parent->Right = y;
	}
	else {
		x->Parent->Left = y;
	}
	y->Right  = x;
	x->Parent  = y;
}

TMultiRBNode* TMultiRBTree::Minimum(TMultiRBNode* x)
{
	TMultiRBNode* Result = x;
	while (Result->Left != NULL)
		Result = Result->Left;
	return Result;
}

TMultiRBNode* TMultiRBTree::Maximum(TMultiRBNode* x)
{
	TMultiRBNode* Result = x;
	while (Result->Right != NULL)
		Result = Result->Right;

	return Result;
}

TMultiRBNode* TMultiRBTree::Add(char* pKey, void* Data)
{
	char Key[MAXLEN_KEY];
	//FORMAT_KEY(pKey, FKeyLen, Key);

	TMultiRBNode *x, *y, *z, *zpp;
	int cmp;
	TMultiRBNode* Result;

	z = new TMultiRBNode;
	
	//{ Initialize fields in new node z }
	strcpy(z->Key, Key);
	z->Left 	= NULL;
	z->Right 	= NULL;
	z->Color 	= clRed;
	z->Duplicates = new TRBTree();
	z->Duplicates->Add( (void*)Data, (void*)Data);
	Result = z;

	//{ Maintain Leftmost and Rightmost nodes }
	if ((Leftmost == NULL) || (CompareFunc(Key, Leftmost->Key) < 0))
		Leftmost = z;

	if ((Rightmost == NULL) || (CompareFunc(Rightmost->Key, Key) < 0))
		Rightmost = z;

	//{ Insert node z }
	y = NULL;
	x = Root;
	while (x != NULL)
	{
		y = x;
		cmp = CompareFunc(Key, x->Key);
		if (cmp < 0) {
			x = x->Left;
		}
		else if (cmp > 0) {
			x = x->Right;
		}
		else
		{
			//{ Value already exists in tree. }
			x->Duplicates->Add(Data, Data);
			Result = x;

			delete z->Duplicates;
			delete z;
			return Result;
		}
	} // while (x != NULL)

	z->Parent = y;
	if (y = NULL)
	{
		Root = z;
	}
	else if (CompareFunc(Key, y->Key) < 0)
	{
		y->Left = z;
	}
	else
	{
		y->Right = z;
	}

	//{ Rebalance tree }
	while ((z != Root) && (z->Parent->Color = clRed))
	{
		zpp = z->Parent->Parent;
		if (z->Parent == zpp->Left)
		{
			y = zpp->Right;
			if ((y != NULL) && (y->Color = clRed))
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
		} // if (z->Parent == zpp->Left)	
		else
		{
			y = zpp->Left;
			if ((y != NULL) && (y->Color == clRed))
			{
				z->Parent->Color = clBlack;
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
	}// end of while ((z != Root) && (z->Parent->Color = clRed))
	Root->Color  = clBlack;

}

void TMultiRBTree::Delete(TMultiRBNode* Z, void* Data)
{
	TMultiRBNode *w, *x, *y, *x_parent;
	TColor tmpcol;

	if (Z == NULL)
		return;

	if (Z->Duplicates->Count > 1)
	{
		Z->Duplicates->Delete(Z->Duplicates->Find(Data));
		return;
	}

	y = Z;
	x = NULL;
	x_parent = NULL;

	if (y->Left == NULL)
	{
		//{ z has at most one non-null child. y = z. }
		x = y->Right;     //{ x might be null. }
	}
	else
	{
		if (y->Right == NULL)
		{
			//{ z has exactly one non-null child. y = z. }
			x = y->Left;    //{ x is not null. }
		}
		else
		{
			//{ z has two non-null children.  Set y to }
			y = y->Right;   //{   z's successor.  x might be null. }
			while (y->Left != NULL) {
				y = y->Left;
			}
		}
		x = y->Right;
	}


	if (y != Z)
	{
		//{ "copy y's sattelite data into z" }
		//{ relink y in place of z.  y is z's successor }
		Z->Left->Parent = y;
		y->Left = Z->Left;

		if (y != Z->Right)
		{
			x_parent = y->Parent;
			if (x != NULL)
			{
				x->Parent = y->Parent;
			}
			y->Parent->Left = x; //{ y must be a child of Left }
			y->Right = Z->Right;
			Z->Right->Parent = y;
		}
		else
		{
			x_parent = y;
		}

		if (Root == Z) {
			Root = y;
		}
		else if (Z->Parent->Left == Z) {
			Z->Parent->Left = y;
		}
		else {
			Z->Parent->Right = y;
		}
		y->Parent = Z->Parent;
		tmpcol = y->Color;
		y->Color = Z->Color;
		Z->Color = tmpcol;
		y = Z;

		//{ y now points to node to be actually deleted }
	} // if (y != Z)
	else
	{                        //{ y = z }
		x_parent = y->Parent;
		if (x != NULL)
		{
			x->Parent = y->Parent;
		}

		if (Root == Z)
		{
			Root = x;
		}
		else
		{
			if (Z->Parent->Left == Z)
			{
				Z->Parent->Left = x;
			}
			else
			{
				Z->Parent->Right = x;
			}
		}

		if (Leftmost == Z)
		{
			if (Z->Right == NULL)
			{	      //{ z->Left must be null also }
				Leftmost = Z->Parent;
			}
			else
			{
				Leftmost = Minimum(x);
			}
		}
		if (Rightmost == Z)
		{
			if (Z->Left == NULL)
			{       //{ z->Right must be null also }
				Rightmost = Z->Parent;
			}
			else
			{                     //{ x == z->Left }
				Rightmost = Maximum(x);
			}
		}
	}// else of if (y != Z)

	//{ Rebalance tree }
	if (y->Color == clBlack)
	{
		while ((x != Root) && ((x == NULL) || (x->Color == clBlack)))
		{
			if (x == x_parent->Left)
			{
				w = x_parent->Right;
				if (w->Color = clRed)
				{
					w->Color = clBlack;
					x_parent->Color = clRed;
					RotateLeft(x_parent);
					w = x_parent->Right;
				}
				if (((w->Left == NULL) || (w->Left->Color == clBlack)) && ((w->Right = NULL) || (w->Right->Color == clBlack)))
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
				//{ same as above, with Right <-> Left. }
				w = x_parent->Left;
				if (w->Color == clRed)
				{
					w->Color = clBlack;
					x_parent->Color = clRed;
					RotateRight(x_parent);
					w = x_parent->Left;
				}
				if (((w->Right == NULL) || (w->Right->Color == clBlack)) && ((w->Left == NULL) || (w->Left->Color = clBlack)))
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
			}// else of if (x == x_parent->Left)

		} // while ((x != Root) && ((x == NULL) || (x->Color == clBlack)))
		if (x != NULL)
		{
			x->Color = clBlack;
		}
	} // if (y->Color == clBlack)
	delete y->Duplicates;
	delete y;
}
