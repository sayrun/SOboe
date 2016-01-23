#include "stdafx.h"
#include "NetExLib.h"
#include "MemoData.h"
#include "RcvMemoData.h"
#include "resource.h"

CNetExLib::CNetExLib() : CObject()
{
	m_hInstance					= NULL;
	m_unNxlID					= 0;
	m_unNxlStyle				= 0;
	m_unVersion					= 0;

	m_blInitialized				= FALSE;
	m_blFunctionAssign			= FALSE;

	m_lpfInitializeNetEx		= NULL;
	m_lpfUnInitializeNetEx		= NULL;
	m_lpfGetNxlStatus			= NULL;
	m_lpfStartEntry				= NULL;
	m_lpfStartEntryEx			= NULL;
	m_lpfAbsenceEntry			= NULL;
	m_lpfExitEntry				= NULL;

	m_lpfLockEntryList			= NULL;
	m_lpfUnlockEntryList		= NULL;
	m_lpfGetGroupCount			= NULL;
	m_lpfGetGroupData			= NULL;
	m_lpfGetEntryCount			= NULL;
	m_lpfGetEntryData			= NULL;
	m_lpfCompareEntryData		= NULL;
	m_lpfGetEntryInfo			= NULL;

	m_lpfSendMemoData			= NULL;
	m_lpfnSendMemoDataInBluk	= NULL;
	m_lpfSendResultData			= NULL;
	m_lpfGetErrorMsg			= NULL;
}

CNetExLib::~CNetExLib()
{
	if( m_hInstance)
	{
		FreeLibrary( m_hInstance);
		m_hInstance = NULL;
	}
}

BOOL CNetExLib::SetPath( LPCSTR lpcszPath)
{
	m_cStrNxlPath = lpcszPath;

	return TRUE;
}

BOOL CNetExLib::LoadNxl( void)
{
	if( m_cStrNxlPath.IsEmpty())return FALSE;
	if( m_hInstance)
	{
		FreeLibrary( m_hInstance);
		m_hInstance = NULL;
	}

	m_hInstance = LoadLibrary( m_cStrNxlPath);
	if( NULL == m_hInstance)
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		wsprintf( szWork, "EC=%d", GetLastError());
		::MessageBox( NULL, szWork, "LoadLibraryの失敗", MB_OK);
#endif
		return FALSE;
	}

	// 機能確認
	NETEXSPECIFICATION	lpfNetExSpecification;
	lpfNetExSpecification = ( NETEXSPECIFICATION)GetProcAddress( m_hInstance, "NetExSpecification");
	if( NULL != lpfNetExSpecification)
	{
		NXLSPECIFICATION	stNxlSpec;
		if( lpfNetExSpecification( &stNxlSpec))
		{
			m_cStrLibName = stNxlSpec.szProtcolString;
			m_unNxlID = stNxlSpec.unNxlID;
			m_unVersion = stNxlSpec.unLowerStructVersion;
			m_unNxlStyle = stNxlSpec.unNxlStyle;

			if( AssignProcAddress())
			{
				m_blFunctionAssign = TRUE;
				return TRUE;
			}
			else
			{
				m_blFunctionAssign = FALSE;
#ifdef	_DEBUG
				::MessageBox( NULL, "AssignProcAddressの失敗", NULL, MB_OK);
#endif
			}
		}
#ifdef	_DEBUG
		else
		{
			::MessageBox( NULL, "NetExSpecification呼び出しの失敗", NULL, MB_OK);
		}
#endif
	}
#ifdef	_DEBUG
	else
	{
		::MessageBox( NULL, "GetProcAddressの失敗", "NetExSpecification", MB_OK);
	}
#endif
	return FALSE;
}

BOOL CNetExLib::AssignProcAddress( void)
{
	if( m_hInstance)
	{
		m_lpfInitializeNetEx = ( INITIALIZENETEX)GetProcAddress( m_hInstance, "InitializeNetEx");
		m_lpfUnInitializeNetEx = ( UNINITIALIZENETEX)GetProcAddress( m_hInstance, "UnInitializeNetEx");
		m_lpfGetNxlStatus = ( GETNXLSTATUS)GetProcAddress( m_hInstance, "GetNxlStatus");
		m_lpfStartEntry = ( STARTENTRY)GetProcAddress( m_hInstance, "StartEntry");
		m_lpfStartEntryEx = ( STARTENTRYEX)GetProcAddress( m_hInstance, "StartEntryEx");
		m_lpfAbsenceEntry = ( ABSENCEENTRY)GetProcAddress( m_hInstance, "AbsenceEntry");
		m_lpfExitEntry = ( EXITENTRY)GetProcAddress( m_hInstance, "ExitEntry");

		m_lpfLockEntryList = ( LOCKENTRYLIST)GetProcAddress( m_hInstance, "LockEntryList");
		m_lpfUnlockEntryList = ( UNLOCKENTRYLIST)GetProcAddress( m_hInstance, "UnlockEntryList");
		m_lpfGetEntryImage = ( GETENTRYIMAGE)GetProcAddress( m_hInstance, "GetEntryImage");
		m_lpfGetGroupCount = ( GETGROUPCOUNT)GetProcAddress( m_hInstance, "GetGroupCount");
		m_lpfGetGroupData = ( GETGROUPDATA)GetProcAddress( m_hInstance, "GetGroupData");
		m_lpfGetEntryCount = ( GETENTRYCOUNT)GetProcAddress( m_hInstance, "GetEntryCount");
		m_lpfGetEntryData = ( GETENTRYDATA)GetProcAddress( m_hInstance, "GetEntryData");
		m_lpfCompareEntryData = ( COMPAREENTRYDATA)GetProcAddress( m_hInstance, "CompareEntryData");
		m_lpfGetEntryInfo = ( GETENTRYINFO)GetProcAddress( m_hInstance, "GetEntryInfo");
		m_lpfInputEntry = ( INPUTENTRY)GetProcAddress( m_hInstance, "InputEntry");
		m_lpfGetExtData = ( GETEXTDATA)GetProcAddress( m_hInstance, "GetExtData");

		m_lpfSendMemoData = ( SENDMEMODATA)GetProcAddress( m_hInstance, "SendMemoData");
		m_lpfnSendMemoDataInBluk = ( SENDMEMODATAINBULK)GetProcAddress( m_hInstance, "SendMemoDataInBluk");
		m_lpfSendResultData = ( SENDRESULTDATA)GetProcAddress( m_hInstance, "SendResultData");

		m_lpfGetErrorMsg = ( GETERRORMSG)GetProcAddress( m_hInstance, "GetErrorMsg");

		if( NULL == m_lpfInitializeNetEx ||
			NULL == m_lpfUnInitializeNetEx ||
			NULL == m_lpfGetNxlStatus ||
			NULL == m_lpfStartEntry ||
			NULL == m_lpfAbsenceEntry ||
			NULL == m_lpfExitEntry ||
			//
			NULL == m_lpfLockEntryList ||
			NULL == m_lpfUnlockEntryList ||
			NULL == m_lpfGetEntryCount ||
			NULL == m_lpfGetEntryData ||
			NULL == m_lpfCompareEntryData ||
			NULL == m_lpfGetEntryInfo)
		{
			m_lpfInitializeNetEx		= NULL;
			m_lpfUnInitializeNetEx		= NULL;
			m_lpfGetNxlStatus			= NULL;
			m_lpfStartEntry				= NULL;
			m_lpfStartEntryEx			= NULL;
			m_lpfAbsenceEntry			= NULL;
			m_lpfExitEntry				= NULL;

			m_lpfLockEntryList			= NULL;
			m_lpfUnlockEntryList		= NULL;
			m_lpfGetGroupCount			= NULL;
			m_lpfGetGroupData			= NULL;
			m_lpfGetEntryCount			= NULL;
			m_lpfGetEntryData			= NULL;
			m_lpfCompareEntryData		= NULL;
			m_lpfGetEntryInfo			= NULL;
			m_lpfInputEntry				= NULL;
			m_lpfGetExtData				= NULL;

			m_lpfSendMemoData			= NULL;
			m_lpfnSendMemoDataInBluk	= NULL;
			m_lpfSendResultData			= NULL;

			m_lpfGetErrorMsg			= NULL;

			return FALSE;
		}

		// 上の判定と一緒にしてもいいンだけど、見にくくなるから分けた
		if( ( IsSupportGrouping() && ( NULL == m_lpfGetGroupCount || NULL == m_lpfGetGroupData)) ||
			( IsSupportSend() && ( NULL == m_lpfSendMemoData || NULL == m_lpfSendResultData)) ||
			( IsSupportInputEntry() && NULL == m_lpfInputEntry) ||
			( IsSupportBulkSend() && NULL == m_lpfnSendMemoDataInBluk))
		{
			m_lpfInitializeNetEx		= NULL;
			m_lpfUnInitializeNetEx		= NULL;
			m_lpfStartEntry				= NULL;
			m_lpfStartEntryEx			= NULL;
			m_lpfAbsenceEntry			= NULL;
			m_lpfExitEntry				= NULL;

			m_lpfLockEntryList			= NULL;
			m_lpfUnlockEntryList		= NULL;
			m_lpfGetGroupCount			= NULL;
			m_lpfGetGroupData			= NULL;
			m_lpfGetEntryCount			= NULL;
			m_lpfGetEntryData			= NULL;
			m_lpfCompareEntryData		= NULL;
			m_lpfGetEntryInfo			= NULL;
			m_lpfInputEntry				= NULL;
			m_lpfGetExtData				= NULL;

			m_lpfSendMemoData			= NULL;
			m_lpfnSendMemoDataInBluk	= NULL;
			m_lpfSendResultData			= NULL;

			return FALSE;
		}

		return TRUE;
	}
	return FALSE;
}

