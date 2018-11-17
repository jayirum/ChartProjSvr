#include "TimeInterval.h"
#include <timeapi.h>
#include <Mmsystem.h>



CTimeInterval::CTimeInterval()
{
	ZeroMemory(&m_time, sizeof(m_time));
}


CTimeInterval::~CTimeInterval()
{
}

void CTimeInterval::start()
{
	m_time.dwStart = timeGetTime();
	m_bShouldCheck = TRUE;
}

void CTimeInterval::reset()
{
	m_time.dwStart = timeGetTime();
	m_bShouldCheck = TRUE;
}

void CTimeInterval::lapse()
{
	if (!m_bShouldCheck)
		return;

	m_time.dwNow = timeGetTime();

	m_time.interval_ms = (int)(m_time.dwNow - m_time.dwStart);
	m_time.interval_sec = m_time.interval_ms / 1000;
	m_time.interval_min = m_time.interval_sec / 60;
	m_time.interval_hour = m_time.interval_min / 60;
}

int	 CTimeInterval::interval_ms()
{
	return m_time.interval_ms;
}
int CTimeInterval::interval_sec()
{
	return m_time.interval_sec;
}
int CTimeInterval::interval_min()
{
	return m_time.interval_min;
}
int CTimeInterval::interval_hour()
{
	return m_time.interval_hour;
}

BOOL CTimeInterval::isPassed(TIME_MODE mode,  int checkVal)
{
	if (!m_bShouldCheck)
		return FALSE;

	BOOL bPassed = FALSE;

	lapse();

	switch (mode)
	{
	case MODE_MS:
		bPassed = (interval_ms() >= checkVal) ? TRUE : FALSE;
		break;

	case MODE_SEC:
		bPassed = (interval_sec() >= checkVal) ? TRUE : FALSE;
		break;

	case MODE_MIN:
		bPassed = (interval_min() >= checkVal) ? TRUE : FALSE;
		break;

	case MODE_HOUR:
		bPassed = (interval_hour() >= checkVal) ? TRUE : FALSE;
		break;
	}

	return bPassed;
}