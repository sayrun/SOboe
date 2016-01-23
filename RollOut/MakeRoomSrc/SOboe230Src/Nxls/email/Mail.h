#ifndef	__MAIL_H__
#define	__MAIL_H__

class CMail
{
public:
	CMail( LPCSTR lpcszMail);
	~CMail( void);

	BOOL SetMailSource( LPCSTR lpcszMail);

	BOOL DoDecode( void);

	LPCSTR GetMessageID( void) const;
	LPCSTR GetMailer( void) const;
	LPCSTR GetFrom( void) const;
	LPCSTR GetSubject( void) const;
	LPCSTR GetBody( void) const;

	int GetRawDataSize( void) const;
	BOOL GetRawData( LPBYTE lpabyRawData, int nSize) const;

protected:
	BOOL DecodeHeader( void);
	BOOL DecodeBody( void);

protected:
	static char*	sm_pszFrom;
	static int		sm_nFrom;
	static char*	sm_pszSubject;
	static int		sm_nSubject;
	static char*	sm_pszMultiPart;
	static int		sm_nMultiPart;
	static char*	sm_pszBoundary;
	static int		sm_nBoundary;

	static char*	sm_pszPlain;
	static int		sm_nPlain;
	static char*	sm_pszCharset;
	static int		sm_nCharset;
	static char*	sm_pszEncoding;
	static int		sm_nEncoding;
	static char*	sm_pszBase64;
	static int		sm_nBase64;
	static WORD		sm_wBS;

	static char*	sm_pszRawData;
	static int		sm_nRawData;

	static char*	sm_pszTextType;
	static int		sm_nTextType;
	static char*	sm_pszChipData;
	static int		sm_nChipData;

	LPSTR	m_lpHead;
	LPSTR	m_lpBody;

	BOOL	m_blExchanged;
	LPSTR	m_lpszMessageID;
	LPSTR	m_lpszMailer;
	LPSTR	m_lpszFrom;
	LPSTR	m_lpszSubject;
	LPSTR	m_lpszMBoundary;
	LPSTR	m_lpszBody;
	BOOL	m_blBase64Encoding;

	LPBYTE	m_lpbyRawData;
	int		m_nRawDaraSize;
};

#include "Mail.inl"

#endif	//__MAIL_H__