// TabGeneral.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ConfigUI.h"
#include "TabGeneral.h"
#include "afxdialogex.h"


// CTabGeneral 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabGeneral, CDialogEx)

CTabGeneral::CTabGeneral(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_GENERAL, pParent)
{

}

CTabGeneral::~CTabGeneral()
{
}

void CTabGeneral::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabGeneral, CDialogEx)
END_MESSAGE_MAP()


// CTabGeneral 메시지 처리기입니다.
