
#pragma once

#include <winsock2.h>
#include "Common.h"
#include "MemTable.h"
#include "RBStringTree.h"
#include <Windows.h>
#include "Thread.h"
#include <vector>

#pragma comment(lib, "ws2_32.lib")
class TSMClient;

typedef struct _TDestinationRecord
{
	char	Destination[MAXLEN_DEST];
	char	Msg[MAXLEN_DEST_MSG];
	HWND	RcvHandle;
}TDestinationRecord;	// , *pTDestinationRecord;



class TDestinationList : public TBaseMemTable
{
public:
	TDestinationList();
	virtual ~TDestinationList();

	void InsertData(char* Dest, char* Msg, HWND RcvHandle);
	virtual void* CreateRecord();

protected:
	virtual void DataFree(void* Data);;
	virtual char*  GetKey(int Index, void* Data, char* oKey);
	
};


class TBinaryData
{
public:
	TBinaryData( char* Buffer, int Len);
	virtual ~TBinaryData();
	char* BufferGet() { return FBuffer; };
	void BufferSet(char* buff) { strcpy(FBuffer, buff); };
	int LenGet() { return FLen; };
	void LenSet(int len) { FLen = len; };

	property(char*, BufferGet, BufferSet) Buffer;
	property(int, LenGet, LenSet) Len;
private:
	char* FBuffer;
	int FLen;
};

struct TFieldData
{
	TMsgFieldType MsgFieldType;
	int Int;
	double DF;
	char Str[MAXLEN_FIELD_STR];
	TBinaryData* Bin;
};
typedef TFieldData* PFieldData;

class TSMessage
{
public:
	TSMessage();
	virtual ~TSMessage();
private:
	int MakeToPChar(char* Buffer);
	bool MakeToSMessage(char* Buffer);

	bool SMessageToSMessage(TSMessage SMessage);
	char* GetStringFieldValue(char* FieldName, char* pOut);
	int GetIntegerFieldValue(char* FieldName);
	double GetDoubleFieldValue(char* FieldName);
	TBinaryData* GetBinaryFieldValue(char* FieldName);
	void SetField(char* Name, int Value);
	void SetField(char* Name, double Value);
	void SetField(char* Name, char* Value);
	void SetField(char* Name, char* Buffer, int Len);
	void Clear();
public:
	TDeliveryMode DeliveryModeGet() { return FDeliveryMode; };
	void DeliveryModeSet(TDeliveryMode mode) { FDeliveryMode = mode; };
	char* MsgGet() { return FMsg; };
	void MsgSet(char* msg) { strcpy(FMsg, msg); };
	char* DestinationGet() { return FDest; };
	void DestinationSet(char* dest) { strcpy(FDest, dest); };
	int ClientSessionGet() { return FClientSession; };
	void ClientSessionSet(int sess) { FClientSession = sess; };
	char *ClientUniqKeyGet() { return FClientUniqKey; };
	void ClientUniqKeySet() { return ; };

	property(TDeliveryMode, DeliveryModeGet, DeliveryModeSet) DeliveryMode;
	property(char*, MsgGet, MsgSet) Msg;
	property(char*, DestinationGet, DestinationSet) Destination;
	property(int, ClientSessionGet, ClientSessionSet) ClientSession;
	property(char*, ClientUniqKeyGet, ClientUniqKeySet) ClientUniqKey;


private:
	TRBStringTree FFieldList;
	TDeliveryMode FDeliveryMode;
	char FDest[MAXLEN_DEST];
	char FMsg[MAXLEN_DEST_MSG];
	int FClientSession;
	char FClientUniqKey[MAXLEN_CLIENT_UID];
};

class TSMSConnectThread : public TThread
{
public:
	TSMSConnectThread(TSMClient* FSMClient);
	virtual ~TSMSConnectThread();

	bool FUserDisconnect;

protected:
	void Execute(); 

private:
	TSMClient *FSMClient;
};


class TSMClient
{

public:
	TSMClient();
	virtual ~TSMClient();


	BOOL Connected();
	char* ClientIP();

	int ThreadCountGet() { return FThreadCount; };
	void ThreadCountSet(int cnt=1) { FThreadCount=cnt; };

	//todo property ThreadClass: TWorkThreadClass read FWorkThreadClass write FWorkThreadClass;
	//todo property MessageFilterHandle: HWND read FMessageFilterHandle write FMessageFilterHandle;
	//property BandWidth: double read FBandWidth write SetBandWidth; // 제한 없음
	//property ThreadMode: ThreadMode read FThreadMode write SetThreadMode default thSingal;
	property(INT, ThreadCountGet, ThreadCountSet) ThreadCount;
	//todo property OnThreadWorkEvent: TThreadWorkEvent read FThreadWorkEvent write FThreadWorkEvent;
	//property UseNagle: boolean read FUseNagle write SetUseNagle default True;

	

//
//	void SetThreadMode(ThreadMode Value);
//	void SetThreadCount(int Value);
//	int GetAutoThreadCount();
//	void SetBandWidth(double Value);
//	void SetUseNagle(bool Value);
//
//private:
	WNDPROC WndProc(Messages::);	// (TMessage Msg);
//	void IncRcvCount();
//	void ServerClientUniqKey();
//	void ServerAddDest(char* Dest);
//	void ServerRemoveDest(char* Dest);
//	void ServerSendDest(char* pBuffer, int SendSize);
//	void IncSendThreadCount();
//	void DecSendThreadCount();
//	int  SendThreadCount();
//	void Lock();
//	void UnLock();
//public:
//	int RcvCount();
//	bool Connect(char* IP, int Port);
	char* ClientUniqKey();
	void Disconnect();
//	void WriteBuffer(char* ABuffer, int AByteCount);
//
//	//void TransactionLogPath(const LogFileName : string; const LogKey : String);
//	void BeginTransaction();
//	void EndTransaction();

	HWND AllocateHWnd(WNDPROC WndMethod);
	void DeallocateHWnd(HWND hWnd);



public:
	double FBandWidth;
	ThreadMode FThreadMode;
	long FlthreadCnt;
	bool FUseNagle;
private:
	HWND FMessageFilterHandle;
	int FLogPos;
	CRITICAL_SECTION FLogLock, FLock;
	int FSendThreadCount;
	int FBeginTransaction;
	int FEndTransaction;
	int FThreadCount;
	TSMSConnectThread *FSMSConnectThread;
	HWND FWindowHandle;
	int FRcvCount;
	char FClientUniqKey[MAXLEN_CLIENT_UID];
	TDestinationList FDestinationList;
	std::vector<void*>	FWorkThreadList;	//TWorkThreadClass

	//FIFOQueue: TFIFOQueue;
	
	//FTCPClient: TIdTCPClient;
	//FSMessageFilterEvent: TSMessageFilterEvent;
	//FProcessLog: TProcessLog;
	//FThreadWorkEvent: TThreadWorkEvent;
	//FWorkThreadClass: TWorkThreadClass;


};
