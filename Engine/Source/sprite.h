//*****************************************************************************************//
//                                        Sprite                                           //
//*****************************************************************************************//

#ifndef spriteModule
#define spriteModule 

class Sprite : public Object {
public:
	Sprite() {
		picture = NULL;
		waypoint = NULL;
		movementSpeed = 0;
	}

	~Sprite() { }

	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures, WaypointCollection & waypoints);
private:
	char * name;
	Point position;
	Point center;
	Point bottomCenter;
	Point extent;
	Texture * picture;
	Waypoint * waypoint;
	double movementSpeed;
};

declareCollection(Sprite);

#endif //spriteModule