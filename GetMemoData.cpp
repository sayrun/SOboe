#include "stdafx.h"
#include "SOboe.h"
#include "DrawEx_IF.h"
#include "GetMemoData.h"
#include "MemoChip.h"
#include "MemoData.h"

int GetMemo( LPVOID lp, LPVOID lpData, int nDataSize)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();

	if( 0 == nDataSize || NULL == lpData)
	{
		return pcMemoData->GetMemoLength();
	}
	else
	{
		int nLength = min( nDataSize, pcMemoData->GetMemoLength() + 1);
		lstrcpyn( ( char*)lpData, pcMemoData->GetMemo(), nLength);
		return lstrlen( ( char*)lpData);
	}
}

int GetTitle( LPVOID lp, LPVOID lpData, int nDataSize)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	CString	cStr;

	pcMemoData->GetTitle( cStr);
	if( 0 == nDataSize || NULL == lpData)
	{
		return cStr.GetLength();
	}
	else
	{
		int nLength = min( nDataSize, cStr.GetLength() + 1);
		lstrcpyn( ( char*)lpData, cStr, nLength);
		return lstrlen( ( char*)lpData);
	}
}

int GetFont( LPVOID lp, LPVOID lpData, int nDataSize)
{
	if( sizeof( LOGFONT) <= nDataSize && NULL != lpData)
	{
		CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
		const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
		pcMemoData->GetFont( ( LPLOGFONT)lpData);
	}

	return sizeof( LOGFONT);
}

int GetForeColor( LPVOID lp, LPVOID lpData, int nDataSize)
{
	if( sizeof( COLORREF) <= nDataSize && NULL != lpData)
	{
		CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
		*( COLORREF*)lpData = pcMemoChip->GetInternalForeColor();
	}

	return sizeof( COLORREF);
}

int GetBackColor( LPVOID lp, LPVOID lpData, int nDataSize)
{
	if( sizeof( COLORREF) <= nDataSize && NULL != lpData)
	{
		CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
		*( COLORREF*)lpData = pcMemoChip->GetInternalBackColor();
	}

	return sizeof( COLORREF);
}

int GetIconIndex( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	return pcMemoData->GetIconIndex();
}

int GetZOrder( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	return pcMemoData->GetZOrder();
}

int GetWindowType( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	return pcMemoData->GetWindowType();
}

int GetNormalRect( LPVOID lp, LPVOID lpData, int nDataSize)
{
	if( sizeof( RECT) <= nDataSize && NULL != lpData)
	{
		CMemoChip*			pcMemoChip;
		const CMemoData*	pcMemoData;
		CRect				cRect;

		pcMemoChip = ( CMemoChip*)lp;
		pcMemoData = pcMemoChip->AccessMemoData();
		cRect = pcMemoData->GetNormalRect();

		RECT*	pstRect = ( RECT*)lpData;
		pstRect->left	= cRect.left;
		pstRect->top	= cRect.top;
		pstRect->right	= cRect.right;
		pstRect->bottom	= cRect.bottom;
	}

	return sizeof( RECT);
}

int GetTitleRect( LPVOID lp, LPVOID lpData, int nDataSize)
{
	if( sizeof( RECT) <= nDataSize && NULL != lpData)
	{
		CMemoChip*			pcMemoChip;
		const CMemoData*	pcMemoData;
		CRect				cRect;

		pcMemoChip = ( CMemoChip*)lp;
		pcMemoData = pcMemoChip->AccessMemoData();
		cRect = pcMemoData->GetTitleRect();

		RECT*	pstRect = ( RECT*)lpData;
		pstRect->left	= cRect.left;
		pstRect->top	= cRect.top;
		pstRect->right	= cRect.right;
		pstRect->bottom	= cRect.bottom;
	}

	return sizeof( RECT);
}

int GetIconRect( LPVOID lp, LPVOID lpData, int nDataSize)
{
	if( sizeof( RECT) <= nDataSize && NULL != lpData)
	{
		CMemoChip*			pcMemoChip;
		const CMemoData*	pcMemoData;
		CRect				cRect;

		pcMemoChip = ( CMemoChip*)lp;
		pcMemoData = pcMemoChip->AccessMemoData();
		cRect = pcMemoData->GetTitleRect();

		RECT*	pstRect = ( RECT*)lpData;
		pstRect->left	= cRect.left;
		pstRect->top	= cRect.top;
		pstRect->right	= cRect.right;
		pstRect->bottom	= cRect.bottom;
	}

	return sizeof( RECT);
}

