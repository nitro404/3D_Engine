==========================
 ASSIGNMENT: Assignment 1
     COURSE: COMP 4002A
       NAME: Kevin Scroggins
 STUDENT ID: 100679071
     E-MAIL: kscrogg2@connect.carleton.ca
   DATE DUE: January 27, 2010
  SUBMITTED: January 24, 2010
==========================

INSTRUCTIONS
 + To test the game engine, simply click on "Run Game.bat", right click and
   open "assignment1.wrl".

NOTES
 + I have written helper batch script files to make testing easier. Here is a
   summary of what each one does:
   - "Build Maps.bat" - Compiles all of the maps with one click and places them
                        in a corresponding directory using my added parameter
                        parsing feature. You may now pass in a path to a single
                        file or a folder as a parameter and it will compile the
                        map. (ie. builder.exe -fC:\Test\Maps\room.uni) You may
                        also pass in a directory containing a set of universal
                        map (*.uni) files and it will automatically compile
                        them and place them in a corresponding directory. (ie.
                        builder.exe -d..\Project\Maps)
   - "Convert Maps.bat" - At the moment, this script does not work, since I was
                          trying to implement a parameter feature, similar to
                          what I did on the builder, however I have been
                          unsuccessful so far. It will crash the converter if
                          you attempt to execute it.
   - "Converter.bat" - Executes a normal, working version of the converter so
                       that you may manually convert your map (*.map) file to a
                       universal map (*.uni).
   - "Release.bat" - Builds the maps automatically and copies them, copies
                     the textures, game data files and the game executable 
                     automatically into the game release directory (folder
                     named "Game") so that you may easily text an up-to-date
                     version of the game.
   - "Wally.bat" - Executes Wally, nothing special - mostly just used as a
                   shortcut.
   - "Worldcraft.bat" - Executes Worldcraft (Valve Hammer Editor), nothing
                        special - mostly just used as a shortcut.

 + Translators may not work backwards at this point in time, a minor issue I
   will fix later. They work perfectly fine for forward values, however. The
   same goes for rotators, there is no reset value for -360 degrees, so it may
   eventually result in an integer overflow.

 + FGD file stored in "/Worldcraft/fgd/game/"

 + WAD file stored in "/Wally/"

 + Unable to compile Release version of Converter due to errors in the original
   source code, however the Debug version compiles without a problem.
