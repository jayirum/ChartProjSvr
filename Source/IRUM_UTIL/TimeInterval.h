#pragma once

#include <Windows.h>
//#include "BaseThread.h"
#pragma comment(lib, "Winmm.lib")


typedef enum _TIME_MODE{
	MODE_NONE=-1, MODE_MS, MODE_SEC, MODE_MIN, MODE_HOUR
}TIME_MODE;

typedef struct _TIME_INTERVAL
{
	DWORD	dwStart;
	DWORD	dwNow;
	int		interval_ms;
	int		interval_sec;
	int		interval_min;
	int		interval_hour;
}TIME_INTRVAL;


class CTimeInterval //: public CBaseThread
{
public:
	CTimeInterval();
	~CTimeInterval();

	void	start();
	void	reset();
	void	nomore_check() { m_bShouldCheck = FALSE; }

	void	lapse();
	int		interval_ms();
	int		interval_sec();
	int		interval_min();
	int		interval_hour();

	BOOL	isPassed(TIME_MODE mode,int checkVal);

	//virtual void ThreadFunc();

private:
	TIME_INTRVAL	m_time;
	BOOL			m_bShouldCheck;
};
