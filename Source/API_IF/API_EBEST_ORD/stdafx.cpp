
// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// Sample_xingACE.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"


//--------------------------------------------------------------------------------------
// Packet �����͸� Display �� �����ͷ� ��ȯ�Ѵ�.
//--------------------------------------------------------------------------------------
CString GetDispData( char* psData, int nSize, int nType, int nDotPos )
{
	CString strData( psData, nSize );

	// Raw �����͸� �������� �������� SPACE, ���ھ��� 0, �׸��� �Ҽ����� �߰��Ѵ�.
//	if( g_bShowRaw == FALSE )
	{
		//-----------------------------------------------------------------------
		// ���ڿ� : �޺κ��� Space ����
		if( nType == DATA_TYPE_STRING )
		{
			strData.TrimRight();
		}

		//-----------------------------------------------------------------------
		// ���� : ���� 0 �� ����. ���� �ϱ� ���� ������ ��ȯ�� �ٽ� ���ڷ� ��ȯ
		else if( nType == DATA_TYPE_LONG )
		{
			CString strTemp;
			strTemp.Format( "%d", atoi( strData ) );
			strData = strTemp;
		}

		//-----------------------------------------------------------------------
		// �Ǽ� : ���� 0 �� ����. ���� �ϱ� ���� �Ǽ��� ��ȯ�� �ٽ� nDotPos ��ŭ ǥ��
		else if( nType == DATA_TYPE_FLOAT )
		{
			// �Ҽ����� ������ ���� �������̹Ƿ� nDotPos ��ġ�� ���� ��´�.
			strData.Insert( strData.GetLength() - nDotPos, '.' );

			// �Ҽ��� ���ϸ� nDotPos ��ŭ ǥ���ϱ� ���� format�� �����.
			CString strFormat;
			strFormat.Format( "%%.%df", nDotPos );
			CString strTemp;
			strTemp.Format( strFormat, atof( strData ) );
			strData = strTemp;
		}

		//-----------------------------------------------------------------------
		// �Ǽ� : ���� 0 �� ����. ���� �ϱ� ���� �Ǽ��� ��ȯ�� �ٽ� nDotPos ��ŭ ǥ��
		else if( nType == DATA_TYPE_FLOAT_DOT )
		{
			// �Ҽ����� �����ִ�
			int nFind = strData.Find( "." );
			nDotPos = strData.GetLength() - nFind - 1;
			
			// �Ҽ��� ���ϸ� nDotPos ��ŭ ǥ���ϱ� ���� format�� �����.
			CString strFormat;
			strFormat.Format( "%%.%df", nDotPos );
			CString strTemp;
			strTemp.Format( strFormat, atof( strData ) );
			strData = strTemp;
		}
	}

	return strData;
}

void SetPacketData( char* psData, int nSize, CWnd* pWnd, int nID, int nType, int nDotPos )
{
	CString str;
	pWnd->GetDlgItemText( nID, str );
	SetPacketData( psData, nSize, str, nType, nDotPos );
}

