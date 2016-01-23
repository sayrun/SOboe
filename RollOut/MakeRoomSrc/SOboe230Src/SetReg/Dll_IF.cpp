#include "stdafx.h"
#include "resource.h"
#include "DLL_IF.h"
#include "ProfileData.h"
#include "CustomizeSheet.h"

UINT GetSetRegVersion( void)
{
	return ( UINT)_SETREG_VERSION;
}

int DoCustomize( BOOL blSpec, HWND hWnd)
{
	CProfileData	cProfileData;
	cProfileData.LoadProfile();


	CWnd	cWnd;
	cWnd.Attach( hWnd);
	CCustomizeSheet	cCustomizeSheet( blSpec, &cWnd);
	cCustomizeSheet.SetProfileData( &cProfileData);
	if( IDOK == cCustomizeSheet.DoModal())
	{
		cProfileData.SaveProfile();
		cWnd.Detach();
		return IDOK;
	}
	cWnd.Detach();
	return IDCANCEL;
}
