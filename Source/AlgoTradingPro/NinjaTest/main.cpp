//#import "C:\Windows\SysWOW64\NtDirect.dll"
#include <stdio.h>
#include <string>

#include<Windows.h> //LoadLibray, FreeLibrary, GetProscAddress에 필요

//using func = double(*)(double, double); //dll에 있는 함수의 형태의 별칭

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

	//dll 읽어 오기

	HMODULE nt_lib = LoadLibrary(TEXT("NtDirect.dll"));

	if (nt_lib == NULL) {

		printf("로드 실패");

		exit(0);

	}

	else {

		printf("로드");

	}



	//dll 내의 함수 찾기

	Connected = (connected_func)GetProcAddress(nt_lib, "Connected");
	if (!Connected)
	{
		printf("connected error\n");
		exit(0);
	}
	else {

		printf("connected ok\n");
	}


	//dll 내려 놓기

	FreeLibrary(nt_lib);

	puts("언로드");

	getchar();

}