// MemoChip.h : �w�b�_�[ �t�@�C��
//
#ifndef	__MEMOCHIP_H__
#define	__MEMOCHIP_H__

/////////////////////////////////////////////////////////////////////////////
// CMemoChip �E�B���h�E

#define	MOVEAREAPOINT_MAX	4
#define	WM_BYEBYE			( WM_USER + 100)
#define	MAX_FILESIZE		( 1024 * 1024)
#define	MAX_ICON_INDEX		14

#define	_ICON_WIDTH			16
#define	_ICON_HEIGHT		16

#define	_BRW_IF_DATA_OLD_VER		0x00000200
#define	_BRW_IF_DATA_300_VER		0x00000300
#define	_BRW_IF_DATA_CUR_VER		0x00000400

#pragma pack( push, 1)
typedef struct tagIFDATA
{
	int				m_nSize;				// ���̍\���̂̃T�C�Y
	int				m_nVersion;				// ���̍\���̂̃o�[�W�����F_IF_DATA_VER���w��̎�

	char			m_achszMemoTitle[64];	// ���Ђ̃^�C�g��
	HGLOBAL			m_hachszMemoData;		// ���Ђ̓��e
	COLORREF		m_clrTextColor;			// �e�L�X�g�F
	COLORREF		m_clrBackColor;			// �w�i�F
	LOGFONT			m_stLogFont;			// �t�H���g�X�^�C��

	int				m_nZOder;				// �y�I�[�_�[

	HGLOBAL			m_hachszPassWord;		// �p�X���[�h

	BOOL			m_blShellExec;			// �֘A�t���g�p�̗L��
	BOOL			m_blTimerExec;			// �֘A�t���̃^�C�}�ł̋N��
	HGLOBAL			m_hachszFileName;		// �֘A�t���̃t�@�C������
	HGLOBAL			m_hachszProgName;		// �֘A�t���̃v���O��������

	BOOL			m_blTimerEnable;		// �^�C�}�g�p�̗L��
	int				m_nHour;				// �^�C�}�̎�
	int				m_nMin;					// �^�C�}�̕�
	int				m_nTimerType;			// �^�C�}�^�C�v 0/1/2 = ����/�j��/����
	int				m_nMonth;				// �^�C�}�̌�
	int				m_nDay;					// �^�C�}�̓�
	int				m_nWeekDay;				// �j����ʃ^�C�}�̗j���w�� 0bit=���j�� 1bit���j��...
	int				m_nNotifyShift;			// �ʒm�������j���������ꍇ�̃V�t�g
	int				m_nBeforeNotify;		// �����O����̒ʒm
	BOOL			m_blExpireTimer;		// �o�ߌ�̃^�C�}�[
	HGLOBAL			m_hachszSoundFile;		// �T�E���h�t�@�C��

	BOOL			m_blMinSize;			// ���Ђ̕\����� �O�ʏ�^�O�ȊO�A�C�R��
	BOOL			m_blShow;				// �\�����
	int				m_nIconIndex;			// �A�C�R�����
	RECT			m_stRctChip;			// ���ЃT�C�Y
	POINT			m_stPntIcon;			// �A�C�R���ʒu
	BOOL			m_blChipPosLock;		// �ʒu�T�C�Y�Œ�

	int				m_nDeleteYear;			// �폜�̔N
	int				m_nDeleteMonth;			// �폜�̌�
	int				m_nDeleteDay;			// �폜�̓�

	BOOL			m_blSeeTrough;			// �V�[�X���[�t���O:1.60
	UINT			m_unDXLID;				// Dxl�̃��j�[�N��ID:1.60
	int				m_nDxlSaveDataSize;		// dxl�̌ŗL�f�[�^�̃T�C�Y:1.60
	HGLOBAL			m_hDxlSaveData;			// dxl�̌ŗL�f�[�^:1.60
}IFDATA;
#pragma pack( pop)


#include "MemoData.h"

class CMemoChip : public CWnd
{
// �R���X�g���N�V����
public:
	const BOOL IsPassWordLock( LPCSTR lpcszPassWord = NULL);
	BOOL GetMemoData( CMemoData& cMemoData);
	void SetMemoData( CMemoData& cMemoData);
	~CMemoChip();
	CMemoChip();
	CMemoChip( CMemoChip& cMemoChip);
	// �ϐ�

	void Serialize( CArchive& ar );

	int GetIFData( IFDATA* pstIFData, LPCSTR lpcszPassWord);
	BOOL SetIFData( IFDATA* pstIFData, LPCSTR lpcszPassWord);
	static BOOL ReleaseIFData( IFDATA* pstIFData);


	DECLARE_SERIAL( CMemoChip);
// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CMemoChip)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	BOOL IsLink( void) const;
	BOOL GetFont( LOGFONT* pLogFont);
	int GetIconIndex( void) const;
	COLORREF GetBackColor( void) const;
	COLORREF GetTextColor( void) const;
	int GetMemo( LPSTR lpszData, int nSize, LPCSTR lpcszPassWord);
	BOOL GetMemoTitle( LPSTR lpszTitle);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	/*
	BOOL GetIFData( IFDATA& stIFData);
	BOOL ReleaseIFData( IFDATA& stIFData);
	*/


	BOOL	m_blMove;
	COLORREF m_clrInternalTextColor;
	COLORREF m_clrInternalBackColor;
	CString m_cStrResult2;
	CString m_cStrResult1;
	BOOL m_blCheckOpen;			// �J���m�F:1.40
	BOOL m_blCheckYesNo;		// �͂��^�������m�F:1.40
	BOOL m_blOtherResult;		// ���̑��̉�:1.42
	CString m_cStrSignature;	// �����
	CString m_cStrComp;			// ���茳
	CString m_cStrUser;			// ���茳
	CString m_cStrCaption;		// 
	CString m_cStrPeerAddres;	// ���茳�A�h���X
	BOOL m_blCheckedReceive;

	BOOL m_blSysResize;
	BOOL m_blBeforeHide;
	BOOL m_blChipPosLock;
	BOOL m_blOndeHide;
	BOOL m_blFrameDraw;
	BOOL m_blSizeBackUp;
	CString m_cStrUndo;
	BOOL m_blUndo;
	BOOL m_blShow;
	CWnd* m_pcWndParent;
	CToolTipCtrl m_cTtcTipHelp;
//	CMemochipTip m_cTtcTipHelp;
	BOOL m_blFlash;
	int m_nTodayTimer;
	BOOL m_blClose;
	CPoint m_cPntStart;
	BOOL m_blMinSize;
	POINT m_astMoveRgnPoints[ MOVEAREAPOINT_MAX];
	BOOL m_blDataAppry;
	CMemoData m_cMemoData;

	//{{AFX_MSG(CMemoChip)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif	//__MEMOCHIP_H__
