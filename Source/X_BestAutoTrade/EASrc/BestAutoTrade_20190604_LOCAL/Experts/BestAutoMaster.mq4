//+------------------------------------------------------------------+
//|                                               SimpleBestAuto.mq4 |
//|                              Copyright 2019, BestAutoTrade Corp. |
//|                                                                  |
//+------------------------------------------------------------------+
#property copyright "Copyright 2019, BestAutoTrade Corp."
#property link      ""
#property version   "1.00"
#property strict



#include "../Include/BestAutoTrade/CBARelay.mqh"
#include "../Include/BestAutoTrade/BAUtils.mqh"
#include <stderror.mqh>
#include <stdlib.mqh>

//--- input parameters
input string    LicenseKey;
input string    IsThisMaster_YN     = "Y";
input string    InputMasterAccNo    = "9051645";
input int       CheckOrdTimeoutMS   = 10;   
input string    PublishOrderYN      = "Y";
input int       SleepMS             = 10;
    

string      EA_NAME         = "BestAutoMaster";
int         ReceiveTimeout  = 10;    // ms
string      g_sMsg;
char        g_zMsg[UTILS_BUF_LEN];
CBARelay    g_baRelay;
CSleep      g_sleep(SleepMS);   // 0.01 sec

#define MAX_DELETED_ORDERS  10
int     DeletedTickets[MAX_DELETED_ORDERS];
double  DeletedLots[MAX_DELETED_ORDERS];


int OnInit()
{
    StringToCharArray(EA_NAME, g_zMsg);
    BAUtils_OpenLog(g_zMsg);
    g_sMsg = StringFormat("============================= %s =============================", EA_NAME);
    printlog(g_sMsg);
    
    //+----------------------
    LoadTradeInfo();
    //+----------------------
    
    
    if(!g_baRelay.Initialize()){
        printlog(g_baRelay.GetMsg());
        return -1;
    }
        
    g_baRelay.RegisterAsMaster();
    printlog(g_baRelay.GetMsg());
    
    EventSetMillisecondTimer(CheckOrdTimeoutMS);

    return(INIT_SUCCEEDED);
}
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
{ 
    g_baRelay.DeInitialize();
    BAUtils_OMDeInitialize();

//--- destroy timer
    EventKillTimer();
}   
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//---
    
  }
//+------------------------------------------------------------------+
//| Timer function                                                   |
//+------------------------------------------------------------------+
void OnTimer()
{

    EventKillTimer();
    while(!IsStopped())
    {
        if(!IsExpertEnabled()) break;
        g_sleep.Start();
        
        MainProc();//
        
        g_sleep.CheckSleep();
    }
}

void OnClick()
{
//---
    MainProc();
}


void MainProc()
{
    if(PublishOrderYN=="N") return;
    
    int nTotal = OrdersTotal();
    //g_sMsg = StringFormat("[MainProc]OrdersTotal:%d", nTotal);
    //printlog(g_sMsg);
 
 
    BAUtils_OMBeginCheck();
    for(int i = 0; i<nTotal; i++) 
    {
        Sleep(1);
        if(!OrderSelect(i, SELECT_BY_POS)) 
        {
            // if one order is delete while this loop is running, the index is over the real index
            if( i>=OrdersTotal())
            {
                printlog(StringFormat("Index[%d] is over the real count[%d] as one order is deleted", i, OrdersTotal()));
                return;
            }
            
            g_sMsg = StringFormat("[%d]MainProc OrderSelect error:%s", ErrorDescription(GetLastError()));
            printlog(g_sMsg, true);
            return;
        }
        
        // save only market order (how distinguish open order and pending order?)
        if( OrderType() != OP_BUY && OrderType() != OP_SELL)
            continue;
    
        
        int     refOpenedTicket = -1;
        double  refOpenedPrc  = -1;
        double	refOpenedLots = -1;
        int		refOpenedTm   = -1;
        StringToCharArray(OrderComment(),g_zMsg);
        CHANGED_RET ret = BAUtils_OMCheckChange(
                               OrderTicket()           //int		ticket
                               , OrderType()           //int		type
                               , OrderLots()           //double	lots
                               , (int)OrderOpenTime()  //int		open_time
                               , OrderOpenPrice()      //double	open_price
                               , OrderStopLoss()       //double	stoploss
                               , OrderTakeProfit()     //double	takeprofit
                               , (int)OrderCloseTime() //int		close_time
                               , OrderClosePrice()     //double	close_price
                               , OrderCommission()     // double	commission
                               , OrderSwap()           // double	swap
                               , OrderProfit()         //double	profit
                               , g_zMsg                //char&		comment[]
                               , OrderMagicNumber()    // int		magic
                               , refOpenedTicket         // out
                               , refOpenedPrc            // out
                               , refOpenedLots           // out
                               , refOpenedTm             // out
                               );
                                    
        ProcessByChange(ret, refOpenedTicket, refOpenedPrc, refOpenedLots, refOpenedTm);
       
    } // for(int i = 0; i<nTotal; i++) 
   
    
    //+-------------------------------------------
    // check close order
    //+-------------------------------------------
    
    //int nDeletedOrders = BAUtils_DeletedOrderCnt();
    //if( nDeletedOrders <= 0 )
    //    return;
    int  nDeletedOrders = BAUtils_OMGetClosedOrd(DeletedTickets, DeletedLots);
    for ( int i=0; i<nDeletedOrders; i++ )
    {
        g_sMsg = StringFormat("[%d][CLOSE](DeletedTicket:%d)(DeletedLots:%f)",i, DeletedTickets[i], DeletedLots[i]);
        printlog(g_sMsg);
        //+------------------------------------------------------------------------------+//
        SendCloseOrder(DeletedTickets[i], "C", DeletedLots[i]);
        //+------------------------------------------------------------------------------+//
    }
  
}


