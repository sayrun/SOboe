// IconSelectMenu.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SOboe.h"
#include "IconSelectMenu.h"
#include ".\iconselectmenu.h"


// CIconSelectMenu

CIconSelectMenu::CIconSelectMenu() : CMenu()
{
}

CIconSelectMenu::~CIconSelectMenu()
{
	DestroyMenu();
}


// CIconSelectMenu メンバ関数

void CIconSelectMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO :  指定された項目を描画するためのコードを追加してください。
	MENUITEM* pItem = reinterpret_cast< MENUITEM*>( lpDrawItemStruct->itemData);
	if( NULL != pItem)
	{
		CDC cDC;
		if( cDC.Attach( lpDrawItemStruct->hDC))
		{
			int nSaveID = cDC.SaveDC();

			if( 0 < nSaveID)
			{
				COLORREF clrFore;
				COLORREF clrBack;
				if( ODS_SELECTED & lpDrawItemStruct->itemState)
				{
					clrFore = ::GetSysColor( COLOR_HIGHLIGHTTEXT);
					clrBack = ::GetSysColor( COLOR_HIGHLIGHT);
				}
				else
				{
					clrFore = ::GetSysColor( COLOR_MENUTEXT);
					clrBack = ::GetSysColor( COLOR_MENU);
				}

				cDC.SetBkColor( clrBack);
				cDC.SetTextColor( clrFore);
				cDC.FillSolidRect( &lpDrawItemStruct->rcItem, clrBack);

				int nXSpace = ::GetSystemMetrics( SM_CXFRAME);

				int nLeft = 12;//( ( lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left) - pItem->nWidth) / 2;

				CDC cMemDC;
				cMemDC.CreateCompatibleDC( &cDC);
				cMemDC.SelectObject( pItem->hBitmap);
				cDC.BitBlt( nLeft + lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, _ICON_WIDTH, _ICON_HEIGHT, &cMemDC, 0, 0, SRCCOPY);
				cDC.TextOut( nLeft + lpDrawItemStruct->rcItem.left + _ICON_WIDTH + nXSpace, lpDrawItemStruct->rcItem.top, pItem->szItemName);


				cDC.RestoreDC( nSaveID);
			}
			cDC.Detach();
		}
	}
}

void CIconSelectMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO :  指定された項目のサイズを決定するコードを追加してください。
	MENUITEM* pItem = reinterpret_cast< MENUITEM*>( lpMeasureItemStruct->itemData);
	if( NULL != pItem)
	{
		lpMeasureItemStruct->itemHeight = pItem->nHeight;
		lpMeasureItemStruct->itemWidth = pItem->nWidth;
	}
}

BOOL CIconSelectMenu::AppendMenu( CDC* pcDC, UINT nItemID, LPCTSTR lpcszItem, HBITMAP hBitmap)
{
	MENUITEM* pItem = reinterpret_cast< MENUITEM*>( malloc( sizeof( MENUITEM) + lstrlen( lpcszItem)));
	if( NULL != pItem)
	{
		pItem->hBitmap = hBitmap;
		lstrcpy( pItem->szItemName, lpcszItem);

		int nXSpace = ( ::GetSystemMetrics( SM_CXFRAME) * 2);
		int nYSpace = ( ::GetSystemMetrics( SM_CYBORDER) * 2);

		HFONT hFont = static_cast< HFONT>( ::GetStockObject( DEFAULT_GUI_FONT));

		HFONT hFontOld = ( HFONT)pcDC->SelectObject( hFont);

			RECT	stRect;
			stRect.top = 0;
			stRect.left = 0;
			stRect.right = 1024;
			stRect.bottom = 400;
			pcDC->DrawText( lpcszItem, -1, &stRect, DT_LEFT | DT_CALCRECT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
			pItem->nWidth = ( stRect.right - stRect.left) + nXSpace + _ICON_WIDTH;
			pItem->nHeight = max( _ICON_HEIGHT, ( stRect.bottom - stRect.top));

		pcDC->SelectObject( hFontOld);


		if( CMenu::AppendMenu( MF_OWNERDRAW, nItemID, reinterpret_cast< LPCTSTR>( pItem)))
		{
			return TRUE;
		}
		else
		{
			delete pItem;

			return false;
		}
	}
	else
	{
		return FALSE;
	}
}

BOOL CIconSelectMenu::DestroyMenu()
{
	int nCount = this->GetMenuItemCount();
	if( 0 >= nCount)
	{
		return TRUE;
	}

	DeleteMyItem( this->GetSafeHmenu());
	return TRUE;
}

void CIconSelectMenu::DeleteMyItem( HMENU hMenu)
{
	int nCount = ::GetMenuItemCount( hMenu);
	if( 0 < nCount)
	{
		MENUITEMINFO stMenuItemInfo;
		ZeroMemory( &stMenuItemInfo, sizeof( MENUITEMINFO));

		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			stMenuItemInfo.cbSize = sizeof( MENUITEMINFO);
			stMenuItemInfo.fMask = MIIM_DATA | MIIM_SUBMENU;
			::GetMenuItemInfo( hMenu, nIndex, TRUE, &stMenuItemInfo);

			if( 0 != stMenuItemInfo.dwItemData)
			{
				free( reinterpret_cast< void*>( stMenuItemInfo.dwItemData));

				stMenuItemInfo.dwItemData = 0;

				::SetMenuItemInfo( hMenu, nIndex, TRUE, &stMenuItemInfo);
			}
			else
			{
				if( NULL != stMenuItemInfo.hSubMenu)
				{
					DeleteMyItem( stMenuItemInfo.hSubMenu);
				}
			}

		}
	}
}
