// SetReg.h : SETREG �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_SETREG_H__38AFB2B5_7C6A_11D2_91F8_004005469C82__INCLUDED_)
#define AFX_SETREG_H__38AFB2B5_7C6A_11D2_91F8_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��
#include "IconImgList.h"

/////////////////////////////////////////////////////////////////////////////
// CSetRegApp
// ���̃N���X�̓���̒�`�Ɋւ��Ă� SetReg.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CSetRegApp : public CWinApp
{
public:
	CSetRegApp();
	const CIconImgList*	GetIconList( void);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSetRegApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSetRegApp)
		// ���� -  ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//         ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CIconImgList	m_cIconImgs;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SETREG_H__38AFB2B5_7C6A_11D2_91F8_004005469C82__INCLUDED_)
