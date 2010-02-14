	@ECHO OFF
CLS

SET RELEASE=Game
SET TEXTURECOMPILER=Compiled
SET BUILDER=Compiled

IF NOT EXIST "Texture Compiler/bin" MKDIR "Texture Compiler/bin"
IF NOT EXIST "Texture Compiler/bin/AnimatedTexture.class" 	SET TEXTURECOMPILER=Not Compiled
IF NOT EXIST "Texture Compiler/bin/Property.class" 		SET TEXTURECOMPILER=Not Compiled
IF NOT EXIST "Texture Compiler/bin/Texture.class" 		SET TEXTURECOMPILER=Not Compiled
IF NOT EXIST "Texture Compiler/bin/TextureCompiler.class" 	SET TEXTURECOMPILER=Not Compiled

IF NOT EXIST "Builder 2.0/bin" MKDIR "Builder 2.0/bin"
IF NOT EXIST "Builder 2.0/bin/AnimatedTexture.class" 	SET BUILDER=Not Compiled
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
IF NOT EXIST "Builder 2.0/bin/Texture.class"		SET BUILDER=Not Compiled
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
CD "Texture Compiler"
IF "%TEXTURECOMPILER%" == "Compiled" GOTO TEXTURECOMPILER_COMPILED
ECHO Compiling Texture Compiler...
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
:TEXTURECOMPILER_COMPILED
CD bin
ECHO Compiling Texture List...
java TextureCompiler -d"../../textures/" -i"../animations.ini" -o"../textures.ini"
ECHO Done Compiling Texture List!
CD "../../"
ECHO.
ECHO Copying Texture List...
XCOPY "Texture Compiler\textures.ini" "Builder 2.0" /I /Y
XCOPY "Texture Compiler\textures.ini" "Data" /I /Y

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
java Builder -d../../Maps -iuni -owrl -m../../Maps -t../textures.ini
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
XCOPY "Data" "%RELEASE%" /I /Y /S

ECHO.
ECHO Copying Game Executable...
XCOPY "Engine\Release\game.exe" "%RELEASE%" /I /Y
REN "%RELEASE%\game.exe" "Game.exe"

REM IF EXIST log DEL log

ECHO.
ECHO Done!

ECHO.
PAUSE
