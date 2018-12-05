#ifndef  __G7_COMMON_PACKET_20050220__
#define  __G7_COMMON_PACKET_20050220__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "COMMON.h"

#define MAX_BUFSIZE		1024L
#define MAX_FILESIZE	4096L
#define	MAX_TRSIZE		2048L
#define	MAX_SENDSIZE	4096L
#define	LEN_SIZE		4L
#define CORRECT_LEN		6L
#define MAX_UPGRADE		6144L	//	6k
#define MAX_LOGGING		1024

#define MAX_ACNT_NO		11
#define MAX_USERID		12
#define MAX_STK_CODE	32
#define MAX_GDS_CODE	2
#define MAX_ARTC_CODE	10
#define MAX_ORD_NO		9
#define MAX_ORD_QTY		9
#define MAX_ORD_PRC		20
#define MAX_ORD_PRC_DOT	8

#define MAX_ORD_NO_EX		(20)	// ++ skeo 2011-04-29	

#define SET_USERID(id)	{							\
	char temp[MAX_USERID+1];						\
	int	len = min( MAX_USERID, strlen(id) );		\
	sprintf( temp, "%-*.*s", MAX_USERID, len, id);	\
	strcpy( id, temp);								\
}

#define	SET_LEN( i_len, o_len ) sprintf( o_len, "%0*d", LEN_SIZE, i_len-CORRECT_LEN )

//#define USE_USERCCYPL	1

//########################################################
//##	//HEADER, C_HEADER �� ���� ��Ŷ��
//##	Stx, Etx �� �ٿ��� ����ؾ� �Ѵ�.
//#########################################################


/**	\brief	CLIENT - SERVER �� ���
*/
typedef struct  _ST_CLIENT_HEADER
{
	char	Len		[LEN_SIZE];		// ���� 
	char	Code	[5];		// �ڵ� 
}C_HEADER, * LPCLIENT_HEADER;	//	9
#define C_HEADER_LEN	sizeof(C_HEADER)

/**	\brief	CLIENT - SERVER �� TR ��Ŷ�� ����ϴ� TR //HEADER

	*	//HEADER + TRHEADER

	*	MsgGb	: ����ڵ� (N, E)
		EndGb	: ��Ŷ�� ������ ���۵� �� ������ ��Ŷ���� ����
		DataHeaderSize : TR_HEADER ���� ������ ���� �� �ش� ������ �����ϴ� SIZE
*/
typedef struct _TR_HEADER
{
	char	MsgGb	[1];		//	'N' OK, 'E' ERROR
	char	MsgCode	[4];
	char	FormNo	[4];
	char	EndGb	[1];		//	1: end, 0:not end
	char	DataHeaderSize[4];	//	14
}TR_HEADER, * LPTR_HEADER;

#define TRHEADER_ENDGB_END	'1'
#define TRHEADER_ENDGB_NEXT	'0'
#define TRHEADER_MSGGB_OK	'N'
#define TRHEADER_MSGGB_ERR	'E'

/*==================================================================================
/////////////		UPGRADE PACKET		/////////////
==================================================================================*/
/**	\brief	���׷��̵�� ��Ŷ �� CLIENT �� SERVER �� �����ϴ� ��Ŷ

	*	Complete	:	������ �ٿ�ε尡 �Ϸ�Ǿ����� ���� (0-�̿�, 1-�Ϸ�)
	*	LastDate	:	���� �ٿ�ε� ����
	*	FileName	:	���� �ٿ�ε尡 �Ϸ���� �ʾ��� ��(Complete �� 0 �϶�)
						���������� �ٿ�ε��� ���ϸ�
*/
typedef struct _ST_UPGRADE_TR
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;
	char		Complete	[1];	// ���� �ٿ�ε�ϷῩ�� (0-�̿�  1-�Ϸ�)
	char		LastDate	[14];	// YYYYMMDDHHMMSS
	char		FileName	[32];	// ���� �̸�. client ���� ���� �ٿ�ε尡 �������� ������ 
									// �ø��� ���� ���ϸ��� �ѱ��.	
	char		FilePath	[32];	// ���� ���丮 (Root �� ���� space)
	char		ETX			[1];
} ST_UPGRADE_TR, *PTR_UPGRADE_TR;
#define SIZE_UPGRADE_TR	sizeof(ST_UPGRADE_TR)

/**	\brief	���׷��̵� ���� �� SERVER �� CLIENT ���� �����ϴ� ��Ŷ

	*	�� ������ ������ ���۵Ǿ�� �� �� ���� �����ϴ� ��Ŷ

	*	FileFirstCheck	:	1 - ���� ����, 0 - ���� �ƴϴ�, 2 - ������ ����.
		FileLastCheck	:	1 - ������, 0 - ������ �ƴϴ�.
		Binary			:	���� ������ ������
*/
typedef struct _PKT_UPGRADE 
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;
	char		FileFirstCheck[1];	// 1 - ���� ����, 0 - not, 2 - ���� ����.
	char		FileLastCheck[1];	// 1 - ���� ������, 0 - not
	char		FileCnt		[4];		// ���׷��̵� �� ���� ����
	char		FileDate	[14];	// YYYYMMDDHHMMSS
	char		FileName	[32];	// ���� �̸�
	char		FilePath	[32];	// ���
	char		FileSize	[8];	// ���� ũ��	:: �̻� Data Header

	char		Binary[MAX_FILESIZE];

	char		ETX[1];
} PKT_UPGRADE, *PTR_PKT_UPGRADE;
#define SIZE_PKT_UPGRADE sizeof(PKT_UPGRADE)


/**	\brief	���׷��̵� ���� �� SERVER �� CLIENT ���� �����ϴ� ��Ŷ

	*	�� ������ ������ ���۵Ǿ�� �Ҷ� ���� ������ �����ϴ� ��Ŷ
*/

typedef struct _PKT_UPGRADE_NEXT
{
	char				STX[1];
	C_HEADER			Header;
	TR_HEADER			TRHeader;
	char				FileFirstCheck[1];
	char				FileLastCheck[1];
	char				Binary[MAX_FILESIZE];
	char				ETX[1];
} PKT_UPGRADE_NEXT, *PTR_PKT_UPGRADE_NEXT;
#define SIZE_PKT_UPGRADE_NEXT	sizeof(PKT_UPGRADE_NEXT)



typedef struct _PKT_TR
{
	char			STX[1];
	C_HEADER		Header;
	TR_HEADER		TRHeader;
	char			Data[MAX_TRSIZE];
	char			ETX[1];
}PKT_TR, *PTR_PKT_TR;



//////////////////////////////////////////////////////////////////////////
//	LOGIN PACKET
typedef struct _ST_TR_00013_C
{
	char	STX		[1];
	C_HEADER		Header;
	TR_HEADER		TRHeader;
	char			UserID				[MAX_USERID];
	char			Conn_Pwd			[10];
	char			CLIENT_IP_LOCAL		[15];
	char			CLIENT_IP_PUBLIC	[15];
	char			CLIENT_MAC			[12];
	char			Reserved			[100];
	char			ETX					[1];
}ST_TR_00013_C;

typedef struct _ST_TR_00013_S
{
	char	STX		[1];
	C_HEADER		Header;
	TR_HEADER		TRHeader;
	char			UserID		[MAX_USERID]; 	
	char			SERVER_IP	[15];
	char			SERVER_PORT	[5]; 	
	char			Reserved	[100];
	char			ETX			[1];
}ST_TR_00013_S;


//////////////////////////////////////////////////////////////////////////
//	LOGIN COMMIT PACKET
typedef struct _ST_TR_00014_C
{
	char	STX		[1];
	C_HEADER		Header;
	TR_HEADER		TRHeader;
	char			UserID				[MAX_USERID];
	char			CLIENT_IP_LOCAL		[15];
	char			CLIENT_IP_PUBLIC	[15];
	char			CLIENT_MAC			[12];
	char			Reserved			[100];
	char			ETX					[1];
}ST_TR_00014_C;

typedef struct _ST_TR_00014_S
{
	char	STX		[1];
	C_HEADER		Header;
	TR_HEADER		TRHeader;
	char			UserID		[MAX_USERID]; 	
	char			Reserved	[100];
	char			ETX			[1];
}ST_TR_00014_S;

//	FRONT RELOAD
typedef struct _ST_TR_00018
{
	char		STX			[ 1];
	C_HEADER	Header			;
	TR_HEADER	TRHeader		;
	char		ETX			[ 1];
}ST_TR_00018;

//////////////////////////////////////////////////////////////////////////
//	�ֹ�
typedef struct _ST_TR_20000
{
	char		STX	[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;

	char		GDS_CODE	[2];
	char		UserID		[MAX_USERID];
	char		AcntPwd		[4];
	char		ACNT_NO		[MAX_ACNT_NO];
	char		STK_CODE	[MAX_STK_CODE];
	char		ORD_ST		[1];			//	���尡, ������, ..
	char		COND_ORD_ST	[1];
	char		BUY_SELL_TP	[1];			//	1:SELL, 2:BUY
	char		ORD_PROC_TP	[1];			//	�ű�, ����, ���
	char		ORG_ORD_NO	[MAX_ORD_NO];	//	
	char		ORG_ORD_PRC	[MAX_ORD_PRC];	//	20,8 (�Ҽ����� 8)	//	���/���� �ֹ����� ���

	char		ORD_PRC		[MAX_ORD_PRC];	//	�ű��ֹ�����, ������ ��� �������� ONLY, 20,8 (�Ҽ����� 8)
	char		ORD_QTY		[MAX_ORD_QTY];	//	�ֹ�����
// 	char		MDFY_ORD_PRC[20];			//	�����ֹ� ONLY 20,8 (�Ҽ����� 8)
// 	char		MDFY_ORD_QTY[9];			//	�����ֹ��� ��� ONLY
// 	char		CNCL_ORD_QTY[9];			//	����ֹ��� ��� ONLY
	char		ALL_YN		[1];			//	ALL Cancel/Modify? or Partially cancel/modify?
	char		MKT_RNG		[3];			//	Slippage ����
	char		SL_PRC		[MAX_ORD_PRC];	//	20,8 (�Ҽ����� 8)//	�����ֹ�(STOP/LIMIT) �ֹ�ȯ��
	char		TS_RNG		[3];			//	TS ���� (TICK)
	char		IFD_YN		[1];
	char		ORD_TERM_TP	[1];
	char		FCM_CODE	[5];
	char		FORCE_ORD_YN[1];			//	 
	char		MNG_USERID	[MAX_USERID];
	
	//char		ORG_ORD_NO_FU[MAX_ORD_NO_EX];//	�ϳ����� �ؿܼ��� �ֹ���ȣ. ++ skeo 2011-04-29		

	char		ETX	[1];
}ST_TR_20000;


// ���缱��(FU) �ֹ� ��Ŷ
typedef struct _DONGYANG_FU_TR_20000_I
{
	char		API_USERID		[MAX_USERID];
	char		API_ACNT_NO		[MAX_ACNT_NO];
	char		API_ACNT_PWD	[4];
	char		STK_CODE		[MAX_STK_CODE];
	char		ORD_PRC			[MAX_ORD_PRC];	//	�ű��ֹ�����, ������ ��� �������� ONLY, 20,8 (�Ҽ����� 8)
	char		ORD_QTY			[MAX_ORD_QTY];	//	�ֹ�����
	char		BUY_SELL_TP		[1];			//	1:�ż�, 2:�ŵ�
	char		ORD_ST			[1];			//	1: ������, 2: ���尡
	char		ORD_TERM_TP		[1];			//	1: FAS, 3: FAS
	char		ORD_ST2			[1];			//	1: �Ϲ�, 2: STOP
	char		STOP_PRC		[MAX_ORD_PRC];	//	STOP ORDER ��������
	char		CLORD_NO		[MAX_ORD_NO];
	char		PROC_TP			[1];			// 1:NEW, 2:MODIFY, 3:CANCEL
	char		ORG_ORD_NO		[MAX_ORD_NO];	// Modify and Cancel Order
}DONGYANG_FU_TR_20000_I;


// �ϳ�����(KS) �ֹ� ��Ŷ
typedef struct _HANADT_FU_TR_20000_I
{
	char		Rcnt		[4];	// ó�� �Ǽ�
	char		Odgb		[1];	// 1:�Ϲ�, 2:�����ɼ�
	char		Mkgb		[1];	// 1:�ŷ���, 2:�ڽ���, 3:��������, 5:���
	char		Mmgb		[1];	// 1:�ŵ�, 2:�ż�, 3:����, 4:���
	char		Acno		[10];
	char		Pswd		[8];
#if _NO_NINE	// �ֹ���ȣ�ڸ��� 9 �ڸ� (SAEHAN, BLASH ������))
	char		Ogno		[12];	// ����/��ҽ� ���ֹ���ȣ
#else
	// ++ skeo 2011-04-29	
	char		Ogno		[MAX_ORD_NO_EX];	// ����/��ҽ� ���ֹ���ȣ	
#endif
	char		Code		[12];	// �����ڵ�
	char		Jqty		[8];	// �ֹ�����
	char		Jprc		[10];	// �ֹ�����(�����ɼ��� ��� 100��� ó�� [ex] 112.13 -> 11213
	char		Hogb		[2];	// 00:������, 03:���尡, 05:���Ǻ�������, 06:������������, 07:�ֿ켱������
	char		Jmgb		[2];	// 01:IOC, 02:FOK
	char		Mdgb		[1];	// 1:�Ϻ�, 2:����
	char		Prgb		[1];	// 'X'
}HANADT_FU_TR_20000_I;


//	OrdChk class ���� ��ȯ�� ����
typedef struct _ST_SV_ORD_CHK_RSLT
{
	char			MINI_YN		[1];	//	MINI �� ����
	char			API_CODE	[11];
	char			API_ACNT_NO	[20];
	char			API_PWD		[10];
	char			ClORD_NO	[MAX_ORD_NO];	//	�츮�� ������ �ֹ���ȣ
	char			ORD_PRC		[MAX_ORD_PRC];
	char			ORD_QTY		[MAX_ORD_QTY];
//	char			SERVER_IP	[15];
	char			ORD_TM		[9];	//	HHMMSSmmm
	char			API_USERID	[MAX_USERID];
	char			NTTN_CODE	[2];
	char			Reserved	[6];	//���亿. 2byte �� ü������ ����Ѵ�.(%��)
	ST_TR_20000		stC;
	
}ST_SV_ORD_CHK_RSLT;




/************************************************************************
	API ���� ����
************************************************************************/

//	IF ���α׷��� MQ �� ���� �� ä���� �� ����
typedef struct	_ST_MQLABEL_API_INFO
{
	char	CODE		[2];			//	TR, RL
	char	GDS_CODE	[2];
	char	API_CODE	[11];
	char	NTTN_CODE	[2];	//	�ش� ������ �����ڵ�
}ST_MQLABEL_API_INFO;

// �ϳ����� �ؿܼ��� �ֹ���ȣ ����. skeo 2011-04-29		
#define HANADT_FU_ORDER_NO_LEN		14	

//	�� API���� ���� �ֹ�TR ���䵥���͸� ��ȯ�� ���� ����
typedef struct _ST_IFRECV_TR_ORD
{
	char	GDS_CODE	[2];
	char	CLORD_NO	[MAX_ORD_NO];	//	CLIENT ORDER NO
#if _NO_NINE	// �ֹ���ȣ�ڸ��� 9 �ڸ� (SAEHAN, BLASH ������))
	char	ORD_NO		[MAX_ORD_NO];	//	API ORD NO
#else
	// ++ skeo 2011-04-29	
	char	ORD_NO		[MAX_ORD_NO_EX];	//	API ORD NO
#endif
	char	RSLT_CODE	[4];			//	�������θ� �츮�� RET_CODE ������ ǥ��(RET_SUCCESS or RET_ORD_RJCT)
	char	API_CODE	[11];
	char	API_MSGCODE	[6];
	char	API_MSG		[100];
}ST_IFRECV_TR_ORD;

//	�� API���� ���� ü��REAL ���䵥���͸� ��ȯ�� ���� ����
typedef struct _ST_IFRECV_REAL_CNTR
{
	char	ORD_RSLT_CODE	[4];	//	0~3	9000(�ű��ֹ�)/9001(����)/9002(���)/9003(�ź�)/9004(ü��)/9005(�ֹ�TR)
	char	GDS_CODE		[2];	//	4~5
	char	API_CODE		[11];	//	6~16
	char	STK_CODE		[MAX_STK_CODE];	//	17~48
	char	API_USERID		[MAX_USERID];	//	49~60
	char	API_ACNT_NO		[20];			//	61~80
#if _NO_NINE	// �ֹ���ȣ�ڸ��� 9 �ڸ� (SAEHAN, BLASH ������))
	char	API_ORD_NO		[MAX_ORD_NO];	//	81~89
	char	ClORD_NO		[MAX_ORD_NO];	//	82~98
	char	API_ORD_ORG_NO	[MAX_ORD_NO];	//	99~107
	char	API_CNTR_NO		[MAX_ORD_NO];	//	108~116
#else
	// ++ skeo 2011-04-29	
	char	API_ORD_NO		[MAX_ORD_NO_EX];	//	
	char	ClORD_NO		[MAX_ORD_NO];		//	
	char	API_ORD_ORG_NO	[MAX_ORD_NO_EX];	//	
	char	API_CNTR_NO		[MAX_ORD_NO_EX];	//	
#endif
	char	API_MKT_TP		[1];		//	���屸��(1/5/6/7)
	char	ORD_ST			[1];
	char	PROC_TP			[1];
	//char	ACPT_TP			[1];
	char	BUY_SELL_TP		[1];
	char	ORD_QTY			[MAX_ORD_QTY];
	char	ORD_PRC			[MAX_ORD_PRC];
	char	CNTR_QTY		[MAX_ORD_NO];
	char	CNTR_PRC		[MAX_ORD_PRC];
	char	REMN_QTY		[MAX_ORD_NO];		//	��ü���ܷ�
	char	API_RJCT_CODE	[4];		//	�źνø�,
	char	API_RJCT_MSG	[40];		//	�źνø�
	char	DT				[8];
	char	TM				[9];		//	hhmmssmmm
}ST_IFRECV_REAL_CNTR;


//////////////////////////////////////////////////////////////////////////
//	MINI �� ���� ��Ŷ
// typedef struct _ST_MINI_TR_ORD_I
// {
// 	char	incnt		[4];	//	�ֹ�ī��Ʈ (zero padding)
// 	char	mapno		[5];	//	TR ����ϴ� MAP ��ȣ
// 	char	clikey		[9];	//	�ֹ�key(zero padding)
// 	char	gb			[1];	//	1:�ŵ�, 2:�ż�, 3:����, 4:���
// 	char	zAcntNo		[20];	//	'-'�� ��� ����. (space trailing)
// 	char	zInptPwd	[8];	//	���º�й�ȣ
// 	char	zFnoIsuNo	[15];	//	�����ڵ�(space trailing)
// 	char	lOrgOrdNo	[9];	//	zero padding
// 	char	Ord_ST		[2];	//	00(������), 03(���尡), 05(���Ǻ�������), 06(������������)
// 	char	dOrdPrc		[12];	//	zero padding
// 	char	lOrdQty		[15];	//	zero padding
// 	char	ip			[12];	//	199.99.9.90 -> 199099009090
// 	//	�̻� ���� API �״��
// 	
// 	char	ord_term_tp	[1];	//	DAILY, GTC, FOK, IOC
// }ST_MINI_TR_ORD_I;
// 
// typedef struct _ST_MINI_TR_ORD_O
// {
// 	char	outcnt		[4];	//	�ֹ�ī��Ʈ (zero padding)
// 	char	clikey		[9];	//	�ֹ�key(zero padding)
// 	char	lOrdNo		[9];	//	0 ������ ��� �ֹ����
// 	char	msgcd		[6];	//	
// 	char	msgcm		[100];	
// 	//	�̻� ���� API �״��
// 	
// 	char	ord_term_tp	[1];	//	DAILY, GTC, FOK, IOC
// 	
// }ST_MINI_TR_ORD_O;

//	�ֹ���Ŷ
typedef ST_SV_ORD_CHK_RSLT	ST_MINI_TR_ORD_I;
//	�ֹ�/ü�� �뺸
typedef ST_IFRECV_REAL_CNTR	ST_MINI_REAL_CNTR;


//////////////////////////////////////////////////////////////////////////
//	NOTIFY REAL PACKET
typedef struct _ST_REAL_84901
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;
	char		ItemCd		[2];	//	11:�帶�����뺸, 12:�帶���뺸
	char		Title		[80];	
	char		Data		[MAX_BUFSIZE];
	char		ETX			[1];
}ST_REAL_84901;




