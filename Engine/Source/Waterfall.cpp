#include "Waterfall.h"

void Waterfall::tick() {
	Pool::tick();

	for(unsigned int i=0;i<faces.size();i++) {
		faces.at(i)->tick();
	}
}

void Waterfall::import(ifstream & input, vector<AnimatedTexture *> & animatedTextures) {
	Pool::import(input, animatedTextures);

	for(unsigned int i=0;i<faces.size();i++) {
		faces.at(i)->translationSpeed = 0.1;
	}
}
