//*****************************************************************************************//
//                                        Sprite                                           //
//*****************************************************************************************//

#ifndef spriteModule
#define spriteModule 

class Sprite : public Object {
public:
	Point position;
	Texture * picture;

	Sprite() {
		picture = NULL;
		origin = NULL;
		destination = NULL;
		lastWaypoint = NULL;
		movementSpeed = 0;
	}
	
	~Sprite() {
		delete [] name;
	}

	double distanceFrom(Point & p) const;
	
	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures, WaypointCollection & waypoints);
private:
	void switchDestinations();
	char * name;
	Point center;
	Point bottomCenter;
	Point extent;
	Waypoint * origin;
	Waypoint * lastWaypoint;
	Waypoint * destination;
	double movementSpeed;
	Vector unitVector;
	double distanceTravelled;
	double distanceToTravel;
};

declareCollection(Sprite);

#endif //spriteModule