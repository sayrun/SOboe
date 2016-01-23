#include "stdafx.h"
#include "SetReg.h"
#include "IconImgList.h"
#include "Base64.h"
#include "IconSelectMenu.h"

CIconImgList::CIconImgList( void)
{
	m_hBmpDefault = NULL;
	m_hBmpExtend = NULL;
	m_nCountExtend = 0;
	m_astIconStatus = NULL;
	m_nEnableCount = 0;

	m_nGroupCount = 0;
	m_astGroupData = NULL;
	m_astIconStatus4Menu = NULL;
}

CIconImgList::~CIconImgList( void)
{
	ReleaseExtIconData();
}

void CIconImgList::ReleaseExtIconData( void)
{
	if( NULL != m_hBmpDefault)
	{
		::DeleteObject( m_hBmpDefault);
		m_hBmpDefault = NULL;
	}
	if( NULL != m_hBmpExtend)
	{
		::DeleteObject( m_hBmpExtend);
		m_hBmpExtend = NULL;
	}
	if( NULL != m_astIconStatus)
	{
		delete [] m_astIconStatus;
		m_astIconStatus = NULL;
	}
	m_nCountExtend = 0;
	m_nEnableCount = 0;

	if( NULL != m_astGroupData)
	{
		for( int nIndex = 0; nIndex < m_nGroupCount; nIndex++)
		{
			if( NULL != m_astGroupData[ nIndex])
			{
				free( m_astGroupData[ nIndex]);
				m_astGroupData[ nIndex] = NULL;
			}
		}
		delete [] m_astGroupData;
		m_astGroupData = NULL;
	}
	m_nGroupCount = 0;

	if( NULL != m_astIconStatus4Menu)
	{
		delete [] m_astIconStatus4Menu;
		m_astIconStatus4Menu = NULL;
	}
}

BOOL CIconImgList::LoadExtIconData( void)
{
	// 通常アイコンの読み込み
	m_hBmpDefault = ::LoadBitmap( AfxGetResourceHandle(), MAKEINTRESOURCE( IDB_ICONIMG));

	TCHAR szFilePath[ MAX_PATH];
	int nLength = GetModuleFileName( NULL, szFilePath, MAX_PATH);
	for( int nIndex = nLength; nIndex >= 0; nIndex--)
	{
		if( TEXT( '\\') == szFilePath[ nIndex])
		{
			lstrcpy( &szFilePath[ nIndex + 1], TEXT( "GetIcon.xml"));
			break;
		}
	}


	BOOL blResult = FALSE;
	MSXML2::IXMLDOMDocumentPtr piDocument;
	if( SUCCEEDED( piDocument.CreateInstance( _T( "Microsoft.XMLDOM"))))
	{
		piDocument->PutvalidateOnParse( VARIANT_TRUE);
		if( VARIANT_FALSE != piDocument->load( _variant_t( _bstr_t( szFilePath))))
		{
			if( LoadIcons( piDocument))
			{
				blResult = LoadGroups( piDocument);
			}
		}
		else
		{
			MSXML2::IXMLDOMParseErrorPtr piError;

			piError = piDocument->GetparseError();
			long lCode = piError->GeterrorCode();
			TRACE( _T( "0x%08X\n"), lCode);	// 0x800C0005

			if( NULL != piError && 0 != lCode && 0x800C0005/*INET_E_RESOURCE_NOT_FOUND*/ != lCode)
			{
				long lRaw = piError->Getline();
				long lCol = piError->Getlinepos();

				_bstr_t cBstr = piError->Getreason();
				_bstr_t cBstrSrc = piError->GetsrcText();

				CString cStr;

				cStr.Format( _T( "拡張アイコンデータファイルを解析できませんでした。\n%s\n\nソース：%s\n位置：%d行 %d文字"), ( ( char*)cBstr), ( ( char*)cBstrSrc), lRaw, lCol);
				MessageBox( NULL, cStr, NULL, MB_OK | MB_ICONEXCLAMATION);
			}
		}
	}
	return blResult;
}

