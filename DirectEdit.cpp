// DirectEdit.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SOboe.h"
#include "DirectEdit.h"
#include ".\directedit.h"


// CDirectEdit

IMPLEMENT_DYNAMIC(CDirectEdit, CEdit)
CDirectEdit::CDirectEdit( const LOGFONT* pstLogFont, COLORREF clrFore, COLORREF clrBack) : m_clrFore( clrFore), m_clrBack( clrBack)
{
	m_cFont.CreateFontIndirect( pstLogFont);
	m_cBrushBack.CreateSolidBrush( m_clrBack);
}

CDirectEdit::~CDirectEdit()
{
}


BEGIN_MESSAGE_MAP(CDirectEdit, CEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_COMMAND(IDM_COPY, OnCopy)
END_MESSAGE_MAP()



// CDirectEdit メッセージ ハンドラ


void CDirectEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO : ここにメッセージ ハンドラ コードを追加します。
	this->PostMessage( WM_CLOSE);
}

BOOL CDirectEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( WM_KEYDOWN == pMsg->message)
	{
		switch( pMsg->wParam)
		{
		case VK_ESCAPE:
			if( 0 != ( GetKeyState( VK_SHIFT) & ~1))
			{
				this->PostMessage( WM_CLOSE);
			}
			break;
		case 'C':
			if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
			{
				this->PostMessage( WM_COMMAND, IDM_COPY);
			}
		default:
			break;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

BOOL CDirectEdit::Create( CWnd* pParentWnd, UINT nID)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CRect cRect;
	pParentWnd->GetClientRect( &cRect);

	cRect.top += GetSystemMetrics( SM_CYFRAME);

	return CEdit::Create( WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOIME | ES_READONLY, cRect, pParentWnd, nID);
}

int CDirectEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO :  ここに特定な作成コードを追加してください。
	this->SetFont( &m_cFont);
	this->SetMargins( GetSystemMetrics( SM_CXFRAME), 0);

	return 0;
}

void CDirectEdit::PostNcDestroy()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	delete this;

	CEdit::PostNcDestroy();
}

HBRUSH CDirectEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO :  ここで DC の属性を変更してください。
	pDC->SetBkColor( m_clrBack);
	pDC->SetTextColor( m_clrFore);

	// TODO :  親のハンドラを呼び出せない場合は NULL 以外のブラシを返します。
	return reinterpret_cast< HBRUSH>( m_cBrushBack.GetSafeHandle());
}

void CDirectEdit::OnCopy()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	this->Copy();
}
