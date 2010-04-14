#pragma once

#include "Includes.h"
#include "Variables.h"
#include "Object.h"

class Geometry : public Object {
public:
	Geometry(Shader * externalShader);
	~Geometry();

	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import (ifstream &input, vector<Texture *> & textures);

public:
	DualTransformation * transformation;
	vector<Face *> faces;

	Shader * shader;
};
