#include "Geometry.h"

Geometry::Geometry(Shader * externalShader) : shader(externalShader), transformation(NULL) {
	
}

Geometry::~Geometry() {
	for(unsigned int i=0;i<faces.size();i++) {
		delete faces.at(i);
	}
	if(transformation != NULL) { delete transformation; }
}

double Geometry::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - transformation->m41, 2) + pow(p.y - transformation->m42, 2) + pow(p.z - transformation->m43, 2) );
}

void Geometry::tick () {
	
}

void Geometry::draw () {
	shader->activate();
	glPushMatrix();
		Transformation & normal = transformation->normal();
		glMultMatrixd(normal);
		for(unsigned int i=0;i<faces.size();i++) {
			faces.at(i)->draw();
		}
	glPopMatrix(); 
	shader->deactivate();
}

void Geometry::import(ifstream & input, vector<Texture *> & textures) {
	char line[256];
	
	transformation = DualTransformation::import(input);
	
	//Input the faces.
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfFaces = atoi(line);
	for(int faceIndex=0;faceIndex<numberOfFaces;faceIndex++) {
		Face * face = new Face;
		face->import(input, textures);
		faces.push_back(face);
	}
}
