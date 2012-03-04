#ifndef OBJECT_H
#define OBJECT_H

#include "Includes.h"
#include "Colour.h"
#include "Point.h"
#include "Transformation.h"
#include "Camera.h"
#include "Face.h"
#include "BoundingBox.h"

class Object {
public:
	Object();
	virtual ~Object();
	
	virtual double distanceFrom(Point & p) const { return 0; }
	
	virtual void update(double timeElapsed);
	virtual void draw();

	virtual void drawWithCull();
	virtual void drawBoundingBox();

	virtual void import(ifstream & input, vector<Texture *> & textures);

public:
	DualTransformation * transformation;

protected:
	BoundingBox * box;
};

#endif // OBJECT_H
