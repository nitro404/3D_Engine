#include "Rotator.h"

Rotator::Rotator() : shader(NULL), angleInDegrees(0), rateInDegreesPerSecond(0) { }

Rotator::~Rotator() {
	delete [] name;
	for(unsigned int i=0;i<faces.size();i++) {
		delete faces.at(i);
	}
	box = origPointer;
}

double Rotator::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - transformation->m41, 2) + pow(p.y - transformation->m42, 2) + pow(p.z - transformation->m43, 2) );
}

void Rotator::update(double timeElapsed) {
	angleInDegrees += rateInDegreesPerSecond * timeElapsed;
	if(angleInDegrees >= 360 || angleInDegrees <= -360) {
		angleInDegrees = 0;
	}

	curBox = *origPointer;
	curBox.expandForRotation();
	//curBox.rotateBy(angleInDegrees, axis);
}

void Rotator::draw() {
	if(shader != NULL) { shader->activate(); }

	Point p = transformation->position();
	glPushMatrix(); 
		glTranslated(p.x, p.y, p.z);
		glRotated(angleInDegrees, axis.x, axis.y, axis.z);
		for(unsigned int i=0;i<faces.size();i++) {
			faces.at(i)->draw();
		}
	glPopMatrix();

	if(shader != NULL) { shader->deactivate(); }
}

void Rotator::import(ifstream & input, vector<Texture *> & textures, vector<Shader *> shaders) {
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
		else if(_stricmp(key, "axis") == 0) {
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
			axis = Point(xPos, yPos, zPos);
			delete [] str;
		}
		else if(_stricmp(key, "rateInDegreesPersecond") == 0) {
			rateInDegreesPerSecond = atof(str);
			delete [] str;
		}
		else if(_stricmp(key, "shader") == 0) {
			int shaderIndex = atoi(str);
			if(shaderIndex >= 0 && shaderIndex < (int) shaders.size()) { shader = shaders.at(shaderIndex); }
			delete [] str;
		}
		else {
			printf("WARNING: Encountered unexpected property when parsing rotator object: \"%s\".\n", key);
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
		faces.push_back (face);
	}

	origPointer = box;
	box = &curBox;

}
