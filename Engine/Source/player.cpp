
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                       Player                                            //
//*****************************************************************************************//

Player *player = NULL;

void Player::setup () {
	player = new Player;
}

void Player::wrapup () {
	delete player;
} 

void Player::tick () {
	//Once collision detection is added, will need to deal with movement operations as requests
	//since it may not be possible to do what is requested (after alll, you shouldn't be able
	//to go through walls).
}

void Player::draw () {
	//In first person mode, the player is invisible but not in third person mode.
	//You might also consider drawing a cursor in the center of the screen.
}

void Player::beginCamera () {
	glPushMatrix ();
		glMultMatrixd (playerMatrix.inverse);
}

void Player::endCamera () {
	glPopMatrix (); //Match indentation in beginCamera.
}

void Player::moveBy (Point &translation) {
	const bool logging = false;
	if (logging) {
//		::log ("\nPlayer told to translate by "); translation.log ();
//		::log ("\nTransformation before:"); playerMatrix.normal ().log (1);
	}
	playerMatrix.translateBy (translation);
//	if (logging) {::log ("\nTransformation after:"); playerMatrix.normal ().log (1);}
}

void Player::rotateBy (Point &rotation) {
	const bool logging = false;
//	if (logging) {::log ("\nPlayer told to rotate by "); rotation.log ();}
	playerMatrix.rotateBy (rotation);
}

void Player::reset (Point &worldPosition) {
	playerMatrix.setToIdentity ();
	playerMatrix.preTranslateBy (worldPosition);
}
