
// ListCtrlTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CListCtrlTestApp:
// �� Ŭ������ ������ ���ؼ��� ListCtrlTest.cpp�� �����Ͻʽÿ�.
//

class CListCtrlTestApp : public CWinApp
{
public:
	CListCtrlTestApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CListCtrlTestApp theApp;