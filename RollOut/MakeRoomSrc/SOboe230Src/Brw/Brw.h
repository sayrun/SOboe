// Brw.h : BRW アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_BRW_H__4CEAD6CE_8BCF_11D1_9011_00804C15184E__INCLUDED_)
#define AFX_BRW_H__4CEAD6CE_8BCF_11D1_9011_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CBrwApp
// このクラスの動作の定義に関しては Brw.cpp ファイルを参照してください。
//

class CBrwApp : public CWinApp
{
public:
	CBrwApp();

	BOOL GetDefaultFont( LOGFONT* pstLogFont);
	COLORREF GetDefaultForeColor( void);
	COLORREF GetDefaultBackColor( void);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CBrwApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBrwApp)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_BRW_H__4CEAD6CE_8BCF_11D1_9011_00804C15184E__INCLUDED_)
