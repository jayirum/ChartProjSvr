#ifndef _GUI_CNFG_SLAVE_H
#define _GUI_CNFG_SLAVE_H

/*
    [BACKGROUND]
X=0
Y=0
W=400
H=700

    [LOTSIZE]
TYPE=1  //1-AUTO SCALING, 2-FIXED
SCALING_FACTOR=0.5

    [MASTERINFO]
MASTERID=JAYKIM

    [PRICE]
MAX_SLIPPAGE=50 //POINTS

    [SYMBOLS]
EURUSD=EURUSD   //MASTER-SLAVE

    [CHANNEL_INFO]
IP=110.4.89.206
READ_PORT=64999
MASTER_SUB_PORT=64998
SLAVE_SUB_PORT=64997

    
    
1) TRADE or JUST SIGNAL
2) MAX SLIPPAGE (POINTS)
3) LOT SIZE - AUTO SCALING / FIXED
   SCALING FACTOR 
4) MASTER ID 
5) MAX SLIPPAGE    
*/


#include "../mt4gui2.mqh"
#include "../BAUtils.mqh"

#define MAX_HANDLES 30
string  BTN_SAVE    = "Save";
string  BTN_CANCEL  = "Cancel";

struct BASIC
{
    int pnlX;
    int objX;
    int pnlW;
    int pnlHeaderH;
    int currHeaderY;
};


struct GUI_HANDLES
{
    int nTotalGui;
    int Handles[MAX_HANDLES];
};


struct CNFG_VALUE
{
    bool    bTrade;
    bool    bAutoScale;
    double  dAutoScaleVal;
    double  dFixedLotVal;
    bool    bMaxLot;
    double  dMaxLot;
    bool    bMaxSlippage;
    double  dMaxSlippage;
};


class CGUICnfgSlave
{
public:
    CGUICnfgSlave(string sMasterId);
    ~CGUICnfgSlave();
    
    
    bool    LoadChannelInfo();
    bool    LoadGUI();
    void    CheckGUI();
    void    EraseAll();
    bool    IsClosedGUI()   { return (m_btnShowUI>-1);}
    
    string  SendChannel(){ return m_sSendChannel;}
    string  RecvChannel(){ return m_sRecvChannel;}
    
    //+--------------------------------------------
    bool    IsAutoScaleYN()         { return guiIsChecked(m_hwnd, m_rdoLotsFixed); }
    long    LotsAutoScaleValue()    { return StringToInteger(guiGetText(m_hwnd, m_edtLotsScale)); }       
    long    LotsFixedValue()        { return StringToInteger(guiGetText(m_hwnd, m_edtLotsFixed)); }       
    long    MaxLotSize()            { return StringToInteger(guiGetText(m_hwnd, m_edtMaxLotSize)); }       
private:

    void    GUIShowButton(bool bShow);

    void    SetBasicPos();
    bool    LoadTradeSignalCnfg();
    bool    LoadLotsCnfg();
    bool    LoadMaxSlippage();
    void    LoadSaveCancelBtn();
    
    void    Save();
    void    Cancel();
    
    void    ClearHandles();
    
    void    AddHandle(int handle);
    void    PrintCnfgErr(string sSection, string sKey);
    void    DrawLine(int nY);
    void    DrawHeader(string sCaption);
    
    bool    GetCnfgValue(string sSection, string sKey, string& sValue);
    bool    SetCnfgValue(string sSection, string sKey, string sValue);
private:

    string  m_sCnfgPath;
    string  m_sCnfgFile;
    string  m_sCnfgFull;
    char    m_zCnfgFull[128];
    
    string  m_sSendChannel;
    string  m_sRecvChannel;
    
    string  m_sMasterId;
    
    int     m_hwnd;
    
    BASIC   m_basic;
    
    //int     m_pnlLots;
    int     m_rdoLotsFixed, m_edtLotsFixed;
    int     m_rdoLotsScale, m_edtLotsScale;
    int     m_chkMaxLotSize, m_edtMaxLotSize;
    
    int     m_rdoTrade, m_rdoSignal;
    
