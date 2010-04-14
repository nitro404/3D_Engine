#pragma once

#include "Includes.h"
#include "Object.h"

class Vehicle : public Object {
public:
	Vehicle();
	~Vehicle();

	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import(ifstream &input, vector<Texture *> & textures);

public:
	DualTransformation * transformation;
	vector<Face *> faces;

private:
	char * name;
	int style; // 0 = car, 1 = bike, 2 = tank
};