BOOL CIconImgList::LoadIcons( MSXML2::IXMLDOMDocumentPtr& piDocument)
{
	BOOL blResult = FALSE;

	MSXML2::IXMLDOMElementPtr piRoot;
	piRoot = piDocument->GetdocumentElement();
	if( NULL == piRoot)
	{
		return FALSE;
	}


	MSXML2::IXMLDOMElementPtr piGroups;
	piGroups = piRoot->selectSingleNode( _bstr_t( _T( "GROUPS")));


	MSXML2::IXMLDOMElementPtr piIcons;
	piIcons = piRoot->selectSingleNode( _bstr_t( _T( "ICONS")));
	if( NULL == piIcons)
	{
		return FALSE;
	}

	MSXML2::IXMLDOMNodeListPtr piIconsList;
	piIconsList = piIcons->GetchildNodes();
	if( NULL == piIconsList)
	{
		return FALSE;
	}

	long lIconsCount;
	lIconsCount = piIconsList->Getlength();
	if( 0 >= lIconsCount)
	{
		return FALSE;
	}

	m_nCountExtend = lIconsCount;
	if( 0 < m_nCountExtend)
	{
		m_astIconStatus = new ICONSTATUS[ m_nCountExtend];
		if( NULL != m_astIconStatus)
		{
			/*
			ZeroMemory( m_astIconStatus, sizeof( ICONSTATUS) * m_nCountExtend);
			*/
			for( int nIndex = 0; nIndex < m_nCountExtend; nIndex++)
			{
				m_astIconStatus[ m_nEnableCount].nID = 0;
				m_astIconStatus[ m_nEnableCount].blEnable = FALSE;
				m_astIconStatus[ m_nEnableCount].nParent = 0;
				m_astIconStatus[ m_nEnableCount].nPosition = 0;
				m_astIconStatus[ m_nEnableCount].cStrIconName.Empty();
			}

			int nHeight = ( m_nCountExtend / 10) + ( ( m_nCountExtend % 10) ? 1 : 0);

			m_hBmpExtend = ::CreateBitmap( _ICON_WIDTH * 10, _ICON_HEIGHT * nHeight, 1, 1, NULL);
			HDC hExtDC = ::CreateCompatibleDC( NULL);
			HDC hDefDC = ::CreateCompatibleDC( NULL);
			HDC hWorkDC = ::CreateCompatibleDC( NULL);
			HBITMAP hOldExtBmp = ( HBITMAP)::SelectObject( hExtDC, m_hBmpExtend);
			HBITMAP hOldDefBmp = ( HBITMAP)::SelectObject( hDefDC, m_hBmpDefault);

			m_nEnableCount = 0;
			CString cStrEntry;
			LPBYTE lpabyData;
			TCHAR szWork[ 64];
			MSXML2::IXMLDOMElementPtr piIcon;
			MSXML2::IXMLDOMElementPtr piImage;
			MSXML2::IXMLDOMElementPtr piParent;
			_variant_t cVariant;
			_bstr_t cBstr;
			CString strTitle;
			for( int nIndex = 0; nIndex < m_nCountExtend; nIndex++)
			{
				cVariant = VARIANT_TRUE;
				if( NULL != piGroups)
				{
					wsprintf( szWork, _T( "//ITEM[@REF='%d']"), nIndex + 1);
					piIcon = piGroups->selectSingleNode( _bstr_t( szWork));
					if( NULL != piIcon)
					{
						cVariant = piIcon->getAttribute( _bstr_t( _T( "ENABLE")));
						if( cVariant.vt != VT_NULL)
						{
							VariantChangeType( &cVariant, &cVariant, 0, VT_BOOL);
						}
						else
						{
							cVariant = VARIANT_TRUE;
						}
					}
				}

				if( VARIANT_FALSE != cVariant.boolVal)
				{
					wsprintf( szWork, _T( "//ICON[@ID='%d']"), nIndex + 1);
					piIcon = piRoot->selectSingleNode( _bstr_t( szWork));
					if( NULL == piIcon)
					{
						continue;
					}

					cVariant = piIcon->getAttribute( _bstr_t( _T( "TITLE")));
					if( cVariant.vt != VT_NULL)
					{
						strTitle = cVariant;
					}
					else
					{
						strTitle = _T( "NoTitle");
					}

					piImage = piIcon->selectSingleNode( _bstr_t( _T( "IMAGE")));
					if( NULL == piImage)
					{
						continue;
					}
					cBstr = piImage->Gettext();
					int nSize = Base64Decode( NULL, 0, cBstr);
					lpabyData = new BYTE [ nSize];
					Base64Decode( lpabyData, nSize, cBstr);

					HBITMAP hWork = ::CreateBitmap( _ICON_WIDTH, _ICON_HEIGHT, 1, 1, lpabyData);
					
					delete [] lpabyData;

					HBITMAP hOldWorkBmp = ( HBITMAP)::SelectObject( hWorkDC, hWork);
					::BitBlt( hExtDC, ( nIndex % 10) * _ICON_WIDTH, ( nIndex / 10) * _ICON_HEIGHT, _ICON_WIDTH, _ICON_HEIGHT, hWorkDC, 0, 0, SRCCOPY);
					::SelectObject( hWorkDC, hOldWorkBmp);
					::DeleteObject( hWork);
					m_astIconStatus[ m_nEnableCount].nID = nIndex;
					m_astIconStatus[ m_nEnableCount].blEnable = TRUE;
					m_astIconStatus[ m_nEnableCount].cStrIconName = strTitle;

					m_nEnableCount++;
					continue;
				}
				::BitBlt( hExtDC, ( nIndex % 10) * _ICON_WIDTH, ( nIndex / 10) * _ICON_HEIGHT, _ICON_WIDTH, _ICON_HEIGHT, hDefDC, ( COUNT_DEFAULTICON) * _ICON_WIDTH, 0, SRCCOPY);
			}

			::SelectObject( hExtDC, hOldExtBmp);
			::SelectObject( hDefDC, hOldDefBmp);

			::DeleteDC( hExtDC);
			::DeleteDC( hDefDC);
			::DeleteDC( hWorkDC);

			m_astIconStatus4Menu = new ICONSTATUS*[ m_nEnableCount];
			if( m_astIconStatus4Menu)
			{
				for( int nIndex = 0; nIndex < m_nEnableCount; nIndex++)
				{
					m_astIconStatus4Menu[ nIndex] = &m_astIconStatus[ nIndex];
				}
				qsort( m_astIconStatus4Menu, m_nEnableCount, sizeof( ICONSTATUS*), IconCompare);

				blResult = TRUE;
			}
		}
	}

	return blResult;
}

