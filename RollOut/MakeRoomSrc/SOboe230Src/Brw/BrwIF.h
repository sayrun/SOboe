#ifndef	__BACKUPCHIP_READ_WRITE_H__
#define	__BACKUPCHIP_READ_WRITE_H__

#define	_BRW_IF_DATA_200_VER		0x00000200
#define	_BRW_IF_DATA_300_VER		0x00000300
#define	_BRW_IF_DATA_CUR_VER		0x00000400

#pragma pack( push, 1)
typedef struct tagBRWIFDATA
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
}BRWIFDATA;

#pragma pack( pop)

#ifdef	__cplusplus
extern "C" 
{
#endif

__declspec( dllexport ) DWORD WINAPI GetBrwVersion( int* pnOldStruct, int* pnNewStruct);

__declspec( dllexport ) void WINAPI InitializeBrw( void);
__declspec( dllexport ) void WINAPI UnInitializeBrw( void);
__declspec( dllexport ) int WINAPI CreateChipGroup( void);
__declspec( dllexport ) int WINAPI ReleaseChipGroup( int nChipGroupID);
__declspec( dllexport ) int WINAPI ReadChipGroup( int nChipGroupID, LPCSTR lpcszFileName);
__declspec( dllexport ) int WINAPI WriteChipGroup( int nChipGroupID, LPCSTR lpcszFileName);
__declspec( dllexport ) int WINAPI GetChipCount( int nChipGroupID);
__declspec( dllexport ) int WINAPI GetChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData, LPCSTR lpcszPassWord);
__declspec( dllexport ) int WINAPI SetChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData, LPCSTR lpcszPassWord);
__declspec( dllexport ) int WINAPI AddChipData( int nChipGroupID, BRWIFDATA* pBrwData);
__declspec( dllexport ) int WINAPI InsertAfterChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData);
__declspec( dllexport ) int WINAPI InsertBeforeChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData);
__declspec( dllexport ) int WINAPI RemoveChipData( int nChipGroupID, int nDataIndex, LPCSTR lpszPassWord);
__declspec( dllexport ) int WINAPI FreeChipData( BRWIFDATA* pBrwData);

__declspec( dllexport ) int WINAPI GetChipTitle( int nChipGroupID, int nDataIndex, LPSTR lpszTitle);
__declspec( dllexport ) int WINAPI GetChipMemo( int nChipGroupID, int nDataIndex, LPSTR lpszData, int nSize, LPCSTR lpcszPassWord);
__declspec( dllexport ) int WINAPI GetChipFontColor( int nChipGroupID, int nDataIndex, COLORREF* lpColor);
__declspec( dllexport ) int WINAPI GetChipBackColor( int nChipGroupID, int nDataIndex, COLORREF* lpColor);
__declspec( dllexport ) int WINAPI GetChipIcon( int nChipGroupID, int nDataIndex);
__declspec( dllexport ) int WINAPI GetChipFont( int nChipGroupID, int nDataIndex, LOGFONT* pstLogFont);
__declspec( dllexport ) int WINAPI IsPwLockChip( int nChipGroupID, int nDataIndex);
__declspec( dllexport ) int WINAPI TestChipPw( int nChipGroupID, int nDataIndex, LPCSTR lpszPassWord);
__declspec( dllexport ) int WINAPI IsLink( int nChipGroupID, int nDataIndex);

__declspec( dllexport ) int WINAPI MoveChipData( int nToChipGroupID, int nToDataIndex, int nFromChipGroupID, int nFromDataIndex);
__declspec( dllexport ) int WINAPI SwapChipData( int nDstChipGroupID, int nDstDataIndex, int nSrcFromChipGroupID, int nSrcFromDataIndex);
__declspec( dllexport ) int WINAPI CopyChipData( int nToChipGroupID, int nToDataIndex, int nFromChipGroupID, int nFromDataIndex);

__declspec( dllexport ) int WINAPI DrawIconHelper( HDC hDC, int nLeft, int nTop, int nIconIndex, COLORREF clrFont, COLORREF clrBack);
__declspec( dllexport ) int WINAPI GetMaxIconIndex( void);
__declspec( dllexport ) HBITMAP WINAPI GetIconImage( void);

__declspec( dllexport ) int WINAPI ExportChipGroup( int nChipGroupID, LPCSTR lpcszFileName);
__declspec( dllexport ) int WINAPI ExportChipData( int nChipGroupID, int nDataIndex, LPCSTR lpcszFileName);
__declspec( dllexport ) int WINAPI ImportChipGroup( int nChipGroupID, LPCSTR lpcszFileName, BOOL blAppend);

#ifdef	__cplusplus
}
#endif
#endif	//__BACKUPCHIP_READ_WRITE_H__
