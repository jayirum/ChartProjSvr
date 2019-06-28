#ifndef _CTICKET_LIST_H
#define _CTICKET_LIST_H

#include "../BAUtils.mqh"

/*
    Array of SLAVE TICKET - MASTER TICKET Pairs
*/

/*
    array[100][2] = { 
                        {masterticket, slaveticket},
                        {masterticket, slaveticket},
                        ......
                        {masterticket, slaveticket},
                     }
*/

#define SIZE_MASTER_ACC     5
#define SIZE_OPEN_TICKET    100

enum {IDX_MASTER = 0, IDX_SLAVE};

struct TICKET_INFO
{
    int MasterAcc;
    int TicketCnt;
    int TicketPair[SIZE_OPEN_TICKET][2];
};


class CTicketList
{
public:
    CTicketList();
    ~CTicketList();
    
    // add info
    void    AddTicketPair     (int nMasterAcc, int nMasterTicket, int nSlaveTicket);
    
    // return the slave ticket.
    int     GetSlaveTicket (int nMasterAcc,  int nMasterTicket);
    //int     FindMasterTicket(int nMasterTicket, /*out*/int& nSaveTicket,    /*out*/int& nMasterAcc); 

    // delete one ticket pair. Being called when the slaveticket doesn't exist.
    void    DeleteTicketPair(int nMasterAcc,  int nMasterTicket);

    //  clear array
    void    Clear(bool bAll, int nMasterAcc);  
    
    void    PrintAll(int nMasterAcc);

private:
    // return idx of the master acc
    int     FindMasterAcc(int nMasterAcc);  
    
    int     FindMasterTicket(int nMasterAcc, int nMasterTicket, int& nMasterIdx);  
    
    // clear the ticket array 
    void    ClearOneMasterAcc(TICKET_INFO &refTicketInfo);
    
    
    
private:
    TICKET_INFO     m_arrTicket[SIZE_MASTER_ACC];
    int             m_nCurrMasterCnt;
    string          m_msg;
    
};



CTicketList::CTicketList()
{
    m_nCurrMasterCnt = 0;
    Clear(true,0);
}

CTicketList::~CTicketList()
{
}




void CTicketList::PrintAll(int nMasterAcc)
{
    int idx = FindMasterAcc(nMasterAcc);
    if(idx<0){
        printlog(StringFormat("master acc err:%d", nMasterAcc));
        return;
        }
        
    for( int i=0; i<m_arrTicket[idx].TicketCnt; i++)
    {
        m_msg = StringFormat("[%d][%d](%d)(%d)", 
                        nMasterAcc,
                        i, 
                        m_arrTicket[idx].TicketPair[i][IDX_MASTER], 
                        m_arrTicket[idx].TicketPair[i][IDX_SLAVE]
                        );
        printlog(m_msg);
        Comment(m_msg);
    }
}

// return idx of the master acc
int CTicketList::FindMasterAcc(int nMasterAcc)
{
    int idx = -1;
    for( int i=0; i<SIZE_MASTER_ACC;i++)
    {
        if( m_arrTicket[i].MasterAcc==0)
            break;
        
        if( m_arrTicket[i].MasterAcc==nMasterAcc)
        {
            idx = i;
            break;
        }            
    }
    
    return idx;
}


int  CTicketList::FindMasterTicket(int nMasterAcc, int nMasterTicket, int& nMasterIdx)
{
    nMasterIdx = FindMasterAcc(nMasterAcc);
    if(nMasterIdx<0)
        return -1;
        
    int idxMTicket = ArrayBsearch(m_arrTicket[nMasterIdx].TicketPair, nMasterTicket, WHOLE_ARRAY, 0, MODE_DESCEND);
    if( m_arrTicket[nMasterIdx].TicketPair[idxMTicket][IDX_MASTER]!= nMasterTicket)
        return -1;
     
     
    return idxMTicket;
}

// return the slave ticket. when bClose, remove this info
int  CTicketList::GetSlaveTicket (int nMasterAcc,  int nMasterTicket)
{
    int idxMAcc;
    int idxMTicket = FindMasterTicket(nMasterAcc, nMasterTicket, idxMAcc);
    if(idxMTicket<0)
        return -1;
            
    int nSlaveTicket = m_arrTicket[idxMAcc].TicketPair[idxMTicket][IDX_SLAVE]; 
    
    //if(bClose)
    //{
    //    m_arrTicket[idxMAcc].TicketPair[idxMTicket][IDX_MASTER] = 0;
    //    m_arrTicket[idxMAcc].TicketPair[idxMTicket][IDX_SLAVE] = 0;
    //    m_arrTicket[idxMAcc].TicketCnt--;
    //    ArraySort(m_arrTicket[idxMAcc].TicketPair, WHOLE_ARRAY, 0, MODE_DESCEND);
    //}
    
    return nSlaveTicket;
}