BOOL CNetExLib::FreeNxl( void)
{
	if( m_hInstance)
	{
		FreeLibrary( m_hInstance);
		m_hInstance = NULL;
	}
	m_cStrLibName.Empty();
	m_unVersion = 0;

	return TRUE;
}

const CString& CNetExLib::GetLibName( void) const
{
	return m_cStrLibName;
}

BOOL CNetExLib::IsSupportGrouping( void) const
{
	return ( 0 != ( m_unNxlStyle & _NXL_ENTRYGROUP));
}

BOOL CNetExLib::IsSupportSend( void) const
{
	return ( 0 != ( m_unNxlStyle & _NXL_SEND));
}

BOOL CNetExLib::IsSupportOriginalImage( void) const
{
	return ( 0 != ( m_unNxlStyle & _NXL_SUPPORTORIGINALICON));
}

BOOL CNetExLib::IsSupportExtData( void) const
{
	return ( 0 != ( m_unNxlStyle & _NXL_RESERVED));
}

BOOL CNetExLib::IsSupportInputEntry( void) const
{
	return ( 0 != ( m_unNxlStyle & _NXL_SUPPORTINPUTENTRY));
}

BOOL CNetExLib::IsSupportBulkSend( void) const
{
	return ( 0 != ( m_unNxlStyle & _NXL_SUPPORTBULKSEND));
}

BOOL CNetExLib::IsAssignFunction( void) const
{
	return m_blFunctionAssign;
}

BOOL CNetExLib::IsInitialied( void) const
{
	return m_blInitialized;
}

UINT CNetExLib::GetNxlID( void) const
{
	return m_unNxlID;
}

BOOL CNetExLib::About( HWND hWnd)
{
	NETEXABOUT	lpfNetExAbout;

	lpfNetExAbout = ( NETEXABOUT)GetProcAddress( m_hInstance, "NetExAbout");
	if( NULL != lpfNetExAbout)
	{
		lpfNetExAbout( hWnd);

		return TRUE;
	}
	return FALSE;
}

BOOL CNetExLib::Initialize( HWND hWnd, LPCSTR lpcszEntryName)
{
	BOOL	blResult = FALSE;

	if( NULL != m_lpfInitializeNetEx)
	{
		if( m_lpfInitializeNetEx( hWnd, lpcszEntryName, ReceiveData))
		{
			m_blInitialized = TRUE;
			blResult = TRUE;
		}
		else
		{
			m_blInitialized = FALSE;
			blResult = FALSE;
#ifdef	_DEBUG
			::MessageBox( NULL, "InitializeNetExの失敗", NULL, MB_OK);
#endif
		}
	}

	return blResult;
}

BOOL CNetExLib::UnInitialize( HWND hWnd)
{
	BOOL	blResult = FALSE;

	if( NULL != m_lpfUnInitializeNetEx)
	{
		if( m_lpfUnInitializeNetEx( hWnd))
		{
			m_blInitialized = FALSE;
			blResult = TRUE;
		}
	}

	return blResult;
}

BOOL CNetExLib::IsNxlActive( void) const
{
	BOOL	blResult = FALSE;

	if( NULL != m_lpfGetNxlStatus)
	{
		blResult = ( 0 != ( _NXLSTATUS_ACTIVE & m_lpfGetNxlStatus()));
	}

	return blResult;
}

BOOL CNetExLib::IsReadyReceive( void) const
{
	BOOL	blResult = FALSE;

	if( NULL != m_lpfGetNxlStatus)
	{
		blResult = ( 0 != ( _NXLSTATUS_READY_RECEIVE & m_lpfGetNxlStatus()));
	}

	return blResult;
}

