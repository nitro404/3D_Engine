//*****************************************************************************************//
//                                        Sprite                                           //
//*****************************************************************************************//

#ifndef spriteModule
#define spriteModule 

class Sprite : public Object {
public:
	Sprite() {
		picture = NULL;
		origin = NULL;
		destination = NULL;
		lastWaypoint = NULL;
		movementSpeed = 0;
	}
	
	~Sprite() { }
	
	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures, WaypointCollection & waypoints);
private:
	void switchDestinations();
	
	char * name;
	Point position;
	Point center;
	Point bottomCenter;
	Point extent;
	Texture * picture;
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