#pragma once


#define	SERVICENAME		"A-Bot No1"
#define DISPNAME		"A-Bot No1"
#define DESC			"Strategy No 1"
#define EXENAME			"Abot_No1.exe"


#define __APP_VERSION "v1.0"


enum {
	I_SYMBOL = 0, I_NAME, I_TICKVALUE, I_TICKSIZE, I_DOTCNT, I_CURRPRC, I_OPENPRC, I_ORDQTY,
	I_ENDTM,	//�帶���ð�
	I_MAXSL,	//��������Ƚ��
	I_MAXPT,	//��������Ƚ��
	I_ENTRY_SPREAD,	//����SPREAD		0.1%
	I_CLR_SPREAD,	//����SPAREAD	0.5%
	I_PT_POINT	//�ְ��� ��� �������� ���� 50%
};
enum { N_SYBOL = 0, N_BS, N_OPENPRC, N_ENTRYPRC, N_CURRPRC, N_PLTICK, N_QTY, N_PL };

#define MEM_BLOCK_SIZE	1024
#define MEM_PRE_ALLOC	100
#define MEM_MAX_ALLOC	1000

