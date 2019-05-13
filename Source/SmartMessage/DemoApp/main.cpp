//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "SMDataTypes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmMain *fmMain;

//extern "C" __declspec(dllimport) int _Initialize(void);

bool DLLInitialized = false;
int SelectedInstance = -1;

//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
	: TForm(Owner)
{

}

int __fastcall TfmMain::InitDLL(void)
{

	HMODULE myDLL = LoadLibraryA("SmartMessage.dll");
	if (myDLL <= 0)
	{
		ShowMessage("DLL not found !");
		return -1;
	}

	//ShowMessage(IntToStr((int)sizeof(TFutExec)));

	Initialize = NULL;
	Initialize = (TInitialize)GetProcAddress(myDLL, "Initialize");
	if (Initialize == NULL)
	{
	  ShowMessage("Initialize function not found in the DLL !");
	  return -1;
	}

	SMClientConnect = NULL;
	SMClientConnect = (TSMClientConnect) GetProcAddress(myDLL, "SMClientConnect");
	if (SMClientConnect == NULL)
	{
	  ShowMessage("SMClientConnect function not found in the DLL !");
	  return -1;
	}

	SMClientDisconnect = NULL;
	SMClientDisconnect = (TSMClientDisconnect) GetProcAddress(myDLL, "SMClientDisconnect");
	if (SMClientDisconnect == NULL)
	{
	  ShowMessage("SMClientDisconnect function not found in the DLL !");
	  return -1;
	}

	SMClientIsConnected = NULL;
	SMClientIsConnected = (TSMClientIsConnected) GetProcAddress(myDLL, "SMClientIsConnected");
	if (SMClientIsConnected == NULL)
	{
	  ShowMessage("SMClientIsConnected function not found in the DLL !");
	  return -1;
	}

	SMSetMessageParameters = NULL;
	SMSetMessageParameters = (TSMSetMessageParameters) GetProcAddress(myDLL, "SMSetMessageParameters");
	if (SMSetMessageParameters == NULL)
	{
	  ShowMessage("SMSetMessageParameters function not found in the DLL !");
	  return -1;
	}

	SMSetMessageBinaryField = NULL;
	SMSetMessageBinaryField = (TSMSetMessageBinaryField) GetProcAddress(myDLL, "SMSetMessageBinaryField");
	if (SMSetMessageBinaryField == NULL)
	{
	  ShowMessage("SMSetMessageBinaryField function not found in the DLL !");
	  return -1;
	}

	SMSetMessageStringField = NULL;
	SMSetMessageStringField = (TSMSetMessageStringField) GetProcAddress(myDLL, "SMSetMessageStringField");
	if (SMSetMessageStringField == NULL)
	{
	  ShowMessage("SMSetMessageStringField function not found in the DLL !");
	  return -1;
	}

	SMSetMessageIntegerField = NULL;
	SMSetMessageIntegerField = (TSMSetMessageIntegerField) GetProcAddress(myDLL, "SMSetMessageIntegerField");
	if (SMSetMessageIntegerField == NULL)
	{
	  ShowMessage("SMSetMessageIntegerField function not found in the DLL !");
	  return -1;
	}

	SMSendMessage = NULL;
	SMSendMessage = (TSMSendMessage) GetProcAddress(myDLL, "SMSendMessage");
	if (SMSendMessage == NULL)
	{
	  ShowMessage("SMSendMessage function not found in the DLL !");
	  return -1;
	}

	SMGetClientUniqKey = NULL;
	SMGetClientUniqKey = (TSMGetClientUniqKey) GetProcAddress(myDLL, "SMGetClientUniqKey");
	if (SMGetClientUniqKey == NULL)
	{
	  ShowMessage("SMGetClientUniqKey function not found in the DLL !");
	  return -1;
	}

	SMEventAddDestination = NULL;
	SMEventAddDestination = (TSMEventAddDestination) GetProcAddress(myDLL, "SMEventAddDestination");
	if (SMEventAddDestination == NULL)
	{
	  ShowMessage("SMEventAddDestination function not found in the DLL !");
	  return -1;
	}

	SMGetReceivedCnt = NULL;
	SMGetReceivedCnt = (TSMGetReceivedCnt) GetProcAddress(myDLL, "SMGetReceivedCnt");
	if (SMGetReceivedCnt == NULL)
	{
	  ShowMessage("SMGetReceivedCnt function not found in the DLL !");
	  return -1;
	}

	SMSetWorkEventCallBack = NULL;
	SMSetWorkEventCallBack = (TSMSetWorkEventCallBack) GetProcAddress(myDLL, "SMSetWorkEventCallBack");
	if (SMSetWorkEventCallBack == NULL)
	{
	  ShowMessage("SMSetWorkEventCallBack function not found in the DLL !");
	  return -1;
	}

	SMMessageGetBinaryFieldValue = NULL;
	SMMessageGetBinaryFieldValue = (TSMMessageGetBinaryFieldValue) GetProcAddress(myDLL, "SMMessageGetBinaryFieldValue");
	if (SMMessageGetBinaryFieldValue == NULL)
	{
	  ShowMessage("SMMessageGetBinaryFieldValue function not found in the DLL !");
	  return -1;
	}

	SMMessageGetStringFieldValue = NULL;
	SMMessageGetStringFieldValue = (TSMMessageGetStringFieldValue) GetProcAddress(myDLL, "SMMessageGetStringFieldValue");
	if (SMMessageGetStringFieldValue == NULL)
	{
	  ShowMessage("SMMessageGetStringFieldValue function not found in the DLL !");
	  return -1;
	}

	SMMessageGetIntegerFieldValue = NULL;
	SMMessageGetIntegerFieldValue = (TSMMessageGetIntegerFieldValue) GetProcAddress(myDLL, "SMMessageGetIntegerFieldValue");
	if (SMMessageGetIntegerFieldValue == NULL)
	{
	  ShowMessage("SMMessageGetIntegerFieldValue function not found in the DLL !");
	  return -1;
	}

	SMGetObjectsNumber = NULL;
	SMGetObjectsNumber = (TSMGetObjectsNumber) GetProcAddress(myDLL, "SMGetObjectsNumber");
	if (SMGetObjectsNumber == NULL)
	{
	  ShowMessage("SMGetObjectsNumber function not found in the DLL !");
	  return -1;
	}

	SMGetMaximumObjectsNumber = NULL;
	SMGetMaximumObjectsNumber = (TSMGetMaximumObjectsNumber) GetProcAddress(myDLL, "SMGetMaximumObjectsNumber");
	if (SMGetMaximumObjectsNumber == NULL)
	{
	  ShowMessage("SMGetMaximumObjectsNumber function not found in the DLL !");
	  return -1;
	}

	SMCreateInstance = NULL;
	SMCreateInstance = (TSMCreateInstance) GetProcAddress(myDLL, "SMCreateInstance");
	if (SMCreateInstance == NULL)
	{
	  ShowMessage("SMCreateInstance function not found in the DLL !");
	  return -1;
	}

	SMGetClientIP = NULL;
	SMGetClientIP = (TSMGetClientIP) GetProcAddress(myDLL, "SMGetClientIP");
	if (SMGetClientIP == NULL)
	{
	  ShowMessage("SMGetClientIP function not found in the DLL !");
	  return -1;
	}

	SMEventRemoveDestination = NULL;
	SMEventRemoveDestination = (TSMEventRemoveDestination) GetProcAddress(myDLL, "SMEventRemoveDestination");
	if (SMEventRemoveDestination == NULL)
	{
	  ShowMessage("SMEventRemoveDestination function not found in the DLL !");
	  return -1;
	}

	SMEventAllRemoveDestination = NULL;
	SMEventAllRemoveDestination = (TSMEventAllRemoveDestination) GetProcAddress(myDLL, "SMEventAllRemoveDestination");
	if (SMEventAllRemoveDestination == NULL)
	{
	  ShowMessage("SMEventAllRemoveDestination function not found in the DLL !");
	  return -1;
	}

	SMMessageGetDeliveryType = NULL;
	SMMessageGetDeliveryType = (TSMMessageGetDeliveryType) GetProcAddress(myDLL, "SMMessageGetDeliveryType");
	if (SMMessageGetDeliveryType == NULL)
	{
	  ShowMessage("SMMessageGetDeliveryType function not found in the DLL !");
	  return -1;
	}

	SMSendResponse = NULL;
	SMSendResponse = (TSMSendResponse) GetProcAddress(myDLL, "SMSendResponse");
	if (SMSendResponse == NULL)
	{
	  ShowMessage("SMSendResponse function not found in the DLL !");
	  return -1;
	}


	long int resi;
	resi = Initialize();
	DLLInitialized = (resi == 0);
	return resi;

}


