#pragma pack( push, 1)
typedef struct tagBRWIFDATA200
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

	int				m_nReserved;			// 
}BRWIFDATA200;

typedef struct tagBRWIFDATA300
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
}BRWIFDATA300;
#pragma pack( pop)
