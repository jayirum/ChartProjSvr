
#include "UserStratOrd.h"
#include "../../IRUM_UTIL/LogMsg.h"
#include "../../IRUM_UTIL/util.h"
#include "../../IRUM_INC/IRUM_Common.h"
#include "Main.h"

extern CLogMsg g_log;
extern char	g_zConfig[_MAX_PATH];



// �ֹ�����
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
COrdInfo::COrdInfo()
{
	
}

COrdInfo::~COrdInfo()
{
	std::map<int, ST_ORD*>::iterator it;
	for (it = m_mLimit.begin();it != m_mLimit.end();it++)
		delete (*it).second;

	m_mLimit.clear();

	IT_MIT it2;
	for (it2 = m_mMIT.begin();it2 != m_mMIT.end();it2++)
	{
		IT_MIT_INNER itIn;
		for (itIn = (*it2).second.begin();itIn != (*it2).second.end();itIn++)
			delete (*itIn).second;
	}
	m_mMIT.clear();
}


VOID COrdInfo::SetOrdInfo(char* pzSymbol, char* pzStratID, char cOrdTp, char* pzOrdNo, char* pzOrigNo, 
							char* pzRemainQty, char cSide, char* pzPrc, char* pzApiDT, char* pzApiTM)
{
	ST_ORD* p = new ST_ORD;

	FORMAT_SYMBOL(pzSymbol);
	strcpy(p->zSymbol, pzSymbol);
	p->cOrdTp[0] = cOrdTp;
	p->nOrdNo = atoi(pzOrdNo);
	p->nOrigNo = atoi(pzOrigNo);
	p->nRemainQty = atoi(pzRemainQty);
	p->cSide[0] = cSide;
	FORMAT_PRC_STR(pzPrc, LEN_PRC, p->zPrc);
	strcpy(p->zStratID, pzStratID);
	ZeroMemory(p->zChartNm, sizeof(p->zChartNm));
	strcpy(p->zApiDT, pzApiDT);
	strcpy(p->zApiTM, pzApiTM);
	TODAY_NONE_DOT(p->zSysDT);
	NOWTIME_NONE_DOT(p->zSysTM);

	if (cOrdTp == CD_ORD_TP_LIMIT) {
		IT_LIMIT it = m_mLimit.find(p->nOrdNo);
		if (it != m_mLimit.end())
		{
			delete (*it).second;
			m_mLimit.erase(it);
		}
		m_mLimit[p->nOrdNo] = p;
	}
	if (cOrdTp == CD_ORD_TP_MIT) 
	{
		std::string sPrc = p->zPrc;
		IT_MIT itMIT = m_mMIT.find(cSide);
		
		// ���� ������ map �� ���� ���
		if (itMIT == m_mMIT.end())
		{
			m_mMIT[cSide][sPrc] = p;
		}
		if (itMIT != m_mMIT.end())
		{
			IT_MIT_INNER inMit = (*itMIT).second.find(sPrc);
			if (inMit != (*itMIT).second.end())
			{
				delete (*inMit).second;
				(*itMIT).second.erase(inMit);
			}
			(*itMIT).second[sPrc] = p;
		}
	}
}

/*
	Ư���ֹ���ȣ �ֹ����� ����Ʈ���� ����
*/
INT COrdInfo::DelLimitOrdInfoByOrdNo(char* pzOrdNo)
{
	INT nOrdNo = atoi(pzOrdNo);
	std::map<INT, ST_ORD*>::iterator it = m_mLimit.find(nOrdNo);
	if (it == m_mLimit.end())
		return 0;

	m_mLimit.erase(it);
	delete (*it).second;
	return 1;
}

/*
	Ư�������� MIT ����

	- pzPrc �� formatting �Ǿ� �־�� �Ѵ�.
*/
INT COrdInfo::DelMITOrdInfoByPrc(char cSide, char* pzFormatPrc)
{
	std::string sPrc = pzFormatPrc;

	int nCnt = 0;

	IT_MIT itMIT = m_mMIT.find(cSide);

	// ���� ������ map �� ���� ���
	if (itMIT == m_mMIT.end())
	{
		return nCnt;
	}

	if (itMIT != m_mMIT.end())
	{
		IT_MIT_INNER inMit = (*itMIT).second.find(sPrc);
		if (inMit != (*itMIT).second.end())
		{
			delete (*inMit).second;
			(*itMIT).second.erase(inMit);
			nCnt++;
		}
	}
	return nCnt;
}

