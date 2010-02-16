#include "Waypoint.h"

double Waypoint::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - position.x, 2) + pow(p.y - position.y, 2) + pow(p.z - position.z, 2) );
}

void Waypoint::addNeighbour(Waypoint * w) {
	if(w == NULL) { return; }
	
	this->neighbours.push_back(w);
}

bool Waypoint::hasNeighbours() {
	return this->neighbours.size() > 0;
}

Waypoint * Waypoint::getRandomNeighbour() {
	srand((unsigned int) time(NULL));
	return this->neighbours.at((rand() % this->neighbours.size()));
}

Point & Waypoint::getPosition() {
	return position;
}

void Waypoint::import (ifstream &input) {
	char * line;
	char * key;
	char * value;
	char * str;
	line = new char[256];
	key = new char[256];
	value = new char[256];
	
	//Input the header.
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int currentIndex = atoi(line);
	input.getline(line, 256, '\n');
	
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
		else if(stricmp(key, "neighbours") == 0) {
			neighbourValues = str;
		}
	}
	
	this->position = this->transformation.normal().position();
	
	delete [] line;
	delete [] key;
	delete [] value;
}