    int     m_chkMaxSlippage, m_edtMaxSlippage;
    
    int     m_btnSave, m_btnCancel;
    
    int     m_btnShowUI;
    
    GUI_HANDLES m_guiHandles;
    
   CNFG_VALUE   m_cnfgVal;
};



CGUICnfgSlave::CGUICnfgSlave(string sMasterId)
{
    m_hwnd = -1;
    ArrayInitialize(m_guiHandles.Handles, -1);
    
    m_sMasterId = sMasterId;
    
    m_sCnfgPath = TerminalInfoString(TERMINAL_DATA_PATH) + "\\MQL4\\Files";
    m_sCnfgFile = WindowExpertName()+".cnfg";
    m_sCnfgFull = m_sCnfgPath+"/"+m_sCnfgFile;
    StringToCharArray(m_sCnfgFull, m_zCnfgFull);
    
    m_btnShowUI = -1;
    
    ZeroMemory(m_cnfgVal);
}
 
CGUICnfgSlave::~CGUICnfgSlave()
{
    //EraseAll();
}


void CGUICnfgSlave::ClearHandles()
{
    for( int i=0; i<MAX_HANDLES; i++ )
    {
        m_guiHandles.Handles[i] = -1;
    }
    m_guiHandles.nTotalGui = 0;
    
    ZeroMemory(m_cnfgVal);
}


//TODO. 
//void CGUICnfgSlave::SetCnfgValue(void)
//{
//    if( guiIsChecked(m_hwnd, m_rdoTrade) )
//        m_cnfgVal.bTrade = "Y")? true:false;
//}

void CGUICnfgSlave::Save(void)
{
    printlog("Save......");
    
    string sSection, sKey, sValue;
    
    //+----------------------------------------------------
    // TRADESIGNAL
    sSection    = "TRADESIGNAL";
    sKey        = "TRADE_YN";
    
    sValue = "N";
    if( guiIsChecked(m_hwnd, m_rdoTrade) )
        sValue = "Y";
    
    SetCnfgValue(sSection, sKey, sValue);    
    //+----------------------------------------------------
     
    //+----------------------------------------------------
    // LOTSIZE
    string sLotSize;
    sSection    = "LOTSIZE";
    sKey        = "TYPE";
    
    sValue = "A";
    if( guiIsChecked(m_hwnd, m_rdoLotsFixed) )
        sValue = "F";
    
    m_cnfgVal.bAutoScale = (sValue=="A")? true:false;
    
    SetCnfgValue(sSection, sKey, sValue);
    
    sLotSize = guiGetText(m_hwnd, m_edtLotsScale);
    SetCnfgValue(sSection, "VALUE_SCALING", sLotSize);
    m_cnfgVal.dAutoScaleVal = StringToDouble(sLotSize);
    
    sLotSize = guiGetText(m_hwnd, m_edtLotsFixed);
    SetCnfgValue(sSection, "VALUE_FIXED", sLotSize);
    m_cnfgVal.dFixedLotVal = StringToDouble(sLotSize);
    
    // max size
    sValue = "N";
    if( guiIsChecked(m_hwnd, m_chkMaxLotSize) )
        sValue = "Y";
    
    m_cnfgVal.bMaxLot = (sValue=="Y")? true:false;
    SetCnfgValue(sSection, "USE_MAXLOT_YN", sValue);
    
    sLotSize = guiGetText(m_hwnd, m_edtMaxLotSize);
    SetCnfgValue(sSection, "VALUE_MAXLOT", sLotSize);
    m_cnfgVal.dMaxLot = StringToDouble(sLotSize);
 
    //+----------------------------------------------------


    //+----------------------------------------------------
    // MAX SLIPPAGE
    sSection    = "PRICE";
    sKey        = "USE_MAX_SLIPPAGE_YN";
    
    sValue = "N";
    if( guiIsChecked(m_hwnd, m_chkMaxSlippage) )
        sValue = "Y";
    
    m_cnfgVal.bMaxSlippage = (sValue=="Y")? true:false;
    SetCnfgValue(sSection, sKey, sValue);
    
    string sSlippage = guiGetText(m_hwnd, m_edtMaxSlippage);
    SetCnfgValue(sSection, "MAX_SLIPPAGE", sSlippage);
    m_cnfgVal.dMaxSlippage = StringToDouble(sLotSize);
    //+----------------------------------------------------
    
    EraseAll();
}


