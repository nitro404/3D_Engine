#pragma once

#include "Includes.h"
#include "Texture.h"

class AnimatedTexture {
public:
	char * name;
	int frames;
	int speed;
	vector<Texture *> textures;

	AnimatedTexture() { }

	~AnimatedTexture() {
		delete [] name;
	}
	
	void import(ifstream &input, vector<Texture *> & worldTextures);
};
