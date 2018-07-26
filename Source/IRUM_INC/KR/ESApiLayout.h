//////////////////////////////////////////////////////////////////////
// ESApiLayout.h	: ���� ���α׷����� ����ϴ� ����
//
//////////////////////////////////////////////////////////////////////

#if !defined(__COMMON_ESAPILAYOUT_FORMAT_H__)
#define __COMMON_ESAPILAYOUT_FORMAT_H__

#include <afxtempl.h>

enum _tagErrorCode
{
	ERR_NONE			= 0,
	ERR_AUTOKEY,			// ���� �ڵ�������Ʈ key ���Է�
	ERR_USERID,				// ���� ���̵� ���Է�
	ERR_USERPASS,			// ���� ��й�ȣ ���Է�
	ERR_CERTPASS,			// ���� ���� ��й�ȣ ���Է� (����� ����)
	ERR_SVRSENDDATA,		// ���� ���� ����
	ERR_SVRCONNECT,			// ���� ���� ����
	ERR_SVRNOCONNECT,		// ���� ������ ����
	ERR_CONNECT,			// ���� ���� ����
	ERR_CERTSET,			// ���� ������ �������� ����
	ERR_DLLNOEXIST,			// signkorea dll ���� ���
	ERR_TRCODE,				// ������ ���� TR��ȣ
};

enum _tagServerKind
{
	SERVER_REAL,
	SERVER_TEST,
	SERVER_DEMO,
	SERVER_REAL_80,
	SERVER_TEST_80,
	SERVER_DEMO_80,
	SERVER_STAFF,
	SERVER_STAFF_80,
};

enum _tagNextKind
{
	NEXT_NONE	= '0',
	NEXT_DATA	= '1',
};

#define OUTSCALE_10		'0' //10����
#define OUTSCALE_32		'1' //32����
#define OUTSCALE_64		'2' //64����
#define OUTSCALE_128	'3' //128����
#define OUTSCALE_4		'4' //4����
#define OUTSCALE_8		'5' //8����

#define TICKVALUE_4		    25
#define TICKVALUE_8		   125
#define TICKVALUE_32	  3125
#define TICKVALUE_64	 15625
#define TICKVALUE_128	 78125

///////////////////////////////////////////////////////////////////////////////
// ������ ����
typedef	struct	_ESAccntInfo
{
	char	AccNo				[11];	/* ���¹�ȣ		*/
	char	AccNm				[30];	/* ���¸�		*/
	char	AccPass				[ 8];	/* ���� ��й�ȣ	*/
}	ESAccntInfo_st, * lpESAccntInfo_st;
#define	SZ_ESAccntInfo	sizeof(ESAccntInfo_st)

typedef CArray < ESAccntInfo_st, ESAccntInfo_st > CArrayAcct;

// ����ĸ ������ ����
typedef	struct	_ESAccntInfoFC
{
	char	AccNo				[11];	/* ���¹�ȣ							*/
	char	AccNm				[30];	/* ���¸�							*/
	char	AccPass				[ 8];	/* ���� ��й�ȣ						*/
	char    leader_grade        [ 1];	/* N: None(��޾���)				*/
										/* C: Challenger				*/
										/* P: Pro						*/
										/* B: Best						*/
										/* S: SuperStar					*/
	char    system_yn			[ 1];	/* Y: �ý��� N: �Ϲ�				*/
	char    common_nego_fee		[19];	/* �Ϲ�, �̴ϻ�ǰ ������			*/
	char    micro_nego_fee		[19];	/* ����ũ�� ��ǰ ������			*/
}	ESAccntInfoFC_st, * lpESAccntInfoFC_st;
#define	SZ_ESAccntInfoFC	sizeof(ESAccntInfoFC_st)

typedef CArray < ESAccntInfoFC_st, ESAccntInfoFC_st > CArrayFCAcct;

typedef	struct _ESHeader_st
{
	char	WinId				[10];	/* Client WINDOWS ID										 	*/
	char	PCKey				[ 1];	/* Request key													*/
	char	ErrCode				[ 4];	/* Error Code
											�����̳� 0000�̸� ����, �׿� ����, _ESErr_st �� ����)		*/
	char	NextKind			[ 1];	/* ���� ���� (0.��������, 1.���� ����)							*/
	char	filler				[15];	/* filler														*/
} ESHeader_st, *lpESHeader_st;
#define SZ_ESHeader sizeof(ESHeader_st)

typedef struct _ESErr_st
{
	ESHeader_st					Head;
	char	ErrMsg				[100];	/* Message														*/
}ESErr_st, * lpESErr_st;
#define	SZ_ESErr_st	sizeof(ESErr_st)

typedef struct _ESAutoKey
{
	ESHeader_st					Head;
	char	AutoKey				[32];	/* ������ ��� ���¹�ȣ, �ü� ��� ����ǥ���ڵ�					*/
} ESAutoKey_st, *lpESAutoKey_st;
#define SZ_ESAutoKey	sizeof(ESAutoKey_st)


//---------------------------------------------------------------------------------------------------------------------------------
//
// �ؿܿ� API ����
//
//---------------------------------------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// ���� ����
typedef	struct	_ESCodeInfo
{
	char	FullCode			[32];	/* ���� Full code												*/
	char	ShortCode			[ 5];	/* ���� Short code												*/
	char	Index				[ 4];	/* ���� �ε���													*/
	char	CodeName			[32];	/* �����														*/
	char	DecimalInfo			[ 5];	/* ��ǰ���ݿ����ѼҼ�������										*/
	char	TickSize			[20];	/* TICK SIZE �Ҽ���7�ڸ�����									*/
	char	TickValue			[20];	/* �ּҰ��ݺ����ݾ� �Ҽ���7�ڸ�����								*/
}	ESCodeInfo_st, * lpESCodeInfo_st;
#define	SZ_ESCodeInfo	sizeof(ESCodeInfo_st)

typedef CArray < ESCodeInfo_st, ESCodeInfo_st >	CArrayCode;

//////////////////////////////////////////////////////////////////////////////////
// ���� ���� ��Ŷ
//////////////////////////////////////////////////////////////////////////////////

// ���� ����
#define ESID_5601				5601	// �ű��ֹ�
#define ESID_5602				5602	// �����ֹ�
#define ESID_5603				5603	// ����ֹ�
#define ESID_5605				5605	// �ڱݰ����ֹ�(���, ����, ����, ����)
#define ESID_5606				5606	// �ڱݰ�����ȸ
#define ESID_5611				5611	// ��ü��
#define ESID_5612				5612	// ���ܰ�
#define ESID_5614				5614	// ���º� �ֹ�ü����Ȳ(USD)
#define ESID_5615				5615	// ��Ź�ڻ�� ���ű�
#define ESID_5633				5633	// �ֹ� ���ɼ��� ��ȸ
#define ESID_5634				5634	// �Ⱓ�������ڻ꺯������
#define ESID_5715				5715	// ��Ź�ڻ�� ���ű�(��ȭ)
// Sure-net
#define ESID_5801				5801	// Sure-net ���ڴ�ȸ ������ȸ
#define ESID_5802				5802	// Sure-net ���ڴ�ȸ �Ÿų���
// �ü� ����
#define ESID_5501				5501	// ���� Master
#define ESID_5502				5502	// ���� �ü�
#define ESID_5503				5503	// ���� ȣ��
#define ESID_5511				5511	// ���� ü�᳻��
#define	ESID_5522				5522	// ���� CHART DATA
#define ESID_5560				5560	// �ֱٿ��� ��� ��ȸ
// �ڵ�������Ʈ
#define AUTO_0930				 930	//shjang 20160406 >>  �ڵ� Update : ��ް��� <<
#define AUTO_0931				 931	// ���� Master �ǽð�
#define AUTO_0932				 932	// ���� ȣ�� �ǽð�
#define AUTO_0933				 933	// ���� �ü� �ǽð�
#define AUTO_0985				 985	// �ܰ�/ü�� �ǽð�





