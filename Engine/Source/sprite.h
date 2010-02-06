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
	}

	~Sprite() { }

	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures, WaypointCollection & waypoints);
	
	void printOn(ostream & o) const;
	
private:
	char * name;
	Point position;
	Point center;
	Point bottomCenter;
	Point extent;
	Texture * picture;
	Waypoint * waypoint;
};

declareCollection(Sprite);

ostream & operator << (ostream & o, const Sprite & x);

#endif //spriteModule