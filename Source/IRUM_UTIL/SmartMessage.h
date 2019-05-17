#pragma once
#include "SMDataTypes.h"
#include <Windows.h>
#include <stdio.h>

typedef long(__stdcall *RECV_CALLBACK)(int index, char* WorkThread, char*);

class CSmartMessage
{
public:
	CSmartMessage();
	virtual ~CSmartMessage();
	
	BOOL Begin();
	VOID End();

	BOOL ConnectSMSrv(char* pzIp, long lPort);
	VOID DisConnectSMSrv();
	BOOL AddDest(char* pDest, char* pMsg);
	void SetRecvCallBackFn();// (RECV_CALLBACK RecvCallBack);
	

	BOOL GetMsgOfRecvMsg(_Out_ char* pValue);
	BOOL GetBinaryFieldValue(char* pFieldName, int nValSize, _Out_ char* pValue);
	BOOL GetStringFieldValue(char* pFieldName, int nValSize, _Out_ char* pValue);
	BOOL GetIntegerFieldValue(char* pFieldName, int nValSize, _Out_ int* pValue);

	BOOL SMessageToSMessage();
	BOOL SetHeaderOfSMessage(int nDeliveryMode, char* pDest, char* pMsg, int ClientSession);
	BOOL SetBinaryFieldValue(char* pFieldName, char* pData, int nDataSize);
	BOOL SetStringFieldValue(char* pFieldName, char* pzData);
	BOOL SetIntegerFieldValue(char* pFieldName, int nData);
	BOOL SendData();


	char* GetMsg() { return m_zMsg; }
private:
	BOOL CreateInstance();
	long GetCurrInstanceCnt();

	
private:

	HINSTANCE	m_hIns;
	long		m_lIdx;
	char		m_zMsg[1024];
};


