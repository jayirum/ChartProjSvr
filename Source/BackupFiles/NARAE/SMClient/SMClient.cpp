#include "SMClient.h"
#include <windows.h>
#include <string.h>
#include <limits.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <TSMessage>

TSMessage::TSMessage()
{

	ZeroMemory(FClientUniqKey, sizeof(FClientUniqKey));
	FDeliveryMode = Delivery_Push;
	//FFieldList = TRBStringTree.Create;
}

TSMessage::~TSMessage()
{

	TRBStringNode* IT;

	IT = FFieldList.First;
	if (IT != NULL)
	{
		while (IT != FFieldList.Last)
		{
			if ((PFieldData(IT->Data))->MsgFieldType == mfBinary)
				delete PFieldData(IT->Data)->Bin;
			//else if (PFieldData(IT->Data)->MsgFieldType == mfString )
			//	StrDispose(PFieldData(IT->Data) ^ .Str);
			delete (IT->Data);
			FFieldList.RBInc(IT);
		}

		if (PFieldData(IT->Data)->MsgFieldType == mfBinary)
			delete PFieldData(IT->Data)->Bin;
		//else if PFieldData(IT->Data) ^ .MsgFieldType = mfString then
		//	StrDispose(PFieldData(IT->Data) ^ .Str);

		delete (IT->Data);
	}
}



TBinaryData* TSMessage::GetBinaryFieldValue(char* FieldName)
{
	TRBStringNode* RBNodeP;
	TBinaryData* Result = NULL;

	_strupr(FieldName);
	RBNodeP = FFieldList.Find(FieldName);
	if (RBNodeP != NULL)
	{
		if (PFieldData(RBNodeP->Data)->MsgFieldType = mfBinary)
			Result = PFieldData(RBNodeP->Data)->Bin;
		else
			Result = NULL;	// raise EConvertError.Create('(double,integer,string) is not a valid TBinaryData value');
	}

	return Result;
}




double TSMessage::GetDoubleFieldValue(char* FieldName)
{
	TRBStringNode* RBNodeP; 
	double Result = 0;
	_strupr(FieldName);
	RBNodeP = FFieldList.Find(FieldName);
	if (RBNodeP != NULL)
	{
		if (PFieldData(RBNodeP->Data)->MsgFieldType = mfInteger)
			Result = PFieldData(RBNodeP->Data)->Int;
		else if (PFieldData(RBNodeP->Data)->MsgFieldType == mfDouble)
			Result = PFieldData(RBNodeP->Data)->DF;
		else if (PFieldData(RBNodeP->Data)->MsgFieldType == mfString)
			Result = atof(PFieldData(RBNodeP->Data)->Str);
		else
			Result = 0;	// raise EConvertError.Create('binary is not a valid double value');
	}
	return Result;
}

int TSMessage::GetIntegerFieldValue(char* FieldName)
{
	TRBStringNode* RBNodeP;
	int Result = 0;
	_strupr(FieldName);
	RBNodeP = FFieldList.Find(FieldName);
	if (RBNodeP != NULL)
	{
		if (PFieldData(RBNodeP->Data)->MsgFieldType == mfInteger)
			Result = PFieldData(RBNodeP->Data)->Int;
		else if (PFieldData(RBNodeP->Data)->MsgFieldType == mfDouble)
			Result = trunc(PFieldData(RBNodeP->Data)->DF);
		else if (PFieldData(RBNodeP->Data)->MsgFieldType == mfString)
			Result = atoi(PFieldData(RBNodeP->Data)->Str);
		else
			Result = 0; // raise EConvertError.Create('binary is not a valid integer value');
	}
	return Result;
}

