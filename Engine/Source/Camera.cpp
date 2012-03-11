#include "Game.h"
#include "Camera.h"

Camera::Camera() : xRotation(0), yRotation(0), fly(true), collisionMesh(NULL), initialized(false) {
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
	if(collisionMesh != NULL) { collisionMesh->release(); }
}

bool Camera::init() {
	if(initialized || !Game::physics->isInitialized()) { return false; }

	collisionMesh = Game::physics->createSphereMesh(Game::camera->cameraMatrix.position(), Point(0.0, 0.0, 0.0), 0.5f, 0.1f), 

	initialized = true;
	
	return true;
}

void Camera::toggleFly() {
	fly = !fly;
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

	lastCameraMatrix = cameraMatrix;
}

void Camera::update(double timeElapsed) {
	static double translationSpeed = 16.0;
	static double rotationSpeed = 30.0;

	lastCameraMatrix = cameraMatrix;

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

void Camera::handleCollisions(double timeElapsed) {
	if(!initialized) { return; }

	Point intersection;
	Point from = lastCameraMatrix.position();
	Point to = cameraMatrix.position();

	if(!fly) {
		if(from != to && Game::physics->sweepSphereHits(*collisionMesh, from, to, intersection)) {
			cameraMatrix = lastCameraMatrix;
//			cameraMatrix.translateTo(intersection);
		}

		to = cameraMatrix.position() + Point(0, -9.81 * timeElapsed, 0);

		if(Game::physics->sweepSphereHits(*collisionMesh, from, to, intersection)) {
//			cameraMatrix.translateTo(intersection);
			cameraMatrix.m42 = lastCameraMatrix.m42;
		}
		else {
			cameraMatrix.translateBy(Point(0, -9.81 * timeElapsed, 0));
		}
	}
}
