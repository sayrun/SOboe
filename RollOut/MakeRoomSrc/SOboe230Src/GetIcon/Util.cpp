#include "stdafx.h"
#include "Util.h"

BOOL GetAppIniPath( LPTSTR lpszFilePath)
{
	if( lpszFilePath)
	{
		GetModuleFileName( NULL, lpszFilePath, MAX_PATH);

		int nLength = lstrlen( lpszFilePath);
		while( nLength)
		{
			if( TEXT( '\\') == lpszFilePath[ nLength])
			{
				lstrcpy( &lpszFilePath[ nLength + 1], TEXT( "GetIcon.ini"));
				break;
			}
			nLength--;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL GetAppXmlPath( LPTSTR lpszFilePath)
{
	if( lpszFilePath)
	{
		GetModuleFileName( NULL, lpszFilePath, MAX_PATH);

		int nLength = lstrlen( lpszFilePath);
		while( nLength)
		{
			if( TEXT( '\\') == lpszFilePath[ nLength])
			{
				lstrcpy( &lpszFilePath[ nLength + 1], TEXT( "GetIcon.xml"));
				break;
			}
			nLength--;
		}
		return TRUE;
	}
	return FALSE;
}
