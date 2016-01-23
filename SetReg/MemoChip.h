#if !defined(AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_)
#define AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemoChip.h : �w�b�_�[ �t�@�C��
//

#include "MemoData.h"

/////////////////////////////////////////////////////////////////////////////
// CMemoChip �E�B���h�E

#define	MOVEAREAPOINT_MAX	4

class CMemoChip : public CWnd
{
// �R���X�g���N�V����
public:
	CMemoChip();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	const CMemoChip& operator=( const CMemoChip& cMemoChip);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CMemoChip)
	public:
	protected:
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMemoChip();

	// �f�[�^�ւ̃A�N�Z�X
	void SetMemoData( const CMemoData& cMemoData);
	BOOL GetMemoData( CMemoData& cMemoData) const;
	const CMemoData* AccessMemoData( void) const;

	void Serialize( CArchive& ar );

	DECLARE_SERIAL( CMemoChip);
	// �v���C�x�[�g�ȃ��\�b�h
protected:
	CMemoData	m_cMemoData;
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMemoChip)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "MemoChip.inl"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_)
