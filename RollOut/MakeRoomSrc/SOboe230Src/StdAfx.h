// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__386BA42F_26B0_11D2_90A3_00804C15184E__INCLUDED_)
#define AFX_STDAFX_H__386BA42F_26B0_11D2_90A3_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>
#include <afxmt.h>
#include <afxole.h>
#include <atlbase.h>

#include <MMSystem.h>
#include <imm.h>
#include <pbt.h>
#include <afx.h>

#define	_MAX_ICONINDEX		13
#define	_ICON_WIDTH			16
#define	_ICON_HEIGHT		16

//#ifdef	_KEYCONTROL_ENABLE_		// ���̒�`��L���ɂ���ƃL�[�{�[�h�ŏ��Ђ��ړ��ł���悤�ɂȂ�

#import "c:\windows\system32\msxml3.dll"

// CG: �ȉ��̍s�� Windows �}���`���f�B�A �R���|�[�l���g�ɂ���Ēǉ�����܂����B
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "imm32.lib")
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__386BA42F_26B0_11D2_90A3_00804C15184E__INCLUDED_)
