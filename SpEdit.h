#if !defined(AFX_SPEDIT_H__EA817CF3_09DE_11D3_92FF_004005469C82__INCLUDED_)
#define AFX_SPEDIT_H__EA817CF3_09DE_11D3_92FF_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpEdit.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CSpEdit �E�B���h�E

class CSpEdit : public CEdit
{
// �R���X�g���N�V����
public:
	CSpEdit();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSpEdit)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CSpEdit();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	int	m_nSelStart;
	int	m_nSelEnd;
	//{{AFX_MSG(CSpEdit)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SPEDIT_H__EA817CF3_09DE_11D3_92FF_004005469C82__INCLUDED_)
