#ifndef	__POP3CLIENT_H__
#define	__POP3CLIENT_H__

#if(_WIN32_WINNT < 0x0400)

typedef unsigned long ULONG_PTR;

typedef ULONG_PTR HCRYPTPROV;
typedef ULONG_PTR HCRYPTKEY;
typedef ULONG_PTR HCRYPTHASH;
typedef unsigned int ALG_ID;

#define PROV_RSA_FULL           1
#define CRYPT_NEWKEYSET         0x00000008

#define ALG_CLASS_HASH                  (4 << 13)
#define ALG_TYPE_ANY                    (0)
#define ALG_SID_MD5                     3

#define CALG_MD5                (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_MD5)

#define HP_HASHVAL              0x0002  // Hash value

typedef DECLSPEC_IMPORT BOOL ( WINAPI* LPCRYPTACQUIRECONTEXT)( HCRYPTPROV *phProv, LPCSTR szContainer, LPCSTR szProvider, DWORD dwProvType, DWORD dwFlags);
typedef DECLSPEC_IMPORT BOOL ( WINAPI* LPCRYPTCREATEHASH)( HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH *phHash);
typedef DECLSPEC_IMPORT BOOL ( WINAPI* LPCRYPTHASHDATA)( HCRYPTHASH hHash, CONST BYTE *pbData, DWORD dwDataLen, DWORD dwFlags);
typedef DECLSPEC_IMPORT BOOL ( WINAPI* LPCRYPTGETHASHPARAM)( HCRYPTHASH hHash, DWORD dwParam, BYTE *pbData, DWORD *pdwDataLen, DWORD dwFlags);
typedef DECLSPEC_IMPORT BOOL ( WINAPI* LPCRYPTDESTROYHASH)( HCRYPTHASH hHash);
typedef DECLSPEC_IMPORT BOOL ( WINAPI* LPCRYPTRELEASECONTEXT)( HCRYPTPROV hProv, DWORD dwFlags);

class CAdvApi32
{
public:
	CAdvApi32();
	~CAdvApi32();

	BOOL IsEnable()
	{
		return ( NULL != m_hAdvapi32DLL);
	}

	BOOL CryptAcquireContext( HCRYPTPROV *phProv, LPCSTR szContainer, LPCSTR szProvider, DWORD dwProvType, DWORD dwFlags)
	{
		return m_lpfnCryptAcquireContext( phProv, szContainer, szProvider, dwProvType, dwFlags);
	}
	BOOL CryptCreateHash( HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH *phHash)
	{
		return m_lpfnCryptCreateHash( hProv, Algid, hKey, dwFlags, phHash);
	}
	BOOL CryptHashData( HCRYPTHASH hHash, CONST BYTE *pbData, DWORD dwDataLen, DWORD dwFlags)
	{
		return m_lpfnCryptHashData( hHash, pbData, dwDataLen, dwFlags);
	}
	BOOL CryptGetHashParam( HCRYPTHASH hHash, DWORD dwParam, BYTE *pbData, DWORD *pdwDataLen, DWORD dwFlags)
	{
		return m_lpfnCryptGetHashParam( hHash, dwParam, pbData, pdwDataLen, dwFlags);
	}
	BOOL CryptDestroyHash( HCRYPTHASH hHash)
	{
		return m_lpfnCryptDestroyHash( hHash);
	}
	BOOL CryptReleaseContext( HCRYPTPROV hProv, DWORD dwFlags)
	{
		return m_lpfnCryptReleaseContext( hProv, dwFlags);
	}

private:
	HINSTANCE m_hAdvapi32DLL;

	LPCRYPTACQUIRECONTEXT m_lpfnCryptAcquireContext;
	LPCRYPTCREATEHASH m_lpfnCryptCreateHash;
	LPCRYPTHASHDATA m_lpfnCryptHashData;
	LPCRYPTGETHASHPARAM m_lpfnCryptGetHashParam;
	LPCRYPTDESTROYHASH m_lpfnCryptDestroyHash;
	LPCRYPTRELEASECONTEXT m_lpfnCryptReleaseContext;
};
#endif

#define	PORT_POP3	110

// POP3を管理するんだよ
// 考えてみれば、SMTPもclassにすれば良かったんじゃねぇの？ってカンジじゃ。
class CPop3Client
{
public:
	CPop3Client( bool blApop = false);
	~CPop3Client( void);

	BOOL Create( void);
	BOOL Connect( LPCSTR lpcszServerName, UINT unPort = PORT_POP3);

	BOOL LogIn( LPCSTR lpcszUser, LPCSTR lpcszPass);

	int GetMessageCount( void);
	BOOL TestMailer( int nIndex, LPSTR lpszMail);
	int GetMessageSize( int nIndex);
	int GetMessageBody( int nIndex, LPSTR lpszMail, int nSize);
	int GetMessageBody( int nIndex, LPSTR* lppszMail);
	BOOL DeleteMessage( int nIndex);

	BOOL Quit( void);

	BOOL Disconnect( void);

	HANDLE GetAbortEvent( void) const;

protected:
	void CleanUp( void);
	BOOL CheckResult( LPSTR* lppszResult = NULL);

	BOOL GetPopStatus( void);
	int CheckPopStat( void);
	BOOL CheckPopList( int *panSize, int nCount);

	BOOL LogInAPOPMode( LPCSTR lpcszUser, LPCSTR lpcszPass);
	BOOL LogInNormalMode( LPCSTR lpcszUser, LPCSTR lpcszPass);

	LPSTR CreateDigest( LPCSTR lpcszPass);


protected:
	SOCKET	m_sSocket;
	HANDLE	m_hAbortEvent;
	HANDLE	m_hSockEvent;

	long	m_ulTimeOunt;

	int m_nMessageCount;
	int *m_panMassageSize;

	bool m_blAPOP;
	char* m_pszAPOPKey;

#if(_WIN32_WINNT < 0x0400)
	static CAdvApi32 m_cAdvApi32;
#endif

};
#endif	//__POP3CLIENT_H__