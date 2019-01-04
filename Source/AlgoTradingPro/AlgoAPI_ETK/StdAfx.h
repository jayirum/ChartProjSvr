// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__03BDAA55_6CFE_4FB1_8FCC_807F84D7A3A1__INCLUDED_)
#define AFX_STDAFX_H__03BDAA55_6CFE_4FB1_8FCC_807F84D7A3A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "IXingAPI.h"
extern IXingAPI		g_iXingAPI;
extern	BOOL		g_bShowRaw;



//////////////////////////////////////////////////////////////////////////
//	CUSTOM STRUCTURE

typedef struct _API_INFO
{
	char	API_USERID	[20];
	char	API_ACNT_NO	[20];
	char	API_CONN_PWD[20];
	char	API_CRTF_PWD[20];
	char	API_ACNT_PWD[20];
	char	LOGON_YN	[1];	
}API_INFO;

//////////////////////////////////////////////////////////////////////////
//	CUSTOMER 변수
extern API_INFO	g_stAPI;
#define DEF_MSG_LEN	25




#define WMU_SHOW_TR		( WM_USER + 1000 )
#define WMU_SET_MESSAGE	( WM_USER + 1001 )


#define MY_API_CODE		"KS_ETK"
#define	MY_NTTN_CODE	"D2"

enum
{
	DATA_TYPE_STRING = 0,		// 문자열
	DATA_TYPE_LONG,				// 정수
	DATA_TYPE_FLOAT,			// 실수
	DATA_TYPE_FLOAT_DOT,		// 실수( 소숫점을 가지고 있음 )
};

CString GetDispData( char* psData, int nSize, int nType, int nDotPos=0 );
void SetPacketData( char* psData, int nSize, LPCTSTR pszSrc, int nType, int nDotPos=0 );
void SetPacketData( char* psData, int nSize, CWnd* pWnd, int nID, int nType, int nDotPos=0 );


//시세중계
#define DEF_SISE_RELAY	0


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__03BDAA55_6CFE_4FB1_8FCC_807F84D7A3A1__INCLUDED_)
