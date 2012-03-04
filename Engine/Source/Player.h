#ifndef PLAYER_H
#define PLAYER_H

#include "Includes.h"
#include "Transformation.h"

class Player {
public:
	DualTransformation playerMatrix;

	Player() { }
	~Player() { }
	
	void moveBy(Point & translation);
	void rotateBy(Point & rotation);

	void reset(Point & worldPosition);
};

extern Player * player;

#endif // PLAYER_H
