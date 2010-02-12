#include "includes.all"

void AnimatedTexture::import (ifstream &input, TextureCollection & worldTextures) {
	char line [256];
	
	//Input the header.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	int currentIndex = atoi(line);
	CLEAR_THE_LINE;

	//Input the properties
	int firstTexture;
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	int propertiesSize = atoi(line);
	CLEAR_THE_LINE;
	for(int propertiesIndex = 0; propertiesIndex < propertiesSize; propertiesIndex++) {
		SKIP_TO_ENDLINE;
		char key [256]; char value [256]; value [0] = '\0';
		sscanf (line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
		convertToLowercase (key);
		char *str = new char [strlen (value) + 1]; strcpy (str, value);
		
		//Parse properties to local variables
		if(stricmp(key, "name") == 0) {
			name = str;
		}
		else if(stricmp(key, "firsttexture") == 0) {
			firstTexture = atoi(str);
			delete [] str;
		}
		else if(stricmp(key, "frames") == 0) {
			frames = atoi(str);
			delete [] str;
		}
		else if(stricmp(key, "speed") == 0) {
			speed = atoi(str);
			delete [] str;
		}
	}
	
	for(int i=firstTexture;i<firstTexture+frames;i++) {
		textures.push_back(worldTextures.at(i));
	}
}
