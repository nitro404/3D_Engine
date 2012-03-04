#ifndef ANIMATED_FACE_H
#define ANIMATED_FACE_H

#include "Includes.h"
#include "AnimatedTexture.h"
#include "Face.h"

extern double DT;

class AnimatedFace : public Face {
public:
	AnimatedFace() : textureIndex(0), animatedTexture(NULL), animationSpeed(-1) { }

	~AnimatedFace() { }

	void tick();
	void draw();
	void draw(Colour & colour);
	void draw(Colour & colour, bool drawBothSides);
	void import(ifstream & input, vector<AnimatedTexture * > & animatedTextures);

public:
	AnimatedTexture * animatedTexture;
	double animationSpeed;

private:
	double textureIndex;
};

#endif // ANIMATED_FACE_H