int __cdecl CIconImgList::IconCompare(const void *elem1, const void *elem2)
{
	ICONSTATUS** lpElem1 = ( ICONSTATUS**)elem1;
	ICONSTATUS** lpElem2 = ( ICONSTATUS**)elem2;

	int nResult = ( *lpElem1)->nPosition - ( *lpElem2)->nPosition;
	if( 0 == nResult)
	{
		nResult = ( *lpElem1)->nID - ( *lpElem2)->nID;
	}
	return nResult;
}

BOOL CIconImgList::LoadGroups( MSXML2::IXMLDOMDocumentPtr& piDocument)
{
	// グループの追加処理
	MSXML2::IXMLDOMElementPtr piRoot;
	piRoot = piDocument->GetdocumentElement();
	if( NULL == piRoot)
	{
		return FALSE;
	}

	MSXML2::IXMLDOMElementPtr piGroups;
	piGroups = piRoot->selectSingleNode( _bstr_t( _T( "GROUPS")));
	if( NULL == piGroups)
	{
		return FALSE;
	}

	m_nGroupCount = GetGroupCount( piGroups);
	if( 0 < m_nGroupCount)
	{
		int nParent = 0;
		m_astGroupData = new GROUPDATA*[ m_nGroupCount];
		ZeroMemory( m_astGroupData, sizeof( GROUPDATA*) * m_nGroupCount);

		PickUpGroups( piGroups, nParent);
		/*
		_bstr_t cBstrName;
		BOOL blEnable;

		MSXML2::IXMLDOMElementPtr piGroup;
		_bstr_t cBstr;
		_variant_t cVariant;
		TCHAR szWork[ 32];

		for( int nIndex = 0; nIndex < m_nGroupCount; nIndex++)
		{
			wsprintf( szWork, _T( "//GROUP[@ID='%d']"), nIndex + 1);
			piGroup = piRoot->selectSingleNode( _bstr_t( szWork));
			if( NULL == piGroup)
			{
				continue;
			}

			cBstrName = piGroup->getAttribute( _bstr_t( _T( "TITLE")));

			cVariant = piGroup->getAttribute( _bstr_t( _T( "ENABLE")));
			VariantChangeType( &cVariant, &cVariant, 0, VT_BOOL);
			blEnable = ( VARIANT_FALSE == cVariant.boolVal) ? FALSE : TRUE;

			if( 0 >= cBstrName.length())cBstrName = _T( "new group!");

			m_astGroupData[ nIndex] = ( GROUPDATA*)malloc( sizeof( GROUPDATA) + ( sizeof( TCHAR) * ( cBstrName.length() + 1)));
			if( NULL != m_astGroupData[ nIndex])
			{
				m_astGroupData[ nIndex]->nParent = nParent;
				if( 1 <= nParent)
				{
					if( FALSE == m_astGroupData[ nParent - 1]->blEnable)
					{
						blEnable = FALSE;
					}
				}
				m_astGroupData[ nIndex]->blEnable = blEnable;
				lstrcpy( m_astGroupData[ nIndex]->szName, cBstrName);
			}
		}*/
	}
	return TRUE;
}

