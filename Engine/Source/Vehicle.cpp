#include "Vehicle.h"

Vehicle::Vehicle() : transformation(NULL), shader(NULL) { }

Vehicle::~Vehicle() {
	delete [] name;
	for(unsigned int i=0;i<faces.size();i++) {
		delete faces.at(i);
	}
	if(transformation != NULL) { delete transformation; }
}

double Vehicle::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - transformation->m41, 2) + pow(p.y - transformation->m42, 2) + pow(p.z - transformation->m43, 2) );
}

void Vehicle::tick() {
	
}

void Vehicle::draw() {
	if(shader != NULL) { shader->activate(); }

	if(faces.size() > 0) {
		glPushMatrix();
		Transformation & normal = transformation->normal();
		glMultMatrixd(normal);
		for(unsigned int i=0;i<faces.size();i++) {
			faces.at(i)->draw();
		}
		glPopMatrix(); 
	}

	if(shader != NULL) { shader->deactivate(); }
}

void Vehicle::import(ifstream & input, vector<Texture *> & textures, vector<Shader *> shaders) {
	char line[256];
	char key[256];
	char value[256];
	char * str;
	
	transformation = DualTransformation::import(input);
	
	//Input the properties
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfProperties = atoi(line);
	input.getline(line, 256, '\n');
	for(int propertyIndex=0;propertyIndex<numberOfProperties;propertyIndex++) {
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf_s(line, " \"%[^\"]\" => \"%[^\"]\"", key, 256, value, 256);
		str = new char[strlen(value) + 1];
		strcpy_s(str, strlen(value) + 1, value);

		//Parse properties to local variables
		if(_stricmp(key, "name") == 0) {
			name = str;
		}
		else if(_stricmp(key, "style") == 0) {
			style = atoi(str);
			delete [] str;
		}
		else if(_stricmp(key, "shader") == 0) {
			int shaderIndex = atoi(str);
			if(shaderIndex >= 0) { shader = shaders.at(shaderIndex); }
			delete [] str;
		}
		else {
			printf("WARNING: Encountered unexpected property when parsing vehicle object: \"%s\".\n", key);
			delete [] str;
		}
	}
	
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
