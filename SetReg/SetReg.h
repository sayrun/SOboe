// SetReg.h : SETREG アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_SETREG_H__38AFB2B5_7C6A_11D2_91F8_004005469C82__INCLUDED_)
#define AFX_SETREG_H__38AFB2B5_7C6A_11D2_91F8_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル
#include "IconImgList.h"

/////////////////////////////////////////////////////////////////////////////
// CSetRegApp
// このクラスの動作の定義に関しては SetReg.cpp ファイルを参照してください。
//

class CSetRegApp : public CWinApp
{
public:
	CSetRegApp();
	const CIconImgList*	GetIconList( void);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSetRegApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSetRegApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CIconImgList	m_cIconImgs;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SETREG_H__38AFB2B5_7C6A_11D2_91F8_004005469C82__INCLUDED_)