long CIconImgList::GetGroupCount( MSXML2::IXMLDOMElementPtr piGroups)
{
	long lCount = 0;

	MSXML2::IXMLDOMNodeListPtr piGroupsList;
	piGroupsList = piGroups->selectNodes( _bstr_t( _T( "GROUP")));
	if( NULL != piGroupsList)
	{
		long lChildCount = piGroupsList->Getlength();

		lCount = lChildCount;

		MSXML2::IXMLDOMElementPtr piGroupChild;
		for( long lIndex = 0; lIndex < lChildCount; lIndex++)
		{
			piGroupChild = piGroupsList->Getitem( lIndex);
			lCount += GetGroupCount( piGroupChild);
		}
	}
	return lCount;
}

BOOL CIconImgList::PickUpGroups( MSXML2::IXMLDOMElementPtr& piGroup, int& nIndex)
{
	MSXML2::IXMLDOMNodeListPtr piGroupList;
	piGroupList = piGroup->GetchildNodes();

	MSXML2::IXMLDOMElementPtr piNode;
	MSXML2::IXMLDOMElementPtr piChild;

	int nParent = nIndex;

	_bstr_t cBstr;
	_variant_t cVariant;
	long lCount = piGroupList->Getlength();
	for( long lIndex = 0; lIndex < lCount; lIndex++)
	{
		piNode = piGroupList->Getitem( lIndex);
		if( _bstr_t( _T( "GROUP")) == piNode->GetbaseName())
		{
			cBstr = piNode->getAttribute( _bstr_t( _T( "TITLE")));
			cVariant = piNode->getAttribute( _bstr_t( _T( "ENABLE")));
			cVariant.ChangeType( VT_BOOL);

			m_astGroupData[ nIndex] = ( GROUPDATA*)malloc( sizeof( GROUPDATA) + ( sizeof( TCHAR) * ( cBstr.length() + 1)));

			m_astGroupData[ nIndex]->nParent = nParent;
			if( 1 <= nIndex)
			{
				if( FALSE == m_astGroupData[ nParent]->blEnable)
				{
					cVariant.boolVal = VARIANT_FALSE;
				}
			}
			m_astGroupData[ nIndex]->blEnable = ( VARIANT_FALSE == cVariant.boolVal) ? FALSE : TRUE;
			lstrcpy( m_astGroupData[ nIndex]->szName, cBstr);

			nIndex++;

			PickUpGroups( piNode, nIndex);
		}
		else if( _bstr_t( _T( "ITEM")) == piNode->GetbaseName())
		{
			cVariant = piNode->getAttribute( _bstr_t( _T( "REF")));
			cVariant.ChangeType( VT_I4);
			for( int nIndex = 0; nIndex < m_nEnableCount; nIndex++)
			{
				if( cVariant.intVal == ( m_astIconStatus[ nIndex].nID + 1))
				{
					m_astIconStatus[ nIndex].nParent = nParent;
				}
			}
		}
	}

	return TRUE;
}

BOOL CIconImgList::Draw( int nID, HDC hDC, LPCRECT lpcRect) const
{
	HDC hMemDC = CreateCompatibleDC( hDC);

	if( 0 <= nID && COUNT_DEFAULTICON >= nID)
	{
		// 通常アイコンの描画
		HBITMAP hOldDefBmp = ( HBITMAP)::SelectObject( hMemDC, m_hBmpDefault);

		::BitBlt( hDC, lpcRect->left, lpcRect->top, lpcRect->right - lpcRect->left, lpcRect->bottom - lpcRect->top, hMemDC, nID * _ICON_WIDTH, 0, SRCCOPY);

		::SelectObject( hMemDC, hOldDefBmp);
	}
	else
	{
		// 拡張アイコンの描画
		if( ( COUNT_DEFAULTICON + m_nCountExtend) >= nID)
		{
			HBITMAP hOldDefBmp = ( HBITMAP)::SelectObject( hMemDC, m_hBmpExtend);

			int nIconIndex = min( max( nID - ( COUNT_DEFAULTICON + 1), 0), m_nCountExtend - 1);
			::BitBlt( hDC, lpcRect->left, lpcRect->top, lpcRect->right - lpcRect->left, lpcRect->bottom - lpcRect->top, hMemDC, ( nIconIndex % 10) * _ICON_WIDTH, ( nIconIndex / 10) * _ICON_HEIGHT, SRCCOPY);

			::SelectObject( hMemDC, hOldDefBmp);
		}
		else
		{
			// 何にも適合しない場合の？アイコンの描画
			HBITMAP hOldDefBmp = ( HBITMAP)::SelectObject( hMemDC, m_hBmpDefault);

			::BitBlt( hDC, lpcRect->left, lpcRect->top, lpcRect->right - lpcRect->left, lpcRect->bottom - lpcRect->top, hMemDC, COUNT_DEFAULTICON * _ICON_WIDTH, 0, SRCCOPY);

			::SelectObject( hMemDC, hOldDefBmp);
		}
	}
	::DeleteDC( hMemDC);

	return TRUE;
}

