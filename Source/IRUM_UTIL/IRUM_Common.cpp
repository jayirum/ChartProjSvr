#include "IRUM_Common.h" //todo after completion - remove ../NEW/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

int StrToN(char* pszIn, int nLen)
{
	if (!nLen)	return atoi(pszIn);

	char result[128];
	sprintf(result, "%*.*s", nLen, nLen, pszIn);
	//ReplaceChr(result, result+nLen, 0x20, '0');
	return atoi(result);
}


char* ComposeChartName(char* date, char* time, int tp, int nameTp, char* out)
{
	int divider, ret; 
	char zMin[32], zSec[32];	
	char zTm[32];
	sprintf(zTm, "%.8s", time);
	sprintf(zMin, "%.2s", zTm + 3);
	sprintf(zSec, "%.2s", zTm + 6);
	
	if (tp == TP_1MIN) divider = 1;
	if (tp == TP_3MIN) divider = 3;
	if (tp == TP_5MIN) divider = 5;
	if (tp == TP_10MIN) divider = 10;
	if (tp == TP_15MIN) divider = 15;
	if (tp == TP_20MIN) divider = 20;
	if (tp == TP_60MIN) divider = 60;
	if (tp == TP_120MIN) divider = 120;

	// 00:01:00 ~ 00:01:59 ==> 01분 차트
	if (nameTp == CHARTNAME_TP_NEAR)
	{
		ret = (atoi(zMin) / divider);
	}
	// 00:01:01 ~ 00:02:00 ==> 02분 차트
	else 
	{ 
		if (strncmp(time + 6, "00", 2) == 0) 
			ret = ((atoi(zMin)) / divider); 
		else 
			ret = ((atoi(zMin) + 1) / divider);
	}
	int min = (ret)*divider;
	if (tp == TP_60MIN) 
	{ 
		int h = S2N(zTm, 2); 
		if (h == 24)
			h = 0; 
		sprintf(out, "%.8s%02d00", date, h); 
	}
	else if (tp == TP_120MIN) { 
		int h = S2N(zTm, 2); 
		int hRemain = h % 2; 
		if (hRemain == 1)	
			h -= 1; 
		sprintf(out, "%.8s%02d00", date, h); 
	}
	else { 
		sprintf(out, "%.8s%.2s%02d", date, zTm, min); 
	}

	return out;
}



bool ir_isbrokerKR(char* psBroker)
{
	return (strncmp(psBroker, IR_BROKER_KR, 2) == 0);
}

bool ir_isbrokerHD(char* psBroker)
{
	return (strncmp(psBroker, IR_BROKER_HD, 2) == 0);
}

// CLQ17 -> CLQ7
//#define CVTCODE_17_7(symbol, out){sprintf(out,"%.*s%c", strlen(symbol) - 2, symbol, symbol[strlen(symbol) - 1]);}
char* ir_cvtcode_HD_KR(char *pzhdCode, char* pzOut)	// CLQ17 -> CLQ7
{
	int len = strlen(pzhdCode);

	// CLQ17, CLQ20 => 2nd character from right is 1 or 2
	if (pzhdCode[len - 2] == '1' || pzhdCode[len - 2] == '2')
		sprintf(pzOut, "%.*s%c", strlen(pzhdCode) - 2, pzhdCode, pzhdCode[strlen(pzhdCode) - 1]);
	else
		strcpy(pzOut, pzhdCode);
	return pzOut;
}

// URO -> 6E
char* ir_cvtcode_uro_6e(char* pzIn, char* pzOut)
{
	if (strncmp(pzIn, "URO", 3) == 0) sprintf(pzOut, "6E%s", pzIn + 3);
	else if(strncmp(pzIn,"AD",2)==0) sprintf(pzOut,"6A%s", pzIn +2);
	else if(strncmp(pzIn,"BP",2)==0) sprintf(pzOut,"6B%s", pzIn +2);
	else if(strncmp(pzIn,"JY",2)==0) sprintf(pzOut,"6J%s", pzIn +2);
	else strcpy(pzOut, pzIn);

	return pzOut;
}