char* TSMessage::GetStringFieldValue(char* FieldName, char* pOut)
{
	TRBStringNode* RBNodeP;
	_strupr(FieldName);
	RBNodeP = FFieldList.Find(FieldName);
	if (RBNodeP != NULL)
	{
		if (PFieldData(RBNodeP->Data)->MsgFieldType == mfInteger)
			sprintf(pOut, "%d", PFieldData(RBNodeP->Data)->Int);
		else if (PFieldData(RBNodeP->Data)->MsgFieldType == mfDouble)
			sprintf(pOut, "%f", PFieldData(RBNodeP->Data)->DF);
		else if (PFieldData(RBNodeP->Data)->MsgFieldType == mfString)
			strcpy(pOut, PFieldData(RBNodeP->Data)->Str);
		else
			pOut = NULL;	// raise EConvertError.Create('binary is not a valid string value');
	}
	return pOut;
}

void TSMessage::SetField(char* Name, int Value)
{
	PFieldData FieldData  ;
	TRBStringNode* RBNodeP ;
	char UName[MAXLEN_KEY];
	strcpy(UName, _strupr(Name));
	RBNodeP = FFieldList.Find(UName);
	if (RBNodeP == NULL)
		FieldData = new TFieldData;
	else
		FieldData = PFieldData(RBNodeP->Data);

	FieldData->MsgFieldType = mfInteger;
	FieldData->Int = Value;

	if (RBNodeP == NULL)
		FFieldList.Add(UName, FieldData);
}

void TSMessage::SetField(char* Name, char* Value)
{
	PFieldData FieldData  ;
	TRBStringNode* RBNodeP ;
	char UName[MAXLEN_KEY];
	strcpy(UName, _strupr(Name));

	RBNodeP = FFieldList.Find(UName);
	if (RBNodeP == NULL)
		FieldData = new TFieldData;
	else {
		FieldData = PFieldData(RBNodeP->Data);
		ZeroMemory(FieldData->Str, sizeof(FieldData->Str));
	}

	FieldData->MsgFieldType = mfString;
	strcpy(FieldData->Str, Value);

	if (RBNodeP == NULL)
		FFieldList.Add(UName, FieldData);
}

void TSMessage::SetField(char* Name, double Value)
{
	PFieldData FieldData;
	TRBStringNode* RBNodeP;
	char UName[MAXLEN_KEY];
	strcpy(UName, _strupr(Name));

	RBNodeP = FFieldList.Find(UName);
	if (RBNodeP == NULL)
		FieldData = new TFieldData;
	else 
		FieldData = PFieldData(RBNodeP->Data);

	FieldData->MsgFieldType = mfDouble;
	FieldData->DF = Value;

	if (RBNodeP == NULL)
		FFieldList.Add(UName, FieldData);
}

int GetByteSize(int Len)
{
	int Result = 0;
	if (Len <= SCHAR_MAX * 2)
		Result = sizeof(char);
	else if (Len <= SHRT_MAX)
		Result = sizeof(short);
	else if (Len <= INT_MAX)
		Result = sizeof(int);

	return Result;
}

int CalcDataSize(TRBStringNode* IT)
{
	int iLen;
	int	Result = 0;
	
	/////////////////////////////////////////
	// FieldName 정보
	Result++;	// Data Info (1Byte)
					// 길이를 표현하기 위해 필요한 바이트수
	iLen = strlen(IT->Key);
	Result += GetByteSize(iLen);
	Result += iLen; // Data Len
	
	/////////////////////////////////////////
	// Data 정보
	Result++;	// Data Info (1Byte)
	if (PFieldData(IT->Data)->MsgFieldType == mfInteger)
		Result += sizeof(int);
	else if (PFieldData(IT->Data)->MsgFieldType == mfDouble)
		Result += sizeof(double);
	else if (PFieldData(IT->Data)->MsgFieldType == mfString)
	{
		iLen = strlen(PFieldData(IT->Data)->Str);
		Result += GetByteSize(iLen);
		Result += iLen;
	}
	else if (PFieldData(IT->Data)->MsgFieldType == mfBinary)
	{
		TBinaryData* p = PFieldData(IT->Data)->Bin;
		iLen = p->Len;
		Result += GetByteSize(iLen);
		Result += iLen;
	}

	return Result;
}

