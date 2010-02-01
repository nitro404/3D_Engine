# Microsoft Developer Studio Generated NMAKE File, Based on game.dsp
!IF "$(CFG)" == ""
CFG=game - Win32 Debug
!MESSAGE No configuration specified. Defaulting to game - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "game - Win32 Release" && "$(CFG)" != "game - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "game.mak" CFG="game - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "game - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "game - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "game - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\game.exe"


CLEAN :
	-@erase "$(INTDIR)\camera.obj"
	-@erase "$(INTDIR)\face.obj"
	-@erase "$(INTDIR)\fileDialog.obj"
	-@erase "$(INTDIR)\game.obj"
	-@erase "$(INTDIR)\logging.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\plane.obj"
	-@erase "$(INTDIR)\points.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\world.obj"
	-@erase "$(INTDIR)\worldcraftReader.obj"
	-@erase "$(OUTDIR)\game.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\game.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\game.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\game.pdb" /machine:I386 /out:"$(OUTDIR)\game.exe" 
LINK32_OBJS= \
	"$(INTDIR)\camera.obj" \
	"$(INTDIR)\fileDialog.obj" \
	"$(INTDIR)\game.obj" \
	"$(INTDIR)\logging.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\points.obj" \
	"$(INTDIR)\world.obj" \
	"$(INTDIR)\plane.obj" \
	"$(INTDIR)\worldcraftReader.obj" \
	"$(INTDIR)\face.obj"

"$(OUTDIR)\game.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\game.exe" "$(OUTDIR)\game.bsc"


CLEAN :
	-@erase "$(INTDIR)\camera.obj"
	-@erase "$(INTDIR)\camera.sbr"
	-@erase "$(INTDIR)\face.obj"
	-@erase "$(INTDIR)\face.sbr"
	-@erase "$(INTDIR)\fileDialog.obj"
	-@erase "$(INTDIR)\fileDialog.sbr"
	-@erase "$(INTDIR)\game.obj"
	-@erase "$(INTDIR)\game.sbr"
	-@erase "$(INTDIR)\logging.obj"
	-@erase "$(INTDIR)\logging.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\plane.obj"
	-@erase "$(INTDIR)\plane.sbr"
	-@erase "$(INTDIR)\points.obj"
	-@erase "$(INTDIR)\points.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\world.obj"
	-@erase "$(INTDIR)\world.sbr"
	-@erase "$(INTDIR)\worldcraftReader.obj"
	-@erase "$(INTDIR)\worldcraftReader.sbr"
	-@erase "$(OUTDIR)\game.bsc"
	-@erase "$(OUTDIR)\game.exe"
	-@erase "$(OUTDIR)\game.ilk"
	-@erase "$(OUTDIR)\game.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\game.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\game.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\camera.sbr" \
	"$(INTDIR)\fileDialog.sbr" \
	"$(INTDIR)\game.sbr" \
	"$(INTDIR)\logging.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\points.sbr" \
	"$(INTDIR)\world.sbr" \
	"$(INTDIR)\plane.sbr" \
	"$(INTDIR)\worldcraftReader.sbr" \
	"$(INTDIR)\face.sbr"

"$(OUTDIR)\game.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=glut32.lib glu32.lib opengl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\game.pdb" /debug /machine:I386 /out:"$(OUTDIR)\game.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\camera.obj" \
	"$(INTDIR)\fileDialog.obj" \
	"$(INTDIR)\game.obj" \
	"$(INTDIR)\logging.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\points.obj" \
	"$(INTDIR)\world.obj" \
	"$(INTDIR)\plane.obj" \
	"$(INTDIR)\worldcraftReader.obj" \
	"$(INTDIR)\face.obj"

"$(OUTDIR)\game.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("game.dep")
!INCLUDE "game.dep"
!ELSE 
!MESSAGE Warning: cannot find "game.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "game - Win32 Release" || "$(CFG)" == "game - Win32 Debug"
SOURCE=.\Source\camera.cpp

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\camera.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\camera.obj"	"$(INTDIR)\camera.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Source\face.cpp

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\face.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\face.obj"	"$(INTDIR)\face.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Source\fileDialog.cpp

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\fileDialog.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\fileDialog.obj"	"$(INTDIR)\fileDialog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Source\game.cpp

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\game.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\game.obj"	"$(INTDIR)\game.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Source\logging.cpp

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\logging.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\logging.obj"	"$(INTDIR)\logging.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Source\main.cpp

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Source\plane.cpp

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\plane.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\plane.obj"	"$(INTDIR)\plane.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Source\points.cpp

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\points.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\points.obj"	"$(INTDIR)\points.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Source\world.cpp

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\world.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\world.obj"	"$(INTDIR)\world.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Source\worldcraftReader.cpp

!IF  "$(CFG)" == "game - Win32 Release"


"$(INTDIR)\worldcraftReader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "game - Win32 Debug"


"$(INTDIR)\worldcraftReader.obj"	"$(INTDIR)\worldcraftReader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