BOOL CNetExLib::IsReadySend( void) const
{
	BOOL	blResult = FALSE;

	if( NULL != m_lpfGetNxlStatus)
	{
#ifdef	_DEBUG
		DWORD dwData = m_lpfGetNxlStatus();
		char	szData[ 128];
		wsprintf( szData, "0x%08X\n", dwData);
		OutputDebugString( szData);
		blResult = ( 0 != ( _NXLSTATUS_READY_SEND & dwData));
#else
		blResult = ( 0 != ( _NXLSTATUS_READY_SEND & m_lpfGetNxlStatus()));
#endif
	}

	return blResult;
}

BOOL CNetExLib::StartEntry( BOOL blAbsence)
{
	BOOL	blResult = FALSE;

	if( m_blInitialized)
	{
		if( NULL != m_lpfStartEntryEx)
		{
			if( m_lpfStartEntryEx( blAbsence))
			{
				blResult = TRUE;
			}
		}
		else
		{
			if( NULL != m_lpfStartEntry)
			{
				if( m_lpfStartEntry())
				{
					blResult = TRUE;
				}
			}
		}
	}

	return blResult;
}

BOOL CNetExLib::AbsenceEntry( BOOL blAbsence)
{
	BOOL	blResult = FALSE;

	if( m_blInitialized)
	{
		if( NULL != m_lpfAbsenceEntry)
		{
			if( m_lpfAbsenceEntry( blAbsence))
			{
				blResult = TRUE;
			}
		}
	}

	return blResult;
}

BOOL CNetExLib::ExitEntry()
{
	BOOL	blResult = FALSE;

	if( m_blInitialized)
	{
		if( NULL != m_lpfExitEntry)
		{
			if( m_lpfExitEntry())
			{
				blResult = TRUE;
			}
		}
	}

	return blResult;
}

BOOL CNetExLib::LockEntryList()
{
	BOOL	blResult = FALSE;

	if( m_blInitialized)
	{
		if( NULL != m_lpfLockEntryList)
		{
			if( m_lpfLockEntryList())
			{
				blResult = TRUE;
			}
		}
	}

	return blResult;
}

BOOL CNetExLib::UnlockEntryList()
{
	BOOL	blResult = FALSE;

	if( m_blInitialized)
	{
		if( NULL != m_lpfUnlockEntryList)
		{
			if( m_lpfUnlockEntryList())
			{
				blResult = TRUE;
			}
		}
	}

	return blResult;
}

int CNetExLib::GetGroupCount()
{
	int nResult = 0;

	if( m_blInitialized)
	{
		if( NULL != m_lpfGetGroupCount)
		{
			nResult = m_lpfGetGroupCount();
		}
	}

	return nResult;
}

BOOL CNetExLib::GetGroupData( int nIndex, CGroupData& cGroupData)
{
	BOOL blResult = FALSE;

	if( m_blInitialized)
	{
		if( NULL != m_lpfGetGroupData)
		{
			int nSize = m_lpfGetGroupData( nIndex, NULL, 0);
			if( 0 < nSize)
			{
				GROUPDATA* pstGroup;
				pstGroup = ( GROUPDATA*)malloc( nSize);
				if( pstGroup)
				{
					m_lpfGetGroupData( nIndex, pstGroup, nSize);
					cGroupData.SetGroupData( pstGroup, nSize);
					free( pstGroup);
					blResult = TRUE;
				}
			}
		}
	}

	return blResult;
}

BOOL CNetExLib::CompareEntryData( const CEntryData& cEntryDataDst, const CEntryData& cEntryDataSrc)
{
	BOOL blResult = FALSE;

	if( m_blInitialized)
	{
		if( NULL != m_lpfCompareEntryData)
		{
			return m_lpfCompareEntryData( cEntryDataDst.AccessEntryData(), cEntryDataDst.GetEntryDataSize(), cEntryDataSrc.AccessEntryData(), cEntryDataSrc.GetEntryDataSize());
		}
	}

	return blResult;
}

void CNetExLib::GetEntryInfo( CString& cStrInfo, const CEntryData& cEntryDataDst)
{
	if( m_blInitialized)
	{
		if( NULL != m_lpfGetEntryInfo)
		{
			int nSize = m_lpfGetEntryInfo( NULL, 0, cEntryDataDst.AccessEntryData(), cEntryDataDst.GetEntryDataSize());
			if( 0 < nSize)
			{
				nSize += 10;	// 10に深い意味はない
				char*	pszWork;
				pszWork = new char [ nSize];
				ZeroMemory( pszWork, nSize);
				m_lpfGetEntryInfo( pszWork, nSize, cEntryDataDst.AccessEntryData(), cEntryDataDst.GetEntryDataSize());
				cStrInfo = pszWork;
				delete [] pszWork;
			}
		}
	}
}

BOOL CNetExLib::DoInputEntry( HWND hWnd, CEntryData& cEntryData)
{
	BOOL blResult = FALSE;
	if( m_blInitialized)
	{
		if( NULL != m_lpfInputEntry)
		{
			HGLOBAL	hgEntry = NULL;
			if( IDOK == m_lpfInputEntry( hWnd, &hgEntry))
			{
				int	nSize = GlobalSize( hgEntry);
				ENTRYDATA* lpEntry = ( ENTRYDATA*)GlobalLock( hgEntry);
				blResult = cEntryData.SetEntryData( lpEntry, nSize);
				GlobalUnlock( hgEntry);
				GlobalFree( hgEntry);
			}
		}
	}
	return blResult;
}

int CNetExLib::GetEntryCount()
{
	int nResult = 0;

	if( m_blInitialized)
	{
		if( NULL != m_lpfGetEntryCount)
		{
			nResult = m_lpfGetEntryCount();
		}
	}

	return nResult;
}

BOOL CNetExLib::GetEntryData( int nIndex, CEntryData& cEntryData)
{
	BOOL blResult = FALSE;

	if( m_blInitialized)
	{
		if( NULL != m_lpfGetEntryData)
		{
			int nSize = m_lpfGetEntryData( nIndex, NULL, 0);
			if( 0 < nSize)
			{
				ENTRYDATA* pstEntry;
				pstEntry = ( ENTRYDATA*)malloc( nSize);
				if( pstEntry)
				{
					m_lpfGetEntryData( nIndex, pstEntry, nSize);
					cEntryData.SetEntryData( pstEntry, nSize);
					free( pstEntry);
					blResult = TRUE;
				}
			}
		}
	}

	return blResult;
}

BOOL CNetExLib::GetEntryImage( CSize& cSize, HBITMAP& hBitmal, COLORREF& clrMask)
{
	BOOL blResult = FALSE;

	if( IsSupportOriginalImage())
	{
		if( m_blInitialized)
		{
			if( NULL != m_lpfGetEntryImage)
			{
				IMAGEDATA	stImageData;
				if( m_lpfGetEntryImage( &stImageData))
				{
					cSize = stImageData.stSize;
					hBitmal = stImageData.hImage;
					clrMask = stImageData.clrMask;
					blResult = TRUE;
				}
			}
		}
	}

	return blResult;
}

