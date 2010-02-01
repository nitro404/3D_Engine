//*****************************************************************************************//
//                                      File Dialog                                        //
//*****************************************************************************************//

#ifndef fileDialogModule
#define fileDialogModule 

enum DirectoryChoice {MapChoice, WorldChoice, TextureChoice, AnyChoice};

void useDirectory (DirectoryChoice choice);
void memorizeDirectoryUsed (DirectoryChoice choice);

char *promptForMapRead ();
char *promptForMapWrite ();

char *promptForWorldRead ();
char *promptForWorldWrite ();

char *promptForTextureRead ();
char *promptForTextureWrite ();

char *promptForAnyRead ();
char *promptForAnyWrite ();

//Recent additions...
char *promptForTexture (char *textureName);
char *textureDirectory ();
char *promptForReadFile (DirectoryChoice choice, char *filter, char *initialFileName = "");
char *promptForWriteFile (DirectoryChoice choice, char *filter, char *initialFileName = "");
void setupStartupDirectory ();
void setupTextureSearchPaths ();
void setupFGDFilePath ();
void addToTextureSearchPaths (char *directory);

#endif