void SetFieldData(TRBStringNode* IT, int iPos, char* Buffer)	// char* Buffer 파라미터는 MakeToPChar 에 인자로 들어오는 Buffer 이다.
{
	int iLen, iByteCnt;
	int iDataHead;

	/////////////////////////////////////////
	// FieldName 정보 설정
	iDataHead = iPos;
	Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_FIELD;
	iPos++;
	iLen = strlen(IT->Key);
	iByteCnt = GetByteSize(iLen);
	if (iByteCnt == 1)
	{
		Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_BYTE_1;
		*((char*)(Buffer + iPos)) = iLen;	//*(Buffer + iPos)  = iLen;
	}
	else if (iByteCnt == 2)
	{
		Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_BYTE_2;
		*((short*)(Buffer + iPos)) = iLen;
	}
	else if (iByteCnt == 4)
	{
		Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_BYTE_4;
		*((long*)(Buffer + iPos)) = iLen;
	}
	iPos += iByteCnt;
	memcpy(Buffer + iPos, &(IT->Key[1]), iLen);
	iPos += iLen;
	iDataHead = iPos;
	iPos++;

	/////////////////////////////////////////
	// Data 정보
	if (PFieldData(IT->Data)->MsgFieldType == mfInteger)
	{
		Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_INTEGER;
		*((int*)(Buffer + iPos)) = PFieldData(IT->Data)->Int;

		iPos += sizeof(int);
	}
	else if (PFieldData(IT->Data)->MsgFieldType == mfDouble)
	{
		Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_DOUBLE;
		*((double*)(Buffer + iPos)) = PFieldData(IT->Data)->DF;
		iPos += sizeof(double);
	}
	else if (PFieldData(IT->Data)->MsgFieldType == mfString)
	{
		Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_STRING;
		iLen = strlen(PFieldData(IT->Data)->Str);
		iByteCnt = GetByteSize(iLen);
		if (iByteCnt == 1)
		{
			Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_BYTE_1;
			*((char*)(Buffer + iPos)) = iLen;
		}
		else if (iByteCnt == 2)
		{
			Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_BYTE_2;
			*((short*)(Buffer + iPos)) = iLen;
		}
		else if (iByteCnt == 4)
		{
			Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_BYTE_4;
			*((long*)(Buffer + iPos)) = iLen;
		}
		iPos += iByteCnt;
		memcpy(Buffer + iPos, PFieldData(IT->Data)->Str, iLen);
		iPos += iLen;
	}
	else if (PFieldData(IT->Data)->MsgFieldType == mfBinary)
	{
		Buffer[iDataHead] = __toascii(Buffer[iPos]) | SM_DATA_BINARY;
		iLen = PFieldData(IT->Data)->Bin->Len;
		iByteCnt = GetByteSize(iLen);
		if (iByteCnt == 1)
		{
			Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_BYTE_1;
			*((char*)(Buffer + iPos)) = iLen;
		}
		else if (iByteCnt == 2)
		{
			Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_BYTE_2;
			*((short*)(Buffer + iPos)) = iLen;
		}
		else if (iByteCnt == 4)
		{
			Buffer[iDataHead] = __toascii(Buffer[iDataHead]) | SM_DATA_BYTE_4;
			*((long*)(Buffer + iPos)) = iLen;
		}
		iPos += iByteCnt;
		memcpy(Buffer + iPos, PFieldData(IT->Data)->Bin->Buffer, iLen);
		iPos+=iLen;
	}
}


int TSMessage::MakeToPChar(char* Buffer /*out*/)
{
	TRBStringNode* IT;
	int BufferSize, iPos;
	int Result = 0;

	BufferSize = 0;

	// Buufer Size 계산
	IT = FFieldList.First;
	if (IT != NULL)
	{
		while (IT != FFieldList.Last)
		{
			BufferSize += CalcDataSize(IT);
			FFieldList.RBInc(IT);
		}
		BufferSize += CalcDataSize(IT);
	}
	Result = sizeof(TSMH_Head) + sizeof(short) + strlen(FDest) + BufferSize;
	Buffer = new char[Result];
	ZeroMemory(Buffer, Result);
	iPos = sizeof(TSMH_Head) + sizeof(short) + strlen(FDest);
	IT = FFieldList.First;
	if (IT != NULL)
	{
		while (IT != FFieldList.Last)
		{
			SetFieldData(IT, iPos, Buffer);
			FFieldList.RBInc(IT);
		}
		SetFieldData(IT, iPos, Buffer);
	}
	Result = sizeof(TSMH_Head) + sizeof(short) + strlen(FDest) + BufferSize;
	return Result;
}



