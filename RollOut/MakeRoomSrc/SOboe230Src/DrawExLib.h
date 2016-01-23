#ifndef	__DRAWEXLIB_H__
#define	__DRAWEXLIB_H__

#include "DrawEx_IF.h"

class CDrawExLib : public CObject
{
public:
	CDrawExLib();
	~CDrawExLib();

	const CString& GetPath( void) const;
	BOOL SetPath( LPCSTR lpcsz);

	HINSTANCE GetInstance( void) const;
	const CString& GetMenuString( void) const;
	BOOL SetMenuString( LPCSTR lpcsz);
	UINT GetDataVersion( void) const;

	UINT GetDxlID( void) const;
	UINT GetDrawType( void) const;
	UINT GetDxlStyle( void) const;

	BOOL LoadDxl( void);
	BOOL FreeDxl( void);

	BOOL IsFunctionAssign( void) const;
	BOOL IsSupportEditHelper( void) const;
	BOOL IsSupportExSend( void) const;
	BOOL IsSupportNativeData( void) const;

	const int UseStart( void);
	const int UseEnd( void);
	int GetUseCount( void) const;

	BOOL DoEditHelper( HWND hWnd, EDITDATA* pstEditData) const;
	BOOL About( HWND hWnd) const;
	BOOL ExchangeSendString( EDITDATA* pstEditData) const;
protected:
	// �@�\�m�F
	typedef	BOOL ( *DRAWEXSPECIFICATION)( DXLSPECIFICATION* pDxlSpecification);
	// Copyright�ύX�p
	typedef	BOOL ( *DRAWEXABOUT)( HWND hWnd);
	// ���M���镶����
	typedef	int ( *EXCHANGESENDSTRING)( HGLOBAL* hgMem);
	// ����ҏW�F���̓w���p�[
	typedef	int ( *EDITHELPER)( HWND hDlgWnd, EDITDATA* pstEditData);
	// �ȍ~�̃C���^�[�t�F�C�X�́A�V���o�����ŗ��p
	// ���M���镶����F�g��
	typedef	int ( *EXCHANGESENDSTRINGEX)( EDITDATA* pstEditData);

	UINT		m_unDxlDataVersion;		// �����\�ȍŐV�o�[�W����
	CString*	m_pcStrMenuString;		// �f�t�H���g�̏�������
	UINT		m_unDXLID;				// �Sdxl�ԂŃ��j�[�N�Ȃh�c�FT.Chiba�̔��s�ɂ��
	UINT		m_unDrawType;			// dxl�̏������e
	UINT		m_unStyle;				// dxl�̃X�^�C��

	HINSTANCE	m_hInstance;			// 
	CString*	m_pcStrDxlPath;			// �t�@�C����

	int			m_nUseCount;
};

#include "DrawExLib.inl"
#endif	//__DRAWEXLIB_H__