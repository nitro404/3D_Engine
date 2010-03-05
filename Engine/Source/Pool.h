#pragma once

#include "Includes.h"
#include "Object.h"
#include "AnimatedTexture.h"
#include "AnimatedFace.h"

class Pool : public Object {
public:
	vector<Face *> faces;

	Pool() { }
	
	~Pool() {
		delete [] name;
		for(unsigned int i=0;i<faces.size();i++) {
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
