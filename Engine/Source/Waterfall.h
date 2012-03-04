#ifndef WATERFALL_H
#define WATERFALL_H

#include "Pool.h"

class Waterfall : public Pool {
public:
	Waterfall();
	~Waterfall();

	void update(double timeElapsed);

	void import(ifstream & input, vector<AnimatedTexture *> & textures, vector<Shader *> shaders);

private:

};

#endif // WATERFALL_H
