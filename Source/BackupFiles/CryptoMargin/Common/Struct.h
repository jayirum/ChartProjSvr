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
	double	gap;// ���ϴ��(�ǹ���ô� ���ذ����)
	double	tick;// ���簡
	double	open;// �ð�
	double	high;// ��
	double	low;// ����
	double	accmlqty;// �����ŷ���
	double	accmlamt;// �����ŷ����
	int		time;// time
	char	side[1]; // side
	char	diffsign[1];//���ϴ���ȣ  (5:�϶� 2:��� )
	char	chgrate[7];// [float ,  6.2] �����
}TICK_DATA;