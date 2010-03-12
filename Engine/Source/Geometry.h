#pragma once

#include "Includes.h"
#include "Object.h"

class Geometry : public Object {
public:
	Geometry() : transformation(NULL) { }

	~Geometry() {
		for(unsigned int i=0;i<faces.size();i++) {
			delete faces.at(i);
		}
		if(transformation != NULL) { delete transformation; }
	}

	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import (ifstream &input, vector<Texture *> & textures);

public:
	DualTransformation * transformation;
	vector<Face *> faces;
};
