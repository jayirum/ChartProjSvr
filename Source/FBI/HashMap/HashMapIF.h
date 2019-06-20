#pragma once

#include "HashMapInc.h"

typedef void*	(*TSimpMAP_CreateSS)();
typedef void*	(*TSimpMAP_CreateSL)();
typedef void*	(*TSimpMAP_CreateSD)();
typedef void*	(*TSimpMAP_CreateSR)();

//typedef void*	(*TSimpMAP_CreateLS);
//typedef void*	(*TSimpMAP_CreateLL);
//typedef void*	(*TSimpMAP_CreateLD);
//typedef void*	(*TSimpMAP_CreateLR);



typedef void	(*TSimpMAP_Delete)(void*);

typedef bool	(*TSimpMAP_AddValueSS)(void*, char*, char*, int, bool);
typedef bool	(*TSimpMAP_AddValueSL)(void*, char*, long*, int, bool);
typedef bool	(*TSimpMAP_AddValueSD)(void*, char*, double*, int, bool );
typedef bool	(*TSimpMAP_AddValueSR)(void*, char*, RECORD_VAL*, int, bool );

typedef bool (*TSimpMAP_GetStringValueS)(void*, char*, /*out*/char*, int );
typedef bool (*TSimpMAP_GetLongValueS)(void*, char*, /*out*/long* );
typedef bool(*TSimpMAP_GetDoubleValueS)(void*, char*, /*out*/double*);
typedef bool(*TSimpMAP_GetRecordValueS)(void*, char*, /*out*/RECORD_VAL*);