int GetTitlePoint( LPVOID lp, LPVOID lpData, int nDataSize)
{
	if( sizeof( POINT) <= nDataSize && NULL != lpData)
	{
		CMemoChip*			pcMemoChip;
		const CMemoData*	pcMemoData;
		CRect				cRect;

		pcMemoChip = ( CMemoChip*)lp;
		pcMemoData = pcMemoChip->AccessMemoData();
		cRect = pcMemoData->GetTitleRect();

		POINT*	pstPoint = ( POINT*)lpData;
		pstPoint->x	= cRect.left;
		pstPoint->y	= cRect.top;
	}

	return sizeof( POINT);
}

int GetIconPoint( LPVOID lp, LPVOID lpData, int nDataSize)
{
	if( sizeof( POINT) <= nDataSize && NULL != lpData)
	{
		CMemoChip*			pcMemoChip;
		const CMemoData*	pcMemoData;
		CRect				cRect;

		pcMemoChip = ( CMemoChip*)lp;
		pcMemoData = pcMemoChip->AccessMemoData();
		cRect = pcMemoData->GetTitleRect();

		POINT*	pstPoint = ( POINT*)lpData;
		pstPoint->x	= cRect.left;
		pstPoint->y	= cRect.top;
	}

	return sizeof( POINT);
}

int IsTimerEnable( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	return pcMemoData->IsTimer();
}

int GetTimerType( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	return pcMemoData->GetTimerType();
}

int GetTimerMonth( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	return pcMemoData->GetTimerMonth();
}

int GetTimerDay( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	return pcMemoData->GetTimerDay();
}

int GetTimerHour( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	return pcMemoData->GetTimerHour();
}

int GetTimerMin( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	return pcMemoData->GetTimerMin();
}

int GetTimerWeek( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	return pcMemoData->GetWeek();
}

int GetTimerSound( LPVOID lp, LPVOID lpData, int nDataSize)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	CString	cStr;

	pcMemoData->GetNotifySound( cStr);
	if( 0 == nDataSize || NULL == lpData)
	{
		return cStr.GetLength();
	}
	else
	{
		int nLength = min( nDataSize, cStr.GetLength() + 1);
		lstrcpyn( ( char*)lpData, cStr, nLength);
		return lstrlen( ( char*)lpData);
	}
}

int IsLink( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	return pcMemoData->IsLink();
}

int GetLinkFile( LPVOID lp, LPVOID lpData, int nDataSize)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	CString	cStr;
	CString	cStrWork;
	
	pcMemoData->GetLinkData( cStr, cStrWork);
	if( 0 == nDataSize || NULL == lpData)
	{
		return cStr.GetLength();
	}
	else
	{
		int nLength = min( nDataSize, cStr.GetLength() + 1);
		lstrcpyn( ( char*)lpData, cStr, nLength);
		return lstrlen( ( char*)lpData);
	}
}

int GetLinkParam( LPVOID lp, LPVOID lpData, int nDataSize)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	CString	cStr;
	CString	cStrWork;
	
	pcMemoData->GetLinkData( cStrWork, cStr);
	if( 0 == nDataSize || NULL == lpData)
	{
		return cStr.GetLength();
	}
	else
	{
		int nLength = min( nDataSize, cStr.GetLength() + 1);
		lstrcpyn( ( char*)lpData, cStr, nLength);
		return lstrlen( ( char*)lpData);
	}
}

int GetCreateYear( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	int nYear;
	int nMonth;
	int nDay;
	pcMemoData->GetCreateDate( nYear, nMonth, nDay);
	return nYear;
}

int GetCreateMonth( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	int nYear;
	int nMonth;
	int nDay;
	pcMemoData->GetCreateDate( nYear, nMonth, nDay);
	return nMonth;
}

