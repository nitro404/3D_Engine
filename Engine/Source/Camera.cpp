#include "Camera.h"

Camera::Camera() : xRotation(0), yRotation(0) {
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

Camera::~Camera() {

}

void Camera::beginCamera() {
	glPushMatrix();
		glRotated(-xRotation, 1.0, 0.0, 0.0);
		
		glPushMatrix();
			glMultMatrixd(cameraMatrix.inverse);
			manager.prepareForDraw();
}

void Camera::endCamera() {
		glPopMatrix();
	glPopMatrix();
}

Point Camera::forwardVector() const {
	return (Point(0.0, 0.0, -1.0).transformBy(cameraMatrix)).normalized();
}

void Camera::moveBy(Point & translation) {
	cameraMatrix.translateBy(translation);
}

void Camera::rotateBy(Point & rotation) {
	xRotation += rotation.x;
	yRotation += rotation.y;
	cameraMatrix.rotateBy(Point(0.0, rotation.y, 0.0));
}

void Camera::reset(Point & position) {
	cameraMatrix.setToIdentity();
	cameraMatrix.preTranslateBy(position);
}

void Camera::update(double timeElapsed) {
	static double translationSpeed = 10.0;
	static double rotationSpeed = 30.0;

	Point translation (
		(translateLeft ? -translationSpeed : 0.0) + (translateRight ? translationSpeed : 0.0),
		(translateDown ? -translationSpeed : 0.0) + (translateUp ? translationSpeed : 0.0),
		(translateAhead ? -translationSpeed : 0.0) + (translateBack ? translationSpeed : 0.0));

	Point rotation (
		(rotateDown ? -rotationSpeed : 0.0) + (rotateUp ? rotationSpeed : 0.0),
		(rotateRight ? -rotationSpeed : 0.0) + (rotateLeft ? rotationSpeed : 0.0),
		0.0);

	rotateBy(rotation * timeElapsed);
	moveBy(translation * timeElapsed);
}
