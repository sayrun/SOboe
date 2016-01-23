#ifndef	__CELCONTROL_H__
#define	__CELCONTROL_H__


#define	_CEL_MAX				10

#define	_CEL_MENUTOP			11000
#define	_CEL_MENU0				11000
#define	_CEL_MENU1				11001
#define	_CEL_MENU2				11002
#define	_CEL_MENU3				11003
#define	_CEL_MENU4				11004
#define	_CEL_MENU5				11005
#define	_CEL_MENU6				11006
#define	_CEL_MENU7				11007
#define	_CEL_MENU8				11008
#define	_CEL_MENU9				11009
#define	_CEL_MENUBOTTOM			11009

#define	_CELCTL_ERROR			0x80000000
#define	_CELCTL_NOP				0x00000000
#define	_CELCTL_REDRAW			0x00000001
#define	_CELCTL_MOVE			0x00000002
#define	_CELCTL_RESIZE			0x00000004

class CMemoData;

class CCelControl
{
public:
	CCelControl( void);

	BOOL SetSelData( int nIndex, LPCSTR lpcszPath, LPCSTR lpcszMenu = NULL);
	BOOL SetMenuSeparator( LPCSTR lpcsz);

	HMENU GetMenu( void) const;

	static BOOL IsUsed();

	DWORD DoCel( HWND hWnd, UINT unMenuID, CMemoData& cMemoData) const;

protected:
	BOOL	m_blValid;
	CString	m_acStrMenu[ _CEL_MAX];
	CString	m_acStrPath[ _CEL_MAX];
	CString	m_cStrSeparator;

	static CSemaphore	sm_cSmpDoCel;

	typedef struct tagSPECIFICATION
	{
		UINT	unLowerStructVersion;	// �����\�ȍŌÃo�[�W����
		UINT	unUpperStructVersion;	// �����\�ȍŐV�o�[�W����
		char	szMenuString[ 50];		// �f�t�H���g�̃��j���[����
	}SPECIFICATION;


// �\���̂̃o�[�W����
#define	_IF_DATA_VER			0x00000100

// �ύX�t���O
#define	_MODIFY_MEMO			0x00000001
#define	_MODIFY_COLOR			0x00000002
#define	_MODIFY_FONT			0x00000004
#define	_MODIFY_SHELL			0x00000008
#define	_MODIFY_TIMER			0x00000010
#define	_MODIFY_ICON			0x00000020
#define	_MODIFY_PASSWORD		0x00000040
#define	_MODIFY_ZODER			0x00000080
#define	_MODIFY_RECTCHIP		0x00000100
#define	_MODIFY_POINTICON		0x00000200
#define	_MODIFY_AUTORESIZE		0x00000400

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
		int				m_nTimerType;			// �^�C�}�^�C�v 0/1/2 = ����/�j��/����
		int				m_nMonth;				// �^�C�}�̌�
		int				m_nDay;					// �^�C�}�̓�
		int				m_nWeekDay;				// �j����ʃ^�C�}�̗j���w�� 0bit=���j�� 1bit���j��...
		int				m_nNotifyShift;			// �ʒm�������j���������ꍇ�̃V�t�g
		int				m_nBeforeNotify;		// �����O����̒ʒm
		BOOL			m_blExpireTimer;		// �o�ߌ�̃^�C�}�[
		HGLOBAL			m_hachszSoundFile;		// �T�E���h�t�@�C��

		int				m_nIconIndex;			// �A�C�R�����

		RECT			m_stRctChip;			// ���ЃT�C�Y
		POINT			m_stPntIcon;			// �A�C�R���ʒu
	}IFDATA;
	typedef	BOOL ( *CHIPEXSPECIFICATION)( SPECIFICATION* pSpecification);
	typedef	BOOL ( *CHIPEXPROC)( HWND hWnd, UINT unMenuNum, IFDATA* pstIFData);

	BOOL MakeIFData( IFDATA& stIfData, CMemoData& cMemoData) const;
	BOOL DeleteIFData( IFDATA& stIFData) const;
	DWORD ModifyIFData( CMemoData& cMemoData, IFDATA& stIFData) const;
};

#endif	//__CELCONTROL_H__