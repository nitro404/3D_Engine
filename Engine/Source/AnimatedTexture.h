#ifndef ANIMATED_TEXTURE_H
#define ANIMATED_TEXTURE_H

#include "Includes.h"
#include "Texture.h"

class AnimatedTexture {
public:
	AnimatedTexture() { }

	~AnimatedTexture() {
		delete [] name;
	}
	
	void import(ifstream &input, vector<Texture *> & worldTextures);

public:
	vector<Texture *> textures;
	int frames;
	int speed;

private:
	char * name;
};

#endif // ANIMATED_TEXTURE_H
