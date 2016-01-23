#include "StdeMail.h"
#include "RStream.hpp"


CRStream::CRStream( int nWorkSize)
{
	m_pszWorkBuffer = NULL;
	m_nWorkSize = 0;
	m_nWorkLen = 0;
	ReAllocWorkArea( nWorkSize);
}

CRStream::~CRStream( void)
{
	if( NULL != m_pszWorkBuffer)
	{
		free( m_pszWorkBuffer);
		m_pszWorkBuffer = NULL;
	}
	m_nWorkSize = 0;
	m_nWorkLen = 0;
}

int CRStream::AddReceiveString( const char* const pszReceiveString)
{
	if( NULL == pszReceiveString)return 0;


	const char*	pszWork = pszReceiveString;
	if( NULL != m_pszWorkBuffer)
	{
		if( 0 >= lstrlen( m_pszWorkBuffer))
		{
			int					nIndex = 0;
			if( '\r' == pszReceiveString[ nIndex] || '\n' == pszReceiveString[ nIndex])
			{
				for( ; nIndex < lstrlen( pszReceiveString); nIndex++)
				{
					if( '\r' != pszReceiveString[ nIndex] && '\n' != pszReceiveString[ nIndex])break;
					if( 0 == pszReceiveString[ nIndex])break;
				}
			}
			pszWork = &pszReceiveString[ nIndex];
		}
	}

	int nLen = lstrlen( pszWork);
	if( 0 == nLen)return 0;

	ReAllocWorkArea( nLen + m_nWorkLen + 1);
	lstrcat( m_pszWorkBuffer, pszWork);
	m_nWorkLen += nLen;

	return GetHeadStringLength();
}

int CRStream::GetHeadStringLength( void)
{
	if( NULL != m_pszWorkBuffer)
	{
		for( int nIndex = 0; nIndex < m_nWorkLen; nIndex++)
		{
			if( '\r' == m_pszWorkBuffer[ nIndex] || '\n' == m_pszWorkBuffer[ nIndex])return nIndex;
		}
	}
	return 0;
}

int CRStream::GetHeadString( char* pszData, int nDataSize)
{
	int nHeadStrLen = GetHeadStringLength();
	if( nHeadStrLen < nDataSize)
	{
		for( int nIndex = 0; nIndex < m_nWorkLen; nIndex++)
		{
			if( '\r' == m_pszWorkBuffer[ nIndex] || '\n' == m_pszWorkBuffer[ nIndex])
			{
				pszData[ nIndex] = 0;
				LPSTR lpsz = NULL;
				for( ; nIndex < m_nWorkLen; nIndex++)
				{
					if( '\r' != m_pszWorkBuffer[ nIndex] && '\n' != m_pszWorkBuffer[ nIndex])
					{
						lpsz = &m_pszWorkBuffer[ nIndex];
						break;
					}
				}
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
			pszData[ nIndex] = m_pszWorkBuffer[ nIndex];
		}
	}
	return nHeadStrLen;
}

void CRStream::ReAllocWorkArea( int nWork)
{
	if( 0 >= nWork)return;
	if( m_nWorkSize >= nWork)return;

	m_pszWorkBuffer = ( char*)realloc( m_pszWorkBuffer, nWork);
	ZeroMemory( &m_pszWorkBuffer[ m_nWorkSize], nWork - m_nWorkSize);
	m_nWorkSize = nWork;
	/*
	char*	pszTemp;
	pszTemp = ( char*)malloc( nWork);
	FillMemory( pszTemp, nWork, 0);
	if( m_pszWorkBuffer)
	{
		lstrcpy( pszTemp, m_pszWorkBuffer);
		free( m_pszWorkBuffer);
	}
	m_pszWorkBuffer = pszTemp;
	m_nWorkSize = nWork;
	m_nWorkLen = lstrlen( m_pszWorkBuffer);
	*/
}

LPCSTR CRStream::GetWork( void) const
{
	return m_pszWorkBuffer;
}
