#include "InputManager.h"

const double InputManager::translationSpeed = 10.0; //meters per second.
const double InputManager::rotationSpeed = 30.0; //degrees per second

InputManager * inputManager = NULL;

void InputManager::update(double timeElapsed) {
	Point translation (
		(translateLeft ? -translationSpeed : 0.0) + (translateRight ? translationSpeed : 0.0),
		(translateDown ? -translationSpeed : 0.0) + (translateUp ? translationSpeed : 0.0),
		(translateAhead ? -translationSpeed : 0.0) + (translateBack ? translationSpeed : 0.0));

	Point rotation (
		(rotateDown ? -rotationSpeed : 0.0) + (rotateUp ? rotationSpeed : 0.0),
		(rotateRight ? -rotationSpeed : 0.0) + (rotateLeft ? rotationSpeed : 0.0),
		0.0);

	rotateBy(rotation * timeElapsed); //degrees = degrees per second * second
	moveBy(translation * timeElapsed); //meters = meters per second * meters
}