void CGUICnfgSlave::EraseAll()
{
    if(m_hwnd>-1)
        guiRemoveAll(m_hwnd);
        
    ObjectsDeleteAll();
    
    m_hwnd = -1;
    ClearHandles();
    
    GUIShowButton(true);
}


void CGUICnfgSlave::GUIShowButton(bool bShow)
{
    if(m_hwnd==-1)
    {
        m_hwnd = WindowHandle(Symbol(),Period());    
        if(m_hwnd==0){
            printlog("WindowHandle error", true);
            return ;
        }
    }
    
    if(bShow){
        m_btnShowUI = guiAdd( m_hwnd, "button", 0, 0, 300, 50, "Show & Set Configuration");
    }
    else{
        guiRemove(m_hwnd, m_btnShowUI);
        m_btnShowUI = -1;
    }
        
}

void CGUICnfgSlave::PrintCnfgErr(string sSection, string sKey)
{
    printlog(
        StringFormat("Failed to get config [%s][SECTION:%s][KEY:%s]", m_sCnfgFile, sSection, sKey)
        , true
        ); 
}


bool  CGUICnfgSlave::GetCnfgValue(string sSection, string sKey, string& sValue)
{
    char zSection[32], zKey[32], zValue[32];
    
    StringToCharArray(sSection, zSection);
    StringToCharArray(sKey, zKey);
    
    if(!BAUtils_GetConfig(m_zCnfgFull, zSection, zKey, zValue)){
        PrintCnfgErr( CharArrayToString(zSection), CharArrayToString(zKey));    return false;
    }
    if( GetCharLen(zValue)==0 ) {
        PrintCnfgErr( CharArrayToString(zSection), CharArrayToString(zKey));    return false;
    }
    
    sValue = CharArrayToString(zValue);
    return true;
}



bool  CGUICnfgSlave::SetCnfgValue(string sSection, string sKey, string sValue)
{
    char zSection[32], zKey[32], zValue[32];
    
    StringToCharArray(sSection, zSection);
    StringToCharArray(sKey, zKey);
    StringToCharArray(sValue, zValue);
    
    if(!BAUtils_SetConfig(m_zCnfgFull, zSection, zKey, zValue)){
        PrintCnfgErr( CharArrayToString(zSection), CharArrayToString(zKey));    
        return false;
    }
    
    return true;
}


/*
IP=110.4.89.206
READ_PORT=64999
MASTER_SUB_PORT=64998
SLAVE_SUB_PORT=64997

get from DB or WEB
*/
bool CGUICnfgSlave::LoadChannelInfo()
{    
    string sIp, sPort;
    
    if( !GetCnfgValue("CHANNEL_INFO", "IP", sIp) )          return false;
    if( !GetCnfgValue("CHANNEL_INFO", "READ_PORT", sPort) ) return false;
    
    m_sSendChannel = sIp + ":" + sPort;
    
    
    //+----------------------------------------------------------------
    
    //+----------------------------------------------------------------
    if( !GetCnfgValue("CHANNEL_INFO", "SLAVE_SUB_PORT", sPort) )   return false;
    
    m_sRecvChannel = sIp + ":" + sPort;
    
    printlog( StringFormat("[CHANNEL_INFO](Send Channel:%s)(Read Channel:%s)",m_sSendChannel, m_sRecvChannel ));
    
    return true;
}


void CGUICnfgSlave::CheckGUI()
{
    if(m_hwnd<0)    return;
    
    for ( int i=0; i<m_guiHandles.nTotalGui; i++ )
    {
        if( guiIsClicked(m_hwnd, m_guiHandles.Handles[i]) )
        {
            if( guiGetText(m_hwnd, m_guiHandles.Handles[i])==BTN_SAVE )
            {
                Save();
            }
            if( guiGetText(m_hwnd, m_guiHandles.Handles[i])==BTN_CANCEL )
            {
                Cancel();
            }
        }
    }
    
    if( guiIsClicked(m_hwnd, m_btnShowUI) )
    {
        LoadGUI();
    }
}


