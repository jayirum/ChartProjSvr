// TabSymbol.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ConfigUI.h"
#include "TabSymbol.h"
#include "afxdialogex.h"


// CTabSymbol 대화 상자입니다.

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


// CTabSymbol 메시지 처리기입니다.
