@ECHO OFF
CLS

SET BUILDER=Compiled

IF NOT EXIST "Builder 2.0/bin" MKDIR "Builder 2.0/bin"
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
PAUSE
