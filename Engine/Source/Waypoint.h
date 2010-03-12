#pragma once

#include "Includes.h"
#include "Object.h"

class Waypoint : public Object {
public:
	Waypoint() { }
	
	~Waypoint() {
		delete [] name;
		delete [] neighbourValues;
	}
	
	double distanceFrom(Point & p) const;
	
	void addNeighbour(Waypoint * w);
	bool hasNeighbours();
	Waypoint * getRandomNeighbour();
	Point & getPosition();
	
	void tick() { }
	void draw() { }
	
	void import (ifstream &input);

public:
	Point position;
	char * name;
	char * neighbourValues;

private:
	vector<Waypoint *> neighbours;
};