int CIconImgList::GetExtendStartIndex( void) const
{
	return COUNT_DEFAULTICON + 1;
}

int CIconImgList::GetDefaultCount( void) const
{
	return COUNT_DEFAULTICON;
}

int CIconImgList::GetExtendCount( void) const
{
	return m_nCountExtend;
}

int CIconImgList::GetEnableCount( void) const
{
	return m_nEnableCount;
}

BOOL CIconImgList::IsEnable( int nIndex) const
{
	if( 0 <= nIndex)
	{
		if( COUNT_DEFAULTICON >= nIndex)
		{
			return ( NULL != m_hBmpDefault);
		}
		else
		{
			if( ( COUNT_DEFAULTICON + m_nCountExtend) >= nIndex)
			{
				int nIconIndex = min( max( nIndex - ( COUNT_DEFAULTICON + 1), 0), m_nCountExtend - 1);
				return m_astIconStatus[ nIconIndex].blEnable;
			}
		}
	}
	return FALSE;
}

int CIconImgList::GetParentGroup( int nIndex) const
{
	if( 0 <= nIndex)
	{
		if( COUNT_DEFAULTICON >= nIndex)
		{
			return 0;
		}
		else
		{
			if( ( COUNT_DEFAULTICON + m_nCountExtend) >= nIndex)
			{
				int nIconIndex = min( max( nIndex - ( COUNT_DEFAULTICON + 1), 0), m_nCountExtend - 1);
				return m_astIconStatus[ nIconIndex].nParent;
			}
		}
	}
	return 0;
}


int CIconImgList::TrackPopupMenu( int x, int y, CWnd* pcWnd, BOOL blForce) const
{
	HMENU hMenu = CreatePopupMenu( pcWnd, blForce);

	UINT unCmdID = 0;
	if( NULL != hMenu)
	{
		unCmdID = ::TrackPopupMenu( hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, x, y, 0, pcWnd->GetSafeHwnd(), NULL);

		if( 0 == DestroyMenu( hMenu))
		{
			LPVOID lpMsgBuf = NULL;
			FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL,
							GetLastError(),
							MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT),
							(LPTSTR)&lpMsgBuf,
							0,
							NULL);

			OutputDebugString( ( LPCTSTR)lpMsgBuf);
			OutputDebugString( TEXT( "\r\n"));

			LocalFree( lpMsgBuf );
		}
	}

	return unCmdID;

	/*CreateWindow( */return 0;
}

