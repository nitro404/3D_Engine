=============================
   3D Engine Final Copy
=============================
     COURSE: COMP 4002A

       NAME: Kevin Scroggins
 STUDENT ID: 100679071
     E-MAIL: nitro404@hotmail.com
 ALT E-MAIL: kscrogg2@connect.carleton.ca

       NAME: Ian Doerr
 STUDENT ID: 100771017
     E-MAIL: idoerr@connect.carleton.ca

   DATE DUE: April 19, 2010
  SUBMITTED: April 19, 2010
=============================

INSTRUCTIONS
 + A pre-compiled runnable version of the game has been placed in the "Game"
   folder. It can be executed by simply running "Run Game.bat", however if you
   have any issues running it, use the following steps to re-compile:

 + If you need to re-compile at all, open "Engine.sln" in the "Engine" folder
   using Visual Studio 2008. Compile under release mode, close, then run
   "Release.bat" to create a runnable version of the game in a folder called
   "Game". If the Map Builder or Texture Compiler complains about running on
   the wrong version of Java, ensure that the 1.6 JDK is installed and in the
   system path, then run "Build Maps.bat" to recompile the Map Builder and
   Texture Compiler, then run "Release.bat" again to create a runnable version
   of the game.

NEAT FEATURES
 + Kevin Scroggins
    - I added full dynamic support for shaders. You can specfy a
      different shader for each object in Worldcraft, and this will cause the
      corresponding shader to be used to render the object. If you open
      "shader_demo.wrl", it has a brief demo of some simple shaders; namely a
      gradient, a green shader and a transluscent "toon" shader which doesn't
      quite work properly because we don't have lighting implmented yet.

    - Although this is not neccessarily a feature, I have a fullly interactive,
      relatively dynamic menu for easier user interaction.

 + Ian Doerr
    - Did not implement any neat features, ensured that all last-minute
      requirements were met.

NOTES
 + Made with Visual Studio 2008.

 + Requires an OpenGL 2.0 compliant video card for shader support (disables
   shaders automatically if they are not supported) and an OpenGL 1.5
   compliant video card for vertex buffers for the terrain (defaults to
   triangle strips if video card is incompatible).

 + Compile under RELEASE MODE ONLY if you need to re-compile our engine. The
   debug version of our engine renders bounding boxes for each object and runs
   slower.

 + Code isn't overly commented, if you have any questions or concernes please
   feel free to contact us.

 + Water has a special draw order sorting routine in World which swaps the draw
   order to account for proper transparency. It is not perfect and currently
   has a minor issue where water drawn behind a transparent sprite is not
   rendered. This would be fixed, but we did not have enough time to do so. All
   other known transparency cases are accounted for though.

 + Started a Waterfall abstraction, but did not have time to complete it. Would
   have set the texture coordinates to translate, fairly simple to do.

 + Terrain has the ability to have a tiled texture as well as any corresponding
   texture map - the texture is scaled to fit onto the terrain.

 + Micro height map is demonstrated in "assignment3.wrl", mini height map is
   demonstrated in "assignment4.wrl" and huge height map is demonstrated in
   "huge_terrain.wrl".

 + An option has been added to the menu to enable/disable culling since it has
   a tendancy to flicker, but works (for the most part).

 + Kevin Scroggins wrote two tools for compiling our maps and game data:
   - Texture Compiler
     + A program to compile a list of all of the textures, height maps,
       animation data and height map data which is read by both the builder and
       the the engine. The map builder pre-indexes all of the textures and
       animations to point to texture indexes rather than texture names. The
       texture list also now contains the full file name, so there is no more
       need for attempting to determine the file extension of the texture. It
       also does the same for height maps. When the engine reads the texture
       data file, it pre-loads all of the textures to the video card and
       assumes that any maps loaded will use textures contained in the file.
       This allows for quicker map load times and no duplicate loading of
       textures. This will be an issue for games with a LOT more textures, but
       for our purposes it is sufficient.

   - Map Builder 2.0
     + A re-written version of the map builder written entirely in Java. Runs
       MUCH faster than the original builder. Has had a lot of features and
       functionality added to it including external texture / animation /
       height map data, a command line parameter list interface, etc.

 + Kevin Scroggins wrote helper batch script files to make testing easier. Here
   is a summary of what each one does:
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

   - "Run Game.bat" - Runs the release executable from the "Game" directory,
                      essentially acts as a shortcut.

   - "Run Test.bat" - Copies and runs a temporary debug executable from the
                      "Game" directory.

   - "Wally.bat" - Executes Wally, nothing special - mostly just used as a
                   shortcut.

   - "Worldcraft.bat" - Executes Worldcraft (Valve Hammer Editor), nothing
                        special - mostly just used as a shortcut.

 + FGD file stored in "/Worldcraft/fgd/game/"

 + WAD file stored in "/Wally/"
