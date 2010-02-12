#include "includes.all"

Player * player = NULL;

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
	playerMatrix.translateBy (translation);
}

void Player::rotateBy (Point &rotation) {
	playerMatrix.rotateBy (rotation);
}

void Player::reset (Point &worldPosition) {
	playerMatrix.setToIdentity ();
	playerMatrix.preTranslateBy (worldPosition);
}
