
// HDFCommClient.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CHDFCommClientApp:
// �� Ŭ������ ������ ���ؼ��� HDFCommClient.cpp�� �����Ͻʽÿ�.
//

class CHDFCommClientApp : public CWinApp
{
public:
	CHDFCommClientApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	int RegisterOCX(CString strFileName);
	BOOL RegisterWindowClass();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CHDFCommClientApp theApp;

const TCHAR CLASSNAME_APPINSTANCE[] = _T("_HDFClass");