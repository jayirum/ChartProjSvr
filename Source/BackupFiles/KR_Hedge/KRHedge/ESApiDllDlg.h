
// ESApiDllDlg.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

#define TRAY_STRING "KR Hedge"
// CESApiDllDlgApp:
// �� Ŭ������ ������ ���ؼ��� ESApiDllDlg.cpp�� �����Ͻʽÿ�.
//

class CESApiDllDlgApp : public CWinApp
{
public:
	CESApiDllDlgApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CESApiDllDlgApp theApp;