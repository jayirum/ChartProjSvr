#ifndef _BA_UTILS_H
#define _BA_UTILS_H

//+------------------------------------------------------------------+
//|                                                      BAUtils.mqh |
//|                              Copyright 2019, BestAutoTrade Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+

/*
    CLog
    COrdManager
*/

#define UTILS_BUF_LEN 128



enum CHANGED_RET {
	CHANGED_NONE = 0,
	CHANGED_POS_OPEN,
	CHANGED_POS_OPEN_ADD,
	CHANGED_POS_CLOSE_FULL,
	CHANGED_POS_CLOSE_PARTIAL,
	CHANGED_POS_OPENPRC,
	CHANGED_ORD_TYPE,
	CHANGED_OPEN_PRC,
	CHANGED_SL_PRC,
	CHANGED_PT_PRC,
	CHANGED_PENDING_NEW,
	CHANGED_PENDING_DEL,
	CHNAGED_PENDING_MOIDIFY
};



// define in OrdManager.h in BAUtils.dll

//+-----------------------------------------------------------------
//+     "BAUtils.dll"
//+-----------------------------------------------------------------
#import "BAUtils.dll"

    void BAUtils_OpenLog(char& pzEAName[]);
    void BAUtils_Log(char& pzData[]);
    
    //+------------------------------------------------------------+
    //	COrdManager - master
    //+------------------------------------------------------------+
    void  BAUtils_OMInitialize(
    	int		ticket
    	, int		type
    	, double	lots
    	, int		open_time
    	, double	open_price
    	, double	stoploss
    	, double	takeprofit
    	, int		close_time
    	, double	close_price
    	, double	commission
    	, double	swap
    	, double	profit
    	, char&		comment[]
    	, int		magic
    );
    
    void BAUtils_OMDeInitialize();
    
    
    CHANGED_RET BAUtils_OMCheckChange(
    	int		ticket
    	, int		type
    	, double	lots
    	, int		open_time
    	, double	open_price
    	, double	stoploss
    	, double	takeprofit
    	, int		close_time
    	, double	close_price
    	, double	commission
    	, double	swap
    	, double	profit
    	, char&		comment[]
    	, int		magic
    	, /*out*/int&		oOpenedTicket
    	, /*out*/double&	oOpenedPrc
    	, /*out*/double&	oOpenedLots
    	, /*out*/int&		oOpenedTm
    );
    
   
    int 	BAUtils_OMGetClosedOrd(int& arrTicket[], double& arrLots[]);
    void	BAUtils_OMBeginCheck();
    int	    BAUtils_OMDeletedOrderCnt();
    	
    void BAUtils_HeaderTime(/*out*/char& zTime[]);
#import
//+-----------------------------------------------------------------
//+     
//+-----------------------------------------------------------------



void BAUtils_OpenLogEx(string name)
{
    char zName[128];
    StringToCharArray(name, zName);
    BAUtils_OpenLog(zName);
}


void printlog(string msg, bool bAlert=false)
{
    char zMsg[];
    ArrayResize(zMsg, StringBufferLen(msg), UTILS_BUF_LEN);
    StringToCharArray(msg, zMsg);
    //Alert("in printlog:"+msg+",size:"+IntegerToString(ArraySize(zMsg)));
    BAUtils_Log(zMsg);
    if(bAlert)
        Alert(msg);
}

class CSleep
{
public:
    CSleep(int nTimeout)            {m_nSleepTimeout=nTimeout;}
    ~CSleep(){};
    
    void ResetTimeout(int nTimeout) { m_nSleepTimeout = nTimeout;}
    void Start()                    { m_nStart = GetTickCount();}
    void CheckSleep()               { m_nCheck = GetTickCount()-m_nStart; if(m_nSleepTimeout>m_nCheck) Sleep(m_nSleepTimeout-m_nCheck); }
private:
    uint m_nStart;
    uint m_nCheck;
    uint m_nSleepTimeout;
            
};




#endif //_BA_UTILS_H