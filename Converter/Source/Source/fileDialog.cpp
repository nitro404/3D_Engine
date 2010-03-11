
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"
#include <commdlg.h>

//*****************************************************************************************//
//                                     File Dialogs                                        //
//*****************************************************************************************//


//*****************************************************************************************//
//                                       Private                                           //
//*****************************************************************************************//

#define directorySize (_MAX_PATH + 30) //Extra room for network names...

/*
char lastMapDirectory [directorySize] = {'.','\0'};
char lastWorldDirectory [directorySize] = {'.','\0'};
char lastTextureDirectory [directorySize] = {'.','\0'};
char lastDirectory [directorySize] = {'\0'};
*/

char lastMapDirectory [_MAX_DIR] = {'.','.','\\','.','.','\\','.','.','\\','M','A','P','S','\0'};
char lastWorldDirectory [_MAX_DIR] = {'.','.','\\','.','.','\\','.','.','\\','M','A','P','S','\0'};
//char lastTextureDirectory [_MAX_DIR] = {'.','.','\\','.','.','\\','.','.','\\','T','E','X','T','U','R','E','S','\0'};
char lastTextureDirectory [_MAX_DIR] = {'.','.','\\','.','.','\\','.','.','\\','C','o','n','t','e','n','t','\\','D','a','t','a','\\','T','E','X','T','U','R','E','S','\0'};
char lastDirectory [_MAX_DIR] = {'.','.','\\','.','.','\\','.','.','\\','M','A','P','S','\0'};

char lastFilename [directorySize]; 
char lastFileExtension [directorySize]; 

void memorizeDirectoryUsed (char *filename) {
	for (long index = strlen (filename) - 1; index > 0 && filename [index] != '\\' && filename [index] != '/' && filename [index] != ':'; index--) {
	}
	//assert index > 0 because at least one occurrence of : will be encountered...
	CopyMemory (lastDirectory, filename, index); lastDirectory [index] = '\0';
}

void memorizeExtensionUsed (char *filename) {
	for (long index = strlen (filename) - 1; index > 0 && filename [index] != '\\' && filename [index] != '/' && filename [index] != ':' && filename [index] != '.'; index--) {
	}
	//assert index > 0 because at least one occurrence of : will be encountered...
	if (filename [index] == '.')
		strcpy (lastFileExtension, &filename [index + 1]);
	else
		lastFileExtension [0] = '\0'; //There was no extension...
}

void memorizeFilenameUsed (char *filename) {
	strcpy (lastFilename, filename); 
	memorizeDirectoryUsed (filename);
	memorizeExtensionUsed (filename);
}

char *directories [4] = {lastMapDirectory, lastWorldDirectory, lastTextureDirectory, lastDirectory};

void setupInternalFilter (char *internalFilter, char *userFilter) {
	//Filter format example: "File (*.bmp;*.tga)|*.bmp;*.tga|All (*.*)|*.*||"
	strcpy (internalFilter, userFilter);
	for (long index = strlen (internalFilter); index >= 0; index--) {
		if (internalFilter [index] == '|') internalFilter [index] = '\0';
	}
}

void setupQueryStructure (OPENFILENAME &queryStructure, char *filter, char *initialFileName) {
	//Warning: Static variable needed to be able to access values after this routine returns...
	const bufferSize = MAX_PATH * 1000; //Large number of multiple-selected files...
	static char filenameBuffer [bufferSize]; strcpy (filenameBuffer, initialFileName);
	static char fileFilter [1024]; setupInternalFilter (fileFilter, filter);
	if (lastDirectory [0] == '\0') {_getcwd (lastDirectory, MAX_PATH);}

	queryStructure.lStructSize = sizeof (OPENFILENAME);     
	queryStructure.hwndOwner = NULL;     
	queryStructure.hInstance = GetModuleHandle (NULL);         
	queryStructure.lpstrFilter = fileFilter;     
	queryStructure.lpstrCustomFilter = NULL;     
	queryStructure.nMaxCustFilter = 0;    
	queryStructure.nFilterIndex = 1;     
	queryStructure.lpstrFile = filenameBuffer;     //directory name1 name2 name3...
	queryStructure.nMaxFile = bufferSize;    
	queryStructure.lpstrFileTitle = NULL;     
	queryStructure.nMaxFileTitle = MAX_PATH;     
	queryStructure.lpstrInitialDir = lastDirectory;    
	queryStructure.lpstrTitle = NULL;     
	queryStructure.Flags = OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_LONGNAMES | OFN_ALLOWMULTISELECT; //For write, OFN_OVERWRITEPROMPT
	queryStructure.nFileOffset = 0;     
	queryStructure.nFileExtension = 0;     
	queryStructure.lpstrDefExt = NULL;     
	queryStructure.lCustData = 0;     
	queryStructure.lpfnHook = NULL;     
	queryStructure.lpTemplateName = NULL;     
}

char *promptForReadFile (DirectoryChoice choice, char *filter, char *initialFileName) {
	useDirectory (choice);
	OPENFILENAME query; setupQueryStructure (query, filter, initialFileName);      
	query.lpstrTitle = "File(s) to convert...";
	if (GetOpenFileName (&query)) {   
		memorizeFilenameUsed (query.lpstrFile);
		memorizeDirectoryUsed (choice); 
		return query.lpstrFile; //Multiple null terminated strings if using multiple-selection...
	}     
	return NULL;
}

