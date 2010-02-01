
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                       Camera                                            //
//*****************************************************************************************//

Camera *camera = NULL;

void Camera::setup () {
	camera = new Camera;
	Point backward (0.0, 0.0, 40); //+20 is out of the screen.
	camera->moveBy (backward);
}

void Camera::wrapup () {
	delete camera;
}

void Camera::tick () {
	//::log ("\nCamera tick %s%s%s%s%s%s %s%s%s%s", 
	//	(translateLeft ? "T" : "F"),  (translateRight ? "T" : "F"),
	//	(translateDown ? "T" : "F"), (translateUp ? "T" : "F"),
	//	(translateAhead ? "T" : "F"), (translateBack ? "T" : "F"), 
	//	(rotateLeft ? "T" : "F"),  (rotateRight ? "T" : "F"),
	//	(rotateDown ? "T" : "F"), (rotateUp ? "T" : "F"));

	//Deal with control keys... 
	const double translationSpeed = 10.0; //meters per second.
	const double rotationSpeed = 90.0; //degrees per second

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

void Camera::draw () {
	//Normally, the camera is invisible but you might consider drawing a cursor in the center of the screen.
}

void Camera::pushMatrix () {
	//If M = RxRyT, M-1 = T-1Ry-1Rx-1. In our case, Rx is NOT in M,
	//glPushMatrixd (cameraMatrix.inverse);
	glPushIdentity ();
	glRotated (-totalXRotation, 1.0, 0.0, 0.0); 
	glMultMatrixd (cameraMatrix.inverse);

}

void Camera::popMatrix () {
	glPopMatrix ();
}
