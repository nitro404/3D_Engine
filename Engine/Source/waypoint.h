//*****************************************************************************************//
//                                        Waypoint                                         //
//*****************************************************************************************//

#ifndef waypointModule
#define waypointModule 

class Waypoint : public Object {
public:
	Waypoint();
	~Waypoint();

	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures);
	
	void printOn(ostream & o) const;
	
private:
	char * name;
	char * neighbours;
};

declareCollection(Waypoint);

ostream & operator << (ostream & o, const Waypoint & x);

#endif //waypointModule