int CNetExLib::SendMemoData( const CEntryData* pcEntry, const INNERSENDDATA* pstInnerSendData, HANDLE hCancelEvent)
{
	const ENTRYDATA*	pstEntry = pcEntry->AccessEntryData();

	if( NULL == pstEntry)return -1;
	if( sizeof( ENTRYDATA) > pstEntry->nSize)return -1;

	int	nResult = -1;
	if( m_blInitialized)
	{
		if( m_lpfSendMemoData)
		{
			BYTE*		pbyData;
			SENDDATA*	pstSendData;
			int			nSendDataSize;

			nSendDataSize = CalcSendData( pstInnerSendData);
			pbyData = new BYTE [ nSendDataSize];
			pstSendData = ( SENDDATA*)pbyData;

			ExchangeSendData( pstSendData, nSendDataSize, pstInnerSendData);

			nResult = m_lpfSendMemoData( pstEntry, pstSendData, nSendDataSize, hCancelEvent);

			delete [] pbyData;
			pbyData = NULL;
		}
	}

	return nResult;
}

int CNetExLib::SendMemoDataInBulk( const CList < CEntryData*, CEntryData*&>& cEntryList, int* panResult, int nCount, const INNERSENDDATA* pstInnerSendData, HANDLE hCancelEvent)
{
	int	nResult = -1;
	if( m_blInitialized)
	{
		if( m_lpfnSendMemoDataInBluk)
		{
			int nEntryCount = cEntryList.GetCount();

			CEntryData*			pcEntry;
			const ENTRYDATA**	papstEntry;
			papstEntry = new const ENTRYDATA*[ nEntryCount];
			for( int nIndex = 0; nIndex < nEntryCount; nIndex++)
			{
				pcEntry = cEntryList.GetAt( cEntryList.FindIndex( nIndex));
				papstEntry[ nIndex] = pcEntry->AccessEntryData();
			}

			BYTE*		pbyData;
			SENDDATA*	pstSendData;
			int			nSendDataSize;

			nSendDataSize = CalcSendData( pstInnerSendData);
			pbyData = new BYTE [ nSendDataSize];
			pstSendData = ( SENDDATA*)pbyData;

			ExchangeSendData( pstSendData, nSendDataSize, pstInnerSendData);

			int nResult = m_lpfnSendMemoDataInBluk( papstEntry, panResult, nCount, pstSendData, nSendDataSize, hCancelEvent);

			delete [] pbyData;
			pbyData = NULL;
			delete [] papstEntry;
			papstEntry = NULL;
		}
	}

	return nResult;
}

int CNetExLib::SendResultData( const CEntryData* pcEntry, const INNERRESULTDATA* pstInnerResultData, HANDLE hCancelEvent)
{
	const ENTRYDATA*	pstEntry = pcEntry->AccessEntryData();

	if( NULL == pstEntry)return -1;
	if( sizeof( ENTRYDATA) > pstEntry->nSize)return -1;

	int	nResult = -1;
	if( m_blInitialized)
	{
		if( m_lpfSendResultData)
		{
			BYTE*		pbyData;
			RESULTDATA*	pstResultData;
			int			nResultDataSize;

			nResultDataSize = CalcSendResultData( pstInnerResultData);
			pbyData = new BYTE [ nResultDataSize];
			pstResultData = ( RESULTDATA*)pbyData;

			ExchangeSendResultData( pstResultData, nResultDataSize, pstInnerResultData);

			nResult = m_lpfSendResultData( pstEntry, pstResultData, nResultDataSize, hCancelEvent);

			delete [] pbyData;
			pbyData = NULL;
		}
	}
	return nResult;
}

BOOL CNetExLib::GetErrorMsg( int nCode, CString& cStrErrorMsg)
{
	if( _SEND_USER <= nCode)
	{
		if( m_lpfGetErrorMsg)
		{
			int nLength = m_lpfGetErrorMsg( nCode, NULL, 0);
			if( 0 < nLength)
			{
				char*	pszWork;
				pszWork = new char [ nLength + 1];
				m_lpfGetErrorMsg( nCode, pszWork, nLength + 1);
				cStrErrorMsg = pszWork;
				delete [] pszWork;
				return TRUE;
			}
		}
	}

	switch( nCode)
	{
	case _SEND_ABSENCE:		// 送信したが、相手は留守だった！
		cStrErrorMsg.LoadString( IDS_ABSENCE);
		break;
	case _SEND_FORCEDEND:	// nxlが開放され、処理を中断した
		cStrErrorMsg.LoadString( IDS_FORCEDEND);
		break;
	case _SEND_ERROR:		// 通信中にエラーが発生した
		cStrErrorMsg.LoadString( IDS_SENDERROR);
		break;
	case _SEND_TIMEOUT:		// 処理がタイムアウトした
		cStrErrorMsg.LoadString( IDS_TIMEOUT);
		break;
	case _SEND_CANCEL:		// ユーザによりキャンセルされ、処理を中断した
		cStrErrorMsg.LoadString( IDS_USERCANCELED);
		break;
	default:
		cStrErrorMsg.LoadString( IDS_UNDEFINECODE);
	}
	return FALSE;
}

