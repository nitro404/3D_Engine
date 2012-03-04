#include "Waypoint.h"

Waypoint::Waypoint() { }

Waypoint::~Waypoint() {
	delete [] name;
	delete [] neighbourValues;
}

double Waypoint::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - position.x, 2) + pow(p.y - position.y, 2) + pow(p.z - position.z, 2) );
}

void Waypoint::addNeighbour(Waypoint * w) {
	if(w != NULL) {
		this->neighbours.push_back(w);
	}
}

bool Waypoint::hasNeighbours() {
	return this->neighbours.size() > 0;
}

Waypoint * Waypoint::getRandomNeighbour() {
	if(hasNeighbours()) {
		return this->neighbours.at((rand() % this->neighbours.size()));
	}
	else {
		quit("Waypoint \"%s\" has no neighbours.", name);
		return NULL;
	}
}

Point & Waypoint::getPosition() {
	return position;
}

void Waypoint::update(double timeElapsed) { }

void Waypoint::draw() { }

void Waypoint::import (ifstream &input) {
	char line[256];
	char key[256];
	char value[256];
	char * str;
	
	//Input the header.
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int currentIndex = atoi(line);
	input.getline(line, 256, '\n');

	//Input the position
	double x, y, z;
	input.getline(line, 256, ':');
	input.getline(line, 256, ',');
	x = atof(line);
	input.getline(line, 256, ',');
	y = atof(line);
	input.getline(line, 256, ';');
	z = atof(line);
	position = Point(x, y, z);
	input.getline(line, 256, '\n');
	
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
		else if(_stricmp(key, "neighbours") == 0) {
			neighbourValues = str;
		}
		else {
			printf("WARNING: Encountered unexpected property when parsing waypoint object: \"%s\".\n", key);
			delete [] str;
		}
	}
}