/*
library SmartMessage;

{ Important note about DLL memory management: ShareMem must be the
first unit in your library's USES clause AND your project's (select
Project-View Source) USES clause if your DLL exports any procedures or
functions that pass strings as parameters or function results. This
applies to all strings passed to and from your DLL--even those that
are nested in records and classes. ShareMem is the interface unit to
the BORLNDMM.DLL shared memory manager, which must be deployed along
with your DLL. To avoid using BORLNDMM.DLL, pass string information
using PChar or ShortString parameters. }

uses
SysUtils,
Classes,
SMClientU,
Windows,
Messages, Variants, Dialogs,
SubjectHeader, FieldHeader, CommonUnit, SiseHeader, System, MsgTypeHeader,
FieldValueHeader;

const MAX_INSTANCES_NUMBER = 10;

// messages types

const MESSAGE_TO_SEND = 0;
const RECEIVED_MESSAGE = 1;
const RESPOND_MESSAGE = 2;

(*uses
Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
Dialogs, OleServer, StdCtrls, ExtCtrls, DB,
ADODB, MemDS, DBAccess, MSAccess, SMClientU, ActiveX, FIFOQueue,
CommonUnit, SiseHeader, MsgTypeHeader, FieldHeader, ETradeHeader,
SubjectHeader, RBStringTree, SyncObjs, FieldValueHeader, LogUnit, IniFiles, AAUsers,
ipwcore, ipwipdaemon, StdUtilsKR;  *)


type TMyObject = class(TObject)
public
procedure OnThreadWorkEvent(WorkThread: TWorkThread; SMessage: TSMessage; var Handled: Boolean);
end;

type TWorkEventCallBack = function (index : integer; WorkThread : pointer; SMessage: pointer) : integer; stdcall;


var
SMClients : Array[0..MAX_INSTANCES_NUMBER] of TSMClient;
SMEvents : Array[0..MAX_INSTANCES_NUMBER] of TSMEvent;
AbstractObjects : Array[0..MAX_INSTANCES_NUMBER] of TMyObject;
WorkEventCallBacks : Array[0..MAX_INSTANCES_NUMBER] of TWorkEventCallBack;
MessagesToSend : Array[0..MAX_INSTANCES_NUMBER] of TSMessage;
ThreadWorkEvents : Array[0..MAX_INSTANCES_NUMBER] of TThreadWorkEvent;
LastReceivedMessages : Array[0..MAX_INSTANCES_NUMBER] of TSMessage;
RespondMessages : Array[0..MAX_INSTANCES_NUMBER] of TSMessage;
BinaryData : Array[0..MAX_INSTANCES_NUMBER] of TBinaryData;
StringValue : Array[0..MAX_INSTANCES_NUMBER] of string;
PCharValue : Array[0..MAX_INSTANCES_NUMBER] of Array[0..255] of byte;
WorkThreads : Array[0..MAX_INSTANCES_NUMBER] of TWorkThread;

ObjectsNumber : integer;

FutExec : TFutExec;

UniqKey : Array[0..255] of byte;
ClientIP : Array[0..255] of byte;

type CharPointer = ^char;

procedure TMyObject.OnThreadWorkEvent(WorkThread: TWorkThread; SMessage: TSMessage; var Handled: Boolean);
var resi, i1 : integer;
ownerindex : integer;
begin

ownerindex := -1;
for i1 := 0 to MAX_INSTANCES_NUMBER - 1 Do
begin
if Self = AbstractObjects[i1] then
begin
ownerindex := i1;
Break;
end;
end;

if ownerindex = -1 then
begin
Exit;
end;

WorkThreads[ownerindex] := WorkThread;

if Assigned(WorkEventCallBacks[ownerindex]) then
begin
LastReceivedMessages[ownerindex] := SMessage;
resi := WorkEventCallBacks[ownerindex](ownerindex, Addr(WorkThread), Addr(SMessage));
end;

(*if resi <> 0 then
begin
WorkThread.SendResponse(RespondMessages[ownerindex]);
ShowMessage('Response sent !');
end;       *)

//ShowMessage(IntToStr(resi));

end;

{$R *.res}

function ConvertPCharToString(p : PChar) : string;
var len, i1 : integer;
p2 : PChar;
s : string;
begin
len := 0;
p2 := p;
while (TRUE) do
begin
if (p2^ <> Chr(0)) then
begin
Inc(p2);
Inc(len);
end
else Break;
end;

SetLength(s, len);
for i1 := 1 to len Do
begin
s[i1] := p^;
Inc(p);
end;

Result := s;
end;

function ConvertStringToPChar(instr : string; outstr : PChar) : integer;
var len, i1 : integer;
p2 : PChar;
s : string;
begin
p2 := outstr;
len := Length(instr);
for i1 := 0 to (len - 1) Do
begin
p2^ := instr[i1 + 1];
Inc(p2);
end;
p2^ := #0;
end;

// create set of instances and return index of created instance
// if MAX Number reached then return -1;
function SMCreateInstance : integer; stdcall;
begin
if ObjectsNumber >= MAX_INSTANCES_NUMBER then
begin
Result := -1;
Exit;
end;

SMClientS[ObjectsNumber] := TSMClient.Create(nil);
SMEventS[ObjectsNumber] := TSMEvent.Create(nil);
SMClientS[ObjectsNumber].ThreadMode := tbCustom;
SMClientS[ObjectsNumber].ThreadCount := 10;
SMEventS[ObjectsNumber].SMSClient := SMClientS[ObjectsNumber];
SMEventS[ObjectsNumber].Active := TRUE;
MessagesToSend[ObjectsNumber] := TSMessage.Create;
RespondMessages[ObjectsNumber] := TSMessage.Create;

SMEventS[ObjectsNumber].Tag := ObjectsNumber;
SMClientS[ObjectsNumber].Tag := ObjectsNumber;

AbstractObjects[ObjectsNumber] := TMyObject.Create;
SMClientS[ObjectsNumber].OnThreadWorkEvent := AbstractObjects[ObjectsNumber].OnThreadWorkEvent;

// nil means no callbacks assigned,
// caller program need to assign callback function later to use it
WorkEventCallBacks[ObjectsNumber] := nil;

Inc(ObjectsNumber);

Result := ObjectsNumber;

end;

function Initialize : integer; stdcall;
var i1 : integer;
begin

ObjectsNumber := 0;
for i1 := 0 to MAX_INSTANCES_NUMBER - 1 Do
begin
SMClientS[i1] := nil;
SMEvents[i1] := nil;
AbstractObjects[i1] := nil;
WorkEventCallBacks[i1] := nil;
MessagesToSend[i1] := nil;
LastReceivedMessages[i1] := nil;
RespondMessages[i1] := nil;
BinaryData[i1] := nil;
WorkThreads[i1] := nil;
end;

Result := 0;

//SMClientR.OnThreadWorkEvent(MessageToSend, Handled);

//ShowMessage(IntToStr(SizeOf(FutExec)));
//ShowMessage('Debug output : Initialize inside DLL !');

end;

function DeInitialize : integer; stdcall;
begin
Result := 0;
end;

function SMClientDisconnect(index : integer) : integer; stdcall;
begin
Result := 0;
try
SMClients[index].Disconnect;
except
Result := -1;
end;
end;

function SMClientConnect(index : integer; host : PChar; port : integer) : integer; stdcall;
var hoststr : string;
resb, resb1, resb2 : boolean;
begin
	Result := 0;
	hoststr := ConvertPCharToString(host);
	resb := FALSE;
	try
		resb := SMClientS[index].Connect(hoststr, port);
	except
		resb := FALSE;
	end;
	if resb then 
		Result := 0 
	else Result := -1;
end;

function SMClientIsConnected(index : integer) : boolean; stdcall;
begin
Result := FALSE;
if not Assigned(SMClients[index]) then Exit;
Result := SMClientS[index].Connected;
end;

function SMSetMessageParameters(index : integer; messtype : integer; DeliveryMode : TDeliveryMode; Destination : PChar; Msg : PChar) : integer; stdcall;
begin

if messtype = MESSAGE_TO_SEND then
begin
MessagesToSend[index].DeliveryMode := DeliveryMode;
MessagesToSend[index].Destination := ConvertPCharToString(Destination);
MessagesToSend[index].Msg := ConvertPCharToString(Msg);
end;

if messtype = RECEIVED_MESSAGE then
begin
LastReceivedMessages[index].DeliveryMode := DeliveryMode;
LastReceivedMessages[index].Destination := ConvertPCharToString(Destination);
LastReceivedMessages[index].Msg := ConvertPCharToString(Msg);
end;

if messtype = RESPOND_MESSAGE then
begin
RespondMessages[index].DeliveryMode := DeliveryMode;
RespondMessages[index].Destination := ConvertPCharToString(Destination);
RespondMessages[index].Msg := ConvertPCharToString(Msg);
end;

Result := 0;
end;

function SMSetMessageBinaryField(index, messtype : integer; FieldName : PChar; Data : Pointer; DataSize : integer) : integer; stdcall;
var fname : string;
begin
Result := 0;
try
fname := ConvertPCharToString(FieldName);
if messtype = MESSAGE_TO_SEND then
MessagesToSend[index].SetField(fname, data, DataSize);
if messtype = RECEIVED_MESSAGE then
LastReceivedMessages[index].SetField(fname, data, DataSize);
if messtype = RESPOND_MESSAGE then
RespondMessages[index].SetField(fname, data, DataSize);
except
Result := -1;
end;
end;

function SMSetMessageStringField(index, messtype : integer; FieldName : PChar; Str : Pointer) : integer; stdcall;
var fname : string;
datastr : string;
begin
Result := 0;
try
fname := ConvertPCharToString(FieldName);
datastr := ConvertPCharToString(Str);
if messtype = MESSAGE_TO_SEND then
MessagesToSend[index].SetField(fname, datastr);
if messtype = RECEIVED_MESSAGE then
LastReceivedMessages[index].SetField(fname, datastr);
if messtype = RESPOND_MESSAGE then
RespondMessages[index].SetField(fname, datastr);
except
Result := -1;
end;
end;

function SMSetMessageIntegerField(index, messtype : integer; FieldName : PChar; Val : integer) : integer; stdcall;
var fname : string;
datastr : string;
begin
Result := 0;
try
fname := ConvertPCharToString(FieldName);
if messtype = MESSAGE_TO_SEND then
MessagesToSend[index].SetField(fname, Val);
if messtype = RECEIVED_MESSAGE then
LastReceivedMessages[index].SetField(fname, Val);
if messtype = RESPOND_MESSAGE then
RespondMessages[index].SetField(fname, Val);
except
Result := -1;
end;
end;

function SMSetMessageDoubleField(index, messtype : integer; FieldName : PChar; Val : double) : integer; stdcall;
var fname : string;
datastr : string;
begin
Result := 0;
try
fname := ConvertPCharToString(FieldName);
if messtype = MESSAGE_TO_SEND then
MessagesToSend[index].SetField(fname, Val);
if messtype = RECEIVED_MESSAGE then
LastReceivedMessages[index].SetField(fname, Val);
if messtype = RESPOND_MESSAGE then
RespondMessages[index].SetField(fname, Val);
except
Result := -1;
end;
end;

function SMSendMessage(index, messtype : integer) : integer; stdcall;
var resb : boolean;
begin
Result := -1;
try
if messtype = MESSAGE_TO_SEND then
resb := SMEventS[index].SendSMessage(MessagesToSend[index]);
if messtype = RESPOND_MESSAGE then
resb := SMEventS[index].SendSMessage(RespondMessages[index]);
if resb then Result := 0 else Result := -1;
except
Result := -1;
Exit;
end;
end;

function SMGetClientUniqKey(index : integer) : PChar; stdcall;
var key : string;
begin
key := SMClients[index].ClientUniqKey;
ConvertStringToPChar(key, Addr(UniqKey));
Result := Addr(UniqKey);
end;

function SMGetClientIP(index : integer) : PChar; stdcall;
var ip : string;
begin
ip := SMClients[index].ClientIP;
ConvertStringToPChar(ip, Addr(ClientIP));
Result := Addr(ClientIP);
end;

function SMEventAddDestination(index : integer; Dest : PChar; Msg : PChar) : integer; stdcall;
var deststr, msgstr : string;
begin
	Result := 0;
	deststr := ConvertPCharToString(Dest);
	msgstr := ConvertPCharToString(Msg);
	SMEvents[index].AddDest(deststr, msgstr);
end;

function SMEventRemoveDestination(index : integer; Dest : PChar; Msg : PChar) : integer; stdcall;
var deststr, msgstr : string;
begin
Result := 0;
deststr := ConvertPCharToString(Dest);
msgstr := ConvertPCharToString(Msg);
SMEvents[index].RemoveDest(deststr, msgstr);
end;

function SMEventAllRemoveDestination(index : integer) : integer; stdcall;
var deststr, msgstr : string;
begin
Result := 0;
SMEvents[index].AllRemoveDest;
end;

function SMGetReceivedCnt(index : integer) : integer; stdcall;
begin
Result := SMClientS[index].RcvCount;
end;

function SMSetWorkEventCallBack(index : integer; Addr : pointer) : integer; stdcall;
begin
WorkEventCallBacks[index] := Addr;
Result := 0;
end;

function SMMessageGetBinaryFieldValue(index, messtype : integer; FieldName : PChar) : PChar; stdcall;
var 
	fn, fv : string;
	fvp : PChar;
	Mess2 : TSMessage;
	P1 : pointer;
begin
	fn := ConvertPCharToString(FieldName);
	if messtype = MESSAGE_TO_SEND then
		BinaryData[index] := MessagesToSend[index].GetBinaryFieldValue(fn);
	if messtype = RECEIVED_MESSAGE then
		BinaryData[index] := LastReceivedMessages[index].GetBinaryFieldValue(fn);
	if messtype = RESPOND_MESSAGE then
		BinaryData[index] := RespondMessages[index].GetBinaryFieldValue(fn);

	Result := BinaryData[index].Buffer;
end;

function SMMessageGetStringFieldValue(index, messtype : integer; FieldName : PChar) : PChar; stdcall;
var 
	fn, fv : string;
	fvp : PChar;
	Mess2 : TSMessage;
	P1 : pointer;
begin
	fn := ConvertPCharToString(FieldName);
	try
		if messtype = MESSAGE_TO_SEND then
		StringValue[index] := MessagesToSend[index].GetStringFieldValue(fn);
		if messtype = RECEIVED_MESSAGE then
		StringValue[index] := LastReceivedMessages[index].GetStringFieldValue(fn);
		if messtype = RESPOND_MESSAGE then
		StringValue[index] := RespondMessages[index].GetStringFieldValue(fn);
	except
		Result := nil;
	Exit;
end;

ConvertStringToPChar(StringValue[index], Addr(PCharValue[index]));
Result := Addr(PCharValue[index]);
end;

function SMMessageGetIntegerFieldValue(index, messtype : integer; FieldName : PChar) : integer; stdcall;
var 
	fn, fv : string;
	fvp : PChar;
	Mess2 : TSMessage;
	P1 : pointer;
begin
	fn := ConvertPCharToString(FieldName);
	try
		if messtype = MESSAGE_TO_SEND then
		Result := MessagesToSend[index].GetIntegerFieldValue(fn);
		if messtype = RECEIVED_MESSAGE then
		Result := LastReceivedMessages[index].GetIntegerFieldValue(fn);
		if messtype = RESPOND_MESSAGE then
		Result := RespondMessages[index].GetIntegerFieldValue(fn);
	except
		Result := -1;
		Exit;
	end;
end;

function SMSMessageToSMessage(index, inmesstype, outmesstype : integer) : integer; stdcall;
begin
	Result := -1;

	if outmesstype = RESPOND_MESSAGE then
	begin
		if inmesstype = RECEIVED_MESSAGE then
		begin
			RespondMessages[index].SMessageToSMessage(LastReceivedMessages[index]);
			Result := 1;
		end;
		if inmesstype = MESSAGE_TO_SEND then
		begin
			RespondMessages[index].SMessageToSMessage(MessagesToSend[index]);
			Result := 2;
		end;
	end;

	if outmesstype = RECEIVED_MESSAGE then
	begin
		if inmesstype = RESPOND_MESSAGE then
		begin
			LastReceivedMessages[index].SMessageToSMessage(RespondMessages[index]);
			Result := 3;
		end;
		if inmesstype = MESSAGE_TO_SEND then
		begin
			LastReceivedMessages[index].SMessageToSMessage(MessagesToSend[index]);
			Result := 4;
		end;
	end;

	if outmesstype = MESSAGE_TO_SEND then
	begin
		if inmesstype = RESPOND_MESSAGE then
		begin
			MessagesToSend[index].SMessageToSMessage(RespondMessages[index]);
			Result := 5;
		end;
		if inmesstype = RECEIVED_MESSAGE then
		begin
			MessagesToSend[index].SMessageToSMessage(LastReceivedMessages[index]);
			Result := 6;
		end;
	end;

end;

function SMGetObjectsNumber : integer; stdcall;
begin
Result := ObjectsNumber;
end;

function SMGetMaximumObjectsNumber : integer; stdcall;
begin
Result := MAX_INSTANCES_NUMBER;
end;

function SMSendResponse(index, messtype : integer) : integer; stdcall;
begin
Result := -1;
if WorkThreads[index] <> nil then
begin
try
Result := 0;
WorkThreads[index].SendResponse(RespondMessages[index]);
except
Result := -1;
end;
end;
end;

function SMMessageGetDeliveryType(index, messtype : integer) : integer; stdcall;
begin
if messtype = RECEIVED_MESSAGE then
begin
Result := Ord(LastReceivedMessages[index].DeliveryMode);
end;
if messtype = RESPOND_MESSAGE then
begin
Result := Ord(RespondMessages[index].DeliveryMode);
end;
if messtype = MESSAGE_TO_SEND then
begin
Result := Ord(MessagesToSend[index].DeliveryMode);
end;

end;

exports // Make available to calling applications

Initialize,
DeInitialize,
SMClientConnect,
SMClientIsConnected,
SMClientDisconnect,
SMSetMessageParameters,
SMSetMessageBinaryField,
SMSetMessageStringField,
SMSetMessageIntegerField,
SMSetMessageDoubleField,
SMSendMessage,
SMGetClientUniqKey,
SMEventAddDestination,
SMEventRemoveDestination,
SMEventAllRemoveDestination,
SMGetReceivedCnt,
SMSetWorkEventCallBack,
SMMessageGetBinaryFieldValue,
SMMessageGetStringFieldValue,
SMMessageGetIntegerFieldValue,
SMMessageGetDeliveryType,
SMGetObjectsNumber,
SMGetMaximumObjectsNumber,
SMCreateInstance,
SMGetClientIP,
SMSMessageToSMessage,
SMSendResponse;

begin



end.

*/