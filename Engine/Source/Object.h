#pragma once

#include "Includes.h"
#include "Colour.h"
#include "Point.h"
#include "Transformation.h"
#include "Camera.h"
#include "Face.h"

extern double DT;

class Object {
public:
	Object() { } 
	virtual ~Object() { }
	
	virtual double distanceFrom(Point & p) const { return 0; }
	
	virtual void tick() { }
	virtual void draw() { }

	virtual void import(ifstream & input, vector<Texture *> & textures) { }
};
