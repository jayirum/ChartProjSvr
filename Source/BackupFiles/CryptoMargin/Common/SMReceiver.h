#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include "SMParsing.h"
#include <list>
using namespace std;


class CSMReceiver
{
public:
	CSMReceiver();
	virtual ~CSMReceiver();

	VOID	SetDestination(char* psDest);
	VOID	SetDelieveryMode(TDeliveryMode mode);
	VOID	SetMsg(char* psMsgKey, char* psMsg);
	VOID	SetData(char* psDataKey, char* pData, FIELD_TYPE DataType, int nDataSize);

	char*	GetSendData(_Out_ int *pnSendSize);
	char*	GetErrMsg() { return m_zErrMsg; }
private:
	char*	GetSendDataFn(_Out_ int *pnSendSize);
	int GetByteSize(int nLen);
	int CalcDataSize(char* pKey, FIELD_TYPE type, char* pData, int nDataSize);
	int SetFieldData_Msg(int iPos, char* Buffer, char* pKey, char* pData);
	int SetFieldData_Data(int iPos, char* Buffer, char* pKey, char* pData, int nDataSize);

private:

	// ������ �ΰ�
	HANDLE	m_hRThread;			// �ü�����
	HANDLE	m_hPThread;			// CSMessage �� ��Ŷ �Ľ� ������
	DWORD	m_dwRThread, m_dwPThread;
	CSMParsing	m_parse;
	list<CSMessage*>	m_lstMsg;
	SOCKET	m_sockClient;

	char*	m_pSendBuff;
	char*	m_pData;
	char	m_zDestination[128];
	char	m_zMsgKey[128], m_zMsgData[128];
	char	m_zDataKey[128];
	FIELD_TYPE	m_DataType;
	int			m_nDataSize;
	int			m_nSendSize;
	TR_CODE		m_Tr;

	char	m_zErrMsg[1024];
	char Buffer[1024];
};