//---------------------------------------------------------------------------

void __fastcall TfmMain::Timer1Timer(TObject *Sender)
{
	if (!DLLInitialized)
	{
		Button12->Enabled = false;
		Button13->Enabled = false;
		Button14->Enabled = false;
		Button15->Enabled = false;
		Button16->Enabled = false;
		Button17->Enabled = false;
		Button18->Enabled = false;
		Button19->Enabled = false;
		Button20->Enabled = false;
		Button21->Enabled = false;
		Button11->Enabled = false;
		Button12->Font->Style = TFontStyles() >> fsBold;
		Button13->Font->Style = TFontStyles() >> fsBold;
		Button14->Font->Style = TFontStyles() >> fsBold;
		Button15->Font->Style = TFontStyles() >> fsBold;
		Button16->Font->Style = TFontStyles() >> fsBold;
		Button17->Font->Style = TFontStyles() >> fsBold;
		Button18->Font->Style = TFontStyles() >> fsBold;
		Button19->Font->Style = TFontStyles() >> fsBold;
		Button20->Font->Style = TFontStyles() >> fsBold;
		Button21->Font->Style = TFontStyles() >> fsBold;
	}

	if (!DLLInitialized) return;

	int objnumber = SMGetObjectsNumber();
	//int maxobjnumber =
	Button11->Enabled = DLLInitialized;

	Button12->Enabled = DLLInitialized && (Button12->Tag < objnumber);
	Button13->Enabled = DLLInitialized && (Button13->Tag < objnumber);
	Button14->Enabled = DLLInitialized && (Button14->Tag < objnumber);
	Button15->Enabled = DLLInitialized && (Button15->Tag < objnumber);
	Button16->Enabled = DLLInitialized && (Button16->Tag < objnumber);
	Button17->Enabled = DLLInitialized && (Button17->Tag < objnumber);
	Button18->Enabled = DLLInitialized && (Button18->Tag < objnumber);
	Button19->Enabled = DLLInitialized && (Button19->Tag < objnumber);
	Button20->Enabled = DLLInitialized && (Button20->Tag < objnumber);
	Button21->Enabled = DLLInitialized && (Button21->Tag < objnumber);

	Button12->Font->Style = TFontStyles() >> fsBold;
	Button13->Font->Style = TFontStyles() >> fsBold;
	Button14->Font->Style = TFontStyles() >> fsBold;
	Button15->Font->Style = TFontStyles() >> fsBold;
	Button16->Font->Style = TFontStyles() >> fsBold;
	Button17->Font->Style = TFontStyles() >> fsBold;
	Button18->Font->Style = TFontStyles() >> fsBold;
	Button19->Font->Style = TFontStyles() >> fsBold;
	Button20->Font->Style = TFontStyles() >> fsBold;
	Button21->Font->Style = TFontStyles() >> fsBold;

	if (SelectedInstance == 0)  { Button12->Font->Style = TFontStyles() << fsBold; }
	if (SelectedInstance == 1)  { Button13->Font->Style = TFontStyles() << fsBold; }
	if (SelectedInstance == 2)  { Button14->Font->Style = TFontStyles() << fsBold; }
	if (SelectedInstance == 3)  { Button15->Font->Style = TFontStyles() << fsBold; }
	if (SelectedInstance == 4)  { Button16->Font->Style = TFontStyles() << fsBold; }
	if (SelectedInstance == 5)  { Button17->Font->Style = TFontStyles() << fsBold; }
	if (SelectedInstance == 6)  { Button18->Font->Style = TFontStyles() << fsBold; }
	if (SelectedInstance == 7)  { Button19->Font->Style = TFontStyles() << fsBold; }
	if (SelectedInstance == 8)  { Button20->Font->Style = TFontStyles() << fsBold; }
	if (SelectedInstance == 9)  { Button21->Font->Style = TFontStyles() << fsBold; }


	/*bool resb = SMClientIsConnected();
	if (resb)
	{
	  lblConnectionState->Caption = "Connection state : CONNECTED";
	  lblReceived->Caption = "Received packets : " + IntToStr((int) SMGetReceivedCnt());
	}
	else
	{
	  lblConnectionState->Caption = "Connection state : DISCONNECTED";
	}           */

	lblObjNumber->Caption = "Objects number : " + IntToStr((int) SMGetObjectsNumber());
	lblMaxObjNumber->Caption = "Maximum objects number : " + IntToStr((int) SMGetMaximumObjectsNumber());

	lblSelectedInstance->Caption = "Selected object index : " + IntToStr((int) SelectedInstance);

	if (SelectedInstance >= 0)
	{
	  bool isconn = SMClientIsConnected(SelectedInstance);
	  if (isconn)
	  lblConnState->Caption = "Connection state : CONNECTED";
	  else
	  lblConnState->Caption = "Connection state : DISCONNECTED";

	  edUnKey->Text = SMGetClientUniqKey(SelectedInstance);
	  if (isconn) edClientIP->Text = SMGetClientIP(SelectedInstance);
	  else edClientIP->Text = "";
	}


}
//---------------------------------------------------------------------------