/*
//����ݽǽð�
  - �����������Ʈ(70302)���� �����Ȯ�� �� �� ��Ŷ�� Ŭ���̾�Ʈ�� ������
    Ŭ���̾�Ʈ�� TR�� �ٽ� ���� ���� ����
*/
typedef struct _ST_REAL_84902
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;
	char		RESERVED	[20];
	char		ETX			[1];
}ST_REAL_84902;


//	CLIENT ���� ������ �ֹ�/ü�� ��� ��Ŷ
typedef struct _ST_REAL_82004
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;

	char	ORD_RSLT_CODE	[4];
	char	GDS_CODE		[2];
	char	STK_CODE		[MAX_STK_CODE];
	char	USERID			[MAX_USERID];
	char	ACNT_NO			[MAX_ACNT_NO];
	char	ORD_NO			[MAX_ORD_NO];
	char	ORD_ORG_NO		[MAX_ORD_NO];
	char	CNTR_NO			[MAX_ORD_NO];
	char	ORD_ST			[1];
	char	PROC_TP			[1];
	char	ACPT_TP			[1];
	char	BUY_SELL_TP		[1];
	char	ORD_QTY			[MAX_ORD_QTY];
	char	ORD_PRC			[MAX_ORD_PRC];
	char	CNTR_QTY		[MAX_ORD_NO];
	char	CNTR_PRC		[MAX_ORD_PRC];
	char	REMN_QTY		[MAX_ORD_NO];		//	��ü���ܷ�
	char	PRC_REMN_QTY	[MAX_ORD_QTY];
	char	DT				[8];
	char	TM				[9];				//	hhmmssmmm

	char	AFTER_CLR_PL_SUM[15];
	char	AFTER_POS_SUM	[MAX_ORD_QTY];		//	ü�� �� ������ ���� SUM
	char	AFTER_AVG_PRC	[MAX_ORD_PRC];
	char	AFTER_BUY_SELL_TP[1];

	char	AFTER_CAN_ORD_QTY_S	[4];	//	ü�� �� �ŵ� �ֹ����ɼ���
	char	AFTER_CAN_ORD_QTY_B	[4];	//	ü�� �� �ż� �ֹ����ɼ���

	char	NOW_PRC				[15];	//	���簡�� �ִ´�.
	char	CMSN				[5];
	char	STK_REMNQ_SELL		[4];	//	�ش� ������ ��ü���ܷ� ��ü
	char	STK_REMNQ_BUY		[4];
	char	TOT_EVLT_AMT		[15];
	char	EVLT_AMT			[15];
	
	char	ETX			[1];
}ST_REAL_82004;

//	CLIENT ���� ������ �ǽð� �ܰ�
typedef struct _ST_REAL_82005
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;
	
	char	ACNT_NO			[MAX_ACNT_NO];
	char	USERID			[MAX_USERID];
	char	ACPT_TP			[1];
	char	STK_CODE		[MAX_STK_CODE];
	char	AFTER_CAN_ORD_QTY_S	[4];	//	ü�� �� �ŵ� �ֹ����ɼ���
	char	AFTER_CAN_ORD_QTY_B	[4];	//	ü�� �� �ż� �ֹ����ɼ���
	char	NOW_PRC				[20];	//	���簡�� �ִ´�.
	char	STK_REMNQ_SELL		[4];	//	�ش� ������ ��ü���ܷ� ��ü
	char	STK_REMNQ_BUY		[4];
	char	AFTER_CLR_PL_SUM	[15];	//	�ش����� ����
	char	AFTER_POS_SUM		[MAX_ORD_QTY];		//	ü�� �� ������ ���� SUM
	char	AFTER_AVG_PRC		[MAX_ORD_PRC];
	char	AFTER_BUY_SELL_TP	[1];
	char	CMSN				[5];
	char	TOT_EVLT_AMT		[15];
	char	EVLT_AMT			[15];
	
	//	���� 20101124 ���� �߰�
	char	AFTER_TOT_PL_SUM	[15];	//	������ ����
	char	AFTER_REMNQ_SELL	[4];	//	������ ��ü���ܷ�
	char	AFTER_REMNQ_BUY		[4];	//	������ ��ü���ܷ�	

	//	20101216 ���� �߰�
	char	LossCutAmt			[15];	//	���������� �ν��� �ݾ�
	char	ETX			[1];
}ST_REAL_82005;


//////////////////////////////////////////////////////////////////////////
//	�ý��۽�ȣ ���� 1 (Ư������ ��������)
typedef struct _ST_REAL_83005
{
	char		STX			[1];
	C_HEADER	Header;
	TR_HEADER	TRHeader;

	char	GROUP_IDX	[2];
	char	STK_CODE	[MAX_STK_CODE];
	char	POS_QTY		[15];
	char	AVG_PRC		[MAX_ORD_PRC];
	char	EVLT_PL		[15];
	char	CLR_PL		[15];
	char	TOT_PL		[15];
	char	ETX			[1];
}ST_REAL_83005;


//////////////////////////////////////////////////////////////////////////
//	MASTER FILE
typedef	struct _MASTER_KSF
{
	char	STK_CODE	[MAX_STK_CODE];	
	char	STK_NM		[30];	
	char	ARTC_CODE	[MAX_ARTC_CODE];	//	ǰ���ڵ�
	char	ARTC_NM		[30];	//	ǰ���
	char	NTTN_CODE	[2]; 	//	�����ڵ�
	char	LSTNG_DT	[8];	//	������
	char	MTR_DT		[8];	//	������
	char	TICK_SIZE	[7];	//	xxxx.xx
	char	TICK_VALUE	[7];
} MASTER_KSF;


typedef	struct _MASTER_KSO
{
	char	STK_CODE		[MAX_STK_CODE];	
	char	STK_NM			[30];	
	char	ARTC_CODE		[MAX_ARTC_CODE];	//	ǰ���ڵ�
	char	ARTC_NM			[30];	//	ǰ���
	char	NTTN_CODE		[2]; 	//	�����ڵ�
	char	LSTNG_DT		[8];	//	������
	char	MTR_DT			[8];	//	������
	char	STRK_PRC		[MAX_ORD_PRC];	//	STRIKE PRICE
	char	TICK_CHG_STND	[7];
	char	TICK_SIZE_1		[7];	//	xxxx.xx
	char	TICK_VALUE_1	[7];
	char	TICK_SIZE_2		[7];	//	xxxx.xx
	char	TICK_VALUE_2	[7];
}MASTER_KSO;


typedef	struct _MASTER_FU
{
	char	STK_CODE	[MAX_STK_CODE];	
	char	STK_NM		[30];	
	char	ARTC_CODE	[MAX_ARTC_CODE];	//	ǰ���ڵ�
	char	ARTC_NM		[30];	//	ǰ���
	char	NTTN_CODE	[2]; 	//	�����ڵ�
	char	LSTNG_DT	[8];	//	������
	char	MTR_DT		[8];	//	������
	char	FND_DT		[8];	//	�����ε���
	char	TICK_SIZE	[7];	//	xxxx.xx
	char	TICK_VALUE	[7];
} MASTER_FU;


typedef	struct _MASTER_FX
{
	char	STK_CODE			[MAX_STK_CODE];	
	char	STK_NM				[30];	
	char	FCM_CODE			[12];
	char	FCM_NM				[30];
	char	DEAL_UNIT			[15];
	char	PIP_UNIT			[11];	//	XXXXX.XXXXX
	char	HOGA_UNIT			[11];	//	TICK SIZE
	char	PIP_VALUE_TP		[1];	//	1(�̱���),2(������),3(�����̱���),4(����������)
	char	PIP_VALUE_STK_CODE	[32];
} MASTER_FX;

typedef	struct _MASTER_MSG
{
	char	MSGCODE				[4];	
	char	MSG_KO				[200];	
	char	MSG_ENG				[200]; // ERROR MESSAGE ���� ����
	char	MSG_CHINA			[200]; // ERROR MESSAGE �߱��� ����

} MASTER_MSG;


//	API �� �����ִ� MASTER FILE
typedef struct _API_MASTER_KSF
{
	char	STK_GB				[1];	//	������ 'F'
	char	Filler_1			[1];	//	SPACE
	char	YearMon				[4];	//	2010�� 3���̸� "1003 "
	char	Filler_2			[24];	//	SPACE
	char	STK_CODE			[8];
}API_MASTER_KSF;

typedef struct _API_MASTER_KSO
{
	char	CALL_PUT			[1];	//	C:CALL, P:PUT
	char	Filler_1			[1];	//	SPACE
	char	YearMon				[4];	//	2010�� 3���̸� "1003 "
	char	Filler_2			[1];	//	SPACE
	char	STRK_PRC			[6];	//	��簡
	char	Filler_3			[1];	//	space
	char	STK_CODE			[8];
}API_MASTER_KSO;


//////////////////////////////////////////////////////////////////////////
//	KOSPI200 ����/�ɼ� ���簡 ���� ���� (81003)
typedef struct _REAL_SISE_KS
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;

	char	stk_code			[MAX_STK_CODE];		//32
	char	gds_code			[2];	// 2	KSF, KSO ���� 2010/07/26 �߰�	//34
	char	tm					[6];											//40
	char	now_prc				[7];	//	xxxx.xx								//47
	char	open				[7];	//	2									//54
	char	high				[7];	//	2									//61
	char	low					[7];	//	2									//68
	char	acml_cntr_qty		[7];	//	0	����ü�����					//75
	char	acml_buy_cntr_qty	[6];	//	0	�����ż�ü�ᷮ	//	������� ����	//81
	char	buy_sell_tp			[1];												//82
	char	acml_amt			[15];	//	0	�����ŷ����						//97
	char	daebi_sign			[1];	//	0	���ϴ���ȣ						//98
	char	chg					[7];	//	2	���ϴ��							//105
	char	chg_rate			[7];	//	2	�����
	char	cntr_qty			[6];	//	0	ü�����
	char	o_hoga_1			[7];	//	2	
	char	o_hoga_2			[7];	//	2	
	char	b_hoga_3			[7];	//	2	
	char	o_hoga_4			[7];	//	2	
	char	o_hoga_5			[7];	//	2	
	char	o_rmnq_1			[6];	//	0	�ŵ��ܷ�
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	�Ѹŵ�ȣ���ܷ�
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	�Ѹŵ��Ǽ�
	char	o_hoga_cnt_chg_1	[4];	//	0	�ŵ�ȣ���Ǽ����
	char	o_hoga_cnt_chg_2	[4];	//	0	
	char	o_hoga_cnt_chg_3	[4];	//	0	
	char	o_hoga_cnt_chg_4	[4];	//	0	
	char	o_hoga_cnt_chg_5	[4];	//	0	
	char	b_hoga_1			[7];	//	2	
	char	b_hoga_2			[7];	//	2	
	char	o_hoga_3			[7];	//	2	
	char	b_hoga_4			[7];	//	2	
	char	b_hoga_5			[7];	//	2	
	char	b_rmnq_1			[6];	//	0	�ż��ܷ�
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		�Ѹż�ȣ���ܷ�
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	�Ѹż��Ǽ�
	char	b_hoga_cnt_chg_1	[4];	//	0	
	char	b_hoga_cnt_chg_2	[4];	//	0	
	char	b_hoga_cnt_chg_3	[4];	//	0	
	char	b_hoga_cnt_chg_4	[4];	//	0	
	char	b_hoga_cnt_chg_5	[4];	//	0	
	char	ks200				[7];	//	2	
	char	hoga_trmnq_chg		[7];	//	0	ȣ�����ܷ�����
	char	qty_chg				[9];	//		�ŷ�������
	char	amt_chg				[9];	//		�ŷ��������
	char	theory_prc			[7];	//	2	�̷а�
	char	open_yak			[7];	//	0	�̰�����������(��ȣ)-�ɼǸ���ȣ����
	char	open_yak_chg		[6];	//		�̰��������������
	char	gyuri_rate			[7];	//	2	������
	char	basis				[7];	//	2	���庣�̽ý�
	char	open_chg			[7];	//	2	�ð����
	char	high_chg			[7];	//	2	�����
	char	low_chg				[7];	//	2	�������
	char	theory_basis		[7];	//	2	�̷к��̽ý�
	char	imp_v				[7];	//		���纯���� - OPTION ONLY
	char	delta				[7];	//		��Ÿ - OPTION ONLY
	char	gmma				[7];	//		���� - OPTION ONLY
	char	veta				[7];	//		��Ÿ - OPTION ONLY
	char	theta				[7];	//		��Ÿ - OPTION ONLY
	char	rho					[7];	//		�� - OPTION ONLY
	char	in_val				[6];	//		���簡ġ - OPTION ONLY
	char	tm_value			[6];	//		�ð���ġ - OPTION ONLY
	char	cntr_net_buy		[9];	//		ü����ż� - OPTION ONLY
	char	rmnq_gap			[7];	//		�ܷ���(�ż��ܷ�-�ŵ��ܷ�) - OPTION ONLY
	char	cnt_gap				[5];	//		�Ǽ���(�ż��Ǽ�-�ŵ��Ǽ�) - OPTION ONLY
	
	char	ETX					[1];

}REAL_SISE_KS;		// 508


