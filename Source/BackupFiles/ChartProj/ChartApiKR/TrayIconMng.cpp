// TrayIconMng.cpp: implementation of the CTrayIconMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESApiDllDlg.h"
#include "TrayIconMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrayIconMng::CTrayIconMng()
{

}

CTrayIconMng::~CTrayIconMng()
{

}

void CTrayIconMng::MakePopupMenu(HWND hWnd,int x, int y)
{
	HMENU hMenu = CreatePopupMenu();
	if(m_bHide)
	{
		AppendMenu(hMenu, MF_STRING, WM_DIALOG_SHOW,"���̾�α׺��̱�");
	}
	else
	{
		AppendMenu(hMenu, MF_STRING, WM_DIALOG_SHOW,"���̾�αװ��߱�");
	}
	AppendMenu(hMenu, MF_STRING, WM_APP_EXIT,"����");
	SetForegroundWindow(hWnd);

	TrackPopupMenu(hMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON, x,y,0,hWnd,NULL);
}

void CTrayIconMng::ProcTrayMsg(HWND hWnd,WPARAM wParam,LPARAM IParam)
{
	HMENU hMenu = NULL;
	POINT pos;

	if(IParam == WM_RBUTTONDOWN)
	{
		GetCursorPos(&pos);
		MakePopupMenu(hWnd, pos.x,pos.y);
	}

}

bool CTrayIconMng::AddTrayIcon(HWND hWnd, char* pzAppName)

{

 if(m_bAdded)           //�̹�Ʈ���̾��������ִٸ�����

 {return FALSE;}

 

 NOTIFYICONDATA nid;    //�������������Ͽ�����

    ZeroMemory(&nid, sizeof(NOTIFYICONDATA));

    nid.cbSize = sizeof(NOTIFYICONDATA);

    nid.hWnd = hWnd;

    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

    nid.uCallbackMessage = WM_TRAYICON;

    sprintf((char*)nid.szTip, (char*)_T(pzAppName));

    nid.uID = 0;

    nid.hIcon = theApp.LoadIcon(IDR_MAINFRAME);




    if(Shell_NotifyIcon(NIM_ADD, &nid)==0) //Ʈ���̾�����ǥ��

 {return FALSE;}




 m_bAdded = true;       

    return TRUE;

}




bool CTrayIconMng::DelTrayIcon(HWND hWnd)

{

 NOTIFYICONDATA nid;

    ZeroMemory(&nid, sizeof(NOTIFYICONDATA));

    nid.cbSize = sizeof(NOTIFYICONDATA);

    nid.hWnd = hWnd;

    nid.uFlags = NULL;




 if(Shell_NotifyIcon(NIM_DELETE, &nid)==0) //Ʈ���̾����ܻ���

 {

        return FALSE;

 }

    return TRUE;

}

