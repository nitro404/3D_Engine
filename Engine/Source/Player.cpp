#include "Player.h"

Player * player = NULL;

void Player::beginCamera() {
	glPushMatrix();
		glMultMatrixd(playerMatrix.inverse);
}

void Player::endCamera() {
	glPopMatrix();
}

void Player::moveBy(Point & translation) {
	playerMatrix.translateBy(translation);
}

void Player::rotateBy(Point & rotation) {
	playerMatrix.rotateBy(rotation);
}

void Player::reset(Point & worldPosition) {
	playerMatrix.setToIdentity();
	playerMatrix.preTranslateBy(worldPosition);
}