long int __stdcall CallBackProc(int index, char* WorkThread, char* Message)
{
	char* fv;
	char *p;
	int resint;
	//ShowMessage("Our CallBack func !");

	fv = SMMessageGetBinaryFieldValue(index, RECEIVED_MESSAGE, (char * )fldFXExec);
	TFutExec FutExec;
	memcpy(&FutExec, fv, sizeof(FutExec));
	//ShowMessage(FutExec.issue);

	int delivmode = SMMessageGetDeliveryType(index, RECEIVED_MESSAGE);

	//ShowMessage(FutExec.issue);
	fmMain->ListBox2->Items->Add("Message received via instance " + IntToStr(index) + " !");
	fmMain->ListBox2->Items->Add("Message Delivery Type : " + IntToStr(delivmode));
	fmMain->ListBox2->Items->Add("----- BINARY FIELD -----");
	fmMain->ListBox2->Items->Add("ISSUE : " + String(FutExec.issue));
	fmMain->ListBox2->Items->Add("TIME : " + IntToStr(FutExec.time));

	p = SMMessageGetStringFieldValue(index, RECEIVED_MESSAGE, (char * ) fldHDMsg);
	fmMain->ListBox2->Items->Add("STRING FIELD : " + AnsiString(p,strlen(p)+1));

	resint = SMMessageGetIntegerFieldValue(index, RECEIVED_MESSAGE, (char * ) fldHDUserID);
    //resint = 100;
	fmMain->ListBox2->Items->Add("INTEGER FIELD : " + IntToStr(resint));

	return 0;
}




