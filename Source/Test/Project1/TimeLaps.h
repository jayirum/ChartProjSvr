#pragma once

#include <Windows.h>
#pragma comment(lib, "Winmm.lib")


typedef enum _TIME_TP{
TP_MS, TP_SEC, TP_MIN, TP_HOUR
}TIME_TP;

typedef struct _TIME_INTERVAL
{
	DWORD	dwStart;
	DWORD	dwNow;
	int		interval_ms;
	int		interval_sec;
	int		interval_min;
	int		interval_hour;
}TIME_INTRVAL;


class CTimeInterval
{
public:
	CTimeInterval();
	~CTimeInterval();

	void	start();
	void	reset() { start(); }
	void	lapse();
	int		interval_ms();
	int		interval_sec();
	int		interval_min();
	int		interval_hour();

	BOOL	isPassed(TIME_TP tp, int checkVal);

private:
	TIME_INTRVAL	m_time;
};
