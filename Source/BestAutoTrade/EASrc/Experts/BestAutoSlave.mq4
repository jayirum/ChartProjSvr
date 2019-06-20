//+------------------------------------------------------------------+
//|                                               SimpleBestAuto.mq4 |
//|                              Copyright 2019, BestAutoTrade Corp. |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright "Copyright 2019, BestAutoTrade Corp."
#property link      ""
#property version   "1.00"
#property strict


//--- input parameters
input string    LicenseKey;
input string    InputMasterID       = "MasterJay";
input string    InputMyID           = "SlaveJay";
input string    InputMasterAccNo    = "9051645";

input double    MaxSlippagePoint    = 50;
input int       SleepMS             = 10;

#include "../Include/BestAutoTrade/BAUtils.mqh"
#include "../Include/BestAutoTrade/RelayFiles/CBARelaySlave.mqh"
#include "../include/BestAutoTrade/SlaveFiles/CFilter.mqh"
#include "../include/BestAutoTrade/SlaveFiles/CSendOrder.mqh"
#include "../include/BestAutoTrade/SlaveFiles/CMasterOrdInfo.mqh"
#include "../include/BestAutoTrade/CLogOnOff.mqh"
#include "../include/BestAutoTrade/SlaveFiles/CGUICnfgSlave.mqh"

#include <stderror.mqh>
#include <stdlib.mqh>

/*

void    LogOn(char& RecvData[], int nRecvLen)
void    LogOff(char& RecvData[], int nRecvLen)
void    CopyOrders(char& RecvData[], int nRecvLen)	
string  SymboPair(string sMasterSymbol)
int     FindSlaveOrderByMasterTicket(int nMasterTicket)
void    SaveSymbolPairs()
*/
//+----------------------------------------------------------------------------------------
// Global Variables

string  EA_NAME         = WindowExpertName();//"BestAutoSlave";
int     TIMER_TIMOUT_MS = 10;
int     GUICHECK_CNT    = 100;
int     g_nGuiCheckCnt  = 0;

string  SendChannel;
string  RecvChannel;

string  g_sMsg;
char    g_zMsg[UTILS_BUF_LEN];

char    g_zRecvBuff[BUF_LEN];
string  g_sRecbBuff;

bool    g_bTrade = false;
//+----------------------------------------------------------------------------------------


//+----------------------------------------------------------------------------------------
// classes
CBARelaySlave   g_Relay;
CSleep          g_sleep(SleepMS);   // 0.01 sec
CFilter         *g_filter;
CSendOrder      *g_ordSend;
CGUICnfgSlave   *g_guiCnfg;
//+----------------------------------------------------------------------------------------