HMENU CIconImgList::CreatePopupMenu( CWnd* pcWnd, BOOL blForce) const
{
	HMENU hMenuPopup = NULL;

	HMENU hMenuExitIcon = ::CreatePopupMenu();
	if( hMenuExitIcon)
	{
		CDC* pcDC = pcWnd->GetDC();
		if( pcDC)
		{
			CDC cMemDC;

			cMemDC.CreateCompatibleDC( pcDC);
			pcWnd->ReleaseDC( pcDC);

			HMENU* pahMenuGroup = NULL;
			pahMenuGroup = new HMENU [ m_nGroupCount];
			if( pahMenuGroup)
			{
				ZeroMemory( pahMenuGroup, sizeof( HMENU) * m_nGroupCount);
				for( int nIndex = 0; nIndex < m_nGroupCount; nIndex++)
				{
					if( blForce || m_astGroupData[ nIndex]->blEnable)
					{
						pahMenuGroup[ nIndex] = ::CreatePopupMenu();
						if( NULL != pahMenuGroup[ nIndex])
						{
							HMENU hWork = ( 0 >= ( m_astGroupData[ nIndex]->nParent)) ? hMenuExitIcon : pahMenuGroup[ m_astGroupData[ nIndex]->nParent - 1];
							::AppendMenu( hWork, MF_POPUP, ( int)pahMenuGroup[ nIndex], m_astGroupData[ nIndex]->szName);
						}
					}
				}
				for( int nIndex = 0; nIndex < m_nGroupCount; nIndex++)
				{
					if( blForce || m_astGroupData[ nIndex]->blEnable)
					{
						InsertIcons( pahMenuGroup[ nIndex], nIndex + 1, cMemDC, blForce);
					}
				}
				delete [] pahMenuGroup;
			}
			InsertIcons( hMenuExitIcon, 0, cMemDC, blForce);

			// ぐへ、汚にゃい
			hMenuPopup = ::CreatePopupMenu();
			if( hMenuPopup)
			{
				if( 0 < GetMenuItemCount( hMenuExitIcon))
				{
					// なんか、イヤーンな感じだ
					HBITMAP hBitmapTemp;
					HBITMAP hBitmap = ::CreateCompatibleBitmap( cMemDC, _ICON_WIDTH, _ICON_HEIGHT);
					hBitmapTemp = ( HBITMAP)::SelectObject( cMemDC, hBitmap);
					Draw( COUNT_DEFAULTICON, cMemDC, CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));
					::SelectObject( cMemDC, hBitmapTemp);
					::AppendMenu( hMenuPopup, MF_POPUP | MF_BITMAP, ( int)hMenuExitIcon, ( LPCTSTR)hBitmap);
					::AppendMenu( hMenuPopup, MF_SEPARATOR, 0, 0);
				}

				// デフォルトのアイコンさんたち！
				for( int nIndex = 0; nIndex < COUNT_DEFAULTICON; nIndex++)
				{
					HBITMAP hBitmapTemp;
					HBITMAP hBitmap = ::CreateCompatibleBitmap( cMemDC, _ICON_WIDTH, _ICON_HEIGHT);
					hBitmapTemp = ( HBITMAP)::SelectObject( cMemDC, hBitmap);
					Draw( nIndex, cMemDC, CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));
					::SelectObject( cMemDC, hBitmapTemp);

					::AppendMenu( hMenuPopup, MF_BITMAP, IDM_ICONIMG00 + nIndex, ( LPCTSTR)hBitmap);
				}
			}
			else
			{
				DestroyMenu( hMenuExitIcon);
			}
		}
	}

	return hMenuPopup;
}

int CIconImgList::InsertIcons( HMENU hMenu, int nID, CDC& cDCWork, BOOL blForce) const
{
	int nResult = 0;

	HMENU hMenuTemp = hMenu;
	int nCount = 0;

	for( int nIndex = 0; nIndex < m_nEnableCount; nIndex++)
	{
		if( nID == m_astIconStatus4Menu[ nIndex]->nParent)
		{
			if( blForce || m_astIconStatus4Menu[ nIndex]->blEnable)
			{
				if( 20 <= nCount)
				{
					HMENU hMenuWork = ::CreatePopupMenu();
					::InsertMenu( hMenuTemp, 0, MF_BYPOSITION | MF_SEPARATOR, NULL, NULL);
					::InsertMenu( hMenuTemp, 0, MF_BYPOSITION | MF_POPUP, ( int)hMenuWork, ( LPCTSTR)TEXT( "&next"));

					nCount = 0;
					hMenuTemp = hMenuWork;
				}

				HBITMAP hBitmapTemp;
				HBITMAP hBitmap = ::CreateCompatibleBitmap( cDCWork, _ICON_WIDTH, _ICON_HEIGHT);
				hBitmapTemp = ( HBITMAP)::SelectObject( cDCWork, hBitmap);
				Draw( m_astIconStatus4Menu[ nIndex]->nID + GetExtendStartIndex(), cDCWork, CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));
				::SelectObject( cDCWork, hBitmapTemp);

				::AppendMenu( hMenuTemp, MF_BITMAP, ( IDM_ICONIMG13 + 1) + 1 + m_astIconStatus4Menu[ nIndex]->nID, ( LPCTSTR)hBitmap);
				nResult++;
				nCount++;
			}
		}
	}
	/*
	int nIconIndex;
	for( int nIndex = 0; nIndex < GetExtendCount(); nIndex++)
	{
		nIconIndex = nIndex + GetExtendStartIndex();
		if( IsInvalid( nIconIndex))
		{
			if( nID == GetParentGroup( nIconIndex))
			{
				HBITMAP hBitmapTemp;
				HBITMAP hBitmap = ::CreateCompatibleBitmap( cDCWork, _ICON_WIDTH, _ICON_HEIGHT);
				hBitmapTemp = ( HBITMAP)::SelectObject( cDCWork, hBitmap);
				Draw( nIconIndex, cDCWork, CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));
				::SelectObject( cDCWork, hBitmapTemp);

				::AppendMenu( hMenuTemp, MF_BITMAP, ( IDM_ICONIMG13 + 1) + 1 + nIndex, ( LPCTSTR)hBitmap);
				nResult++;
				nCount++;
				if( 20 <= nCount)
				{
					HMENU hMenuWork = ::CreatePopupMenu();
					::InsertMenu( hMenuTemp, 0, MF_BYPOSITION | MF_SEPARATOR, NULL, NULL);
					::InsertMenu( hMenuTemp, 0, MF_BYPOSITION | MF_POPUP, ( int)hMenuWork, ( LPCTSTR)TEXT( "&next"));

					nCount = 0;
					hMenuTemp = hMenuWork;
				}
			}
		}
	}
	*/
	return nResult;
}

