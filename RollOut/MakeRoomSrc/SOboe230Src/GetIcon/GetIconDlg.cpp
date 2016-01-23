#include "stdafx.h"
#include "resource.h"
#include "GetIconDlg.h"
#include "util.h"
#include "base64.h"

extern HINSTANCE g_hInstance;

CLoginDialog::CLoginDialog( LPCTSTR lpcszID, LPCTSTR lpcszPW, HWND hWndParent) : CDialog( hWndParent)
{
	lstrcpyn( m_szID, ( NULL != lpcszID) ? lpcszID : TEXT( ""), 128);
	lstrcpyn( m_szPW, ( NULL != lpcszPW) ? lpcszPW : TEXT( ""), 128);
}

CLoginDialog::~CLoginDialog( void)
{
}

LPCTSTR CLoginDialog::GetID( void) const
{
	return m_szID;
}

LPCTSTR CLoginDialog::GetPW( void) const
{
	return m_szPW;
}

int CLoginDialog::DoModal( void)
{
	return DialogBoxParam( g_hInstance, MAKEINTRESOURCE( IDD_LOGINIDALOG), m_hWndParent, ( DLGPROC)CLoginDialog::LoginDialogProc, ( LPARAM)this);
}

void CLoginDialog::OnInitDialog( void)
{
	CenterWindow();
	SendDlgItemMessage( IDC_USERID, WM_SETTEXT, 0, ( LPARAM)m_szID);
	SendDlgItemMessage( IDC_USERPW, WM_SETTEXT, 0, ( LPARAM)m_szPW);
}

BOOL CLoginDialog::OnCommand( WORD wID, WORD wNotifyCode)
{
	if( IDOK == wID)
	{
		SendDlgItemMessage( IDC_USERID, WM_GETTEXT, 128, ( LPARAM)m_szID);
		SendDlgItemMessage( IDC_USERPW, WM_GETTEXT, 128, ( LPARAM)m_szPW);
		CDialog::OnOK();
	}
	else
	{
		if( IDCANCEL == wID)
		{
			CDialog::OnCancel();
		}
	}
	return FALSE;
}

BOOL CLoginDialog::LoginDialogProc( HWND hDlg, UINT unMessage, WPARAM wParam ,LPARAM lParam)
{
	if( WM_INITDIALOG == unMessage)
	{
		SetWindowLong( hDlg, DLGWINDOWEXTRA, lParam);
		CLoginDialog* pcLoginDlg = ( CLoginDialog*)lParam;
		pcLoginDlg->m_hWnd = hDlg;

		pcLoginDlg->OnInitDialog();

		return TRUE;
	}
	else
	{
		CLoginDialog* pcLoginDlg = ( CLoginDialog*)GetWindowLong( hDlg, DLGWINDOWEXTRA);;

		switch( unMessage)
		{
		case WM_COMMAND:
			return pcLoginDlg->OnCommand( LOWORD( wParam), HIWORD( wParam));
		}
	}
	return FALSE;
}

LPCTSTR CGetIconDialog::m_slpszDefaultURL = _T( "http://www.hh.iij4u.or.jp/~chiba/GetIcon.xml");

CGetIconDialog::CGetIconDialog( HWND hWndParent) : CDialog( hWndParent), m_piDocument( NULL)
{
	m_blModify = FALSE;

	m_blDragging = FALSE;
	m_nIconDataCount = 0;
}

CGetIconDialog::~CGetIconDialog( void)
{
}

