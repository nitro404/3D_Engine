==========================
 ASSIGNMENT: Assignment 2
     COURSE: COMP 4002A
       NAME: Kevin Scroggins
 STUDENT ID: 100679071
     E-MAIL: kscrogg2@connect.carleton.ca
   DATE DUE: February 10, 2010
  SUBMITTED: February 10, 2010
==========================

INSTRUCTIONS
 + To test the game engine, simply click on "Run Game.bat", right click and
   open "assignment2.wrl".

NOTES
 + I have written helper batch script files to make testing easier. Here is a
   summary of what each one does:
   - "Build Maps.bat" - Compiles all of the maps with one click and places them
                        in a corresponding directory using a new custom built
                        map builder written in Java. Run the builder without
                        parameters for more information.
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

 + FGD file stored in "/Worldcraft/fgd/game/"

 + WAD file stored in "/Wally/"

 + There are some issues with the draw order of transluscent polygons and
   sprites with transparency, but this is unavoidable. There is a partially
   implemented set draw order sorting algorithms to make things look better.

 + AnimatedFace is temporarily hard-coded to only load the caustic textures.
   There are also multiple instances of the caustic animated textures loading,
   this will be fixed later.

 + Textures used in the skybox and animated textures are temporarily stored
   only locally to the skybox and animated texture instances and removed when
   they are deleted. This will be fixed later.

 + Function to determine if a point is inside of a Pool object is flawed, but
   will be fixed later.
