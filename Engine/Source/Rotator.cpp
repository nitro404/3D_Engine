#include "Rotator.h"

double Rotator::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - transformation.m41, 2) + pow(p.y - transformation.m42, 2) + pow(p.z - transformation.m43, 2) );
}

void Rotator::tick () {
	angleInDegrees += rateInDegreesPersecond * DT;
	if(angleInDegrees >= 360 || angleInDegrees <= -360) {
		angleInDegrees = 0;
	}
}

void Rotator::draw () {
	Point p = transformation.position();
	glPushMatrix(); 
	glTranslated(p.x, p.y, p.z);
	glRotated(angleInDegrees, axis.x, axis.y, axis.z);
	for(int i=0;i<faces.size();i++) {
		faces.at(i)->draw();
	}
	glPopMatrix ();
}

void Rotator::import (ifstream & input, vector<Texture *> & textures) {
	char * line;
	char * key;
	char * value;
	char * str;
	line = new char[256];
	key = new char[256];
	value = new char[256];

	transformation.import(input);
	
	//Input the properties
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfProperties = atoi(line);
	input.getline(line, 256, '\n');
	for(int propertyIndex=0;propertyIndex<numberOfProperties;propertyIndex++) {
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf(line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
		str = new char[strlen(value) + 1];
		strcpy(str, value);
		
		//Parse properties to local variables
		if(stricmp(key, "name") == 0) {
			name = str;
		}
		else if(stricmp(key, "axis") == 0) {
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
		else if(stricmp(key, "rateInDegreesPersecond") == 0) {
			rateInDegreesPersecond = atof(str);
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
		faces.push_back (face);
	}
	
	delete [] line;
	delete [] key;
	delete [] value;
}