/*
	Ư���ֹ���ȣ �ֹ����� ��ȯ

	// �ݵ�� �ۿ��� delte o_pInfo �ؾ� �Ѵ�.
*/
BOOL COrdInfo::GetLimitOrdInfoByOrdNo(int nOrdNo, ST_ORD* o_pInfo)
{
	std::map<INT, ST_ORD*>::iterator it = m_mLimit.find(nOrdNo);
	if (it == m_mLimit.end())
		return FALSE;

	o_pInfo = ((*it).second);
	return TRUE;
}


/*
	�ش� ���� ���� ���� ��� �ֹ����� ��ȯ
	pzSymbol �� formatting �Ǿ ���´�.
	return : ��� �ֹ����� ����
	o_pArrOrdInfo : ��� �ֹ����� array
	==> �ݵ�� �ۿ��� delete[] o_pArrOrdInfo �ؾ��Ѵ�.
*/
int  COrdInfo::GetOrdInfoBySide(char* pzSymbol, char cOrdTp, char cSide, char* o_pArrOrdInfo)
{
	FORMAT_SYMBOL(pzSymbol);
	IT_MIT itMIT;
	IT_LIMIT itLimit;

	int nCnt = 0;
	if (cOrdTp == CD_ORD_TP_LIMIT)	nCnt = m_mLimit.size();
	if (cOrdTp == CD_ORD_TP_MIT) {
		itMIT = m_mMIT.find(cSide);
		if (itMIT == m_mMIT.end())
			nCnt = 0;
		else
		{
			nCnt = (*itMIT).second.size();
		}
	}

	if (nCnt == 0)
		return 0;

	o_pArrOrdInfo = (char*)new ST_ORD[nCnt];
	int nSize = sizeof(ST_ORD);
	int nLoop = 0;

	if (cOrdTp == CD_ORD_TP_LIMIT)
	{
		IT_LIMIT it;
		for (it = m_mLimit.begin(); it != m_mLimit.end();it++)
		{
			ST_ORD* p = (*it).second;
			if (strncmp(p->zSymbol, pzSymbol, strlen(pzSymbol)) == 0)
			{
				if (p->cSide[0] == cSide)
				{
					char* pDest = o_pArrOrdInfo + (nLoop*nSize);
					memcpy(pDest, p, nSize);
					nLoop++;
				}
			}
		}
	}
	if (cOrdTp == CD_ORD_TP_MIT)
	{
		itMIT = m_mMIT.find(cSide);
		IT_MIT_INNER itInner;
		for (itInner = (*itMIT).second.begin(); itInner != (*itMIT).second.end();itInner++)
		{			
			ST_ORD* p = (*itInner).second;
			if (strncmp(p->zSymbol, pzSymbol, strlen(pzSymbol)) == 0)
			{
				char* pDest = o_pArrOrdInfo + (nLoop*nSize);
				memcpy(pDest, p, nSize);
				nLoop++;
			}
		}
	}
		
	return nLoop;
}

/*
	bAllTp : true - ��� Ÿ��
	SYMBOL �� formatting �Ǿ ���´�.
*/
INT COrdInfo::DelOrdInfoBySymbol(char* pzSymbol, char cOrdTp, char cSide, BOOL bAllSide, BOOL bAllTp)
{
	INT nCnt = 0;
	BOOL bLimitDel = FALSE, bMITDel = FALSE;
	if (bAllTp) {
		bLimitDel = bMITDel = TRUE;
	}
	else
	{
		if (cOrdTp == CD_ORD_TP_LIMIT)	bLimitDel = TRUE;
		if (cOrdTp == CD_ORD_TP_MIT)	bMITDel = TRUE;
	}

	if (bLimitDel)
	{
		IT_LIMIT it;
		for (it = m_mLimit.begin(); it != m_mLimit.end(); )
		{
			ST_ORD* p = (*it).second;
			if (strncmp(p->zSymbol, pzSymbol, strlen(pzSymbol)) == 0)
			{
				if (bAllSide) {
					it = m_mLimit.erase(it);
					nCnt++;
				}
				else
				{
					if(p->cSide[0]==cSide) {
						it = m_mLimit.erase(it);
						nCnt++;
					}
				}
				continue;
			}
			else
				it++;
		}
	}


	if (bMITDel)
	{
		IT_MIT itMit;
		for (itMit = m_mMIT.begin(); itMit != m_mMIT.end(); )
		{
			// ���� ��� �ƴϸ鼭 ������ �ٸ��� ó������ �ʴ´�.
			if (!bAllSide && cSide != (*itMit).first) {
				itMit++;
				continue;
			}


			IT_MIT_INNER itIn;
			for (itIn = (*itMit).second.begin(); itIn != (*itMit).second.end(); )
			{
				ST_ORD* p = (*itIn).second;
				if (strncmp(p->zSymbol, pzSymbol, strlen(pzSymbol)) == 0)
				{
					itIn = (*itMit).second.erase(itIn);
					nCnt++;
					continue;
				}
				else
					itIn++;
			}

			itMit = m_mMIT.erase(itMit);
		}
	}

	return nCnt;
}