//////////////////////////////////////////////////////////////////////////
//	KOSPI200 �����ɼ� ���簡 (TR:21004, REAL:81004)
typedef struct _REAL_NOW_PRC
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;
	char	STK_CODE	[MAX_STK_CODE];
	char	NOW_PRC		[7];
	char	ETX					[1];

}REAL_NOW_PRC;

//////////////////////////////////////////////////////////////////////////
//	KOSPI200 ����/�ɼ� ȣ�� ���� ���� (81103)
typedef struct _REAL_HOGA_KS
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;
	
	char	stk_code			[MAX_STK_CODE];	//��Ŷũ�� ������ 'MAX_STK_CODE'(32) �ƴ� '8'Byte
	char	gds_code			[2];	// 2	KSF, KSO ���� 2010/07/26 �߰�
	char	tm					[6];
	char	o_hoga_1			[7];	//	2	
	char	o_hoga_2			[7];	//	2	
	char	o_hoga_3			[7];	//	2	
	char	o_hoga_4			[7];	//	2	
	char	o_hoga_5			[7];	//	2	
	char	o_rmnq_1			[6];	//	0	�ŵ��ܷ�
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	�Ѹŵ�ȣ���ܷ�
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	�Ѹŵ��Ǽ�
// 	char	o_hoga_cnt_chg_1	[4];	//	0	�ŵ�ȣ���Ǽ����
// 	char	o_hoga_cnt_chg_2	[4];	//	0	
// 	char	o_hoga_cnt_chg_3	[4];	//	0	
// 	char	o_hoga_cnt_chg_4	[4];	//	0	
// 	char	o_hoga_cnt_chg_5	[4];	//	0	
	char	b_hoga_1			[7];	//	2	
	char	b_hoga_2			[7];	//	2	
	char	b_hoga_3			[7];	//	2	
	char	b_hoga_4			[7];	//	2	
	char	b_hoga_5			[7];	//	2	
	char	b_rmnq_1			[6];	//	0	�ż��ܷ�
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		�Ѹż�ȣ���ܷ�
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	�Ѹż��Ǽ�
// 	char	b_hoga_cnt_chg_1	[4];	//	0	
// 	char	b_hoga_cnt_chg_2	[4];	//	0	
// 	char	b_hoga_cnt_chg_3	[4];	//	0	
// 	char	b_hoga_cnt_chg_4	[4];	//	0	
// 	char	b_hoga_cnt_chg_5	[4];	//	0	
// 	char	hoga_trmnq_chg		[7];	//	0	ȣ�����ܷ�����
	
	char	ETX					[1];
	
}REAL_HOGA_KS;		// 281 (234)


//////////////////////////////////////////////////////////////////////////
//	KOSPI200 ����/�ɼ� ���簡 ���� ���� �ι�°
typedef struct _REAL_SISE_KS_84003
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;
	
	char	stk_code			[MAX_STK_CODE];	
	char	gds_code			[2];	// 2	KSF, KSO ���� 2010/07/26 �߰�
	char	tm					[6];
	char	now_prc				[7];	//	xxxx.xx
	char	open				[7];	//	2	
	char	high				[7];	//	2	
	char	low					[7];	//	2	
	char	acml_cntr_qty		[7];	//	0	����ü�����
	char	acml_buy_cntr_qty	[6];	//	0	�����ż�ü�ᷮ	//2012.08.22. basis ������� ��ü.
	char	buy_sell_tp			[1];
	char	acml_amt			[15];	//	0	�����ŷ����
	char	daebi_sign			[1];	//	0	���ϴ���ȣ
	char	chg					[7];	//	2	���ϴ��
	char	chg_rate			[7];	//	2	�����
	char	cntr_qty			[6];	//	0	ü�����
	char	ks200				[7];	//	2	
	char	qty_chg				[9];	//		�ŷ�������
	char	amt_chg				[9];	//		�ŷ��������
	char	open_yak			[7];	//	0	�̰�����������(��ȣ)-�ɼǸ���ȣ����
	char	open_yak_chg		[6];	//		�̰�������������� 
	char	ETX					[1];
	
}REAL_SISE_KS_84003;		// 220


//////////////////////////////////////////////////////////////////////////
//	�ؿܼ��� ���簡, ȣ�� ���� ����
typedef struct _REAL_SISE_FU
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;
	
	char	stk_code			[MAX_STK_CODE];	
	char	tm					[6];
	char	nttn_code			[2];
	char	now_prc				[MAX_ORD_PRC];	//	20,8
	char	open				[MAX_ORD_PRC];	//	20,8
	char	high				[MAX_ORD_PRC];	//	20,8
	char	low					[MAX_ORD_PRC];	//	20,8
	char	last_close			[MAX_ORD_PRC];	//	20,8
	char	setl_price			[MAX_ORD_PRC];	//	20,8	
	char	acml_cntr_qty		[MAX_ORD_QTY];	//	0	����ü�����
	char	acml_buy_cntr_qty	[MAX_ORD_QTY];	//	0	�����ż�ü�ᷮ
	char	acml_amt			[15];	//	0	�����ŷ����
	char	daebi_sign			[1];	//	0	���ϴ���ȣ
	char	chg					[7];	//	2	���ϴ��
	char	chg_rate			[7];	//	2	�����
	char	cntr_qty			[9];	//	0	ü�����
	char	o_hoga_1			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_2			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_3			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_4			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_5			[MAX_ORD_PRC];	//	20,8
	char	o_rmnq_1			[6];	//	0	�ŵ��ܷ�
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	�Ѹŵ�ȣ���ܷ�
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	�Ѹŵ��Ǽ�
	char	o_hoga_cnt_chg_1	[4];	//	0	�ŵ�ȣ���Ǽ����
	char	o_hoga_cnt_chg_2	[4];	//	0	
	char	o_hoga_cnt_chg_3	[4];	//	0	
	char	o_hoga_cnt_chg_4	[4];	//	0	
	char	o_hoga_cnt_chg_5	[4];	//	0	
	char	b_hoga_1			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_2			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_3			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_4			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_5			[MAX_ORD_PRC];	//	20,8
	char	b_rmnq_1			[6];	//	0	�ż��ܷ�
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		�Ѹż�ȣ���ܷ�
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	�Ѹż��Ǽ�
	char	gl_str				[MAX_STK_CODE];
	char	dspl_now_prc		[MAX_ORD_PRC];
	char	ETX					[1];
}REAL_SISE_FU;


//////////////////////////////////////////////////////////////////////////
//	�ؿܼ��� ���簡, ȣ�� ���� ����
typedef struct _REAL_HOGA_FU
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;
	
	char	stk_code			[MAX_STK_CODE];	
	char	tm					[6];
	char	o_hoga_1			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_2			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_3			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_4			[MAX_ORD_PRC];	//	20,8
	char	o_hoga_5			[MAX_ORD_PRC];	//	20,8
	char	o_rmnq_1			[6];	//	0	�ŵ��ܷ�
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	�Ѹŵ�ȣ���ܷ�
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	�Ѹŵ��Ǽ�
	char	b_hoga_1			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_2			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_3			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_4			[MAX_ORD_PRC];	//	20,8
	char	b_hoga_5			[MAX_ORD_PRC];	//	20,8
	char	b_rmnq_1			[6];	//	0	�ż��ܷ�
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		�Ѹż�ȣ���ܷ�
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	�Ѹż��Ǽ�
	
	char	ETX					[1];
	
}REAL_HOGA_FU;


//////////////////////////////////////////////////////////////////////////
//	FX�� ���簡, ȣ�� ���� ����
typedef struct _REAL_SISE_FX
{
	char	STX					[1];
	C_HEADER				Header;
	TR_HEADER				TRHeader;

	char	stk_code			[MAX_STK_CODE];	
	char	tm					[6];
	char	nttn_code			[2];
	char	o_now_prc			[11];	//	20,8
	char	b_now_prc			[11];	//	20,8
	char	spread_sign			[1];
	char	spread				[11];
	char	o_open				[11];
	char	b_open				[11];
	char	open				[11];
	char	o_high				[11];
	char	b_high				[11];
	char	high				[11];
	char	o_low				[11];
	char	b_low				[11];
	char	low					[11];
	char	o_intr				[11];	//	11,2
	char	b_intr				[11];	//	11,2
	char	pip_value			[11];	//	11,2
	
	char	ETX					[1];
	
}REAL_SISE_FX;

/***********************************************************************

	COMPONENT(������Ʈ) �� ��Ŷ ���� (���ļ��� : TR��ȣ��)

***********************************************************************/

///////////////////////////////////////////////////////////////////////////
//	������ȸ ȭ��: X1TR00015 :  ȭ���ȣ (????)

typedef struct _ST_TR_00015_I
{
	char	UserID		[MAX_USERID];	//	�����ID
}ST_TR_00015_I;

typedef struct _ST_TR_00015_O
{
	char	UserID		[MAX_USERID];	//	�����ID
	char	Acnt_No_Ks	[MAX_ACNT_NO];	//	���¹�ȣ (KS) ��: 00-00-00000 (11�ڸ�)
	char	Acnt_Nm_Ks	[20];			//	���¸� (KS)
	char	Acnt_Pwd_Ks	[4];			//	���º�к�ȣ (KS)
	char	Acnt_No_Fu	[MAX_ACNT_NO];	//	���¹�ȣ (FU)
	char	Acnt_Nm_Fu	[20];			//	���¸� (FU)
	char	Acnt_Pwd_Fu	[4];			//	���º�к�ȣ (FU)
	char	Acnt_No_Fx	[MAX_ACNT_NO];	//	���¹�ȣ (FX)
	char	Acnt_Nm_Fx	[20];			//	���¸� (FX)
	char	Acnt_Pwd_Fx	[4];			//	���º�к�ȣ (FX)
}ST_TR_00015_O;

///////////////////////////////////////////////////////////////////////////
//	������(�ܰ�) ���� ��ȸ ȭ��: X1TR20001 :  ȭ���ȣ (2101)

typedef struct _ST_TR_20001_I
{
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ
	char	AcntPwd		[ 4];			//	���� ��й�ȣ
	char	StkCode		[MAX_STK_CODE];	//	�����ڵ�
}ST_TR_20001_I;

typedef struct _ST_TR_20001_O
{
	char	StkCode		[MAX_STK_CODE];	//	�����ڵ�
	char	GdsCode		[ 2];			//	��ǰ�ڵ� (00:KS����, 01:KS�ɼ�, 02:�ؿܼ���, 03:FX����)
	char	BuySell_Tp	[ 1];			//	�ֹ����� (1.�ŵ� , 2.�ż�)
	char	Qty			[10];			//	ü�����
	char	AvgPrc		[MAX_ORD_PRC];	//	��մܰ�
	char	EvalPnlAmt	[15];			//	�򰡼���
	char	AvgRate		[10];			//	���ͷ�
	char	CLTL		[10];			//	�㺸��
}ST_TR_20001_O;


//////////////////////////////////////////////////////////////////////////
//	��ü���ֹ���ȸ ȭ��: X1TR20005  , ȭ���ȣ (1102)

typedef struct _ST_TR_20005_I 
{
	char		ACNT_NO		[MAX_ACNT_NO];	//	���¹�ȣ
	char		ACNT_PWD	[4];			//	���� ��й�ȣ
	char		GDS_CODE	[2];
	char		STK_CODE	[MAX_STK_CODE];
}ST_TR_20005_I;

typedef struct _ST_TR_20005_O
{
	char	ACNT_NO		[MAX_ACNT_NO];
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
	char	ORD_NO		[MAX_ORD_NO];
	char	ORG_NO		[MAX_ORD_NO];
	char	BUY_SELL_TP	[1];
	char	ORD_PRC		[MAX_ORD_PRC];
	char	ORD_QTY		[MAX_ORD_QTY];
	char	REMN_QTY	[MAX_ORD_QTY];
	char	ORD_ST		[1];
	char	MNG_USERID	[MAX_USERID];
	
}ST_TR_20005_O;


//////////////////////////////////////////////////////////////////////////
//	���ں� �����
typedef struct _ST_TR_20011_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[MAX_ACNT_NO];
	char	ACNT_PWD	[4];
	char	DT_FROM		[8];
	char	DT_TO		[8];
	char	NEXT_KEY	[10];
}ST_TR_20011_I;

typedef struct _ST_TR_20011_O
{
	char	ACNT_NO		[MAX_ACNT_NO];
	char	USERID		[MAX_USERID];
	char	USER_NM		[20];
	char	REQ_DT		[8];
	char	REQ_TM		[8];
	char	REQ_CODE	[4];		//	�Ա�, ���
	char	REQ_MONEY	[10];
	char	RESULT		[4];
	char	RSLT_DT		[8];
	char	RSLT_TM		[8];
	char	NEXT_KEY	[10];
}ST_TR_20011_O;


///////////////////////////////////////////////////////////////////////////
//	�㺸�� �Ա� ȭ��: X1TR20012 :  ȭ���ȣ (3001) , �Ա�

typedef struct _ST_TR_20012_I
{
	char	Proc_Tp		[1];			//	Q : ��ȸ, U : ��û
	char	UserID		[MAX_USERID];	//	�����ID
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ
	char	AcntPwd		[ 4];			//	���� ��й�ȣ
	char	Req_Money	[15];			//	��û�ݾ� ( Proc_TP �� Q �϶��� ���� ��� �� )
	char	GujaCnt		[2];			//	��û ���¼�
	char	DayCnt		[3];			//	��û ����� �� 
	char	Dpst_Nm		[20];			//	�Ա��ڸ� ( �Ա� ��û�� �Է�)
}ST_TR_20012_I;

typedef struct _ST_TR_20012_O
{
	char	Cpny_Bank_Nm		[20];	//	�Ա��� ȸ�� ��������
	char	Cpny_Acnt_No		[20];	//	�Ա��� ȸ�� ���¹�ȣ
	char	Cpny_Acnt_Nm		[20];	//	�Ա��� ȸ�� ���¹���(�����)��
	char	NOW_CLTL			[11];
	char	Success_Yn			[ 1];	//	�Աݼ��� ���� (0:���� , 1:����)
}ST_TR_20012_O;



///////////////////////////////////////////////////////////////////////////
//	�㺸�� �Ա� ȭ��: X1TR30012 :  ȭ���ȣ (3003) , �ؿܼ��� �Ա�

typedef struct _ST_TR_30012_I
{
	char	Proc_Tp		[1];			//	Q : ��ȸ, U : ��û
	char	UserID		[MAX_USERID];	//	�����ID
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ
	char	AcntPwd		[ 4];			//	���� ��й�ȣ
	char	Req_Money	[15];			//	��û�ݾ� ( Proc_TP �� Q �϶��� ���� ��� �� )
	char	Req_Money_KRW	[15];			//	��û�ݾ� ( Proc_TP �� Q �϶��� ���� ��� �� )
	char	Forex		[7];			//	ȯ��
	char	Dpst_Nm		[20];			//	�Ա��ڸ� ( �Ա� ��û�� �Է�)
}ST_TR_30012_I;

typedef struct _ST_TR_30012_O
{
	char	Cpny_Bank_Nm		[20];	//	�Ա��� ȸ�� ��������
	char	Cpny_Acnt_No		[20];	//	�Ա��� ȸ�� ���¹�ȣ
	char	Cpny_Acnt_Nm		[20];	//	�Ա��� ȸ�� ���¹���(�����)��
	char	NOW_CLTL			[11];
	char	Forex_Buy			[7];	//	��ȭ����ȯ��(��ȭ�Ա�)
	char	Forex_Sell			[7];	//	��ȭ�ŵ�ȯ��(��ȭ���)
	char	Success_Yn			[ 1];	//	�Աݼ��� ���� (0:���� , 1:����)

}ST_TR_30012_O;



///////////////////////////////////////////////////////////////////////////
//	�㺸�� ��� ȭ��: X1TR20013 :  ȭ���ȣ (3001) , ���

