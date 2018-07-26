#pragma once
#include <curses.h>			/* ncurses.h includes stdio.h */  
#include <string> 
#include <map>

using namespace std;

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