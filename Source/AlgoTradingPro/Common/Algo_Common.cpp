#include "Algo_Common.h" //todo after completion - remove ../NEW/
#include <string.h>
#include <stdio.h>

int getDotCnt(char* psSymbol)
{
	int nDotCnt=0;
	if (strncmp(psSymbol, "6E", 2) == 0)	nDotCnt = 5;
	if (strncmp(psSymbol, "6A", 2) == 0)	nDotCnt = 4;
	if (strncmp(psSymbol, "6B", 2) == 0)	nDotCnt = 4;
	if (strncmp(psSymbol, "6C", 2) == 0)	nDotCnt = 4;
	if (strncmp(psSymbol, "6J", 2) == 0)	nDotCnt = 1;
	if (strncmp(psSymbol, "CL", 2) == 0)	nDotCnt = 2;
	if (strncmp(psSymbol, "GC", 2) == 0)	nDotCnt = 1;
	if (strncmp(psSymbol, "ES", 2) == 0)	nDotCnt = 2;
	if (strncmp(psSymbol, "NQ", 2) == 0)	nDotCnt = 2;
	if (strncmp(psSymbol, "YM", 2) == 0)	nDotCnt = 0;
	if (strncmp(psSymbol, "HS", 2) == 0)	nDotCnt = 0;
	if (strncmp(psSymbol, "SC", 2) == 0)	nDotCnt = 1;
	if (strncmp(psSymbol, "FD", 2) == 0)	nDotCnt = 1;

	return nDotCnt;
}

void change_etk_to_normal(char* etk_symbol, char* pOut)
{
	// URO==>6E
	if (strncmp(etk_symbol, "URO", 3) == 0)
		sprintf(pOut, "6E%.3s", etk_symbol + 3);
	else if (strncmp(etk_symbol, "BP", 2) == 0)
		sprintf(pOut, "6B%.3s", etk_symbol + 2);
	else if (strncmp(etk_symbol, "JY", 2) == 0)
		sprintf(pOut, "6J%.3s", etk_symbol + 2);
	else
		sprintf(pOut, "%s", etk_symbol);

}