typedef struct _ST_TR_20013_I
{
	char	Proc_Tp				[1];			//	Q : ��ȸ, U : ��û
	char	UserID				[MAX_USERID];	//	�����ID
	char	AcntNo				[MAX_ACNT_NO];	//	���¹�ȣ
	char	AcntPwd				[ 4];			//	���� ��й�ȣ
	char	Req_Money			[15];			//	��û�ݾ�. ��û�� �Է�
}ST_TR_20013_I;

typedef struct _ST_TR_20013_O
{
	char	User_Bank_Nm		[20];	//	���� ���� ���� ��
	char	User_Bank_Acnt_No	[20];	//	���� ���� ���� ����
	char	User_Bank_User_Nm	[20];	//	�� ���� ������
	char	User_CanOut_Entr	[15];	//	��ݰ��ɱݾ�	
	char	Success_Yn			[ 1];	//	��ݽ�û ���� ���� (0:���� , 1:����)
}ST_TR_20013_O;


// 1-016-007-005-003. client logging
typedef struct _ST_TR_88888_C
{
	char			Code				[2];	// 00 : client logging
	//char			SeqNo				[9];
	char			UserID				[MAX_USERID];
	char			UserNm				[20];
	char			DT					[8];
	char			TM					[12];
	char			Data				[128];
	char			Reserved			[50];
	
}ST_TR_88888_C;

typedef struct _ST_TR_88888_S 
{
	char			Code		[2];
	char			Result		[1];
	char			Reserved	[50];
}ST_TR_88888_S;

///////////////////////////////////////////////////////////////////////////
//	�㺸�� ��� ȭ��: X1TR20013 :  ȭ���ȣ (3001) , ���

typedef struct _ST_TR_30013_I
{
	char	Proc_Tp				[1];			//	Q : ��ȸ, U : ��û
	char	UserID				[MAX_USERID];	//	�����ID
	char	AcntNo				[MAX_ACNT_NO];	//	���¹�ȣ
	char	AcntPwd				[ 4];			//	���� ��й�ȣ
	char	Req_Money			[15];			//	��û�ݾ�. ��û�� �Է�
	char	Req_Money_KRW		[15];			//	��û�ݾ�. ��û�� �Է�
	char	Forex				[7];			//	ȯ��
}ST_TR_30013_I;

typedef struct _ST_TR_30013_O
{
	char	User_Bank_Nm		[20];	//	���� ���� ���� ��
	char	User_Bank_Acnt_No	[20];	//	���� ���� ���� ����
	char	User_Bank_User_Nm	[20];	//	�� ���� ������
	char	User_CanOut_Entr	[15];	//	��ݰ��ɱݾ�	
	char	Forex_Buy			[7];	//	��ȭ����ȯ��(��ȭ�Ա�)
	char	Forex_Sell			[7];	//	��ȭ�ŵ�ȯ��(��ȭ���)
	char	Success_Yn			[ 1];	//	��ݽ�û ���� ���� (0:���� , 1:����)

}ST_TR_30013_O;


///////////////////////////////////////////////////////////////////////////
//	���¼� ��ȸ �� ���� ȭ��: X1TR20014 :  ȭ���ȣ (3001) , ���

typedef struct _ST_TR_20014_I
{
	char	Proc_Tp				[1];			//	Q : ��ȸ, U : ��û
	char	GDS_CODE			[2];
	char	AcntNo				[MAX_ACNT_NO];	//	���¹�ȣ
	char	AcntPwd				[ 4];			//	���� ��й�ȣ
	char	GUJA_CNT			[4];			//	��û���¼�. ��û�� �Է�
	char	SITE				[5];			//	X1 OR BLASH
}ST_TR_20014_I;

typedef struct _ST_TR_20014_O
{
	char	GUJA_CNT			[ 4];	//	���� ���¼�
	char	CLTL				[ 11];	//	���� �㺸��
	char	MAX_GUJA_CNT		[4];	//	���� ���¼�
	char	CAN_GUJA_CNT		[4];	//	�ܾ״�� ���� ���¼�
}ST_TR_20014_O;

///////////////////////////////////////////////////////////////////////////
//	ü�᳻�� ��ȸ ȭ��: X1TR20017 :  ȭ���ȣ (1201)

typedef struct _ST_TR_20017_I
{
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ
	char	AcntPwd		[ 4];			//	���� ��й�ȣ
	char	StkCode		[32];			//	�����ڵ�
	char	StartDt		[ 8];			//	��ȸ ������
	char	EndDt		[ 8];			//	��ȸ ����
	char	NextKey		[17];			//	���� ��ȸŰ
}ST_TR_20017_I;

typedef struct _ST_TR_20017_O
{
	char	CntrNo		[MAX_ORD_NO];	//	ü���ȣ
	char	CntrDt		[ 8];			//	ü������
	char	StkCode		[MAX_STK_CODE];	//	�����ڵ�
	char	BuySell_Tp	[ 1];			//	�ֹ����� (1.�ŵ�, 2.�ż�)
	char	CntrQty		[10];			//	ü�����
	char	OrdPrice	[MAX_ORD_PRC];	//	�ֹ�����
	char	CntrPrice	[MAX_ORD_PRC];	//	ü�ᰡ��
	char	CntrTime	[ 8];			//	ü��ð� (hh:mm:ss)
	char	ClrPl		[15];			//	����
	char	OrdSt		[ 1];			//	�ֹ����� (1. ���尡 , 2. ������)
	char	OrdNO		[MAX_ORD_NO];	//	ü��� �ֹ��� �ֹ���ȣ
	char	CMSN		[5];			//	������
}ST_TR_20017_O;


///////////////////////////////////////////////////////////////////////////
//	��Ź�� ��ȸ: X1TR20018 :  ȭ���ȣ (2001)  , X1TR20019 : ȭ���ȣ (2002)

// ����(KOSPI200) ��Ź�� ��ȸ I/O
typedef struct _ST_TR_20018_I
{
	char	AcntNo			[MAX_ACNT_NO];	//	���¹�ȣ
	char	AcntPwd			[ 4];			//	���� ��й�ȣ
}ST_TR_20018_I;

typedef struct _ST_TR_20018_O
{
	char	OpenTotAmt		[15];	//	����� ��Ź�Ѿ�
	char	RlzPnlAmt		[15];	//	��������
	char	CurrentTotAmt	[15];	//	���� ��Ź�Ѿ�
	char	EvalPnlAmt		[15];	//	�򰡼���
	char	EvalTotAmt		[15];	//	�� ��Ź�Ѿ�
	char	LossCutAmt		[15];	//	�ν��� �����ݾ�
	char	AcntStatCode	[ 8];	//	���»���
	char	OrdAbleAmt		[15];	//	�ֹ����ɱݾ�
	char	OpenPldgAmt		[15];	//	����� �㺸�ݾ�
	char	Cmsn			[15];	//	������
	char	Utlfree			[15];	//	�̿��
	char	CurrentPldgAmt	[15];	//	���� �㺸�ݾ�
	char	EvalPldgAmt		[15];	//	�� �㺸�ݾ�
	char	LossCutFreeAmt	[15];	//	�ν��� �����ݾ�
	char	FcurrMgn		[15];	//	��Ź���ű�
	char	OutAbleAmt		[15];	//	��ݰ��ɱݾ�
}ST_TR_20018_O;

// �ؿ�(FUFX) ��Ź�� ��ȸ I/O
typedef struct _ST_TR_20019_I
{
	char	AcntNo			[MAX_ACNT_NO];	//	���¹�ȣ
	char	AcntPwd			[ 4];			//	���� ��й�ȣ
}ST_TR_20019_I;

typedef struct _ST_TR_20019_O
{
	char	OpenTotAmt_USD		[15];	//	����� ��Ź�Ѿ� (USD)
	char	RlzPnlAmt_USD		[15];	//	�������� (USD)
	char	CurrentTotAmt_USD	[15];	//	���� ��Ź�Ѿ� (USD)
//	char	RlzPnlAmt_KRW		[15];	//	�������� (KRW)
	char	Xchrat				[ 8];	//	ȯ��
	char	EvalPnlAmt_USD		[15];	//	�򰡼��� (USD)
	char	EvalTotAmt_USD		[15];	//	�� ��Ź�Ѿ� (USD)
	char	LossCutAmt			[15];	//	�ν��� �����ݾ�
	char	AcntStatCode		[ 2];	//	���»���
	char	OrdAbleAmt_USD		[15];	//	�ֹ����ɱݾ� (USD)
	char	OpenPldgAmt_USD		[15];	//	����� �㺸�ݾ� (USD)
//	char	OpenPldgAmt_KRW		[15];	//	����� �㺸�ݾ� (KRW)
	char	Cmsn_USD			[15];	//	������ (USD)
	char	Utlfree				[15];	//	�̿��
//	char	Cmsn_KRW			[15];	//	������ (KRW)
	char	CurrentPldgAmt		[15];	//	���� �㺸�ݾ�
//	char	EvalPldgAmt_KRW		[15];	//	�� ���� (KRW)
	char	EvalPldgAmt			[15];	//	�� �㺸�ݾ�
	char	LossCutFreeAmt		[15];	//	�ν��� �����ݾ�
	char	FcurrMgn_USD		[15];	//	��Ź���ű� (USD)
//	char	OutAbleAmt_KRW		[15];	//	��ݰ��ɱݾ� (KRW)
}ST_TR_20019_O;


///////////////////////////////////////////////////////////////////////////
//	���� ���ͺм� ��ȸ ȭ��: X1TR20020 :  ȭ���ȣ (2201)

typedef struct _ST_TR_20020_I
{
	char	GDS_CODE	[2];
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ
	char	AcntPwd		[ 4];			//	���� ��й�ȣ
	char	StartDt		[ 8];			//	��ȸ ������
	char	EndDt		[ 8];			//	��ȸ ����
	char	NEXT_KEY	[85];
}ST_TR_20020_I;

// typedef struct _ST_TR_20020_O
// {
// 	char	OpenTotAmt		[15];	//	��Ź�Ѿ�
// 	char	RlzPnlSumAmt	[15];	//	�������� ����
// 	char	FutsRlzPnlAmt	[15];	//	���� ��������
// 	char	OptRlzPnlAmt	[15];	//	�ɼ� ��������
// 	char	EvalPlnSum		[15];	//	�򰡼��� �հ�
// 	char	FutsEvalPnlAmt	[15];	//	���� �򰡼���
// 	char	OptEvalPnlAmt	[15];	//	�ɼ� �򰡼���
// 	char	TotSumErn		[15];	//	�Ѽ��� �հ�
// 	char	FutsCtrctAmt	[15];	//	���� �����ݾ� ����
// 	char	OptCtrctAmt		[15];	//	�ɼ� �����ݾ� ����
// 	char	CmsnSumAmt		[10];	//	������ ����
// 	char	FutsCmsn		[10];	//	���� ������
// 	char	OptCmsn			[10];	//	�ɼ� ������
// 	char	NetPnlSumAmt	[15];	//	�� �����հ�
// }ST_TR_20020_O;
typedef struct _ST_TR_20020_O
{	
	char	DT		[8];
	char	CLTL	[15];
	char	IN_AMT	[15];	//	�Աݾ�
	char	OUT_AMT	[15];	//	��ݾ�
	char	CLR_PL	[15];
	char	CMSN	[15];
	char	ACML_PL	[15];	//	��������
	char	SEQ_NO	[8];
}ST_TR_20020_O;



//////////////////////////////////////////////////////////////////////////
//	���ͷ� ��ȸ
typedef struct _ST_TR_20030_I
{

	char	GDS_CODE	[2];
	char	ACNT_NO		[11];
	char	DT_FROM		[8];
	char	DT_TO		[8];
}ST_TR_20030_I;


#ifdef _WISEMULTI
typedef struct _ST_TR_20030_O
{
	char	DT			[8];
	char	IN_AMT		[15];
	char	OUT_AMT		[15];
	char	CLR_PL_KS	[15];	// 	��������
	char	CMSN_KS		[15];	//	����������
	char	CLR_PL_EUR	[15];	//	���μ���
	char	CMSN_EUR	[15];	//	�ؿܼ�����
	char	CLR_PL		[15];	// 	��������
	char	ACCM_PL		[15];	//	��������
	char	CMSN_NQ		[15];	//	NQ������
	char	CLR_PL_NQ	[15];	//	NQ����
	char	PL_RATE		[7];	//	XXX.XXX %
	//char	ACCM_RATE	[7];	//	XXX.XXX %
	char	ENTR_RT		[3];
	char	CNTR_QTY	[5];
	char	ESTM_CLTL	[15];	//	���ϴ㺸�� + ��������
	
}ST_TR_20030_O;
#else
typedef struct _ST_TR_20030_O
{
	char	DT			[8];
	char	IN_AMT		[15];
	char	OUT_AMT		[15];
	char	CLR_PL_F	[15];	// 	��������
	char	CMSN_F		[15];	//	����������
	char	CLR_PL_O	[15];	//	���μ���
	char	CMSN_O		[15];	//	�ؿܼ�����
	char	CLR_PL		[15];	// 	��������
	char	ACCM_PL		[15];	//	�����ڼ���
	char	START_CLTL	[15];	//	������ �㺸��
	char	ESTM_CLTL	[15];	//	���ϴ㺸�� + ��������
	char	PL_RATE		[7];	//	XXX.XXX %
	//char	ACCM_RATE	[7];	//	XXX.XXX %
	char	ENTR_RT		[3];
	char	CNTR_QTY	[5];
	
}ST_TR_20030_O;
#endif

//////////////////////////////////////////////////////////////////////////
//	��õ�� ������ ��ȸ
typedef struct _ST_TR_20040_I
{	
	char	GDS_CODE	[2];
	char	ACNT_NO		[11];
	char	DT_FROM		[8];
	char	DT_TO		[8];
	char	Gb			[1];
	char	SEL_IDX		[1];  	//  0:��ü, 1:�̸�, 2:�ʸ�
    char	SEL_VAL		[10];
}ST_TR_20040_I;

typedef struct _ST_TR_20040_O
{
	char	Gb		[1];
	char	DT		[8];
	char	UserNM	[20];
	char	NickNM	[20];
	char	CMSN_F	[15];
	char	CMSN_O	[15];
	char	CMSN_SUM[15];
	char	CMSN_Back[15];
	char	RSLT	[1]; // Y:SUCCESS, N:FAIL
    char	MSG     [50]; // ������ �޼���
	
}ST_TR_20040_O;

//////////////////////////////////////////////////////////////////////////
//	��õ�� ������ ��ȸ 2
typedef struct _ST_TR_20041_I
{
	char	ACNT_NO		[11];
	char	DT_FROM		[8];
	char	DT_TO		[8];
	char	Gb			[1];
	char	SEL_IDX		[1];  	//  0:��ü, 1:�̸�, 2:�ʸ�
    char	SEL_VAL		[10];
}ST_TR_20041_I;

typedef struct _ST_TR_20041_O
{
	char	Gb			[1];
	char	DT			[8];
	char	UserNM		[20];
	char	NickNM		[20];
	char	TrdeDTCnt	[5];
	char	CondYN		[1];
	char	RSLT		[1]; // Y:SUCCESS, N:FAIL
    //char	MSG			[50]; // ������ �޼���
	
}ST_TR_20041_O;


//////////////////////////////////////////////////////////////////////////
//	HI-PRO ��õ�� ������ ��ȸ
typedef struct _ST_TR_20042_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[11];	//	��õ�� ���¹�ȣ
	char	DT_FROM		[8];	//	��ȸ��
	char	DT_TO		[8];	
	char	Gb			[1];
	char	SEL_IDX		[1];  	
    char	SEL_VAL		[10];
}ST_TR_20042_I;

typedef struct _ST_TR_20042_O
{
	char	Gb		[1];
	char	DT		[8];
	char	UserNM	[20];
	char	NickNM	[20];
	char	CMSN_F1	[15];
	char	CMSN_O1	[15];
	char	CMSN_F2	[15];
	char	CMSN_O2	[15];
	char	CMSN_SUM[15];
	char	CMSN_Back[15];
	char	RSLT	[1]; // Y:SUCCESS, N:FAIL
    char	MSG     [50]; // ������ �޼���
	
}ST_TR_20042_O;

///////////////////////////////////////////////////////////////////////////
//	KOSPI 200 ����/�ɼ� ���簡 ��ȸ: X1TR21003 :  ȭ���ȣ (Real Packet TR)

