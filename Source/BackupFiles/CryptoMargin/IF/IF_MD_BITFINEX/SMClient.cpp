#include "SMClient.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>

int GetByteSize(int nLen)
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


int CalcDataSize(char* pKey, char cFieldType, char* pData, int nDataSize)
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

	if (cFieldType == 'I')	//(IT^.Data) ^ .MsgFieldType = mfInteger then
		Result += sizeof(long);
	else if (cFieldType == 'D')	//PFieldData(IT^.Data) ^ .MsgFieldType = mfDouble then
		Result += sizeof(double);
	else if (cFieldType == 'S')	//PFieldData(IT^.Data) ^ .MsgFieldType = mfString then
	{
		iLen = strlen(pData);
		Result += GetByteSize(iLen);
		Result += iLen;
	}
	else if (cFieldType == 'B')	//PFieldData(IT^.Data) ^ .MsgFieldType = mfBinary then
	{
		iLen = nDataSize;	// : = PFieldData(IT^.Data) ^ .Bin.FLen;
		Result += GetByteSize(iLen);
		Result += iLen;
	}

	return Result;
}

int SetFieldData_Msg(int iPos, char* Buffer, char* pKey, char* pData)
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


int SetFieldData_Data(int iPos, char* Buffer, char* pKey, char* pData, int nDataSize)
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