#include "stdafx.h"
#include "soboe.h"
#include "MainFrm.h"
#include "CatalogList.h"

CCatalogList::~CCatalogList( void)
{
	MENUITEM*	pstMenuItem;
	while( GetCount())
	{
		pstMenuItem = RemoveHead();
		ASSERT( pstMenuItem);
		delete pstMenuItem;
	}
}

BOOL CCatalogList::AppendData( HDC hWorkDC, UINT nItemID, const CMemoData* pccMemoData)
{
	MENUITEM*	pstMenuItem = new MENUITEM;
	if( pstMenuItem)
	{
		AddTail( pstMenuItem);

		int			nXSpace = ( GetSystemMetrics( SM_CXFRAME) * 2);
		int			nYSpace = ( GetSystemMetrics( SM_CYBORDER) * 2);

		pccMemoData->GetTitle( pstMenuItem->cStrTitle);
		pstMenuItem->nIndex = pccMemoData->GetIconIndex();
		pstMenuItem->unItemID = nItemID;
		pccMemoData->GetForeColor( pstMenuItem->clrFore);
		pccMemoData->GetBackColor( pstMenuItem->clrBack);
		LOGFONT	stFont;
		pccMemoData->GetFont( &stFont);
		lstrcpy( pstMenuItem->lfFaceName, stFont.lfFaceName);

		ZeroMemory( &stFont, sizeof(LOGFONT));
		stFont.lfCharSet = DEFAULT_CHARSET;
		stFont.lfHeight = _ICON_HEIGHT - nYSpace;
		lstrcpy( stFont.lfFaceName, pstMenuItem->lfFaceName);
		HFONT hFont = ::CreateFontIndirect( &stFont);
		
		HFONT hFontOld = ( HFONT)::SelectObject( hWorkDC, hFont);

			RECT	stRect;
			stRect.top = 0;
			stRect.left = 0;
			stRect.right = 1024;
			stRect.bottom = 400;
			::DrawText( hWorkDC, pstMenuItem->cStrTitle, -1, &stRect, DT_LEFT | DT_CALCRECT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
			pstMenuItem->stSize.cx = ( stRect.right - stRect.left) + nXSpace + _ICON_WIDTH;
			pstMenuItem->stSize.cy = _ICON_HEIGHT;

		::SelectObject( hWorkDC, hFontOld);
		::DeleteObject( hFont);

		return TRUE;
	}
	return FALSE;
}