int OnInit()
{
    //+----------------------------------------------------------------------------------------
    // log open
    BAUtils_OpenLogEx(EA_NAME);
    g_sMsg = StringFormat("============================= %s =============================", EA_NAME);
    printlog(g_sMsg);
    //+----------------------------------------------------------------------------------------
       
    //+----------------------------------------------------------------------------------------
    // Load config information
    //+----------------------------------------------------------------------------------------
    g_guiCnfg = new CGUICnfgSlave(InputMasterID);
    if(!g_guiCnfg.LoadChannelInfo() ){
        return -1;    
    }
    
    // Load GUI for config
    if(!g_guiCnfg.LoadGUI()){
        return false;
    }
    
    
    //+----------------------------------------------------------------------------------------
    //TODO SaveSymbolPairs();
    //+----------------------------------------------------------------------------------------
    
    //+----------------------------------------------------------------------------------------
    // filters
    g_filter = new CFilter;    
    LOT_TYPE tp = (g_guiCnfg.IsAutoScaleYN())? AUTO:FIXED;
    g_filter.SetLotsFilter(tp, g_guiCnfg.LotsAutoScaleValue(), g_guiCnfg.LotsAutoScaleValue(), g_guiCnfg.LotsAutoScaleValue());
    //+----------------------------------------------------------------------------------------
    
//    //+----------------------------------------------------------------------------------------
//    // create send copy order class
//    g_ordSend = new CSendOrder(g_filter);
//    //+----------------------------------------------------------------------------------------
//    
//    //+----------------------------------------------------------------------------------------
//    // initialze communication channel
//    if(!g_Relay.InitChannel(InputMyID, InputMasterID, SendChannel, RecvChannel))
//        return -1;
//    //+----------------------------------------------------------------------------------------
    
    EventSetMillisecondTimer(1000);
    
    
    return(INIT_SUCCEEDED);
}
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
{
    if(CheckPointer(g_filter)!=POINTER_INVALID)
        delete g_filter;
        
    if(CheckPointer(g_ordSend)!=POINTER_INVALID)
        delete g_ordSend;

    if(CheckPointer(g_guiCnfg)!=POINTER_INVALID)
        delete g_guiCnfg;

    g_Relay.UnRegisterAsSlave(InputMasterAccNo);
    BAUtils_OMDeInitialize();
   
}   
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
{
    while(!IsStopped())
    {
        if(!IsExpertEnabled())  break;

        g_sleep.Start();
        
        
        if(g_filter.IsAbleTradeNow() && g_bTrade && g_guiCnfg.IsClosedGUI() )
        {
            ZeroMemory(g_zRecvBuff);
            int nRet = g_Relay.RecvData(g_zRecvBuff, BUF_LEN); 
            if( nRet>0)
            {
                string sCode = CProtoUtils::PacketCode(g_zRecvBuff);
                
                if(sCode==CODE_PUBLISH_ORDER)
                {
                    CopyOrders(g_zRecvBuff, nRet);
                }
                
                if(sCode==CODE_LOGOFF)
                {
                    LogOff(g_zRecvBuff, nRet);
                }
                
                if(sCode==CODE_LOGON)
                {
                    LogOn(g_zRecvBuff, nRet);
                }
            } // if( nRet>0)
            
        }// if(g_filter.IsAbleTradeNow())
        
        if( ++g_nGuiCheckCnt > GUICHECK_CNT )
        {
            g_guiCnfg.CheckGUI();
            g_nGuiCheckCnt = 0;
        }    
        g_sleep.CheckSleep();
    }
     
}
//+------------------------------------------------------------------+
//| Timer function                                                   |
//+------------------------------------------------------------------+
void OnTimer()
{
    EventKillTimer();
    g_Relay.RegisterAsSlave(InputMasterAccNo);
}




void LogOn(char& RecvData[], int nRecvLen)
{
    CSlaveLogOnOff logOn(InputMasterID, InputMyID);
    if(!logOn.ParsingPacket(g_zRecvBuff, nRecvLen)){
        return;
    }
        
    printlog(logOn.GetLogOffMsg(),true);
    
    if( logOn.IsMyLogOn() )
        g_bTrade = true;
}


void LogOff(char& RecvData[], int nRecvLen)
{
    CSlaveLogOnOff logOff(InputMasterID, InputMyID);
    if(!logOff.ParsingPacket(g_zRecvBuff, nRecvLen)){
        return;
    }


    printlog(logOff.GetLogOffMsg(),true);
    
    if(logOff.IsMyLogOff()){
        g_bTrade = true;
        ExpertRemove();
    }
}

