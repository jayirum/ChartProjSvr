#include <stdio.h>
#include "../../IRUM_UTIL/Util.h"


void main()
{
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