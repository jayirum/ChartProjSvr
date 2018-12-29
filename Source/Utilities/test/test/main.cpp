#pragma warning(disable:4996)

#include <time.h>

#include <stdio.h>
#include <stdlib.h>


int main(void)

{
	time_t rawtime;
	struct tm * timeinfo;
	int year=2018, month=12, day=11;
	const char * weekday[] = { "Sunday", "Monday",
		"Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday" };

	/* prompt user for date */
	//printf("Enter year: "); fflush(stdout); scanf("%d", &year);
	//printf("Enter month: "); fflush(stdout); scanf("%d", &month);
	//printf("Enter day: "); fflush(stdout); scanf("%d", &day);

	/* get current timeinfo and modify it to the user's choice */
	char org[] = "20181211183600";
	char y[32]; sprintf(y, "%.4s", org);
	char m[32]; sprintf(m, "%.2s", org+4);
	char d[32]; sprintf(d, "%.2s", org+6);
	char hh[32]; sprintf(hh, "%.2s", org+8);
	char mm[32]; sprintf(mm, "%.2s", org + 10);
	char ss[32]; sprintf(ss, "%.2s", org + 12);

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	timeinfo->tm_year = atoi(y) - 1900;
	timeinfo->tm_mon = atoi(m) - 1;
	timeinfo->tm_mday = atoi(d);
	timeinfo->tm_hour = atoi(hh);
	timeinfo->tm_min = atoi(mm);
	timeinfo->tm_sec = atoi(ss);

	/* call mktime: timeinfo->tm_wday will be set */
	time_t timer = mktime(timeinfo) - (60*40);	// 1분 20개
	struct tm *ltime = localtime(&timer);
	printf("1분40개:%04d%02d%02d-%02d:%02d:%02d\n",
			1900 + ltime->tm_year, ltime->tm_mon + 1, ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

	timeinfo = localtime(&rawtime);
	timeinfo->tm_year = atoi(y) - 1900;
	timeinfo->tm_mon = atoi(m) - 1;
	timeinfo->tm_mday = atoi(d);
	timeinfo->tm_hour = atoi(hh);
	timeinfo->tm_min = atoi(mm);
	timeinfo->tm_sec = atoi(ss);
	timer = mktime(timeinfo) - (60 * 3 * 10);	// 1분 20개
	ltime = localtime(&timer);
	printf("3분10개:%04d%02d%02d-%02d:%02d:%02d\n",
		1900 + ltime->tm_year, ltime->tm_mon + 1, ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

	getchar();

	//time_t timer;
	///struct tm *ltime;


	//timer = time(NULL) - (24 * 60 * 60);
	//ltime = localtime(&timer);

	//printf("어제: %04d%02d%02d-%02d:%02d:%02d\n",
	//		1900+ltime->tm_year,ltime->tm_mon+1, ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

	//timer = time(NULL) - (60 * 60);
	//ltime = localtime(&timer);

	//printf("한시간전: %04d%02d%02d-%02d:%02d:%02d\n",
	//	1900 + ltime->tm_year, ltime->tm_mon + 1, ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec);


	//timer = time(NULL);
	//ltime = localtime(&timer);

	//char org[] = "20181211123456";
	//char dt[32]; sprintf(dt, "%.2s", org+10);
	//ltime->tm_min = atoi(dt);
	//char newdt[32]; 
	//


	//timer = time(NULL);
	//ltime = localtime(&timer);

	//printf("5년전: %04d%02d%02d-%02d:%02d:%02d\n",
	//	1900 + ltime->tm_year-5, ltime->tm_mon + 1, ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec);


	getchar();

	//__time32_t now_time;
	//_time32(&now_time);


	////struct tm *ltime = _localtime32(&now_time); //현재 초 단위 시간을 측정

	//time_t rawtime;
	//time(&rawtime);
	//struct tm *ltime = gmtime(&rawtime); //현재 초 단위 시간을 측정

	//printf("현재 시각: %04d%02d%02d-%02d:%02d:%02d",
	//	1900+ltime->tm_year,ltime->tm_mon+1, ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

	//printf("3분*20 분전 시각: %04d%02d%02d-%02d:%02d:%02d",
	//	1900 + ltime->tm_year, ltime->tm_mon + 1, ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

	//getchar();

	return 0;

}

