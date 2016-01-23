#include "stdafx.h"
#include "SOboe.h"
#include "PrinterDC.h"

CPrinterDC::CPrinterDC( void) : CDC()
{
	// ����͂����A����m_hDC����悤�ɂ���Ηv��ˁ[�񂶂�˂��H
	// �����̂��I�������̕����C���I�ɁI
	m_blAttach = FALSE;
	// �����StartDoc�Ƃ��ł����l���ǂ���łȂ��H
	m_nPage = 1;
}

BOOL CPrinterDC::Attach( HDC hDC, const RECT& stRect)
{
	if( CDC::Attach( hDC))
	{
		m_blAttach = TRUE;

		// +----------------------------PHYSICALWIDTH---------------------------------+
		// | PHYSICALOFFSETY                *--PHYSICALOFFSETY                        |
		// |  | +--------------------------HORZRES-------------------------------+  * |
		// |  * |                                                                |  | |
		// |    |                        ( PHYSICALWIDTH - HORZRES) - PHYSICALOFFSETY |
		// |    |                                                                |    |
		// *--------- PHYSICALHEIGHT                                             |    |
		// |    *---- VERTRES                                                    |    |
		// |    |                                                                |    |
		// |    +----------------------------------------------------------------+    |
		// |                       *--( PHYSICALHEIGHT - VERTRES) - PHYSICALOFFSETY   |
		// +--------------------------------------------------------------------------+
		int nHorzRes = GetDeviceCaps( HORZRES);
		int nVertRes = GetDeviceCaps( VERTRES);

		int	nLogPixelsX = GetDeviceCaps( LOGPIXELSX);
		int	nLogPixelsY = GetDeviceCaps( LOGPIXELSY);
		int nPhysicalWidth = GetDeviceCaps( PHYSICALWIDTH);
		int nPhysicalHeight = GetDeviceCaps( PHYSICALHEIGHT);
		int nPhysicalOffsetX = GetDeviceCaps( PHYSICALOFFSETX);
		int nPhysicalOffsetY = GetDeviceCaps( PHYSICALOFFSETY);

		int nRightMargin = ( nPhysicalWidth - nHorzRes) - nPhysicalOffsetX;
		int nBottomMargin = ( nPhysicalHeight - nVertRes) - nPhysicalOffsetY;

		m_cPaperRect.left = ( ( nLogPixelsX * stRect.left) / 2540);
		m_cPaperRect.right = nHorzRes - max( ( ( ( nLogPixelsX * stRect.right) / 2540) - nRightMargin), nRightMargin);
		m_cPaperRect.top = ( ( nLogPixelsY * stRect.top) / 2540);
		m_cPaperRect.bottom = nVertRes - max( ( ( ( nLogPixelsY * stRect.bottom) / 2540) - nBottomMargin), nBottomMargin);

		m_nCenterPos = ( nPhysicalHeight / 2) - nPhysicalOffsetY;

		// �t�b�^�ʒu�̒���
		CString	cStrPageTest( "00");
		CRect	cRctPage = m_cPaperRect;
		DrawText( cStrPageTest, cRctPage, DT_LEFT | DT_CALCRECT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
		m_cRctPage = m_cPaperRect;
		if( m_cPaperRect.bottom > ( nVertRes + nPhysicalOffsetY) - cRctPage.Height())
		{
			m_cRctPage.top = ( nVertRes + nPhysicalOffsetY) - cRctPage.Height();
			m_cPaperRect.bottom = m_cRctPage.top;
		}
		else
		{
			m_cRctPage.top = m_cPaperRect.bottom;
		}
		m_cRctPage.bottom = m_cRctPage.top + cRctPage.Height();

		// �������̈悪����
		if( m_cPaperRect.right <= m_cPaperRect.left)return FALSE;
		if( m_cPaperRect.bottom <= m_cPaperRect.top)return FALSE;

		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		m_blColor = pcSOboe->IsColorPrint();

		return TRUE;
	}
	return FALSE;
}

BOOL CPrinterDC::PrintForm( void)
{
	if( !m_blAttach)return FALSE;

	// �t�b�^�[�������
	int nOldMode = SetBkMode( TRANSPARENT);
		// �y�[�W�������̈��
		CString	cStrFooter;
		cStrFooter.Format( "- %d -", m_nPage);
		m_nPage++;
		DrawText( cStrFooter, m_cRctPage, DT_RIGHT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
		// ���t�̈��
		SYSTEMTIME	stSysTime;
		GetLocalTime( &stSysTime);
		COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
		cStrFooter = cTime.Format( LOCALE_NOUSEROVERRIDE, LANG_USER_DEFAULT);
		DrawText( cStrFooter, m_cRctPage, DT_LEFT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
	SetBkMode( nOldMode);

	// �g���
	CPen	cSPen( PS_SOLID, 0, RGB( 0, 0, 0));
	CPen*	pcPenOld;
	pcPenOld = SelectObject( &cSPen);
	Rectangle( m_cPaperRect);
	SelectObject( pcPenOld);

	// ���S�����
	MoveTo( 0, m_nCenterPos);
	LineTo( m_cPaperRect.left, m_nCenterPos);

	// ����J�n�\�ʒu�̍X�V
	m_nCurrentPos = m_cPaperRect.top;

	return TRUE;
}

BOOL CPrinterDC::PrintChip( const CMemoData* pcMemoData)
{
	if( !m_blAttach)return FALSE;
	if( NULL == pcMemoData)return FALSE;

	CString	cStrTitle;
	CString	cStrMemo;
	COLORREF	clrFore;
	COLORREF	clrBack;

	pcMemoData->GetTitle( cStrTitle);
	if( pcMemoData->IsPassWordLock())
	{
		cStrMemo.LoadString( IDS_DONOTVIEW);
	}
	else
	{
		UINT unDxlID = pcMemoData->GetDxlID();
		if( 0 != unDxlID)
		{
			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);

			int nIndex = pcSOboe->FindDxlID( unDxlID);
			if( 0 <= nIndex)
			{
				CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
				if( pcDrawExLib)
				{
					EDITDATA	stEditData;
					stEditData.m_nSelStart = 0;
					stEditData.m_nSelEnd = 0;
					stEditData.m_hEdit = NULL;
					stEditData.m_nSize = 0;
					stEditData.m_hSaveData = NULL;

					CString	cStrWork;
					cStrWork = pcMemoData->GetMemo();
					if( !cStrWork.IsEmpty())
					{
						stEditData.m_hEdit = GlobalAlloc( GHND, cStrWork.GetLength() + 1);
						LPSTR lpsz = ( LPSTR)GlobalLock( stEditData.m_hEdit);
						lstrcpy( lpsz, cStrWork);
						GlobalUnlock( stEditData.m_hEdit);
					}
					if( pcDrawExLib->IsSupportNativeData())
					{
						stEditData.m_nSize = pcMemoData->GetDxlExtData();
						if( stEditData.m_nSize)
						{
							stEditData.m_hSaveData = GlobalAlloc( GHND, stEditData.m_nSize);
							LPBYTE lpbyData = ( LPBYTE)GlobalLock( stEditData.m_hSaveData);
							pcMemoData->GetDxlExtData( lpbyData, stEditData.m_nSize);
							GlobalUnlock( stEditData.m_hSaveData);
						}
						else
						{
							stEditData.m_hSaveData = NULL;
						}
					}
					else
					{
						stEditData.m_nSize = 0;
						stEditData.m_hSaveData = NULL;
					}

					if( 0 < pcDrawExLib->ExchangeSendString( &stEditData))
					{
						LPSTR lpsz = ( LPSTR)GlobalLock( stEditData.m_hEdit);
						cStrMemo = lpsz;
						GlobalUnlock( stEditData.m_hEdit);
					}
					else
					{
						cStrMemo = pcMemoData->GetMemo();
					}

					GlobalFree( stEditData.m_hEdit);
					GlobalFree( stEditData.m_hSaveData);
				}
			}
		}
		if( cStrMemo.IsEmpty())
		{
			cStrMemo = pcMemoData->GetMemo();
		}
	}
	pcMemoData->GetForeColor( clrFore);
	pcMemoData->GetBackColor( clrBack);

	LOGFONT	stLogFont;
	pcMemoData->GetFont( &stLogFont);

	return PrintChip( cStrTitle, cStrMemo, clrFore, clrBack, stLogFont.lfFaceName);
}

BOOL CPrinterDC::PrintChip( const CString& cStrTitle, const CString& cStrMemo, COLORREF clrFore, COLORREF clrBack, const CString& cStrFontFace)
{
	if( !m_blAttach)return FALSE;
	if( cStrTitle.IsEmpty())return FALSE;
	if( cStrMemo.IsEmpty())return FALSE;

	CRect	cRctTitle;
	CRect	cRctMemo;

	CFont	cFont;
	CFont*	pcFontOld;

	cFont.CreatePointFont( 105, cStrFontFace, this);
	pcFontOld = SelectObject( &cFont);

	cRctTitle = m_cPaperRect;
	cRctTitle.left += GetSystemMetrics( SM_CXFRAME);
	DrawText( cStrTitle, cRctTitle, DT_LEFT | DT_CALCRECT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
	cRctMemo = m_cPaperRect;
	cRctMemo.left += GetSystemMetrics( SM_CXFRAME);
	DrawText( cStrMemo, cRctMemo, DT_LEFT | DT_CALCRECT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
	cRctMemo.Height();

	// �˂��A�V�����y�[�W�ɂ���H
	if( ( m_nCurrentPos + cRctMemo.bottom + ( GetSystemMetrics( SM_CYFRAME) * 2)) >= m_cPaperRect.bottom)
	{
		EndPage();
		StartPage();
		// �y�[�W����Ƃ����邩��A���̃t�H���g�ɖ߂��Ȃ���
		SelectObject( pcFontOld);
		PrintForm();
		pcFontOld = SelectObject( &cFont);
	}

	cRctTitle.bottom -= cRctTitle.top;
	cRctTitle.top = m_nCurrentPos + GetSystemMetrics( SM_CYFRAME);
	cRctTitle.bottom += cRctTitle.top + GetSystemMetrics( SM_CYFRAME);
	cRctTitle.left += GetSystemMetrics( SM_CXFRAME);
	cRctTitle.right = m_cPaperRect.right;

	cRctMemo.bottom -= cRctMemo.top;
	cRctMemo.top = cRctTitle.bottom + GetSystemMetrics( SM_CYFRAME);
	cRctMemo.bottom += cRctMemo.top + GetSystemMetrics( SM_CYFRAME);
	cRctMemo.bottom = min( cRctMemo.bottom, m_cPaperRect.bottom);
	cRctMemo.left += GetSystemMetrics( SM_CXFRAME);
	cRctMemo.right = m_cPaperRect.right;

	int nOldMode = SetBkMode( TRANSPARENT);
	COLORREF clrOldFore = GetTextColor();
	
	if( m_blColor)	// �����H�J���[�H
	{
		clrOldFore = SetTextColor( clrFore);

		CRect	cRect;
		cRect = m_cPaperRect;
		cRect.left++;
		cRect.right -= 2;
		cRect.top = m_nCurrentPos + 1;
		cRect.bottom = cRctMemo.bottom;

		FillSolidRect( cRect, clrBack);
	}
	
	CPen	cSPen( PS_SOLID, 0, RGB( 0, 0, 0));
	CPen	cDPen( PS_DOT, 0, RGB( 0, 0, 0));
	CPen*	pcPenOld;

	DrawText( cStrTitle, cRctTitle, DT_LEFT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
	pcPenOld = SelectObject( &cDPen);
	MoveTo( CPoint( m_cPaperRect.left, cRctTitle.bottom));
	LineTo( CPoint( m_cPaperRect.right - 1, cRctTitle.bottom));
	SelectObject( pcPenOld);

	DrawText( cStrMemo, cRctMemo, DT_LEFT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
	pcPenOld = SelectObject( &cSPen);
	MoveTo( CPoint( m_cPaperRect.left, cRctMemo.bottom));
	LineTo( CPoint( m_cPaperRect.right - 1, cRctMemo.bottom));
	SelectObject( pcPenOld);

	SetBkMode( nOldMode);
	SetTextColor( clrOldFore);
	SelectObject( pcFontOld);

	m_nCurrentPos = cRctMemo.bottom;

	return TRUE;
}

