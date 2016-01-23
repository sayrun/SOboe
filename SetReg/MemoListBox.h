#if !defined(AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_)
#define AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemoListBox.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CMemoListBox �E�B���h�E
#include "MemoChip.h"
#include "OleDrop.h"

#define	_MAX_TEMPLET	10

class CMemoListBox : public CListBox
{
// �R���X�g���N�V����
public:
	CMemoListBox();

	void StartDD( void);
	void EndDD( void);

	BOOL OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );
	DROPEFFECT OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	DROPEFFECT OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	int AddMemoChip( const CMemoChip* pcMemoChip);
	int InsertMemoChip( int nIndex, const CMemoChip* pcMemoChip);
	const CMemoChip* GetMemoChip( int nIndex);
	int DeleteMemoChip( int nIndex);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMemoListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMemoListBox();

	DECLARE_DYNAMIC( CMemoListBox);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	COleDrop	m_cOleDrop;			// OLE D&D�p

	//{{AFX_MSG(CMemoListBox)
	afx_msg void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_)
