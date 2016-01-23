#ifndef	__MAILS_H__
#define	__MAILS_H__

#include "NetEx_IF.h"

// 送信に使用するメイルを管理するクラス
// 受信側トラ蹴る為にSがついてるの

class CMailS
{
public:
	CMailS( void);
	~CMailS( void);

	BOOL SetToAddress( const ENTRYDATA** pstEntry, int nCount);
	BOOL SetFromAddress( LPCSTR lpcszAddress, LPCSTR lpcszRealName = NULL);
	BOOL SetMailBody( const SENDDATA* pstSendData, int nSendDataSize, LPCSTR lpcszSignaturePath);
	BOOL SetMailBody( const RESULTDATA* pResultData, int nResultDataSize, LPCSTR lpcszSignaturePath);
	BOOL SetSubject( LPCSTR lpcszSubject);

	int GetToCount( void) const;
	LPCSTR GetToAddress( int nIndex) const;
	LPCSTR GetToAddressRaw( int nIndex) const;
	LPCSTR GetFromAddress( void) const;
	LPCSTR GetSubject( void) const;

	LPCSTR GetBody( void) const;

	int	GetRawDataSize( void) const;
	LPVOID GetRawData( void) const;

protected:
	int		m_nToAddressCount;
	char**	m_papszToAddress;
	char**	m_papszToAddressRaw;
	char*	m_pszFromAddress;

	char*	m_pszSubject;
	char*	m_pszBody;

	LPVOID	m_lpvRawData;
	int		m_nRawDataSize;

	static LPCSTR	m_lpcszCheck;
	static LPCSTR	m_lpcszOtherA;
	static LPCSTR	m_lpcszOtherD;
};

#include "MailS.inl"

#endif	//__MAILS_H__