void ProcessByChange(CHANGED_RET ret, int nOpenedTicket, double dOpenedPrc, double dOpenedLots, int nOpenedTm)
{
    switch(ret)
    {
        case CHANGED_NONE:
            break;
            
        case CHANGED_POS_OPEN:
            g_sMsg = StringFormat("[NEW ORDER](ticket:%d)(Prc:%f)(TM:%d)", OrderTicket(), OrderOpenPrice(), OrderOpenTime());
            printlog(g_sMsg);
            //+------------------------------------------------------------------------------+//
            SendOrder(OrderTicket(), "O", OrderOpenPrice(), OrderLots(), OrderOpenTime(), -1, -1, -1, 0);
            //+------------------------------------------------------------------------------+//
            break;
        
        //case CHANGED_POS_CLOSE_FULL:
        //    g_sMsg = StringFormat("[CLOSE ORDER](OpenedTicket:%d)(OpenedPrc:%f)(OpenedLots:%f)(TM:%d)", refOpenedTicket, refOpenedPrc, refOpenedLots, refOpenedTm);
        //    printlog(g_sMsg);
        //    //+------------------------------------------------------------------------------+//
        //    SendCloseOrder(refOpenedTicket, "C", refOpenedLots);
        //    //+------------------------------------------------------------------------------+//
        //    break;

        case CHANGED_POS_CLOSE_PARTIAL:
            g_sMsg = StringFormat("[PARTIAL](ticket:%d)(Prc:%f)(Lots:%f)(TM:%d)<OpenTicket:%d><OpenPrc:%f><OpenLots:%f><OpenTM:%D>",
                                    OrderTicket(), OrderClosePrice(), OrderLots(), OrderCloseTime(), 
                                    nOpenedTicket, dOpenedPrc, dOpenedLots, nOpenedTm);
            printlog(g_sMsg);
            //+------------------------------------------------------------------------------+//
            SendCloseOrder(nOpenedTicket, "P", dOpenedLots);
            //+------------------------------------------------------------------------------+//
            break;
        
        case CHANGED_SL_PRC:
            g_sMsg = StringFormat("[MainProc]StopLoss Prc ticket:%d", OrderTicket());
            printlog(g_sMsg);
            break;
        
        case CHANGED_PT_PRC:
            g_sMsg = StringFormat("[MainProc]ProfitTaing Prc ticket:%d", OrderTicket());
            printlog(g_sMsg);
            break;
            
        case CHANGED_POS_OPEN_ADD:
        case CHANGED_POS_OPENPRC:
        case CHANGED_ORD_TYPE:
        case CHANGED_OPEN_PRC:
        case CHANGED_PENDING_NEW:
        case CHANGED_PENDING_DEL:
        case CHNAGED_PENDING_MOIDIFY:
            g_sMsg = StringFormat("CHANGED_RET err:%d", ret);
            printlog(g_sMsg);
            break;
    }// switch(ret)
}

