#pragma once
#include <stdio.h>
#include <Windows.h>

#pragma warning(disable:4996)

#define MAX_MAP	10
#define MAX_VALUE_SIZE		1024
#define MAX_STRING_VALUE	512

enum KEY_TYPE {
	KEY_STRING = 0,
	KEY_LONG
};

enum VAL_TYPE {
	VAL_STRING = 0,
	VAL_LONG,
	VAL_DOUBLE,
	VAL_RECORD
};


enum MAP_ERRNO {
	ERR_OK	= 0
	,ERR_ALREADY_EXISTS
	,ERR_KEY_ABSENCE
};


enum MAP_STATUS {
	READY = 0
	,CREATED
	,REMOVED
};


struct RECORD_VAL
{
	char val[MAX_VALUE_SIZE];
	//RECORD_VAL* pNext;
};