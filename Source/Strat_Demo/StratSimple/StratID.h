#pragma once
#include "../../IRUM_INC/IRUM_Common.h"

/*
STRAT01	Y	�ż�����
STRAT02	Y	�ŵ�����
STRAT03	Y	�ŵ�û��OF�ż�����
STRAT04	Y	�ż�û��OF�ŵ�����
*/

#define STRAT_OPEN			"STRAT_OPEN"

#define STRATID_BUY_OPEN	"STRAT01"
#define STRATID_SELL_OPEN	"STRAT02"
#define STRATID_SELL_CLOSE	"STRAT03"
#define STRATID_BUY_CLOSE	"STRAT04"


// STRUCTURE FOR SAVING STRATEGY ON DB
/*
@I_SYMBOL		VARCHAR(10)
,@I_GROUP_KEY	VARCHAR(10)
,@I_CHART_NM	CHAR(12)
,@I_STRAT_ID	VARCHAR(10)
,@I_STRAT_PRC	VARCHAR(15)*/
typedef struct _ST_STRAT_SAVE
{
	char zSymbol[LEN_SYMBOL+1];
	char zGroupKey[LEN_GROUP_KEY+1];
	char zChartNm[LEN_CHART_NM+1];
	char zStratID[32];
	char zStratPrc[LEN_PRC + 1];
	char zSysDT[8 + 1];
	char zTM[12 + 1];
	char zNote[100];
}ST_STRAT_SAVE;