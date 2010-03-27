#pragma once

#include "Pool.h"

class Waterfall : public Pool {
public:
	Waterfall() { }
	~Waterfall() { }

	void tick();

	void import(ifstream & input, vector<AnimatedTexture *> & textures);

private:

};
