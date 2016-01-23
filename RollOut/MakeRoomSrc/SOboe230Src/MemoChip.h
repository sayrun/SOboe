#if !defined(AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_)
#define AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemoChip.h : �w�b�_�[ �t�@�C��
//

#include "MemoData.h"
#include "GroupEntry.h"
#include "DxlControl.h"
#include "OleDrop.h"
#include "NetExLib.h"

/////////////////////////////////////////////////////////////////////////////
// CMemoChip �E�B���h�E

#define	MOVEAREAPOINT_MAX	4
#define	TOOLTIPSTR_MAX		512

#ifndef	WS_EX_LAYERED
/* Windows 2000�ŃT�|�[�g�����AlphaBlend�̎���
*/
#define	WS_EX_LAYERED	0x00080000L
#define	LWA_COLORKEY	1
#define	LWA_ALPHA		2
#endif

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
	BOOL ShowWindow( int nCmdShow);

	BOOL OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );
	DROPEFFECT OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	DROPEFFECT OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CMemoChip)
	public:
	virtual BOOL Create( UINT unID, HWND hParentWnd, BOOL blHide, HICON hIcon, CToolTipCtrl* pcToolTip) ;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMemoChip();

	// �f�[�^�ւ̃A�N�Z�X
	void SetMemoData( const CMemoData& cMemoData);
	BOOL GetMemoData( CMemoData& cMemoData) const;
	const CMemoData* AccessMemoData( void) const;
	UINT GetID( void) const;

	// �T�C�Y�v�Z
	BOOL CalcDefaultRect( BOOL blCentering = FALSE);
	BOOL CalcTitleRect( CRect& cRectNew);

	void ActiveZOrder( void);

	BOOL DelOnQuit( BOOL blDelOnQuit);
	BOOL IsDelOnQuit( void) const;
	BOOL DrawResizeFrame( BOOL blResize);

	// ���t�ύX�ɂ��^�C�}����
	BOOL CheckTimerOnDayChange( const COleDateTime & cTime);
	void CheckTimer( const COleDateTime& cTime);
	void DxlCheckTimer( const COleDateTime& cTime);

	// OLE�ɂ��D&D
	BOOL OleDrop( LPCSTR lpcszData, const POINT& stPoint, BOOL blLink);

	// ���Ђ̏I��
	void SmartDelete();

	// �ꎞ��\��
	void OnceHide( void);
	void RestoreOnceHide( void);

	// �����ł̊Ǘ��F
	const COLORREF GetInternalForeColor( void) const;
	const COLORREF GetInternalBackColor( void) const;

	BOOL SetEntryData( const CEntryData* pcEntryData);
	const CEntryData* GetEntryData( void) const;
	BOOL SetResults( const INNERRESULTS& stResults);

	BOOL IsPack( void) const;
	void SetResultID( const CString& cStrResultID);
	void Pack( BOOL blNeedPassWord);
	void CheckOpen( BOOL blCheckOpen);
	BOOL ReceiveNotify( void);
	void AddResult( const CString& cStrResult);

	void Serialize( CArchive& ar );

	BOOL ArrangeIcon( const POINT& pstPoint);
	BOOL ArrangeTitle( const RECT& stRect);
	BOOL ArrangeChip( const RECT& stRect);

	void SetContextID( LPCSTR lpcszID);
	BOOL IsEqualContextID( LPCSTR lpcszID);

	void SetReceiveTime( const COleDateTime& cReceiveTime);
	
	BOOL NoCheckReceive( void) const;

	void OneTimeTop( void);

	DECLARE_SERIAL( CMemoChip);
	// �v���C�x�[�g�ȃ��\�b�h