//SendOrder(OrderTicket(), "O", OrderOpenPrice(), OrderLots(), OrderOpenTime(), -1, -1, -1, 0);
void SendOrder(
    int ticket, string sAction, double dOrdPrc, double dOrderLots, datetime OrdTm, int nOpenedTicket, double dOpenedPrc, double dOpenedLots, datetime dtOpenedTm)
{
    g_baRelay.SendMasterOrder(
        IntegerToString(AccountNumber())    // string sMasterAccNo// char MasterAccNo    [20];
        ,ticket
        ,OrderSymbol()
        ,sAction            // O:Open, C:Close, M:Modify
        ,OrderType()        // int    nType
        ,dOrdPrc            // char OrdPrc
        ,OrderStopLoss()    // double dSLPrc
        ,OrderTakeProfit()  // double dPTPrc
        ,dOrderLots         // double dLots
        ,OrdTm              // datetime dtOrdTm
        ,nOpenedTicket      // int     nOpenedTicket
        ,dOpenedPrc         // double  dOpenedPrc
        ,dOpenedLots        // double dOpenedLots
        ,dtOpenedTm         // datetime dtOpenedTm
        ,OrderProfit()      // double dProfit
        ,OrderSwap()        // double dSwap
        ,"MasterOpenOrder"
     );

    printlog(StringFormat("[SEND OPEN](%s)",g_baRelay.GetMsg()));
}



/*
, refOpenedTicket         // out
, refOpenedPrc            // out
, refOpenedLots           // out
, refOpenedTm             // out

    action : C, P
*/
void SendCloseOrder(int ticket, string sOrdAction, double dOpenedLots)
{
    if(!OrderSelect(ticket, SELECT_BY_TICKET,MODE_HISTORY  )) 
    {
        printlog(StringFormat("Fail to OrderSelect for check closed order.error:%s",ErrorDescription( GetLastError())));
        return;
    }
        
    g_baRelay.SendMasterOrder(
        IntegerToString(AccountNumber())    // string sMasterAccNo// char MasterAccNo    [20];
        ,ticket
        ,OrderSymbol()
        ,sOrdAction         // O:Open, C:Close, P:Partial, M:Modify
        ,OrderType()        // int    nType
        ,OrderClosePrice()  // char OrdPrc
        ,OrderStopLoss()    // double dSLPrc
        ,OrderTakeProfit()  //double dPTPrc
        ,OrderLots()        //double dLots
        ,OrderCloseTime()   //datetime dtOrdTm
        ,ticket             //int     nOpenedTicket
        ,OrderOpenPrice()   //double  dOpenedPrc
        ,dOpenedLots      //double    dOpenedLots
        ,OrderOpenTime()    //datetime  dtOpenedTm
        ,OrderProfit()      // double   dProfit
        ,OrderSwap()        // double   dSwap
        ,"MasterCloseOrder"
     );

    printlog(StringFormat("[SEND CLOSE](%s)",g_baRelay.GetMsg()));
}

void OnChartEvent(const int id,         // Event ID
                  const long& lparam,   // Parameter of type long event
                  const double& dparam, // Parameter of type double event
                  const string& sparam  // Parameter of type string events
  )
{
    if(id==CHARTEVENT_CLICK)
    {
        MainProc();
    }
}

void LoadTradeInfo()
{
    g_sMsg = "======================================================";
    printlog(g_sMsg);

    int nTotal = OrdersTotal();
    g_sMsg = StringFormat("OrdersTotal:%d", nTotal);
    printlog(g_sMsg);
    for(int i = 0; i<nTotal; i++) 
    {
        if(!OrderSelect(i, SELECT_BY_POS)) 
        {
            g_sMsg = StringFormat("LoadTradeInfo OrderSelect error:%d",GetLastError());
            printlog(g_sMsg);
            return;
        }
        
        // save only market order (how distinguish open order and pending order?)
        if( OrderType() != OP_BUY && OrderType() != OP_SELL){
            g_sMsg = StringFormat("LoadTradeInfo Not Market Order.type=%d", OrderType());
            printlog(g_sMsg);
            continue;
        }
        
        StringToCharArray(OrderComment(),g_zMsg);
        BAUtils_OMInitialize(
        	OrderTicket()   //int		ticket
        	, OrderType()   //int		type
        	, OrderLots()   //double	lots
        	, (int)OrderOpenTime()   //int		open_time
        	, OrderOpenPrice()    //double	open_price
        	, OrderStopLoss()     //double	stoploss
        	, OrderTakeProfit() //double	takeprofit
        	, (int)OrderCloseTime()  //int		close_time
        	, OrderClosePrice() //double	close_price
        	, OrderCommission() // double	commission
        	, OrderSwap()       // double	swap
        	, OrderProfit()     //double	profit
        	, g_zMsg          //char&		comment[]
        	, OrderMagicNumber()    // int		magic
        );
        
   }
   return;
}
//+------------------------------------------------------------------+
