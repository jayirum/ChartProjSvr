#ifndef _SEND_ORDER_H
#define _SEND_ORDER_H

#include "CFilter.mqh"
#include <stderror.mqh>
#include <stdlib.mqh>
#include "../BAUtils.mqh"

/*
OP_BUY - buy order,
OP_SELL - sell order,
OP_BUYLIMIT - buy limit pending order,
OP_BUYSTOP - buy stop pending order,
OP_SELLLIMIT - sell limit pending order,
OP_SELLSTOP - sell stop pending order.

int  OrderSend(
   string   symbol,              // symbol
   int      cmd,                 // operation
   double   volume,              // volume
   double   price,               // price
   int      slippage,            // slippage
   double   stoploss,            // stop loss
   double   takeprofit,          // take profit
   string   comment=NULL,        // comment
   int      magic=0,             // magic number
   datetime expiration=0,        // pending order expiration
   color    arrow_color=clrNONE  // color
   
*/
class CSendOrder
{
public:
    CSendOrder(CFilter *filter){LinkFilter(filter);};
    ~CSendOrder(){};
    
    
    
    int     SendNewOrder(int nMasterAcc, int nMasterTicket, string sMasterSymbol, int nMasterOrdType, double dMasterLots, double dMasterPrc, int nMasterSlippage);
    bool     SendCloseOrder(int nOpenedTicket, int nMasterAcc, int nMasterTicket, string sMasterSymbol, int nMasterOrdType, double dMasterLots, double dMasterPrc, int nMasterSlippage);
 
    string  GetMsg(){return m_msg;}
private:
    double  CalcOrderPrc(string sSymbol, double dOrgPrc, int nOrdType, bool bClose);
    bool    CheckOrderResult(bool bOpen, /*inout*/int& refSlaveTicket, int nMasterAcc, int nMasterTicket, string sSymbol, int nMasterOrdType, double dMasterLots, double dMasterPrc, int nMasterSlippage);
    bool    LinkFilter(CFilter *filter);
private:
    CFilter  *m_filter;
    string   m_msg;   
};

bool CSendOrder::LinkFilter(CFilter *filter)
{
    if(CheckPointer(filter)==POINTER_INVALID){
        m_msg = "CFilter is not valid pointer";
        return false;
    }
    
    m_filter = filter;
    return true;
}


double CSendOrder::CalcOrderPrc(string sSymbol, double dOrgPrc, int nOrdType, bool bClose)
{
    double dPrc=0;
    
    if(nOrdType==OP_BUY || nOrdType==OP_SELL )
    {
        dPrc = (nOrdType==OP_SELL)? MarketInfo(sSymbol, MODE_BID) : MarketInfo(sSymbol, MODE_ASK); //NormalizeDouble ?
    }
    
    //TODO. DO SOMETHING
    if(nOrdType==OP_BUYLIMIT || nOrdType==OP_SELLLIMIT )
    {
        dPrc = dOrgPrc;
    }
    
    //TODO. DO SOMETHING
    if(nOrdType==OP_BUYSTOP || nOrdType==OP_SELLSTOP )
    {
        dPrc = dOrgPrc;
    }
    return NormalizeDouble (dPrc, (int)MarketInfo(sSymbol, MODE_DIGITS));
}

int CSendOrder::SendNewOrder(int nMasterAcc, int nMasterTicket, string sMasterSymbol, int nMasterOrdType, double dMasterLots, double dMasterPrc, int nMasterSlippage)
{
    double dLotsF   = m_filter.GetLots(sMasterSymbol, dMasterLots);  
    if(dLotsF)
        return -1;
        
    double dNewPrc  = CalcOrderPrc(sMasterSymbol, dMasterPrc, nMasterOrdType, false);
    int nSlippage   = nMasterSlippage;  //CalSlippage();
    
    printlog(StringFormat( "[BEFORE NEW ORD](symbol:%s)(OrdType:%d)(Lots:%f)(Prc:%f)(Magic:%d)",sMasterSymbol, nMasterOrdType, dLotsF, dNewPrc, nMasterTicket));
    int nTicket = OrderSend(
                       sMasterSymbol,     // symbol
                       nMasterOrdType,    // operation
                       dLotsF,      // volume
                       dNewPrc,     // price
                       nSlippage,   // slippage
                       0,           // stop loss
                       0,           // take profit
                       NULL,        // comment
                       nMasterTicket,   //nMasterAcc,  // magic number
                       0,           // pending order expiration
                       0         // color
                    );
    
    if(nTicket==-1)
    {
        int nErr = GetLastError();
        m_msg = StringFormat("[%d]%s", nErr, ErrorDescription(nErr));
        printlog(m_msg, true);
        return -1;
    }
    
    bool bResult = true;
    for(int i=0; i<5; i++)
    {
        bool bOpened = true;
        bResult = CheckOrderResult(bOpened, nTicket, nMasterAcc, nMasterTicket, sMasterSymbol, nMasterOrdType, dMasterLots, dMasterPrc, nMasterSlippage);
        if(bResult) break;
        else{
            nTicket = -1;
            Sleep(10);
            continue;
        }
    }
    return nTicket;
}