int CNetExLib::ExchangeSendData( SENDDATA* pstSendData, int nSendDataExSize, const INNERSENDDATA* pstInnerSendData)
{
	CString	cStrWork1;
	CString	cStrWork2;
	int		nSendDataExSizeCal;

	nSendDataExSizeCal = CalcSendData( pstInnerSendData);

	if( NULL == pstSendData || 0 == nSendDataExSize)return nSendDataExSizeCal;
	if( nSendDataExSizeCal > nSendDataExSize)return -1;

	BYTE*		pabyData;
	SENDDATA* pstWork;

	pabyData = new BYTE [ nSendDataExSizeCal];
	ZeroMemory( pabyData, nSendDataExSizeCal);

	pstWork = ( SENDDATA*)pabyData;

	int		nOffset = sizeof( SENDDATA);
	
	// フォーマットコード
	lstrcpy( pstWork->szHeader, "OboeData");
	// 構造体情報
	pstWork->nSize				= nSendDataExSizeCal;
	pstWork->unStructVersion	= _SENDDATA_VER100;

	// バージョン差異通知
	pstWork->m_unVersion		= pstInnerSendData->m_unVersion;		// バージョン番号
	if( pstInnerSendData->m_cStrVersion.IsEmpty())
	{
		pstWork->m_nszVersion		= -1;		// バージョン名称:offset from abyExtDatas[ 0]
	}
	else
	{
		pstWork->m_nszVersion		= nOffset;		// バージョン名称:offset from abyExtDatas[ 0]
		lstrcpy( ( LPSTR)&pabyData[ nOffset], pstInnerSendData->m_cStrVersion);
		nOffset += pstInnerSendData->m_cStrVersion.GetLength() + 1;
	}

	// 基本設定のデータ
	// タイトル:offset from abyExtDatas[ 0]
	pstInnerSendData->m_cMemoData.GetTitle( cStrWork1);
	if( cStrWork1.IsEmpty())
	{
		pstWork->m_nszMemoTitle		= -1;
	}
	else
	{
		pstWork->m_nszMemoTitle		= nOffset;
		lstrcpy( ( LPSTR)&pabyData[ nOffset], cStrWork1);
		nOffset += cStrWork1.GetLength() + 1;
	}

	// 内容:offset from abyExtDatas[ 0]
	cStrWork1 = pstInnerSendData->m_cMemoData.GetMemo();
	if( cStrWork1.IsEmpty())
	{
		pstWork->m_nszMemoData		= -1;
	}
	else
	{
		pstWork->m_nszMemoData		= nOffset;
		lstrcpy( ( LPSTR)&pabyData[ nOffset], cStrWork1);
		nOffset += cStrWork1.GetLength() + 1;
	}

	// 装飾情報
	// フォント構造体(LOGFONT):offset from abyExtDatas[ 0]
	pstWork->m_nstLogFont = nOffset;
	LOGFONT	stLogFont;
	pstInnerSendData->m_cMemoData.GetFont( &stLogFont);
	CopyMemory( &pabyData[ nOffset], &stLogFont, sizeof( LOGFONT));
	nOffset += sizeof( LOGFONT);
	// 文字色
	pstInnerSendData->m_cMemoData.GetForeColor( pstWork->m_clrForeColor);
	// 背景色
	pstInnerSendData->m_cMemoData.GetBackColor( pstWork->m_clrBackColor);
	// アイコン種別
	pstWork->m_nIconIndex		= pstInnerSendData->m_cMemoData.GetIconIndex();;

	// 関連付け
	// 関連付けの有無
	pstWork->m_blShellExec		= pstInnerSendData->m_cMemoData.IsLink();
	// 関連付けプログラム:offset from abyExtDatas[ 0]
	pstInnerSendData->m_cMemoData.GetLinkData( cStrWork1, cStrWork2);
	if( cStrWork1.IsEmpty())
	{
		pstWork->m_nszProgName	= -1;
	}
	else
	{
		pstWork->m_nszProgName	= nOffset;
		lstrcpy( ( LPSTR)&pabyData[ nOffset], cStrWork1);
		nOffset += cStrWork1.GetLength() + 1;
	}
	// 関連付けファイル:offset from abyExtDatas[ 0]
	if( cStrWork2.IsEmpty())
	{
		pstWork->m_nszFileName	= -1;
	}
	else
	{
		pstWork->m_nszFileName	= nOffset;
		lstrcpy( ( LPSTR)&pabyData[ nOffset], cStrWork2);
		nOffset += cStrWork2.GetLength() + 1;
	}

	// タイマ関連
	// タイマの有無
	pstWork->m_blTimerEnable	= pstInnerSendData->m_cMemoData.IsTimer();
	// 時間
	pstWork->m_nHour			= pstInnerSendData->m_cMemoData.GetTimerHour();
	// 分
	pstWork->m_nMin				= pstInnerSendData->m_cMemoData.GetTimerMin();
	// タイマの種別
	pstWork->m_nTimerType		= pstInnerSendData->m_cMemoData.GetTimerType();
	// 通知月
	pstWork->m_nMonth			= pstInnerSendData->m_cMemoData.GetTimerMonth();
	// 通知日
	pstWork->m_nDay				= pstInnerSendData->m_cMemoData.GetTimerDay();
	// 通知日が日曜日だった場合のシフト:1.40
	pstWork->m_nNotifyShift		= pstInnerSendData->m_cMemoData.GetNotifyShift();
	// ｎ日前からの通知:1.40
	pstWork->m_nBeforeNotify	= pstInnerSendData->m_cMemoData.GetBeforeNotify();
	// 経過後のタイマー:1.40
	pstWork->m_blExpireTimer	= pstInnerSendData->m_cMemoData.IsAfterNotify();
	// サウンドファイル:1.40:offset from abyExtDatas[ 0]
	pstInnerSendData->m_cMemoData.GetNotifySound( cStrWork1);
	if( cStrWork1.IsEmpty())
	{
		pstWork->m_nszWaveFile	= -1;
	}
	else
	{
		pstWork->m_nszWaveFile	= nOffset;
		lstrcpy( ( LPSTR)&pabyData[ nOffset], cStrWork1);
		nOffset += cStrWork1.GetLength() + 1;
	}
	// 通知曜日
	pstWork->m_nWeekDay			= pstInnerSendData->m_cMemoData.GetWeek();

	// 送信のデータ
	// 署名の有無
	pstWork->m_blSendMaster		= pstInnerSendData->m_blAppendSig;
	// 署名追加位置
	pstWork->m_nAppendPos		= pstInnerSendData->m_nAppendPos;
	// 署名:offset from abyExtDatas[ 0]
	if( pstInnerSendData->m_cStrSignature.IsEmpty())
	{
		pstWork->m_nszSignature	= -1;
	}
	else
	{
		pstWork->m_nszSignature	= nOffset;
		lstrcpy( ( LPSTR)&pabyData[ nOffset], pstInnerSendData->m_cStrSignature);
		nOffset += pstInnerSendData->m_cStrSignature.GetLength() + 1;
	}
	// 送信時間
	pstWork->m_blSendTime		= pstInnerSendData->m_blAppendSendTime;
	// 送信後のリサイズ
	pstWork->m_blSendResize		= pstInnerSendData->m_blSendResize;
	// 表示サイズ
	CopyMemory( &pstWork->m_stRctNormal, pstInnerSendData->m_cMemoData.GetNormalRect(), sizeof( RECT));
	// センタリング受信:1.40
	pstWork->m_blSendCenter		= pstInnerSendData->m_blSendCenter;
	// 裏返し送信:1.40
	pstWork->m_blPack			= pstInnerSendData->m_blPack;
	// パスワード要求:1.40
	pstWork->m_blPassWord		= pstInnerSendData->m_blPassWord;
	// 開封確認:1.40
	pstWork->m_blCheckOpen		= pstInnerSendData->m_blCheckOpen;
	// 終了時の削除
	pstWork->m_blDelOnQuit		= pstInnerSendData->m_blDelOnQuit;
	// 送信元:offset from abyExtDatas[ 0]
	if( pstInnerSendData->m_cStrFrom.IsEmpty())
	{
		pstWork->m_nszFrom	= -1;
	}
	else
	{
		pstWork->m_nszFrom	= nOffset;
		lstrcpy( ( LPSTR)&pabyData[ nOffset], pstInnerSendData->m_cStrFrom);
		nOffset += pstInnerSendData->m_cStrFrom.GetLength() + 1;
	}
	// 送信先一覧:offset from abyExtDatas[ 0]
	// 送信先一覧は、アドレスのみで、","カンマで区切る事
	if( pstInnerSendData->m_cStrDistribute.IsEmpty())
	{
		pstWork->m_nszTo	= -1;
	}
	else
	{
		pstWork->m_nszTo	= nOffset;
		lstrcpy( ( LPSTR)&pabyData[ nOffset], pstInnerSendData->m_cStrDistribute);
		nOffset += pstInnerSendData->m_cStrDistribute.GetLength() + 1;
	}

	// ネットワーク決議
	// はい／いいえ確認:1.40
	pstWork->m_blCheckYesNo		= pstInnerSendData->m_blChackResult;
	if( pstInnerSendData->m_blChackResult)
	{
		// 選択肢１:offset from abyExtDatas[ 0]
		if( pstInnerSendData->m_stResults.m_cStrResult1.IsEmpty())
		{
			pstWork->m_nszResult1	= -1;
		}
		else
		{
			pstWork->m_nszResult1	= nOffset;
			lstrcpy( ( LPSTR)&pabyData[ nOffset], pstInnerSendData->m_stResults.m_cStrResult1);
			nOffset += pstInnerSendData->m_stResults.m_cStrResult1.GetLength() + 1;
		}
		// 選択肢２:offset from abyExtDatas[ 0]
		if( pstInnerSendData->m_stResults.m_cStrResult2.IsEmpty())
		{
			pstWork->m_nszResult2	= -1;
		}
		else
		{
			pstWork->m_nszResult2	= nOffset;
			lstrcpy( ( LPSTR)&pabyData[ nOffset], pstInnerSendData->m_stResults.m_cStrResult2);
			nOffset += pstInnerSendData->m_stResults.m_cStrResult2.GetLength() + 1;
		}

		// その他の解:1.42
		pstWork->m_blOtherResult	= pstInnerSendData->m_stResults.m_blOtherResult;
		// その他の解デフォルト:offset from abyExtDatas[ 0]
		pstWork->m_nszResult3	= -1;
		if( pstInnerSendData->m_stResults.m_blOtherResult)
		{
			if( pstInnerSendData->m_stResults.m_cStrResult3.IsEmpty())
			{
				pstWork->m_nszResult3	= -1;
			}
			else
			{
				pstWork->m_nszResult3	= nOffset;
				lstrcpy( ( LPSTR)&pabyData[ nOffset], pstInnerSendData->m_stResults.m_cStrResult3);
				nOffset += pstInnerSendData->m_stResults.m_cStrResult3.GetLength() + 1;
			}
		}
	}

	// 応答コード（文字列）:offset from abyExtDatas[ 0]
	if( pstInnerSendData->m_cStrResultID.IsEmpty())
	{
		pstWork->m_nszCHIPID	= -1;
	}
	else
	{
		pstWork->m_nszCHIPID	= nOffset;
		lstrcpy( ( LPSTR)&pabyData[ nOffset], pstInnerSendData->m_cStrResultID);
		nOffset += pstInnerSendData->m_cStrResultID.GetLength() + 1;
	}

	// 小片の作成時間
	// 作成の年:1.40
	pstWork->m_nCreateYear		= pstInnerSendData->m_nCreateYear;
	// 作成の月:1.40
	pstWork->m_nCreateMonth		= pstInnerSendData->m_nCreateMonth;
	// 作成の日:1.40
	pstWork->m_nCreateDay		= pstInnerSendData->m_nCreateDay;
	// 削除期日:1.40；使用しない
	pstWork->m_nZapDays			= -1;

	// 描画拡張
	// DxlのID
	pstWork->m_unDxlID			= pstInnerSendData->m_cMemoData.GetDxlID();
	if( pstWork->m_unDxlID)
	{
		// Dxl未使用時の表示文字:offset from abyExtDatas[ 0]
		if( pstInnerSendData->m_cStrDxlDummy.IsEmpty())
		{
			pstWork->m_nszDxlDummy	= -1;
		}
		else
		{
			pstWork->m_nszDxlDummy	= nOffset;
			lstrcpy( ( LPSTR)&pabyData[ nOffset], pstInnerSendData->m_cStrDxlDummy);
			nOffset += pstInnerSendData->m_cStrDxlDummy.GetLength() + 1;
		}
		// dxlの拡張情報:offset from abyExtDatas[ 0]
		if( 0 >= pstInnerSendData->m_cMemoData.GetDxlExtData( NULL, 0))
		{
			pstWork->m_nDxlDataSize	= 0;
			pstWork->m_nabyDxlData	= -1;
		}
		else
		{
			pstWork->m_nDxlDataSize	= pstInnerSendData->m_cMemoData.GetDxlExtData( NULL, 0);
			pstWork->m_nabyDxlData	= nOffset;
			pstInnerSendData->m_cMemoData.GetDxlExtData( &pabyData[ nOffset], pstWork->m_nDxlDataSize);
			nOffset += pstWork->m_nDxlDataSize;
		}
	}

	CopyMemory( pstSendData, pstWork, nSendDataExSizeCal);
	pstWork = NULL;
	delete [] pabyData;
	pabyData = NULL;

	return nSendDataExSizeCal;
}

