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
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_INC/IRUM_Common.h"
using namespace std;
typedef std:: vector<std::string> stringvec;	// list, dequeu, set, map

#define	SERVICENAME		"IRUM_LogDeleter"
#define DISPNAME		"IRUM_LogDeleter"
#define DESC			"This service deletes log files for IRUMNET charting applications according to a certain number of days stated in INI file"
#define EXENAME			"LogDeleter.exe"
#define SERVICEPATTERN		"\\*.log"
#define SERVICECONFIG		"\\Logger.ini"
#define SERVICEWAITMILLISECONDS 3000 // 3 seconds of wait for the service loop

#define DELETENUMBERKEY		"DeleteAfterNDays"
#define LOGDIRECTORYKEY		"LOGDIRECTORY"
#define SERVICECONFIG		"\\Logger.ini"
#define SERVICEPATTERN		"\\*.log"
#define SERVICESECTION		"Setting"
//#define SERVICESTARTDATEKEY	"StartDate" // files from the date to be deleted
//#define SERVICEENDDATEKEY	"EndDate"	// files till the date to be deleted
