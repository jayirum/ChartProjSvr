#pragma once


#define	SERVICENAME		"A-Bot_No2"
#define DISPNAME		"A-Bot No2"
#define DESC			"Strategy No 2"
#define EXENAME			"Abot_No2.exe"


#define __APP_VERSION "v1.0"


enum {
	I_SYMBOL = 0, I_NAME, I_TICKVALUE, I_TICKSIZE, I_DOTCNT, I_CURRPRC, I_OPENPRC, I_ORDQTY,
	I_ENDTM,	//장마감시간
	I_MAXSL,	//손절제한횟수
	I_MAXPT,	//익절제한횟수
	I_ENTRY_SPREAD,	//진입SPREAD		0.1%
	I_CLR_SPREAD,	//익절SPAREAD	0.5%
	I_PT_POINT	//최고점 찍고 떨어질때 손절 50%
};
enum { N_SYBOL = 0, N_BS, N_OPENPRC, N_ENTRYPRC, N_CURRPRC, N_PLTICK, N_QTY, N_PL };

#define MEM_BLOCK_SIZE	1024
#define MEM_PRE_ALLOC	100
#define MEM_MAX_ALLOC	1000