void TSMessage::SetField(char* Name, char* Buffer, int Len)
{
	PFieldData FieldData;
	TRBStringNode* RBNodeP;
	char UName[MAXLEN_KEY];
	strcpy(UName, _strupr(Name));
	RBNodeP = FFieldList.Find(UName);
	if (RBNodeP == NULL)
		FieldData = new TFieldData;
	else
		FieldData = PFieldData(RBNodeP->Data);

	FieldData->MsgFieldType = mfBinary;
	FieldData->Bin = new TBinaryData(Buffer, Len);

	if (RBNodeP == NULL)
		FFieldList.Add(UName, FieldData);

}


bool TSMessage::SMessageToSMessage(TSMessage SMessage)
{
	
	TRBStringNode*	IT  ;
	//PFieldData FieldData ;
	
	Clear();
	bool Result = false;
	
	FDeliveryMode = SMessage.FDeliveryMode;
	//
	IT = SMessage.FFieldList.First;
	if (IT != NULL)
	{
		while (IT != SMessage.FFieldList.Last)
		{
			if (PFieldData(IT->Data)->MsgFieldType == mfBinary)
			{
				SetField(IT->Key, PFieldData(IT->Data)->Bin->Buffer, PFieldData(IT->Data)->Bin->Len);
			}
			else if (PFieldData(IT->Data)->MsgFieldType == mfString)
			{
				SetField(IT->Key, PFieldData(IT->Data)->Str);
			}
			else if (PFieldData(IT->Data)->MsgFieldType == mfInteger)
			{
				SetField(IT->Key, PFieldData(IT->Data)->Int);
			}
			else if (PFieldData(IT->Data)->MsgFieldType == mfDouble)
			{
				SetField(IT->Key, PFieldData(IT->Data)->DF);
			}
			SMessage.FFieldList.RBInc(IT);
		}

		if (PFieldData(IT->Data)->MsgFieldType == mfBinary)
		{
			SetField(IT->Key, PFieldData(IT->Data)->Bin->Buffer, PFieldData(IT->Data)->Bin->Len);
		}
		else if (PFieldData(IT->Data)->MsgFieldType == mfString)
		{
			SetField(IT->Key, PFieldData(IT->Data)->Str);
		}
		else if (PFieldData(IT->Data)->MsgFieldType == mfInteger)
		{
			SetField(IT->Key, PFieldData(IT->Data)->Int);
		}
		else if (PFieldData(IT->Data)->MsgFieldType == mfDouble)
		{
			SetField(IT->Key, PFieldData(IT->Data)->DF);
		}
	} // if (IT != NULL)
	//
	strcpy(FMsg, SMessage.FMsg);
	FClientSession = SMessage.FClientSession;
	strcpy(FClientUniqKey, SMessage.FClientUniqKey);
	Result = true;
	return Result;
}



