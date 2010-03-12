#pragma once

#include "Includes.h"
#include "Object.h"
#include "AnimatedTexture.h"
#include "AnimatedFace.h"

class Pool : public Object {
public:
	Pool();
	~Pool();
	
	double distanceFrom(Point & p) const;
	bool insideOf(Point & p) const;
	
	void tick();
	void draw();
	
	void import (ifstream &input, vector<AnimatedTexture *> & animatedTextures);
	
public:
	DualTransformation * transformation;
	vector<Face *> faces;

private:
	double maxX, maxY, maxZ;
	double minX, minY, minZ;
	char * name;
	Colour waterColour;
};