BOOL CIconImgList::DestroyMenu( HMENU hMenu) const
{


	MENUITEMINFO stMenuInfo;
	ZeroMemory( &stMenuInfo, sizeof( MENUITEMINFO));
	stMenuInfo.cbSize = sizeof( MENUITEMINFO);
	stMenuInfo.fMask = MIIM_ID | MIIM_TYPE | MIIM_SUBMENU;
	while( ::GetMenuItemInfo( hMenu, 0, TRUE, &stMenuInfo))
	{
		if( NULL != stMenuInfo.hSubMenu)
		{
			::RemoveMenu( hMenu, 0, MF_BYPOSITION);
			DestroyMenu( stMenuInfo.hSubMenu);
		}
		else
		{
			if( MFT_BITMAP == ( MFT_BITMAP & stMenuInfo.fType))
			{
#ifndef _DEBUG
				::DeleteObject( ( HBITMAP)stMenuInfo.dwTypeData);
#else
				if( 0 == ::DeleteObject( ( HBITMAP)stMenuInfo.dwTypeData))
				{
					LPVOID lpMsgBuf = NULL;
					FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
									NULL,
									GetLastError(),
									MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT),
									(LPTSTR)&lpMsgBuf,
									0,
									NULL);

					TCHAR szDebug[ 8];
					wsprintf( szDebug, TEXT( "%d-"), ( stMenuInfo.wID - ( ( IDM_ICONIMG13 + 1) + 1)));
					OutputDebugString( szDebug);
					OutputDebugString( ( LPCTSTR)lpMsgBuf);
					OutputDebugString( TEXT( "\r\n"));

					LocalFree( lpMsgBuf );
				}
				else
				{
					TCHAR szDebug[ 8];
					wsprintf( szDebug, TEXT( "%d-\r\n"), ( stMenuInfo.wID - ( ( IDM_ICONIMG13 + 1) + 1)));
					OutputDebugString( szDebug);
				}
#endif
			}
			::DeleteMenu( hMenu, 0, MF_BYPOSITION);
		}

		ZeroMemory( &stMenuInfo, sizeof( MENUITEMINFO));
		stMenuInfo.cbSize = sizeof( MENUITEMINFO);
		stMenuInfo.fMask = MIIM_ID | MIIM_TYPE | MIIM_SUBMENU;
	}
	::DestroyMenu( hMenu);

	return TRUE;
}


