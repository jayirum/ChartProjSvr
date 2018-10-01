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
		--	버퍼를 검사하여 옳바른 패킷 하나를 꺼낸다.

	retrun value:
		0	: 버퍼가 비었거나, 아직 미완성
		-1	: 오류
*/
int	CPktHandler::GetOnePktFn(char* pOutBuf)
{
	if (!pOutBuf) return -1;

	if (m_strBuf.empty()) {
		return 0;
	}

	/////////////////////////////////////////////////////////////
	//	STX + LEN + ..... + ETX
	//	LEN : 전체 패킷 길이

	char tmp[128];
	int nStx = 0, nEtx = 0;

	//	find stx
	nStx = m_strBuf.find_first_of(0x02); // sequence 상에서의 위치를 반환
	if (nStx == m_strBuf.npos) {
		strcpy(pOutBuf, "버퍼내 STX 가 없습니다.");
		RemoveAll(FALSE);
		return -1;
	}

	// 패킷상의 길이
	sprintf(tmp, "%.*s", SIZE_PACKET_LEN, m_strBuf.c_str() + nStx + 1);
	int nNominalLen = atoi(tmp);

	// 실제길이
	int nRealLen = m_strBuf.size();

	// 실제길이가 짧으면 아직 미완의 패킷이므로 기다린다.
	if (nNominalLen > nRealLen)
		return 0;

	//	find etx
	nEtx = m_strBuf.find_first_of(0x03, nStx);

	//	etx 가 있다.
	if (nEtx != m_strBuf.npos)
	{
		// stx ~ etx 까지의 길이
		int nLen = nEtx - nStx + 1;

		// success //////////////////////////////////////////////////////////////////////
		if (nLen == nNominalLen)
		{
			strncpy(pOutBuf, m_strBuf.c_str() + nStx, nLen);
			m_strBuf.erase(nStx, nLen);
			return nLen;
		}
		// success //////////////////////////////////////////////////////////////////////


		// 패킷상 len 과 실제 stx~etx 까지의 길이 다르면 버린다.
		sprintf(pOutBuf, "[길이오류]패킷상길이(%d) STX-ETX 까지 길이(%d)(%.20s)", nNominalLen, nLen, m_strBuf.c_str());
		m_strBuf.erase(nStx, nLen);
		return -1;
	}

	// ETX 가 없다.

	// 이후 또 STX 가 있으면 그 전까지 버리고 다시 체크할 수 있게 한다.
	nStx = m_strBuf.find_first_of(0x02, nStx + 1);

	//	stx 가 없으면 버퍼를 비운다.(초기화)
	if (nStx == m_strBuf.npos) {
		sprintf(pOutBuf, "1차 etx 없는데, 그 이후 다른 STX 없는 경우");
		RemoveAll(FALSE);
		return -1;
	}

	//	stx 가 있으면 이전의 잘못된 만큼만 지운다.
	m_strBuf.erase(0, nStx);
	sprintf(pOutBuf, "1차 etx 없고, 그 이후 다른 STX 는 있어서 잘못된 패킷만 제거");
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