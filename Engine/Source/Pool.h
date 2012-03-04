#ifndef POOL_H
#define POOL_H

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
	virtual void draw(bool drawInside = false);
	
	virtual void import(ifstream &input, vector<AnimatedTexture *> & animatedTextures, vector<Shader *> shaders);
	
public:
	vector<Face *> faces;
	Colour waterColour;
	Shader * shader;

private:
	double maxX, maxY, maxZ;
	double minX, minY, minZ;
	char * name;
};

#endif // POOL_H