void CopyOrders(char& RecvData[], int nRecvLen)
{
    CMasterOrdInfo mOrder(InputMasterID, InputMyID);
    if(!mOrder.ParsingPacket(g_zRecvBuff, nRecvLen))
        return;
    
    
    if( mOrder.IsCloseOrder() || mOrder.IsModifyOrder())
    {
        // find the slave ticket which has been linked with the master ticket
        int nSlaveTicket = FindSlaveOrderByMasterTicket(mOrder.MasterTicket());
        if(nSlaveTicket<0)
        {
            g_sMsg = StringFormat("Can't find opened SlaveTicket linked with MasterTicket(%d)(%d)", mOrder.MasterAcc(), mOrder.MasterTicket());
            printlog(g_sMsg, true);
            return;
        }
        printlog(StringFormat("Find Slave Ticket for Closing.(Master Ticket:%d)(Slave Ticket:%d)",mOrder.MasterTicket(), nSlaveTicket));
        
        
        //TODO. Send close order
        
        if(mOrder.IsCloseOrder()){
            g_ordSend.SendCloseOrder(nSlaveTicket, mOrder.MasterAcc(),mOrder.MasterTicket(), SymboPair(mOrder.SymbolCode()), mOrder.OrdType(), mOrder.OrdLots(), mOrder.OrdPrc(), 0);
            //g_ticketList.DeleteTicketPair(mOrder.MasterAcc(), mOrder.MasterTicket());
        }        
    }
    else if(mOrder.IsNewOrder())
    {
        int nSlaveTicket = g_ordSend.SendNewOrder(mOrder.MasterAcc(),mOrder.MasterTicket(), SymboPair(mOrder.SymbolCode()), mOrder.OrdType(), mOrder.OrdLots(), mOrder.OrdPrc(), 0);
        if(nSlaveTicket<0){
            printlog(g_ordSend.GetMsg());
            return;
         }
         //printlog(StringFormat("[New Ord OK]Ticket:%d", nSlaveTicket));
         

        // Add ticket pair
        //g_ticketList.AddTicketPair(mOrder.MasterAcc(), mOrder.MasterTicket(), nSlaveTicket);
        
    }
     
}

string SymboPair(string sMasterSymbol)
{
    char zMasterSymbol[32];
    char zSlaveSymbol[32];
    
    StringToCharArray(sMasterSymbol, zMasterSymbol);
    if( !BAUtils_SymbolPairGet(zMasterSymbol, zSlaveSymbol))
        return sMasterSymbol;

    return CharArrayToString(zSlaveSymbol);
}

int FindSlaveOrderByMasterTicket(int nMasterTicket)
{
    int nSlaveTicket = -1;
    
    int nTotal = OrdersTotal();
    for(int i = 0; i<nTotal; i++) 
    {
        if(!OrderSelect(i, SELECT_BY_POS)) 
        {
            g_sMsg = StringFormat("FindSlaveOrderByMasterTicket OrderSelect error:%s",ErrorDescription(GetLastError()));
            printlog(g_sMsg);
            break;
        }
        
        if( OrderMagicNumber()== nMasterTicket)
        {
            nSlaveTicket = OrderTicket();
            break;
        }
    }
    return nSlaveTicket;
}


void SaveSymbolPairs()
{
    char MasterSymbol[32];
    char SlaveSymbol[32];
    
    StringToCharArray("USDCHF", MasterSymbol);  StringToCharArray("USDCHF", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("USDJPY", MasterSymbol);  StringToCharArray("USDJPY", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("USDCAD", MasterSymbol);  StringToCharArray("USDCAD", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("AUDUSD", MasterSymbol);  StringToCharArray("AUDUSD", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("EURUSD", MasterSymbol);  StringToCharArray("EURUSD", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("EURAUD", MasterSymbol);  StringToCharArray("EURAUD", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("EURCHF", MasterSymbol);  StringToCharArray("EURCHF", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("EURJPY", MasterSymbol);  StringToCharArray("EURJPY", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("GBPCHF", MasterSymbol);  StringToCharArray("GBPCHF", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("CADJPY", MasterSymbol);  StringToCharArray("CADJPY", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("GBPJPY", MasterSymbol);  StringToCharArray("GBPJPY", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("AUDNZD", MasterSymbol);  StringToCharArray("AUDNZD", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("AUDCAD", MasterSymbol);  StringToCharArray("AUDCAD", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("AUDCHF", MasterSymbol);  StringToCharArray("AUDCHF", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    StringToCharArray("AUDJPY", MasterSymbol);  StringToCharArray("AUDJPY", SlaveSymbol);
    BAUtils_SymbolPairAdd(MasterSymbol, SlaveSymbol);
    
    
}

//+------------------------------------------------------------------+
 