BOOL CGetIconDialog::ListUpIcon( void)
{
	BOOL* pablUserCheck = NULL;
	BOOL blResult = FALSE;
	int* panIndexs = NULL;
	LPBYTE lpabyData = NULL;
	long lGroupCount = 0;
	HTREEITEM* phItemGroups = NULL;
	_variant_t cVariant;
	_bstr_t cBstr;

	int nSize;
	_bstr_t ctBstr;
	_variant_t stVariant;
	_bstr_t cBstrName;
	HIMAGELIST hImage;
	HBITMAP hBmpIconimage;
	long lIndex;
	long lIconsCount = 0;

	MSXML2::IXMLDOMElementPtr piParent;
	MSXML2::IXMLDOMElementPtr piIcon;
	MSXML2::IXMLDOMElementPtr piIconImage;
	MSXML2::IXMLDOMNodeListPtr piIconsList;
	MSXML2::IXMLDOMNodeListPtr piGroupsList;
	MSXML2::IXMLDOMElementPtr piGroups;
	MSXML2::IXMLDOMElementPtr piIcons;
	MSXML2::IXMLDOMElementPtr piRoot;

	m_cTrExtIcon.DeleteAllItems();
	m_cTrExtIcon.SetItemHeight( 20);

	SendDlgItemMessage( IDC_LISTICON, WM_SETREDRAW, FALSE, 0);

	if( NULL == m_piDocument)
	{
		goto ErrorTrap;
	}

	piRoot = m_piDocument->GetdocumentElement();
	if( NULL == piRoot)
	{
		goto ErrorTrap;
	}
	piIcons = piRoot->selectSingleNode( _bstr_t( _T( "ICONS")));
	if( NULL != piIcons)
	{
		piIconsList = piIcons->GetchildNodes();
		if( NULL != piIconsList)
		{
			lIconsCount = piIconsList->Getlength();
			if( 0 < lIconsCount)
			{
				hImage = ImageList_Create( 16, 16, ILC_COLOR, 0, 5);
				lpabyData = new BYTE [ 32];
				if( NULL == lpabyData)
				{
					goto ErrorTrap;
				}

				BITMAP stBitmap;
				stBitmap.bmType = 0;
				stBitmap.bmWidth = 16;
				stBitmap.bmHeight = 16;
				stBitmap.bmWidthBytes = 2;
				stBitmap.bmPlanes = 1;
				stBitmap.bmBitsPixel = 1;
				FillMemory( lpabyData, 32, 0xFF);
				stBitmap.bmBits = lpabyData;
				hBmpIconimage = CreateBitmapIndirect( &stBitmap);
				ImageList_Add( hImage, hBmpIconimage, NULL);
				DeleteObject( hBmpIconimage);
				delete [] lpabyData;
				lpabyData = NULL;
				TCHAR szWork[ 32];

				panIndexs = new int [ lIconsCount];
				if( NULL != panIndexs)
				{
					for( lIndex = 0; lIndex < lIconsCount; lIndex++)
					{
						panIndexs[ lIndex] = 0;
						wsprintf( szWork, _T( "//ICON[@ID='%d']"), lIndex + 1);
						piIcon = piIcons->selectSingleNode( _bstr_t( szWork));
						if( NULL == piIcon)
						{
							continue;
						}

						piIconImage = piIconImage = piIcon->selectSingleNode( _bstr_t( _T( "IMAGE")));
						ctBstr = piIconImage->Gettext();
						if( 0 >= ctBstr.length())
						{
							continue;
						}

						nSize = Base64Decode( NULL, 0, ctBstr);
						lpabyData = new BYTE [ nSize];
						Base64Decode( lpabyData, nSize, ctBstr);

						hBmpIconimage = ::CreateBitmap( _ICON_WIDTH, _ICON_HEIGHT, 1, 1, lpabyData);
						// 登録した場所覚えておかないとね！
						panIndexs[ lIndex] = ImageList_Add( hImage, hBmpIconimage, NULL);

						DeleteObject( hBmpIconimage);

						delete [] lpabyData;
						lpabyData = NULL;
					}
					m_cTrExtIcon.SetImageList( hImage, TVSIL_NORMAL);
				}
			}
		}
	}

	if( 0 < lIconsCount)
	{
		pablUserCheck = new BOOL [ lIconsCount];
		ZeroMemory( pablUserCheck, sizeof( BOOL) * lIconsCount);
	}

	piGroups = piRoot->selectSingleNode( _bstr_t( _T( "GROUPS")));
	if( NULL != piGroups)
	{

		SetTree( TVI_ROOT, piRoot, piGroups, panIndexs, pablUserCheck);
	}

	HTREEITEM hItem;
	TCHAR szWork[ 32];
	for( lIndex = 0; lIndex < lIconsCount; lIndex++)
	{
		if( FALSE == pablUserCheck[ lIndex])
		{
			wsprintf( szWork, _T( "//ICON[@ID='%d']"), lIndex + 1);
			piIcon = piRoot->selectSingleNode( _bstr_t( szWork));
			if( NULL != piIcon)
			{
				cVariant = piIcon->getAttribute( _bstr_t( _T( "TITLE")));
				cVariant.ChangeType( VT_BSTR);
				cBstr = cVariant.bstrVal;

				hItem = m_cTrExtIcon.InsertItem( cBstr,
													( ( NULL == panIndexs) ? 0 : panIndexs[ lIndex]),
													( ( NULL == panIndexs) ? 0 : panIndexs[ lIndex]),
													TVI_ROOT, TVI_LAST, ( LPARAM)( lIndex + 1));
				m_cTrExtIcon.SetCheck( hItem, TRUE);
			}
		}
	}

ErrorTrap:
	SendDlgItemMessage( IDC_LISTICON, WM_SETREDRAW, TRUE, 0);
	delete [] panIndexs;
	panIndexs = NULL;

	delete [] lpabyData;
	lpabyData = NULL;

	delete [] phItemGroups;
	phItemGroups = NULL;

	delete [] pablUserCheck;
	pablUserCheck = NULL;

	return blResult;
}

BOOL CGetIconDialog::SetTree( HTREEITEM hParent, MSXML2::IXMLDOMElementPtr& piRoot, MSXML2::IXMLDOMElementPtr& piGroup, const int* const lpcanIconIndex, BOOL* const lpablIconsCheck)
{
	MSXML2::IXMLDOMNodeListPtr piGroupList;
	piGroupList = piGroup->GetchildNodes();

	MSXML2::IXMLDOMElementPtr piNode;
	MSXML2::IXMLDOMElementPtr piChild;

	HTREEITEM hThis;
	TCHAR szWork[ 32];
	long lID;
	_bstr_t cBstr;
	_variant_t cVariant;
	long lCount = piGroupList->Getlength();
	for( long lIndex = 0; lIndex < lCount; lIndex++)
	{
		piNode = piGroupList->Getitem( lIndex);
		if( _bstr_t( _T( "GROUP")) == piNode->GetbaseName())
		{
			cBstr = piNode->getAttribute( _bstr_t( _T( "TITLE")));

			hThis = m_cTrExtIcon.InsertItem( cBstr, hParent, TVI_LAST, NULL);
			cVariant = piNode->getAttribute( _bstr_t( _T( "ENABLE")));
			cVariant.ChangeType( VT_BOOL);
			if( TVI_ROOT != hParent && FALSE == m_cTrExtIcon.GetCheck( hParent))
			{
				cVariant.boolVal = VARIANT_FALSE;
			}
			m_cTrExtIcon.SetCheck( hThis, ( VARIANT_FALSE == cVariant.boolVal) ? FALSE : TRUE);

			SetTree( hThis, piRoot, piNode, lpcanIconIndex, lpablIconsCheck);
		}
		else if( _bstr_t( _T( "ITEM")) == piNode->GetbaseName())
		{
			cBstr = piNode->getAttribute( _bstr_t( _T( "REF")));
			cVariant = cBstr;
			cVariant.ChangeType( VT_I4);
			if( 0 < cVariant.intVal)
			{
				lID = cVariant.intVal;
				wsprintf( szWork, _T( "//ICON[@ID='%d']"), lID);
				piChild = piRoot->selectSingleNode( _bstr_t( szWork));
				if( NULL != piChild)
				{
					lpablIconsCheck[ lID - 1] = TRUE;

					cVariant = piChild->getAttribute( _bstr_t( _T( "TITLE")));
					cVariant.ChangeType( VT_BSTR);
					cBstr = cVariant.bstrVal;

					cVariant = piNode->getAttribute( _bstr_t( _T( "ENABLE")));
					cVariant.ChangeType( VT_BOOL);

					hThis = m_cTrExtIcon.InsertItem( cBstr,
														( ( NULL == lpcanIconIndex) ? 0 : lpcanIconIndex[ lID - 1]),
														( ( NULL == lpcanIconIndex) ? 0 : lpcanIconIndex[ lID - 1]),
														hParent, TVI_LAST, ( LPARAM)lID);
					m_cTrExtIcon.SetCheck( hThis, ( VARIANT_FALSE == cVariant.boolVal) ? FALSE : TRUE);
				}
			}
		}
	}

	return FALSE;
}