void CGUICnfgSlave::Cancel(void)
{
    printlog("Close......");
    
    EraseAll();
}

    
bool CGUICnfgSlave::LoadGUI()
{
    m_hwnd = WindowHandle(Symbol(),Period());    
    if(m_hwnd==0){
        printlog("WindowHandle error", true);
        return false;
    }
    
    ClearHandles();
    
    GUIShowButton(false);
    
    SetBasicPos();
    
    LoadTradeSignalCnfg();
    
    LoadLotsCnfg();
    
    LoadMaxSlippage();
    
    LoadSaveCancelBtn();
    
    return true;
}


void CGUICnfgSlave::SetBasicPos()
{
    m_basic.pnlX = 0;
    m_basic.objX = 20;
    m_basic.pnlW = 660;
    m_basic.pnlHeaderH = 25;
    m_basic.currHeaderY = 0;
}


bool CGUICnfgSlave::LoadTradeSignalCnfg()
{
    DrawHeader("Copy Trade or Just Show Signal");
    
    
    int h = 50;
    int headerBody = guiAdd(m_hwnd,"label", m_basic.pnlX, m_basic.currHeaderY, m_basic.pnlW, h, "");
    
    int nRdoY = m_basic.currHeaderY + 10;
    
    m_basic.currHeaderY += h;
    
     
    int nRdoWidth   = 150;
    int nRdoH       = 30;

    guiGroupRadio(m_hwnd);  // m_rdoTrade, m_rdoSignal;

    // trade    
    m_rdoTrade = guiAdd(m_hwnd, "radio", m_basic.objX, nRdoY, nRdoWidth, nRdoH, "Copy Trade");
    AddHandle(m_rdoLotsFixed);
    
    m_rdoSignal = guiAdd(m_hwnd, "radio", m_basic.objX+nRdoWidth, nRdoY, nRdoWidth, nRdoH, "Just Signal");
    AddHandle(m_rdoLotsFixed);
    
    
    string sValue;
    if( !GetCnfgValue("TRADESIGNAL", "TRADE_YN", sValue) )
        sValue = "N";

    if( sValue=="Y" ){
        guiSetChecked(m_hwnd, m_rdoTrade, true);
        guiSetChecked(m_hwnd, m_rdoSignal, false);
    }
    else{
        guiSetChecked(m_hwnd, m_rdoTrade, false);
        guiSetChecked(m_hwnd, m_rdoSignal, true);
    }
    
    
    return true;    
}