typedef struct _ST_TR_21003_I			// ���� KS
{
	char	GdsCode		[ 2];			//	��ǰ�ڵ� (00:KS����, 01:KS�ɼ�, 02:�ؿܼ���, 03:FX����)
	char	StkCode		[12];			//	�����ڵ� (��Ŷ�� ũ�� ������ MAX_STK_CODE = 32 ������� �ʴ´�.)
}ST_TR_21003_I;

typedef struct _ST_TR_21003_O
{
	char	StkCode				[12];	//	�����ڵ�
	char	GdsCode				[2];	// 	��ǰ�ڵ� (00:KS����, 01:KS�ɼ�, 02:�ؿܼ���, 03:FX����)
	char	tm					[6];
	char	now_prc				[7];	//	xxxx.xx
	char	open				[7];	//	2	
	char	high				[7];	//	2	
	char	low					[7];	//	2	
	char	acml_cntr_qty		[7];	//	0	����ü�����
	char	acml_buy_cntr_qty	[7];	//	0	�����ż�ü�ᷮ
	char	acml_amt			[15];	//	0	�����ŷ����
	char	daebi_sign			[1];	//	0	���ϴ���ȣ
	char	chg					[7];	//	2	���ϴ��
	char	chg_rate			[7];	//	2	�����
	char	cntr_qty			[6];	//	0	ü�����
	char	o_hoga_1			[7];	//	2	
	char	o_hoga_2			[7];	//	2	
	char	b_hoga_3			[7];	//	2	
	char	o_hoga_4			[7];	//	2	
	char	o_hoga_5			[7];	//	2	
	char	o_rmnq_1			[6];	//	0	�ŵ��ܷ�
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	�Ѹŵ�ȣ���ܷ�
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	�Ѹŵ��Ǽ�
	char	o_hoga_cnt_chg_1	[4];	//	0	�ŵ�ȣ���Ǽ����
	char	o_hoga_cnt_chg_2	[4];	//	0	
	char	o_hoga_cnt_chg_3	[4];	//	0	
	char	o_hoga_cnt_chg_4	[4];	//	0	
	char	o_hoga_cnt_chg_5	[4];	//	0	
	char	b_hoga_1			[7];	//	2	
	char	b_hoga_2			[7];	//	2	
	char	o_hoga_3			[7];	//	2	
	char	b_hoga_4			[7];	//	2	
	char	b_hoga_5			[7];	//	2	
	char	b_rmnq_1			[6];	//	0	�ż��ܷ�
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		�Ѹż�ȣ���ܷ�
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	�Ѹż��Ǽ�
	char	b_hoga_cnt_chg_1	[4];	//	0	
	char	b_hoga_cnt_chg_2	[4];	//	0	
	char	b_hoga_cnt_chg_3	[4];	//	0	
	char	b_hoga_cnt_chg_4	[4];	//	0	
	char	b_hoga_cnt_chg_5	[4];	//	0	
	char	ks200				[7];	//	2	
	char	hoga_trmnq_chg		[7];	//	0	ȣ�����ܷ�����
	char	qty_chg				[9];	//		�ŷ�������
	char	amt_chg				[9];	//		�ŷ��������
	char	theory_prc			[7];	//	2	�̷а�
	char	open_yak			[7];	//	0	�̰�����������(��ȣ)-�ɼǸ���ȣ����
	char	open_yak_chg		[6];	//		�̰��������������
	char	gyuri_rate			[7];	//	2	������
	char	basis				[7];	//	2	���庣�̽ý�
	char	open_chg			[7];	//	2	�ð����
	char	high_chg			[7];	//	2	�����
	char	low_chg				[7];	//	2	�������
	char	theory_basis		[7];	//	2	�̷к��̽ý�
	char	imp_v				[7];	//		���纯���� - OPTION ONLY
	char	delta				[7];	//		��Ÿ - OPTION ONLY
	char	gmma				[7];	//		���� - OPTION ONLY
	char	veta				[7];	//		��Ÿ - OPTION ONLY
	char	theta				[7];	//		��Ÿ - OPTION ONLY
	char	rho					[7];	//		�� - OPTION ONLY
	char	in_val				[6];	//		���簡ġ - OPTION ONLY
	char	tm_value			[6];	//		�ð���ġ - OPTION ONLY
	char	cntr_net_buy		[9];	//		ü����ż� - OPTION ONLY
	char	rmnq_gap			[7];	//		�ܷ���(�ż��ܷ�-�ŵ��ܷ�) - OPTION ONLY
	char	cnt_gap				[5];	//		�Ǽ���(�ż��Ǽ�-�ŵ��Ǽ�) - OPTION ONLY
}ST_TR_21003_O;


///////////////////////////////////////////////////////////////////////////
//	KOSPI 200 ����/�ɼ� ȣ�� ��ȸ: X1TR21103 :  ȭ���ȣ (Real Packet TR)

typedef struct _ST_TR_21103_I
{
	char	GdsCode		[ 2];			//	��ǰ�ڵ� (00:KS����, 01:KS�ɼ�, 02:�ؿܼ���, 03:FX����)
	char	StkCode		[12];			//	�����ڵ�
}ST_TR_21103_I;

typedef struct _ST_TR_21103_O
{
	char	StkCode				[32];	//	�����ڵ�
	char	GdsCode				[2];	// 	��ǰ�ڵ� (00:KS����, 01:KS�ɼ�, 02:�ؿܼ���, 03:FX����)
	char	tm					[6];
	char	o_hoga_1			[7];	//	2	
	char	o_hoga_2			[7];	//	2	
	char	o_hoga_3			[7];	//	2	
	char	o_hoga_4			[7];	//	2	
	char	o_hoga_5			[7];	//	2	
	char	o_rmnq_1			[6];	//	0	�ŵ��ܷ�
	char	o_rmnq_2			[6];	//	0	
	char	o_rmnq_3			[6];	//	0	
	char	o_rmnq_4			[6];	//	0	
	char	o_rmnq_5			[6];	//	0
	char	o_trmnq				[6];	//	�Ѹŵ�ȣ���ܷ�
	char	o_cnt_1				[4];	//	
	char	o_cnt_2				[4];	//	
	char	o_cnt_3				[4];	//	
	char	o_cnt_4				[4];	//	
	char	o_cnt_5				[4];	//	
	char	o_tot_cnt			[5];	//	0	�Ѹŵ��Ǽ�
	char	b_hoga_1			[7];	//	2	
	char	b_hoga_2			[7];	//	2	
	char	b_hoga_3			[7];	//	2	
	char	b_hoga_4			[7];	//	2	
	char	b_hoga_5			[7];	//	2	
	char	b_rmnq_1			[6];	//	0	�ż��ܷ�
	char	b_rmnq_2			[6];	//	0	
	char	b_rmnq_3			[6];	//	0	
	char	b_rmnq_4			[6];	//	0	
	char	b_rmnq_5			[6];	//	0	
	char	b_trmnq				[6];	//		�Ѹż�ȣ���ܷ�
	char	b_cnt_1				[4];	//	
	char	b_cnt_2				[4];	//	
	char	b_cnt_3				[4];	//	
	char	b_cnt_4				[4];	//	
	char	b_cnt_5				[4];	//	
	char	b_tot_cnt			[5];	//	0	�Ѹż��Ǽ�
}ST_TR_21103_O;


///////////////////////////////////////////////////////////////////////////
//	 ��Ʈ��ȸ ȭ��: X1TR35001 :  ȭ���ȣ (5001) (1��,2��,3��,5��,10��,30��,60��,��,��,�� DATA)

typedef struct _ST_TR_35001_I
{
	char	DataGb		[ 4];			//	��Ʈ������ ���� (1��,2��,3��,5��,10��,30��,60��,��,��,��)
	char	GdsCode		[ 2];			//	��ǰ����
	char	StkCode		[MAX_STK_CODE];	//	�����ڵ�
	char	StartDt		[ 8];			//	��ȸ ������
	char	EndDt		[ 8];			//	��ȸ ����
}ST_TR_35001_I;

typedef struct _ST_TR_35001_O
{
	char	Cnt			[ 4];			//	������ ī��Ʈ
	char	DT			[ 8];			//	����
	char	TM			[ 6];			//	�ð�
	char	Open		[15];			//	�ð�
	char	High		[15];			//	��
	char	Low			[15];			//	����
	char	Close		[15];			//	����
}ST_TR_35001_O;


///////////////////////////////////////////////////////////////////////////
//	 TICK ��Ʈ��ȸ ȭ��: X1TR35003 :  ȭ���ȣ (????)

typedef struct _ST_TR_35003_I
{
	char	SeqNo		[ 4];			//	Sequence ��ȣ
	char	GdsCode		[ 2];			//	��ǰ����
	char	StkCode		[MAX_STK_CODE];	//	�����ڵ�
}ST_TR_35003_I;

typedef struct _ST_TR_35003_O
{
	char	Cnt			[ 4];			//	������ ī��Ʈ
	char	TM			[ 6];			//	�ð�
	char	Close		[15];			//	����
}ST_TR_35003_O;


///////////////////////////////////////////////////////////////////////////
//	�������� ȭ��: X1TR41001 , X1TR41002 :  ȭ���ȣ (6201)

// X1TR41001 : ���񳻿� I/O
typedef struct _ST_TR_41001_I
{
	char		StanDate[8];	//	�������� (YYYYMMDD)
	char		UrgentTp[1];	//	��ޱ��� (0.��ü��ȸ, 1.����, 2.��Ȳ, 3.���)
	char		Seq		[20];	//	��������ȣ (0. ��ü, �׿� Number �� ��ȸ)
	char		BtnGb	[1];	//	������ư ���� (0. ���� , 1. ����)
}ST_TR_41001_I;


typedef struct _ST_TR_41001_O
{
	char		StanDate[8];	//	�������� (YYYYMMDD)
	char		StanTime[8];	//	�����ð� (hh:mm:ss)
	char		Seq		[20];	//	��û��������ȣ
	char		Title	[80];	//	��������
}ST_TR_41001_O;

// X1TR41002 : �������� I/O 
typedef struct _ST_TR_41002_I
{
	char		Seq		[20];	//	��û��������ȣ
	char		StanDate[8];	//	�������� (YYYYMMDD)
}ST_TR_41002_I;


typedef struct _ST_TR_41002_O
{
	//char		Body	[800]; //	��������
	char		Body	[1500]; //	��������
}ST_TR_41002_O;


//////////////////////////////////////////////////////////////////////////
//	������ ��ȸ ȭ��: X1TR51001 : ��ȸ / ����  , ȭ���ȣ (5001)

typedef struct _ST_TR_51001_I 
{
	char		TrxTp		[ 1];			//	������ ��ȸ(Q) / ����(U) �����ڵ�
	char		UserID		[MAX_USERID];	//	��ID
	char		UserNm		[20];			//	����
//	char		UserPNm		[20];			//	���ʸ�
	char		Email		[50];			//	E-mail �ּ�
	char		HpNo		[20];			//	�ڵ��� ��ȣ
//	char		TelNo		[20];			//	��ȭ��ȣ
	char		BankNM		[20];
	char		BankAcntNo	[20];
	char		BankUserNM	[20];
}ST_TR_51001_I;

typedef struct _ST_TR_51001_O
{
	char		UserID		[MAX_USERID];	//	��ID
	char		UserNm		[20];			//	����
//	char		UserPNm		[20];			//	���ʸ�
	char		Email		[50];			//	E-mail �ּ�
	char		HpNo		[20];			//	�ڵ��� ��ȣ
//	char		TelNo		[20];			//	��ȭ��ȣ
	char		BankNM		[20];
	char		BankAcntNo	[20];
	char		BankUserNM	[20];
	char		UserNO		[5];			//	����ȣ
}ST_TR_51001_O;

//////////////////////////////////////////////////////////////////////////
//	�������߱� �̺�Ʈ (SMILOR. INDEXEVENT)
typedef struct _ST_TR_55001_I
{
	char	TR			[5];
	char	ACNT_NO		[11];
}ST_TR_55001_I;

typedef struct _ST_TR_55001_O
{
	char	TR			[5];
	char	OPEN		[10];
	char	HIGH		[10];
	char	LOW			[10];
	char	CLOSE		[10];
	char	OPEN_MY		[10];
	char	HIGH_MY		[10];
	char	LOW_MY		[10];
	char	CLOSE_MY	[10];
	char	OPEN_YN		[1];
	char	HIGH_YN		[1];
	char	LOW_YN		[1];
	char	CLOSE_YN	[1];
	char	OPEN_PRIZE	[8];
	char	HIGH_PRIZE	[8];
	char	LOW_PRIZE	[8];
	char	CLOSE_PRIZE	[8];	
}ST_TR_55001_O;

typedef struct _ST_TR_55002_I
{
	char	TR			[5];
	char	ACNT_NO		[11];
	char	OPEN		[10];
	char	HIGH		[10];
	char	LOW			[10];
	char	CLOSE		[10];
}ST_TR_55002_I;

typedef struct _ST_TR_55002_O
{
	char	TR			[5];
	char	SUCC_YN		[1];
	char	MSG			[100];
}ST_TR_55002_O;





//////////////////////////////////////////////////////////////////////////
//	��й�ȣ ���� ȭ��: X1TR51002 : ����  , ȭ���ȣ (5101)

typedef struct _ST_TR_51002_I 
{
	char		PwdGb		[ 2];			//	�н����� ���� �����ڵ� (CU:���Ӻ�� , AU:���º��)
	char		UserID		[MAX_USERID];	//	��ID
	char		AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ
	char		BeforePwd	[10];			//	���� ��й�ȣ
	char		AfterPwd	[10];			//	���� �� ��й�ȣ
	char		CheckPwd	[10];			//	���� ��й�ȣ Ȯ��
}ST_TR_51002_I;

typedef struct _ST_TR_51002_O
{
	char		Success_YN	[ 1];			//	��й�ȣ ���� ���� ���� (0:���� , 1:����)
}ST_TR_51002_O;


//////////////////////////////////////////////////////////////////////////
//	�������� ���� ȭ��: X1TR53001 : ��ȸ(Q),��û(U),���(D)  , ȭ���ȣ (5301)

typedef struct _ST_TR_53001_I 
{
	char		TrxTp		[ 1];			//	�������� ���汸�� �ڵ� (Q:��ȸ, U:��û, D:���)
	char		UserID		[MAX_USERID];	//	��ID
	char		AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ
	char		AcntPwd		[4];			//	���� ��й�ȣ
}ST_TR_53001_I;

typedef struct _ST_TR_53001_O
{
//	char		Success_Yn	[ 1];			//	���� �������� �ڵ� (Y:���� , N:����)
	char		OverStat_Yn	[ 1];			//	�������� �����ڵ� (Y:�������� ���� , N:�������� �Ұ�)
}ST_TR_53001_O;


//////////////////////////////////////////////////////////////////////////
//	��ü���ֹ���ȸ  , ȭ���ȣ (1102)

typedef struct _ST_TR_11001_I 
{
	char		ACNT_NO		[MAX_ACNT_NO];	//	���¹�ȣ
	char		ACNT_PWD	[4];			//	���� ��й�ȣ
	char		GDS_CODE	[2];
	char		STK_CODE	[MAX_STK_CODE];
}ST_TR_11001_I;

typedef struct _ST_TR_11001_O
{
	char	ACNT_NO		[MAX_ACNT_NO];
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
	char	ORD_NO		[MAX_ORD_NO];
	char	ORG_NO		[MAX_ORD_NO];
	char	BUY_SELL_TP	[1];
	char	ORD_PRC		[MAX_ORD_PRC];
	char	ORD_QTY		[MAX_ORD_QTY];
	char	REMN_QTY	[MAX_ORD_QTY];
	char	ORD_ST		[1];
	char	MNG_USERID	[MAX_USERID];

}ST_TR_11001_O;


//////////////////////////////////////////////////////////////////////////
//	�ܰ���ȸ(���ǵ��ֹ�â)
typedef struct _ST_TR_20021_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[MAX_ACNT_NO];
	char	STK_CODE	[MAX_STK_CODE];
	char	CRNC_CODE	[3];
}ST_TR_20021_I;

typedef struct _ST_TR_20021_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	CLTL		[15];
	char	AVG_PRC		[MAX_ORD_PRC];	
	char	ESTM_PL		[15];	//	�򰡼���
	char	CLR_PL		[15];	//	��������
	char	TOT_PL		[15];
	char	POS_QTY		[9];
	char	BUY_SELL_TP	[1];
	char	TICK_SIZE	[15];
	char	TICK_VALUE	[15];
	char	TOT_ESTM_ENTR	[15];
	char	ESTM_ENTR		[15];
	char	TOT_REMNQ_SELL	[4];
	char	TOT_REMNQ_BUY	[4];
	char	LOSS_CUT_AMT	[15];
	char	Leverage		[3];
}ST_TR_20021_O;