// delete one ticket pair
void  CTicketList::DeleteTicketPair(int nMasterAcc,  int nMasterTicket)
{
    int idxMAcc, idxMTicket;
    idxMTicket = FindMasterTicket(nMasterAcc, nMasterTicket, idxMAcc);
    //printlog(StringFormat("[%d][%d][%d]", nMasterAcc, nMasterTicket, idxMTicket));
    if(idxMTicket<0)
        return;
    
    int nSlaveTicket = m_arrTicket[idxMAcc].TicketPair[idxMTicket][IDX_SLAVE];
    
    m_arrTicket[idxMAcc].TicketPair[idxMTicket][IDX_MASTER] = 0;
    m_arrTicket[idxMAcc].TicketPair[idxMTicket][IDX_SLAVE] = 0;
    m_arrTicket[idxMAcc].TicketCnt--;
    
    ArraySort(m_arrTicket[idxMAcc].TicketPair, WHOLE_ARRAY, 0, MODE_DESCEND);
    
    printlog(StringFormat("DEL[%d][%d][%d]-[Master has Orders(%d)", nMasterAcc, nMasterTicket, nSlaveTicket, m_arrTicket[idxMAcc].TicketCnt));

}
    
void CTicketList::Clear(bool bAll, int nMasterAcc)
{
    if(bAll)
    {
        for(int i=0; i<SIZE_MASTER_ACC; i++)
        {
            ClearOneMasterAcc(m_arrTicket[i]);
        }
        m_nCurrMasterCnt = 0;
    }
    else
    {
        int idx = FindMasterAcc(nMasterAcc);
        if(idx>0){
            ClearOneMasterAcc(m_arrTicket[idx]);
        }
            
    }
}


void CTicketList::ClearOneMasterAcc(TICKET_INFO &refTicketInfo)
{
    for(int i=0; i<SIZE_OPEN_TICKET; i++)
    {
        for(int j=0; j<2; j++)
        {
            refTicketInfo.TicketPair[i,j] = 0;
        }
    }
    refTicketInfo.TicketCnt = 0;
    refTicketInfo.MasterAcc = 0;
    m_nCurrMasterCnt--;
}
    
void CTicketList::AddTicketPair(int nMasterAcc, int nMasterTicket, int nSlaveTicket)
{
    int idxMAcc;
    int idxMTicket = FindMasterTicket(nMasterAcc, nMasterTicket, idxMAcc);
    
    // find 
    if(idxMTicket>0)
    {
        m_arrTicket[idxMAcc].TicketPair[idxMTicket][IDX_MASTER] = nMasterTicket; 
        m_arrTicket[idxMAcc].TicketPair[idxMTicket][IDX_SLAVE] = nSlaveTicket;
    }
    else
    {
        // the 1st time of Master Account
        if( idxMAcc<0 )
        {
            //printlog(StringFormat("Master Acc is no[%d][%d]", nMasterAcc, m_nCurrMasterCnt));
            idxMAcc     = m_nCurrMasterCnt++;
            idxMTicket  = 0;
            m_arrTicket[idxMAcc].MasterAcc = nMasterAcc;
        }
        else
        {
            // the 1st time of Master Ticket (New)
            //printlog(StringFormat("Master Acc ok, ticet no [%d][%d][%d][%d]", 
            //    nMasterAcc, m_nCurrMasterCnt, nMasterTicket, m_arrTicket[idxMAcc].TicketCnt));
            idxMTicket = m_arrTicket[idxMAcc].TicketCnt;
        }
        m_arrTicket[idxMAcc].TicketPair[idxMTicket][IDX_MASTER] = nMasterTicket; 
        m_arrTicket[idxMAcc].TicketPair[idxMTicket][IDX_SLAVE] = nSlaveTicket;
        m_arrTicket[idxMAcc].TicketCnt++; 
    }
    
    printlog(StringFormat("ADD[%d][%d][%d]-[Master has Orders(%d)", nMasterAcc, nMasterTicket, nSlaveTicket, m_arrTicket[idxMAcc].TicketCnt));
    
    // Sorting for BSearch. Desc mode as this array is filled with zero(0).
    ArraySort(m_arrTicket[idxMAcc].TicketPair, WHOLE_ARRAY, 0, MODE_DESCEND);
}










#endif // _CTICKET_LIST_H