#pragma warning(disable:4996)

#include <time.h>

#include <stdio.h>



int main(void)

{

	__time32_t now_time;
	_time32(&now_time);


	struct tm *ltime = _localtime32(&now_time); //현재 초 단위 시간을 측정

	printf("현재 시각: %02d:%02d:%02d", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

	getchar();

	return 0;

}

