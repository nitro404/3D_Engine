
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>    
#include <windows.h>
#include <commdlg.h>
#include <direct.h>
#include "fileDialog.h"
//#include "Variables.h"


//*****************************************************************************************//
//                                     File Dialogs                                        //
//*****************************************************************************************//


//*****************************************************************************************//
//                                       Private                                           //
//*****************************************************************************************//

//char lastMapDirectory [_MAX_DIR] = {'.','.','\\','M','A','P','S','\0'};
//char lastWorldDirectory [_MAX_DIR] = {'.','.','\\','W','O','R','L','D','S','\0'};
//char lastTextureDirectory [_MAX_DIR] = {'.','.','\\','T','E','X','T','U','R','E','S','\0'};
//char lastDirectory [_MAX_DIR] = {'.','.','\\','M','A','P','S','\0'};

char lastMapDirectory [_MAX_DIR] = {'.','.','\\','M','A','P','S','\0'};
char lastWorldDirectory [_MAX_DIR] = {'.','.','\\','M','A','P','S','\0'};
char lastTextureDirectory [_MAX_DIR] = {'.','.','\\','T','E','X','T','U','R','E','S','\0'};
char lastDirectory [_MAX_DIR] = {'.','.','\\','M','A','P','S','\0'};

char *directories [4] = {lastMapDirectory, lastWorldDirectory, lastTextureDirectory, lastDirectory};
char lastFilename [MAX_PATH]; 

/*
Variables * systemVariables = NULL;
char * lastMapDirectory;
char * lastWorldDirectory;
char * lastTextureDirectory;
char * lastDirectory;
bool systemVariablesRead = false;

char *directories [4];
char lastFilename [MAX_PATH];


void readSystemVariables() {
	if(!systemVariablesRead) {
		string tempstr;

		systemVariables = new Variables;
		systemVariables->parseFrom("settings.ini");
		
		tempstr.erase();
		tempstr.append(systemVariables->getValue("Relative Path"));
		tempstr.append(systemVariables->getValue("Map Directory"));
		lastMapDirectory = new char[(tempstr.length() + 1 > MAX_PATH) ? MAX_PATH : tempstr.length() + 1];
		strncpy(lastMapDirectory, tempstr.c_str(), MAX_PATH - 1);
		
		tempstr.erase();
		tempstr.append(systemVariables->getValue("Relative Path"));
		tempstr.append(systemVariables->getValue("Map Directory"));
		lastWorldDirectory = new char[(tempstr.length() + 1 > MAX_PATH) ? MAX_PATH : tempstr.length() + 1];
		strncpy(lastWorldDirectory, tempstr.c_str(), MAX_PATH - 1);
		
		tempstr.erase();
		tempstr.append(systemVariables->getValue("Relative Path"));
		tempstr.append(systemVariables->getValue("Texture Directory"));
		lastTextureDirectory = new char[(tempstr.length() + 1 > MAX_PATH) ? MAX_PATH : tempstr.length() + 1];
		strncpy(lastTextureDirectory, tempstr.c_str(), MAX_PATH - 1);
		
		tempstr.erase();
		tempstr.append(systemVariables->getValue("Relative Path"));
		tempstr.append(systemVariables->getValue("Texture Directory"));
		lastDirectory = new char[(tempstr.length() + 1 > MAX_PATH) ? MAX_PATH : tempstr.length() + 1];
		strncpy(lastDirectory, tempstr.c_str(), MAX_PATH - 1);
		
		directories[0] = new char[strlen(lastMapDirectory) + 1];
		strcpy(directories[0], lastMapDirectory);

		directories[1] = new char[strlen(lastWorldDirectory) + 1];
		strcpy(directories[1], lastWorldDirectory);

		directories[2] = new char[strlen(lastTextureDirectory) + 1];
		strcpy(directories[2], lastTextureDirectory);

		directories[3] = new char[strlen(lastDirectory) + 1];
		strcpy(directories[3], lastDirectory);
		
		systemVariablesRead = true;
	}
}
*/

