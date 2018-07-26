#pragma once

#include "../../IRUM_INC/IRUM_Common.h"

typedef enum { SIGNAL_ERR = -1, SIGNAL_NONE, SIGNAL_EXIST } RET_SIGNAL;

/////////////////////////////////////////////////////////////////////////
typedef struct _CHECKSIG_RESULT_OPENPRC_ONE
{
	RET_SIGNAL	SigRet;
	char		zStratID[LEN_STRAT_ID + 1];
	char		zStratNm[64];
	char		zGroupKey[LEN_GROUP_KEY + 1];
	char		zChartNm[LEN_CHART_NM + 1];
	char		ChartGb[1];	// minus, plus
	char		zSigPrc[LEN_PRC + 1];
	int			nCrossTp;
}CHECKSIG_RESULT_OPENPRC_ONE;

typedef struct _CHECKSIG_RESULT_OPENPRC
{
	CHECKSIG_RESULT_OPENPRC_ONE	sigFilled;
	CHECKSIG_RESULT_OPENPRC_ONE	sigEmpty;
}CHECKSIG_RESULT_OPENPRC;

/////////////////////////////////////////////////////////////////////////
typedef struct _CHECKSIG_RESULT_ASSIST
{
	RET_SIGNAL	SigRet;
	char		zStratID[LEN_STRAT_ID + 1];
	char		zStratNm[64];
	char		zGroupKey[LEN_GROUP_KEY + 1];
	char		zChartNm[LEN_CHART_NM + 1];
	char		zAssistPrc[LEN_PRC + 1];
	char		ChartGb[1];						// minus, plus
	char		zMatchedChartNm[LEN_CHART_NM + 1];	// chart which makes assist signal with base chart
	char		cClrYN[1];		// 청산신호인가?
	char		cClrCrossTp[1];	// 청산신호. U:돌파, D:붕괴
	char		zClrAssistPrc[LEN_PRC+1];	// 청산신호인 경우 보조지표 가격
	char		zClrNowPrc[LEN_PRC + 1];		// 청산신호의 경우 현재가격
}CHECKSIG_RESULT_ASSIST;


typedef struct _CHECKSIG_RESULT_CROSSMA
{
	RET_SIGNAL	SigRet;
	char		zStratID[LEN_STRAT_ID + 1];
	char		zStratNm[64];
	char		zGroupKey[LEN_GROUP_KEY + 1];
	char		zChartNm[LEN_CHART_NM + 1];
	char		zMAPrice[LEN_PRC + 1];
	//char		zPrevPrice[LEN_PRC + 1];
	char		zNowPrice[LEN_PRC + 1];
	//char		MATp[1];	// 1: 10min
	char		Tp[1];		// U : 상향돌파.upward breakthrough  D:하향붕괴 downward breakthrough
	

}CHECKSIG_RESULT_CROSSMA;


typedef struct _CHECKSIG_RESULT_SIDEWAY
{
	RET_SIGNAL	SigRet;
	char		zStratID[LEN_STRAT_ID + 1];
	char		zStratNm[64];
	char		zGroupKey[LEN_GROUP_KEY + 1];
	char		zChartNm[LEN_CHART_NM + 1];
	char		zHihgLowPrice[LEN_PRC + 1];	// when Tp=U High, when Tp=D, Low
	char		zNowPrice[LEN_PRC + 1];
	char		Tp[1];						// U : 상향돌파.upward breakthrough  D:하향붕괴 downward breakthrough

}CHECKSIG_RESULT_SIDEWAY;