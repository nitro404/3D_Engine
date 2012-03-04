#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Includes.h"
#include "Object.h"

class Environment : public Object {
public:
	Environment();
	~Environment();

	double distanceFrom(Point & p) const { return 0; }

	void update(double timeElapsed);
	void draw();

	void import(ifstream &input, vector<Texture *> & textures, vector<Shader *> shaders);

public:
	Shader * shader;

private:
	GLuint skyboxList;
	char * name;
	Colour ambientLight;
	int cloudLayers;
	Texture ** skyboxTextures;
	Colour skyColour;
	int surrounds;
};

#endif // ENVIRONMENT_H
