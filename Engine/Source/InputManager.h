#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Includes.h"
#include "Point.h"
#include "Camera.h"

class InputManager {
public:
	InputManager () {
		translateLeft = false;
		translateRight = false;
		translateAhead = false;
		translateBack = false;
		translateUp = false;
		translateDown = false;
		
		rotateLeft = false;
		rotateRight = false;
		rotateUp = false;
		rotateDown = false;
	}
	~InputManager() { }

	static const double translationSpeed; //meters per second.
	static const double rotationSpeed; //degrees per second

	bool translateLeft, translateRight, translateAhead, translateBack, translateUp, translateDown;
	bool rotateLeft, rotateRight, rotateUp, rotateDown;

	void update(double timeElapsed);
	
	void moveBy(Point & translation) { camera->moveBy(translation); }
	void rotateBy(Point & rotation) { camera->rotateBy(rotation); }
};

extern InputManager * inputManager;

#endif // INPUT_MANAGER_H
