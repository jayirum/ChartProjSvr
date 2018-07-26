#include "SMReceiver.h"


CSMessage::CSMessage()
{}
CSMessage::~CSMessage()
{
	delete[] m_pData;
}



void CSMessage::SetMsg(char* pzMsg)
{
	strcpy(m_zMsg, pzMsg);
}
void CSMessage::SetField(char* pzField)
{
	strcpy(m_zField, pzField);
}
void CSMessage::SetData(void* pData, int nDataSize)
{
	m_pData = new char[nDataSize];
	ZeroMemory(m_pData, nDataSize);
	CopyMemory(m_pData, pData, nDataSize);
}