# Microsoft Developer Studio Project File - Name="email" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=email - Win32 Debug for winsock11
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "email.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "email.mak" CFG="email - Win32 Debug for winsock11"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "email - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "email - Win32 Debug for winsock11" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "email - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "email - Win32 Release for winsock11" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "email - Win32 Release SMTP Only" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "email - Win32 Release SMTP Only for winsock11" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SOboe/emailEx", VJAAAAAA"
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "email - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D _WIN32_WINNT=0x0400 /D "_MBCS" /D "_POP3_TOO" /Yu"StdeMail.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_WINSOCK_2" /d "_POP3_TOO"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib comctl32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../Debug/email.nxl" /pdbtype:sept

!ELSEIF  "$(CFG)" == "email - Win32 Debug for winsock11"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "email___"
# PROP BASE Intermediate_Dir "email___"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug4ws11"
# PROP Intermediate_Dir "Debug4ws11"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D _WIN32_WINNT=0x0400 /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_POP3_TOO" /Yu"StdeMail.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_WINSOCK_1" /d "_POP3_TOO"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../Debug/email.nxl" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib comctl32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../Debug/email.nxl" /pdbtype:sept

!ELSEIF  "$(CFG)" == "email - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "email___Win32_ReleasePop3Too"
# PROP BASE Intermediate_Dir "email___Win32_ReleasePop3Too"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _WIN32_WINNT=0x0400 /D "_MBCS" /D "_POP3_TOO" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _WIN32_WINNT=0x0400 /D "_MBCS" /D "_POP3_TOO" /Yu"StdeMail.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_WINSOCK_2"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_WINSOCK_2" /d "_POP3_TOO"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib comctl32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"ReleaseSMTPOnly/email.nxl"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib comctl32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/email.nxl"

!ELSEIF  "$(CFG)" == "email - Win32 Release for winsock11"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "email___Win32_Release95Pop3Too"
# PROP BASE Intermediate_Dir "email___Win32_Release95Pop3Too"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release4ws11"
# PROP Intermediate_Dir "Release4ws11"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_POP3_TOO" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_POP3_TOO" /Yu"StdeMail.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_WINSOCK_1"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_WINSOCK_1" /d "_POP3_TOO"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib comctl32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"ReleaseSMTPOnly4ws11/email.nxl"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib comctl32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release4ws11/email.nxl"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "email___Win32_Release"
# PROP BASE Intermediate_Dir "email___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseSMTP"
# PROP Intermediate_Dir "ReleaseSMTP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _WIN32_WINNT=0x0400 /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _WIN32_WINNT=0x0400 /D "_MBCS" /Yu"StdeMail.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_WINSOCK_2"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_WINSOCK_2"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib comctl32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"ReleaseSMTP/email.nxl"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib comctl32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"ReleaseSMTP/email.nxl"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only for winsock11"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "email___Win32_Release95"
# PROP BASE Intermediate_Dir "email___Win32_Release95"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseSMTP4ws11"
# PROP Intermediate_Dir "ReleaseSMTP4ws11"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"StdeMail.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_WINSOCK_1"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_WINSOCK_1"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib comctl32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"ReleaseSMTP4ws11/email.nxl"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib comctl32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"ReleaseSMTP4ws11/email.nxl"

!ENDIF 

# Begin Target

# Name "email - Win32 Debug"
# Name "email - Win32 Debug for winsock11"
# Name "email - Win32 Release"
# Name "email - Win32 Release for winsock11"
# Name "email - Win32 Release SMTP Only"
# Name "email - Win32 Release SMTP Only for winsock11"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Base64.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Dialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\DInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DllMain.cpp
# ADD CPP /Yc"StdeMail.h"
# End Source File
# Begin Source File

SOURCE=.\email.cpp
# End Source File
# Begin Source File

SOURCE=.\email.def
# End Source File
# Begin Source File

SOURCE=.\email.rc
# End Source File
# Begin Source File

SOURCE=.\emailslv.cpp
# End Source File
# Begin Source File

SOURCE=.\EntryList.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupList.cpp
# End Source File
# Begin Source File

