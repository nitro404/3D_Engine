#include "Translator.h"

double Translator::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - transformation.m41, 2) + pow(p.y - transformation.m42, 2) + pow(p.z - transformation.m43, 2) );
}

void Translator::tick() {
	if(forward) {
		position += unitVector * (rateInMetersPerSecond * DT);
		distanceTravelled = sqrt(pow(position.x - origin.x, 2) + pow(position.y - origin.y, 2) + pow(position.z - origin.z, 2));
	}
	else {
		position -= unitVector * (rateInMetersPerSecond * DT);
		distanceTravelled = sqrt(pow(destination.x - position.x, 2) + pow(destination.y - position.y, 2) + pow(destination.z - position.z, 2));
	}
	if(distanceTravelled >= distanceToTravel) {
		forward = !forward;
	}
}

void Translator::draw() {
	//Draw the faces in this object.
	Point p = transformation.position();
	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	for(int i=0;i<faces.size();i++) {
		faces.at(i)->draw();
	}
	glPopMatrix();
}

void Translator::import (ifstream & input, vector<Texture *> & textures) {
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
		else if(stricmp(key, "offset") == 0) {
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
		else if(stricmp(key, "rateInMetersPerSecond") == 0) {
			rateInMetersPerSecond = atof(str);
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
	
	origin = transformation.position();
	position = origin;
	destination = origin + offset;
	distanceToTravel = sqrt(pow(destination.x - origin.x, 2) + pow(destination.y - origin.y, 2) + pow(destination.z - origin.z, 2));
	unitVector = Vector((destination.x - origin.x) / distanceToTravel, (destination.y - origin.y) / distanceToTravel, (destination.z - origin.z) / distanceToTravel);
	distanceTravelled = 0;
	
	delete [] line;
	delete [] key;
	delete [] value;
}
