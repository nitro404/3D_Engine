#pragma once

#include "Includes.h"
#include "Object.h"

class Rotator : public Object {
public:
	Rotator();
	
	~Rotator();
	
	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import(ifstream & input, vector<Texture *> & textures, vector<Shader *> shaders);

public:
	DualTransformation * transformation;
	vector<Face *> faces;
	Shader * shader;
	
private:
	char * name;
	Point axis;
	double rateInDegreesPerSecond;
	double angleInDegrees;
};
