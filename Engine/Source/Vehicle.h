#ifndef VEHICLE_H
#define VEHICLE_H

#include "Includes.h"
#include "Object.h"

class Vehicle : public Object {
public:
	Vehicle();
	~Vehicle();

	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import(ifstream &input, vector<Texture *> & textures, vector<Shader *> shaders);

public:
	vector<Face *> faces;
	Shader * shader;

private:
	char * name;
	int style; // 0 = car, 1 = bike, 2 = tank
};

#endif // VEHICLE_H
