#pragma once

#include "Includes.h"
#include "Object.h"

class Rotator : public Object {
public:
	Rotator() : transformation(NULL), angleInDegrees(0), rateInDegreesPerSecond(0) { }
	
	~Rotator() {
		delete [] name;
		for(unsigned int i=0;i<faces.size();i++) {
			delete faces.at(i);
		}
		if(transformation != NULL) { delete transformation; }
	}
	
	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import(ifstream & input, vector<Texture *> & textures);

public:
	DualTransformation * transformation;
	vector<Face *> faces;
	
private:
	char * name;
	Point axis;
	double rateInDegreesPerSecond;
	double angleInDegrees;
};
