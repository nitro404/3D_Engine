#include "Camera.h"

Camera * camera = NULL;

void Camera::beginCamera() {
	//If R is the camera's rotation, T is the camera's translation, and W is the player's 
	//transformation, then moving the camera LOCALLY is a pre-transformation. So the movement
	//result needed is RTW... However, we are being asked to push the transformation
	//needed to play the role of a camera... This means we need to push (RTW)-1  (where -1 means
	//inverse) which by the theorem dealing with inverses becomes W-1T-1R-1... Since the operations 
	//on the stack are done right to left, we need to deal with R-1, then T-1, and then W-1.

	//rotate by R-1 which pre-multiplies whatever is on the stack already, and finally
	//translate by T-1...
	//on the stack are done right to left, we let the target set up the camera and then we
	//rotate by R-1 which pre-multiplies whatever is on the stack already, and finally
	//translate by T-1...

	glPushMatrix();
		glRotated(-xRotation, 1.0, 0.0, 0.0);
		glTranslated(-offset.x, -offset.y, -offset.z);
		
		glPushMatrix();
			glMultMatrixd(player->playerMatrix.inverse);
			manager.prepareForDraw();
}

void Camera::endCamera() {
		glPopMatrix();
	glPopMatrix();
}
