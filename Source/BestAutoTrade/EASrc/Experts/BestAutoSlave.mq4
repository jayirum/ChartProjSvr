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
input string    SendChannel         = "110.4.89.206:64999";
input string    RecvChannel         = "110.4.89.206:64997";
input string    UseAlertYN          = "Y";
input string    LotsAutoScaleYN     = "Y";
input double    LotsAutoScaleValue  = 1.0;
input string    LotsFixedYN         = "N";
input double    LotsFixedValue      = 0.02;
input double    MaxLotSize          = 5;
input double    MaxSlippagePoint    = 50;
input int       SleepMS             = 10;

#include "../Include/BestAutoTrade/BAUtils.mqh"
#include "../Include/BestAutoTrade/RelayFiles/CBARelaySlave.mqh"
#include "../include/BestAutoTrade/SlaveOrder/CFilter.mqh"
#include "../include/BestAutoTrade/SlaveOrder/CMasterOrdInfo.mqh"
#include "../include/BestAutoTrade/SlaveOrder/CSendOrder.mqh"
#include <stderror.mqh>
#include <stdlib.mqh>

string      EA_NAME         = "BestAutoSlave";
int         TIMER_TIMOUT_MS = 10;

CBARelaySlave    g_Relay;
CFilter         *g_filter;
CSendOrder      g_ordSend;
CSleep          g_sleep(SleepMS);   // 0.01 sec

string      g_sMsg;
char        g_zMsg[UTILS_BUF_LEN];

char        g_zRecvBuff[BUF_LEN];
string      g_sRecbBuff;


int OnInit()
{
    // log open
    BAUtils_OpenLogEx(EA_NAME);
    g_sMsg = StringFormat("============================= %s =============================", EA_NAME);
    printlog(g_sMsg);
    
    
    // filters
    g_filter = new CFilter;    
    LOT_TYPE tp = (LotsAutoScaleYN=="Y")? AUTO:FIXED;
    g_filter.SetLotsFilter(tp, LotsAutoScaleValue,LotsFixedValue,MaxLotSize);
    //g_filter.SetMaxSlippage(MaxSlippagePoint);
    
    
    
    
    // initialze communication channel
    if(!g_Relay.InitChannel(InputMyID, InputMasterID, SendChannel, RecvChannel))
        return -1;
    
    
    g_ordSend.LinkFilter(g_filter);
    
    EventSetMillisecondTimer(500);
    
    return(INIT_SUCCEEDED);
}
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
{
    if(CheckPointer(g_filter)!=POINTER_INVALID)
        delete g_filter;

    g_Relay.UnRegisterAsSlave(InputMasterAccNo);
    BAUtils_OMDeInitialize();
  
////--- destroy timer
//    EventKillTimer();
   
}   
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
{
    while(!IsStopped())
    {
        if(!IsExpertEnabled()) break;
        g_sleep.Start();
        
        MainProc();//
        
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



void MainProc()
{
    if(!g_filter.IsAbleTradeNow()){
        return;
    }
    
    //RefreshRates();
    
    ZeroMemory(g_zRecvBuff);
    int nRet = g_Relay.RecvData(g_zRecvBuff, BUF_LEN); 
    if( nRet>0)
    {
        
        CMasterOrdInfo  mOrder;
        mOrder.ParsingPacket(nRet,g_zRecvBuff);

        //printlog(StringFormat("OrdAction:%s", mOrder.OrdAction()));
        
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
                g_ordSend.SendCloseOrder(nSlaveTicket, mOrder.MasterAcc(),mOrder.MasterTicket(), mOrder.SymbolCode(), mOrder.OrdType(), mOrder.OrdLots(), mOrder.OrdPrc(), 0);
                //g_ticketList.DeleteTicketPair(mOrder.MasterAcc(), mOrder.MasterTicket());
            }        
        }
        else 
        if(mOrder.IsNewOrder())
        {
            int nSlaveTicket = g_ordSend.SendNewOrder(mOrder.MasterAcc(),mOrder.MasterTicket(), mOrder.SymbolCode(), mOrder.OrdType(), mOrder.OrdLots(), mOrder.OrdPrc(), 0);
            if(nSlaveTicket<0){
                printlog(g_ordSend.GetMsg());
                return;
             }
             //printlog(StringFormat("[New Ord OK]Ticket:%d", nSlaveTicket));
             

            // Add ticket pair
            //g_ticketList.AddTicketPair(mOrder.MasterAcc(), mOrder.MasterTicket(), nSlaveTicket);
            
        }
        
     }
     
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

//+------------------------------------------------------------------+
 