#include "Translator.h"

Translator::Translator() : shader(NULL), forward(true) { }

Translator::~Translator() {
	delete [] name;
	for(unsigned int i=0;i<faces.size();i++) {
		delete faces.at(i);
	}
}

double Translator::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - transformation->m41, 2) + pow(p.y - transformation->m42, 2) + pow(p.z - transformation->m43, 2) );
}

void Translator::update(double timeElapsed) {
	if(forward) {
		position += unitVector * (rateInMetersPerSecond * timeElapsed);
		distanceTravelled = sqrt(pow(position.x - origin.x, 2) + pow(position.y - origin.y, 2) + pow(position.z - origin.z, 2));
	}
	else {
		position -= unitVector * (rateInMetersPerSecond * timeElapsed);
		distanceTravelled = sqrt(pow(destination.x - position.x, 2) + pow(destination.y - position.y, 2) + pow(destination.z - position.z, 2));
	}

	if (forward) {
		box->setCenter(unitVector * distanceTravelled);
	} else {
		box->setCenter(unitVector * (distanceToTravel - distanceTravelled));
	}

	if(distanceTravelled >= distanceToTravel) {
		forward = !forward;
	}
}

void Translator::draw() {
	if(shader != NULL) { shader->activate(); }

	Point p = transformation->position();
	glPushMatrix();
		glTranslated(position.x, position.y, position.z);
		for(unsigned int i=0;i<faces.size();i++) {
			faces.at(i)->draw();
		}
	glPopMatrix();

	if(shader != NULL) { shader->deactivate(); }
}

void Translator::import (ifstream & input, vector<Texture *> & textures, vector<Shader *> shaders) {
	char line[256];
	char key[256];
	char value[256];
	char * str;
	
	transformation = DualTransformation::import(input);
	
	// input the properties
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
		
		// parse properties to local variables
		if(_stricmp(key, "name") == 0) {
			name = str;
		}
		else if(_stricmp(key, "offset") == 0) {
			double xPos, yPos, zPos;
			char * temp = strchr(str + sizeof(char), ' ');
			*temp = '\0';
			xPos = atof(str);
			temp += sizeof(char);
			char * temp2 = strchr(temp, ' ');
			*temp2 = '\0';
			yPos = atof(temp);
			temp2 += sizeof(char);
			zPos = atof(temp2);
			offset = Point(xPos, yPos, zPos);
			delete [] str;
		}
		else if(_stricmp(key, "rateInMetersPerSecond") == 0) {
			rateInMetersPerSecond = atof(str);
			delete [] str;
		}
		else if(_stricmp(key, "shader") == 0) {
			int shaderIndex = atoi(str);
			if(shaderIndex >= 0 && shaderIndex < (int) shaders.size()) { shader = shaders.at(shaderIndex); }
			delete [] str;
		}
		else {
			printf("WARNING: Encountered unexpected property when parsing translator object: \"%s\".\n", key);
			delete [] str;
		}
	}

	// input the bounding box
	box = BoundingBox::import(input);
	
	// input the faces
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfFaces = atoi(line);
	for(int faceIndex=0;faceIndex<numberOfFaces;faceIndex++) {
		Face * face = new Face;
		face->import(input, textures);
		faces.push_back(face);
	}
	
	origin = transformation->position();
	position = origin;
	destination = origin + offset;
	distanceToTravel = sqrt(pow(destination.x - origin.x, 2) + pow(destination.y - origin.y, 2) + pow(destination.z - origin.z, 2));
	unitVector = Point((destination.x - origin.x) / distanceToTravel, (destination.y - origin.y) / distanceToTravel, (destination.z - origin.z) / distanceToTravel);
	distanceTravelled = 0;
}
