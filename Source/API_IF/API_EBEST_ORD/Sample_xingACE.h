
// Sample_xingACE.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSample_xingACEApp:
// �� Ŭ������ ������ ���ؼ��� Sample_xingACE.cpp�� �����Ͻʽÿ�.
//

class CSample_xingACEApp : public CWinApp
{
public:
	CSample_xingACEApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CSample_xingACEApp theApp;