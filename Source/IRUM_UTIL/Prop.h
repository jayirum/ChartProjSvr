/////////////////////////////////////////////////////////////////////////////
// (주) 에스엔뱅크
// 2001. 6. 1	Made by Park. Jun-Young
/////////////////////////////////////////////////////////////////////////////
// Prop.h: interface for the CProp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROP_H__393A6081_775C_11D3_A7DD_0090278BFE96__INCLUDED_)
#define AFX_PROP_H__393A6081_775C_11D3_A7DD_0090278BFE96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef _WINDOWS_
#undef _WINDOWS_
#endif

#include <Windows.h>
#include <comdef.h>
//##ModelId=42046BE201A3
class CProp  
{
protected:
	VOID DumpErr(LONG lErrNo);
	//##ModelId=42046BE201AD
	HKEY m_hKey;
	//##ModelId=42046BE201B8
	char szReturn[256];
	
	char m_szErrMsg[4096];
public:
	BOOL EnumValue(LONG lIndex, LPCSTR lpszName, VARIANT *pvValue);
	
	LPCSTR GetErrorMsg();
	
	//##ModelId=42046BE201C2
	CProp();
	CProp(HKEY hKey, LPCTSTR lpszSub, BOOL bQueryOnly = TRUE);

	//##ModelId=42046BE201CC
	virtual ~CProp();
	
	//##ModelId=42046BE201CE
	BOOL SetBaseKey(HKEY hKey, LPCTSTR lpszSub, BOOL bQueryOnly = TRUE);
	//##ModelId=42046BE201E0
	long GetLongValue(LPCTSTR lpszName);
	//##ModelId=42046BE201E2
	char *GetValue(LPCTSTR lpszName);
	//##ModelId=42046BE201FE
	BOOL IsExistValue(LPCTSTR lpszName);

	//##ModelId=42046BE20208
	BOOL SetValue(LPCTSTR lpszName, long lValue);
	//##ModelId=42046BE2021C
	BOOL SetValue(LPCTSTR lpszName, LPCTSTR lpszValue);
	//##ModelId=42046BE20226
	BOOL DeleteValue(LPCTSTR lpszName);

	//##ModelId=42046BE20230
	BOOL EnumStringValue(long lIndex, LPCTSTR lpszName, LPCTSTR lpszValue);
	//##ModelId=42046BE20245
	BOOL EnumKey(long lIndex, LPCTSTR lpszName);
	
	BOOL CreateRegKey(LPCTSTR lpszName);

	BOOL CProp::DeleteKey(LPCTSTR strKeyName);

	//##ModelId=42046BE2024F
	void Close();
};

#endif // !defined(AFX_PROP_H__393A6081_775C_11D3_A7DD_0090278BFE96__INCLUDED_)