//////////////////////////////////////////////////////////////////////////////////
// �ֹ� ���� (�ű�, ����, ���)
typedef struct _Es5601I
{
	ESHeader_st					Head;
	char	Account				[11];	/* ���¹�ȣ														*/
	char	Pass				[ 8];	/* ��й�ȣ														*/
	char	Order_Type			[ 1];	/* �ֹ�����	(1.�ű��ֹ� 2.�����ֹ� 3.����ֹ�)					*/
	char	ShortCode			[32];	/* ��������ڵ�													*/
	char	BuySell_Type		[ 1];	/* �ŵ��ż����� (1.�ż� 2.�ŵ�)									*/
	char    Price_Type			[ 1];	/* ��������	(1.������ 2.���尡)									*/
	char	Trace_Type			[ 1];	/* ü������ (���尡�ϰ��(3) �������ϰ��(1))					*/
	char    Order_Price			[20];	/* �ֹ�����														*/
	char    Order_Volume		[ 5];	/* �ֹ�����														*/
	char	Order_Org_No		[ 5];	/* ���ֹ���ȣ (����/��ҽø�)									*/
	char    Order_Comm_Type		[ 1];	/* ����ֹ�����													*/
	char	Stop_Type			[ 1];	/* �ֹ��������� (1.�Ϲ��ֹ� 2.STOP�ֹ�)							*/
	char	Stop_Price			[20];	/* STOP�ֹ����� (STOP�ֹ��̸� �Է� �ƴϸ� 0 ����)				*/
	char	Oper_Type			[ 1];	/* �ֹ�����	(1.�ű��ֹ� 2.�����ֹ� 3.����ֹ�)					*/
}	Es5601I_st, *lpEs5601I_st;
#define	SZ_Es5601I	sizeof(Es5601I_st)
	
typedef struct _Es5601O
{
	ESHeader_st					Head;
	char	Order_No  			[ 5];	/* �ֹ���ȣ														*/
	char    Order_Org_No		[ 5];	/* ���ֹ���ȣ													*/
	char	Account				[11];	/* ���¹�ȣ														*/
	char	Order_Type			[ 1];	/* �ֹ�����	(1.�ű��ֹ� 2.�����ֹ� 3.����ֹ�)					*/
	char	ShortCode			[32];	/* ��������ڵ�														*/
	char	BuySell_Type		[ 1];	/* �ŵ��ż����� (1.�ż� 2.�ŵ�)									*/
	char    Order_Volume		[ 5];	/* �ֹ�����														*/
	char    Order_Price			[20];	/* �ֹ�����														*/
	char    Price_Type			[ 1];	/* ��������	(1.������ 2.���尡)									*/
	char	Trade_Type			[ 1];	/* ü������ (���尡�ϰ��(3) �������ϰ��(1))					*/
	char	Stop_Price			[20];	/* STOP�ֹ�����													*/
}	Es5601O_st,	* lpEs5601O_st;
#define	SZ_Es5601O	sizeof(Es5601O_st)

//////////////////////////////////////////////////////////////////////////////////
// �ڱݰ����ֹ�(���, ����, ����, ����)
typedef struct _Es5605I
{
	ESHeader_st					Head;
	char    Crc_tp	            [ 1];   /* 1.�޷� 2.��ȭ				*/
	char    Proc_tp				[ 1];	/* 1.��� 2.���� 3.���� 4.���� 5.����û�� 6.�ս�û�� ���ֹ�	*/
	char	User_id				[ 8];	/* �����ID				*/
	char	Account				[11];	/* ���¹�ȣ 			*/
	char	Pass				[ 8];	/* ��й�ȣ				*/	
	char	Take_profit_amt		[20];	/* ���ͽ��� ��Ź�ܾ�			*/
	char	Stop_loss_amt		[20];	/* �ս����� ��Ź�ܾ�			*/
	char	Ssm_rsrb_dt			[ 8];	/* û���������					*/
	char	Ssm_rsrb_tm			[ 6];	/* û�����ð�					*/
	char	Auto_dt_tp			[ 1];	/* �ڵ����౸�� 1.�ڵ� 2.����	*/
	char	Ord_rest_tp			[ 1];	/* �ű��ֹ����� 1.���� 2.������	*/
}	Es5605I_st, *lpEs5605I_st;
#define	SZ_Es5605I	sizeof(Es5605I_st)

typedef struct _Es5605O
{
	ESHeader_st					Head;
	char    Crc_tp	            [ 1];   /* 1.�޷� 2.��ȭ				*/
	char    Proc_tp				[ 1];	/* 1.��� 2.���� 3.���� 4.���� 5.����û�� 6.�ս�û�� ���ֹ�	*/
	char	User_id				[ 8];	/* �����ID				*/
	char	Account				[11];	/* ���¹�ȣ 			*/
	char	Pass				[ 8];	/* ��й�ȣ				*/
	char	Take_profit_amt		[20];	/* ���ͽ��� ��Ź�ܾ�			*/
	char	Stop_loss_amt		[20];	/* �ս����� ��Ź�ܾ�			*/
	char	Ssm_rsrb_dt			[ 8];	/* û���������					*/
	char	Ssm_rsrb_tm			[ 6];	/* û�����ð�					*/
}	Es5605O_st,	* lpEs5605O_st;
#define	SZ_Es5605O	sizeof(Es5605O_st)

//////////////////////////////////////////////////////////////////////////////////
// �ڱݰ�����ȸ
//
typedef struct _Es5606I
{
	ESHeader_st					Head;
	char    Crc_tp	            [ 1];   /* 1.�޷� 2.��ȭ				*/
	char	User_id				[ 8];	/* �����ID				*/
	char	Account				[11];	/* ���¹�ȣ 			*/
	char	Pass				[ 8];	/* ��й�ȣ				*/	
	char	Proc_tp				[ 1];   /* ��ȸ���� 1.��ü 2.��������	*/
}	Es5606I_st,	* lpEs5606I_st;
#define	SZ_Es5606I	sizeof(Es5606I_st)

