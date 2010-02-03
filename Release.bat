@ECHO OFF
CLS

SET RELEASE=Game

ECHO Creating new release of Game in folder "%RELEASE%"...

ECHO.
ECHO Building Maps...
CD Builder 2.0/bin
java Builder -d../../Maps -iuni -owrl -m../../Maps
CD ../../

ECHO.
ECHO Copying Maps...
XCOPY "Maps\*.wrl" "%RELEASE%\Maps" /I /Y

ECHO.
ECHO Copying Textures...
XCOPY "Textures" "%RELEASE%\Textures" /I /Y

ECHO.
ECHO Copying Data Files...
XCOPY "Data" %RELEASE% /I /Y /S

ECHO.
ECHO Copying Game Executable...
XCOPY "Engine\Release\game.exe"  %RELEASE% /I /Y
REN "%RELEASE%\game.exe" "Game.exe"

IF EXIST log DEL log

ECHO.
ECHO Done!
ECHO.

PAUSE
