
// UITester.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CUITesterApp:
// �� Ŭ������ ������ ���ؼ��� UITester.cpp�� �����Ͻʽÿ�.
//

class CUITesterApp : public CWinApp
{
public:
	CUITesterApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CUITesterApp theApp;