typedef struct _Es5606Sub
{
	char	Account				[11];	/* ���¹�ȣ 			*/
	char	Acct_nm				[40];	/* ���¸�				*/
	char	Ssm_stat_tp			[ 1];	/* SSM �ֹ�����			*/
	/* 0.����(���) 1.���� 2.���ͽ��� 3.�ս����� 4.����û�� 5.�ð�û��*/
	char	Profit				[20];	/* ���ͽ��� ��Ź�ܾ�	*/
	char	Loss				[20];	/* �ս����� ��Ź�ܾ�	*/
	char	Ssm_rsrb_dt			[ 8];	/* û���������			*/
	char	Ssm_rsrb_tm			[ 6];	/* û�����ð�			*/
	char    Open_qt             [10];   /* �ܰ���� 			*/
	char    Open_pl             [20];   /* �򰡼��� 			*/
	char	Te_amt				[20];	/* �򰡿�Ź�Ѿ�			*/
	char	Auto_dt_tp			[ 1];	/* �ڵ����౸�� 1.�ڵ� 2.����	*/
	char	Ord_rest_tp			[ 1];	/* �ű��ֹ����� 1.���� 2.������	*/
}	Es5606Sub_st, *lpEs5606Sub_st;

typedef struct _Es5606O
{
	ESHeader_st					Head;
	char	Renu				[ 5];	/* �ݺ�Ƚ�� 					*/
	char    Crc_tp	            [ 1];   /* 1.�޷� 2.��ȭ				*/
	char	User_id				[ 8];	/* �����ID				*/
	char	Account				[11];	/* ���¹�ȣ 			*/
	char	Pass				[ 8];	/* ��й�ȣ				*/
	char	Take_profit_amt		[20];	/* ���ͽ��� ��Ź�ܾ�	*/
	char	Stop_loss_amt		[20];	/* �ս����� ��Ź�ܾ�	*/
	char	Ssm_rsrb_dt			[ 8];	/* û���������			*/
	char	Ssm_rsrb_tm			[ 6];	/* û�����ð�			*/
	char    Open_qt             [10];   /* �ܰ���� 			*/
	char    Open_pl             [20];   /* �򰡼��� 			*/
	char	Te_amt				[20];	/* �򰡿�Ź�Ѿ�			*/
	char	Dtno				[ 5];	/* �ݺ�Ƚ�� 					*/
	Es5606Sub_st		Sub		[20];
}	Es5606O_st,	*lpEs5606O_st;
#define	SZ_Es5606O	sizeof(Es5606O_st)

//////////////////////////////////////////////////////////////////////////////////
// ���� ��ü�� ��ȸ
typedef struct _Es5611I
{
	ESHeader_st					Head;
	char	Account				[11];	/* ���¹�ȣ														*/
	char	Pass				[ 8];	/* ��й�ȣ														*/
	char	Trd_gb				[ 1];	/* ü�ᱸ�� (0:��ü 1:ü��2:��ü��)								*/
	char	Base_dt				[ 8];	/* �ֹ�����														*/
	char	Gubn				[ 1];	/* ��ȸ���� (1:���� 2:����)										*/
}	Es5611I_st,	* lpEs5611I_st;
#define	SZ_Es5611I	sizeof(Es5611I_st)

typedef struct _Es5611Sub
{
	char	Ord_No				[ 5];	/* �ֹ��ֹ�ȣ													*/
	char	Org_ord_No			[ 5];	/* ���ֹ��ι�ȣ													*/
	char	Trd_cond			[ 1];	/* ü������ (1.FAS 2.FOK 3.FAK)									*/
	char	ShortCode			[32];	/* ��������ڵ�													*/
	char	Bysl_tp				[ 1];	/* �Ÿű����ڵ�
											1.�ż� 2.�ŵ� 3.�ż�����
											4.�ŵ����� 5.�ż���� 6.�ŵ���� ' '.��Ÿ					*/
	char	Prce_tp				[ 1];	/* ��������	(1.������ 2.���尡)									*/
	char	Ord_q				[ 5];	/* �ֹ�����														*/
	char	Ord_p				[20];	/* �ֹ����� or ü������											*/
	char	Trd_q				[ 5];	/* ü�����														*/
	char	Mcg_q				[ 5];	/* ��ü�����													*/
	char	Ord_tp				[ 1];	/* �ֹ����� (1.�ű� 2.���� 3.���)								*/
	char	Stop_p				[20];	/* STOP�ֹ�����													*/
	char	Ex_ord_tm			[ 6];	/* �ֹ��ð�														*/
	char	Proc_stat			[ 1];	/* �ֹ�ó������ (0.�������� 1.�ŷ������� 2.�����ź� 3.FEP�ź�)	*/
	char	Account				[11];	/* ���¹�ȣ														*/
}	Es5611Sub_st, *lpEs5611Sub_st;

typedef struct _Es5611O
{
	ESHeader_st					Head;
	char	Renu				[ 5];	/* �ݺ�Ƚ��														*/
	char	Account				[11];	/* ���¹�ȣ														*/
	char	AcctNm				[20];	/* ���¸�														*/
	char	Dtno				[ 5];	/* �ݺ�Ƚ��														*/
	Es5611Sub_st Sub			[12];	/* ü�� ����Ÿ													*/
}	Es5611O_st,	*lpEs5611O_st;
#define	SZ_Es5611O	sizeof(Es5611O_st)


//////////////////////////////////////////////////////////////////////////////////
// ���� ���ܰ� ��ȸ
typedef struct _Es5612I
{
	ESHeader_st					Head;
	char	Account				[11];	/* ���¹�ȣ														*/
	char	Pass				[ 8];	/* ��й�ȣ														*/
}	Es5612I_st,	* lpEs5612I_st;
#define	SZ_Es5612I	sizeof(Es5612I_st)

typedef struct _Es5612Sub
{
	char	Base_dt				[ 8];	/* ��������														*/
	char	FullCode			[32];	/* ����ǥ���ڵ�													*/
	char	Bysl_tp				[ 1];	/* �Ÿű���	(1.�ż� 2.�ŵ�)										*/
	char	Trd_no				[ 5];	/* ü���ȣ														*/
	char	Open_q				[10];	/* �̰�������													*/
	char	Avgt_p				[20];	/* ��հ�														*/
	char	Curr_p				[20];	/* ���簡 														*/
	char	Open_pl				[20];	/* �򰡼���														*/
	char	Rsrb_q				[10];	/* û�갡�ɼ���													*/
	char	Trd_amt				[20];	/* ü��ݾ�														*/
	char	Account				[11];	/* ���¹�ȣ														*/
}	Es5612Sub_st, *lpEs5612Sub_st;

typedef struct _Es5612O
{
	ESHeader_st					Head;
	char	Renu				[ 5];	/* �ݺ�Ƚ��														*/
	char	Account				[11];	/* ���¹�ȣ														*/
	char	AcctNm				[20];	/* ���¸�														*/
	char	Dtno				[ 5];	/* �ݺ�Ƚ��														*/
	Es5612Sub_st	Sub			[16];
}	Es5612O_st,	*lpEs5612O_st;
#define	SZ_Es5612O	sizeof(Es5612O_st)


//////////////////////////////////////////////////////////////////////////////////
// (�ؿܼ���)���º� �ֹ�ü����Ȳ
//
typedef struct _Es5614I
{
	ESHeader_st					Head;
	char	Account				[11];	/* ���¹�ȣ														*/
	char	Pass				[ 8];	/* ��й�ȣ														*/
	char	Trd_gb				[ 1];	/* ü�ᱸ��
											0.��ü 1.ü�� 2.��ü�� 3.�������� 4.������� 5.�ֹ��ź�
											6.ü��+��ü��												*/
	char	Base_dt				[ 8];	/* �ֹ�����														*/
	char	Gubn				[ 1];	/* ��ȸ���� (1.���� 2.����)										*/
	char	Ord_no				[ 5];	/* �����ֹ���ȣ													*/
	char	Dtno				[ 5];	/* ��û����														*/
}	Es5614I_st,	* lpEs5614I_st;
#define	SZ_Es5614I	sizeof(Es5614I_st)

