#pragma once

#include "Includes.h"
#include "Object.h"
#include "Waypoint.h"

class Sprite : public Object {
public:
	Sprite();
	~Sprite();

	double distanceFrom(Point & p) const;
	
	void tick();
	void draw();
	
	void import (ifstream &input, vector<Texture *> & textures, vector<Waypoint *> & waypoints, vector<Shader *> shaders);
	
public:
	Point position;
	Texture * picture;
	Shader * shader;

private:
	GLuint spriteList;
	char * name;
	Point center;
	Point bottomCenter;
	Point extent;
	Waypoint * origin;
	Waypoint * lastWaypoint;
	Waypoint * destination;
	double movementSpeed;
	Point unitVector;
	double distanceTravelled;
	double distanceToTravel;
	
	void switchDestinations();
};
