//#import "C:\Windows\SysWOW64\NtDirect.dll"
#include <stdio.h>
#include <string>

#include<Windows.h> //LoadLibray, FreeLibrary, GetProscAddress�� �ʿ�

//using func = double(*)(double, double); //dll�� �ִ� �Լ��� ������ ��Ī

using namespace std;
typedef int(*command_func)(string command, string account, string instrument, string action, int quantity, string orderType, double limitPrice, double stopPrice,
	string timeInForce, string oco, string orderId, string strategy, string strategyId);
typedef int(*connected_func)(int showMessage);
typedef int(*subscribe_market_data_func)(string instrument);
typedef int(*unsubscribe_market_data_func)(string instrument);
typedef double(*market_data_func)(string instrument, int zero);
typedef int(*tear_down_func)();

tear_down_func TearDown;
connected_func Connected;
command_func Command;
subscribe_market_data_func SubscribeMarketData;
unsubscribe_market_data_func UnsubscribeMarketData;
market_data_func MarketData;

int main()

{

	//dll �о� ����

	HMODULE nt_lib = LoadLibrary(TEXT("NtDirect.dll"));

	if (nt_lib == NULL) {

		printf("�ε� ����");

		exit(0);

	}

	else {

		printf("�ε�");

	}



	//dll ���� �Լ� ã��

	Connected = (connected_func)GetProcAddress(nt_lib, "Connected");
	if (!Connected)
	{
		printf("connected error\n");
		exit(0);
	}
	else {

		printf("connected ok\n");
	}


	//dll ���� ����

	FreeLibrary(nt_lib);

	puts("��ε�");

	getchar();

}