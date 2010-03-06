@ECHO OFF
CLS

SET GAME_RELEASE_DIR=Game
SET CONTENT_DIR=Content
SET DATA_DIR=Data
SET MAP_DIR=Maps
SET TEXTURE_DIR=Textures
SET HEIGHTMAP_DIR=Height Maps
SET TEXTURE_COMPILER_DIR=Texture Compiler
SET MAP_BUILDER_DIR=Builder 2.0

SET TEXTURE_COMPILER_ARGS=-t"../../Content/Data/Textures/" -m"../../Content/Data/Height Maps/" -a"../animations.ini" -h"../heightmaps.ini" -o"../../Content/Data/textures.ini"
SET MAP_BUILDER_ARGS=-d"../../Maps/" -iuni -owrl -m"../../Content/Data/Maps/" -t"../../Content/Data/textures.ini" -h"../../Content/Data/Height Maps/"

SET ENGINE_RELEASE_DIR=Engine\Release
SET ENGINE_DEBUG_DIR=Engine\Debug
SET ENGINE_EXE_COMPILED=Engine.exe
SET GAME_EXE=Game.exe
SET GAME_EXE_DEBUG=Debug.exe

SET TEXTURECOMPILER_COMPILED=Compiled
SET MAPBUILDER_COMPILED=Compiled

IF NOT EXIST "%TEXTURE_COMPILER_DIR%/bin" MKDIR "Texture Compiler/bin"
IF NOT EXIST "%TEXTURE_COMPILER_DIR%/bin/AnimatedTexture.class" 	SET TEXTURECOMPILER_COMPILED=Not Compiled
IF NOT EXIST "%TEXTURE_COMPILER_DIR%/bin/HeightMap.class" 	SET TEXTURECOMPILER_COMPILED=Not Compiled
IF NOT EXIST "%TEXTURE_COMPILER_DIR%/bin/Property.class" 		SET TEXTURECOMPILER_COMPILED=Not Compiled
IF NOT EXIST "%TEXTURE_COMPILER_DIR%/bin/Texture.class" 		SET TEXTURECOMPILER_COMPILED=Not Compiled
IF NOT EXIST "%TEXTURE_COMPILER_DIR%/bin/TextureCompiler.class" 	SET TEXTURECOMPILER_COMPILED=Not Compiled

IF NOT EXIST "%MAP_BUILDER_DIR%/bin" MKDIR "Builder 2.0/bin"
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/AnimatedTexture.class" 	SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Builder.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Converter.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Face.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/GamePoint3D.class" 	SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Geometry.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/HeightMap.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Map3D.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Point3D.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Pool.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Property.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Rotator.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Sprite.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Terrain.class"		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Texture.class"		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Transformation.class" 	SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Translator.class"		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/UniversalMap.class" 	SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/UniversalObject.class" 	SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Vehicle.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/Waypoint.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/World.class" 		SET MAPBUILDER_COMPILED=Not Compiled
IF NOT EXIST "%MAP_BUILDER_DIR%/bin/WorldcraftMap.class" 	SET MAPBUILDER_COMPILED=Not Compiled

ECHO Creating new release of Game in folder "%GAME_RELEASE_DIR%"...

ECHO.
CD "%TEXTURE_COMPILER_DIR%"
IF "%TEXTURECOMPILER_COMPILED%" == "Compiled" GOTO TEXTURECOMPILER_ISCOMPILED
ECHO Compiling Texture Compiler...
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
:TEXTURECOMPILER_ISCOMPILED
CD bin
ECHO Compiling Texture List...
java TextureCompiler %TEXTURE_COMPILER_ARGS%
ECHO Done Compiling Texture List!
CD "../../"

ECHO.
CD "%MAP_BUILDER_DIR%"
IF "%MAPBUILDER_COMPILED%" == "Compiled" GOTO MAPBUILDER_ISCOMPILED
ECHO Compiling Map Builder...
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
:MAPBUILDER_ISCOMPILED
CD bin
ECHO Building Maps...
java Builder %MAP_BUILDER_ARGS%
ECHO Done Building Maps!
CD "../../"

ECHO.
ECHO Copying Maps...
XCOPY "%MAP_DIR%\*.wrl" "%CONTENT_DIR%\%DATA_DIR%\%MAP_DIR%" /I /Y

ECHO.
ECHO Copying Data Files...
XCOPY "%CONTENT_DIR%" "%GAME_RELEASE_DIR%" /I /Y /S

ECHO.
ECHO Copying Game Executable...
IF EXIST "%GAME_RELEASE_DIR%\%GAME_EXE%" DEL "%GAME_RELEASE_DIR%\%GAME_EXE%"
XCOPY "%ENGINE_RELEASE_DIR%\%ENGINE_EXE_COMPILED%" "%GAME_RELEASE_DIR%" /I /Y
REN "%GAME_RELEASE_DIR%\%ENGINE_EXE_COMPILED%" "%GAME_EXE%"

ECHO.
ECHO Done!

ECHO.
ECHO Copy Debug Executable? (Y/N)
IF EXIST "%GAME_RELEASE_DIR%\%GAME_EXE_DEBUG%" DEL "%GAME_RELEASE_DIR%\%GAME_EXE_DEBUG%"
SET /P DEBUG=
IF "%DEBUG:~0,1%" == "Y" GOTO COPY_DEBUG
IF "%DEBUG:~0,1%" == "y" GOTO COPY_DEBUG
GOTO SKIP_DEBUG
:COPY_DEBUG
XCOPY "%ENGINE_DEBUG_DIR%\%ENGINE_EXE_COMPILED%" "%GAME_RELEASE_DIR%" /I /Y
REN "%GAME_RELEASE_DIR%\%ENGINE_EXE_COMPILED%" "%GAME_EXE_DEBUG%"
ECHO.
ECHO Done!
:SKIP_DEBUG

ECHO.
PAUSE
