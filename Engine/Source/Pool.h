#ifndef _POOL_H
#define _POOL_H

#include "Includes.h"
#include "Object.h"
#include "AnimatedTexture.h"
#include "AnimatedFace.h"

class Pool : public Object {
public:
	DualTransformation transformation;
	vector<Face *> faces;

	Pool() { }
	
	~Pool() {
		delete [] name;
		for(int i=0;i<faces.size();i++) {
			delete faces.at(i);
		}
	}
	
	double distanceFrom(Point & p) const;
	bool insideOf(Point & p) const;
	
	void tick();
	void draw();
	
	void import (ifstream &input, vector<AnimatedTexture *> & animatedTextures);
	
private:
	double maxX, maxY, maxZ;
	double minX, minY, minZ;
	char * name;
	Colour waterColour;
};

#endif