void CGetIconDialog::CleanupList( void)
{
}

BOOL CGetIconDialog::SaveList( void)
{
	int nGroupIndex = 0;
	int nIconIndex = 0;

	if( NULL != m_piDocument)
	{
		MSXML2::IXMLDOMElementPtr piRoot;
		piRoot = m_piDocument->GetdocumentElement();
		if( NULL == piRoot)
		{
			return TRUE;
		}
		MSXML2::IXMLDOMNodePtr piGroups;
		piGroups = piRoot->selectSingleNode( _bstr_t( _T( "GROUPS")));
		if( NULL != piGroups)
		{
			piRoot->removeChild( piGroups);
		}

		piGroups = m_piDocument->createElement( _bstr_t( _T( "GROUPS")));
		if( NULL != piGroups)
		{
			piRoot->appendChild( piGroups);
		}

		MSXML2::IXMLDOMElementPtr piParent = piGroups;
		if( FALSE != GetTree( TVI_ROOT, piRoot, piParent))
		{
			TCHAR szXmlFilePath[ MAX_PATH];
			GetAppXmlPath( szXmlFilePath);
			m_piDocument->save( szXmlFilePath);
		}
	}

	return TRUE;
}

BOOL CGetIconDialog::GetTree( HTREEITEM hParent, MSXML2::IXMLDOMElementPtr& piRoot, MSXML2::IXMLDOMElementPtr& piParent)
{
	HTREEITEM hChild;
	TVITEM stTVItem;
	MSXML2::IXMLDOMElementPtr piNode;
	_variant_t cVariant;
	TCHAR szWork[ 128];

	hChild = m_cTrExtIcon.GetChildItem( hParent);
	while( NULL != hChild)
	{
		ZeroMemory( szWork, sizeof( TCHAR) * 128);
		stTVItem.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_TEXT;
		stTVItem.hItem = hChild;
		stTVItem.pszText = szWork;
		stTVItem.cchTextMax = 128;
		m_cTrExtIcon.GetItem( &stTVItem);

		if( NULL == ( LPVOID)stTVItem.lParam)
		{
			piNode = m_piDocument->createElement( _bstr_t( _T( "GROUP")));
			if( NULL != piNode)
			{
				piNode->setAttribute( _bstr_t( _T( "TITLE")), _variant_t( szWork));

				cVariant = bool(( 0 != ( INDEXTOSTATEIMAGEMASK( TVIS_SELECTED) & stTVItem.state)) ? true : false);
				VariantChangeType( &cVariant, &cVariant, VARIANT_ALPHABOOL, VT_BSTR);
				piNode->setAttribute( _bstr_t( _T( "ENABLE")), cVariant);

				piParent->appendChild( piNode);

				if( FALSE == GetTree( hChild, piRoot, piNode))
				{
					return FALSE;
				}
			}
		}
		else
		{
			piNode = m_piDocument->createElement( _bstr_t( _T( "ITEM")));
			if( NULL != piNode)
			{
				cVariant = bool(( 0 != ( INDEXTOSTATEIMAGEMASK( TVIS_SELECTED) & stTVItem.state)) ? true : false);
				VariantChangeType( &cVariant, &cVariant, VARIANT_ALPHABOOL, VT_BSTR);
				piNode->setAttribute( _bstr_t( _T( "ENABLE")), cVariant);

				cVariant = long( stTVItem.lParam);
				piNode->setAttribute( _bstr_t( _T( "REF")), cVariant);

				piParent->appendChild( piNode);
			}
		}
		hChild = m_cTrExtIcon.GetNextSiblingItem( hChild);
	}

	return TRUE;
}

