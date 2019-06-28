#pragma once

#include "HashMapInc.h"


//////////////////////////////////////////////////////////////////////////////////////
//
//
//
//	Simple Map
//
//
//
//////////////////////////////////////////////////////////////////////////////////////

// create
typedef void*	(*TSimpMAP_CreateSS)();
typedef void*	(*TSimpMAP_CreateSL)();
typedef void*	(*TSimpMAP_CreateSD)();
typedef void*	(*TSimpMAP_CreateSR)();

typedef void*	(*TSimpMAP_CreateLS)();
typedef void*	(*TSimpMAP_CreateLL)();
typedef void*	(*TSimpMAP_CreateLD)();
typedef void*	(*TSimpMAP_CreateLR)();


// destroy
typedef void	(*TSimpMAP_DestroyS)(void*);	// map class pointer
typedef void	(*TSimpMAP_DestroyL)(void*);	// map class pointer

// add value
typedef bool	(*TSimpMAP_AddValueSS)(void*, char*, char*, int, bool);		// map pointer, key, value, value size, update
typedef bool	(*TSimpMAP_AddValueSL)(void*, char*, long*, int, bool);
typedef bool	(*TSimpMAP_AddValueSD)(void*, char*, double*, int, bool );
typedef bool	(*TSimpMAP_AddValueSR)(void*, char*, RECORD_VAL*, int, bool );

typedef bool	(*TSimpMAP_AddValueLS)(void*, long, char*, int, bool);		// map pointer, key, value, value size, update
typedef bool	(*TSimpMAP_AddValueLL)(void*, long, long*, int, bool);
typedef bool	(*TSimpMAP_AddValueLD)(void*, long, double*, int, bool);
typedef bool	(*TSimpMAP_AddValueLR)(void*, long, RECORD_VAL*, int, bool);

// get value
typedef bool	(*TSimpMAP_GetStringValueS)	(void*, char*, /*out*/char*, int );	// map pointer, key, value, value size
typedef bool	(*TSimpMAP_GetLongValueS)	(void*, char*, /*out*/long* );
typedef bool	(*TSimpMAP_GetDoubleValueS)	(void*, char*, /*out*/double*);
typedef bool	(*TSimpMAP_GetRecordValueS)	(void*, char*, /*out*/RECORD_VAL*);

typedef bool	(*TSimpMAP_GetStringValueL)	(void*, long, /*out*/char*, int);	// map pointer, key, value, value size
typedef bool	(*TSimpMAP_GetLongValueL)	(void*, long, /*out*/long*);
typedef bool	(*TSimpMAP_GetDoubleValueL)	(void*, long, /*out*/double*);
typedef bool	(*TSimpMAP_GetRecordValueL)	(void*, long, /*out*/RECORD_VAL*);

// delete
typedef void	(*TSimpMAP_DelS) (void*, char*);	// map pointer, key
typedef void	(*TSimpMAP_DelL) (void*, char*);

//////////////////////////////////////////////////////////////////////////////////////
//
//
//
//	List Map
//
//
//
//////////////////////////////////////////////////////////////////////////////////////

// create
typedef void*	(*TListMAP_CreateS)();
typedef void*	(*TListMAP_CreateL)();

// destroy
typedef void	(*TListMAP_DestroyS)(void*);	// map class pointer
typedef void	(*TListMAP_DestroyL)(void*);

// add value
typedef bool	(*TListMAP_AddValueS)(void*, char*, RECORD_VAL*);	// map pointer, key, value
typedef bool	(*TListMAP_AddValueL)(void*, char*, RECORD_VAL*);	// map pointer, key, value

// get value
typedef RECORD_VAL* (*TListMAP_GetRecordValueS)	(void*, char*, /*out*/int*);	// map pointer, key, record count
typedef RECORD_VAL* (*TListMAP_GetRecordValueL)	(void*, long, /*out*/int*);		// map pointer, key, record count

// del
typedef void	(*TListMAP_DelS)(void*);	// map class pointer
typedef void	(*TListMAP_DelL)(void*);

// mem free
typedef void	(*TListMap_MemFreeS)(void*, void*);	// map class pointer, pointer to be free
typedef void	(*TListMap_MemFreeL)(void*, void*);

