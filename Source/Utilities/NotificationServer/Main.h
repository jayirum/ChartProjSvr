#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <io.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include "windows.h" //for DWORD and LPTSTR
#include <cstdio>
#include <ctime>
#include <winsvc.h>
#include <stdio.h>
#include <time.h>
#include <list>
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_UTIL/Prop.h"
#include "../../IRUM_UTIL/Log.h"
#include "../../IRUM_UTIL/LogMsg.h" //todo after completion - remove /NEW
#include "../../IRUM_UTIL/IRUM_Common.h" //todo after completion - remove /NEW
using namespace std;
typedef std:: vector<std::string> stringvec;	// list, dequeu, set, map

#define	SERVICENAME		"IRUM_NotifierServer"
#define DISPNAME		"IRUM_NotifierServer"
#define DESC			"This service receive from the Notifier Client of any log message with notification (from LogMsg). This is a test notification log class"
#define SERVICEWAITMILLISECONDS 900 // 1 second = 1000milliseconds of wait for the service loop