/*
int     m_rdoLotsFixed, m_edtLotsFixed;
int     m_rdoLotsScale, m_edtLotsScale;
*/
bool CGUICnfgSlave::LoadLotsCnfg()
{

    DrawHeader("Order LotSize Options");

    int pnlBodyH = 150;
    int PannelBody = guiAdd(m_hwnd,"label", m_basic.pnlX, m_basic.currHeaderY, m_basic.pnlW, pnlBodyH, "");
    
    int nCurrY      = m_basic.currHeaderY + 20;
    int nRdoWidth   = 150;
    int nRdoH       = 30;
    int nDescPnlPos = 0;
    
    m_basic.currHeaderY += pnlBodyH;

    guiGroupRadio(m_hwnd);

    //+---------------------------------------------------------------------------------------------------
    // scale
    m_rdoLotsScale = guiAdd(m_hwnd, "radio", m_basic.objX, nCurrY, nRdoWidth, 30, "Scale Up/Down");
    AddHandle(m_rdoLotsScale);
    
    m_edtLotsScale = guiAdd(m_hwnd, "text", m_basic.objX+nRdoWidth+10, nCurrY, nRdoWidth, 30, "");
    guiSetBorderColor(m_hwnd, m_edtLotsScale, clrBlack);
    AddHandle(m_edtLotsScale);
    
    nDescPnlPos = m_basic.objX + nRdoWidth + nRdoWidth + 10 + 20;
    guiAdd(m_hwnd,"label", nDescPnlPos, nCurrY, 250, nRdoH, "scaling factor (0.1, 0.5, 1, 5 ...)"); 
    //+---------------------------------------------------------------------------------------------------
    
    nCurrY += nRdoH+5;
    
    //+---------------------------------------------------------------------------------------------------
    // fixed    
    m_rdoLotsFixed = guiAdd(m_hwnd, "radio", m_basic.objX, nCurrY, nRdoWidth, nRdoH, "Fixed LotSize");
    AddHandle(m_rdoLotsFixed);
    
    m_edtLotsFixed = guiAdd(m_hwnd, "text", m_basic.objX+nRdoWidth+10, nCurrY, nRdoWidth, nRdoH, "");
    guiSetBorderColor(m_hwnd, m_edtLotsFixed, clrBlack);
    AddHandle(m_edtLotsFixed);
    
    nDescPnlPos = m_basic.objX + nRdoWidth + nRdoWidth + 10 + 20;
    
    // desc
    guiAdd(m_hwnd,"label", nDescPnlPos, nCurrY, 200, nRdoH, "lot size (0.01, 0.5, 1, 5 ...)"); 
    //+---------------------------------------------------------------------------------------------------
    
    string sType, sValFixed, sValScaling, sValue;
    if( !GetCnfgValue("LOTSIZE", "TYPE", sValue) )
        sValue = "A";    // auto scaling

    sType = StringSubstr(sValue,0,1);
    
    // auto scaling
    if( sType == "A" )
    {
        if( !GetCnfgValue("LOTSIZE", "VALUE_SCALING", sValScaling) ){
            sValScaling = "";
        }
        
        guiSetChecked(m_hwnd, m_rdoLotsScale, true);
        guiSetChecked(m_hwnd, m_rdoLotsFixed, false);
        guiSetText(m_hwnd, m_edtLotsScale, sValScaling, 0, "");
    }
    
    // fixed
    if( sType=="F" ){
        if( !GetCnfgValue("LOTSIZE", "VALUE_FIXED", sValFixed) )
            sValFixed = "";    

        guiSetChecked(m_hwnd, m_rdoLotsScale, false);
        guiSetChecked(m_hwnd, m_rdoLotsFixed, true);
        guiSetText(m_hwnd, m_edtLotsFixed, sValFixed, 0, "");
    }

    nCurrY += 30+15;
    
    
    //+---------------------------------------------------------------------------------------------------
    DrawLine(nCurrY);

    nCurrY += 5;

    // max lotsize
    m_chkMaxLotSize = guiAdd(m_hwnd, "checkbox", m_basic.objX, nCurrY, nRdoWidth, 30, "Max LotSize");
    AddHandle(m_chkMaxLotSize);

    m_edtMaxLotSize = guiAdd(m_hwnd, "text", m_basic.objX+nRdoWidth+10, nCurrY, nRdoWidth, 30, "");
    guiSetBorderColor(m_hwnd, m_edtMaxLotSize, clrBlack);
    AddHandle(m_edtMaxLotSize);
    
    nDescPnlPos = m_basic.objX+nRdoWidth+nRdoWidth+10+20; 
    guiAdd(m_hwnd,"label", nDescPnlPos, nCurrY, 250, nRdoH, "lot size (0.01, 0.5, 1, 5 ...)"); 
    
    string sUseMaxLotYN, sValLots;
    if( !GetCnfgValue("LOTSIZE", "USE_MAXLOT_YN", sUseMaxLotYN) ){
        sUseMaxLotYN = "N";
    }

    if( sUseMaxLotYN=="Y" )
    {
        guiSetChecked(m_hwnd, m_chkMaxLotSize, true);
        
        if( !GetCnfgValue("LOTSIZE", "VALUE_MAXLOT", sValLots) )
            sValLots = "";
            
        guiSetText(m_hwnd, m_edtMaxLotSize, sValLots, 0, "");
    }
    //+---------------------------------------------------------------------------------------------------
    
    return true;
}

