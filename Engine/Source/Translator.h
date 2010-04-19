#pragma once

#include "Includes.h"
#include "Object.h"

class Translator : public Object {
public:	
	Translator();
	~Translator();

	double distanceFrom(Point & p) const;
	
	void tick();
	void draw();

	void import(ifstream & input, vector<Texture *> & textures, vector<Shader *> shaders);

public:
	vector<Face *> faces;
	Shader * shader;

private:
	char * name;
	Point origin;
	Point position;
	Point offset;
	Point destination;
	Point unitVector;
	double rateInMetersPerSecond;
	bool forward;
	double distanceTravelled;
	double distanceToTravel;
};
