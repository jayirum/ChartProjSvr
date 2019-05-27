#pragma once
#pragma warning(disable:4996)

#define DEF_DELI    0x01

#define FDS_CODE     1
#define FDS_COMMAND  2   //  ORDER, COMMAND
#define FDS_SYS      3   //  MT4, 
#define FDS_TM_HEADER  4   //  yyyymmdd_hhmmssmmm
#define FDS_BROKER   5
#define FDS_REGUNREG 6   //  R, U
#define FDS_SYMBOL   7
#define FDS_ORD_ACTION   8   // O:Open, C:Close, P:Partial
#define FDS_OPENED_TM    10
#define FDS_CLOSED_TM    11
#define FDS_COMMENTS     12
#define FDS_LIVEDEMO     13

#define FDN_ACCNO_MY         100
#define FDN_ACCNO_MASTER     101
#define FDN_OPENED_TICKET    103
#define FDN_CLOSED_TICKET    104
#define FDN_ORD_TYPE         105 // OP_BUY, OP_SELL, OP_BUYLIMIT, OP_SELLLIMIT, OP_BUYSTOP, OP_SELLSTOP

const char SYS_MT4[] = "MT4";
const char SYS_NINJA[] = "NINJA";
const char SYS_ZORRO[] = "ZORRO";


const char CODE_REG_MASTER[] = "1001";
const char CODE_REG_SLAVE[] = "1002";
const char CODE_MASTER_ORDER[] = "1003";
const char CODE_PUBLISH_ORDER[] = "1004";

const char TP_COMMAND = 'C';
const char TP_ORDER = 'O';
const char TP_REG = 'R';
const char TP_UNREG = 'U';

#define FDD_OPENED_PRC   200
#define FDD_CLOSED_PRC   201  
#define FDD_OPENED_LOTS  202
#define FDD_CLOSED_LOTS  203
#define FDD_SLPRC        204
#define FDD_PTPRC        205
#define FDD_PROFIT       206
#define FDD_SWAP         207
#define FDD_COMMISSION   208

#define MAX_BUF 1024
#define DOT_CNT 5
#define MAX_BUF	1024

#include <string>
#include <map>
using namespace std;


class CProtoSet
{
public:
	CProtoSet();
	~CProtoSet();

	void Begin();
	int Complete(/*out*/string& result);
	int Complete(/*out*/char* pzResult);

	void CopyFromRecvData(char* pzData);
	void SetVal(int nFd, string val);
	void SetVal(int nFd, char* val);
	void SetVal(int nFd, int val);
	void SetVal(int nFd, double val);
private:
	void DelSameField(int nFd);
private:
	string	m_sBuf;
	char	m_zTemp[MAX_BUF];
};

struct ST_VAL
{
	string	sVal;
	int		nVal;
	double	dVal;
};

class CProtoGet
{
public:
	CProtoGet();
	~CProtoGet();

	int		Parsing(_In_  char* pData, int nDataLen);
	char*	GetRecvData() { return m_zBuf; }
	bool	GetCode(_Out_ string& sCode);
	bool	GetVal(int nFd, _Out_ char* pzVal);
	bool	GetVal(int nFd, _Out_ string* psVal);
	bool	GetVal(int nFd, _Out_ int* pnVal);
	bool	GetVal(int nFd, _Out_ double* pdVal);

private:
	void SetValByField(int nFd, char* pzVal, ST_VAL* pVal);
	void SetValByField(char* pzFd, char* pzVal, ST_VAL* pVal);
	bool IsParsed();
private:
	char				m_zBuf[MAX_BUF];
	map<int, ST_VAL>	m_mapResult;
};

