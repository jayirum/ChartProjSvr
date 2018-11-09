//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: LOG_SUCCESS
//
// MessageText:
//
//  작업을 성공적으로 수행했습니다. 
//  (%1)
//
#define LOG_SUCCESS                      ((DWORD)0x00000001L)

//
// MessageId: LOG_INFORMATION
//
// MessageText:
//
//  DELIVERY 정보 Log
//  (%1)
//
#define LOG_INFORMATION                  ((DWORD)0x40000002L)

//
// MessageId: LOG_WARNING
//
// MessageText:
//
//  DELIVERY 경고 Log
//  (%1)
//
#define LOG_WARNING                      ((DWORD)0x80000003L)

//
// MessageId: LOG_ERROR
//
// MessageText:
//
//  DELIVERY 오류 Log
//  (%1)
//
#define LOG_ERROR                        ((DWORD)0xC0000004L)

