#ifndef _PLAYER_H
#define _PLAYER_H

#include "Includes.h"
#include "Transformation.h"

class Player {
public:
	DualTransformation playerMatrix;

	Player() { }
	~Player() { }
	
	void tick();
	void draw();

	void beginCamera();
	void endCamera();
	
	void moveBy(Point & translation);
	void rotateBy(Point & rotation);

	void reset(Point & worldPosition);
};

extern Player * player;

#endif
