// SpEdit.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "soboe.h"
#include "SpEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpEdit

CSpEdit::CSpEdit()
{
	m_nSelStart = -1;
	m_nSelEnd = -1;
}

CSpEdit::~CSpEdit()
{
}


BEGIN_MESSAGE_MAP(CSpEdit, CEdit)
	//{{AFX_MSG_MAP(CSpEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpEdit ���b�Z�[�W �n���h��

void CSpEdit::OnSetFocus(CWnd* pOldWnd) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	if( -1 == m_nSelStart && -1 == m_nSelEnd)
	{
		m_nSelStart = GetWindowTextLength();
		m_nSelEnd = m_nSelStart;
	}
	SetSel( m_nSelStart, m_nSelEnd);
	PostMessage( EM_SCROLLCARET, 0, 0);

	CEdit::OnSetFocus( pOldWnd);
}

void CSpEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	GetSel( m_nSelStart, m_nSelEnd);
}

void CSpEdit::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CEdit::OnShowWindow(bShow, nStatus);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	if( bShow)
	{
		PostMessage( EM_SCROLLCARET, 0, 0);
	}
}