typedef struct _Es5614Sub
{
	char	Ord_no				[ 5];	/* �ֹ��ֹ�ȣ													*/
	char	Orig_ord_no			[ 5];	/* ���ֹ��ι�ȣ													*/
	char	Trd_cond			[ 1];	/* ü������	(1.FAS 2.FOK 3.FAK)									*/
	char	Series				[32];	/* �����ȣ														*/
	char	Bysl_tp				[ 1];	/* �Ÿű����ڵ�
											1.�ż� 2.�ŵ� 3.�ż����� 4.�ŵ����� 5.�ż���� 
											6.�ŵ���� ' '.��Ÿ											*/
	char	Prce_tp				[ 1];	/* �������� (1.������ 2.���尡)									*/
	char	Ord_q				[ 5];	/* �ֹ�����														*/
	char	Ord_p				[20];	/* �ֹ�����														*/
	char	Trd_q				[ 5];	/* ü����� ��													*/
	char	Trd_p				[20];	/* ü�ᰡ��	��հ�												*/
										/* ü������� ü�ᰡ�� SPACE									*/
	char	Trd_no				[ 5];	/* ü���ȣ	SPACE												*/
	char	Ex_trd_tm			[ 6];	/* ü��ð�	SPACE												*/
	char	Mcg_q				[ 5];	/* ��ü�����													*/
	char	Ord_tp				[ 1];	/* �ֹ����� (1.�ű��ֹ� 2.�����ֹ� 3.����ֹ�)					*/
	char	Stop_p				[20];	/* STOP�ֹ�����													*/
	char	Ex_ord_tm			[ 6];	/* �ֹ��ð�														*/
	char	Proc_stat			[ 1];	/* �ֹ�ó������	
											'0'.������ '1'.��������
											'2'.�����ź� '3'.FEP�����ź�								*/
	char	Trd_cnt				[ 1];	/* ü��Ǽ� (1.�ѹ� 2.������)									*/
	char	Trd_gb				[ 1];	/* ü�ᱸ�� 
											1.ü�� 2.��ü�� 3.�������� 4.������� 5.�ֹ��ź�
											6.ü��+��ü��												*/
	char	Account				[11];	/* ���¹�ȣ														*/
}	Es5614Sub_st, *lpEs5614Sub_st;

typedef struct _Es5614O
{
	ESHeader_st					Head;
	char	Renu				[ 5];	/* �ݺ�Ƚ��														*/
	char	Account				[11];	/* ���¹�ȣ														*/
	char	Acct_nm				[20];	/* ���¸�														*/
	char	Dtno				[ 5];	/* �ݺ�Ƚ��														*/
	Es5614Sub_st		Sub		[23];
}	Es5614O_st,	*lpEs5614O_st;
#define	SZ_Es5614O	sizeof(Es5614O_st)


//////////////////////////////////////////////////////////////////////////////////
// ��Ź�ڻ�� ���ű�
typedef struct _Es5615I
{
	ESHeader_st					Head;
	char	Account				[11];	/* ���¹�ȣ														*/
	char	Pass				[ 8];	/* ��й�ȣ														*/
	char	Crc_cd				[ 3];	/* ��ȭ�ڵ�	(USD �� �⺻���� ���)								*/
}	Es5615I_st,	* lpEs5615I_st;
#define	SZ_Es5615I	sizeof(Es5615I_st)

typedef struct _Es5615O
{
	ESHeader_st					Head;
	char	Renu				[ 5];	/* �ݺ�Ƚ�� 													*/
	char	AcctNm				[20];	/* ���¸�														*/
	char	Entr_ch				[20];	/* ��Ź���ܾ�													*/
	char	tdy_repl_amt		[20];	/* ��ȭ���ݾ�(�߰�)											*/
	char	repl_use_amt		[20];	/* ��ȭ�����ݾ�(�߰�)										*/
	char	Fut_rsrb_pl			[20];	/* û�����														*/
	char	Pure_ote_amt		[20];	/* �򰡼���														*/
	char	Fut_trad_fee		[20];	/* ������														*/
	char	Dfr_amt				[20];	/* �̼��ݾ�														*/
	char	Te_amt				[20];	/* ��Ź�ڻ��򰡾�												*/
	char	Open_pos_mgn		[20];	/* �̰������ű�													*/
	char	Ord_mgn				[20];	/* �ֹ����ű�													*/
	char	Trst_mgn			[20];	/* ��Ź���ű�													*/
	char	Mnt_mgn				[20];	/* �������ű�													*/
	char	With_psbl_amt		[20];	/* ���Ⱑ�ɱݾ�													*/
	char	krw_with_psbl_amt	[20];	/* ��ȭ���Ⱑ�ɱݾ�												*/
	char	Add_mgn				[20];	/* �߰����ű�													*/
	char	Ord_psbl_amt		[20];	/* �ֹ����ɱݾ�													*/
	char	Han_psbl_amt		[20];	/* ȯ�����ݾ�													*/
	char	Crc_cd				[ 3];	/* ��ȭ�ڵ�														*/
}	Es5615O_st,	* lpEs5615O_st;
#define	SZ_Es5615O	sizeof(Es5615O_st)

//////////////////////////////////////////////////////////////////////////////////
//shjang 20160408 >> ��Ź�ڻ�� ���ű�(��ȭ) <<
typedef struct _Es5715I
{
	ESHeader_st					Head;
	char	Accnt				[11];	/* ���¹�ȣ	*/
	char	Acct_pwd			[ 8];	/* ��й�ȣ */
	char	Crc_cd				[ 3];	/* ��ȭ�ڵ�	*/

}	Es5715I_st,	* lpEs5715I_st;
#define	SZ_Es5715I	sizeof(Es5715I_st)

typedef struct _Es5715O
{
	ESHeader_st					Head;
	char	Renu				[ 5];	/* �ݺ�Ƚ�� 	*/
	char	Acct_nm				[20];	/* ���¸�		*/
	char	Entr_ch				[20];	/* ��Ź���ܾ�	*/
	char	Fut_rsrb_pl			[20];	/* û�����		*/
	char	Pure_ote_amt		[20];	/* �򰡼���		*/
	char	Fut_trad_fee		[20];	/* ������		*/
	char	Dfr_amt				[20];	/* �̼��ݾ�		*/
	char	Te_amt				[20];	/* ��Ź�ڻ��򰡾�	*/
	char	Open_pos_mgn		[20];	/* �̰������ű�	*/
	char	Ord_mgn				[20];	/* �ֹ����ű�	*/
	char	Trst_mgn			[20];	/* ��Ź���ű�	*/
	char	Mnt_mgn				[20];	/* �������ű�	*/
	char	With_psbl_amt		[20];	/* ���Ⱑ�ɱݾ�	*/
	char	Add_mgn				[20];	/* �߰����ű�	*/
	char	Ord_psbl_amt		[20];	/* �ֹ����ɱݾ�	*/
}	Es5715O_st,	* lpEs5715O_st;
#define	SZ_Es5715O	sizeof(Es5715O_st)
//////////////////////////////////////////////////////////////////////////////////
// �ֹ�/û�갡�ɼ���
typedef struct _Es5633I
{
	ESHeader_st					Head;
	char	Account				[11];	/* ���¹�ȣ														*/
	char	Pass				[ 8];	/* ��й�ȣ														*/
	char	ShortCode			[32];	/* �����ڵ�														*/
	char	Bysl_tp				[ 1];	/* �ż�/�ŵ����� (1.�ż� 2.�ŵ�)								*/
}	Es5633I_st,	* lpEs5633I_st;
#define	SZ_Es5633I	sizeof(Es5633I_st)

