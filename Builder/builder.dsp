# Microsoft Developer Studio Project File - Name="builder" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=builder - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "builder.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "builder.mak" CFG="builder - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "builder - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "builder - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "builder - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "builder - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x1009 /d "_DEBUG"
# ADD RSC /l 0x1009 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 glut32.lib glu32.lib opengl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "builder - Win32 Release"
# Name "builder - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Source\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\face.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\logging.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Methods.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\object.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\plane.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\points.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Strcasecmp.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Variable.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Variables.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\world.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Source\camera.h
# End Source File
# Begin Source File

SOURCE=.\Source\face.h
# End Source File
# Begin Source File

SOURCE=.\Source\fileDialog.h
# End Source File
# Begin Source File

SOURCE=.\Source\game.h
# End Source File
# Begin Source File

SOURCE=.\Source\includes.all
# End Source File
# Begin Source File

SOURCE=.\Source\logging.h
# End Source File
# Begin Source File

SOURCE=.\Source\Methods.h
# End Source File
# Begin Source File

SOURCE=.\Source\object.h
# End Source File
# Begin Source File

SOURCE=.\Source\plane.h
# End Source File
# Begin Source File

SOURCE=.\Source\points.h
# End Source File
# Begin Source File

SOURCE=.\Source\search.h
# End Source File
# Begin Source File

SOURCE=.\Source\Strcasecmp.h
# End Source File
# Begin Source File

SOURCE=.\Source\texture.h
# End Source File
# Begin Source File

SOURCE=.\Source\transformations.h
# End Source File
# Begin Source File

SOURCE=.\Source\utilities.h
# End Source File
# Begin Source File

SOURCE=.\Source\Variable.h
# End Source File
# Begin Source File

SOURCE=.\Source\Variables.h
# End Source File
# Begin Source File

SOURCE=.\Source\world.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Others"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