//////////////////////////////////////////////////////////////////////////
//	��ü���ܷ���ȸ(���ǵ��ֹ�â)
typedef struct _ST_TR_20022_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[MAX_ACNT_NO];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20022_I;

typedef struct _ST_TR_20022_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	PRC			[7];
	char	SELL_REMNQ	[4];
	char	BUY_REMNQ	[4];
}ST_TR_20022_O;



//////////////////////////////////////////////////////////////////////////
//	�ֹ����ɼ��� ��ȸ (���ǵ��ֹ�â)
typedef struct _ST_TR_20031_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[MAX_ACNT_NO];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20031_I;

typedef struct _ST_TR_20031_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	NOW_PRC		[MAX_ORD_PRC];
	char	ABLE_SELL	[4];
	char	ABLE_BUY	[4];
}ST_TR_20031_O;


typedef struct _ST_TR_20031_O_FX
{
	char	STK_CODE	[MAX_STK_CODE];
	char	NOW_PRC_S	[MAX_ORD_PRC];
	char	NOW_PRC_B	[MAX_ORD_PRC];
	char	ABLE_SELL	[4];
	char	ABLE_BUY	[4];
}ST_TR_20031_O_FX;


//////////////////////////////////////////////////////////////////////////
//	�ð����� �� ��ȸ (���ǵ��ֹ�â)
typedef struct _ST_TR_20032_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20032_I;

typedef struct _ST_TR_20032_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	NOW			[MAX_ORD_PRC];
	char	CHG			[7];	//	7,2
	char	CHG_RATE	[7];	//	7,2
	char	KS200		[MAX_ORD_PRC];
	//char	BS			[MAX_ORD_PRC];
	char	LAST_END_PRC[MAX_ORD_PRC];
	char	OPEN		[MAX_ORD_PRC];
	char	HIGH		[MAX_ORD_PRC];
	char	LOW			[MAX_ORD_PRC];
	char	OPEN_YAK	[9];	//	�̰�
	char	OPEN_YAK_CHG[9];	//	�̰�����
}ST_TR_20032_O;


typedef struct _ST_TR_20032_O_FX
{
	char	STK_CODE	[MAX_STK_CODE];
	char	NOW_S		[MAX_ORD_PRC];
	char	NOW_B		[MAX_ORD_PRC];
	char	CHG			[7];	//	7,2
	char	CHG_RATE	[7];	//	7,2
	char	KS200		[MAX_ORD_PRC];
	//char	BS			[MAX_ORD_PRC];
	char	LAST_END_PRC[MAX_ORD_PRC];
	char	OPEN		[MAX_ORD_PRC];
	char	HIGH		[MAX_ORD_PRC];
	char	LOW			[MAX_ORD_PRC];
	char	OPEN_YAK	[9];	//	�̰�
	char	OPEN_YAK_CHG[9];	//	�̰�����
}ST_TR_20032_O_FX;


//////////////////////////////////////////////////////////////////////////
//	�ü� ��ȸ (���ǵ��ֹ�â-30��)
typedef struct _ST_TR_20033_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20033_I;

typedef struct _ST_TR_20033_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	PRC			[20];
	char	QTY			[4];
}ST_TR_20033_O;


//////////////////////////////////////////////////////////////////////////
//	KS200 FUTURES ȣ������ (���ǵ��ֹ�â)
typedef struct _ST_TR_20034_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20034_I;

typedef struct _ST_TR_20034_O
{
	char	PRC			[7];
}ST_TR_20034_O;

//////////////////////////////////////////////////////////////////////////
//	KS200 FUTURES ȣ������ (���ǵ��ֹ�â)
typedef struct _ST_TR_20037_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20037_I;

typedef struct _ST_TR_20037_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	MAX			[7];
	char	MIN			[7];
	char	CNT			[4];
}ST_TR_20037_O;

//////////////////////////////////////////////////////////////////////////
//	KS200 OPTIONS ȣ������ (���ǵ��ֹ�â)
typedef struct _ST_TR_20035_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20035_I;

typedef struct _ST_TR_20035_O
{
	char	PRC			[7];
}ST_TR_20035_O;

//////////////////////////////////////////////////////////////////////////
//	KS200 OPTIONS ȣ������ (���ǵ��ֹ�â)
typedef struct _ST_TR_20038_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20038_I;

typedef struct _ST_TR_20038_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	MAX			[7];
	char	MIN			[7];
	char	CNT			[4];
}ST_TR_20038_O;


//////////////////////////////////////////////////////////////////////////
//	KS200 ������ �ܰ�/��ü�����
typedef struct _ST_TR_20039_I
{
	char	GDS_CODE	[2];
	char	ACNT_NO		[MAX_ACNT_NO];
}ST_TR_20039_I;

typedef struct _ST_TR_20039_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	BUY_SELL_TP	[1];
	char	CNT			[4];
	char	AVG_PRC		[7];
	char	BUY_NCNTR	[4];
	char	SELL_NCNTR	[4];
	char	NOW_PRC		[15];
	char	CLR_PL		[15];
}ST_TR_20039_O;



//////////////////////////////////////////////////////////////////////////
//	FU ȣ������ (���ǵ��ֹ�â)
typedef struct _ST_TR_20036_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_20036_I;

typedef struct _ST_TR_20036_O
{
	char	STK_CODE	[MAX_STK_CODE];
	char	MAX			[20];
	char	MIN			[20];
	char	CNT			[4];
	//char	PRC			[10];
}ST_TR_20036_O;



//////////////////////////////////////////////////////////////////////////
//	����û
typedef struct _ST_TR_52001_I
{
	char	GDS_CODE	[2];
	char	ACTION		[1];			//	'Q':��ȸ, 'A':��û, 'C':���
	char	ACNT_NO		[MAX_ACNT_NO];
	char	ACNT_PWD	[4];
}ST_TR_52001_I;

typedef struct _ST_TR_52001_O
{
	char	GDS_CODE	[2];
	char	ACTION		[1];			//	'Q':��ȸ, 'A':��û, 'C':���
	char	CLTL		[15];			//	�㺸�ݾ�
	char	DAY_CNT		[3];			//	�ܿ��ϼ�
	char	ACNT_STAT	[10];			//	�ŷ����� OR �ŷ��Ұ���
}ST_TR_52001_O;



//////////////////////////////////////////////////////////////////////////
//	Ư������ ���ͷ� ��ȸ(�ý��� ��ȣ ����1ȣ)
typedef struct _ST_TR_32001_I
{
	char	GROUP_IDX	[2];
	char	STK_CODE	[MAX_STK_CODE];
}ST_TR_32001_I;

typedef struct _ST_TR_32001_O
{
	char	GROUP_IDX	[2];
	char	STK_CODE	[MAX_STK_CODE];
	char	POS_QTY		[15];
	char	AVG_PRC		[MAX_ORD_PRC];
	char	EVLT_PL		[15];
	char	CLR_PL		[15];
	char	TOT_PL		[15];
}ST_TR_32001_O;

/*
	CLIENT �� �ü��� ���� ���� ���
*/
typedef struct _ST_TR_20090_I
{
	char	GDS_CODE	[2];
	char	STK_CODE	[MAX_STK_CODE];
	char	RegFlag		[1];			//	0:����, 1:����
}ST_TR_20090_I;


/*
	�������� ��ȸ �� ����
*/
typedef struct _ST_TR_60014_I
{
	char	PROC_TP		[1];	//	Q:��ȸ, U:����
	char	GDS_CODE	[2];
	char	ACNT_NO		[11];
	char	ACNT_PWD	[4];
    char	ENTR_RT		[3];	//	1,5,10,20,100
}ST_TR_60014_I;