char *promptForWriteFile (DirectoryChoice choice, char *filter, char *initialFileName) {
	useDirectory (choice);
	OPENFILENAME query; setupQueryStructure (query, filter, initialFileName);      
	if (GetSaveFileName (&query)) {       
		memorizeFilenameUsed (query.lpstrFile);
		memorizeDirectoryUsed (choice); 
		return &lastFilename [0];
	}     
	return NULL;
}

//*****************************************************************************************//
//                                       Public                                            //
//*****************************************************************************************//

void useDirectory (DirectoryChoice choice) {
	strcpy (lastDirectory, directories [choice]);
}
void memorizeDirectoryUsed (DirectoryChoice choice) {
	//::log ("\nMemorizing Directory used...\n\tlastDirectory is \"%s\"...", lastDirectory);
	//::log ("\n\tCopied to choice %d", choice);
	strcpy (directories [choice], lastDirectory);
	//::log ("\n\tWhich now contains \"%s\"...", directories [choice]);
}

char *promptForMapRead () {return promptForReadFile (MapChoice, "File (*.map;*.3dm;*.xsi;*.mesh;*.uni)|*.map;*.3dm;*.xsi;*.mesh;*.uni||");}
char *promptForMapWrite () {return promptForWriteFile (MapChoice, "File (*.map)|*.map||");}

char *promptForWorldRead () {return promptForReadFile (WorldChoice, "Universal File (*.uni)|*.uni||");}
char *promptForWorldWrite () {return promptForWriteFile (WorldChoice, "Universal File (*.uni)|*.uni||");}

char *promptForTextureRead () {return promptForReadFile (TextureChoice, "Texture File (*.*)|*.*||");}
char *promptForTextureWrite () {return promptForWriteFile (TextureChoice, "Texture File (*.*)|*.*||");}

char *promptForAnyRead () {return promptForReadFile (AnyChoice, "File (*.*)|*.*||");}
char *promptForAnyWrite () {return promptForWriteFile (AnyChoice, "File (*.*)|*.*||");}

char *promptForTexture (char *textureName) {
	DirectoryChoice choice = TextureChoice; char *filter = "Texture File (*.*)|*.*||";
	useDirectory (choice);
	OPENFILENAME query; setupQueryStructure (query, filter, "");
	char title [500];
	strcpy (title, "Locate texture file \"");
	strcat (title, textureName); 
	strcat (title, "\"...");
	query.lpstrTitle = title;
	if (GetOpenFileName (&query)) {         
		memorizeFilenameUsed (query.lpstrFile);
		memorizeDirectoryUsed (choice); 
		return &lastFilename [0];
	}     
	return NULL;
}

char *textureDirectory () {return lastTextureDirectory;}

char startupDirectoryName [directorySize];
const long textureSearchDirectoriesLimit = 256;
char **textureSearchDirectories; long textureSearchDirectoriesSize = 0;


void setupStartupDirectory () {
	_fullpath (startupDirectoryName, ".", directorySize);
	textureSearchDirectories = new char * [textureSearchDirectoriesLimit];
}

void addToTextureSearchPaths (char *directory) {
	if (*directory == '\0') return; //Ignore empty directories...
	for (long directoryIndex = 0; directoryIndex < textureSearchDirectoriesSize; directoryIndex++) {
		//strcpy (fileName, textureDirectory ());
		if (strcmpi (directory, textureSearchDirectories [directoryIndex]) == 0) return; //Already there...
	}

	if (textureSearchDirectoriesSize >= textureSearchDirectoriesLimit) {
		prompt ("\nYou've got to be kidding... You need more than %d texture directories... QUITTING...", textureSearchDirectoriesLimit);
		exit (0);
	}

	textureSearchDirectories [textureSearchDirectoriesSize] = new char [strlen (directory) + 1];
	strcpy (textureSearchDirectories [textureSearchDirectoriesSize], directory);
	textureSearchDirectoriesSize++;
}

void setupTextureSearchPaths () {
	char line [256]; //Working variable...
	::ifstream input; input.open ("textureDirectories"); 
	if (input.bad ()) {prompt ("Unable to open \"textureDirectories\" file which \nshould refer to one or more texture directories."); return;}
	while (input.peek () != EOF) {
		input.getline (line, 256, '\n');
		addToTextureSearchPaths (line);
	}
	input.close ();
	if (textureSearchDirectoriesSize == 0) {
		prompt ("\nIn order to run, you need to add the full path of each of your texture directories"
			"\nto the text file called \"textureDirectories\" in the Universal Converter directory;"
			"\nExample: add something like \"C:\\3d\\Student\\textures\" to the file.");
	}
}


char *fgdFilePath = NULL;

void setupFGDFilePath () {
	char line [256]; //Working variable...
	::ifstream input; input.open ("fgdFilePath"); 
	if (input.bad ()) {prompt ("Unable to open \"fgdFilePath\" file which \nshould refer to the Worldcraft FGD file."); return;}
	while (input.peek () != EOF) {
		input.getline (line, 256, '\n');
		fgdFilePath = new char [strlen (line) + 1];
		strcpy (fgdFilePath, line);
		break; //Keep loop structure but loop only once for better contrast with above method...
	}
	if (fgdFilePath == NULL) {
		prompt ("\nIn order to run, you need to add the full path of the worldcraft fgd file"
			"\nto the text file called \"fgdFilePath\" in the Universal Converter directory;"
			"\nExample: add something like \"C:\\3d\\Student\\StudentFGDandWAD\\student.fgd\" to the file.");
	}
	input.close ();
}
