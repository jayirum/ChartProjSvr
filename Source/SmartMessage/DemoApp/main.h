//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Samples.Spin.hpp>
//---------------------------------------------------------------------------
class TfmMain : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTimer *Timer1;
	TTabSheet *TabSheet2;
	TButton *Button9;
	TEdit *edInitRes2;
	TLabel *lblObjNumber;
	TLabel *lblMaxObjNumber;
	TButton *Button11;
	TEdit *edCreateInst;
	TLabel *lblSelectedInstance;
	TButton *Button12;
	TButton *Button13;
	TButton *Button14;
	TButton *Button15;
	TButton *Button16;
	TButton *Button17;
	TButton *Button18;
	TButton *Button19;
	TButton *Button20;
	TButton *Button21;
	TLabel *lblConnState;
	TButton *Button22;
	TEdit *edConnRes;
	TEdit *edUnKey;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TButton *Button23;
	TEdit *edDisconnRes;
	TLabel *Label5;
	TGroupBox *GroupBox1;
	TButton *Button24;
	TLabel *Label6;
	TEdit *edSetMess;
	TButton *Button25;
	TLabel *Label7;
	TEdit *edSMAddDest;
	TGroupBox *GroupBox2;
	TButton *Button26;
	TEdit *edSetMessageField2;
	TLabel *Label8;
	TButton *Button27;
	TEdit *edSendMessage2;
	TLabel *Label9;
	TButton *Button28;
	TLabel *Label10;
	TEdit *edCallBack2;
	TListBox *ListBox2;
	TLabel *Label11;
	TEdit *edClientIP;
	TButton *Button1;
	TLabel *Label12;
	TEdit *edSMRemoveDest;
	TButton *Button2;
	TLabel *Label13;
	TEdit *edSMAllRemoveDest;
	TButton *Button3;
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	void __fastcall Button12Click(TObject *Sender);
	void __fastcall Button22Click(TObject *Sender);
	void __fastcall Button23Click(TObject *Sender);
	void __fastcall Button24Click(TObject *Sender);
	void __fastcall Button25Click(TObject *Sender);
	void __fastcall Button26Click(TObject *Sender);
	void __fastcall Button27Click(TObject *Sender);
	void __fastcall Button28Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfmMain(TComponent* Owner);
    int __fastcall TfmMain::InitDLL(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
