#ifndef _CAMERA_H
#define _CAMERA_H

#include "Includes.h"
#include "Point.h"
#include "Player.h"

class Camera {
public:
	Camera() {
		xRotation = 0;
		yRotation = 0;
		target = player;
		offset = Point (0.0, 1.0, 1.0); //Above and behind...
	}

	~Camera() { }

	Player * target;
	Point offset;
	double xRotation;
	double yRotation;

	void tick ();
	void draw ();

	void beginCamera ();
	void endCamera ();
	
	//Movement functions that pass the buck to the target (but be careful with rotation)...
	void moveBy (Point &translation) {
		const bool logging = false;
		target->moveBy (translation);
	}

	void rotateBy (Point &rotation) {
		//Accumulate the x-rotation but don't pass it along to the player. Also, discard all z-rotation.
		const bool logging = false;
		xRotation += rotation.x;
		yRotation += rotation.y;
		target->rotateBy (Vector (0.0, rotation.y, 0.0));
	}
};

extern Camera * camera;

#endif