/*
bool  OrderClose(
   int        ticket,      // ticket
   double     lots,        // volume
   double     price,       // close price
   int        slippage,    // slippage
   color      arrow_color  // color
   );
*/
bool CSendOrder::SendCloseOrder(int nOpenedTicket, int nMasterAcc, int nMasterTicket, string sMasterSymbol, int nMasterOrdType, double dMasterLots, double dMasterPrc, int nMasterSlippage)
{
    double dLotsF   = m_filter.GetLots(sMasterSymbol, dMasterLots);
    if(dLotsF<0)
        return false;
        
    double dNewPrc  = CalcOrderPrc(sMasterSymbol, dMasterPrc, nMasterOrdType, false);
    int nSlippage   = nMasterSlippage;  //CalSlippage();
    
    printlog(StringFormat( "[BEFORE CLOSE ORD](OpenedTicket:%d)(symbol:%s)(OrdType:%d)(Lots:%f)(Prc:%f)(Magic:%d)",nOpenedTicket, sMasterSymbol, nMasterOrdType, dLotsF, dNewPrc, nMasterTicket));
    bool bReturn = OrderClose(
                       nOpenedTicket    //int        ticket,      // ticket
                       ,dLotsF          //double     lots,        // volume
                       ,dNewPrc         //double     price,       // close price
                       ,0               //int        slippage,    // slippage
                       ,Red              // color      arrow_color  // color
    );
    
    if(!bReturn)
    {
        int nErr = GetLastError();
        m_msg = StringFormat("[%d]%s", nErr, ErrorDescription(nErr));
        printlog(m_msg, true);
        return false;
    }

    bool bOpen = false;
    CheckOrderResult(bOpen, nOpenedTicket, nMasterAcc, nMasterTicket, sMasterSymbol, nMasterOrdType, dMasterLots, dMasterPrc, nMasterSlippage);
    return true;
}


bool CSendOrder::CheckOrderResult(bool bOpen, int& refSlaveTicket, int nMasterAcc, int nMasterTicket, string sSymbol, int nMasterOrdType, double dMasterLots, double dMasterPrc, int nMasterSlippage)
{
    string   sOpenClose;
    bool    bReturn;
    if(bOpen){
        sOpenClose = "[OPEN ]";
        bReturn = OrderSelect(refSlaveTicket, SELECT_BY_TICKET);
    }
    else{
        sOpenClose = "[CLOSE]";
        bReturn = OrderSelect(refSlaveTicket, SELECT_BY_TICKET, MODE_HISTORY);
    }
    if(!bReturn) 
    {
        int err = GetLastError();
        m_msg = StringFormat("OrderSelect error[%d]%s",err, ErrorDescription(err));
        printlog(m_msg);
        return false;
    }
    
    m_msg = StringFormat("%s[%s](MasterAcc:%d)(Masterticket:%d)(MasterOrdType:%d)(MasterLots:%f)(MasterPrc:%f)(MasterSlippage:%d)"
                        "(Acc:%d)(SlaveTicket:%d)(OrdType:%d)(Lots:%f)(OpenTM:%d)(OpenPrc:%f)(CloseTM:%d)(ClosePrc:%f)(Prfit:%f)(Magic:%d)"
                        , sOpenClose, sSymbol
                        , nMasterAcc, nMasterTicket, nMasterOrdType, dMasterLots, dMasterPrc, nMasterSlippage
                        , AccountNumber(), OrderTicket(), OrderType(), OrderLots(), OrderOpenTime(), OrderOpenPrice()
                        , OrderCloseTime(), OrderClosePrice(), OrderProfit(),OrderMagicNumber()
                        );
   
    printlog(m_msg);

    return true;
}








#endif //_SEND_ORDER_H