// ConfigUI.h : ConfigUI DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CConfigUIApp
// �� Ŭ������ ������ ������ ConfigUI.cpp�� �����Ͻʽÿ�.
//

class CConfigUIApp : public CWinApp
{
public:
	CConfigUIApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
