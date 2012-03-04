#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Includes.h"
#include "Object.h"

class Geometry : public Object {
public:
	Geometry();
	~Geometry();

	double distanceFrom(Point & p) const;

	void update(double timeElapsed);
	void draw();

	void import (ifstream &input, vector<Texture *> & textures, vector<Shader *> shaders);

public:
	vector<Face *> faces;

	Shader * shader;
};

#endif // GEOMETRY_H
