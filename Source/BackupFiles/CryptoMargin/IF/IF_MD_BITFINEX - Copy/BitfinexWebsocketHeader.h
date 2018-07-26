#include <string>
#include <ctime>
#include <time.h> 
#include <curses.h>			/* ncurses.h includes stdio.h */  
#include <string> 
#include <map>
#include "Precompiled.h"
using namespace std;



//void Int64ToDate(__int64 n64Milliseconds, char szStringOut[]);
//void Time_tToString(time_t timeRawTime, char szStringOut[]);

class stringsForDisplay
{
private:
	map<int, string> m_mapStrings;
public:
	stringsForDisplay::stringsForDisplay()
	{

	}

	map<int, string> StringsOutForDisplay()
	{
		return m_mapStrings;
	}

	int AddString(string szString)
	{
		m_mapStrings[m_mapStrings.size() + 1] = szString;
		return 0;
	}

	int DeleteAllStrings()
	{
		for (map<int, string>::iterator iterator = m_mapStrings.begin(); iterator != m_mapStrings.end(); iterator++)
		{
			m_mapStrings.erase(iterator->first);
		}
		return 0;
	}

	int DisplayEmptyStringsAtCoords(int x, int y, int nMaxRows)
	{
		int nCounter = 0;
		for (map<int, string>::iterator iterator = m_mapStrings.begin(); iterator != m_mapStrings.end(); iterator++)
		{
			nCounter++;
			if (nCounter == nMaxRows)
				return 0;
			mvprintw(y + iterator->first - 1, x, "%s", string(iterator->second.length(), ' ').c_str());
		}
		return 0;
	}

	int DisplayStringsAtCoords(int x, int y, int nMaxRows)
	{
		int nCounter = 0;
		for (map<int, string>::iterator iterator = m_mapStrings.begin(); iterator != m_mapStrings.end(); iterator++)
		{
			nCounter++;
			if (nCounter == nMaxRows)
				return 0;
			mvprintw(y + iterator->first - 1, x, m_mapStrings[iterator->first].c_str());
		}
		return 0;
	}
};



//
////Todo : change the function name
//void Int64ToDate(__int64 n64Milliseconds, char szStringOut[])
//{
//	char sTime[15]; //14 characters and 1 trailing endstring character
//	time_t rawtime = n64Milliseconds / 1000; //remove the trailing 1000 
//											 //http://www.cplusplus.com/reference/ctime/strftime/
//	strftime(sTime, 15, "%Y%m%d%H%M%S", localtime(&rawtime));
//	strcpy(szStringOut, sTime);
//}
//
//void Time_tToString(time_t timeRawTime, char szStringOut[])
//{
//	tm* ptm = std::localtime(&timeRawTime);
//	char szBuffer[15];
//	strftime(szBuffer, 15, "%Y%m%d%H%M%S", ptm);
//	strcpy(szStringOut, szBuffer);
//}
