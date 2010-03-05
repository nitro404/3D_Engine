#pragma once

#include "Includes.h"
#include "Object.h"

class Rotator : public Object {
public:
	vector<Face *> faces;
	
	Rotator() {
		angleInDegrees = 0;
		rateInDegreesPersecond = 0;
	}
	
	~Rotator() {
		delete [] name;
		for(unsigned int i=0;i<faces.size();i++) {
			delete faces.at(i);
		}
	}
	
	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import(ifstream & input, vector<Texture *> & textures);
	
private:
	char * name;
	Point axis;
	double rateInDegreesPersecond;
	double angleInDegrees;
};
