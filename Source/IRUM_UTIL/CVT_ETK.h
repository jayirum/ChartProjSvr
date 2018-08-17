/************************************************************************
	eBest API 와의 CONVERTING 을 위한 클래스
************************************************************************/

#if !defined(AFX_CVT_ETK_H__B4A61FF7_9929_4BE2_A216_B742A83110D3__INCLUDED_)
#define AFX_CVT_ETK_H__B4A61FF7_9929_4BE2_A216_B742A83110D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>
//#include "../IRUM_INC/Packet.h"
#include "../IRUM_INC/eBest/O01.h"	//	주문접수 실시간
#include "../IRUM_INC/eBest/H01.h"	//	정정취소 실시간
#include "../IRUM_INC/eBest/C01.h"	//	체결 실시간
#include "../IRUM_INC/eBest/t5511.h"	//	주문 요청
#include "../IRUM_INC/eBest/t5512.h"	//	정정 요청
#include "../IRUM_INC/eBest/t5513.h"	//	취소 요청



class CCVT_ETK
{
public:
	CCVT_ETK();
	virtual ~CCVT_ETK();

	VOID en_KS_TR_Order( ST_SV_ORD_CHK_RSLT* pInPack, LPCSTR i_psCommMedia, char* pOut);
	VOID en_KS_TR_Order_New( ST_SV_ORD_CHK_RSLT* pInPack, LPCSTR i_psCommMedia, char* pOut);
	VOID en_KS_TR_Order_Mdfy( ST_SV_ORD_CHK_RSLT* pInPack, LPCSTR i_psCommMedia, char* pOut);
	VOID en_KS_TR_Order_Cncl( ST_SV_ORD_CHK_RSLT* pInPack, LPCSTR i_psCommMedia, char* pOut);
	
	//	ETK 포맷으로 ENTRYPT 할 때 이 함수 사용 필요
	//	ETK 예제 stdafx.cpp 에 있는 것 copy
	void ETK_SetPacketData( char* psData, int nSize, LPCTSTR pszSrc, int nType, int nDotPos=0 );

	VOID de_KS_TR_Order( char* i_etkData, char* i_szGDS_CODE, char* i_szCLORD_NO, char* o_szLabel, char* o_szBody);
	VOID de_KS_TR_Order_Mdfy( char* i_etkData, char* i_szGDS_CODE, char* i_szCLORD_NO, char* o_szLabel, char* o_szBody);
	VOID de_KS_TR_Order_Cncl( char* i_etkData, char* i_szGDS_CODE, char* i_szCLORD_NO, char* o_szLabel, char* o_szBody);

	VOID de_KS_RL_ORD(char* psOrgData, char* pOut);
	VOID de_KS_RL_MDFYCNCL(char* psOrgData, char* pOut);
	VOID de_KS_RL_CNTR(char* psOrgData, char* pOut);

	VOID de_KS_F_Sise(char* psOrgData, char* pOut);
	
	//시세오류고려.	VOID de_KS_F_SiseEx(char* psOrgData, char* pOut, char* pOutForClient);
	BOOL de_KS_F_SiseEx(char* psOrgData, char* pOut, char* pOutForClient, double* pErrSise);

	//SMILOR0803.VOID de_KS_F_Hoga(char* psOrgData, char* pOut);
	BOOL de_KS_F_Hoga(char* psOrgData, char* pOut);
	BOOL de_KS_F_HogaDongsi(char* psOrgData, char* pOut, BOOL *o_bDongsi/*동시호가여부*/);

	//CME
	VOID de_KS_F_SiseEx_CME(char* psOrgData, char* pOut, char* pOutForClient);
	BOOL de_KS_F_Hoga_CME(char* psOrgData, char* pOut);

	VOID de_KS_O_Sise(char* psOrgData, char* pOut);
	VOID de_KS_O_SiseEx(char* psOrgData, char* pOut, char* pOutForClient);
	VOID de_KS_O_Hoga(char* psOrgData, char* pOut);

	BOOL de_FU_SiseEx(char* psOrgData, char* pOut,  double* pErrSise);
	BOOL de_FU_Hoga(char* psOrgData, char* pOut);


	//	ETK 패킷은 소숫점이 없다. 이를 소수점이 있는 숫자로 만든다.
	char* get_double_withdot( char* i_psOrg, int i_nOrgLen, int i_nDotCnt, int i_nOutLen, char* o_psOut );
	char* get_double_withdot2( char* i_psStkCode, char* i_psOrg, int i_nOrgLen, int i_nOutLen, char* o_psOut );

};





#endif // !AFX_CVT_ETK_H__B4A61FF7_9929_4BE2_A216_B742A83110D3__INCLUDED_