// ����������
////////////////////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
CPosInfo::CPosInfo()
{
	
}

CPosInfo::~CPosInfo()
{
	std::map<std::string, ST_POS*>::iterator it;
	for (it = m_map.begin();it != m_map.end();it++)
		delete (*it).second;

	m_map.clear();
}

/*
	TR �����ͷ� ADD
*/
VOID CPosInfo::SetPosInfoTR(char* pzSymbol, char* pzOpenQty, char cSide, char* pzPosPrc, char* pzOpenTM, CHART_TP tp)
{
	ST_POS *pos = new ST_POS;;
	FORMAT_SYMBOL(pzSymbol);
	std::string sSymbol = std::string(pzSymbol);

	strcpy(pos->zSymbol, pzSymbol);
	pos->nOpenQty = atoi(pzOpenQty);
	pos->cSide[0] = cSide;
	FORMAT_PRC_STR(pzPosPrc, LEN_PRC, pos->zPosPrc);

	sprintf(pos->zOpenTM,"%.2s:%.2s:%.2s", pzOpenTM, pzOpenTM+2, pzOpenTM+4);	//hhmmssmmm 
	// ������ ��ȸ�� �ش� ������ ���� ���ڿ� �ð��� �� �� �����Ƿ�, ������ �ð��� �̿��Ѵ�.
	SYSTEMTIME st;
	GetLocalTime(&st);
	char zDate[9];
	sprintf(zDate, "%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
	GET_CHART_NM_EX(zDate, pos->zOpenTM, tp, pos->zChartNm);

	m_map[sSymbol] = pos;
}


/*
real �����ͷ� ADD
*/
VOID CPosInfo::SetPosInfoReal(char* pzSymbol, int nPosQty, char cSide, char* pzPosPrc, char* pzDT, char* pzTM, CHART_TP tp)
{
	ST_POS *pos = new ST_POS;;
	FORMAT_SYMBOL(pzSymbol);
	std::string sSymbol = std::string(pzSymbol);

	strcpy(pos->zSymbol, pzSymbol);
	pos->cSide[0] = cSide;
	FORMAT_PRC_STR(pzPosPrc, LEN_PRC, pos->zPosPrc);

	sprintf(pos->zOpenTM, pzTM);
	GET_CHART_NM_EX(pzDT, pzTM, tp, pos->zChartNm);

	m_map[sSymbol] = pos;
}

VOID CPosInfo::DelPosInfo(char *pzSymbol)
{
	FORMAT_SYMBOL(pzSymbol);
	std::string symbol = pzSymbol;
	std::map<std::string, ST_POS*>::iterator it = m_map.find(symbol);
	if (it == m_map.end())
		return ;

	m_map.erase(it);
	delete (*it).second;
}

BOOL CPosInfo::IsOpened(char *pzSymbol)
{
	FORMAT_SYMBOL(pzSymbol);
	std::string symbol = pzSymbol;
	std::map<std::string, ST_POS*>::iterator it = m_map.find(symbol);
	if ( it== m_map.end())
		return FALSE;

	return( ((*it).second)->nOpenQty > 0);
}


BOOL CPosInfo::IsSameChart(char *pzSymbol, char* psChartNm)
{
	FORMAT_SYMBOL(pzSymbol);
	std::string symbol = pzSymbol;
	std::map<std::string, ST_POS*>::iterator it = m_map.find(symbol);
	if (it == m_map.end())
		return FALSE;

	char zChartNm[LEN_CHART_NM + 1];
	sprintf(zChartNm, "%.*s", LEN_CHART_NM, ((*it).second)->zChartNm);
	if (zChartNm[0] == 0x00)	return FALSE;

	int nComp = strncmp(zChartNm, psChartNm, LEN_CHART_NM);
	return (nComp >= 0);
}

char CPosInfo::PosSide(char* pzSymbol)
{
	FORMAT_SYMBOL(pzSymbol);
	std::string symbol = pzSymbol;
	std::map<std::string, ST_POS*>::iterator it = m_map.find(symbol);
	if (it == m_map.end())
		return FALSE;

	return ((*it).second)->cSide[0];
}