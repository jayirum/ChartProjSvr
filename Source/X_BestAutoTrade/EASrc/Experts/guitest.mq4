//+------------------------------------------------------------------+
//|                                                      guitest.mq4 |
//|                        Copyright 2019, MetaQuotes Software Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2019, MetaQuotes Software Corp."
#property link      "https://www.mql5.com"
#property version   "1.00"
#property strict
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+

#include "../include/BestAutoTrade/mt4gui2.mqh"


int hwnd = 0;
int Panel = 0;
int List = 0;
int Memo = 0;
int Button = 0;

int OnInit()
{
    ObjectsDeleteAll();
    hwnd = WindowHandle(Symbol(),Period());     
    // Lets remove all Objects from Chart before we start
    guiRemoveAll(hwnd);
    
    long lChartWidth = ChartGetInteger (0, CHART_WIDTH_IN_BARS, 0);
    //Alert(StringFormat("%d", lWidth));
    
    int nPannelWidth    = 400;
    int nPannelHeight   = 700;
    int nY              = 0;
    int nX              = 0;    //lChartWidth;// + nPannelWidth;
    
    string sMsg = StringFormat(">>>>>>>>%d/%d<<<<<<<<<", lChartWidth, nX);
    Panel  = guiAdd (hwnd,"label", nX, nY, nPannelWidth, nPannelHeight, sMsg);  // "This is Label Test"); 
    //guiSetBgColor(hwnd,Panel,LightGray);
    //guiSetTextColor(hwnd,Panel,Black);
    
    
//    List = guiAdd(hwnd,"list",200,150,150,300,"List 1"); 
//            guiAddListItem(hwnd,List,"First List Item");
//            guiAddListItem(hwnd,List,"Second List Item");
//            guiAddListItem(hwnd,List,"Third List Item");
//            guiAddListItem(hwnd,List,"Forth List Item");
//            guiAddListItem(hwnd,List,"Fifth List Item");
//            guiSetListSel(hwnd,List,0); 
//    
//    Memo  = guiAdd(hwnd,"label",100,100,400,700,""); 
//    guiSetBgColor(hwnd,Memo,Yellow);
//    guiSetTextColor(hwnd,Memo,Red);


    return(INIT_SUCCEEDED);
}
//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
{
    if (hwnd>0) { guiRemoveAll(hwnd);     guiCleanup(hwnd ); }

}
//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
  {
//---
   
  }
//+------------------------------------------------------------------+
