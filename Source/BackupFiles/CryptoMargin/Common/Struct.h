#pragma once
#pragma pack(1)





typedef struct _BOOK_DATA
{
	char	symbol[9];
	double	sellprc[5];
	double	sellqty[5];
	int		sellcnt[5];
	double	selltotqty;
	int		selltotcnt;
	double	buyprc[5];
	double	buyqty[5];
	int		buycnt[5];
	double	buytotqty;
	int		buytotcnt;
	int		time;
}BOOK_DATA;

typedef struct _TICK_DATA
{
	char	symbol[9];
	double	gap;// 전일대비(권배락시는 기준가대비)
	double	tick;// 현재가
	double	open;// 시가
	double	high;// 고가
	double	low;// 저가
	double	accmlqty;// 누적거래량
	double	accmlamt;// 누적거래대금
	int		time;// time
	char	side[1]; // side
	char	diffsign[1];//전일대비기호  (5:하락 2:상승 )
	char	chgrate[7];// [float ,  6.2] 등락율
}TICK_DATA;