typedef struct _ST_TR_60014_O
{
	char	Leverage	[3];	//	1,5,10,20,100
}ST_TR_60014_O;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************************/
//																											//
//					�����ڿ� COMPONENT(������Ʈ) ��Ŷ ���� (���ļ��� : MN ��ȣ��)							//
//																											//
/************************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	������ �α��� PACKET

// X1MN90001 : �α��� PACKET
typedef struct _ST_MN_90001_I
{
	char	STX				[ 1];
			C_HEADER		Header;
			TR_HEADER		TRHeader;
	char	ID				[MAX_USERID];	//	���� ������ ID
	char	Conn_Pwd		[10];			//	���� ������ ��й�ȣ
	char	MN_IP_Local		[15];			//	���� ������ IP (Local)
	char	MN_IP_Public	[15];			//	���� ������ IP (Public)
	char	MN_Mac			[12];			//	���� ������ MAC �ּ�
	char	ETX				[ 1];
}ST_MN_90001_I;

typedef struct _ST_MN_90001_O
{
	char	STX				[ 1];
			C_HEADER		Header;
			TR_HEADER		TRHeader;
	char	ManagerCd		[ 5];			//	���� ������ �ڵ� (���� : XX-XX , ��: 00-00, 01-01)
	char	SuccYn			[ 1];			//	���� ���� (Y:����, N:����)
	char	ETX				[ 1];
}ST_MN_90001_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���ں� ������ ��ü ��ȸ ������ ȭ��: X1MN92001

// X1MN92001 : ���񳻿� I/O
typedef struct _ST_MN_92001_I
{
	char	ManagerCd	[ 5];	//	�������ڵ� (���� : 00-00)
	char	StartDt		[ 8];	//	��ȸ �������� (YYYYMMDD)
	char	EndDt		[ 8];	//	��ȸ �� ����
	char	NextKey		[ 8];	//	���� ��ȸ (NextKey='0' �� ������ȸ, �������� NextKey='YYYYMMDD' �� ����)
}ST_MN_92001_I;


typedef struct _ST_MN_92001_O
{
	char	Dt			[ 8];	//	�Ϻ� ��ȸ���� (YYYYMMDD)
	char	SumEntr		[15];	//	�Ϻ� �ܰ� �հ�
	char	SumCltl		[15];	//	�Ϻ� ����� �հ�
	char	SumClrPl	[15];	//	�Ϻ� �������� �հ�
	char	SumCmsn		[15];	//	�Ϻ� ������ �հ�
	char	SumFee		[15];	//	�Ϻ� �̿�� �հ�
}ST_MN_92001_O;


// X1MN92002 : ���κ� ������ ��ȸ (�����ڿ�) I/O 
typedef struct _ST_MN_92002_I
{
	char	ManagerCd	[ 5];			//	�������ڵ�	(����: 00-00)
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ
	char	StartDt		[ 8];			//	��ȸ �������� (YYYYMMDD)
	char	EndDt		[ 8];			//	��ȸ �� ����
	char	NextKey		[ 24];			//	������ȸ: ȸ����ȣ+���¹�ȣ+��¥ ("T000101-01-0000020100831")
}ST_MN_92002_I;


typedef struct _ST_MN_92002_O
{
	char	ActiveYn	[ 1];			//	�α��� ���� (Y:���� , N:������)
	char	UserNo		[ 5];			//	ȸ����ȣ
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ
	char	UserID		[MAX_USERID];	//	ȸ�� ID
	char	UserNm		[20];			//	ȸ���̸�
	char	Dt			[ 8];			//	��¥
	char	Entr		[15];			//	�ܰ�
	char	ClrPl		[15];			//	��������
	char	Cmsn		[15];			//	������
	char	Fee			[15];			//	�̿��
}ST_MN_92002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ȸ�� �Ա� ��û ���� ��ȸ 

// X1MN93000 : ȸ�� ��/��� ���� ���ں� ���� ���� ��ȸ I/O
typedef struct _ST_MN_93000_I
{
	char	ManagerCd	[ 5];			//	�������ڵ�	(����: 00-00)
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ
	char	StartDt		[ 8];			//	��ȸ ������ (����: 20100822)
	char	EndDt		[ 8];			//	��ȸ ����	(����: 20100828)
	char	NextKey		[20];			//	������ȸŰ	(����: 2010082315:23:20:145 , 20�ڸ�) '0' �϶��� ��ü��ȸ
}ST_MN_93000_I;

typedef struct _ST_MN_93000_O
{
	char	Dt			[ 8];			//	����ó�� ��¥ (����: 20100826)
	char	UserNo		[ 5];			//	ȸ�� ��ȣ (����: A0001)
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX)
	char	UserID		[MAX_USERID];	//	ȸ�� ID
	char	UserNm		[20];			//	ȸ�� �̸�
	char	ReqCode		[ 2];			//	��û �ڵ� (�Ա�: 01 , ���: 02)
	char	ReqDt		[ 8];			//	��û ��¥
	char	ReqTm		[12];			//	��û �ð�
	char	ReqMoney	[15];			//	��û ��/���
	char	ProcDt		[ 8];			//	���� ��¥
	char	ProcTm		[12];			//	���� �ð�
	char	ProcMoney	[15];			//	���� ��/��ݾ�
}ST_MN_93000_O;

// X1MN93001 : ȸ�� �Ա� ��û ���� ��ȸ I/O
typedef struct _ST_MN_93001_I
{
	char	ManagerCd	[ 5];			//	�������ڵ� (���� : 00-00)
}ST_MN_93001_I;

typedef struct _ST_MN_93001_O
{
	char	UserNo		[ 5];			//	ȸ�� ��ȣ (A0001)
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ
	char	UserNm		[20];			//	ȸ�� �̸�
	char	Dt			[ 8];			//	��û ��¥
	char	Tm			[12];			//	��û �ð�
	char	ReqMoney	[15];			//	��û �Աݾ�
}ST_MN_93001_O;

// X1MN93002 : ȸ�� �Ա� ��û ==> ������ �Ա� ���� ó�� I/O (�����ڿ�) 
typedef struct _ST_MN_93002_I
{
	char	ManagerCd	[ 5];			//	�������ڵ� (���� : '00-00' , 5�ڸ� )
	char	ProcTp		[ 1];			//	ó������ ( ����: 'U' , ���: 'C' )
	char	DtTm		[20];			//	�Ա� ��û��¥ + �Ա� ��û�ð� (����: 'YYYYMMDD15:26:25.124' , 20�ڸ�)
	char	AcntNo		[MAX_ACNT_NO];	//	�Աݵ� ���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	ReqMoney	[15];			//	�Ա� ��û �ݾ�
	char	ReqGuJaCnt	[15];			//	��û ���¼�
}ST_MN_93002_I;

typedef struct _ST_MN_93002_O
{
	char	SuccYn		[ 1];			//	�������� ��ȯ (Y:���� , N:����)
}ST_MN_93002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ȸ�� ��� ��û ���� ��ȸ 

// X1MN93003 : ȸ�� ��� ��û ���� ��ȸ I/O
typedef struct _ST_MN_93003_I
{
	char		ManagerCd		[ 5];	//	�������ڵ� (���� : 00-00)
}ST_MN_93003_I;

typedef struct _ST_MN_93003_O
{
	char	UserNo			[ 5];			//	ȸ�� ��ȣ (A0001)
	char	AcntNo			[MAX_ACNT_NO];	//	���¹�ȣ
	char	UserNm			[20];			//	ȸ�� �̸�
	char	Dt				[ 8];			//	��û ��¥
	char	Tm				[12];			//	��û �ð�
	char	ReqMoney		[15];			//	��û ��ݾ�
	char	UserBankNm		[20];			//	�����
	char	UserBankAcntNo	[20];			//	���� ���¹�ȣ
	char	UserBankDpstNm	[20];			//	������
}ST_MN_93003_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ȸ�� �Ż����� ��ȸ / ���� (����) ȭ��: X1MN94001(��ȸ) , X1MN94002(����) :  ȭ���ȣ (9401)

// X1MN94001 : ȸ�� �Ż����� ���� ��ȸ I/O (�����ڿ�)
typedef struct _ST_MN_94001_I
{
	char		ManagerCd	[ 5];			//	�������ڵ� (���� : 00-00)
	char		NextKey		[16];			//	������ȸ Ű�� (ȸ����ȣ+���¹�ȣ)
}ST_MN_94001_I;

typedef struct _ST_MN_94001_O
{
	char	UserNo		[ 5];			//	ȸ����ȣ
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	UserID		[MAX_USERID];	//	ȸ�� ID
	char	ConnPwd		[10];			//	���� ��й�ȣ
	char	AcntPwd		[ 4];			//	���� �����ȣ
	char	HpNo		[20];			//	�ڵ��� ��ȣ
	char	TelNo		[20];			//	��ȭ��ȣ
	char	UserNm		[20];			//	ȸ�� �̸�
	char	UserBankNm	[20];			//	����� �����
	char	UserAcntNo	[20];			//	����� ���¹�ȣ
	char	Email		[50];			//	����� Email
}ST_MN_94001_O;


// X1MN94002 : ȸ�� �Ż����� ���� ���� I/O (�����ڿ�) 
typedef struct _ST_MN_94002_I
{
	char	UserNo		[ 5];			//	ȸ����ȣ
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	UserID		[MAX_USERID];	//	ȸ�� ID
	char	ConnPwd		[10];			//	���� ��й�ȣ
	char	AcntPwd		[ 4];			//	���� �����ȣ
	char	HpNo		[20];			//	�ڵ��� ��ȣ
	char	TelNo		[20];			//	��ȭ��ȣ
	char	UserNm		[20];			//	ȸ�� �̸�
	char	UserBankNm	[20];			//	����� �����
	char	UserAcntNo	[20];			//	����� ���¹�ȣ
	char	Email		[50];			//	����� Email
}ST_MN_94002_I;

typedef struct _ST_MN_94002_O
{
	char	SuccYn		[ 1];			//�������� ��ȯ (Y:���� , N:����)
}ST_MN_94002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ȸ�� �ŷ� ���� ���� ��ȸ / ���� ȭ��: X1MN94003(���� ���� ��ȸ) , X1MN94004(��������) :  ȭ���ȣ (9401)

// X1MN94003 : ȸ�� �ŷ� ���� ���� ��ȸ I/O (�����ڿ�)
typedef struct _ST_MN_94003_I
{
	char	ManagerCd	[ 5];			//	�������ڵ� (���� : 00-00)
}ST_MN_94003_I;

typedef struct _ST_MN_94003_O
{
	char	UserNo		[ 5];			//	ȸ�� ��ȣ
	char	AcntNo		[MAX_ACNT_NO];	//	���� ��ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	UserNm		[20];			//	ȸ�� �̸�
	char	ConnSysNm	[20];			//	���� �ý��� (����, ����)
	char	ApplyRt		[10];			//	���� ����
	char	MaxQty		[15];			//	�ִ� ���� ����
	char	CmsnRtF		[15];			//	���� ������
	char	CmsnRtO		[15];			//	�ɼ� ������
	char	CmsnAmt		[15];			//	���� ���� ������ �ݾ�
	char	GujaCnt		[15];			//	���¼�
	char	UserStat	[ 1];			//	����� �����ڵ� (�α��� ����: 4, ����: 5)
}ST_MN_94003_O;


// X1MN94004 : ȸ�� �ŷ� ���� ���� ���� I/O (�����ڿ�) 
typedef struct _ST_MN_94004_I
{
	char	ManageCd	[ 5];			//	������ �ڵ� (����: 00-00, 5�ڸ�)
	char	AcntNo		[MAX_ACNT_NO];	//	���� ��ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	ConnSysNm	[20];			//	���� �ý��� (����, ����)
	char	ApplyRt		[10];			//	���� ����
	char	MaxQty		[15];			//	�ִ� ���� ����
	char	CmsnRtF		[15];			//	���� ������
	char	CmsnRtO		[15];			//	�ɼ� ������
	char	CmsnAmt		[15];			//	���� ���� ������ �ݾ�
	char	GujaCnt		[15];			//	���¼�
	char	UserStat	[ 1];			//	����� �����ڵ� (�α��� ����: 4, ����: 5)
}ST_MN_94004_I;

typedef struct _ST_MN_94004_O
{
	char		SuccYn		[ 1];			//�������� ��ȯ (Y:���� , N:����)
}ST_MN_94004_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�������� ȭ��: X1MN96001(����) , X1MN96011(����), X1MN96002(����/�߼�) :  ȭ���ȣ (9601)

// X1MN96001 : ���񳻿� I/O
typedef struct _ST_MN_96001_I
{
	char	StanDate	[ 8];	//	�������� (YYYYMMDD)
	char	UrgentTp	[ 1];	//	��ޱ��� (0.��ü��ȸ, 1.����, 2.��Ȳ, 3.���)
	char	Seq			[20];	//	��������ȣ (0. ��ü, �׿� Number �� ��ȸ)
	char	BtnGb		[ 1];	//	������ư ���� (0. ���� , 1. ����)
}ST_MN_96001_I;

typedef struct _ST_MN_96001_O
{
	char	StanDate	[ 8];	//	�������� (YYYYMMDD)
	char	StanTime	[ 8];	//	�����ð� (hh:mm:ss)
	char	Seq			[20];	//	��û��������ȣ
	char	Title		[80];	//	��������
	char	SendYn		[ 1];	//	�߼ۿϷ� ���� (Y:�߼ۿϷ�, N:�߼۴��) --2010.08.16 �߰�
}ST_MN_96001_O;


// X1MN96002 : ������ ���� ����/�߼� I/O 
typedef struct _ST_MN_96002_I
{
	char	UrgentTp	[  1];	//	��ޱ��� (1.����, 2.��Ȳ, 3.���)
	//char	UserID		[MAX_USERID];	//SMILOR. ������������
	char	Title		[ 80];	//	��������
	//char	Body		[800]; //	��������
	char	Body		[1500]; //	��������
}ST_MN_96002_I;

typedef struct _ST_MN_96002_O
{
	char	SuccYn	[  1];	//�������� ��ȯ (Y:���� , N:����)
}ST_MN_96002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���������� ��ȸ / ���� (�߰�/����/����) ȭ��: X1MN97001(��ȸ) , X1MN97002(����) :  ȭ���ȣ (9701)

// X1MN97001 : ������ ���� ��ȸ I/O
typedef struct _ST_MN_97001_I
{
	char		ManagerCd	[ 5];	//	�������ڵ� (���� : 00-00)
}ST_MN_97001_I;

typedef struct _ST_MN_97001_O
{
	char	ManagerCd	[ 5];	//	�������ڵ� (���� : XX-XX , ��: 00-00)
	char	ID			[12];	//	������ ID
	char	ConnPwd		[10];	//	������ ���� ��й�ȣ
	char	BankNm		[50];	//	������ �����
	char	AcntDpNm	[20];	//	������ ������
	char	AcntNo		[20];	//	������ ���¹�ȣ
	char	AcntPwd		[ 4];	//	������ ���� ��й�ȣ
	char	IP			[15];	//	������ ������
	char	Phone		[20];	//	������ ��ȭ
	char	Email		[50];	//	������ Email
}ST_MN_97001_O;


// X1MN97002 : ������ ���� ���� I/O 
typedef struct _ST_MN_97002_I
{
	char	ProcGb			[ 1];	//	ó�� �����ڵ� (N:�߰�, C:����, D:����)
	char	ConnManagerCd	[ 5];	//	���� �������ڵ� (���� : XX-XX ��: 00-00)
	char	ManagerCd		[ 5];	//	ó����� �������ڵ� (���� : XX-XX ��: 00-00)
	char	ID				[12];	//	������ ID
	char	ConnPwd			[10];	//	������ ���� ��й�ȣ
	char	BankNm			[50];	//	������ �����
	char	AcntDpNm		[20];	//	������ ������
	char	AcntNo			[20];	//	������ ���¹�ȣ
	char	AcntPwd			[ 4];	//	������ ���� ��й�ȣ
	char	IP				[15];	//	������ ������
	char	Phone			[20];	//	������ ��ȭ
	char	Email			[50];	//	������ Email
}ST_MN_97002_I;

typedef struct _ST_MN_97002_O
{
	char		SuccYn		[ 1];	//�������� ��ȯ (Y:���� , N:����)
}ST_MN_97002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�� ���� ���� ������ ��ȸ / ���� (����) ȭ��: X1MN99001(��ȸ) , X1MN99002(����) :  ȭ���ȣ (9901)

// X1MN99001 : ������ ���� ��ȸ I/O
typedef struct _ST_MN_99001_I
{
	char		ManagerCd	[ 5];	//	�������ڵ� (���� : 00-00)
}ST_MN_99001_I;

typedef struct _ST_MN_99001_O
{
	char	TrdeDt_Ks		[8];	//	KS ���� ������
	char	BatchStatus_Ks	[1];	//	KS ���� �Ϲ�ġ ����
	char	ManageGb_Ks		[1];	//	KS ���� ����� (0:�尳����, 1:�尳��, 2:�帶��, 3:������)
	char	StartTm_Ks		[5];	//	KS ���� �尳�� �ð�
	char	EndTm_Mtr_KsF	[5];	//	KS ���� ���� �������� �帶�� �ð�
	char	EndTm_Mtr_KsO	[5];	//	KS ���� �ɼ� �������� �帶�� �ð�
	char	EndTm_KsF		[5];	//	KS ���� ���� �Ϲ� �帶�� �ð�
	char	EndTm_KsO		[5];	//	KS ���� �ɼ� �Ϲ� �帶�� �ð�
	char	TrdeDt_FuFx		[8];	//	FUFX ���� ������
	char	BatchStatus_FuFx[1];	//	FUFX ���� �Ϲ�ġ ����
	char	ManageGb_FuFx	[1];	//	FUFX ���� ����� (0:�尳����, 1:�尳��, 2:�帶��, 3:������)
	char	StartTm_FuFx	[5];	//	FUFX ���� �尳�� �ð�
	char	EndTm_FuFx		[5];	//	FUFX ���� �帶�� �ð�
}ST_MN_99001_O;


// X1MN99002 : ������ ���� ���� I/O 
typedef struct _ST_MN_99002_I
{
	char	ManagerCd		[5];	//	�������ڵ� (���� : 00-00)
	char	ManageGb_Ks		[1];	//	KS ���� ����� (0:�尳����, 1:�尳��, 2:�帶��, 3:������)
	char	StartTm_Ks		[5];	//	KS ���� �尳�� �ð�
	char	EndTm_Mtr_KsF	[5];	//	KS ���� ���� �������� �帶�� �ð�
	char	EndTm_Mtr_KsO	[5];	//	KS ���� �ɼ� �������� �帶�� �ð�
	char	EndTm_KsF		[5];	//	KS ���� ���� �Ϲ� �帶�� �ð�
	char	EndTm_KsO		[5];	//	KS ���� �ɼ� �Ϲ� �帶�� �ð�
	char	ManageGb_FuFx	[1];	//	FUFX ���� ����� (0:�尳����, 1:�尳��, 2:�帶��, 3:������)
	char	StartTm_FuFx	[5];	//	FUFX ���� �尳�� �ð�
	char	EndTm_FuFx		[5];	//	FUFX ���� �帶�� �ð�
}ST_MN_99002_I;

typedef struct _ST_MN_99002_O
{
	char		SuccYn		[ 1];	//�������� ��ȯ (Y:���� , N:����)
}ST_MN_99002_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ȸ������ �˻� ȭ��: X1MN70100 (�������� ���)

// X1MN70100 : ȸ�� �˻��Ͽ� �ش�ȸ���� ���¹�ȣ ��� ��ȸ I/O (�����ڿ�)
typedef struct _ST_MN_70100_I
{
	char		ProcTp	[1];			//	ó������ ('0':ALL , '1': ID�˻� , '2': �̸��˻�)
	char		UserID	[MAX_USERID];	//	ȸ�� ID
	char		UserNm	[20];			//	ȸ���̸�
}ST_MN_70100_I;

typedef struct _ST_MN_70100_O
{
	char		AcntNo	[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
}ST_MN_70100_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ȸ�� �Ż����� ��ȸ / ���� (����) ȭ��: X1MN70101(��ȸ) , X1MN70102(����)

// X1MN70101 : ȸ�� �Ż����� ���� ��ȸ I/O (�����ڿ�)
typedef struct _ST_MN_70101_I
{
	char	AdminID		[MAX_USERID];	//	������ID
//	char	GdsCode		[ 2];			//	GDS_CODE
	char	UserNm		[20];			//	ȸ���̸�
	char	NextKey		[11];			//	������ȸ Ű�� (ȸ����ȣ+���¹�ȣ)
}ST_MN_70101_I;

typedef struct _ST_MN_70101_O
{
	char	UserID		[MAX_USERID];	//	ȸ�� ID
	char	UserNm		[20];			//	ȸ���̸�
	char	ConnPwd		[10];			//	���� ��й�ȣ
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	AcntPwd		[ 4];			//	���� �����ȣ
	char	UserBankNm	[20];			//	����� �����
	char	UserAcntNo	[20];			//	����� ���¹�ȣ
	char	HpNo		[20];			//	�ڵ��� ��ȣ
	char	Email		[50];			//	����� Email
	char	AdminID		[MAX_USERID];	//	������ID
	char	AdminNm		[20];			//	�����ڸ�
}ST_MN_70101_O;


// X1MN70102 : ȸ�� �Ż����� ���� ���� I/O (�����ڿ�) 
typedef struct _ST_MN_70102_I
{
	char	ProcTp		[ 1];			//	ó������ (I: ȸ���߰�, U: ȸ������ ����, D: ȸ������)
	char	UserID		[MAX_USERID];	//	ȸ�� ID
	char	UserNm		[20];			//	ȸ���̸�
	char	ConnPwd		[10];			//	���� ��й�ȣ
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	AcntPwd		[ 4];			//	���� �����ȣ
	char	UserBankNm	[20];			//	����� �����
	char	UserAcntNo	[20];			//	����� ���¹�ȣ
	char	HpNo		[20];			//	�ڵ��� ��ȣ
	char	Email		[50];			//	����� Email
	char	AdminID		[MAX_USERID];	//	������ID
}ST_MN_70102_I;

typedef struct _ST_MN_70102_O
{
	char		SuccYn	[ 1];			//�������� ��ȯ (Y:���� , N:����)
}ST_MN_70102_O;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ȸ�� �Ż����� ��ȸ / ���� (����) ȭ��: X1MN90101(��ȸ) , X1MN90102(����)

// X1MN70101 : ȸ�� �Ż����� ���� ��ȸ I/O (�����ڿ�)
typedef struct _ST_MN_90101_I
{
	char	AdminID		[MAX_USERID];	//	������ID
	char	GdsCode		[ 2];			//	GDS_CODE
	char	SelCond		[1];			//	��ȸ ���� (0:��ü, 1:����, 2:��ID, 3:���¹�ȣ, 4:����ȣ)
	char	SelVal		[20];			//	
	char	NextKey		[11];
}ST_MN_90101_I;

typedef struct _ST_MN_90101_O
{
	char	UserID[MAX_USERID];
	char	UserNm[20];
	char	ConnPwd[10];
	char	AcntNo[MAX_ACNT_NO];
	char	AcntPwd[4];
	char	UserBankNm[20];
	char	UserAcntNo[20];
	char	HpNo[20];
	char	TelNo[20];
	char	Email[50];
	char	AdminID[12];
	char	AdminNm[20];
	char	USER_NO[5];
	char	REG_NO[13];
	char	FIRST_TRDE_DT[8];
	char	PARTNER_ID[MAX_USERID];
	char	MAX_GUJA[4];
	char	CHG_RT_F[10];
	char	CHG_RT_O[10];
	char	USER_STAT[1];
	char	API_CODE[12];
	char	LEVERAGE[3];
	char	GRADE[1];
	char	Nick_NM[20];
}ST_MN_90101_O;

typedef struct _ST_MN_90102_I
{
	char	UserID[MAX_USERID];
	char	UserNm[20];
	char	ConnPwd[10];
	char	AcntNo[MAX_ACNT_NO];
	char	AcntPwd[4];
	char	UserBankNm[20];
	char	UserAcntNo[20];
	char	HpNo[20];
	char	TelNo[20];
	char	Email[50];
	char	AdminID[12];
	char	AdminNm[20];
	char	USER_NO[5];
	char	REG_NO[13];
	char	FIRST_TRDE_DT[8];
	char	PARTNER_ID[MAX_USERID];
	char	MAX_GUJA[4];
	char	CHG_RT_F[10];
	char	CHG_RT_O[10];
	char	USER_STAT[1];
	char	API_CODE[12];
	char	LEVERAGE[3];
	char	ACTION[1];	//	U:UPD, I:INS
	char	GRADE [1];
	char	NICK_NM	[20];
	char	BLACK_REASON[100];
}ST_MN_90102_I;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ȸ�� �������� ��ȸ / ���� (����) ȭ��: X1MN70103(��ȸ) , X1MN70104(����)

// X1MN70103 : ȸ�� �������� ��ȸ I/O (�����ڿ�)
typedef struct _ST_MN_70103_I
{
	char	AdminID		[MAX_USERID];	//	������ID
//	char	GdsCode		[ 2];			//	GDS_CODE
	char	UserID		[MAX_USERID];	//	ȸ��ID
	char	NextKey		[11];			//	������ȸ Ű�� (���¹�ȣ)
}ST_MN_70103_I;

typedef struct _ST_MN_70103_O
{
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	AcntNm		[20];			//	���¸�
	char	UserID		[MAX_USERID];	//	ȸ�� ID
	char	Entr		[15];			//	�����ܰ�
	char	Cltl		[15];			//	�㺸��
	char	GujaCnt		[15];			//	���¼�
	char	RemnDay		[15];			//	�ܿ��� (�����ϼ�)
	char	AcntStat	[ 1];			//	���»��� (1:�ŷ�����, 2:������, 3:�ŷ��Ұ�, 9:��� �̽�û)
	char	ConnSysNm	[20];			//	���Ӽ���
	char	CmsnRt_F	[15];			//	���� ��������
	char	CmsnRt_O	[15];			//	�ɼ� ��������
	char	RegDate		[ 8];			//	����� (����: YYYYMMDD , ��: 20100928)
}ST_MN_70103_O;


// X1MN70104 : ȸ�� �������� ���� I/O (�����ڿ�) 
typedef struct _ST_MN_70104_I
{
	char	ProcTp		[ 1];			//	ó������ (I: ȸ���߰�, U: ȸ������ ����, D: ȸ������)
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)	--����Ұ�
	char	AcntNm		[20];			//	���¸�									--����Ұ�
	char	UserID		[MAX_USERID];	//	ȸ�� ID									--����Ұ�
	char	Entr		[15];			//	�����ܰ�								--����Ұ�
	char	Cltl		[15];			//	�㺸��									--����Ұ�
	char	GujaCnt		[15];			//	���¼�
	char	RemnDay		[15];			//	�ܿ��� (�����ϼ�)
	char	AcntStat	[ 1];			//	���»��� (1:�ŷ�����, 2:������, 3:�ŷ��Ұ�, 9:��� �̽�û)
	char	ConnSysNm	[20];			//	���Ӽ���
	char	CmsnRt_F	[15];			//	���� ��������
	char	CmsnRt_O	[15];			//	�ɼ� ��������
	char	RegDate		[ 8];			//	����� (����: YYYYMMDD , ��: 20100928)
}ST_MN_70104_I;

typedef struct _ST_MN_70104_O
{
	char	SuccYn		[ 1];			//�������� ��ȯ (Y:���� , N:����)
}ST_MN_70104_O;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ȸ�� ���� �ܰ�/�Ÿ� ���� ���� ��ȸ: X1MN70105(�ܰ� ��ȸ) , X1MN70107(�Ÿų��� ��ȸ)

// X1MN70105 : ���� ���ں� �ܰ��� (�����ڿ�)
typedef struct _ST_MN_70105_I
{
	char	GdsCode		[MAX_GDS_CODE];	//	����/�ؿ� �����ڵ� (����:00 or 01 , �ؿ�: 02)
	char	StartDt		[8];			//	��ȸ �������� (����: YYYYMMDD, ��: 20100928)
	char	EndDt		[8];			//	��ȸ �� ���� (����: YYYYMMDD, ��: 20100928)
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	NextKey		[8];			//	������ȸ - ���� (����: YYYYMMDD, ��: 20100928)
}ST_MN_70105_I;

typedef struct _ST_MN_70105_O
{
	char	Dt			[ 8];			//	��¥ (����: YYYYMMDD, ��: 20100928)
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	PrevCltl	[15];			//	�����ܰ�
	char	InCltl		[15];			//	�����Ա� �㺸��
	char	OutCltl		[15];			//	������� �㺸��
	char	ClrPl		[15];			//	�ŷ�����
	char	Cmsn		[15];			//	������
	char	Fee			[15];			//	�̿��
	char	Cltl		[15];			//	���� �ܰ�
	char	CrncCode	[ 3];			//	��ȭ�ڵ�
}ST_MN_70105_O;

// X1MN70106 : ȸ����ü �ܰ��� ��ȸ (�����ڿ�)
typedef struct _ST_MN_70106_I
{
	char	ProcTp		[ 1];			//	��ȸ���� (0: ��ü��ȸ, 1: üũ�ܰ�, 2: üũ����)
	char	GdsCode		[MAX_GDS_CODE];	//	����/�ؿ� �����ڵ� (��������:00 , �����ɼ�: 01 , �ؿ�: 02)
	char	StanDate	[ 8];			//	��ȸ ���� (����: YYYYMMDD, ��: 20100928)
	char	NextKey		[MAX_ACNT_NO];	//	������ȸ Ű (Ű: ���¹�ȣ , ����: XX-XX-XXXXX , 11�ڸ�)
	char	MiniYN		[1];
}ST_MN_70106_I;

typedef struct _ST_MN_70106_O
{
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	UserNm		[20];			//	ȸ���̸�
	char	PrevCltl	[15];			//	�����ܰ�
	char	Cltl		[15];			//	�����ܰ� (�����ܰ�)
	char	InCltl		[15];			//	�����Ա� �㺸��
	char	OutCltl		[15];			//	������� �㺸��
	char	SumClrPl	[15];			//	�Ѽ���
	char	ClrPl		[15];			//	�ŷ�����
	char	Cmsn		[15];			//	������
	char	Fee			[15];			//	�̿��
	char	PL_F		[15];
	char	PL_O		[15];
	char	CMSN_F		[15];
	char	CMSN_O		[15];

}ST_MN_70106_O;

// X1MN70107 : ���ں� �Ÿų��� ��ȸ ��ȸ I/O (�����ڿ�)
typedef struct _ST_MN_70107_I
{
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	GdsCode		[MAX_GDS_CODE];	//	����/�ؿ� �����ڵ� (����:00 or 01 , �ؿ�: 02)
	char	StkCode		[MAX_STK_CODE];	//	ȸ��ID
	char	StartDt		[ 8];			//	��ȸ �������� (����: YYYYMMDD, ��: 20100928)
	char	EndDt		[ 8];			//	��ȸ �� ���� (����: YYYYMMDD, ��: 20100928)
	char	NextKey		[20];			//	������ȸ Ű�� (ü������+ ü���ȣ?) : TODO : ?????
}ST_MN_70107_I;

typedef struct _ST_MN_70107_O
{
	char	CntrDt		[ 8];			//	ü������ (����: YYYYMMDD, ��: 20100928)
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	StkCode		[MAX_STK_CODE];	//	��������
	char	BuySellTp	[ 1];			//	�Ÿű��� (1:�ŵ� , 2:�ż�)
	char	OrdPrice	[15];			//	�ֹ�����
	char	OrdQty		[15];			//	�ֹ�����
	char	OrdTp		[ 1];			//	�ֹ����� (1:���尡 , 2:������)
	char	OrdTime		[12];			//	�ֹ��ð� (����: hh:mm:ss:xxx, ��: 15:23:45:211)
	char	CntrPrice	[15];			//	ü�ᰡ��
	char	CntrQty		[15];			//	ü�����
	char	CntrTime	[12];			//	ü��ð� (����: hh:mm:ss:xxx, ��: 15:23:45:211)
	char	ClrPl		[15];			//	����
	char	Cmsn		[15];			//	������
}ST_MN_70107_O;

// X1MN70108 : ȸ����ü ü�᳻�� ��ȸ (�����ڿ�)
typedef struct _ST_MN_70108_I
{
	char	GdsCode		[MAX_GDS_CODE];	//	����/�ؿ� �����ڵ� (��������:00 , �����ɼ�: 01 , �ؿ�: 02)
	char	StanDate	[ 8];			//	��ȸ ���� (����: YYYYMMDD, ��: 20100928)
	char	NextKey		[MAX_ACNT_NO];	//	������ȸ Ű (Ű: ���¹�ȣ , ����: XX-XX-XXXXX , 11�ڸ�)
	char	MiniYN		[1];
}ST_MN_70108_I;

typedef struct _ST_MN_70108_O
{
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	UserNm		[20];			//	ȸ���̸�
	char	Apply_Rt	[15];			//	�������
	char	CntrQty_F	[15];			//	���� ü�� ����
 	char	AVG_PRC_F	[20];			//	���ȯ��
// 	char	ESTM_PL_F	[15];			//	�򰡼���
	char	UnCntrQty_F	[15];			//	���� ��ü�� ����
	char	CntrQty_C	[15];			//	�� ü�� ����
	char	UnCntrQty_C	[15];			//	�� ��ü�� ����
	char	CntrQty_P	[15];			//	ǲ ü�� ����
	char	UnCntrQty_P	[15];			//	ǲ ��ü�� ����
}ST_MN_70108_O;

// X1MN70118 : ����ȸ�� �� ü�᳻�� ��ȸ (�����ڿ�)
typedef struct _ST_MN_70118_I
{
	char	GDS_CODE	[2];
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX , 11�ڸ�)
	char	NextKey		[MAX_STK_CODE];	//	������ȸ Ű (Ű: �����ڵ�)
}ST_MN_70118_I;

typedef struct _ST_MN_70118_O
{
	char	StkCode		[MAX_STK_CODE];	//	�����ڵ�
	char	Apply_Rt	[15];			//	�������
	char	UnCntrQty	[15];			//	��ü�� ����
	char	CntrQty		[15];			//	ü�� ����
	char	AvgCntrPrc	[15];			//	ü�� ��հ�
	char	NowPrc		[15];			//	���簡��
	char	EvalPnlAmt	[15];			//	�򰡼���
}ST_MN_70118_O;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	X1 ȸ���Ա� / ��� ��û ��ȸ

// X1MN70301 : ȸ�� �Ա� / ��� ��û ��ȸ (�����ڿ�)
typedef struct _ST_MN_70301_I
{
	char	ACTION		[ 1];	//	I:�Ա�, O:���
	char	GDS_CODE	[ 2];
	char	MNG_ID		[MAX_USERID];
}ST_MN_70301_I;

typedef struct _ST_MN_70301_O
{
	char	SEQNO			[10];
	char	ACTION			[ 1];		//	I:�Ա�, O:���
	char	GDS_CODE		[ 2];
	char	ACNT_NO			[MAX_ACNT_NO];
	char	NAME			[20];
	char	APPL_DT			[ 8];		//	��û����
	char	APPL_TM			[ 8];		//	��û�ð�
	char	REQ_AMT			[10];		//	��û�ݾ�
	char	REQ_AMT_2		[10];		//	�ؿܼ����� ��� ��ȭ�ݾ�
	char	FOREX			[7];
	char	APPL_DAY_CNT	[ 3];		//	(�Ա�)����û�ϼ�
	char	APPL_GUJA_CNT	[ 3];		//	(�Ա�)����û ���¼�
	char	OUT_BANK_NM		[10];		//	(���)��� �����
	char	OUT_ACNT_NO		[20];		//	(���)��ݰ��¹�ȣ
	char	ACNT_OWNER		[10];		//	(���)������
}ST_MN_70301_O;


// X1MN70302 : ȸ�� �Ա� / ��� ��û ó�� (�����ڿ�)
typedef struct _ST_MN_70302_I
{
	char	ACTION			[ 1];		//	I:�Ա�, O:���
	char	SEQNO			[10];
	char	GDS_CODE		[ 2];
	char	ACNT_NO			[MAX_ACNT_NO];
	char	APPL_DAY_CNT	[ 3];		//	(�Ա�)����û�ϼ�
	char	APPL_GUJA_CNT	[ 3];		//	(�Ա�)����û ���¼�
	char	REQ_AMT			[10];		//	��û�ݾ�
	char	RESULT			[ 1];		//	A:����, R:����
	char	MNG_ID			[MAX_USERID];
}ST_MN_70302_I;

typedef struct _ST_MN_70302_O
{
	char	ACTION			[ 1];		//	I:�Ա�, O:���
	char	GDS_CODE		[ 2];
	char	ACNT_NO			[MAX_ACNT_NO];
	char	RESULT			[ 1];		//	A:����, R:����
}ST_MN_70302_O;

// X1MN70303 : ȸ�� ��/��� ���� ���ں� ���� ���� ��ȸ I/O (�����ڿ�)
typedef struct _ST_MN_70303_I
{
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ ('0' �϶��� ��ü ��ȸ) 
	char	StartDt		[ 8];			//	��ȸ ������ (����: 20100822)
	char	EndDt		[ 8];			//	��ȸ ����	(����: 20100828)
	char	NextKey		[20];			//	������ȸŰ	(����: 2010082315:23:20:145 , 20�ڸ�) '0' �϶��� ��ü��ȸ
}ST_MN_70303_I;

typedef struct _ST_MN_70303_O
{
	char	Dt			[ 8];			//	����ó�� ��¥ (����: 20100826)
	char	AcntNo		[MAX_ACNT_NO];	//	���¹�ȣ (����: XX-XX-XXXXX)
	char	UserID		[MAX_USERID];	//	ȸ�� ID
	char	UserNm		[20];			//	ȸ�� �̸�
	char	ReqCode		[ 2];			//	��û �ڵ� (�Ա�: 01 , ���: 02)
	char	ReqDt		[ 8];			//	��û ��¥
	char	ReqTm		[12];			//	��û �ð�
	char	ReqMoney	[15];			//	��û ��/���
	char	ResultYn	[ 1];			//	ó����� (Y:ó���Ϸ� , N:��ó��)
	char	ProcMoney	[15];			//	���� ��/��ݾ�
	char	ProcDt		[ 8];			//	���� ��¥
	char	ProcTm		[12];			//	���� �ð�
}ST_MN_70303_O;

//////////////////////////////////////////////////////////////////////////
//	�ǽð��뺸�� ���� X1MN84901 ȣ�� (SMILOR.������������)
typedef struct _ST_MN_84901_I
{
	char	UserID		[MAX_USERID];
	char	AllYN		[1];
	char	Title		[128];
	char	Msg			[MAX_BUFSIZE];
}ST_MN_84901_I;

typedef struct _ST_MN_84901_O
{
	char Succ_YN	[1];
	char Reserved	[32];
}ST_MN_84901_O;


//////////////////////////////////////////////////////////////////////////
//	������Ÿ���� ������ȭ��
/*
	Code
		- 10001 : �ü���Ʈ��
	
	SubCode	
		-	0011:����ETK����, 0012:����ETK����, 
			0021:�ؿ��ϳ���������, 0022:�ؿ��ϳ���������, 
			0031:�ؿַܼθ����, 0032:�ؿַܼθ�����
*/
typedef struct _ST_MN_99999_I
{
	char	Code	[4];		//	1001 : �ü���Ʈ��, 1002 : �����α׿���, 1003 : ���ΰ�ħ
	char	SubCode	[4];		//	0011:��������, 0012:��������, 0021:�ؿ��ϳ���������,
	char	Body	[512];
}ST_MN_99999_I;


