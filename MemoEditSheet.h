#if !defined(AFX_MEMOEDITSHEET_H__741DB80B_2743_11D2_90A4_00804C15184E__INCLUDED_)
#define AFX_MEMOEDITSHEET_H__741DB80B_2743_11D2_90A4_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemoEditSheet.h : �w�b�_�[ �t�@�C��
//

#include "BaseEditPage.h"
#include "ExtendEditPage.h"
#include "TimerEditPage.h"
#include "MemoData.h"

/////////////////////////////////////////////////////////////////////////////
// CMemoEditSheet

class CMemoEditSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMemoEditSheet)

// �R���X�g���N�V����
public:
	CMemoEditSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMemoEditSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMemoEditSheet)
	public:
	virtual int DoModal();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMemoEditSheet();

	// �ҏW���e�̐ݒ�Ǝ擾
	virtual void SetMemoData( const CMemoData& cMemoData);
	BOOL GetMemoData( CMemoData& cMemoData);
	// ���T�C�Y���̎擾
	BOOL IsResize( void) const;
	// �I�����̔j���t���O
	void DelOnQuit( BOOL blDelOnQuit);
	BOOL IsDelOnQuit( void) const;
	// �f�[�^�������e�X�g�F���e
	BOOL IsMemoEmpty( void) const;
	// Dxl-ID�]���p�̃��\�b�h
	virtual BOOL ForwardDxlID( UINT unDxlID);

	// ���T�C�Y�̋֎~
	void ResizeLock( BOOL blResizeLock);

	// �g�p���H
	static BOOL IsUsed();
	static HWND GetUseWnd();

protected:
	// �����o�[�ϐ�
	CBaseEditPage		m_cBasePage;
	CExtendEditPage		m_cExtPage;
	CTimerEditPage		m_cTimerPage;

	HWND				m_hWndParent;
	HICON m_hIcon;

	static CSemaphore	sm_cSmpDoEdit;
	static HWND			sm_hWndParent;

	// �����o�[�֐�

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMemoEditSheet)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "MemoEditSheet.inl"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MEMOEDITSHEET_H__741DB80B_2743_11D2_90A4_00804C15184E__INCLUDED_)
