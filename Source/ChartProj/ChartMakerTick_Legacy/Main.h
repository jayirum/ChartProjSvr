#pragma once

/*
	종목별로 SHM 을 만든다.
*/


#define	SERVICENAME		"IRUM_ChartMakerTick"
#define DISPNAME		"IRUM_ChartMakerTick"
#define DESC			"Tick차트 데이터를 만든다."
#define EXENAME			"ChartMakerTick.exe"


#define MEM_BLOCK_SIZE	512
#define MEM_PRE_ALLOC	100
#define MEM_MAX_ALLOC	1000

#define __APP_VERSION "v3.0_MCAST"