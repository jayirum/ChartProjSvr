
#include <stdio.h>
#include <ctype.h>
#include <string.h>



typedef void(*callback_key)(int id, int ch);

int handlerNum = 0;
callback_key callArr[16];

void procKeyA(int id, int ch)
{
	if (isdigit(ch)) {
		printf("A: id(%d),input(%d)\n", id, ch);
	}
}

void procKeyB(int id, int ch)
{
	if (isdigit(ch)) {
		printf("B: id(%d),input(%d)\n", id, ch);
	}
	else if (isalpha(ch)) {
		printf("B: id(%d),input(%c)\n", id, ch);
	}

	for (int i = 0; i<10; i++) {
		printf("do something...(%d)\n", i);
	}
}



void callReg(callback_key handler)
{
	callArr[handlerNum] = handler;
	handlerNum++;
}



// user input for simulation

int userInput()
{
	static char in[] = "12abcd234alkfz1ff";
	static int cnt = 0;
	static int inIdx = 0;
	static int len = strlen(in);
	int ret = 0;

	if (cnt % 5 == 0) 
	{
		if (cnt != 0) cnt = 0;
		if (inIdx == len) 
		{
			ret = -1;
		}
		else 
		{
			ret = in[inIdx++];
		}
	}
	cnt++;
	return ret;

}



int main(int argc, char *argv[])
{
	int ch;
	callReg(procKeyA);
	callReg(procKeyB);

	while (1)
	{
		ch = userInput();
		if (ch != 0) 
		{
			// system end
			if (ch == -1) break;

			// call handler
			for (int i = 0; i<handlerNum; i++) {
				callArr[i](i, ch);
			}
		}
		else {
			printf("normal state A, B\n");
		}
	}

	getchar();
	return 0;
}


