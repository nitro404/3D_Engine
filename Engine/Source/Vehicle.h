#pragma once

#include "Includes.h"
#include "Object.h"

class Vehicle : public Object {
public:
	vector<Face *> faces;
	
	Vehicle() { }

	~Vehicle() {
		delete [] name;
		for(unsigned int i=0;i<faces.size();i++) {
			delete faces.at(i);
		}
	}

	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import(ifstream &input, vector<Texture *> & textures);
private:
	char * name;
	int style; // 0 = car, 1 = bike, 2 = tank
};
