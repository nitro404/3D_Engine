#pragma once

#include "Includes.h"
#include "Object.h"
#include "AnimatedTexture.h"
#include "AnimatedFace.h"

class Pool : public Object {
public:
	Pool();
	virtual ~Pool();
	
	double distanceFrom(Point & p) const;
	bool insideOf(Point & p) const;
	
	virtual void tick();
	virtual void draw();
	virtual void draw(bool drawInside);
	
	virtual void import(ifstream &input, vector<AnimatedTexture *> & animatedTextures);
	
public:
	DualTransformation * transformation;
	vector<Face *> faces;
	Colour waterColour;

private:
	double maxX, maxY, maxZ;
	double minX, minY, minZ;
	char * name;
};