CIconSelectMenu* CIconImgList::CreateIconSelectMenu( CWnd* pcWnd, BOOL blForce) const
{
	CIconSelectMenu* pcIconSelectMenu = NULL;

	HMENU hMenuPopup = NULL;

	HMENU hMenuExitIcon = ::CreatePopupMenu();
	if( hMenuExitIcon)
	{
		CDC* pcDC = pcWnd->GetDC();
		if( pcDC)
		{
			CDC cMemDC;

			cMemDC.CreateCompatibleDC( pcDC);
			pcWnd->ReleaseDC( pcDC);

			HMENU* pahMenuGroup = NULL;
			pahMenuGroup = new HMENU [ m_nGroupCount];
			if( pahMenuGroup)
			{
				ZeroMemory( pahMenuGroup, sizeof( HMENU) * m_nGroupCount);
				for( int nIndex = 0; nIndex < m_nGroupCount; nIndex++)
				{
					if( blForce || m_astGroupData[ nIndex]->blEnable)
					{
						pahMenuGroup[ nIndex] = ::CreatePopupMenu();
						if( NULL != pahMenuGroup[ nIndex])
						{
							//InsertIcons( pahMenuGroup[ nIndex], nIndex + 1, cMemDC, blForce);

							HMENU hWork = ( 0 >= ( m_astGroupData[ nIndex]->nParent)) ? hMenuExitIcon : pahMenuGroup[ m_astGroupData[ nIndex]->nParent - 1];
							::AppendMenu( hWork, MF_POPUP, ( int)pahMenuGroup[ nIndex], m_astGroupData[ nIndex]->szName);
						}
					}
				}
				for( int nIndex = 0; nIndex < m_nGroupCount; nIndex++)
				{
					if( blForce || m_astGroupData[ nIndex]->blEnable)
					{
						InsertIconsEx( pahMenuGroup[ nIndex], nIndex + 1, cMemDC, blForce);
					}
				}

				delete [] pahMenuGroup;
			}
			InsertIconsEx( hMenuExitIcon, 0, cMemDC, blForce);

			// ぐへ、汚にゃい
			hMenuPopup = ::CreatePopupMenu();
			if( hMenuPopup)
			{
				if( 0 < GetMenuItemCount( hMenuExitIcon))
				{
					// なんか、イヤーンな感じだ
					HBITMAP hBitmapTemp;
					HBITMAP hBitmap = ::CreateCompatibleBitmap( cMemDC, _ICON_WIDTH, _ICON_HEIGHT);
					hBitmapTemp = ( HBITMAP)::SelectObject( cMemDC, hBitmap);
					Draw( COUNT_DEFAULTICON, cMemDC, CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));
					::SelectObject( cMemDC, hBitmapTemp);
					::AppendMenu( hMenuPopup, MF_POPUP | MF_BITMAP, ( int)hMenuExitIcon, ( LPCTSTR)hBitmap);
					::AppendMenu( hMenuPopup, MF_SEPARATOR, 0, 0);
				}

				// デフォルトのアイコンさんたち！
				for( int nIndex = 0; nIndex < COUNT_DEFAULTICON; nIndex++)
				{
					HBITMAP hBitmapTemp;
					HBITMAP hBitmap = ::CreateCompatibleBitmap( cMemDC, _ICON_WIDTH, _ICON_HEIGHT);
					hBitmapTemp = ( HBITMAP)::SelectObject( cMemDC, hBitmap);
					Draw( nIndex, cMemDC, CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));
					::SelectObject( cMemDC, hBitmapTemp);

					::AppendMenu( hMenuPopup, MF_BITMAP, IDM_ICONIMG00 + nIndex, ( LPCTSTR)hBitmap);
				}

				pcIconSelectMenu = new CIconSelectMenu();
				pcIconSelectMenu->Attach( hMenuPopup);
			}
			else
			{
				DestroyMenu( hMenuExitIcon);
			}
		}
	}

	return pcIconSelectMenu;
}

int CIconImgList::InsertIconsEx( HMENU hMenu, int nID, CDC& cDCWork, BOOL blForce) const
{
	int nResult = 0;

	CIconSelectMenu cMenu;

	cMenu.Attach( hMenu);
	int nCount = 0;

	for( int nIndex = 0; nIndex < m_nEnableCount; nIndex++)
	{
		if( nID == m_astIconStatus4Menu[ nIndex]->nParent)
		{
			if( blForce || m_astIconStatus4Menu[ nIndex]->blEnable)
			{
				if( 20 <= nCount)
				{
					HMENU hMenuWork = ::CreatePopupMenu();

					cMenu.InsertMenu( 0, MF_BYPOSITION | MF_SEPARATOR);
					cMenu.InsertMenu( 0, MF_BYPOSITION | MF_POPUP, ( UINT)hMenuWork, ( LPCTSTR)TEXT( "&next"));

					nCount = 0;

					cMenu.Detach();
					cMenu.Attach( hMenuWork);
				}

				HBITMAP hBitmapTemp;
				HBITMAP hBitmap = ::CreateCompatibleBitmap( cDCWork, _ICON_WIDTH, _ICON_HEIGHT);
				hBitmapTemp = ( HBITMAP)::SelectObject( cDCWork, hBitmap);
				Draw( m_astIconStatus4Menu[ nIndex]->nID + GetExtendStartIndex(), cDCWork, CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));
				::SelectObject( cDCWork, hBitmapTemp);

				cMenu.AppendMenu( &cDCWork, ( IDM_ICONIMG13 + 1) + 1 + m_astIconStatus4Menu[ nIndex]->nID, m_astIconStatus4Menu[ nIndex]->cStrIconName, hBitmap);

				nResult++;
				nCount++;
			}
		}
	}

	cMenu.Detach();

	return nResult;
}
