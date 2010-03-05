#pragma once

#include "Includes.h"
#include "Object.h"

class Translator : public Object {
public:
	vector<Face *> faces;
	
	Translator() {
		forward = true;
	}

	~Translator() {
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
	Point origin;
	Point position;
	Point offset;
	Point destination;
	Vector unitVector;
	double rateInMetersPerSecond;
	bool forward;
	double distanceTravelled;
	double distanceToTravel;
};
