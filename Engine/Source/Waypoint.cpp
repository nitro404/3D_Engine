#include "Waypoint.h"

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
		srand((unsigned int) time(NULL));
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
	}
	
	this->position = this->transformation.normal().position();
	
	delete [] line;
	delete [] key;
	delete [] value;
}