bool TSMessage::MakeToSMessage(char* Buffer)
{
	char FieldName[MAXLEN_FIELD_STR], DataStr[MAXLEN_DATA];
	int iPos, iLen, iDataHead, SMsgLen;
	
	Clear();
	bool Result = false;
	
	iLen = *((short*)(Buffer + sizeof(TSMH_Head))) ;
	//SetLength(FDest, iLen);
	memcpy( FDest+1, (Buffer + sizeof(TSMH_Head) + sizeof(short)) , iLen);

	if (TR_CODE(PSMH_Head(Buffer)->TR) == ctDestPushSend)
		FDeliveryMode = Delivery_Push;
	else if (TR_CODE(PSMH_Head(Buffer)->TR) == ctDestRRSendResponse)
		FDeliveryMode = Delivery_RR_Response;
	else
		FDeliveryMode = Delivery_RR;

	SMsgLen = PSMH_Head(Buffer)->DataLen - sizeof(short) - *((short*)(Buffer + sizeof(TSMH_Head))) ;
	Buffer = Buffer + sizeof(TSMH_Head) + sizeof(short) + *((short*)(Buffer + sizeof(TSMH_Head))) ;
	iPos = 0;

	while (iPos < SMsgLen)
	{
		iDataHead = iPos;
		iPos++;
		if (  ((*(char*)(Buffer[iDataHead])) & SM_DATA_FIELD) > 0)
		{
			if ( ((*(char*)(Buffer[iDataHead])) & SM_DATA_BYTE_1) > 0)
			{
				iLen = (*(char*)(Buffer + iPos));
				iPos += sizeof(char);
			}
			else if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_BYTE_2) > 0)
			{
				iLen = (*(short*)(Buffer + iPos));
				iPos += sizeof(short);
			}
			else if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_BYTE_4) > 0)
			{
				iLen = (*(long*)(Buffer + iPos));
				iPos += sizeof(long);
			}
			//SetLength(FieldName, iLen);
			memcpy(FieldName+1, Buffer + iPos, iLen);
			iPos += iLen;

		} // if ((*(char*)(Buffer[iDataHead])) & SM_DATA_FIELD > 0)

		iDataHead = iPos;
		iPos++;

		if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_INTEGER) > 0)
		{
			SetField(FieldName, (*(int*)(Buffer + iPos))  );
			iPos += sizeof(int);
		}
		else if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_DOUBLE) > 0)
		{
			SetField(FieldName, (*(double*)(Buffer + iPos)));
			iPos += sizeof(double);
		}
		else if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_STRING) > 0)
		{
			if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_BYTE_1) > 0)
			{
				iLen = (*(char*)(Buffer + iPos));
				iPos += sizeof(char);
			}
			else if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_BYTE_2) > 0)
			{
				iLen = (*(short*)(Buffer + iPos));
				iPos += sizeof(short);
			}
			else if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_BYTE_4) > 0)
			{
				iLen = (*(long*)(Buffer + iPos));
				iPos += sizeof(long);
			}
			//SetLength(DataStr, iLen);
			memcpy(DataStr+1, Buffer + iPos, iLen);
			SetField(FieldName, DataStr);

			iPos += iLen;
		}
		else if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_BINARY) > 0)
		{
			if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_BYTE_1) > 0)
			{
				iLen = (*(char*)(Buffer + iPos));
				iPos += sizeof(char);
			}
			else if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_BYTE_2) > 0)
			{
				iLen = (*(short*)(Buffer + iPos));
				iPos += sizeof(short);
			}
			else if( ((*(char*)(Buffer[iDataHead])) & SM_DATA_BYTE_4) > 0)
			{
				iLen = (*(long*)(Buffer + iPos));
				iPos += sizeof(long);
			}
				
			SetField(FieldName, Buffer + iPos, iLen);
			iPos+=iLen;
		}
	}

	GetStringFieldValue(TAG_S_MSG, FMsg);
	FClientSession = GetIntegerFieldValue(TAG_S_HANDLE);
	if (FDeliveryMode == Delivery_RR )
		GetStringFieldValue(TAG_S_CLIENT_UNIQ, FClientUniqKey);

	Result = true;
	return Result;
}


