#pragma once

#include "Includes.h"
#include "Object.h"

class Translator : public Object {
public:	
	Translator() : transformation(NULL), forward(true) { }

	~Translator() {
		delete [] name;
		for(unsigned int i=0;i<faces.size();i++) {
			delete faces.at(i);
		}
		delete transformation;
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
