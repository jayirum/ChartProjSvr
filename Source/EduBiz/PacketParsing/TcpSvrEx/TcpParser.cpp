
#include "TcpParser.h"
#include "../Common/UtilFunc.h"

#pragma warning(disable:4996)

CTcpParser::CTcpParser()
{
	m_Start = 0x00;
	m_End = 0x00;
	m_nSizeOfLenField = -1;
	InitializeCriticalSectionAndSpinCount(&m_cs, 2000);
}

CTcpParser::~CTcpParser()
{
	End();
}

VOID CTcpParser::End()
{
	RemoveAll(TRUE);
	DeleteCriticalSection(&m_cs);
}


VOID CTcpParser::Begin(int nSizeOfLenField, char cStart, char cEnd )
{
	m_Start = cStart;
	m_End = cEnd;
	m_nSizeOfLenField = nSizeOfLenField;
}


BOOL CTcpParser::AddTcpData(char* pBuf, int nSize)
{
	if (m_nSizeOfLenField == -1)
	{
		sprintf(m_zMsg, "You should call Begin() at first");
		return FALSE;
	}
	if (!pBuf)
		return FALSE;

	Lock();
	m_buf += string(pBuf, nSize);
	Unlock();
	return TRUE;
}

int	CTcpParser::GetOnePacket(char* pOutBuf)
{
	if (m_nSizeOfLenField == -1)
	{
		sprintf(m_zMsg, "You should call Begin() at first");
		return FALSE;
	}


	int nRet = -1;
	__try
	{
		Lock();
		__try
		{
			nRet = GetOnePacketFn(pOutBuf);
		}
		__except (ReportException(GetExceptionCode(), "CTcpParser::GetOnePacket", m_zMsg))
		{
			nRet = -1;
		}
	}
	__finally
	{

		Unlock();
	}

	return nRet;
}


/*
	retrun value:
		0	: buffer is empty or not sound packet
		-1	: error
		else : length of one sound packet
*/
int	CTcpParser::GetOnePacketFn(char* pOutBuf)
{
	if (m_buf.empty()) {
		return 0;
	}

	/////////////////////////////////////////////////////////////
	//	STX + LEN + ..... + ETX

	char tmp[128];
	int startpos = 0, endpos = 0;

	// the 1st byte of buffer is supposed to be	[START].
	// if not, remove before 1st [START]
	if (m_buf[0] != m_Start)
	{
		startpos = m_buf.find_first_of(m_Start); // find 1st [START]

		// no [START]? remove all.
		if (startpos == m_buf.npos) {
			strcpy(m_zMsg, "There is no [START] in buffer.");
			RemoveAll(FALSE);
			return -1;
		}

		// is there [START]? remove before [START]
		int nCnt = startpos;
		m_buf.erase(0, nCnt);
		return 0;
	}



	//	find start
	startpos = 0;

	// get packet length from size
	// ex) #  18this is test@  
	//     => sprintf(tmp, "%.4s", buf+1);
	sprintf(tmp, "%.*s", m_nSizeOfLenField, m_buf.c_str() + 1);
	int nPacketLength = atoi(tmp);
	
	// If real length is less than nominal length, this data is not complete packet. return and wait.
	if (nPacketLength > m_buf.size())
		return 0;

	// check [END]
	endpos = nPacketLength - 1;
	if (m_buf[endpos] != m_End)
	{
		// if the end of packet is not [END], this is wrong. remove
		m_buf.erase(0, nPacketLength);
		return -1;
	}

	// right packet and right parsing
	strncpy(pOutBuf, &m_buf[0], nPacketLength);
	m_buf.erase(0, nPacketLength);
	return nPacketLength;

}

VOID CTcpParser::RemoveAll(BOOL bLock)
{
	if (bLock)	Lock();
	m_buf.erase(m_buf.begin(), m_buf.end());
	if (bLock)	Unlock();
}


LPCSTR CTcpParser::GetStrBuff()
{
	return (LPCSTR)m_buf.c_str();
}


VOID  CTcpParser::Erase(int nStartPos, int nLen)
{
	m_buf.erase( nStartPos, nLen );

}