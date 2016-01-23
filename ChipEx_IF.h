#ifndef	__OBOE_EX_IF_H__
#define	__OBOE_EX_IF_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// �\���̂̃o�[�W����
#define	_IF_DATA_VER			0x00000100

// �ύX�ʒm�t���O
#define	_MODIFY_MEMO				0x00000001	// �\�����e�^�^�C�g���̕ύX
#define	_MODIFY_COLOR				0x00000002	// �F�ύX
#define	_MODIFY_FONT				0x00000004	// �t�H���g�ύX
#define	_MODIFY_SHELL				0x00000008	// �֘A�t���ύX
#define	_MODIFY_TIMER				0x00000010	// �^�C�}�ύX
#define	_MODIFY_ICON				0x00000020	// �A�C�R���ύX
#define	_MODIFY_PASSWORD			0x00000040	// �p�X���[�h�ύX
#define	_MODIFY_ZODER				0x00000080	// �y�I�[�_�[�ύX
#define	_MODIFY_RECTCHIP			0x00000100	// ���ЃT�C�Y�ύX
#define	_MODIFY_POINTICON			0x00000200	// �A�C�R���ʒu�ύX
#define	_MODIFY_AUTORESIZE			0x00000400	// ���Ѓ��T�C�Y�v��

// �y�I�[�_�[
#define	_ZODER_NORMAL				0			// �ʏ�\��
#define	_ZODER_TOPMOST				1			// ��Ƀg�b�v
#define	_ZODER_BOTTOMMOST			2			// ��Ƀ{�g��

// �ʒm���̃V�t�g
#define	_NOTIFY_NOSHIFT				0		// �ړ�����
#define	_NOTIFY_SAT_FROM_SUN		1		// �y�j���ֈړ��F���j��
#define	_NOTIFY_FRI_FROM_SAT_SUN	2		// ���j���ֈړ��F�y�^���j��
#define	_NOTIFY_MON_FROM_SUN		3		// ���j���ֈړ��F���j��
#define	_NOTIFY_MON_FROM_SAT_SUN	4		// ���j���ֈړ��F�y�^���j��

// �j����ʃ^�C�}�̗j���w��
#define	_WEEK_SUNDAY				0x00000001	// ���j��
#define	_WEEK_MONDAY				0x00000002	// ���j��
#define	_WEEK_TUESDAY				0x00000004	// �Ηj��
#define	_WEEK_WEDNESDAY				0x00000008	// ���j��
#define	_WEEK_THURSDAY				0x00000010	// �ؗj��
#define	_WEEK_FRIDAY				0x00000020	// ���j��
#define	_WEEK_SATURDAY				0x00000040	// �y�j��

// �^�C�}�^�C�v
#define	_TIMERTYPE_DAILY			0			// ����
#define	_TIMERTYPE_WEEKLY			1			// �j��
#define	_TIMERTYPE_MONTHLY			2			// ����

#define	_ICON_COUNT					14			//�A�C�R�����

typedef struct tagIFDATA
{
	int				m_nSize;				// ���̍\���̂̃T�C�Y
	int				m_nVersion;				// ���̍\���̂̃o�[�W�����F_IF_DATA_VER���w��̎�

	UINT			m_unModify;				// DLL���ł̕ύX�t���O

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
	int				m_nTimerType;			// �^�C�}�^�C�v
	int				m_nMonth;				// �^�C�}�̌�
	int				m_nDay;					// �^�C�}�̓�
	int				m_nWeekDay;				// �j����ʃ^�C�}�̗j���w��
	int				m_nNotifyShift;			// �ʒm���̃V�t�g
	int				m_nBeforeNotify;		// �����O����̒ʒm
	BOOL			m_blExpireTimer;		// �o�ߌ�̃^�C�}�[
	HGLOBAL			m_hachszSoundFile;		// �T�E���h�t�@�C��

	int				m_nIconIndex;			// �A�C�R�����

	RECT			m_stRctChip;			// ���ЃT�C�Y
	POINT			m_stPntIcon;			// �A�C�R���ʒu
}IFDATA;

typedef struct tagSPECIFICATION
{
	UINT	unLowerStructVersion;	// �����\�ȍŌÃo�[�W����
	UINT	unUpperStructVersion;	// �����\�ȍŐV�o�[�W����
	char	szMenuString[ 50];		// �f�t�H���g�̃��j���[����
}SPECIFICATION;

#ifdef	__cplusplus
extern "C" 
{
#endif
// ���̊֐��͕K����������K�v���L��܂��B
__declspec( dllexport ) BOOL ChipExSpecification( SPECIFICATION* pSpecification);
// ���ڐݒ�p
__declspec( dllexport ) BOOL ChipExConfigure( UINT unStructVersion, HWND hWnd, UINT unMenuNum);
// Copyright�ύX�p
__declspec( dllexport ) BOOL ChipExAbout( UINT unStructVersion, HWND hWnd, UINT unMenuNum);
// ���s�֐�
__declspec( dllexport ) BOOL ChipExProc( HWND hWnd, UINT unMenuNum, IFDATA* pstIFData);
#ifdef	__cplusplus
}
#endif
#endif	//__OBOE_EX_IF_H__