void setupInternalFilter (char *internalFilter, char *userFilter) {
	//Filter format example: "File (*.bmp;*.tga)|*.bmp;*.tga|All (*.*)|*.*||"
	strcpy (internalFilter, userFilter);
	for (long index = strlen (internalFilter); index >= 0; index--) {
		if (internalFilter [index] == '|') internalFilter [index] = '\0';
	}
}

void setupQueryStructure (OPENFILENAME &queryStructure, char *filter, char *initialFileName) {
	static char filenameBuffer [MAX_PATH]; strcpy (filenameBuffer, initialFileName);
	static char fileFilter [1024]; setupInternalFilter (&fileFilter [0], filter);
	if (lastDirectory [0] == '\0') {_getcwd (lastDirectory, MAX_PATH);}

	queryStructure.lStructSize = sizeof (OPENFILENAME);     
	queryStructure.hwndOwner = NULL;     
	queryStructure.hInstance = GetModuleHandle (NULL);         
	queryStructure.lpstrFilter = fileFilter;     
	queryStructure.lpstrCustomFilter = NULL;     
	queryStructure.nMaxCustFilter = 0;    
	queryStructure.nFilterIndex = 0;     
	queryStructure.lpstrFile = filenameBuffer;     
	queryStructure.nMaxFile = MAX_PATH;    
	queryStructure.lpstrFileTitle = NULL;     
	queryStructure.nMaxFileTitle = MAX_PATH;     
	queryStructure.lpstrInitialDir = lastDirectory;    
	queryStructure.lpstrTitle = NULL;     
	queryStructure.Flags = OFN_NOCHANGEDIR;     
	queryStructure.nFileOffset = 0;     
	queryStructure.nFileExtension = 0;     
	queryStructure.lpstrDefExt = NULL;     
	queryStructure.lCustData = 0;     
	queryStructure.lpfnHook = NULL;     
	queryStructure.lpTemplateName = NULL; 
}

char *promptForReadFile (DirectoryChoice choice, char *filter, char *initialFileName = "") {
	useDirectory (choice);
	OPENFILENAME query; setupQueryStructure (query, filter, initialFileName);      
	if (GetOpenFileName (&query)) {         
		strcpy (lastFilename, query.lpstrFile);   
		memorizeDirectoryUsed (choice); 
		return lastFilename;
	}     
	return NULL;
}

char *promptForWriteFile (DirectoryChoice choice, char *filter, char *initialFileName = "") {
	useDirectory (choice);
	OPENFILENAME query; setupQueryStructure (query, filter, initialFileName);      
	if (GetSaveFileName (&query)) {       
		strcpy (lastFilename, query.lpstrFile);  
		memorizeDirectoryUsed (choice); 
		return lastFilename;
	}     
	return NULL;
}

//*****************************************************************************************//
//                                       Public                                            //
//*****************************************************************************************//

void useDirectory (DirectoryChoice choice) {
//	readSystemVariables();
	strcpy (lastDirectory, directories [choice]);
}
void memorizeDirectoryUsed (DirectoryChoice choice) {
//	readSystemVariables();
	strcpy (directories [choice], lastDirectory);
}

char *promptForUniRead () {return promptForReadFile (UniChoice, "Uni File (*.uni)|*.uni||");}
char *promptForUniWrite () {return promptForWriteFile (UniChoice, "Uni File (*.uni)|*.uni||");}

char *promptForWorldRead () {return promptForReadFile (WorldChoice, "World File (*.wrl)|*.wrl||");}
char *promptForWorldWrite () {return promptForWriteFile (WorldChoice, "World File (*.wrl)|*.wrl||");}

char *promptForTextureRead () {return promptForReadFile (TextureChoice, "Texture File (*.*)|*.*||");}
char *promptForTextureWrite () {return promptForWriteFile (TextureChoice, "Texture File (*.*)|*.*||");}

char *promptForAnyRead () {return promptForReadFile (AnyChoice, "File (*.*)|*.*||");}
char *promptForAnyWrite () {return promptForWriteFile (AnyChoice, "File (*.*)|*.*||");}