typedef struct _ST_MN_99999_O
{
	char	Code	[4];
	char	SubCode	[4];
	char	SuccYN	[1];
	char	Body	[512];
}ST_MN_99999_O;


// X1MN70303 : ȸ�� �Ա� / ��� ���ں� ��ȸ (�����ڿ�)
// typedef struct _ST_MN_70303_I
// {
// 	char	ACTION			[1];	//	A: ALL, I:�Ա�, O:���
// 	char	GDS_CODE		[2];
// 	char	STRT_DT			[ 8];	//	��ȸ ������ (����: 20100822)
// 	char	END_DT			[ 8];	//	��ȸ ����	(����: 20100828)
// 	char	NEXT_KEY		[10];	//	������ȸŰ	(SEQNO)
// 	char	MNG_ID			[MAX_USERID];
// }ST_MN_70303_I;
// 
// typedef struct _ST_MN_70303_O
// {
// 	char	SEQNO			[10];
// 	char	ACTION			[1];	//	I:�Ա�, O:���
// 	char	GDS_CODE		[2];
// 	char	ACNT_NO			[MAX_ACNT_NO];
// 	char	NAME			[20];
// 	char	APPL_DT			[8];	//	��û����
// 	char	APPL_TM			[12];	//	��û�ð�
// 	char	REQ_AMT			[10];	//	��û�ݾ�
// //	char	APPL_DAY_CNT	[3];	//	����û�ϼ�
// //	char	APPL_GUJA_CNT	[3];	//	����û ���¼�
// //	char	CAN_OUT_AMT		[10];	//	��ݰ��ɱݾ� 
// //	char	OUT_BANK_NM		[10];	//	��� �����
// //	char	OUT_ACNT_NO		[20];	//	��ݰ��¹�ȣ
// //	char	ACNT_OWNER		[10];	//	������
// 	char	RESULT			[1];	//	A:����, R:����
// }ST_MN_70303_O;


#endif