void TSMessage::Clear()
{
	TRBStringNode* IT;
	
	IT = FFieldList.First;
	if (IT != NULL)
	{
		while (IT != FFieldList.Last)
		{
			if (PFieldData(IT->Data)->MsgFieldType == mfBinary)
				delete PFieldData(IT->Data)->Bin;
			delete IT->Data;
			FFieldList.RBInc(IT);
		}
		if (PFieldData(IT->Data)->MsgFieldType == mfBinary)
			delete PFieldData(IT->Data)->Bin;
		delete (IT->Data);
	}
	//FFieldList.Create;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <TDestinationList>
TDestinationList::TDestinationList()
{

}

TDestinationList::~TDestinationList()
{
}

void* TDestinationList::CreateRecord()
{
	char* pRecordBuffer = new char[sizeof(TDestinationRecord)];
	ZeroMemory(pRecordBuffer, sizeof(TDestinationRecord));
	return (void*)pRecordBuffer;
}


void TDestinationList::DataFree(void* Data)
{
	delete[] Data;
}

char* TDestinationList::GetKey(int Index, void* Data, char* oKey)
{
	TDestinationRecord* p = (TDestinationRecord*)Data;
	if (Index == 0)
		sprintf(oKey, "%s;%s;%d", p->Destination, p->Msg,(int) p->RcvHandle);
	else if (Index == 1)
		sprintf(oKey, "%s", p->Destination);
	else if (Index == 2)
		sprintf(oKey, "%s;%s", p->Destination, p->Msg);

	return oKey;
}

void TDestinationList::InsertData(char* Dest, char* Msg, HWND RcvHandle)
{
	TDestinationRecord* pRecord;

	pRecord = (TDestinationRecord*)CreateRecord();
	strcpy(pRecord->Destination, Dest);
	strcpy(pRecord->Msg , Msg);
	pRecord->RcvHandle = RcvHandle;
	Insert(pRecord);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//<TBinaryData>

TBinaryData::TBinaryData(char* Buffer, int Len)
{
	FLen = Len;
	FBuffer = new char[FLen];
	memcpy(FBuffer, Buffer, FLen);
}

TBinaryData::~TBinaryData()
{
	delete[] FBuffer;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <TSMSConnectThread>


TSMSConnectThread::TSMSConnectThread(TSMClient* smClient)
{
	FUserDisconnect = false;
	FSMClient = smClient;
}

TSMSConnectThread::~TSMSConnectThread()
{
	SAFE_DELETE(FSMClient);
}

VOID TSMSConnectThread::Execute()
{
	char RcvBuffer[MAXLEN_TCPBUFFER];
	char HeadBuffer[sizeof(TSMH_Head) + 1];

	InterlockedIncrement(&FSMClient->FlthreadCnt);
	
	__try
	{
		while (!Is_TimeOfStop(1))
		{
			//todo if (!FSMClient->FTCPClient.Connected)
			//	break;

			memset(HeadBuffer, 0x00, sizeof(HeadBuffer));

			// Head
			//TODO FSMClient.FTCPClient.ReadBuffer(HeadBuffer, sizeof(TSMH_Head));

			//GetMem(RcvBuffer, PSMH_Head(@HeadBuffer)^.DataLen + SizeOf(TSMH_Head));
			memset(RcvBuffer, 0, ((TSMH_Head*)RcvBuffer)->DataLen + sizeof(TSMH_Head));
			memcpy(RcvBuffer, HeadBuffer, sizeof(TSMH_Head));

			// Data Read
			//TODO FSMClient.FTCPClient.ReadBuffer((RcvBuffer + sizeof(TSMH_Head)) ^ , PSMH_Head(RcvBuffer) ^ .DataLen);

			//todo FSMClient->FIFOQueue.Push(RcvBuffer);
			//todo FSMClient->IncRcvCount;
		}
		/*todo
		except
			on E  EIdSocketError do
			begin
			case E.LastError of
			10054 MessageBox(0, PChar(Format('서버와의 접속이 끊어 졌습니다. 에러메세지 %s 가발생 했습니다.', [E.Message])),
				'Server Connect Error', MB_ICONERROR);
		end;
		end;
		on E  EIdException do
			begin
			if Not FUserDisconnect then
				if Pos('Closed Gracefully', E.Message) > 0 then
					MessageBox(0, PChar(Format('서버와의 접속이 끊어 졌습니다. 에러메세지 %s 가발생 했습니다.', [E.Message])),
						'Server Connect Error', MB_ICONERROR);
		end;
				else
					//      raise;
					end;
		*/
	}
	__finally
	{
		//todo FSMClient->FTCPClient.Disconnect;
		InterlockedDecrement(&FSMClient->FlthreadCnt);
	}
}
// <TSMSConnectThread>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <TSMClient>


TSMClient::TSMClient()
{
	FUseNagle = true;
	FMessageFilterHandle = 0;
	//FProcessLog = NULL;
	InitializeCriticalSectionAndSpinCount(&FLock, 2000);
	InitializeCriticalSectionAndSpinCount(&FLogLock, 2000);
	FThreadCount = 1;
	FSendThreadCount = 0;
	FBeginTransaction = 0;
	FEndTransaction = 0;
	FBandWidth = 0;
	FRcvCount = 0;
	//todo FIFOQueue = NULL;
	FSMSConnectThread = NULL;
	FThreadMode = thSingal;
	//TODO FWorkThreadClass = TWorkThread;

	//FDestinationList = TDestinationList.Create;
	FDestinationList.AddIndex("DEST_MSG_HANDLE_INDEX_UNIQ");
	FDestinationList.AddIndex("DEST");
	FDestinationList.AddIndex("DEST_MSG");

	//FWorkThreadList = TList.Create;
	FWindowHandle = AllocateHWnd(WndProc);
	//todo FTCPClient = TIdTCPClient.Create(NULL);

}

TSMClient::~TSMClient()
{
	int I;
	TRBNode* IT;
	//  DestList TList;
	// 	PDestRecord PDestinationRecord;
	
	Disconnect;
	//FDestinationList.Free;
	//todo FTCPClient.Free;
	std::vector<void*>::iterator it;
	for (it = FWorkThreadList.begin(); it != FWorkThreadList.end(); it++)
		delete (*it);
	FWorkThreadList.clear();

	DeleteCriticalSection(&FLock);
	DeleteCriticalSection(&FLogLock);
	//FProcessLog.Free;
	
}

BOOL TSMClient::Connected()
{
	return TRUE;	// FTCPClient.connected;
}

CHAR* TSMClient::ClientIP()
{
	return NULL;	// FTCPClient.Socket.Binding.IP;
}

VOID TSMClient::Disconnect()
{
	int I;
	TDestinationRecord* PDestRecord;
	
	// RR용 Dest 삭제
	PDestRecord = new TDestinationRecord;
	strcpy(PDestRecord->Destination, ClientUniqKey());
	ZeroMemory(PDestRecord->Msg, sizeof(PDestRecord->Msg));
	PDestRecord->RcvHandle  = FWindowHandle;
	//
	SendMessage(FWindowHandle, SM_REMOVE_MSG, WPARAM(PDestRecord), 0);
	//
	StrDispose(PDestRecord^.Destination);
	Dispose(PDestRecord);

	if FSMSConnectThread <> NULL then
		FSMSConnectThread.UserDisconnect  = true; // 정상종료

	FTCPClient.Disconnect;
	if FSMSConnectThread <> NULL then
		FSMSConnectThread.Terminate;

	if FIFOQueue <> NULL then
		FIFOQueue.Finalize;

	for I  = 0 to FWorkThreadList.Count - 1 do
		TWorkThread(FWorkThreadList.Items[I]).Terminate;

	FWorkThreadList.Clear;

	{
		while (0 < FlthreadCnt) do
			Sleep(100);
	}
	for I  = 0 to 10 do
		Sleep(100);

FSMSConnectThread = NULL;

	if FIFOQueue <> NULL then
		FIFOQueue.Free;

FIFOQueue = NULL;
}



HWND TSMClient::AllocateHWnd(WNDPROC  WndMethod)
{
	char sClassName[5];

	sprintf(sClassName, "%d", GetTickCount());

	HWND hWndResult;

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = GetModuleHandle(NULL);
	WndClass.lpfnWndProc = WndMethod;
	WndClass.lpszClassName = sClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);


	hWndResult = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		sClassName,
		"",
		WS_POPUP,
		0,
		0,
		0,
		0,
		0,
		0,
		WndClass.hInstance,
		NULL);


	if (!hWndResult)
	{
		int ErrorCode = GetLastError();
		return NULL;
	}

	SetWindowLong(hWndResult, GWL_WNDPROC, (LONG)WndMethod);

	return hWndResult;
}
void TSMClient::DeallocateHWnd(HWND hWnd)
{
	DestroyWindow(hWnd);
}

WNDPROC TSMClient::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// <TSMClient>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

