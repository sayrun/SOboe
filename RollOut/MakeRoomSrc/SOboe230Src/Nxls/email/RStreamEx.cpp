#include "StdeMail.h"
#include "RStreamEx.h"

int CRStreamEx::AddReceiveString( const char* const pszReceiveString)
{
	if( NULL == pszReceiveString)return 0;

	const char*	pszWork = pszReceiveString;

	int nLen = lstrlen( pszWork);
	if( 0 == nLen)return 0;
	ReAllocWorkArea( nLen + m_nWorkLen + 1);
	lstrcat( m_pszWorkBuffer, pszWork);
	m_nWorkLen += nLen;

	return GetHeadStringLength();
}

int CRStreamEx::GetHeadStringLength( void)
{
	if( NULL != m_pszWorkBuffer)
	{
		for( int nIndex = 0; nIndex < m_nWorkLen; nIndex++)
		{
			if( '\n' == m_pszWorkBuffer[ nIndex])return ( nIndex + 1);
		}
	}
	return 0;
}

int CRStreamEx::GetHeadString( char* pszData, int nDataSize)
{
	int nHeadStrLen = GetHeadStringLength();
	if( nHeadStrLen < nDataSize)
	{
		for( int nIndex = 0; nIndex < m_nWorkLen; nIndex++)
		{
			pszData[ nIndex] = m_pszWorkBuffer[ nIndex];
			if( '\n' == m_pszWorkBuffer[ nIndex])
			{
				pszData[ nIndex + 1] = 0;
				LPSTR lpsz = NULL;
				lpsz = &m_pszWorkBuffer[ nIndex + 1];
				if( NULL != lpsz)
				{
					int nNewLen = lstrlen( lpsz);
					int nIndex;
					for( nIndex = 0; nIndex < nNewLen; nIndex++)
					{
						m_pszWorkBuffer[ nIndex] = lpsz[ nIndex];
					}
					for( ; nIndex < m_nWorkSize; nIndex++)
					{
						m_pszWorkBuffer[ nIndex] = 0;
					}
					m_nWorkLen = lstrlen( m_pszWorkBuffer);
				}
				else
				{
					FillMemory( m_pszWorkBuffer, m_nWorkSize, 0);
					m_nWorkLen = 0;
				}
				break;
			}
		}
	}
	return nHeadStrLen;
}