void __fastcall TfmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
   //SMClientDisconnect();
   //SMSetWorkEventCallBack(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button9Click(TObject *Sender)
{
	int resi = InitDLL();
	edInitRes2->Text = IntToStr((int)resi);
	if (resi == 0)
	{
		Button11Click(NULL);
		Button11Click(NULL);
	}

}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button11Click(TObject *Sender)
{
	int resi = SMCreateInstance();
	edCreateInst->Text = IntToStr((int)resi);
	if (resi == 1) SelectedInstance = 0;

}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button12Click(TObject *Sender)
{
   TButton* button = dynamic_cast<TButton*>(Sender);
   SelectedInstance = button->Tag;
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button22Click(TObject *Sender)
{
	int resi;
	edConnRes->Text = "";
    Application->ProcessMessages();
	resi = SMClientConnect(SelectedInstance, "110.4.89.206", (long int) 7789);
	edConnRes->Text = IntToStr((int)resi);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button23Click(TObject *Sender)
{
	int resi;
	edDisconnRes->Text = "";
	Application->ProcessMessages();
	resi = SMClientDisconnect(SelectedInstance);
	edDisconnRes->Text = IntToStr((int)resi);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button24Click(TObject *Sender)
{
	int resi;
	resi = SMSetMessageParameters(SelectedInstance, MESSAGE_TO_SEND, Delivery_Push,(char *) SISE_GW,(char *) MSG_MKT_FX_EXEC);
	edSetMess->Text = IntToStr((int)resi);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button25Click(TObject *Sender)
{
	int resi;
	//resi = SMAddDestination((char * )SISE_GW, (char * )MSG_MKT_FX_HOGA);
	resi = SMEventAddDestination(SelectedInstance, (char * )SISE_GW, (char * )MSG_MKT_FX_EXEC);
	//ShowMessage("SMAddDestination function return : " + IntToStr((int)resi));
	edSMAddDest->Text = IntToStr((int)resi);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button26Click(TObject *Sender)
{
	TFutExec FutExec;
	memset(&FutExec, 0, sizeof(FutExec));
	strcpy((char *) &(FutExec.issue), "CLM19");
	FutExec.gap = 1.5;
	FutExec.cup  = 89.12;   // close
	FutExec.sip  = 88.72;  // open
	FutExec.hip  = 89.76;   // high
	FutExec.lip  = 88.09;    // log
	FutExec.vol  = 876;
	strcpy((char *) &(FutExec.ydiffSign), "1");
	strcpy((char *) &(FutExec.chgrate), "1.2");
	FutExec.amt = 0.0;
	TDateTime DT = Now();
	FutExec.time = StrToInt(FormatDateTime("hhnnss", DT));
	strcpy((char *) &FutExec.side, fvSide_BUY);

	int resi;
	resi = SMSetMessageBinaryField(SelectedInstance, MESSAGE_TO_SEND, (char * )fldFXExec,(char *) &FutExec, (int) sizeof(FutExec));
	resi = SMSetMessageStringField(SelectedInstance, MESSAGE_TO_SEND, (char * )fldHDMsg,(char *) "GETRESULT");
	resi = SMSetMessageIntegerField(SelectedInstance, MESSAGE_TO_SEND, (char * )fldHDUserID, 2);

//fldHDUserID

	//ShowMessage("SMSetMessageField function return : " + IntToStr((int)resi));
	edSetMessageField2->Text = IntToStr((int)resi);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button27Click(TObject *Sender)
{
	int resi = SMSendMessage(SelectedInstance, MESSAGE_TO_SEND);
	//ShowMessage("SMSendMessage function return : " + IntToStr((int)resi));
	edSendMessage2->Text = IntToStr((int)resi);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button28Click(TObject *Sender)
{
	int resi;
	resi = SMSetWorkEventCallBack(SelectedInstance, (char *)CallBackProc);
	edCallBack2->Text = IntToStr((int)resi);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button1Click(TObject *Sender)
{
	int resi;
	resi = SMEventRemoveDestination(SelectedInstance, (char * )SISE_GW, (char * )MSG_MKT_FX_EXEC);
	edSMRemoveDest->Text = IntToStr((int)resi);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button2Click(TObject *Sender)
{
	int resi;
	resi = SMEventAllRemoveDestination(SelectedInstance);
	edSMAllRemoveDest->Text = IntToStr((int)resi);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Button3Click(TObject *Sender)
{
ListBox2->Clear();
}
//---------------------------------------------------------------------------

