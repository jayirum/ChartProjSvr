
// ChartShmViewer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CChartShmViewerApp:
// �� Ŭ������ ������ ���ؼ��� ChartShmViewer.cpp�� �����Ͻʽÿ�.
//

class CChartShmViewerApp : public CWinApp
{
public:
	CChartShmViewerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CChartShmViewerApp theApp;