protected:
	// �p�X���[�h�̃`�F�b�N
	BOOL CheckPassWord( void);

	// ���T�C�Y�G���A�̕`��
	void DrawResizeArea( CDC* pcDC, BOOL blActive);

	// ���T�C�Y���̃A�j���[�V����
	void DoResizeFrameAnime( const CRect& cRectTo, const CRect& cRectFrom);

	// �őO��ւ̈ړ��ƃu�����N�ʒm
	BOOL StartPopupNotify( void);
	BOOL EndPopupNotify( void);
	// �u�����N
	BOOL DoBlink( void);

	// �O����̒ʒm����
	BOOL CheckExtensiveNotify( const COleDateTime& cTime);

	// �}���\��t��
	BOOL PasteMemo( const CString& cStrData);
	// �㏑���\��t��
	BOOL OverWriteMemo( const CString& cStrData, BOOL blNoTriming = FALSE);
	// �\��t���p���쐬
	BOOL CreateMemo( const CString& cStrData);
	// �֘A�t���\��t��
	BOOL PasteLink( const CString& cStrData);

	// �N���b�v�{�[�h����̕����擾
	BOOL GetClipboradString( CString& cStr);
	// �N���b�v�{�[�h����̃t�@�C�������N�擾
	BOOL GetClipboradLink( CString& cStr);

	// OLE D&D
	DWORD DoDragDrop( BOOL blMoveEnable = FALSE);

	// �V�[�X���[�p�̃��[�W�����쐬
	CRgn* CreateSeeThroughRgn( CDC* pcDC, const CRect& cRctClient);

	// �c�[���`�b�v
	void CreateToolTip( void);
	void ModifyToolTip( LPCSTR lpcsz = NULL);
	void DestroyToolTip( void);

	// �_�u���N���b�N�̏���
	BOOL OnDblClk( UINT nFlags, int nFunc);

	// ���Ԃ����J��
	BOOL OpenPackChip( void);

	// Window�̈��������Ƃ���ɂ�����
	const CRect ClipRectToMonitor( const CRect& cRctSrc);

	int EditChip( CMemoData& cMemoData, BOOL& blDelOnQuit, BOOL &blResize);
	int ExEditChip( CMemoData& cMemoData, BOOL& blDelOnQuit, BOOL &blResize);

	// ���C�A�����̓Ǝ���������
	BOOL SetLayeredWindowAttributes( COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	BOOL IsSupportAlphaBlend( void);

	// �����o�[�ϐ�
protected:
	static CString		m_cStrClassName;

	CMemoData	m_cMemoData;

	HWND		m_hParentWnd;	// �e��Window

	POINT		m_astMoveRgnPoints[ MOVEAREAPOINT_MAX];		// ���T�C�Y�G���A�̃|�C���g

	bool		m_blNowMove;	// �ړ����t���b�O
	bool		m_blNowZMove;	// �y�ړ��t���b�O
	bool		m_blMouseClickStart;	// �ړ��̂��߂̃N���b�N�J�n
	CPoint		m_cPntStart;	// �ړ��J�n�̃N���b�N�ʒu

	CDxlControl*	m_pcDxlControl;

	bool		m_blBlink;	// �u�����N

	bool		m_blDelOnQuit;	// �I�����̍폜�w��

	COLORREF	m_clrInternalForeColor;	// ���������p�̕����F
	COLORREF	m_clrInternalBackColor;	// ���������p�̔w�i�F

	bool		m_blBeforeVisible;	// �ʒm�ɂ��\���̑O���
	int			m_nNotifyDay;		// �ʒm�̓��ɂ�

	bool		m_blFrameDraw;		// ���T�C�Y���̃t���[���`��

	UINT		m_unID;				// �\��t�����Ă���Ԃ̂ݗL���ȊǗ�ID

	COleDrop	m_cOleDrop;			// OLE D&D�p

	bool		m_blSmartClose;		// �폜����

	BOOL		m_blShowBeforeOndeHide; // �ꎞ�\��̑O���

	CRgn*		m_pcRgnSeeThrough;

	bool		m_blOnceTop;

	CToolTipCtrl*	m_pcToolTip;

	CString		m_cStrUndo;

	static UINT	m_unURL;

	CEntryData* m_pcEntryData;

	BOOL		m_blPack;
	BOOL		m_blCheckOpen;
	BOOL		m_blNeedPassWord;
	CString*		m_pcStrResultID;
	INNERRESULTS*	m_pstResults;

	BOOL		m_blReceive;

	COleDateTime*	m_pcReceiveTime;

	BOOL		m_blSysResize;

	typedef BOOL (PASCAL FAR *LPSETLAYEREDWINDOWATTRIBUTES)( HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	static LPSETLAYEREDWINDOWATTRIBUTES	m_lpfnSetLayeredWindowAttributes;
	static LPSETLAYEREDWINDOWATTRIBUTES GetSetLayeredWindowAttributesProcAddress( void);

	DWORD m_dwTime;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMemoChip)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnToIcon();
	afx_msg void OnToNormal();
	afx_msg void OnToTitle();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnReedit();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPasteAppend();
	afx_msg void OnPasteOverwrite();
	afx_msg void OnPasteCreate();
	afx_msg void OnPasteLink();
	afx_msg void OnDelete();
	afx_msg void OnNewmemo();
	afx_msg void OnCopy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLinkexec();
	afx_msg void OnDelonquit();
	afx_msg void OnMemoexport();
	afx_msg void OnTextsave();
	afx_msg void OnCopyMemodata();
	afx_msg void OnPoslock();
	afx_msg void OnSeetrough();
	afx_msg void OnHide();
	afx_msg void OnUndo();
	afx_msg void OnSend();
	afx_msg void OnSender();
	afx_msg void OnReply();
	//}}AFX_MSG
	afx_msg void OnCelMenu( UINT nID);
	afx_msg void OnDxlMenu( UINT nID);
	afx_msg void OnAlphaBlend( UINT nID);
	DECLARE_MESSAGE_MAP()

friend class COleDrag;
};

LPCTSTR AFXAPI RegisterMemoChipClass(UINT nClassStyle);
void ResultSendThread( void* lpvData);

#include "MemoChip.inl"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_)
