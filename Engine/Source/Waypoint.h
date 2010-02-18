#ifndef _WAYPOINT_H
#define _WAYPOINT_H

#include "Includes.h"
#include "Object.h"

class Waypoint : public Object {
public:
	Point position;
	char * name;
	char * neighbourValues;
	
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
	
private:
	vector<Waypoint *> neighbours;
};

#endif
