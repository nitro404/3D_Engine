//*****************************************************************************************//
//                                        Sprite                                           //
//*****************************************************************************************//

#ifndef spriteModule
#define spriteModule 

class Sprite : public Object {
public:
	DualTransformation transformation;
	FaceCollection faces;

	Sprite() { }

	~Sprite() {
		deleteFaceCollectionEntries(faces);
	}

	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures, WaypointCollection & waypoints);
	
	void printOn(ostream & o) const;
	
private:
	char * name;
//	Point location;
	Texture * picture;
	Waypoint * waypoint;
};

declareCollection(Sprite);

ostream & operator << (ostream & o, const Sprite & x);

#endif //spriteModule