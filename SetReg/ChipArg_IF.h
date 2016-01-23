#ifndef	__CHIPARG_IF_H__
#define	__CHIPARG_IF_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define	__ARRAGESTRUCT_VERSION100	0x0100
#define	__ARRAGESTRUCT_VERSION200	0x0200

// ����\���̂�:__ARRAGESTRUCT_VERSION100
typedef	struct tagARRANGESTRUCT
{
	RECT		stRctChip;
	POINT		stPntIcon;
	COLORREF	clrText;
	COLORREF	clrBack;
	BOOL		blIcon;
	int			nIconIndex;
	BOOL		blHide;
	BOOL		blPosLock;
}ARRANGESTRUCT;

// ����\���̂�:__ARRAGESTRUCT_VERSION200
#define	ARG_WINDOW_TYPE_NORMAL	0
#define	ARG_WINDOW_TYPE_TITLE	1
#define	ARG_WINDOW_TYPE_ICON	2

typedef	struct tagARRANGESTRUCT200
{
	// �����͖{�̂ɔ��f�����
	RECT		stRctNormal;	// �S�Ă̈ʒu�����f�����
	RECT		stRctTitle;		// �^�C�g���̓^�C�g���̍��ゾ�����f�����
	POINT		stPntIcon;		// �S�Ă̈ʒu�����f�����

	// �ǉ��I�f�[�^
	COLORREF	clrText;		// �e�L�X�g�̐F
	COLORREF	clrBack;		// �w�i�F
	int			nWindowType;	// Window�`��
	int			nIconIndex;		// �A�C�R�����
	BOOL		blHide;			// �\����\��
	BOOL		blPosLock;		// �ʒu�Œ�

	// �g�����Ȃ��c�c
	BOOL		blDelOnQuit;	// �I�����ɍ폜�Ȃ̂ŁA���񂳂��Ȃ��Ƃ��ɕ֗����H�I
	BOOL		blTimerEnabel;	// ����Ȃ������c�c���邢�͏d�˂Ă��܂��悤�ɂ���Ƃ��H�I
	BOOL		blLinkEnabel;	// �����N����Ă�����̂������W�߂�Ƃ�

	// Dxl�ɕt��������́c
	UINT		unDxlID;		// 0��Dxl���ݒ肳��Ă��Ȃ�
	int			nDxlDataSize;	// 0 >= �f�[�^�Ȃ��B
	int			nDxlDataOffset;	// �z��S�̂̐擪����̃I�t�Z�b�g
}ARRANGESTRUCT200;

typedef struct tagARRANGESPEC
{
	UINT	unLowerStructVersion; // �����\�ȍŌÃo�[�W����
	UINT	unUpperStructVersion; // �����\�ȍŐV�o�[�W����
	// �����Ƀ^�b�^�Pbyte�ł��f�[�^���E���Ă����悤�ɍ���Ă�����
	// DLL�Ƃ��b�ł������̂��c�c��������(T-T
}ARRANGESPEC;

#ifdef	__cplusplus
extern "C" 
{
#endif

// ����I/F�o�[�W�����`�F�b�N�F�����K�{
__declspec( dllexport) BOOL GetArrangeSpecification( ARRANGESPEC* pstSpe);

// �ȉ���I/F�̎����͒P�ʖ��Ɏ����̂���

// �ʏ포�А���I/F
__declspec( dllexport) BOOL ArrangeChipConfigure( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeChipAbout( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeChip( UINT unVersion, int nArgDataCount, LPVOID pstArgData, int nIconCount);
// �^�C�g�������А���I/F
__declspec( dllexport) BOOL ArrangeTitleConfigure( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeTitleAbout( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeTitle( UINT unVersion, int nArgDataCount, LPVOID pstArgData, int nIconCount);
// �A�C�R�������А���I/F
__declspec( dllexport) BOOL ArrangeIconConfigure( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeIconAbout( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeIcon( UINT unVersion, int nArgDataCount, LPVOID pstArgData, LPSIZE pstIcon, int nIconCount);
// ��Ԕ�ˑ�����I/F
// ����͑S�̂ɍ�p����B���݃A�C�R����Ԃł����Ă��ʏ�T�C�Y��ݒ肷�邱�Ƃ��ł���
__declspec( dllexport) BOOL ArrangeAllConfigure( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeAllAbout( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeAll( UINT unVersion, int nArgDataCount, LPVOID pstArgData, LPSIZE pstIcon, int nIconCount);

#ifdef	__cplusplus
}
#endif

#endif	//__CHIPARG_IF_H__
