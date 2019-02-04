#pragma once
#pragma warning( disable : 4786 )
#pragma warning( disable : 4819 )
#pragma warning( disable : 4996 )

#include <Windows.h>

namespace _FBI
{
	const int DEAL_STATUS_NONSTART	= 0;	//	�̽���
	const int DEAL_STATUS_ORD = 1;		//	�ֹ�����
	const int DEAL_STATUS_WAIT = 2;		//	���
	const int DEAL_STATUS_RESULT = 3;	//	�������
	const int DEAL_STATUS_DONE = 4;		//	�Ϸ�

	const int FBILEN_SYMBOL		= 10;
	const int FBILEN_PRC		= 10;
	const int FBILEN_VOLUME		= 10;
	const int FBILEN_BUFFER		= 1024;
	const int FBILEN_USERID		= 20;
	const int FBILEN_USER_NM		= 20;
	const int FBILEN_PACKET_CODE	= 4;
	const int FBILEN_PACKET_SIZE	= 4;
	const int FBILEN_FRONT_NAME		= 10;
	const int FBILEN_RSLT_CODE		= 4;
	const int FBILEN_DEAL_SEQ		= 9;

	const char	STX = 0x02;
	const char	ETX = 0x03;

	const int MEM_BLOCK_SIZE	= 1024;
	const int MEM_PRE_ALLOC		= 100;
	const int MEM_MAX_ALLOC		= 1000;

	
	const int WM_TERMINATE = WM_USER + 513;
	const int WM_RECV_API_CHART = WM_USER + 514;
	const int WM_DEAL_STATUS = WM_USER + 515;
	
	const int RT_SUCCESS = 0;

	const int TIMEOUT_CHECK_DEAL = 100;	// miliseconds


	char* dealstatus(const int status, char* pzStatus);
	char* packlen(int len, char* out);
	char* rsltCode(int code, char* out);
	char* now(char* out);



	struct HEADER
	{
		char len[4];
		char code[4];
		char sysTp[1];
		char rsltCode[4];
		char tm[17];
	};

	// API CHART DATA
	struct ST_API_CHART
	{
		char	STX[1];
		char	Len[4];
		char	Symbol[FBILEN_SYMBOL];
		char	Date[8];
		char	ChartType[1];	// m:minute, h:hour, d:day
		char	TimeFrame[3];	// 001: 1min, 005
		char	ChartTime[5];	// hh:mm
		char	Open[FBILEN_PRC];
		char	High[FBILEN_PRC];
		char	Low[FBILEN_PRC];
		char	Close[FBILEN_PRC];
		char	Volume[FBILEN_VOLUME];
		char	ETX[1];
	};
	// API TICK DATA PACKET
	struct ST_TICK_API
	{
		char	STX[1];
		char	Len[4];	// ��ü ����
		char	Date[8];
		char	Time[8];
		char	ShortCode[7];
		char	Close[10];
		char	CntrQty[10];
		char	DecLen[5];				// �Ҽ��� �ڸ���
		char	ETX[1];
	};

	struct ST_DEAL_INFO
	{
		char	Symbol[FBILEN_SYMBOL];
		int		DealSeq;
		char	SysDt[8];
		char	tm_order[8];	//hh:mm:ss
		char	tm_wait[8];		//hh:mm:ss
		char	tm_result[8];	//hh:mm:ss
		int		DealStatus;
		int		DurationMin;
	};

	struct ST_DEAL_STATUS
	{
		char Symbol[FBILEN_SYMBOL];
		char DealSeq[FBILEN_DEAL_SEQ];
		char DealStatus[2];
		char Time[8];				// �� status ���۽ð�
	};
}