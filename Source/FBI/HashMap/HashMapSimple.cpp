#define _HASH_MAP_EXPORS

#include "HashMap.h"
#include "MapClassSimple.h"


template <class KEY, class VAL>
map<int, CSimpleMap<KEY, VAL>* >	g_mapSimple;	// map <idx, CSimpleMap>
CRITICAL_SECTION					g_csSimple;

void	SMAP_Initialize()
{
	InitializeCriticalSection(&g_csSimple);
}

template <class KEY, class VAL>
void	SMAP_DeInitialize()
{
	EnterCriticalSection(&g_csSimple);
	for (auto it = g_mapSimple.begin(); it != g_mapSimple.end(); it++)
	{
		delete (*it).second;
	}
	g_mapSimple.clear();
	LeaveCriticalSection(&g_csSimple);

	DeleteCriticalSection(&g_csSimple);
}


/*
	return : the idx 
*/
template <class KEY, class VAL>
int	SMAP_Create()
{
	CSimpleMap<KEY, VAL>*	pClass = new CSimpleMap;

	pClass->Initialize();

	EnterCriticalSection(&g_csSimple);
	int nIdx = g_mapSimple.size() + 1;
	g_mapSimple[nIdx] = pClass;
	LeaveCriticalSection(&g_csSimple);

	return nIdx;
}

template <class KEY, class VAL>
void SMAP_Delete(int idx)
{
	EnterCriticalSection(&g_csSimple);
	map<int, CSimpleMap<KEY, VAL>* >::iterator it = g_mapSimple.find(idx);

	if (it != g_mapSimple.end())
	{
		delete (*it).second;
		g_mapSimple.erase(it);
	}
	LeaveCriticalSection(&g_csSimple);
}

template <class KEY, class VAL>
bool	SMAP_IsExists(int idx, char* psKey)
{
	bool bExist;
	EnterCriticalSection(&g_csSimple);

	map<int, CSimpleMap<KEY, VAL>* >::iterator it = g_mapSimple.find(idx);
	if (it == g_mapSimple.end())
		bExist = false;
	else
	{
		bExist = ((*it).second)->IsExists(k);
	}
	LeaveCriticalSection(&g_csSimple);
	return bExist;
}
