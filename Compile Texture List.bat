@ECHO OFF
CLS

SET TEXTURECOMPILER=Compiled

IF NOT EXIST "Texture Compiler/bin" MKDIR "Texture Compiler/bin"
IF NOT EXIST "Texture Compiler/bin/AnimatedTexture.class" 	SET TEXTURECOMPILER=Not Compiled
IF NOT EXIST "Texture Compiler/bin/Property.class" 		SET TEXTURECOMPILER=Not Compiled
IF NOT EXIST "Texture Compiler/bin/TextureCompiler.class" 	SET TEXTURECOMPILER=Not Compiled

CD "Texture Compiler"
IF "%TEXTURECOMPILER%" == "Compiled" GOTO TEXTURECOMPILER_COMPILED
ECHO Compiling Texture Compiler...
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
:TEXTURECOMPILER_COMPILED
CD bin
ECHO Compiling Texture List...
java TextureCompiler
REM java TextureCompiler -i"../animations.ini" -o"../textures.ini"
ECHO Done Compiling Texture List!
CD "../../"
ECHO.
ECHO Copying Texture List...
XCOPY "Texture Compiler\textures.ini" "Builder 2.0" /I /Y

PAUSE
