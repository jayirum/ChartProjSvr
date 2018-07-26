#pragma once

/*
	종목별로 SHM 을 만든다.
*/


#define	SERVICENAME		"IRUM_ChartShmQueue"
#define DISPNAME		"IRUM_ChartShmQueue"
#define DESC			"차트용 SHM을 구성하고 DB저장"
#define EXENAME			"ChartShmQueue.exe"


//////////////////////////////////////////////////////////////////////////
/*
	분차트만 구성한다.

	품목별로 SHM 을 별도로 만든다.
	=> 주요 품목을 미리 정의 한다.
*/