typedef struct _Es5633O
{
	ESHeader_st					Head;
	char	Renu				[ 5];	/* �ݺ�Ƚ��														*/
	char	Account				[11];	/* ���¹�ȣ														*/
	char	Filler				[ 9];	
	char	Ord_q				[ 5];	/* ���ɼ���														*/
	char	Chu_q				[ 5];	/* û�����														*/
}	Es5633O_st,	* lpEs5633O_st;
#define	SZ_Es5633O	sizeof(Es5633O_st)

//////////////////////////////////////////////////////////////////////////////////
// �Ⱓ�������ڻ꺯������ (2017/02/24 �߰�)
typedef struct _Es5634I
{
	ESHeader_st					Head;
	char	Accnt				[11];	/* ���¹�ȣ	*/
	char	Acct_pwd			[ 8];	/* ��й�ȣ */
	char    crc_cd              [ 3];   /* ��ȭ�ڵ�     */
	char	FBase_dt			[ 8];	/* ��������	*/
	char	TBase_dt			[ 8];	/* ��������	*/
	char	Dtno				[ 5];	/* ��û����	*/
	char	Etx					[ 1];	/* ETX(0x03) 	*/
}	Es5634I_st,	* lpEs5634I_st;
#define	SZ_Es5634I	sizeof(Es5634I_st)

typedef struct _Es5634Sub
{
	char	Base_dt				[ 8];	/* �ŷ�����	*/
	char	Book_no				[ 5];	/* �����ȣ	*/
	char	Rmrk_nm				[20];	/* �����	*/
	char	Crc_cd				[ 3];	/* ��ȭ�ڵ�	*/
	char	Accnt				[11];	/* ���¹�ȣ	*/
	char	Pdy_entr_ch			[20];	/* ��Ź�������ܾ�	*/
	char	I_amt	 			[20];	/* �Աݾ�(���������� ��� ����ݾ�)	*/
	char	O_amt	 			[20];	/* ��ݾ�	*/
	char	Stl_amt				[20];	/* ���ݼ�����(���������� ��� �����ݾ�)	*/
	char	Fee_amt				[20];	/* ������	*/
	char    dfr_ocr_amt         [20];   /* �̼��߻��ݾ�                     */
	char    dfr_rpym_amt        [20];   /* �̼������ݾ�                     */
	char    arr_clt_amt         [20];   /* ��ü��¡���ݾ�                   */
	char	Entr_ch				[20];	/* ��Ź���ݴ��ܾ�	*/
	char	Series				[32];	/* �����ڵ�	*/
}	Es5634Sub_st, *lpEs5634Sub_st;

typedef struct _Es5634O
{
	ESHeader_st					Head;
	char	Renu				[ 5];	/* �ݺ�Ƚ�� */
	char	Accnt				[11];	/* ���¹�ȣ	*/
	char	Acct_nm				[20];	/* ���¸�	*/
	char	TPdy_entr_ch		[20];	/* ��Ź�������ܾ�	*/
	char	TI_amt	 			[20];	/* ���Աݾ�	*/
	char	TO_amt	 			[20];	/* ����ݾ�	*/
	char	TStl_amt			[20];	/* �����ݼ�����	*/
	char	TFee_amt			[20];	/* �Ѽ�����	*/
	char    tot_dfr_ocr_amt     [20];   /* �ѹ̼��߻��ݾ�   */
	char    tot_dfr_rpym_amt    [20];   /* �ѹ̼������ݾ�   */
	char    tot_arr_clt_amt     [20];   /* �ѿ�ü��¡���ݾ� */
	char	TEntr_ch			[20];	/* ��Ź���ݴ��ܾ�	*/
	char	Dtno				[ 5];	/* �ݺ�Ƚ��	*/
	Es5634Sub_st		Sub		[10];
}	Es5634O_st,	*lpEs5634O_st;
#define	SZ_Es5634O	sizeof(Es5634O_st)
//////////////////////////////////////////////////////////////////////////////////
// �ü�
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// ���� Master
typedef struct _Es5501I
{
	ESHeader_st					Head;
	char	FullCode			[32];	/* ����ǥ���ڵ�													*/
	char	Index				[ 4];	/* ���� �ε���													*/
}	Es5501I_st, * lpEs5501I_st,
	Es5502I_st, * lpEs5502I_st,
	Es5503I_st, * lpEs5503I_st;
#define	SZ_Es5501I	sizeof( Es5501I_st)
#define	SZ_Es5502I	sizeof( Es5502I_st)
#define	SZ_Es5503I	sizeof( Es5503I_st)

//////////////////////////////////////////////////////////////////////////////////
// ���� Master
typedef	struct	_Es5501O
{
	ESHeader_st					Head;
	char	FullCode			[32];	/* ����ǥ���ڵ�													*/
	char	JinbubGb			[ 1];	/* ���� (0.10���� 1.32���� 2.64���� 3:128���� 4.8����)			*/
	char	StandeCode			[12];	/* ����ǥ���ڵ�													*/
	char	HLimitPrice			[20];	/* ���Ѱ�														*/
	char	LLimitPrice			[20];	/* ���Ѱ�														*/
	char	RemainDays			[ 5];	/* �����ϼ�9(3)													*/
	char	LastDate			[ 8];	/* �����ŷ���YYYYMMDD											*/
	char	ExpiryDate			[ 8];	/* ������    YYYYMMDD											*/
	char	ListedHighPrice		[20];	/* �������ְ�													*/
	char	ListedHighPriceDate	[ 8];	/* �������ְ���YYYYMMDD											*/
	char	ListedLowPrice		[20];	/* ������������													*/
	char	ListedLowPriceDate	[ 8];	/* ������������YYYYMMDD											*/
	char	ClosePrice_1		[20];	/* �����갡														*/
	char	ClosePrice_2		[20];	/* ��������														*/
	char	Scn_Exch_cd			[ 5];	/* �ŷ����ڵ�													*/
	char	Scn_Disp_digit 		[10];	/* ǥ������														*/
	char	Scn_Fut_ord_mgn		[20];	/* �����ű��ֹ����ű�											*/
	char	Scn_Ctrt_size   	[20];	/* ������														*/
	char	Scn_Tick_size    	[20];	/* ƽ															*/
	char	Scn_Tick_val    	[20];	/* ƽ��ġ														*/
	char	PrevTotQty			[20];	/* ���� �ŷ���													*/
}	Es5501O_st, * lpEs5501O_st;
#define	SZ_Es5501O	sizeof( Es5501O_st)

