#include "StdeMail.h"
#include "StrList.h"

CStrList::CStrList( void)
{
	m_pszWorkArea = NULL;
	m_panLineHeads = NULL;
	m_nLineCount = 0;;
}

CStrList::~CStrList( void)
{
	CleanUp();
}

BOOL CStrList::SetStreamData( LPCSTR lpcszData)
{
	if( NULL == lpcszData)return FALSE;

	CleanUp();
	if( 0 < lstrlen( lpcszData))
	{
		int	nSrcLen = lstrlen( lpcszData);
		// 改行の個数を確認します。
		for( int nIndex = 0; nIndex < nSrcLen; nIndex++)
		{
			if( '\n' == lpcszData[ nIndex])m_nLineCount++;
		}
		m_nLineCount++;

		m_pszWorkArea = new char [ nSrcLen + m_nLineCount + 1];
		m_panLineHeads = new int [ m_nLineCount];

		m_panLineHeads[ 0] = 0;
		int nDstIndex = 0;
		int nLineIndex = 0;
		for( nIndex = 0; nIndex < nSrcLen + 1; nIndex++)
		{
			if( '\n' == ( m_pszWorkArea[ nDstIndex] = lpcszData[ nIndex]))
			{
				nDstIndex++;
				m_pszWorkArea[ nDstIndex] = 0;
				nLineIndex++;
				m_panLineHeads[ nLineIndex] = nDstIndex + 1;
			}
			nDstIndex++;
		}

		return TRUE;
	}

	return FALSE;
}

int CStrList::GetCount( void) const
{
	return m_nLineCount;
}

LPCSTR CStrList::GetLine( int nIndex) const
{
	if( 0 > nIndex && m_nLineCount <= nIndex)return NULL;

	return &m_pszWorkArea[ m_panLineHeads[ nIndex]];
}

void CStrList::CleanUp( void)
{
	if( NULL != m_pszWorkArea)
	{
		delete [] m_pszWorkArea;
		m_pszWorkArea = NULL;
	}
	if( NULL != m_panLineHeads)
	{
		delete [] m_panLineHeads;
		m_panLineHeads = NULL;
	}
	m_nLineCount = 0;;
}
