#include "includes.all"

const double InputManager::translationSpeed = 12.0; //meters per second.
const double InputManager::rotationSpeed = 40.0; //degrees per second

InputManager * inputManager = NULL;

void InputManager::tick () {
	//Deal with control keys... 
	Point translation (
		(translateLeft ? -translationSpeed : 0.0) + (translateRight ? translationSpeed : 0.0),
		(translateDown ? -translationSpeed : 0.0) + (translateUp ? translationSpeed : 0.0),
		(translateAhead ? -translationSpeed : 0.0) + (translateBack ? translationSpeed : 0.0));

	Point rotation (
		(rotateDown ? -rotationSpeed : 0.0) + (rotateUp ? rotationSpeed : 0.0),
		(rotateRight ? -rotationSpeed : 0.0) + (rotateLeft ? rotationSpeed : 0.0),
		0.0);

	rotateBy (rotation * DT); //degrees = degrees per second * second
	moveBy (translation * DT); //meters = meters per second * meters
}