//////////////////////////////////////////////////////////////////////////////////
// ���� �ü�
typedef	struct	_Es5502O
{
	ESHeader_st					Head;
	char	FullCode  			[32];	/* ����ǥ���ڵ�													*/
	char	JinbubGb  			[ 1];	/* ���� (0.10���� 1.32���� 2.64���� 3.128���� 4.8����)			*/
	char	ISINCd   			[12];	/* ����ǥ���ڵ�													*/
	char	ClosePrice			[20];	/* ���簡														*/
	char	CmpSign				[ 1];	/* ����ȣ
											0.���� 1.���� 2.���� 3.��� 4.�϶� 5.�⼼����
											6.�⼼���� 7.�⼼��� 8.�⼼�϶�							*/
	char	CmpPrice			[20];	/* ���ϴ��9(5)V9(2)											*/
	char	CmpRate				[ 8];	/* �����9(5)V9(2) 												*/
	char	OpenPrice			[20];	/* �ð� 														*/
	char	HighPrice			[20];	/* �� 														*/
	char	LowPrice			[20];	/* ���� 														*/
	char	ContQty				[20];	/* ü�ᷮ 														*/
	char	TotQty 				[20];	/* �ŷ��� 														*/
	char	ClosePrice_1		[20];	/* Close Price 1												*/
}	Es5502O_st, * lpEs5502O_st;
#define	SZ_Es5502O	sizeof( Es5502O_st)

//////////////////////////////////////////////////////////////////////////////////
// ���� ȣ��
typedef	struct	_Es5503O
{
	ESHeader_st					Head;
	char	FullCode  			[32];	/* ����ǥ���ڵ�													*/
	char	JinbubGb  			[ 1];	/* ���� (0.10���� 1.32���� 2.64���� 3.128���� 4.8����)			*/
	char	ISINCd   			[12];	/* ����ǥ���ڵ�													*/
	char	Time				[ 8];	/* �Է½ð� HH:MM:SS											*/
	char	ClosePrice_1		[20];	/* Close Price 1												*/

	struct	_Es5503OSub
	{
		char	BuyNo			[10];	/* �ż���ȣ														*/
		char	BuyPrice		[20];	/* �ż�ȣ��														*/
		char	BuyQty			[20];	/* �ż�����														*/
		char	SellNo			[10];	/* �ŵ���ȣ														*/
		char	SellPrice		[20];	/* �ŵ�ȣ��														*/
		char	SellQty			[20];	/* �ŵ�����														*/
	}	Ary	[ 5];

	char	TotSellQty			[20];	/* �ŵ���ȣ������9(6)											*/
	char	TotBuyQty			[20];	/* �ż���ȣ������9(6)											*/
	char	TotSellNo			[20];	/* �ŵ���ȣ���Ǽ�9(5)											*/
	char	TotBuyNo			[20];	/* �ż���ȣ���Ǽ�9(5)											*/
}	Es5503O_st, * lpEs5503O_st;
#define	SZ_Es5503O	sizeof( Es5503O_st)


//////////////////////////////////////////////////////////////////////////////////
// ���� ü�᳻��
typedef struct _Es5511I
{
	ESHeader_st					Head;
	char	FullCode			[32];	/* ����ǥ���ڵ�													*/
	char	Index				[ 4];	/* �����ڵ� index												*/
} Es5511I_st, * lpEs5511I_st;
#define	SZ_Es5511I	sizeof( Es5511I_st)

typedef	struct	_Es5511OSub
{
	char	Time				[ 8];	/* �ð�(HH:MM:SS)												*/
	char	CmpSign				[ 1];	/* ����ȣ
											0.���� 1.���� 2.���� 3.��� 4.�϶� 5.�⼼����
											6.�⼼���� 7.�⼼��� 8.�⼼�϶�							*/
	char	CmpPrice			[20];	/* ���ϴ��														*/
	char	ClosePrice			[20];	/* ���簡														*/
	char	CmpRate				[ 8];	/* �����														*/
	char	TotQty 				[20];	/* �ŷ���														*/
	char	ContQty				[20];	/* ü�ᷮ														*/
	char	ClosePrice1			[20];	/* Close Price 1												*/
	char	MatchKind			[ 1];	/* ���簡�� ȣ������ (+.�ż� -.�ŵ�)							*/
	char	Date				[ 8];	/* ����(YYYYMMDD)												*/
}	Es5511OSub_st, * lpEs5511OSub_st;

typedef	struct	_Es5511O
{
	ESHeader_st					Head;
	char	FullCode			[32];	/* ����ǥ���ڵ�													*/
	char	JinbubGb  			[ 1];	/* ���� 0:10���� 1:32����										*/
	char	DataCnt				[ 2];	/* Data Count													*/
	Es5511OSub_st		Ary		[20];
}	Es5511O_st, * lpEs5511O_st;
#define	SZ_Es5511O	sizeof( Es5511O_st)


//////////////////////////////////////////////////////////////////////////////////
// ���� CHART DATA
typedef struct _Es5522I
{
	ESHeader_st					Head;
	char	JongUp				[ 1];	/* '5'�� ����													*/
	char	DataGb				[ 1];	/* 1:��															*/
										/* 2:��															*/
										/* 3:��															*/
										/* 4:tick��														*/
										/* 5:�к�														*/
	char	DiviGb				[ 1];	/* ����, �Ϻ��� �׸���� ���� ���Ѵ�: '0'���� ����				*/
	char	FullCode			[32];	/* ����ǥ���ڵ�													*/
	char	Index				[ 4];	/* �ڵ� index													*/
	char	InxDay				[ 8];	/* ��������														*/
	char	DayCnt				[ 4];	/* ���ڰ���														*/
	char	Summary				[ 3];	/* tick, �п��� ������ ����										*/
	char	Linked				[ 1];	/* ���ἱ������ Y/N(�Ϻ�)										*/
	char	JunBonGb			[ 1];	/* 1.������ 2.���� (��/ƽ��)									*/
} Es5522I_st, * lpEs5522I_st;
#define	SZ_Es5522I	sizeof( Es5522I_st)

typedef struct  _Es5522OSub
{
    char    Date				[ 8];   /* YYYYMMDD														*/
    char    Time				[ 8];   /* �ð�(HH:MM:SS)												*/
    char    OpenPrice			[20];   /* �ð� double													*/
    char    HighPrice			[20];   /* �� double													*/
    char    LowPrice			[20];   /* ���� double													*/
    char    ClosePrice			[20];   /* ���� double													*/
    char    Volume				[20];   /* �ŷ��� double												*/
}   Es5522OSub_st, * lpEs5522OSub_st;
#define SZ_Es5522OSub   sizeof( Es5522OSub_st)

typedef struct  _Es5522O
{
	ESHeader_st					Head;
	char    FullCode			[32];   /* ����ǥ���ڵ�													*/
	char    MaxDataCnt			[ 4];   /* ��ü���� ����												*/
	char    DataCnt				[ 3];   /* ����۽����� ����											*/
	char    TickCnt				[ 2];   /* ���������� tick ����											*/
	char    Today				[ 8];   /* �翵����(StockDate[0])										*/
	char    nonedata			[ 4];   /* �������� double												*/
	char    DataGb				[ 1];   /* 1:��															*/
								        /* 2:��															*/
								        /* 3:��															*/
								        /* 4:tick��														*/
								        /* 5:�к�														*/
	char    DayCnt				[ 4];   /* ���ڰ���														*/
	char    Summary				[ 3];   /* tick, �п��� ������ ����										*/
	char    PrevLast			[20];   /* ��������														*/
	Es5522OSub_st		Ary	   [120];
}   Es5522O_st, * lpEs5522O_st;
#define SZ_Es5522O  sizeof( Es5522O_st)

