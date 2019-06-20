//+------------------------------------------------------------------+
//|                                                     Protocol.mq4 |
//|                              Copyright 2019, BestAutoTrade Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2019, BestAutoTrade Corp."
#property link      "https://www.mql5.com"
#property version   "1.00"
#property strict
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+






int OnInit()
  {
//--- create timer
   //EventSetTimer(60);
   
   //char buf[128];
   //StringToCharArray(buf, "1=ID");
   //int len = Arra
    string msg = "1=ID"; 
    msg = StringSetChar(msg, StringLen(msg), 0x01);
    StringAdd(msg, "2=PWD");
    msg = StringSetChar(msg, StringLen(msg), 0x01);
    StringAdd(msg, "3=ORD");
    msg = StringSetChar(msg, StringLen(msg), 0x01);
    StringAdd(msg, "4=PRICE");
    msg = StringSetChar(msg, StringLen(msg), 0x01);
    StringAdd(msg, "5=LOTS");
    msg = StringSetChar(msg, StringLen(msg), 0x01);
    Alert(msg+":"+IntegerToString(StringLen(msg)));

    ushort deli = 0x01;
    string result[];
    string buf;
    int k=StringSplit(msg,deli,result);
   if(k>0)
     {
      for(int i=0;i<k;i++)
        {
            buf = StringFormat("result[%d]=%s",i,result[i]);
         Alert(buf);
        }
     }
//---
   return(INIT_SUCCEEDED);
  }
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
  {
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
//---
   
  }
//+------------------------------------------------------------------+
