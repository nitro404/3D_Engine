#pragma once

#include "Includes.h"
#include "AnimatedTexture.h"
#include "Face.h"

extern double DT;

class AnimatedFace : public Face {
public:
	AnimatedTexture * animatedTexture;
	double animationSpeed;

	AnimatedFace() {
		textureIndex = 0;
		animatedTexture = NULL;
		animationSpeed = -1;
	}

	~AnimatedFace() { }

	void tick();
	void draw();
	void draw(Colour & colour);
	void import(ifstream & input, vector<AnimatedTexture * > & animatedTextures);
	
private:
	double textureIndex;
};