bool CGUICnfgSlave::LoadMaxSlippage()
{
    DrawHeader("Max Slippage");
    
    // pannel
    int pnlBodyH = 50;
    int PannelBody = guiAdd(m_hwnd,"label", m_basic.pnlX, m_basic.currHeaderY, m_basic.pnlW, pnlBodyH, "");
    
    int nCurrY = m_basic.currHeaderY + 10;
    
    m_basic.currHeaderY += pnlBodyH;
    
    
    int nWidth = 200;
    
    // check box
    m_chkMaxSlippage = guiAdd(m_hwnd, "checkbox", m_basic.objX, nCurrY, nWidth, 30, "Max Slippage(Points)");
    AddHandle(m_chkMaxSlippage);
    
    m_edtMaxSlippage = guiAdd(m_hwnd, "text", m_basic.objX+nWidth+10, nCurrY, 150, 30, "");
    guiSetBorderColor(m_hwnd, m_edtMaxSlippage, clrBlack);
    AddHandle(m_edtMaxSlippage);
    
    int nDescPnlPos = m_basic.objX+nWidth+10+150+20;
    guiAdd(m_hwnd,"label", nDescPnlPos, nCurrY, 250, 30, "points (1, 5, 10, 100 ...)"); 
    
    string sUseMaxSlippageYN, sMaxSlippage;
    if( !GetCnfgValue("PRICE", "USE_MAX_SLIPPAGE_YN", sUseMaxSlippageYN) ){
        sUseMaxSlippageYN = "N";
    }

    if( sUseMaxSlippageYN=="Y" )
    {
        guiSetChecked(m_hwnd, m_chkMaxSlippage, true);
        
        if( !GetCnfgValue("PRICE", "MAX_SLIPPAGE", sMaxSlippage) )
            sMaxSlippage = "";
            
        guiSetText(m_hwnd, m_edtMaxSlippage, sMaxSlippage, 0, "");
    }

    
    return true;
}


void CGUICnfgSlave::LoadSaveCancelBtn()
{
    DrawHeader("Save / Cancel");
    
    // pannel
    int pnlBodyH = 100;
    int PannelBody = guiAdd(m_hwnd,"label", m_basic.pnlX, m_basic.currHeaderY, m_basic.pnlW, pnlBodyH, "");
    
    int nY      = m_basic.currHeaderY + 20;
    
    m_basic.currHeaderY += pnlBodyH;
    
    int nBtnW   = 200;
    int nBtnH   = 70;
    int nGap    = 30;
    int nMiddle = m_basic.pnlW / 2;
    
    int nPos = nMiddle - (nGap) - nBtnW;
    
    m_btnSave = guiAdd(m_hwnd, "button", nPos, nY, nBtnW, nBtnH, BTN_SAVE);
    AddHandle(m_btnSave);

    nPos = nMiddle + (nGap);// + nBtnW;
    m_btnCancel = guiAdd(m_hwnd, "button", nPos, nY, nBtnW, nBtnH, BTN_CANCEL);
    AddHandle(m_btnCancel);    
}

    

void CGUICnfgSlave::DrawLine(int nY)
{
    int line = guiAdd(m_hwnd,"label", m_basic.pnlX, nY, m_basic.pnlW, 1, "");    //"BestAutoTrade - Slave"); 
    guiSetBgColor(m_hwnd, line, clrBlue);
}


void CGUICnfgSlave::DrawHeader(string sCaption)
{
    int nHeader = guiAdd(m_hwnd,"label", m_basic.pnlX, m_basic.currHeaderY, m_basic.pnlW, m_basic.pnlHeaderH, " "+sCaption);
    guiSetTextColor(m_hwnd, nHeader, clrYellow);
    guiSetBgColor(m_hwnd, nHeader, clrPurple);
    m_basic.currHeaderY += m_basic.pnlHeaderH;
}

void CGUICnfgSlave::AddHandle(int handle)
{
    m_guiHandles.Handles[m_guiHandles.nTotalGui++] = handle;
}




#endif //_GUI_CNFG_SLAVE_H