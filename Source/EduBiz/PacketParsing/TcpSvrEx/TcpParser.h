#pragma once

#include <windows.h>
#include <string>
using namespace std;



/*
	Definition of Packet Structure

	START : 1byte - Delimiter of Start
	LEN   : x byte - Total Packet Length. 
	END   : 1byte - Delimiter of End

	ex)
	typedef struct _PRICE_DATA
	{
	   char  START[1];
	   char  LEN[4];
	   char  Data[10];
	   char  END[1];
	}PRICE_DATA;

	PRICE_DATA data;
	data.START[0] = 0X02;
	sprintf(data.LEN, "%4d", sizeof(PRICE_DATA));
	strcpy(data.Data, "1234567890");
	data.END[0] = 0x03;

	# I used STX(0x02), ETX(0x03) as delimiter, but you can change it as you want.
	  But, delimiter must not be used for other purpose, that is, unique in the packet.

	# I set the size of LEN as 4, but you can change it as you want.
*/

class CTcpParser  
{
public:
	CTcpParser();
	virtual ~CTcpParser();

	VOID	Begin(int nSizeOfLenField, char cStart = 0x02, char cEnd = 0x03);
	VOID	End();

	BOOL	AddTcpData(char* pBuf, int nSize);
	int		GetOnePacket(/*out*/char* pOutBuf);
	
	char*	GetMsg() { return m_zMsg; }
	LPCSTR GetStrBuff();

private:
	int		GetOnePacketFn(/*out*/char* pOutBuf);
	VOID	Erase(int nStartPos, int nLen);
	VOID	RemoveAll(BOOL bLock);
	
	VOID	Lock() { EnterCriticalSection(&m_cs); }
	VOID	Unlock() { LeaveCriticalSection(&m_cs); }
private:
	string				m_buf;
	CRITICAL_SECTION	m_cs;
	char				m_zMsg[1024];
	
	char	m_Start, m_End;
	int		m_nSizeOfLenField;
		

};
