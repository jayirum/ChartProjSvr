// TrayIconMng.h: interface for the CTrayIconMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAYICONMNG_H__09F1381B_5BA8_470F_BA7A_C8CBC1F2BB86__INCLUDED_)
#define AFX_TRAYICONMNG_H__09F1381B_5BA8_470F_BA7A_C8CBC1F2BB86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4996)

class CTrayIconMng  
{
public:
	bool m_bHide;
	bool m_bAdded;
	CTrayIconMng();
	virtual ~CTrayIconMng();

	void MakePopupMenu(HWND hWnd, int x, int y);
	// 팝업메뉴의이벤트발생시처리함수
	void ProcTrayMsg(HWND hWnd, WPARAM wParam, LPARAM lParam);
	// 트레이아이콘생성
	bool AddTrayIcon(HWND hWnd);
	// 트레이아이콘제거
	bool DelTrayIcon(HWND hWnd);

	#define WM_DIALOG_SHOW	WM_USER + 101	
//다이얼로그감추기/보이기
#define WM_APP_EXIT		WM_USER + 102	
//다이얼로그종료
#define WM_TRAYICON		WM_USER + 103	
//트레이아이콘메시지
};

#endif // !defined(AFX_TRAYICONMNG_H__09F1381B_5BA8_470F_BA7A_C8CBC1F2BB86__INCLUDED_)
