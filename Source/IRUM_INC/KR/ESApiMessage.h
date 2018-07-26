//---------------------------------------------------------------------------
// File Name : ESApiMessage.h
// Date      : 2015-11-04
// Creation  : �� ġ��
// Update    : 
// Comment   : User Message�� �⺻���� ����
//---------------------------------------------------------------------------


#ifndef __ESAPIMESSAGE_H__
#define __ESAPIMESSAGE_H__

enum
{
	// API Dll => user program
	WM_ESM_RECVDATA		= WM_USER + 0x8000,				// �������� ���� ������
														// wParam : �ش� tr ��ȣ, lParam : �ش� ������
	WM_ESM_ACCTLIST,									// ���� ����Ʈ
														// wParam : ���� ����, lParam : ���� ����Ʈ�� ����Ʈ
	WM_ESM_FCACCTLIST,									// ���� ����Ʈ(����ĸ)
														// wParam : ���� ����, lParam : ���� ����Ʈ�� ����Ʈ
	WM_ESM_CODELIST,									// ���� ���� ����Ʈ
														// wParam : ���� ����, lParam : ���� ����Ʈ�� ����Ʈ
	WM_ESM_CONNECT,										// ���� �Ϸ� �̺�Ʈ
	WM_ESM_DISCONNECT,									// ���� ���� �̺�Ʈ
};





#endif	// __MESSAGE_H__
