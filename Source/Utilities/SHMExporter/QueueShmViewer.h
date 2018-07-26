#pragma once
#pragma warning(disable:4786)
#pragma warning(disable:4503)

#include <stdio.h>
#include "../../IRUM_UTIL/Log.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "../../IRUM_UTIL/QueueShm.h"
using namespace std;
#define __APP_VERSION "20171025_SHMEXPORTER_VERSION"

class QueueShmViewer :public CQueueShm
{
public:
	QueueShmViewer() :CQueueShm() {};
	int GroupKeyViewer(_In_ char* psGroupKey, _In_ char* psTimeFrame, _In_ char* pDateTime);

private:
	BOOL GetNextStructKey(_In_ char* pGroupKey, _Out_ char* pStructKey, _In_ int nCount);

private:
	char m_zShmNm[128], m_zMutexNm[128];
};