int CNetExLib::CalcSendData( const INNERSENDDATA* pstInnerSendData)
{
	CString	cStrWork1;
	CString	cStrWork2;
	int		nSendDataExSizeCal;

	nSendDataExSizeCal = sizeof( SENDDATA);

	// 送信元
	if( !pstInnerSendData->m_cStrFrom.IsEmpty())
	{
		nSendDataExSizeCal += pstInnerSendData->m_cStrFrom.GetLength() + 1;
	}
	// シグネチャ
	if( !pstInnerSendData->m_cStrSignature.IsEmpty())
	{
		nSendDataExSizeCal += pstInnerSendData->m_cStrSignature.GetLength() + 1;
	}

	// メモの内容
	cStrWork1 = pstInnerSendData->m_cMemoData.GetMemo();
	if( !cStrWork1.IsEmpty())
	{
		nSendDataExSizeCal += cStrWork1.GetLength() + 1;
	}
	// タイトル
	pstInnerSendData->m_cMemoData.GetTitle( cStrWork1);
	if( !cStrWork1.IsEmpty())
	{
		nSendDataExSizeCal += cStrWork1.GetLength() + 1;
	}
	// フォント
	nSendDataExSizeCal += sizeof( LOGFONT);

	// 関連付け
	pstInnerSendData->m_cMemoData.GetLinkData( cStrWork1, cStrWork2);
	if( !cStrWork1.IsEmpty())
	{
		nSendDataExSizeCal += cStrWork1.GetLength() + 1;
	}
	if( !cStrWork2.IsEmpty())
	{
		nSendDataExSizeCal += cStrWork2.GetLength() + 1;
	}

	// 通知サウンド
	pstInnerSendData->m_cMemoData.GetNotifySound( cStrWork1);
	if( !cStrWork1.IsEmpty())
	{
		nSendDataExSizeCal += cStrWork1.GetLength() + 1;
	}

	// ネットワーク決議用
	if( TRUE == pstInnerSendData->m_blChackResult)
	{
		if( !pstInnerSendData->m_stResults.m_cStrResult1.IsEmpty())
		{
			nSendDataExSizeCal += pstInnerSendData->m_stResults.m_cStrResult1.GetLength() + 1;
		}
		if( !pstInnerSendData->m_stResults.m_cStrResult2.IsEmpty())
		{
			nSendDataExSizeCal += pstInnerSendData->m_stResults.m_cStrResult2.GetLength() + 1;
		}
		if( !pstInnerSendData->m_stResults.m_cStrResult3.IsEmpty())
		{
			nSendDataExSizeCal += pstInnerSendData->m_stResults.m_cStrResult3.GetLength() + 1;
		}
	}
	if( !pstInnerSendData->m_cStrResultID.IsEmpty())
	{
		nSendDataExSizeCal += pstInnerSendData->m_cStrResultID.GetLength() + 1;
	}

	// バージョン差異
	if( -1 != pstInnerSendData->m_unVersion)
	{
		if( !pstInnerSendData->m_cStrVersion.IsEmpty())
		{
			nSendDataExSizeCal += pstInnerSendData->m_cStrVersion.GetLength() + 1;
		}
	}

	// 送信先一覧
	if( !pstInnerSendData->m_cStrDistribute.IsEmpty())
	{
		nSendDataExSizeCal += pstInnerSendData->m_cStrDistribute.GetLength() + 1;
	}
	// Dxl拡張情報
	if( pstInnerSendData->m_cMemoData.GetDxlID())
	{
		// Dxl未使用時の表示文字:offset from abyExtDatas[ 0]
		if( !pstInnerSendData->m_cStrDxlDummy.IsEmpty())
		{
			nSendDataExSizeCal += pstInnerSendData->m_cStrDxlDummy.GetLength() + 1;
		}
		if( 0 <= pstInnerSendData->m_cMemoData.GetDxlExtData( NULL, 0))
		{
			nSendDataExSizeCal += pstInnerSendData->m_cMemoData.GetDxlExtData( NULL, 0) + 1;
		}
	}

	return nSendDataExSizeCal;
}

