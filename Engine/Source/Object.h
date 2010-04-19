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
	Object() { box = NULL; } 
	virtual ~Object() { if (box != NULL) delete box; }
	
	virtual double distanceFrom(Point & p) const { return 0; }
	
	virtual void tick() { }
	virtual void draw() { }

	virtual void drawWithCull() {
		if (box != NULL) {
			if (camera->manager.shouldCull(*box)) {
				return;
			}
		}
		draw();
	}

	virtual void import(ifstream & input, vector<Texture *> & textures) { }
protected:
	BoundingBox* box;
};
