#pragma once

#include "Includes.h"
#include "Object.h"
#include "Waypoint.h"

class Sprite : public Object {
public:
	Point position;
	Texture * picture;
	
	Sprite() {
		picture = NULL;
		origin = NULL;
		destination = NULL;
		lastWaypoint = NULL;
		movementSpeed = 0;
		distanceToTravel = 0;
		distanceTravelled = 0;
	}
	
	~Sprite() {
		delete [] name;
	}

	double distanceFrom(Point & p) const;
	
	void tick();
	void draw();
	
	void import (ifstream &input, vector<Texture *> & textures, vector<Waypoint *> & waypoints);
	
private:
	char * name;
	Point center;
	Point bottomCenter;
	Point extent;
	Waypoint * origin;
	Waypoint * lastWaypoint;
	Waypoint * destination;
	double movementSpeed;
	Vector unitVector;
	double distanceTravelled;
	double distanceToTravel;
	
	void switchDestinations();
};
