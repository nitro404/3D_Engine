#include "AnimatedTexture.h"

void AnimatedTexture::import (ifstream & input, vector<Texture *> & worldTextures) {
	char * line;
	char * key;
	char * value;
	char * str;
	line = new char[256];
	key = new char[256];
	value = new char[256];
	
	//Input the header.
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int currentIndex = atoi(line);
	input.getline(line, 256, '\n');

	//Input the properties
	int firstTexture;
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfProperties = atoi(line);
	input.getline(line, 256, '\n');
	for(int propertyIndex=0;propertyIndex<numberOfProperties;propertyIndex++) {
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf_s(line, " \"%[^\"]\" => \"%[^\"]\"", key, 256, value, 256);
		str = new char[strlen(value) + 1];
		strcpy_s(str, strlen(value) + 1, value);
		
		//Parse properties to local variables
		if(_stricmp(key, "name") == 0) {
			name = str;
		}
		else if(_stricmp(key, "firsttexture") == 0) {
			firstTexture = atoi(str);
			delete [] str;
		}
		else if(_stricmp(key, "frames") == 0) {
			frames = atoi(str);
			delete [] str;
		}
		else if(_stricmp(key, "speed") == 0) {
			speed = atoi(str);
			delete [] str;
		}
		else {
			delete [] str;
		}
	}
	
	for(int i=firstTexture;i<firstTexture+frames;i++) {
		textures.push_back(worldTextures.at(i));
	}
	
	delete [] line;
	delete [] key;
	delete [] value;
}