SOURCE=.\IDMapSubject.cpp
# End Source File
# Begin Source File

SOURCE=.\KanjiUtl.cpp
# End Source File
# Begin Source File

SOURCE=.\Mail.cpp

!IF  "$(CFG)" == "email - Win32 Debug"

!ELSEIF  "$(CFG)" == "email - Win32 Debug for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release"

!ELSEIF  "$(CFG)" == "email - Win32 Release for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only for winsock11"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MailS.cpp
# End Source File
# Begin Source File

SOURCE=.\OrgSock.cpp

!IF  "$(CFG)" == "email - Win32 Debug"

!ELSEIF  "$(CFG)" == "email - Win32 Debug for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "email - Win32 Release for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only for winsock11"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pop3Client.cpp

!IF  "$(CFG)" == "email - Win32 Debug"

!ELSEIF  "$(CFG)" == "email - Win32 Debug for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release"

!ELSEIF  "$(CFG)" == "email - Win32 Release for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only for winsock11"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pop3Part.cpp

!IF  "$(CFG)" == "email - Win32 Debug"

!ELSEIF  "$(CFG)" == "email - Win32 Debug for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release"

!ELSEIF  "$(CFG)" == "email - Win32 Release for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only for winsock11"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Property.cpp
# End Source File
# Begin Source File

SOURCE=.\RStream.cpp
# End Source File
# Begin Source File

SOURCE=.\RStreamEx.cpp

!IF  "$(CFG)" == "email - Win32 Debug"

!ELSEIF  "$(CFG)" == "email - Win32 Debug for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release"

!ELSEIF  "$(CFG)" == "email - Win32 Release for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only for winsock11"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StrList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Base64.h
# End Source File
# Begin Source File

SOURCE=.\Dialogs.h
# End Source File
# Begin Source File

SOURCE=.\DInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\email.h
# End Source File
# Begin Source File

SOURCE=.\emailslv.h
# End Source File
# Begin Source File

SOURCE=.\EntryList.h
# End Source File
# Begin Source File

SOURCE=.\GroupList.h
# End Source File
# Begin Source File

SOURCE=.\IDMapSubject.h
# End Source File
# Begin Source File

SOURCE=.\KanjiUtl.h
# End Source File
# Begin Source File

SOURCE=.\Mail.h

!IF  "$(CFG)" == "email - Win32 Debug"

!ELSEIF  "$(CFG)" == "email - Win32 Debug for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release"

!ELSEIF  "$(CFG)" == "email - Win32 Release for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only for winsock11"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mail.inl

!IF  "$(CFG)" == "email - Win32 Debug"

!ELSEIF  "$(CFG)" == "email - Win32 Debug for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release"

!ELSEIF  "$(CFG)" == "email - Win32 Release for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only for winsock11"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MailS.h
# End Source File
# Begin Source File

SOURCE=.\MailS.inl
# End Source File
# Begin Source File

SOURCE=.\NetEx_IF.h
# End Source File
# Begin Source File

SOURCE=.\OrgSock.h
# End Source File
# Begin Source File

SOURCE=.\Pop3Client.h

!IF  "$(CFG)" == "email - Win32 Debug"

!ELSEIF  "$(CFG)" == "email - Win32 Debug for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release"

!ELSEIF  "$(CFG)" == "email - Win32 Release for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only for winsock11"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pop3Part.h

!IF  "$(CFG)" == "email - Win32 Debug"

!ELSEIF  "$(CFG)" == "email - Win32 Debug for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release"

!ELSEIF  "$(CFG)" == "email - Win32 Release for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only for winsock11"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Property.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\RStream.hpp
# End Source File
# Begin Source File

SOURCE=.\RStreamEx.h

!IF  "$(CFG)" == "email - Win32 Debug"

!ELSEIF  "$(CFG)" == "email - Win32 Debug for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release"

!ELSEIF  "$(CFG)" == "email - Win32 Release for winsock11"

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "email - Win32 Release SMTP Only for winsock11"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdeMail.h
# End Source File
# Begin Source File

SOURCE=.\StrList.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\status.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
