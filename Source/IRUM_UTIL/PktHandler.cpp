// PktHandle.cpp: implementation of the CPktHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "PktHandler.h"
#include "../IRUM_INC/IRUM_Common.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPktHandler::CPktHandler()
{
	InitializeCriticalSectionAndSpinCount(&m_cs, 2000);
}

CPktHandler::~CPktHandler()
{
	RemoveAll(TRUE);
	DeleteCriticalSection(&m_cs);
}

BOOL CPktHandler::AddPkt(char* pBuf, int nSize)
{
	if(!pBuf)
		return FALSE;

	try{
		Lock();
		m_strBuf += string(pBuf, nSize);
	}
	catch(...)
	{
		Unlock();
		strcpy(pBuf, "Add Pkt try catch error");
		return FALSE;
	}

	Unlock();
	return TRUE;
}


int	CPktHandler::GetOnePkt(char* pOutBuf)
{
	if (!pOutBuf) return -1;

	Lock();
	int nRet = GetOnePktFn(pOutBuf);
	Unlock();

	return nRet;
}


/*
	Note:
		--	���۸� �˻��Ͽ� �ǹٸ� ��Ŷ �ϳ��� ������.

	retrun value:
		0	: ���۰� ����ų�, ���� �̿ϼ�
		-1	: ����
*/
int	CPktHandler::GetOnePktFn(char* pOutBuf)
{
	if (!pOutBuf) return -1;

	if (m_strBuf.empty()) {
		return 0;
	}

	/////////////////////////////////////////////////////////////
	//	STX + LEN + ..... + ETX
	//	LEN : ��ü ��Ŷ ����

	char tmp[128];
	int nStx = 0, nEtx = 0;

	//	find stx
	nStx = m_strBuf.find_first_of(0x02); // sequence �󿡼��� ��ġ�� ��ȯ
	if (nStx == m_strBuf.npos) {
		strcpy(pOutBuf, "���۳� STX �� �����ϴ�.");
		RemoveAll(FALSE);
		return -1;
	}

	// ��Ŷ���� ����
	sprintf(tmp, "%.*s", SIZE_PACKET_LEN, m_strBuf.c_str() + nStx + 1);
	int nNominalLen = atoi(tmp);

	// ��������
	int nRealLen = m_strBuf.size();

	// �������̰� ª���� ���� �̿��� ��Ŷ�̹Ƿ� ��ٸ���.
	if (nNominalLen > nRealLen)
		return 0;

	//	find etx
	nEtx = m_strBuf.find_first_of(0x03, nStx);

	//	etx �� �ִ�.
	if (nEtx != m_strBuf.npos)
	{
		// stx ~ etx ������ ����
		int nLen = nEtx - nStx + 1;

		// success //////////////////////////////////////////////////////////////////////
		if (nLen == nNominalLen)
		{
			strncpy(pOutBuf, m_strBuf.c_str() + nStx, nLen);
			m_strBuf.erase(nStx, nLen);
			return nLen;
		}
		// success //////////////////////////////////////////////////////////////////////


		// ��Ŷ�� len �� ���� stx~etx ������ ���� �ٸ��� ������.
		sprintf(pOutBuf, "[���̿���]��Ŷ�����(%d) STX-ETX ���� ����(%d)(%.20s)", nNominalLen, nLen, m_strBuf.c_str());
		m_strBuf.erase(nStx, nLen);
		return -1;
	}

	// ETX �� ����.

	// ���� �� STX �� ������ �� ������ ������ �ٽ� üũ�� �� �ְ� �Ѵ�.
	nStx = m_strBuf.find_first_of(0x02, nStx + 1);

	//	stx �� ������ ���۸� ����.(�ʱ�ȭ)
	if (nStx == m_strBuf.npos) {
		sprintf(pOutBuf, "1�� etx ���µ�, �� ���� �ٸ� STX ���� ���");
		RemoveAll(FALSE);
		return -1;
	}

	//	stx �� ������ ������ �߸��� ��ŭ�� �����.
	m_strBuf.erase(0, nStx);
	sprintf(pOutBuf, "1�� etx ����, �� ���� �ٸ� STX �� �־ �߸��� ��Ŷ�� ����");
	return -1;


}

VOID CPktHandler::RemoveAll(BOOL bLock)
{
	if (bLock)	Lock();
	m_strBuf.erase(m_strBuf.begin(), m_strBuf.end());
	if (bLock)	Unlock();
}


LPCSTR CPktHandler::GetStrBuff()
{
	return (LPCSTR)m_strBuf.c_str();
}


VOID  CPktHandler::Erase(int nStartPos, int nLen)
{
	m_strBuf.erase( nStartPos, nLen );

}