// 6E -> URO
char* ir_cvtcode_6e_uro(char* pzIn, char* pzOut)
{
	if (strncmp(pzIn, "6E", 2) == 0) sprintf(pzOut, "URO%s", pzIn + 2);
	else if (strncmp(pzIn, "6A", 2) == 0) sprintf(pzOut, "AD%s", pzIn + 2);
	else if (strncmp(pzIn, "6B", 2) == 0) sprintf(pzOut, "BP%s", pzIn + 2);
	else if (strncmp(pzIn, "6J", 2) == 0) sprintf(pzOut, "JY%s", pzIn + 2);
	else strcpy(pzOut, pzIn);

	return pzOut;
}


//char* pack_header_fields(char* pData, char* pzName, char* pzOut)
//{
//	*pzOut = 0x00;
//	PACK_HEADER_S2C* h = (PACK_HEADER_S2C*)pData;
//	char zCode[32];
//	sprintf(zCode, "%.*s", LEN_PACKET_CODE, h->Code);
//
//	if (!strcmp(pzName, "STRAT_ID"))
//	{
//		if (!strcmp(zCode, CD_SIG_OPENPRC))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_OPENPRC*)pData)->StratID), ((PACK_STRAT_FB_OPENPRC*)pData)->StratID);
//		if (!strcmp(zCode, CD_SIG_ASSIST))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_ASSIST*)pData)->StratID), ((PACK_STRAT_FB_ASSIST*)pData)->StratID);
//		if (!strcmp(zCode, CD_SIG_MACROSS))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_CROSSMA*)pData)->StratID), ((PACK_STRAT_FB_CROSSMA*)pData)->StratID);
//		if (!strcmp(zCode, CD_SIG_SIDEWAY))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_SIDEWAY*)pData)->StratID), ((PACK_STRAT_FB_SIDEWAY*)pData)->StratID);
//	}
//	if (!strcmp(pzName, "CHART_TP"))
//	{
//		if (!strcmp(zCode, CD_SIG_OPENPRC))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_OPENPRC*)pData)->ChartTp), ((PACK_STRAT_FB_OPENPRC*)pData)->ChartTp);
//		if (!strcmp(zCode, CD_SIG_ASSIST))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_ASSIST*)pData)->ChartTp), ((PACK_STRAT_FB_ASSIST*)pData)->ChartTp);
//		if (!strcmp(zCode, CD_SIG_MACROSS))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_CROSSMA*)pData)->ChartTp), ((PACK_STRAT_FB_CROSSMA*)pData)->ChartTp);
//		if (!strcmp(zCode, CD_SIG_SIDEWAY))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_SIDEWAY*)pData)->ChartTp), ((PACK_STRAT_FB_SIDEWAY*)pData)->ChartTp);
//	}
//	if (!strcmp(pzName, "CHART_NM"))
//	{
//		if (!strcmp(zCode, CD_SIG_OPENPRC))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_OPENPRC*)pData)->ChartNm), ((PACK_STRAT_FB_OPENPRC*)pData)->ChartNm);
//		if (!strcmp(zCode, CD_SIG_ASSIST))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_ASSIST*)pData)->ChartNm), ((PACK_STRAT_FB_ASSIST*)pData)->ChartNm);
//		if (!strcmp(zCode, CD_SIG_MACROSS))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_CROSSMA*)pData)->ChartNm), ((PACK_STRAT_FB_CROSSMA*)pData)->ChartNm);
//		if (!strcmp(zCode, CD_SIG_SIDEWAY))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_SIDEWAY*)pData)->ChartNm), ((PACK_STRAT_FB_SIDEWAY*)pData)->ChartNm);
//	}
//	if (!strcmp(pzName, "STRAT_PRC"))
//	{
//		if (!strcmp(zCode, CD_SIG_OPENPRC))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_OPENPRC*)pData)->StratPrc), ((PACK_STRAT_FB_OPENPRC*)pData)->StratPrc);
//		if (!strcmp(zCode, CD_SIG_ASSIST))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_ASSIST*)pData)->StratPrc), ((PACK_STRAT_FB_ASSIST*)pData)->StratPrc);
//		if (!strcmp(zCode, CD_SIG_MACROSS))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_CROSSMA*)pData)->StratPrc), ((PACK_STRAT_FB_CROSSMA*)pData)->StratPrc);
//		if (!strcmp(zCode, CD_SIG_SIDEWAY))
//			sprintf(pzOut, "%.*s", sizeof(((PACK_STRAT_FB_SIDEWAY*)pData)->StratPrc), ((PACK_STRAT_FB_SIDEWAY*)pData)->StratPrc);
//	}
//	return pzOut;
//}