void SetPacketData( char* psData, int nSize, LPCTSTR pszSrc, int nType, int nDotPos )
{
	//-----------------------------------------------------------------------
	// ���ڿ�
	if( nType == DATA_TYPE_STRING )
	{
		// ���� ����
		// ���� ���ڸ��� ' ' �� ä��

		// ����ũ�Ⱑ �� �����ͺ��� �۴ٸ� �����ʹ� ©���� �ϹǷ� �����߻�
		int nSrcLen = strlen( pszSrc );

		// ���� Space�� ä���
		FillMemory( psData, nSize, ' ' );

		// �տ����� �����͸� �ִ´�.
		// �������Ͱ� ũ�ٸ� �޺κ��� ������.
		CopyMemory( psData, pszSrc, min( nSize, nSrcLen ) );
	}

	//-----------------------------------------------------------------------
	// ����
	else if( nType == DATA_TYPE_LONG )
	{
		// ������ ����
		// ���� ���ڸ��� '0' ���� ä��

		// ����ũ�Ⱑ �� �����ͺ��� �۴ٸ� �����ʹ� ©���� �ϹǷ� �����߻�
		int nSrcLen = strlen( pszSrc );
		ASSERT( nSize >= nSrcLen );

		// ���� 0 ���� ä���
		FillMemory( psData, nSize, '0' );

		// �ڿ������� �����͸� �ִ´�.
		if( nSize >= nSrcLen )
		{
			CopyMemory( psData+nSize-nSrcLen, pszSrc, nSrcLen );
		}
		// �������Ͱ� ũ�ٸ� ���������� �޺κ��� ������.
		else
		{
			CopyMemory( psData, pszSrc, nSize );
		}
	}

	//-----------------------------------------------------------------------
	// �Ǽ� : �Ҽ����� ���� �ʴ´�.
	else if( nType == DATA_TYPE_FLOAT )
	{
		// �Ҽ��� ��ġ�� �������� ����
		// �Ҽ����� ���� ������ �������� ���ڸ��� �Ҽ����� ���ڸ��� 0���� ä��

		int nSrcLen = strlen( pszSrc );

		// ���� 0 ���� ä���
		FillMemory( psData, nSize, '0' );

		// �������Ϳ��� �Ҽ����� ��ġ�� ã�Ƽ�
		// ���������� �������� ���̿� �Ҽ����� ���̸� ���Ѵ�.
		int nSrcIntLen;
		int nSrcDotLen;
		LPTSTR psz = _tcschr( (LPTSTR)pszSrc, '.' );
		if( psz == NULL )		// �Ҽ��ΰ� ����.
		{
			nSrcIntLen = strlen( pszSrc );
			nSrcDotLen = 0;
		}
		else					// �Ҽ��ΰ� �ִ�.
		{
			nSrcIntLen = psz - pszSrc;
			nSrcDotLen = strlen( pszSrc ) - nSrcIntLen - 1;
		}

		// �����θ� �ִ´�.
		if( nSize-nDotPos >= nSrcIntLen )
		{
			CopyMemory( psData+nSize-nDotPos-nSrcIntLen, pszSrc, nSrcIntLen );
		}
		else
		{
			// ���������� ������ ���̰� �� �� ��� �������� ���ڸ��� �����ȴ�.
			ASSERT( FALSE );
			CopyMemory( psData, pszSrc, nSize-nDotPos );
		}

		// �Ҽ��θ� �ִµ� ���������� �Ҽ��� ���̰� �� �� ��� �Ҽ����� ���ڸ��� �����ȴ�.
		ASSERT( nDotPos >= nSrcDotLen );
		CopyMemory( psData+nSize-nDotPos, pszSrc + strlen( pszSrc ) - nSrcDotLen, min( nDotPos, nSrcDotLen ) );
	}

	//-----------------------------------------------------------------------
	// �Ǽ� : �Ҽ����� ����
	else if( nType == DATA_TYPE_FLOAT_DOT )
	{
		// �Ҽ��� ��ġ�� �������� ����
		// �Ҽ����� ���� ������ �������� ���ڸ��� �Ҽ����� ���ڸ��� 0���� ä��
		
		int nSrcLen = strlen( pszSrc );
		
		// ���� 0 ���� ä���
		FillMemory( psData, nSize, '0' );
		
		// �������Ϳ��� �Ҽ����� ��ġ�� ã�Ƽ�
		// ���������� �������� ���̿� �Ҽ����� ���̸� ���Ѵ�.
		int nSrcIntLen;
		int nSrcDotLen;
		LPTSTR psz = _tcschr( (LPTSTR)pszSrc, '.' );
		if( psz == NULL )		// �Ҽ��ΰ� ����.
		{
			nSrcIntLen = strlen( pszSrc );
			nSrcDotLen = 0;
		}
		else					// �Ҽ��ΰ� �ִ�.
		{
			nSrcIntLen = psz - pszSrc;
			nSrcDotLen = strlen( pszSrc ) - nSrcIntLen - 1;
		}
		
		// �����θ� �ִ´�.
		if( nSize-nDotPos-1 >= nSrcIntLen )
		{
			CopyMemory( psData+nSize-nDotPos-nSrcIntLen-1, pszSrc, nSrcIntLen );
		}
		else
		{
			// ���������� ������ ���̰� �� �� ��� �������� ���ڸ��� �����ȴ�.
			ASSERT( FALSE );
			CopyMemory( psData, pszSrc, nSize-nDotPos-1 );
		}

		// �Ҽ����� ��´�.
		psData[nSize-nDotPos-1] = '.';
		
		// �Ҽ��θ� �ִµ� ���������� �Ҽ��� ���̰� �� �� ��� �Ҽ����� ���ڸ��� �����ȴ�.
		ASSERT( nDotPos >= nSrcDotLen );
		CopyMemory( psData+nSize-nDotPos, pszSrc + strlen( pszSrc ) - nSrcDotLen, min( nDotPos, nSrcDotLen ) );
	}
}
