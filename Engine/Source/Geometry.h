#ifndef _GEOMETRY_H
#define _GEOMETRY_H 

#include "Includes.h"
#include "Object.h"

class Geometry : public Object {
public:
	vector<Face *> faces;
	
	Geometry() { }

	~Geometry() {
		for(unsigned int i=0;i<faces.size();i++) {
			delete faces.at(i);
		}
	}

	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import (ifstream &input, vector<Texture *> & textures);
};

#endif
