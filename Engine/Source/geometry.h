#ifndef _GEOMETRY_H
#define _GEOMETRY_H 

#include "Includes.h"
#include "Object.h"

class Geometry : public Object {
public:
	DualTransformation transformation;
	vector<Face *> faces;
	
	Geometry() { }

	~Geometry() {
		for(int i=0;i<faces.size();i++) {
			delete faces.at(i);
		}
	}

	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import (ifstream &input, vector<Texture *> & textures);
};

#endif
