@ECHO OFF
CLS

SET RELEASE=Game
SET BUILDER=Compiled

IF NOT EXIST "Builder 2.0/bin/Builder.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Converter.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Face.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/GamePoint3D.class" 	SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Geometry.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Map3D.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Point3D.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Pool.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Property.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Rotator.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Sprite.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Transformation.class" 	SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Translator.class"		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/UniversalMap.class" 	SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/UniversalObject.class" 	SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Vehicle.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/Waypoint.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/World.class" 		SET BUILDER=Not Compiled
IF NOT EXIST "Builder 2.0/bin/WorldcraftMap.class" 	SET BUILDER=Not Compiled

ECHO Creating new release of Game in folder "%RELEASE%"...

ECHO.
CD "Builder 2.0"
IF "%BUILDER%" == "Compiled" GOTO BUILDER_COMPILED
ECHO Compiling Map Builder...
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
:BUILDER_COMPILED
CD bin
ECHO Building Maps...
java Builder -d../../Maps -iuni -owrl -m../../Maps
ECHO Done Building Maps!
CD "../../"

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
