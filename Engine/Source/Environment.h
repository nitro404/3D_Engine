#pragma once

#include "Includes.h"
#include "Object.h"

class Environment : public Object {
public:
	Environment();
	~Environment();

	double distanceFrom(Point & p) const { return 0; }

	void tick();
	void draw();

	void import(ifstream &input, vector<Texture *> & textures, vector<Shader *> shaders);

private:
	GLuint skyboxList;
	char * name;
	Colour ambientLight;
	int cloudLayers;
	Texture ** skyboxTextures;
	Colour skyColour;
	int surrounds;
};
