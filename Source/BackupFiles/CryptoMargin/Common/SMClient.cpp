#include "SMClient.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "../../IRUM_UTIL/Util.h"

CSMClient::CSMClient()
{
	m_pSendBuff = NULL;
}

CSMClient::~CSMClient()
{
	delete[] m_pSendBuff;
}


VOID CSMClient::SetDestination(char* psDest)
{
	strcpy(m_zDestination, psDest);
}

VOID CSMClient::SetMsg(char* psMsgKey, char* psMsg)
{
	strcpy(m_zMsgKey, psMsgKey);
	strcpy(m_zMsgData, psMsg);
}

VOID CSMClient::SetData(char* psDataKey, char* pData, FIELD_TYPE DataType, int nDataSize)
{
	strcpy(m_zDataKey, psDataKey);
	m_pData = pData;
	m_DataType = DataType;
	m_nDataSize = nDataSize;
}



VOID CSMClient::SetDelieveryMode(TDeliveryMode mode)
{
	if (mode == Delivery_RR)
		m_Tr = ctDestRRSend;
	else if (mode == Delivery_RR_Response)
		m_Tr = ctDestRRSendResponse;
	else
		m_Tr = ctDestPushSend;
}


char* CSMClient::GetSendData(_Out_ int *pnSendSize)
{
	__try
	{
		GetSendDataFn(pnSendSize);
	}
	__except (ReportException(GetExceptionCode(), "GetSendData", m_zErrMsg))
	{
		return NULL;
	}

	return m_pSendBuff;
}

char* CSMClient::GetSendDataFn(_Out_ int *pnSendSize)
{
	int nBufferSize = 0;
	int nLen = CalcDataSize(m_zMsgKey, TYPE_STRING, m_zMsgData, strlen(m_zMsgData));
	nBufferSize += nLen;

	nLen = CalcDataSize(m_zDataKey, m_DataType, m_pData, m_nDataSize);
	nBufferSize += nLen;

	// total sending size
	int nHeadSize = sizeof(SHM_HEAD);
	int nDestPos = sizeof(WORD);
	int nDestLen = strlen(m_zDestination);
	m_nSendSize = nHeadSize + nDestPos + nDestLen + nBufferSize;
	*pnSendSize = m_nSendSize;

	// create send buffer
	//m_pSendBuff = new char[m_nSendSize];

	//int iPos;
	//iPos = nHeadSize + nDestPos + nDestLen;
	//iPos = SetFieldData_Msg(iPos, Buffer, m_zMsgKey, m_zMsgData);
	//iPos = SetFieldData_Data(iPos, Buffer, m_zDataKey, m_pData, m_nDataSize);

	//SHM_HEAD* pH = (SHM_HEAD*)Buffer;
	//pH->TR = (BYTE)m_Tr;
	//pH->DataLen = m_nSendSize - nHeadSize;

	//Buffer[nHeadSize] = (WORD)(nDestLen);
	//CopyMemory(&Buffer[nHeadSize + nDestPos], m_zDestination, nDestLen);



	m_pSendBuff = new char[m_nSendSize];
	ZeroMemory(m_pSendBuff, m_nSendSize);

	int iPos;
	iPos = nHeadSize + nDestPos + nDestLen;
	iPos = SetFieldData_Msg(iPos, m_pSendBuff, m_zMsgKey, m_zMsgData);
	iPos = SetFieldData_Data(iPos, m_pSendBuff, m_zDataKey, m_pData, m_nDataSize);

	SHM_HEAD* pH = (SHM_HEAD*)m_pSendBuff;
	pH->TR = (BYTE)m_Tr;
	pH->DataLen = m_nSendSize - nHeadSize;

	m_pSendBuff[nHeadSize] = (WORD)(nDestLen);
	CopyMemory(&m_pSendBuff[nHeadSize + nDestPos], m_zDestination, nDestLen);

	
	return m_pSendBuff;
}

int CSMClient::GetByteSize(int nLen)
{
	int nResult = 0;

	if (nLen <= 255)
		nResult = sizeof(BYTE);
	else if (nLen <= 65535)
		nResult = sizeof(WORD);
	else if (nLen <= 2147483647)
		nResult = sizeof(int);

	return nResult;
}


