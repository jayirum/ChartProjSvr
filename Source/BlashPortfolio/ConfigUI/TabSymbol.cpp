// TabSymbol.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ConfigUI.h"
#include "TabSymbol.h"
#include "afxdialogex.h"


// CTabSymbol ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTabSymbol, CDialogEx)

CTabSymbol::CTabSymbol(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SYMBOL, pParent)
{

}

CTabSymbol::~CTabSymbol()
{
}

void CTabSymbol::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabSymbol, CDialogEx)
END_MESSAGE_MAP()


// CTabSymbol �޽��� ó�����Դϴ�.