/****************************************************************************/
/* Outbound �ֱٿ���	                                                    */
/****************************************************************************/
typedef struct _Es5560I
{
	ESHeader_st		Head;

	char	item_gb			[ 1];	/* ǰ�񱸺�				*/
	/* 0.��ü	1.��ȭ		*/
	/* 2.����	3.������	*/
	/* 4.ä��	5.�ݼ�		*/
	/* 6.��깰				*/
} Es5560I_st, * lpEs5560I_st;
#define	SZ_Es5560I	sizeof(Es5560I_st)

typedef struct _Es5560O
{
	ESHeader_st		Head;

	char	item_gb			[ 1];	/* ǰ�񱸺�				*/
	/* 0.��ü	1.��ȭ		*/
	/* 2.����	3.������	*/
	/* 4.ä��	5.�ݼ�		*/
	/* 6.��깰				*/
	char	DataCnt			[ 2];	/* ���䰹�� */
} Es5560O_st, *lpEs5560O_st;
#define	SZ_Es5560O	sizeof(Es5560O_st)

typedef struct _Es5560OSub
{
	char	StockCd  		[32];	/* �����ڵ�(Full) */
	char    MarketCd		[ 3];   /* �����ڵ� */
} Es5560OSub_st, *lpEs5560OSub_st;
#define	SZ_Es5560OSub	sizeof(Es5560OSub_st)

/*--------------------------------------------------------------------------*/
/* (�ؿܼ���) ����� ���ڴ�ȸ ������ȸ  5801                                */
/*--------------------------------------------------------------------------*/
typedef struct _Es5801I
{
	ESHeader_st	Head;

	char base_dt         [ 8]; /* ��������                           */
	char user_id         [ 8]; /* �����ID : ������, �Ϲ� ���п뵵   */
	char part_nm         [ 1]; /* �� �̸�  A~Z, 0x20:��ü            */
	char commodity_seq   [ 3]; /* CUR:��ȭ IDX:���� ENG:������ BND:ä�� MTL:�ݼ� AGR:��깰  0x20:��ü */
} Es5801I_st, *lpEs5801I_st;
#define	SZ_Es5801I	sizeof(Es5801I_st)

typedef struct _Es5801O
{
	ESHeader_st	Head;

	char    dtno          [  3];   /* �ݺ�Ƚ��     */
	char    Etx           [  1];   /* ETX(0x03)    */
} Es5801O_st, *lpEs5801O_st;
#define	SZ_Es5801O	sizeof(Es5801O_st)

typedef struct _Es5801OSub
{
	char  leader_nm       [ 20];  /*  ���ʴг���                   */
	char  cntr_yn           [  8];  /*  ü���ֹ�����                 */
	char  mrkt_cd          [ 20];  /*  ����                         */
	char  exp_yyyymm    [ 10];  /*  ����                         */
	char  prce               [ 10];  /*  ����                         */
	char  bysl_tp            [  4];  /*  �Ÿű���                     */
	char  qty                 [  3];  /*  ����                         */
	char  win_point        [  3];  /*  ����                         */
	char  part_nm          [  1];  /*  �� �̸�  A~Z                 */
	char  day_modi_pl    [ 10];  /*  ���ϸ�ȯ�����               */
	char  week_modi_pl  [ 10];  /*  �ְ�ȯ�����                 */
	char  day_pl            [ 10];  /*  ���ϸ����� (û�����)        */
	char  week_pl          [ 10];  /*  �ְ�����   (û�����)        */
	char  accum_pl        [ 10];  /*  ȯ�� ��������   (û�����+������) */
	char  accum_pl2      [ 10];  /*  ��������   (û�����+������) */
} Es5801OSub_st, *lpEs5801OSub_st;

/*--------------------------------------------------------------------------*/
/* (�ؿܼ���) ����� ���ڴ�ȸ �Ÿų���  5802                                */
/*--------------------------------------------------------------------------*/
typedef struct _Es5802I_
{
	ESHeader_st	Head;

	char  gubun           [ 1]; /* 1:�Ÿų��� 2:�ܰ���           */
	char  st_dt           [ 8]; /* ���ؽ�������                    */
	char  ed_dt           [ 8]; /* ���ؽ�������                    */
	char  part_nm         [ 1]; /* �� �̸�  A~Z, 0x20:��ü         */
	char  commodity_seq   [ 3]; /* CUR:��ȭ IDX:���� ENG:������ BND:ä�� MTL:�ݼ� AGR:��깰 0x20:��ü */
} Es5802I_st, * lpEs5802I_st;

typedef struct _Es5802O_
{
	ESHeader_st	Head;
	char    dtno          [  3];   /* �ݺ�Ƚ��     */
	char    Etx           [  1];   /* ETX(0x03)    */
} Es5802O_st, *lpEs5802O_st;
#define	SZ_Es5802O	sizeof(Es5802O_st)

typedef struct _Es5802OSub_
{
	char  date            [  10];  /*  ü������                     */
	char  time            [  8];  /*  ü��ð�                     */
	char  leader_nm       [ 30];  /*  ���ʴг���                   */
	char  mrkt_cd         [ 20];  /*  ����                         */
	char  exp_yyyymm      [ 10];  /*  ����                         */
	char  bysl_tp         [  4];  /*  �Ÿű���                     */
	char  qty             [  8];  /*  ����                         */
	char  prce            [ 10];  /*  ����                         */
	char  last_prce       [ 10];  /*  ���簡                       */
	char  open_pl         [ 10];  /*  �򰡼���                     */
} Es5802OSub_st, *lpEs5802OSub_st;

/****************************************************************************/
// shjang 20160406  >> �ڵ� Update : ��ް��� PopUp	<<
/****************************************************************************/
typedef	struct	_At0930O
{
	ESHeader_st				Head;
	char	OrgTrCode		[ 4];	/* �߻� TR CODE */
	char	Time			[ 9];	/* (KEY)�ð� HHMMSSsss */
	char	SeqNo			[10];	/* (KEY)�Ϸù�ȣ */
	char	TotSize			[10];	/* ��ü�������ũ��(CR �� LF ����) */
	/*		����(CR �� LF ����)		*/
	/*		Etx						*/
}	At0930O_st, * lpAt0930O_st;
#define	SZ_At0930O	sizeof( At0930O_st)
//////////////////////////////////////////////////////////////////////////////////
// ���� Master �ڵ� ������Ʈ
typedef	struct	_At0931O
{
	ESHeader_st					Head;
	char	FullCode 			[32];	/* ����ǥ���ڵ�													*/
	char	JinbubGb  			[ 1];	/* ���� (0.10���� 1.32���� 2.64���� 3.128���� 4.8����)			*/
	char	Date				[ 8];	/* ����(YYYYMMDD) 												*/
	char	Time				[ 8];	/* �ð�(HH:MM:SS) 												*/
	char	ClosePrice			[20];	/* ���簡														*/
	char	CmpSign				[ 1];	/* ����ȣ
											0.���� 1.���� 2.���� 3.��� 4.�϶� 5.�⼼����
											6.�⼼���� 7.�⼼��� 8.�⼼�϶�							*/
	char	CmpPrice			[20];	/* ���ϴ��														*/
	char	CmpRate				[ 8];	/* �����9(5)V9(2)												*/
	char	OpenPrice			[20];	/* �ð� 														*/
	char	HighPrice			[20];	/* �� 														*/
	char	LowPrice			[20];	/* ���� 														*/
	char	ContQty				[20];	/* ü�ᷮ 														*/
	char	TotQty 				[20];	/* �ŷ��� 														*/
	char	ClosePrice_1		[20];	/* Close Price 1												*/
	char	ListedHighPrice		[20];	/* �������ְ�													*/
	char	ListedHighPriceDate	[ 8];	/* �������ְ���YYYYMMDD											*/
	char	ListedLowPrice		[20];	/* ������������													*/
	char	ListedLowPriceDate	[ 8];	/* ������������YYYYMMDD											*/
}	At0931O_st, * lpAt0931O_st;
#define	SZ_At0931O	sizeof( At0931O_st)

