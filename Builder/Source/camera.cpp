
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
	Point backward (0.0, 0.0, 20); //+20 is out of the screen.
	camera->moveBy (backward);
}

void Camera::wrapup () {
	delete camera;
}

void Camera::tick () {
	//Deal with user desires... Information that came from the arrow keys and/or the mouse.
	
	#if (MAKE_CAMERA_LOOK_AROUND == 1)
		enum Algorithm {DoNeither, XRotateOnly, YRotateOnly, DoBoth}; //Note Z rotation...
		
		Algorithm algorithm = DoNeither;	
		//Algorithm algorithm = XRotateOnly;	
		//Algorithm algorithm = YRotateOnly;	
		//Algorithm algorithm = DoBoth;

		double angleSpeed = 60.0; //degrees per second..
		double angleLimit = 10.0; //degrees..
		//Just use globals to look around (say at 30 degrees per second)...
		//Note: 30 degree per second * DT seconds gives number of degrees for this tick...
		static double xRotation = -angleLimit; static double yRotation = 0.0; //-angleLimit; 
		static double xDelta = 0.0; static double yDelta = 0.0;
		static bool xIncreasing = true; static bool yIncreasing = true;

		//Increase toward angleLimit then decrease toward -angleLimit and repeat...
		xDelta = angleSpeed * (xIncreasing ? DT : -DT);
		xRotation += xDelta;
		if (xRotation > angleLimit) {xIncreasing = false;}
		if (xRotation < -angleLimit) {xIncreasing = true;}

		//Do the same for y-rotation independently...
		yDelta = angleSpeed * (yIncreasing ? DT : -DT);
		yRotation += yDelta;
		if (yRotation > angleLimit) {yIncreasing = false;}
		if (yRotation < -angleLimit) {yIncreasing = true;}

		switch (algorithm) {
			case DoNeither:
				break;
			case XRotateOnly:	
				cameraMatrix.preRotateBy (xDelta, Vector (1.0, 0.0, 0.0));
				break;
			case YRotateOnly:	
				cameraMatrix.preRotateBy (yDelta, Vector (0.0, 1.0, 0.0));
				break;
			case DoBoth:
				cameraMatrix.preRotateBy (yDelta, Vector (0.0, 1.0, 0.0));
				cameraMatrix.preRotateBy (xDelta, Vector (1.0, 0.0, 0.0));
				break;
		}
	#endif
}

void Camera::draw () {
	//Normally, the camera is invisible but you might consider drawing a cursor in the center of the screen.
}

void Camera::beginCamera () {
	glPushMatrixd (cameraMatrix.inverse);
}

void Camera::endCamera () {
	glPopMatrix ();
}