int GetCreateDay( LPVOID lp)
{
	CMemoChip*	pcMemoChip = ( CMemoChip*)lp;
	const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
	int nYear;
	int nMonth;
	int nDay;
	pcMemoData->GetCreateDate( nYear, nMonth, nDay);
	return nDay;
}

int GetDefFont( LPVOID lpData, int nDataSize)
{
	if( sizeof( LOGFONT) < nDataSize && NULL != lpData)
	{
		CSOboeApp* pcSOboe = ( CSOboeApp*)AfxGetApp();
		pcSOboe->GetDefaultFont( ( LPLOGFONT)lpData);
	}

	return sizeof( LOGFONT);
}

int GetDefForeColor( LPVOID lpData, int nDataSize)
{
	if( sizeof( COLORREF) <= nDataSize && NULL != lpData)
	{
		CSOboeApp* pcSOboe = ( CSOboeApp*)AfxGetApp();
		*( COLORREF*)lpData = pcSOboe->GetDefaultForeColor();
	}

	return sizeof( COLORREF);
}

int GetDefBackColor( LPVOID lpData, int nDataSize)
{
	if( sizeof( COLORREF) < nDataSize && NULL != lpData)
	{
		CSOboeApp* pcSOboe = ( CSOboeApp*)AfxGetApp();
		*( COLORREF*)lpData = pcSOboe->GetDefaultBackColor();
	}

	return sizeof( COLORREF);
}

int CALLBACK GetMemoData( LPVOID lp, int nGetType, LPVOID lpData, int nDataSize)
{
	switch( nGetType)
	{
	case _GET_MEMO:
		return GetMemo( lp, lpData, nDataSize);
	case _GET_TITLE:
		return GetTitle( lp, lpData, nDataSize);
	case _GET_FONT:
		return GetFont( lp, lpData, nDataSize);
	case _GET_FORECOLOR:
		return GetForeColor( lp, lpData, nDataSize);
	case _GET_BACKCOLOR:
		return GetBackColor( lp, lpData, nDataSize);
	case _GET_ICON:
		return GetIconIndex( lp);
	case _GET_ZORDER:
		return GetZOrder( lp);
	case _GET_WINDOWTYPE:
		return GetWindowType( lp);
	case _GET_RECT_NORMAL:
		return GetNormalRect( lp, lpData, nDataSize);
	case _GET_POINT_TITLE:
		return GetTitlePoint( lp, lpData, nDataSize);
	case _GET_POINT_ICON:
		return GetIconPoint( lp, lpData, nDataSize);
	case _GET_RECT_TITLE:
		return GetTitleRect( lp, lpData, nDataSize);
	case _GET_RECT_ICON:
		return GetIconRect( lp, lpData, nDataSize);

	case _GET_TIMERENABLE:
		return IsTimerEnable( lp);
	case _GET_TIMER_TYPE:
		return GetTimerType( lp);
	case _GET_TIMER_MONTH:
		return GetTimerMonth( lp);
	case _GET_TIMER_DAY:
		return GetTimerDay( lp);
	case _GET_TIMER_HOUR:
		return GetTimerHour( lp);
	case _GET_TIMER_MIN:
		return GetTimerMin( lp);
	case _GET_TIMER_WEEK:
		return GetTimerWeek( lp);
	case _GET_TIMER_SOUND:
		return GetTimerSound( lp, lpData, nDataSize);

	case _GET_LINKENABLE:
		return IsLink( lp);
	case _GET_LINK_FILE:
		return GetLinkFile( lp, lpData, nDataSize);
	case _GET_LINK_PARAM:
		return GetLinkParam( lp, lpData, nDataSize);

	case _GET_CREATE_YEAR:
		return GetCreateYear( lp);
	case _GET_CREATE_MONTH:
		return GetCreateMonth( lp);
	case _GET_CREATE_DAY:
		return GetCreateDay( lp);

	case _GET_DEF_FONT:
		return GetDefFont( lpData, nDataSize);
	case _GET_DEF_FORECLR:
		return GetDefForeColor( lpData, nDataSize);
	case _GET_DEF_BACKCLR:
		return GetDefBackColor( lpData, nDataSize);

	case _GET_VERSION:
		return VERSION_CODE;
	}
	return 0;
}