int CNetExLib::ExchangeSendResultData( RESULTDATA* pstResultData, int nResultDataSize, const INNERRESULTDATA* pstInnerResultData)
{
	int		nSendResultDataSizeCal;
	nSendResultDataSizeCal = CalcSendResultData( pstInnerResultData);

	if( 0 >= nSendResultDataSizeCal)return -1;
	if( NULL == pstResultData || 0 == nResultDataSize)return nSendResultDataSizeCal;
	if( nSendResultDataSizeCal > nResultDataSize)return -1;


	// フォーマットコード
	lstrcpy( pstResultData->szHeader, "OboeRslt");
	// 構造体情報
	pstResultData->nSize = nSendResultDataSizeCal;
	pstResultData->unStructVersion = _SENDDATA_VER100;

	int nMasterIndex = sizeof( RESULTDATA);

	char*	pWorkPointer = ( char*)pstResultData;

	pstResultData->m_nszSignature = nMasterIndex;
	lstrcpy( ( LPSTR)&pWorkPointer[ nMasterIndex], pstInnerResultData->m_cStrSignature);
	nMasterIndex += pstInnerResultData->m_cStrSignature.GetLength() + 1;
	// 応答コード（文字列）:offset from abyExtDatas[ 0]
	pstResultData->m_nszCHIPID = nMasterIndex;
	lstrcpy( ( LPSTR)&pWorkPointer[ nMasterIndex], pstInnerResultData->m_cStrChipID);
	nMasterIndex += pstInnerResultData->m_cStrChipID.GetLength() + 1;

	pstResultData->m_nszResult = nMasterIndex;
	lstrcpy( ( LPSTR)&pWorkPointer[ nMasterIndex], pstInnerResultData->m_cStrResult);
	nMasterIndex += pstInnerResultData->m_cStrResult.GetLength() + 1;

	return nSendResultDataSizeCal;
}

int CNetExLib::CalcSendResultData( const INNERRESULTDATA* pstInnerResultData)
{
	int nSendResultDataSizeCal = sizeof( RESULTDATA);

	nSendResultDataSizeCal += pstInnerResultData->m_cStrSignature.GetLength() + 1;
	nSendResultDataSizeCal += pstInnerResultData->m_cStrChipID.GetLength() + 1;
	nSendResultDataSizeCal += pstInnerResultData->m_cStrResult.GetLength() + 1;

	return nSendResultDataSizeCal;
}