int CSMClient::CalcDataSize(char* pKey, FIELD_TYPE type, char* pData, int nDataSize)
{
	int iLen, Result;

	Result = 0;

	/////////////////////////////////////////
	// FieldName 정보
	Result++;;	// Data Info (1Byte), 길이를 표현하기 위해 필요한 바이트수
	iLen = strlen(pKey);
	Result += GetByteSize(iLen);
	Result += iLen; // Data Len

	/////////////////////////////////////////
	// Data 정보
	Result++;	// Data Info (1Byte)

	if (type == TYPE_INT)	//(IT^.Data) ^ .MsgFieldType = mfInteger then
		Result += sizeof(long);
	else if (type == TYPE_DOUBLE)	//PFieldData(IT^.Data) ^ .MsgFieldType = mfDouble then
		Result += sizeof(double);
	else if (type == TYPE_STRING)	//PFieldData(IT^.Data) ^ .MsgFieldType = mfString then
	{
		iLen = strlen(pData);
		Result += GetByteSize(iLen);
		Result += iLen;
	}
	else if (type==TYPE_BINARY)	//PFieldData(IT^.Data) ^ .MsgFieldType = mfBinary then
	{
		iLen = nDataSize;	// : = PFieldData(IT^.Data) ^ .Bin.FLen;
		Result += GetByteSize(iLen);
		Result += iLen;
	}

	return Result;
}

int CSMClient::SetFieldData_Msg(int iPos, char* Buffer, char* pKey, char* pData)
{
	//char Buffer[1024] = { 0, };

	int iDataHead, iKey, iData, iByteCnt;
	iDataHead = iPos;
	Buffer[iDataHead] = ((BYTE)Buffer[iDataHead]) | SM_DATA_FIELD;
	Buffer[iDataHead] = ((BYTE)Buffer[iDataHead]) | SM_DATA_BYTE_1;	// Byte(Buffer[iDataHead]) := Byte(Buffer[iDataHead]) or SM_DATA_BYTE_1;

																	// MSG 저장 - KEY
	iPos++;
	iKey = strlen(pKey);
	iByteCnt = GetByteSize(iKey);	// 255 미만이다.
	Buffer[iPos] = iKey;
	iPos += iByteCnt;
	CopyMemory(Buffer + iPos, pKey, iKey);
	iPos += iKey;

	// MSG 저장 - DATA
	iDataHead = iPos;
	Buffer[iDataHead] = ((BYTE)Buffer[iDataHead]) | SM_DATA_STRING;
	Buffer[iDataHead] = ((BYTE)Buffer[iDataHead]) | SM_DATA_BYTE_1;	// Byte(Buffer[iDataHead]) := Byte(Buffer[iDataHead]) or SM_DATA_BYTE_1;

	iData = strlen(pData);
	iByteCnt = GetByteSize(iData);	// 255 미만이다.
	iPos++;
	Buffer[iPos] = iData;	// data length
	iPos += iByteCnt;
	CopyMemory(Buffer + iPos, pData, iData);
	iPos += iData;

	//CopyMemory(pBuffer, Buffer, iPos);
	return iPos;
}


int CSMClient::SetFieldData_Data(int iPos, char* Buffer, char* pKey, char* pData, int nDataSize)
{
	//char Buffer[1024] = { 0, };
	//int nStart = iPos;

	int iDataHead, iKey, iData, iByteCnt;
	iDataHead = iPos;
	Buffer[iDataHead] = ((BYTE)Buffer[iDataHead]) | SM_DATA_FIELD;
	Buffer[iDataHead] = ((BYTE)Buffer[iDataHead]) | SM_DATA_BYTE_1;	// Byte(Buffer[iDataHead]) := Byte(Buffer[iDataHead]) or SM_DATA_BYTE_1;
	

	// DATA 저장 - KEY
	iPos++;
	iKey = strlen(pKey);
	iByteCnt = GetByteSize(iKey);	// 255 미만이다.	
	Buffer[iPos] = iKey;
	iPos += iByteCnt;
	CopyMemory(Buffer + iPos, pKey, iKey);
	iPos += iKey;

	// MSG 저장 - DATA
	iDataHead = iPos;
	Buffer[iDataHead] = ((BYTE)Buffer[iDataHead]) | SM_DATA_BINARY;
	iData = nDataSize;
	iByteCnt = GetByteSize(iData);	// 
	if (iByteCnt == 1)
		Buffer[iDataHead] = ((BYTE)Buffer[iDataHead]) | SM_DATA_BYTE_1;
	else if (iByteCnt == 2)
		Buffer[iDataHead] = ((BYTE)Buffer[iDataHead]) | SM_DATA_BYTE_2;
	if (iByteCnt == 4)
		Buffer[iDataHead] = ((BYTE)Buffer[iDataHead]) | SM_DATA_BYTE_4;

	iPos++;
	Buffer[iPos] = iData;
	iPos += iByteCnt;
	CopyMemory(Buffer + iPos, pData, iData);
	iPos += iData;

	//CopyMemory(pBuffer, Buffer + nStart, iPos - nStart);
	return iPos;
}