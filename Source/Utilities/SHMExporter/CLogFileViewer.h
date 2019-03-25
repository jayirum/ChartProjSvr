#pragma once
#pragma warning(disable:4786)
#pragma warning(disable:4503)

#include <stdio.h>
#include "../../IRUM_UTIL/Log.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/IRUM_Common.h"
#include "../../IRUM_UTIL/QueueShm.h"
using namespace std;

class CLogFileViewer :public CLogMsg
{
public:
	CLogFileViewer() :CLogMsg() {};
	void getLogFileName(_Out_ char* szFileName);
};