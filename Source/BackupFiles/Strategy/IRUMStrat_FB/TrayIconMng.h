// TrayIconMng.h: interface for the CTrayIconMng class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afxwin.h>

class CTrayIconMng  
{
public:
	bool m_bHide;
	bool m_bAdded;
	CTrayIconMng();
	virtual ~CTrayIconMng();

	void MakePopupMenu(HWND hWnd, int x, int y);
	// �˾��޴����̺�Ʈ�߻���ó���Լ�
	void ProcTrayMsg(HWND hWnd, WPARAM wParam, LPARAM lParam);
	// Ʈ���̾����ܻ���
	bool AddTrayIcon(HWND hWnd);
	// Ʈ���̾���������
	bool DelTrayIcon(HWND hWnd);

	#define WM_DIALOG_SHOW	WM_USER + 101	
//���̾�αװ��߱�/���̱�
#define WM_APP_EXIT		WM_USER + 102	
//���̾�α�����
#define WM_TRAYICON		WM_USER + 103	
//Ʈ���̾����ܸ޽���
};
