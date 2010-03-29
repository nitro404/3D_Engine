#pragma once

#include "Includes.h"
#include "Point.h"
#include "Player.h"

class Camera {
public:
	Camera() : xRotation(0), yRotation(0), target(player) {
		offset = Point(0.0, 1.0, 1.0);
	}

	~Camera() { }
	
	void beginCamera();
	void endCamera();
	
	void moveBy(Point & translation) {
		target->moveBy(translation);
	}

	void rotateBy(Point & rotation) {
		xRotation += rotation.x;
		yRotation += rotation.y;
		target->rotateBy(Point(0.0, rotation.y, 0.0));
	}

public:
	double xRotation;
	double yRotation;

private:
	Player * target;
	Point offset;
};

extern Camera * camera;
