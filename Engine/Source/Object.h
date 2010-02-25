#ifndef _OBJECT_H
#define _OBJECT_H

#include "Includes.h"
#include "Colour.h"
#include "Point.h"
#include "Transformation.h"
#include "Camera.h"
#include "Texture.h"
#include "Face.h"

extern double DT;

class Object {
public:

	DualTransformation * transformation;

	Object() { transformation = NULL; } 
	virtual ~Object() { if (transformation != NULL) { delete transformation; } }
	
	virtual double distanceFrom(Point & p) const { return 0; }
	
	virtual void tick() { }
	virtual void draw() { }
	virtual void import(ifstream & input, vector<Texture *> & textures) { }
};

#endif
