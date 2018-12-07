#pragma warning(disable:4996)

#include <time.h>

#include <stdio.h>



int main(void)

{

	__time32_t now_time;
	_time32(&now_time);


	//struct tm *ltime = _localtime32(&now_time); //현재 초 단위 시간을 측정

	time_t rawtime;
	time(&rawtime);
	struct tm *ltime = gmtime(&rawtime); //현재 초 단위 시간을 측정

	printf("현재 시각: %04d%02d%02d-%02d:%02d:%02d",
		1900+ltime->tm_year,ltime->tm_mon+1, ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

	getchar();

	return 0;

}

