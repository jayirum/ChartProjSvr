#include <stdio.h>
#include "../../IRUM_UTIL/Util.h"


enum CHART_TP {
	TP_1MIN = 0,
	TP_3MIN,
	TP_5MIN,
	TP_10MIN,
	TP_15MIN,
	TP_20MIN,
	//TP_30MIN, 
	TP_60MIN,
	TP_120MIN,
	TP_DAY,
	TP_WEEK,
	TP_MON,
	TP_TICK1,
	TP_TICK2,
	TP_TICK5,
	TP_TICK10,
	TP_TICK20,
	TP_TICK30,
	TP_TICK40,
	TP_TICK60
};

// 120분 차트는 6시 부터 시작한다
void fnGET_CHART_NM_EX(char* date, char*time, int tp, char* out)
{
	int divider; char tmp[32];	char zTm[32];
	sprintf(zTm, "%.8s", time);
	if (tp == TP_1MIN) divider = 1;
	if (tp == TP_3MIN) divider = 3;
	if (tp == TP_5MIN) divider = 5;
	if (tp == TP_10MIN) divider = 10;
	if (tp == TP_15MIN) divider = 15;
	if (tp == TP_20MIN) divider = 20;
	if (tp == TP_60MIN) divider = 60;
	if (tp == TP_120MIN) divider = 120;
	sprintf(tmp, "%.2s", (zTm + 3));
	int ret = (atoi(tmp) / divider);
	int min = (ret)*divider;
	if (tp == TP_60MIN) {
		int h = S2N(zTm, 2); if (h == 24) h = 0; sprintf(out, "%.8s%02d00", date, h);
	}
	else if (tp == TP_120MIN) {
		int h = S2N(zTm, 2);
		int hRemain = h % 2;
		if (hRemain == 1)	h -= 1;
		if (h == 24) h = 0; sprintf(out, "%.8s%02d00", date, h);
	}
	else
		sprintf(out, "%.8s%.2s%02d", date, zTm, min);
	//if(min==60){ int h = S2N(zTm,2)+1; if(h==24) h=0; sprintf(out, "%.8s%02d00",date,h);}	
	//else if(min==120){ int h = S2N(zTm,2)+2; if(h==24) h=0; sprintf(out, "%.8s%02d00",date,h);}	
	//else sprintf(out, "%.8s%.2s%02d",date, zTm, min);
}


void main()
{

	char date[] = "20180807";
	char time[] = "15:08:00:220";	// "15:14:37:997";
	char out[128] = { 0, };

	fnGET_CHART_NM_EX( date, time, 3,  out);
	 unsigned char flag = 0;
	 printf("%u\n", flag);

	flag |= 1;
	flag |= 2;
	flag |= 4;
	flag |= 8;

	printf("%u\n", flag);
	if (flag & 1)	printf("0000 0001 on\n");
	else			printf("0000 0001 off\n");
	
	if (flag & 2)	printf("0000 0010 on\n");
	else			printf("0000 0010 off\n");
	
	if (flag & 4)	printf("0000 0100 on\n");
	else			printf("0000 0100 off\n");
	
	if (flag & 8)	printf("0000 1000 on\n");
	else			printf("0000 1000 off\n");
	
	if (flag & 16)	printf("0001 0000 on\n");
	else			printf("0001 0000 off\n");

	printf("-------------------------------------\n");
	flag ^= 8;
	flag ^= 16;

	printf("%u\n", flag);
	if (flag & 8)	printf("0000 1000 on\n");
	else			printf("0000 1000 off\n");

	printf("%u\n", flag);
	if (flag & 16)	printf("0001 0000 on\n");
	else			printf("0001 0000 off\n");

	printf("-------------------------------------\n");
	
	flag &= ~4;
	printf("%u\n", flag); 
	
	if (flag & 4)	printf("0000 0100 on\n");
	else			printf("0000 0100 off\n");

	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	flag = 0;
	flag |= 32;
	printf("%u\n", flag);

	if (flag & 32)	printf("0010 0000 on\n");
	else			printf("0010 0000 off\n");

	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	flag = 0;
	flag |= 64;
	printf("%u\n", flag);

	if (flag & 64)	printf("0100 0000 on\n");
	else			printf("0100 0000 off\n");

	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	flag = 0;
	flag |= 128;
	printf("%u\n", flag);

	if (flag & 128)	printf("1000 0000 on\n");
	else			printf("1000 0000 off\n");

	//char guid[52];

	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);
	//MakeGUID(guid);	printf("%s\n", guid);

	getchar();
}