//////////////////////////////////////////////////////////////////////////////////
// ���� ȣ�� �ڵ� ������Ʈ
typedef	struct	_At0932O
{
	ESHeader_st					Head;
	char	FullCode 			[32];	/* ����ǥ���ڵ�													*/
	char	JinbubGb  			[ 1];	/* ���� (0.10���� 1.32���� 2.64���� 3.128���� 4.8����)			*/
	char	Time				[ 8];	/* �ð�(HH:MM:SS)												*/
	char	ClosePrice_1		[20];	/* Close Price 1												*/

	struct	_At0932O_Arr
	{
		char	BuyNo			[10];	/* �ż���ȣ														*/
		char	BuyPrice		[20];	/* �ż�ȣ��														*/
		char	BuyQty			[20];	/* �ż�����														*/
		char	SellNo			[10];	/* �ŵ���ȣ														*/
		char	SellPrice		[20];	/* �ŵ�ȣ��														*/
		char	SellQty			[20];	/* �ŵ�����														*/
	}	Arr	[ 5];

	char	TotSellQty			[20];	/* �ŵ���ȣ������9(6)											*/
	char	TotBuyQty			[20];	/* �ż���ȣ������9(6)											*/
	char	TotSellNo			[20];	/* �ŵ���ȣ���Ǽ�9(5)											*/
	char	TotBuyNo			[20];	/* �ż���ȣ���Ǽ�9(5)											*/
}	At0932O_st, * lpAt0932O_st;
#define	SZ_At0932O	sizeof( At0932O_st)

//////////////////////////////////////////////////////////////////////////////////
// ���� �ü� �ڵ� ������Ʈ
typedef	struct	_At0933O
{
	ESHeader_st					Head;
	char	FullCode	 		[32];	/* ����ǥ���ڵ� 												*/
	char	JinbubGb  			[ 1];	/* ���� (0.10���� 1.32���� 2.64���� 3.128���� 4.8����)			*/
	char	Time				[ 8];	/* �ð�(HH:MM:SS)												*/
	char	CmpSign				[ 1];	/* ����ȣ
											0.���� 1.���� 2.���� 3.��� 4.�϶� 5.�⼼����
											6.�⼼���� 7.�⼼��� 8.�⼼�϶�							*/
	char	CmpPrice			[20];	/* ���ϴ��														*/
	char	ClosePrice			[20];	/* ���簡														*/
	char	CmpRate				[ 8];	/* �����9(5)V9(2)												*/
	char	TotQty 				[20];	/* �ŷ��� 														*/
	char	ContQty				[20];	/* ü�ᷮ 														*/
	char	MatchKind			[ 1];	/* ���簡�� ȣ������ (+.�ż� -.�ŵ�)							*/
	char	Date				[ 8];	/* ����(YYYYMMDD) 												*/
	char	OpenPrice			[20];	/* �ð� 														*/
	char	HighPrice			[20];	/* �� 														*/
	char	LowPrice			[20];	/* ���� 														*/
	char	BuyPrice			[20];	/* �ż�ȣ��														*/
	char	SellPrice			[20];	/* �ŵ�ȣ��														*/
	char	MarketFlag			[ 1];	/* �屸�� 0.���� 1.������										*/
	char	DecLen				[ 5];	/* ���� �Ҽ��� ����												*/
}	At0933O_st, * lpAt0933O_st;
#define	SZ_At0933O	sizeof( At0933O_st)

//////////////////////////////////////////////////////////////////////////////////
// �ֹ�/ü�� �ڵ� ������Ʈ
typedef	struct	_At0985O
{
	ESHeader_st					Head;
	char	Account				[11]; 	/* ���¹�ȣ														*/
	char	ReplyType			[ 1];	/* �������� 
											1.�ֹ����� 2.ü�� 3.����Ȯ�� 4.���Ȯ�� 
											5.�ű԰ź� 6.�����ź� 7.��Ұź� 0.��������					*/
	char	FullCode			[32];	/* �����ڵ� (���������϶� ǥ���ڵ�, �׿� �����ڵ�)				*/
	char	Side				[ 1];	/* �ż�/�ŵ����� (1.�ż� 2.�ŵ�)								*/
	char	Qty					[20];	/* �ֹ�����														*/
	char	Modality			[ 1];	/* ��������	(1.������ 2.���尡)									*/
	char	Price				[20];	/* �ֹ�����														*/
	char	Validity			[ 1];	/* ü������ (1.FAS 2.FOK 3.FAK 4.GTC 5.GTD)						*/
	char	StopLossLimit		[20];	/* stop order ��������											*/
	char	ExecPrice			[20];	/* ü�ᰡ��														*/
	char	ExecQty				[20];	/* ü�����														*/
	char	RemainQty			[20];	/* �ֹ��ܷ�														*/
	char	Ord_no				[ 5];	/* �ֹ���ȣ														*/
	char	Orig_ord_no			[ 5];	/* ���ֹ���ȣ													*/
	char	TradeTime			[ 8];	/* �ֹ�Ȯ��,ü��,�ź� �ð�										*/
	char	ExecAmt				[20];	/* ü��ݾ�														*/
	char	ORD_TP 				[ 1];	/* �ֹ�����	(ReplyType==0�϶� 1.�ű� 2.���� 3.���)				*/
	char    Trd_no				[ 5];   /* ü���ȣ														*/
	char    Trd_date			[ 8];   /* ü������														*/
	char	Rsrb_q				[10];	/* û�갡�ɼ���													*/
	char	Open_q				[10];	/* �ܰ����														*/
	char	Open_tp				[ 1];	/* �ܰ������Ǳ��� (1.�ż� 2.�ŵ�)								*/
	char    Ordp_q				[10];   /* �ֹ����ɼ���													*/
	char	AvgExecAmt			[20];	/* ü��ݾ�	(�̵����)											*/
	char	FreeCap_Kind		[ 1];	/* 1.OMS 2.RISK													*/
}	At0985O_st, * lpAt0985O_st;
#define	SZ_At0985O	sizeof( At0985O_st)


#endif