BOOL CGetIconDialog::SaveTreeItem( HTREEITEM hItem, int nParent, int& nGroupIndex, int& nIconIndex, LPTSTR lpszWork, int nWorkSize)
{
	HTREEITEM hChild;
	TVITEM stTVItem;

	hChild = m_cTrExtIcon.GetChildItem( hItem);
	while( NULL != hChild)
	{
		ZeroMemory( lpszWork, sizeof( TCHAR) * nWorkSize);
		stTVItem.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_TEXT;
		stTVItem.hItem = hChild;
		stTVItem.pszText = lpszWork;
		stTVItem.cchTextMax = nWorkSize;
		m_cTrExtIcon.GetItem( &stTVItem);

		_variant_t cVariant;
		MSXML2::IXMLDOMElementPtr piRoot;
		MSXML2::IXMLDOMElementPtr piGroups;
		MSXML2::IXMLDOMElementPtr piIcon;
		MSXML2::IXMLDOMElementPtr piParent;
		piRoot = m_piDocument->GetdocumentElement();
		if( NULL == piRoot)
		{
			return FALSE;
		}

		if( NULL == ( LPVOID)stTVItem.lParam)
		{
			// グループ
			nGroupIndex++;

			MSXML2::IXMLDOMElementPtr piGroup;
			piGroup = m_piDocument->createElement( _bstr_t( _T( "GROUP")));
			piGroup->Puttext( _bstr_t( lpszWork));

			piGroup->setAttribute( _bstr_t( _T( "ID")), _variant_t( static_cast<long>( nGroupIndex), VT_I4));

			cVariant = bool(( 0 != ( INDEXTOSTATEIMAGEMASK( TVIS_SELECTED) & stTVItem.state)) ? true : false);
			VariantChangeType( &cVariant, &cVariant, VARIANT_ALPHABOOL, VT_BSTR);
			piGroup->setAttribute( _bstr_t( _T( "ENABLE")), cVariant);


			piGroups = piRoot->selectSingleNode( _bstr_t( _T( "GROUPS")));
			if( NULL == piGroups)
			{
				return FALSE;
			}

			piGroups->appendChild( piGroup);

			if( FALSE == SaveTreeItem( hChild, nGroupIndex, nGroupIndex, nIconIndex, lpszWork, nWorkSize))
			{
				return FALSE;
			}
		}
		else
		{
			nIconIndex++;

			// アイコン
			wsprintf( lpszWork, _T( "//ICON[@ID='%d']"), stTVItem.lParam);
			piIcon = piRoot->selectSingleNode( _bstr_t( lpszWork));
			if( NULL == piIcon)
			{
				continue;
			}
			piParent = piIcon->selectSingleNode( _bstr_t( "PARENT"));
			if( NULL == piParent)
			{
				continue;
			}
			piParent->setAttribute( _bstr_t( _T( "INDEX")), _variant_t( static_cast<long>( nIconIndex), VT_I4));
			wsprintf( lpszWork, TEXT( "%d"), nParent);
			piParent->Puttext( _bstr_t( lpszWork));

			// 有効無効
			cVariant = bool(( 0 != ( INDEXTOSTATEIMAGEMASK( TVIS_SELECTED) & stTVItem.state)) ? true : false);
			VariantChangeType( &cVariant, &cVariant, VARIANT_ALPHABOOL, VT_BSTR);
			piIcon->setAttribute( _bstr_t( _T( "ENABLE")), cVariant);
		}

		hChild = m_cTrExtIcon.GetNextSiblingItem( hChild);
	}

	return TRUE;
}

void CGetIconDialog::OnInitDialog( void)
{
	m_cTrExtIcon.Attach( GetDlgItem( IDC_EXTICON));

	if( SUCCEEDED( m_piDocument.CreateInstance( L"Microsoft.XMLDOM")))
	{
		TCHAR szXmlFilePath[ MAX_PATH];
		GetAppXmlPath( szXmlFilePath);

		m_piDocument->PutvalidateOnParse( VARIANT_TRUE);
		if( VARIANT_FALSE == m_piDocument->load( _variant_t( _bstr_t( szXmlFilePath))))
		{
			MSXML2::IXMLDOMParseErrorPtr piError;

			piError = m_piDocument->GetparseError();
			long lCode = piError->GeterrorCode();
			if( NULL != piError && 0 != lCode && 0x800C0005/*INET_E_RESOURCE_NOT_FOUND*/ != lCode)
			{
				long lRaw = piError->Getline();
				long lCol = piError->Getlinepos();

				_bstr_t cBstr = piError->Getreason();
				_bstr_t cBstrSrc = piError->GetsrcText();

				TCHAR szWork[ 31];

				::EnableWindow( GetDlgItem( IDOK), FALSE);
				::EnableWindow( GetDlgItem( IDCANCEL), TRUE);
				::EnableWindow( GetDlgItem( IDC_START), FALSE);
				::EnableWindow( GetDlgItem( IDC_EXTICON), FALSE);
				SendDlgItemMessage( IDC_ICONDATA, EM_REPLACESEL, FALSE, ( LPARAM)_T( "拡張アイコンデータファイルを解析できませんでした。\r\n"));
				SendDlgItemMessage( IDC_ICONDATA, EM_REPLACESEL, FALSE, ( LPARAM)( ( char*)cBstr));
				SendDlgItemMessage( IDC_ICONDATA, EM_REPLACESEL, FALSE, ( LPARAM)_T( "\r\n"));
				SendDlgItemMessage( IDC_ICONDATA, EM_REPLACESEL, FALSE, ( LPARAM)( ( char*)cBstrSrc));
				SendDlgItemMessage( IDC_ICONDATA, EM_REPLACESEL, FALSE, ( LPARAM)_T( "\r\n"));
				wsprintf( szWork, _T( "位置：%d行 %d文字\r\n"), lRaw, lCol);
				SendDlgItemMessage( IDC_ICONDATA, EM_REPLACESEL, FALSE, ( LPARAM)szWork);

				return;
			}
		}
	}

	ListUpIcon();
}

void CGetIconDialog::OnDestroy( void)
{
	CleanupList();

	HIMAGELIST hImageList = m_cTrExtIcon.SetImageList( NULL, TVSIL_NORMAL);

	ImageList_Destroy( hImageList);

	m_cTrExtIcon.Detach();
}

BOOL CGetIconDialog::OnCommand( WORD wID, WORD wNotifyCode)
{
	switch( wID)
	{
	case IDC_START:
		{
			if( IsWindowEnabled( GetDlgItem( IDOK)))
			{
				if( FALSE != OnStartDownLoad())
				{
					::EnableWindow( GetDlgItem( IDOK), FALSE);
					::EnableWindow( GetDlgItem( IDCANCEL), FALSE);
					::EnableWindow( GetDlgItem( IDC_EXTICON), FALSE);

					::SetWindowText( GetDlgItem( IDC_START), _T( "中止"));

					SendDlgItemMessage( IDC_ICONDATA, WM_SETTEXT, 0, ( LPARAM)"アイコンデータをダウンロード中です");
				}
			}
			else
			{
				if( NULL != m_piHttpReq)
				{
					m_piHttpReq->abort();

					::EnableWindow( GetDlgItem( IDC_START), FALSE);
				}
			}
		}
		return TRUE;

	case IDOK:
		if( BN_CLICKED == wNotifyCode)
		{
			SaveList();
			EndDialog( m_hWnd, IDOK);
			return TRUE;
		}
		break;

	case IDCANCEL:
		if( BN_CLICKED == wNotifyCode)
		{
			EndDialog( m_hWnd, m_blModify ? IDOK : IDCANCEL);
			return TRUE;
		}
		break;

	}
	return FALSE;
}

