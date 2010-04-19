#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Object.h"

class Geometry : public Object {
public:
	Geometry();
	~Geometry();

	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import (ifstream &input, vector<Texture *> & textures, vector<Shader *> shaders);

public:
	vector<Face *> faces;

	Shader * shader;
};
