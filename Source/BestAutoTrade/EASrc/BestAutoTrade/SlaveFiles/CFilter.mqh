#ifndef _CFILTER_H
#define _CFILTER_H

#include "../BAUtils.mqh"

/*
    Manage the Filters & Options for Order and Risk Management
    
    for pending order : double StopLevel = MarketInfo(Symbol(),MODE_STOPLEVEL) * Point;
*/


enum LOT_TYPE {NONE=-1, FIXED, AUTO};
struct LOTS_INFO
{
    LOT_TYPE tp;
    double  dAutoScale;     // when type is AUTO, orig lot * dAutoScale
    double  dFixedLots;
    double  dMaxLotSize;
};

class CFilter
{
public:
    CFilter();
    ~CFilter();
    
    bool    SetFilter(){return true;};      //TODO
    bool    SetLotsFilter(LOT_TYPE tp, double  dAutoScale, double  dFixedLots, double dMaxLotSize);
    bool    SetMaxSlippagePips(double dVal) {m_dMaxSlippagePip = dVal; return true;} 
    
    //double  GetMaxSlippagePips(string sSymbol);
    double  GetLots(string sSymbol, double dOrgLots);
    bool    IsAbleTradeNow();

private:
    void    Clear();
    
private:
    LOTS_INFO   m_lots;
    double      m_dMaxSlippagePip;
    int         m_nMultiple;
    string      m_sLotsTp;
    string      m_msg; 
};

CFilter::CFilter()
{
    Clear();
    
    
}

CFilter::~CFilter()
{
    
}

//double  CFilter::GetMaxSlippagePips(string sSymbol)
//{
//    double dSlippage;
//    
//    int digits = MarketInfo(sSymbol, MODE_DIGITS);
//    if( digits==3 || digits==5 )
//        dSlippage = m_dMaxSlippagePip * MarketInfo(sSymbol, MODE_POINT) * 10;
//    
//    if( digits==2 || digits==4 )
//        dSlippage = m_dMaxSlippagePip * MarketInfo(sSymbol, MODE_POINT);
//        
//    return dSlippage;
//}

void CFilter::Clear(void)
{
    m_lots.tp = NONE;
    m_lots.dAutoScale = -1;
    m_lots.dFixedLots = -1;
    m_lots.dMaxLotSize = -1;
    
    //TODO m_dMaxSlippagePip = -1;
}

bool CFilter::SetLotsFilter(LOT_TYPE tp, double  dAutoScale, double  dFixedLots, double dMaxLotSize)
{
    m_lots.tp = tp;
    if(tp==FIXED)
    {
        m_sLotsTp = "FIXED";
        if(dFixedLots<=0)
        {
            m_msg = "FIXED type needs fixed lots size";
            return false;
        }
    }
    if(tp==AUTO)
    {
        m_sLotsTp = "AUTO";
        if(dAutoScale<=0)
        {
            m_msg = "AUTO type needs auto scale";
            return false;
        }
    }
    m_lots.dAutoScale = dAutoScale;
    m_lots.dFixedLots = dFixedLots;
    m_lots.dMaxLotSize  = dMaxLotSize;
    
    printlog(StringFormat("TP:%s, dAutoScale:%f, dFixedLots:%f", m_sLotsTp, dAutoScale, dFixedLots));
    
    return true;
}

double  CFilter::GetLots(string sSymbol, double dOrgLots)
{
    double dOrdLots=dOrgLots;
    if(m_lots.tp==FIXED) dOrdLots = m_lots.dFixedLots;

    if(m_lots.tp==AUTO) dOrdLots = m_lots.dAutoScale * dOrgLots;
    
    printlog(StringFormat("[GetLots] orglots:%f, dAutoScale:%f", dOrgLots, m_lots.dAutoScale));
    
    if(dOrdLots < MarketInfo(sSymbol,MODE_MINLOT))
    {
        //dOrdLots = MarketInfo(sSymbol,MODE_MINLOT);
        m_msg = StringFormat("Order Lots is smaller than the Minimum lot (%d)", MarketInfo(sSymbol,MODE_MINLOT));
        printlog(m_msg, true);
        return -1;
    }
    else if(dOrdLots > MarketInfo(sSymbol,MODE_MAXLOT))
    {
        //dOrdLots = MarketInfo(sSymbol,MODE_MAXLOT);
        m_msg = StringFormat("Order Lots is greater than the Maximum lot (%d)", MarketInfo(sSymbol,MODE_MAXLOT));
        printlog(m_msg, true);
        return -1;
    }


    if(MarketInfo(sSymbol,MODE_LOTSTEP) == 0.1)
    {
        dOrdLots = NormalizeDouble(dOrdLots,1);
    }
    else 
        dOrdLots = NormalizeDouble(dOrdLots,2);
    
    return dOrdLots;
}


bool    CFilter::IsAbleTradeNow()
{
    if(IsStopped()){
        m_msg = "EA Status is [IsStopped]";
        return false;
    }
    
    if(!IsExpertEnabled()){
        m_msg = "EA Status is [IsExpertEnabled]";
        return false;
    }
    
    if(!IsTradeAllowed()){
        m_msg = "EA Status is [IsTradeAllowed]";
        return false;
    }
    
    if(IsTradeContextBusy()){
        m_msg = "EA Status is [IsTradeContextBusy]";
        return false;
    }
    
    if(!IsConnected()){
        m_msg = "EA Status is [IsConnected]";
        return false;
    }
    
    return true;
}







#endif // _CFILTER_H