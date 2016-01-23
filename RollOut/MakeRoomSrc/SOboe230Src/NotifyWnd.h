#if !defined(AFX_NOTIFYWND_H__B636C504_5E6B_11D2_A0FF_00C04FCA10B7__INCLUDED_)
#define AFX_NOTIFYWND_H__B636C504_5E6B_11D2_A0FF_00C04FCA10B7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NotifyWnd.h : �w�b�_�[ �t�@�C��
//

class CMemoChip;

/////////////////////////////////////////////////////////////////////////////
// CNotifyWnd �E�B���h�E

class CNotifyWnd : public CWnd
{
// �R���X�g���N�V����
public:
	CNotifyWnd();

// �A�g���r���[�g
public:
	static UINT m_sunKillMsg;

// �I�y���[�V����
public:
	BOOL Create( LPCTSTR lpszWindowName, CMemoChip* pParentWnd);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CNotifyWnd)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CNotifyWnd();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	int			m_nCount;
	HWND		m_hParentWnd;
	CMemoChip*	m_pcParentWnd;

	//{{AFX_MSG(CNotifyWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNcDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LONG OnKillMsg( UINT, LONG );
	DECLARE_MESSAGE_MAP()
};

class CRenotifyWnd : public CNotifyWnd
{
// �R���X�g���N�V����
public:
	CRenotifyWnd();

// �I�y���[�V����
public:
	BOOL Create( LPCTSTR lpszNotifyMsg, CWnd* pcParent);

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CRenotifyWnd)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static UINT sm_unMsgFromTray;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_NOTIFYWND_H__B636C504_5E6B_11D2_A0FF_00C04FCA10B7__INCLUDED_)
