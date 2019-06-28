//+------------------------------------------------------------------+
//|                                                    ArrayTest.mq4 |
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


#include "../Include/BestAutoTrade/SlaveOrder/CTicketList.mqh"
#include "../Include/BestAutoTrade/BAUtils.mqh"

int array[5]={4,1,6,3,9};

CTicketList list;


string msg;
int OnInit()
  {
        
        BAUtils_OpenLogEx("ARRAY_TEST");
        
        listtest();

    ///////////////////////////////////////////////////////////////////
//    ArraySort(array, WHOLE_ARRAY, 0, MODE_DESCEND);
//    for(int i=0; i<5; i++)
//    {
//        msg += IntegerToString(array[i]);
//    }
//    Alert(msg);
//    
//    msg = "";
//    ArraySort(array, WHOLE_ARRAY, 0, MODE_ASCEND);
//    for(int i=0; i<5; i++)
//    {
//        msg += IntegerToString(array[i]);
//    }
//    Alert(msg);
    ///////////////////////////////////////////////////////////////////
    
    ///////////////////////////////////////////////////////////////////
//    int array2[5][2]={{7,3},{3,1},{57,14},{12,4},{11,1}};
//    ArraySort(array2, WHOLE_ARRAY, 0, MODE_ASCEND);
//    for(int i=0; i<5; i++)
//    {
//        for(int j=0; j<2; j++)
//        {
//            if(j==0) 
//                msg+="{"; 
//            else msg+=",";
//         
//            msg += IntegerToString(array2[i,j]);
//         
//            if(j==1) msg+="}";
//        }
//    }
//    Alert(msg);
    ///////////////////////////////////////////////////////////////////
//    Alert("-------------------------------------------");
//    int array3[5][3]={{722342,3, 2},{3273663,1, 9},{579383,14, 15},{123,4, 0},{113334322,1, 9}};
//    ArraySort(array3, WHOLE_ARRAY, 0, MODE_DESCEND);
//    for(int i=0; i<5; i++)
//    {
//        for(int j=0; j<3; j++)
//        {
//            if(j==0) 
//                msg+="{"; 
//            else msg+=",";
//         
//            msg += IntegerToString(array3[i,j]);
//         
//            if(j==2) msg+="}";
//        }
//    }
//    Alert(msg);
//    
//    int nSearch = ArrayBsearch(array3, 123, WHOLE_ARRAY, 0, MODE_DESCEND);
//    Alert("123:"+IntegerToString(nSearch));
//
//    nSearch = ArrayBsearch(array3, 113334322, WHOLE_ARRAY, 0, MODE_DESCEND);
//    Alert("113334322:"+IntegerToString(nSearch));  
//    
//    nSearch = ArrayBsearch(array3, 33333, WHOLE_ARRAY, 0, MODE_DESCEND);
//    Alert("33333:"+IntegerToString(nSearch));  
    
      
//    msg="";
//    ArraySort(array3, WHOLE_ARRAY, 0, MODE_DESCEND);
//    for(int i=0; i<5; i++)
//    {
//        for(int j=0; j<3; j++)
//        {
//            if(j==0) 
//                msg+="{"; 
//            else msg+=",";
//         
//            msg += IntegerToString(array3[i,j]);
//         
//            if(j==2) msg+="}";
//        }
//    }
//    Alert(msg);


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

int acc1 = 121212;
int acc2 = 232323;

void listtest()
{
  printlog("1---------------------------------------------");  
    list.AddTicketPair(acc1, 123455, 345677);
    list.AddTicketPair(acc1, 123475, 345697);
    list.AddTicketPair(acc1, 123555, 345777);
    list.AddTicketPair(acc1, 123425, 345647);
    list.AddTicketPair(acc1, 123498, 345726);
    list.AddTicketPair(acc1, 123498, 345726);
    list.AddTicketPair(acc1, 123498, 345726);
    list.AddTicketPair(acc1, 123498, 345726);
    list.AddTicketPair(acc1, 123498, 345727);
//    list.AddTicketPair(acc1, 123498, 345726);
//    
    list.AddTicketPair(acc2, 124455, 346677);
    list.AddTicketPair(acc2, 123677, 345899);
    list.AddTicketPair(acc2, 123788, 345800);
  printlog("2---------------------------------------------");  
    
    list.PrintAll(acc1);
    list.PrintAll(acc2);
  printlog("3---------------------------------------------");  
    printlog(StringFormat("[%d][%d]", 123555, list.GetSlaveTicket(121212, 123555)),false);
    printlog(StringFormat("[%d][%d]", 123677, list.GetSlaveTicket(232323, 123677)),false);
    
  printlog("4---------------------------------------------");  
    list.DeleteTicketPair(acc1, 123555);
    list.PrintAll(acc1);
    list.PrintAll(acc2);
  printlog("5---------------------------------------------"); 
    list.Clear(false, acc2);
    list.PrintAll(acc1);
    list.PrintAll(acc2);
  printlog("6---------------------------------------------"); 
    list.Clear(true, acc2);
    list.PrintAll(acc1);
    list.PrintAll(acc2);
}