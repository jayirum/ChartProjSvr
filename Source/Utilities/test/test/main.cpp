#pragma warning(disable:4996)

#include <time.h>

#include <stdio.h>



int main(void)

{

	__time32_t now_time;
	_time32(&now_time);


	struct tm *ltime = _localtime32(&now_time); //���� �� ���� �ð��� ����

	printf("���� �ð�: %02d:%02d:%02d", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

	getchar();

	return 0;

}

