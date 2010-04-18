#include "Waterfall.h"

Waterfall::Waterfall() { }
Waterfall::~Waterfall() { }

void Waterfall::tick() {
	Pool::tick();

	for(unsigned int i=0;i<faces.size();i++) {
		faces.at(i)->tick();
	}
}

void Waterfall::import(ifstream & input, vector<AnimatedTexture *> & animatedTextures, vector<Shader *> shaders) {
	Pool::import(input, animatedTextures, shaders);

	for(unsigned int i=0;i<faces.size();i++) {
		faces.at(i)->translationSpeed = 0.1;
	}
}