struct GETICONTHREADPARAM
{
	IUnknown* piUnknown;
	HWND hParent;
	LPCTSTR lpcszStationURL;
	HANDLE hStartEvent;
};

void GetIconThread( LPVOID lpv)
{
	GETICONTHREADPARAM* lpGetIconParam = reinterpret_cast< GETICONTHREADPARAM*>( lpv);
	MSXML2::IXMLHTTPRequestPtr piHttpReq;
	long lStatus = 0;

	HWND hParent = lpGetIconParam->hParent;
	try
	{
		_bstr_t cBstrStationURL = lpGetIconParam->lpcszStationURL;

		lpGetIconParam->piUnknown->QueryInterface( &piHttpReq);


		::SetEvent( lpGetIconParam->hStartEvent);

		_bstr_t cBstrUser;
		_bstr_t cBstrPass;

		while( TRUE)
		{
			piHttpReq->open( _bstr_t( _T( "GET")), cBstrStationURL, false, cBstrUser, cBstrPass);
			piHttpReq->send();

			if( 4 == piHttpReq->GetreadyState())
			{
				lStatus = piHttpReq->Getstatus();
				if( /*HTTP_STATUS_PROXY_AUTH_REQ*/407 == lStatus || /*HTTP_STATUS_DENIED*/401 == lStatus)
				{
					KillTimer( hParent, ID_TIMEOUT);

					CLoginDialog cLoginDlg( cBstrUser, cBstrPass, hParent);

					if( IDOK != cLoginDlg.DoModal())
					{
						break;
					}

					cBstrUser = cLoginDlg.GetID();
					cBstrPass = cLoginDlg.GetPW();

					SetTimer( hParent, ID_TIMEOUT, 60 * 1000, NULL);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch( _com_error e)
	{
		_bstr_t cBtr = e.Description();
		OutputDebugString( ( ( LPCTSTR)cBtr));
		OutputDebugString( "\n");
		OutputDebugString( e.ErrorMessage());
		OutputDebugString( "\n");
	}
	::PostMessage( hParent, WM_USER + 100, 0, ( ( 200/*HTTP_STATUS_OK*/ == lStatus) ? TRUE : FALSE));
}

void CGetIconDialog::OnUserMessage( WPARAM wParam, LPARAM lParam)
{
	KillTimer( m_hWnd, ID_TIMEOUT);

	::EnableWindow( GetDlgItem( IDOK), TRUE);
	::EnableWindow( GetDlgItem( IDCANCEL), TRUE);
	::EnableWindow( GetDlgItem( IDC_START), TRUE);
	::EnableWindow( GetDlgItem( IDC_EXTICON), TRUE);
	SendDlgItemMessage( IDC_ICONDATA, WM_SETTEXT, 0, ( LPARAM)"");

	::SetWindowText( GetDlgItem( IDC_START), _T( "はじめ！"));

	if( FALSE == lParam)
	{
		MessageBox( _T( "アイコンデータを取得できませんでした"), _T( "お願い！アイコンとってきて。"), MB_OK | MB_ICONHAND);
		return;
	}

	MSXML2::IXMLDOMDocumentPtr piDocuement;
	if( FAILED( piDocuement.CreateInstance( L"Microsoft.XMLDOM")))
	{
		MessageBox( _T( "MSXML2を初期化できませんでした"), TEXT( "お願い！アイコンとってきて。"), MB_OK | MB_ICONHAND);
		return;
	}

	if( VARIANT_FALSE == piDocuement->loadXML( m_piHttpReq->GetresponseText()))
	{
		MessageBox( _T( "XMLを解析できませんでした"), TEXT( "お願い！アイコンとってきて。"), MB_OK | MB_ICONHAND);
		return;
	}

	MSXML2::IXMLDOMElementPtr piLocalGroups;
	MSXML2::IXMLDOMElementPtr piRoot;
	MSXML2::IXMLDOMElementPtr piLocalRoot;

	piLocalRoot = m_piDocument->GetdocumentElement();
	if( NULL == piLocalRoot)
	{
		m_piDocument = piDocuement;
		goto finally;
	}
	piLocalGroups = piLocalRoot->selectSingleNode( _bstr_t( _T( "GROUPS")));
	if( NULL == piLocalGroups)
	{
		m_piDocument = piDocuement;
		goto finally;
	}

	piRoot = piDocuement->GetdocumentElement();
	if( NULL == piRoot)
	{
		MessageBox( _T( "XMLを解析できませんでした"), TEXT( "お願い！アイコンとってきて。"), MB_OK | MB_ICONHAND);
		return;
	}

	piRoot->appendChild( piLocalGroups);
	m_piDocument = piDocuement;

finally:
	ListUpIcon();

	SendDlgItemMessage( IDC_ICONDATA, WM_SETTEXT, 0, ( LPARAM)"アイコンデータをダウンロードが終了しました");
}

void CGetIconDialog::OnTimer( UINT unTimerID)
{
	if( NULL != m_piHttpReq)
	{
		m_piHttpReq->abort();

		::EnableWindow( GetDlgItem( IDC_START), FALSE);
	}
	KillTimer( m_hWnd, unTimerID);
}

BOOL CGetIconDialog::OnStartDownLoad()
{
	if( IDOK != MessageBox( TEXT( "アイコンの取得を開始しますか？"), TEXT( "お願い！アイコンとってきて。"), MB_OKCANCEL | MB_ICONQUESTION))
	{
		return FALSE;
	}

	if( FALSE == SaveList())
	{
		if( IDYES != MessageBox( _T( "現状のデータを保存できませんでした。\n現在のデータを破棄して強行しますか？"), _T( "お願い！アイコンとってきて。"), MB_YESNO | MB_ICONQUESTION))
		{
			return FALSE;
		}
	}


	if( FAILED( m_piHttpReq.CreateInstance( L"Msxml2.XMLHTTP")) || NULL == m_piHttpReq)
	{
		MessageBox( _T( "MSXML2を初期化できませんでした。\nmsxml3.dllが必要です。"), TEXT( "お願い！アイコンとってきて。"), MB_OK | MB_ICONHAND);
		return FALSE;
	}

	MSXML2::IXMLDOMDocumentPtr piDocuement;
	if( FAILED( piDocuement.CreateInstance( L"Microsoft.XMLDOM")))
	{
		MessageBox( _T( "MSXML2を初期化できませんでした"), TEXT( "お願い！アイコンとってきて。"), MB_OK | MB_ICONHAND);
		return FALSE;
	}

	TCHAR szStationURL[ 1024];
	_bstr_t cBstrStationURL;

	TCHAR szPath[ MAX_PATH];
	GetAppXmlPath( szPath);
	if( VARIANT_FALSE != piDocuement->load( szPath))
	{
		MSXML2::IXMLDOMElementPtr piRoot;
		MSXML2::IXMLDOMElementPtr piURL;

		piRoot = piDocuement->GetdocumentElement();
		if( NULL != piRoot)
		{
			piURL = piRoot->selectSingleNode( _bstr_t( _T( "STATION")));
			if( NULL != piURL)
			{
				cBstrStationURL = piURL->Gettext();
			}
		}
	}
	if( 0 >= cBstrStationURL.length())
	{
		cBstrStationURL = m_slpszDefaultURL;
	}

	lstrcpy( szStationURL, cBstrStationURL);

	GETICONTHREADPARAM stGetParam;
	stGetParam.hParent = m_hWnd;
	stGetParam.lpcszStationURL = szStationURL;
	m_piHttpReq->QueryInterface( &stGetParam.piUnknown);
	stGetParam.hStartEvent = ::CreateEvent( NULL, FALSE, FALSE, NULL);
	ResetEvent( stGetParam.hStartEvent);

	if( 0 > _beginthread( GetIconThread, 0, reinterpret_cast< LPVOID>( &stGetParam)))
	{
		CloseHandle( stGetParam.hStartEvent);
		MessageBox( _T( "更新スレッドを生成できませんでした"),  _T( "お願い！アイコンとってきて。"), MB_OK | MB_ICONHAND);
		return FALSE;
	}

	if( WAIT_OBJECT_0 != WaitForSingleObject( stGetParam.hStartEvent, 10000))
	{
		CloseHandle( stGetParam.hStartEvent);
		MessageBox( _T( "更新スレッドが起動しませんでした"),  _T( "お願い！アイコンとってきて。"), MB_OK | MB_ICONHAND);
		return FALSE;
	}
	CloseHandle( stGetParam.hStartEvent);

	SetTimer( m_hWnd, ID_TIMEOUT, 60 * 1000, NULL);

	return TRUE;
}

void CGetIconDialog::OnMouseMove( UINT nFlags, POINT stPoint)
{
	if( m_blDragging)
	{
		RECT stRect;
		::GetWindowRect( m_cTrExtIcon.GetSafeHwnd(), &stRect);

		POINT stWork;
		stWork.x = stRect.left;
		stWork.y = stRect.top;
		ScreenToClient( &stWork);

		stPoint.x -= stWork.x;
		stPoint.y -= stWork.y;

		ImageList_DragMove( stPoint.x, stPoint.y);
		HTREEITEM hItem;
		UINT unFlags;
		if( ( hItem = m_cTrExtIcon.HitTest( stPoint, &unFlags)) != NULL)
		{
			ImageList_DragLeave( m_cTrExtIcon.GetSafeHwnd());
			m_cTrExtIcon.SelectDropTarget( hItem);
			m_hitemDrop = hItem;
			ImageList_DragEnter( m_cTrExtIcon.GetSafeHwnd(), stPoint.x, stPoint.y);
		}
	}
}

BOOL CGetIconDialog::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LPNMHDR lpstNMHDR = ( LPNMHDR)lParam;

	if( IDC_EXTICON == lpstNMHDR->idFrom)
	{
		switch( lpstNMHDR->code)
		{
		case TVN_SELCHANGED:
			{
				LPNMTREEVIEW lpstNMTrv = (LPNMTREEVIEW)lpstNMHDR;
				long lIndex = lpstNMTrv->itemNew.lParam;
				if( 0 < lIndex)
				{
					SendDlgItemMessage( IDC_ICONDATA, WM_SETTEXT, 0, ( LPARAM)"");

					MSXML2::IXMLDOMElementPtr piRoot;
					piRoot = m_piDocument->GetdocumentElement();
					if( NULL != piRoot)
					{
						TCHAR szWork[ 1025];
						wsprintf( szWork, _T( "//ICON[@ID='%d']"), lIndex);
						MSXML2::IXMLDOMElementPtr piIcon;
						piIcon = piRoot->selectSingleNode( _bstr_t( szWork));
						if( NULL != piIcon)
						{
							_bstr_t cBstrName;
							cBstrName = piIcon->getAttribute( _bstr_t( "TITLE"));

							MSXML2::IXMLDOMElementPtr piAuthor;
							piAuthor = piIcon->selectSingleNode( _bstr_t( "AUTHOR"));
							if( NULL != piAuthor)
							{
								_bstr_t cBstrAuthor;
								cBstrAuthor = piAuthor->Gettext();

								wsprintf( szWork, "Name : %s\r\nAuther : %s", ( LPCTSTR)cBstrName, ( LPCTSTR)cBstrAuthor);
								SendDlgItemMessage( IDC_ICONDATA, WM_SETTEXT, 0, ( LPARAM)szWork);
							}
						}
					}
				}
				else
				{
					SendDlgItemMessage( IDC_ICONDATA, WM_SETTEXT, 0, ( LPARAM)TEXT( "group!"));
				}
			}
			return TRUE;

		case TVN_ENDLABELEDIT:
			{
				TV_DISPINFO		*pstTVDispInfo;

				pstTVDispInfo = ( TV_DISPINFO*)lpstNMHDR;
				if( pstTVDispInfo->item.pszText != NULL)
				{
					pstTVDispInfo->item.mask = TVIF_TEXT;
					m_cTrExtIcon.SetItem( &pstTVDispInfo->item);
				}
				if( NULL != pResult)*pResult = TRUE;
			}
			return TRUE;

		case TVN_BEGINLABELEDIT:
			{
				TV_DISPINFO *pstTVDispInfo;
				pstTVDispInfo = ( TV_DISPINFO*)lpstNMHDR;
				if( NULL != pResult)*pResult = ( NULL == ( LPVOID)pstTVDispInfo->item.lParam) ? 0 : 1;
			}
			return TRUE;

		case TVN_BEGINDRAG:
		case TVN_BEGINRDRAG:
			if( !m_blDragging)
			{
				POINT		stPntAction;
				UINT		unFlags;

				GetCursorPos( &stPntAction);
				::ScreenToClient( m_cTrExtIcon.GetSafeHwnd(), &stPntAction);

				m_hitemDrag = m_cTrExtIcon.HitTest( stPntAction, &unFlags);;
				if( NULL != m_hitemDrag)
				{
					m_blDragging = TRUE;
					m_hitemDrop = NULL;

					m_hImageList = m_cTrExtIcon.CreateDragImage( m_hitemDrag);
					if( NULL != m_hImageList)
					{
						ImageList_DragShowNolock( TRUE);
						ImageList_SetDragCursorImage( m_hImageList, 0, 0, 0);
						ImageList_BeginDrag( m_hImageList, 0, 0, 0);
						ImageList_DragMove( stPntAction.x, stPntAction.y);
						ImageList_DragEnter( m_cTrExtIcon.GetSafeHwnd(), stPntAction.x, stPntAction.y);
						SetCapture();
						if( NULL != pResult)*pResult = TRUE;
						return TRUE;
					}
				}
#ifdef	_DEBUG
				OutputDebugString( TEXT( "おかしい！\n"));
#endif
			}
		}
	}
	return FALSE;
}

BOOL CGetIconDialog::OnButtonUp( UINT nFlags, POINT stPoint)
{
	if( m_blDragging)
	{
		ImageList_DragLeave( m_cTrExtIcon.GetSafeHwnd());
		ImageList_EndDrag();
		ImageList_Destroy( m_hImageList);
		m_hImageList = NULL;

		if ( m_hitemDrag != m_hitemDrop)
		{
			TVITEM stTVItem;
			ZeroMemory( &stTVItem, sizeof( stTVItem));
			stTVItem.hItem = m_hitemDrop;
			stTVItem.mask = TVIF_HANDLE | TVIF_PARAM;
			m_cTrExtIcon.GetItem( &stTVItem);

			BOOL blToPrev = FALSE;
			if( 0 == stTVItem.lParam)
			{
				HMENU hMenu = LoadMenu( g_hInstance, MAKEINTRESOURCE( IDM_CONTEXTMENU));
				if( hMenu)
				{
					HMENU hMenuPopup = GetSubMenu( hMenu, 1);
					if( hMenuPopup)
					{
						ClientToScreen( GetSafeHwnd(), &stPoint);
						UINT unID = TrackPopupMenu( hMenuPopup, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, stPoint.x, stPoint.y, 0, GetSafeHwnd(), NULL);
						if( IDM_TOPREV == unID)blToPrev = TRUE;
					}
					DestroyMenu( hMenu);
				}
			}

			SendDlgItemMessage( IDC_LISTICON, WM_SETREDRAW, FALSE, 0);
			m_cTrExtIcon.TransferItem( m_hitemDrop, m_hitemDrag, blToPrev);
			m_cTrExtIcon.DeleteItem( m_hitemDrag);
			SendDlgItemMessage( IDC_LISTICON, WM_SETREDRAW, TRUE, 0);
		}
		/*
		if (m_hitemDrag != m_hitemDrop && !IsChildNodeOf(m_hitemDrop, m_hitemDrag) && 
															GetParentItem( m_hitemDrag) != m_hitemDrop)
		{
			TransferItem(m_hitemDrag, m_hitemDrop);
			DeleteItem(m_hitemDrag);
		}
		else
			MessageBeep(0);
		*/

		ReleaseCapture();
		m_blDragging = FALSE;
		m_cTrExtIcon.SelectDropTarget(NULL);
	}

	return TRUE;
}

void CGetIconDialog::OnContextMenu( HWND hWnd, POINT stPoint)
{
	if( m_cTrExtIcon.GetSafeHwnd() == hWnd)
	{
		HMENU hMenu = LoadMenu( g_hInstance, MAKEINTRESOURCE( IDM_CONTEXTMENU));
		if( hMenu)
		{
			HMENU hMenuPopup = GetSubMenu( hMenu, 0);
			if( hMenuPopup)
			{
				UINT unID = TrackPopupMenu( hMenuPopup, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, stPoint.x, stPoint.y, 0, GetSafeHwnd(), NULL);

				if( IDM_ADDNODE == unID || IDM_DELNODE == unID)
				{
					POINT		stPntAction;
					UINT		unFlags;

					stPntAction.x = stPoint.x;
					stPntAction.y = stPoint.y;
					::ScreenToClient( m_cTrExtIcon.GetSafeHwnd(), &stPntAction);

					HTREEITEM hItemSel = m_cTrExtIcon.HitTest( stPntAction, &unFlags);
					if( hItemSel)
					{
						if( unID == IDM_ADDNODE)
						{
							HTREEITEM hItemParent = m_cTrExtIcon.GetParentItem( hItemSel);

							HTREEITEM hItemNew = m_cTrExtIcon.InsertItem( TEXT( "new group!"), hItemParent, hItemSel, NULL);

							/*
							if( NULL != hItemNew)
							{
								// チェックボックスを排除
								TVITEM stTVItem;
								stTVItem.mask = TVIF_HANDLE | TVIF_STATE;
								stTVItem.hItem = hItemNew;
								stTVItem.stateMask = TVIS_STATEIMAGEMASK;
								stTVItem.state = 0;
								m_cTrExtIcon.SetItem( &stTVItem);
							}*/
						}
						else
						{
							TVITEM stTVItem;
							stTVItem.mask = TVIF_HANDLE | TVIF_PARAM;
							stTVItem.hItem = hItemSel;
							m_cTrExtIcon.GetItem( &stTVItem);

							if( !stTVItem.lParam)
							{
								HTREEITEM hItemChild = m_cTrExtIcon.GetChildItem( hItemSel);

								if( NULL == hItemChild)
								{
									m_cTrExtIcon.DeleteItem( hItemSel);
								}
								else
								{
									MessageBeep( -1);
								}
							}
						}
					}
				}

			}
			DeleteObject( hMenu);
		}
	}
}

BOOL CGetIconDialog::GetIconDlgProc( HWND hDlg, UINT unMessage, WPARAM wParam, LPARAM lParam)
{
	if( WM_INITDIALOG == unMessage)
	{
		SetWindowLong( hDlg, DLGWINDOWEXTRA, lParam);
		CGetIconDialog* pcGetIconDlg = ( CGetIconDialog*)lParam;
		pcGetIconDlg->m_hWnd = hDlg;

		pcGetIconDlg->OnInitDialog();

		return TRUE;
	}
	else
	{
		CGetIconDialog* pcGetIconDlg = ( CGetIconDialog*)GetWindowLong( hDlg, DLGWINDOWEXTRA);;
		switch( unMessage)
		{
		case WM_COMMAND:
			return pcGetIconDlg->OnCommand( LOWORD( wParam), HIWORD( wParam));
		case WM_DESTROY:
			pcGetIconDlg->OnDestroy();
			return TRUE;
		case WM_NOTIFY:
			{
				LRESULT lResult = 0;
				if( pcGetIconDlg->OnNotify( wParam, lParam, &lResult))
				{
					SetWindowLong( pcGetIconDlg->GetSafeHwnd(), DWL_MSGRESULT, lResult);
					return TRUE;
				}
			}
			break;
		case WM_CONTEXTMENU:
			{
				POINT stPoint;
				stPoint.x = LOWORD(lParam);
				stPoint.y = HIWORD(lParam);
				pcGetIconDlg->OnContextMenu( ( HWND)wParam, stPoint);
				return TRUE;
			}

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			{
				POINT stPoint;
				stPoint.x = LOWORD(lParam);
				stPoint.y = HIWORD(lParam);
				return pcGetIconDlg->OnButtonUp( wParam, stPoint);
			}

		case WM_MOUSEMOVE:
			{
				POINT stPoint;
				stPoint.x = LOWORD(lParam);
				stPoint.y = HIWORD(lParam);
				pcGetIconDlg->OnMouseMove( wParam, stPoint);
			}
			return TRUE;

		case ( WM_USER + 100):
			pcGetIconDlg->OnUserMessage( wParam, lParam);
			return TRUE;

		case WM_TIMER:
			pcGetIconDlg->OnTimer( wParam);
			return TRUE;
		}
	}
	return FALSE;
}

int CGetIconDialog::DoModal( void)
{
	return DialogBoxParam( g_hInstance, MAKEINTRESOURCE( IDD_GETICONDIALOG), m_hWndParent, ( DLGPROC)CGetIconDialog::GetIconDlgProc, ( LPARAM)this);
}

int GetIconDlg_DoModal( HWND hWnd)
{
	int nResult = IDCANCEL;

	WNDCLASSEX stWndClassEx;

	stWndClassEx.cbSize = sizeof( WNDCLASSEX);
	stWndClassEx.style = CS_DBLCLKS | CS_SAVEBITS | CS_BYTEALIGNWINDOW;
	stWndClassEx.lpfnWndProc = DefDlgProc;
	stWndClassEx.cbClsExtra = 0;
	stWndClassEx.cbWndExtra = DLGWINDOWEXTRA + sizeof( LPVOID);
	stWndClassEx.hInstance = g_hInstance;
	stWndClassEx.hIcon = LoadIcon( NULL, IDI_WINLOGO);
	stWndClassEx.hCursor = LoadCursor( NULL, IDC_ARROW);
	stWndClassEx.hbrBackground = ( HBRUSH)( COLOR_WINDOW + 1);
	stWndClassEx.lpszMenuName = NULL;
	stWndClassEx.lpszClassName = "GetIconDLL";
	stWndClassEx.hIconSm = LoadIcon( NULL, IDI_WINLOGO);
	if( 0 != RegisterClassEx( &stWndClassEx))
	{
		CGetIconDialog	cGetIconDlg( hWnd);

		nResult = cGetIconDlg.DoModal();

		UnregisterClass( stWndClassEx.lpszClassName, stWndClassEx.hInstance);
	}

	return nResult;
}
