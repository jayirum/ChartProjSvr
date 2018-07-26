#include "UserStratOrd.h"

/*
 char cSideOption : A-모두, S-특정SIDE만, 'N'-DEFAULT
*/
VOID CUserStratOrd::Cncl_LimitOrd(char* pzSymbol, char cSide, int nOrdNo, char cSideOption)
{
	// 특정번호 취소
	if (cSideOption=='N')
	{
		ST_ORD* p = NULL;
		if (m_UserInfo.OrdInfo.GetLimitOrdInfoByOrdNo(nOrdNo, p))
		{
			// 주문전송.
// char cOrdTp, char cOrdProcTp,char* pzPrc,char* pzOrigPrc,char cSide,int nQty,char cClrYN,int nOrigNo,char cApiYN, BOOL bCnclAllQty,char* psChartNm
			SendSaveStratOrd(CD_ORD_TP_LIMIT, CD_ORD_PROC_CNCL,
				p->zPrc, p->zPrc, p->cSide[0], p->nRemainQty, 'N', p->nOrdNo,'Y', TRUE, "");

			delete p;
		}
		return;
	}


	// 전방향 전부 취소
	if (cSideOption='A') {
		SendSaveStratOrd(CD_ORD_TP_LIMIT, CD_ORD_PROC_SYMBOL_CNCL,"", "", 0x20, 0, 'N', 0, 'Y', TRUE, "");
		return;
	}

	char* pArr = NULL;
	int nCnt = m_UserInfo.OrdInfo.GetOrdInfoBySide(pzSymbol, CD_ORD_TP_LIMIT, cSide, pArr);
	if (nCnt > 0)
	{
		for (int i = 0; i < nCnt;i++)
		{
			ST_ORD* p = (ST_ORD*)(pArr + (i * sizeof(ST_ORD)));

			SendSaveStratOrd(CD_ORD_TP_LIMIT, CD_ORD_PROC_CNCL,
				p->zPrc, p->zPrc, p->cSide[0], p->nRemainQty, 'N', p->nOrdNo, 'Y', TRUE, "");
		}
		delete[] pArr;
	}
}


/*
RETURN : 취소한 주문 갯수
*/
INT CUserStratOrd::Cncl_MITOrd(char* pzSymbol, char cSide, BOOL bBothSideAll)
{
	INT nDelCnt = 0;
	
	char cDelSide = (bBothSideAll == TRUE) ? CD_BUY : cSide;

	// 먼저 한쪽방향 모두 취소
	char* pArr = NULL;
	int nCnt = m_UserInfo.OrdInfo.GetOrdInfoBySide(pzSymbol, cDelSide, CD_ORD_TP_MIT, pArr);
	if (nCnt > 0)
	{
		for (int i = 0; i < nCnt;i++)
		{
			ST_ORD* p = (ST_ORD*)(pArr + (i * sizeof(ST_ORD)));

			if (p->cOrdTp[0] != CD_ORD_TP_MIT)
				continue;
			
			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_CNCL,
					p->zPrc, p->zPrc, p->cSide[0], p->nRemainQty, 'N', p->nOrdNo,'Y', TRUE, "");
			nDelCnt++;			
		}
		delete[] pArr;

		// 주문map 삭제
		m_UserInfo.OrdInfo.DelOrdInfoBySymbol(pzSymbol, CD_ORD_TP_MIT, cDelSide, FALSE, FALSE);
	}

	// 한쪽방향 취소인 경우는 여기서 리턴
	if (!bBothSideAll)
		return nDelCnt;

	// 전방향 전부 취소인 경우 다른 한방향 마저 취소.
	char cOtherSide = (cSide == CD_BUY) ? CD_SELL : CD_BUY;
	nCnt = m_UserInfo.OrdInfo.GetOrdInfoBySide(pzSymbol, cOtherSide, CD_ORD_TP_MIT, pArr);
	if (nCnt > 0)
	{
		for (int i = 0; i < nCnt;i++)
		{
			ST_ORD* p = (ST_ORD*)(pArr + (i * sizeof(ST_ORD)));

			if (p->cOrdTp[0] != CD_ORD_TP_MIT)
				continue;

			SendSaveStratOrd(CD_ORD_TP_MIT, CD_ORD_PROC_CNCL,
				p->zPrc, p->zPrc, p->cSide[0], p->nRemainQty, 'N', p->nOrdNo, 'Y', TRUE, "");
			nDelCnt++;

		}
		delete[] pArr;

		// 주문map 삭제
		m_UserInfo.OrdInfo.DelOrdInfoBySymbol(pzSymbol, CD_ORD_TP_MIT, cOtherSide, FALSE, FALSE);
	}
	return nDelCnt;
}
