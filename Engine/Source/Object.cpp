#include "Object.h"

Object::Object() {
	transformation = NULL;
}

Object::~Object() {
	if (transformation != NULL) delete transformation;
}

void Object::draw() {
	
}

void Object::import(ifstream & input, vector<Texture *> & textures) {
	
}