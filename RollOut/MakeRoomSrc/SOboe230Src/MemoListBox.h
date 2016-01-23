#if !defined(AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_)
#define AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemoListBox.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CMemoListBox �E�B���h�E
#include "MemoListNode.h"

class CMemoListBox : public CListBox
{
// �R���X�g���N�V����
public:
	CMemoListBox();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	int AddMemoChip( const CMemoChip& cMemoChip, BOOL blDelData = FALSE);
	int InsertMemoChip( int nIndex, const CMemoChip& cMemoChip, BOOL blDelData = FALSE);
	int GetPreviewData( int nIndex, CString& cStrMemo, COLORREF& clrFore, COLORREF& clrBack);
	int GetPrintData( int nIndex, CMemoData& cMemoData);
	HWND GetWndHandle( int nIndex);

	BOOL IsLink( int nIndex);
	BOOL IsSeeThrough( int nIndex);
	BOOL IsDelOnQuit( int nIndex);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMemoListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMemoListBox();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMemoListBox)
	afx_msg void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_)
