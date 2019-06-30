#ifndef _LOAD_CONFIG_H
#define _LOAD_CONFIG_H


#define SERVER_IP           "110.4.89.206"
#define SERVER_READ_PORT    "64999"
#define MASTER_SUB_PORT     "64998"
#define SLAVE_SUB_PORT      "64997"


bool Slave_GetChannel(string sMasterId, string& sSendChannel, string& sRecvChannel)
{
    //TODO. WebRequest
    
    sSendChannel = StringFormat("%s:%s", SERVER_IP, SERVER_READ_PORT);
    sRecvChannel = StringFormat("%s:%s", SERVER_IP, SLAVE_SUB_PORT);
    
    return true;
}





bool Master_GetChannel(string sMasterId, string& sSendChannel, string& sRecvChannel)
{
    //TODO. WebRequest
    
    sSendChannel = StringFormat("%s:%s", SERVER_IP, SERVER_READ_PORT);
    sRecvChannel = StringFormat("%s:%s", SERVER_IP, MASTER_SUB_PORT);
    
    return true;
}












































#endif //_LOAD_CONFIG_H