BOOL CNetExLib::ExcangeReceiveData( INNERSENDDATA* pstInnerSendData, const SENDDATA* pstSendData, int nSendDataExSize)
{
	if( NULL == pstInnerSendData)return FALSE;
	if( NULL == pstSendData)return FALSE;
	if( 0 == nSendDataExSize)return FALSE;
	if( lstrcmp( pstSendData->szHeader, "OboeData"))return FALSE;
	if( pstSendData->nSize > nSendDataExSize)return FALSE;
	if( pstSendData->unStructVersion != _SENDDATA_VER100)return FALSE;
	if( 0 > pstSendData->m_nszMemoData)return FALSE;

	char*	pWorkPointer = ( char*)pstSendData;
	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszFrom)
	{
		pstInnerSendData->m_cStrFrom = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszFrom];
	}
	else
	{
		pstInnerSendData->m_cStrFrom.Empty();
	}
	
	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszTo)
	{
		pstInnerSendData->m_cStrDistribute = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszTo];
	}
	else
	{
		pstInnerSendData->m_cStrDistribute.Empty();
	}

	pstInnerSendData->m_blAppendSig = pstSendData->m_blSendMaster;
	pstInnerSendData->m_nAppendPos = pstSendData->m_nAppendPos;

	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszSignature)
	{
		pstInnerSendData->m_cStrSignature = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszSignature];
	}
	else
	{
		pstInnerSendData->m_cStrSignature.Empty();
	}
	
	pstInnerSendData->m_blAppendSendTime = pstSendData->m_blSendTime;
	pstInnerSendData->m_blSendResize = pstSendData->m_blSendResize;
	pstInnerSendData->m_blSendCenter = pstSendData->m_blSendCenter;

	pstInnerSendData->m_blPack = pstSendData->m_blPack;
	if( pstInnerSendData->m_blPack)
	{
		pstInnerSendData->m_blPassWord = pstSendData->m_blPassWord;
		pstInnerSendData->m_blCheckOpen = pstSendData->m_blCheckOpen;
	}

	pstInnerSendData->m_blChackResult = pstSendData->m_blCheckYesNo;
	if( pstInnerSendData->m_blChackResult)
	{
		if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszResult1)
		{
			pstInnerSendData->m_stResults.m_cStrResult1 = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszResult1];
		}
		else
		{
			pstInnerSendData->m_stResults.m_cStrResult1 = _T( "Yes");
		}
		if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszResult2)
		{
			pstInnerSendData->m_stResults.m_cStrResult2 = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszResult2];
		}
		else
		{
			pstInnerSendData->m_stResults.m_cStrResult2 = _T( "No");
		}
		pstInnerSendData->m_stResults.m_blOtherResult = pstSendData->m_blOtherResult;
		if( pstInnerSendData->m_stResults.m_blOtherResult)
		{
			if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszResult3)
			{
				pstInnerSendData->m_stResults.m_cStrResult3 = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszResult3];
			}
			else
			{
				pstInnerSendData->m_stResults.m_cStrResult3.Empty();
			}
		}
	}
	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszCHIPID)
	{
		pstInnerSendData->m_cStrResultID = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszCHIPID];
	}

	pstInnerSendData->m_blDelOnQuit = pstSendData->m_blDelOnQuit;

	pstInnerSendData->m_unVersion = pstSendData->m_unVersion;
	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszVersion)
	{
		pstInnerSendData->m_cStrVersion = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszVersion];
	}
	else
	{
		pstInnerSendData->m_cStrVersion.Empty();
	}	

	pstInnerSendData->m_cMemoData.SetDefaultStyle();
	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszMemoTitle)
	{
		pstInnerSendData->m_cMemoData.SetTitle( ( LPCSTR)&pWorkPointer[ pstSendData->m_nszMemoTitle]);
	}
	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszMemoData)
	{
		pstInnerSendData->m_cMemoData.SetMemo( ( LPCSTR)&pWorkPointer[ pstSendData->m_nszMemoData]);
	}
	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nstLogFont)
	{
		pstInnerSendData->m_cMemoData.SetFont( ( LOGFONT*)&pWorkPointer[ pstSendData->m_nstLogFont]);
	}
	pstInnerSendData->m_cMemoData.SetForeColor( pstSendData->m_clrForeColor);
	pstInnerSendData->m_cMemoData.SetBackColor( pstSendData->m_clrBackColor);
	pstInnerSendData->m_cMemoData.SetIconIndex( pstSendData->m_nIconIndex);

	pstInnerSendData->m_cMemoData.Link( pstSendData->m_blShellExec);
	CString	cStrWork1;
	CString	cStrWork2;
	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszProgName)
	{
		cStrWork1 = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszProgName];
	}
	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszFileName)
	{
		cStrWork2 = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszFileName];
	}
	pstInnerSendData->m_cMemoData.SetLinkData( cStrWork1, cStrWork2);

	pstInnerSendData->m_cMemoData.Timer( pstSendData->m_blTimerEnable);
	pstInnerSendData->m_cMemoData.SetTimerHour( pstSendData->m_nHour);
	pstInnerSendData->m_cMemoData.SetTimerMin( pstSendData->m_nMin);

	pstInnerSendData->m_cMemoData.SetTimerType( pstSendData->m_nTimerType);

	pstInnerSendData->m_cMemoData.SetTimerMonth( pstSendData->m_nMonth);
	pstInnerSendData->m_cMemoData.SetTimerDay( pstSendData->m_nDay);

	pstInnerSendData->m_cMemoData.SetNotifyShift( pstSendData->m_nNotifyShift);
	pstInnerSendData->m_cMemoData.SetBeforeNotify( pstSendData->m_nBeforeNotify);
	pstInnerSendData->m_cMemoData.AfterNotify( pstSendData->m_blExpireTimer);

	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszWaveFile)
	{
		cStrWork1 = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszWaveFile];
		pstInnerSendData->m_cMemoData.SetNotifySound( cStrWork1);
	}

	pstInnerSendData->m_cMemoData.SetWeek( pstSendData->m_nWeekDay);

	pstInnerSendData->m_cMemoData.SetNormalRect( &pstSendData->m_stRctNormal);

	// Dxl
	pstInnerSendData->m_cMemoData.SetDxlID( pstSendData->m_unDxlID);
	if( pstSendData->m_unDxlID)
	{
		// 再生不可能な場合
		if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszDxlDummy)
		{
			pstInnerSendData->m_cStrDxlDummy = ( LPCSTR)&pWorkPointer[ pstSendData->m_nszDxlDummy];
		}
		else
		{
			pstInnerSendData->m_cStrDxlDummy = pstInnerSendData->m_cMemoData.GetMemo();
		}
		// dxlの拡張情報:offset from abyExtDatas[ 0]
		if( pstSendData->m_nDxlDataSize)
		{
//			if( ( int)sizeof( SENDDATA) < pstSendData->m_nDxlDataSize)
			if( ( int)sizeof( SENDDATA) < pstSendData->m_nabyDxlData)
			{
				pstInnerSendData->m_cMemoData.SetDxlExtData( ( PBYTE)&pWorkPointer[ pstSendData->m_nabyDxlData], pstSendData->m_nDxlDataSize);
			}
		}
	}

	return TRUE;
}

BOOL CNetExLib::ExcangeReceiveResultData( INNERRESULTDATA* pstInnerResultData, const RESULTDATA* pstResultData, int pstResultDataExSize)
{
	if( NULL == pstInnerResultData)return FALSE;
	if( NULL == pstResultData)return FALSE;
	if( 0 == pstResultDataExSize)return FALSE;
	if( lstrcmp( pstResultData->szHeader, "OboeRslt"))return FALSE;
	if( pstResultData->nSize > pstResultDataExSize)return FALSE;
	if( pstResultData->unStructVersion != _SENDDATA_VER100)return FALSE;
	if( 0 > pstResultData->m_nszResult)return FALSE;
	if( 0 > pstResultData->m_nszCHIPID)return FALSE;
	if( 0 > pstResultData->m_nszSignature)return FALSE;

	char*	pWorkPointer = ( char*)pstResultData;
	if( 0 <= pstResultData->m_nszSignature)
	{
		pstInnerResultData->m_cStrSignature = ( LPCSTR)&pWorkPointer[ pstResultData->m_nszSignature];
	}
	if( 0 <= pstResultData->m_nszCHIPID)
	{
		pstInnerResultData->m_cStrChipID = ( LPCSTR)&pWorkPointer[ pstResultData->m_nszCHIPID];
	}
	if( 0 <= pstResultData->m_nszResult)
	{
		pstInnerResultData->m_cStrResult = ( LPCSTR)&pWorkPointer[ pstResultData->m_nszResult];
	}

	return TRUE;
}