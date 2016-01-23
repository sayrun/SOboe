#ifndef	__RSTREAM_HPP__
#define	__RSTREAM_HPP__

class CRStream
{
public:
	CRStream( int nWorkSize = 100);
	~CRStream( void);

	int AddReceiveString( const char* const pszReceiveString);

	int GetHeadStringLength( void);
	int GetHeadString( char* pszData, int nDataSize);

	LPCSTR GetWork( void) const;
protected:
	void ReAllocWorkArea( int nWork);

protected:
	char*	m_pszWorkBuffer;
	int		m_nWorkSize;
	int		m_nWorkLen;
};
#endif	//__RSTREAM_HPP__
