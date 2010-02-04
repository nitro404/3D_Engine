//*****************************************************************************************//
//                                        Waypoint                                         //
//*****************************************************************************************//

#ifndef waypointModule
#define waypointModule 

class Waypoint : public Object {
public:
	
	Waypoint();
	~Waypoint();
	
	void addNeighbour(Waypoint * w);

	void tick();
	void draw();

	void import (ifstream &input);
	
	void printOn(ostream & o) const;
	
	char * name;
	char * neighbourValues;
private:
	vector<Waypoint *> neighbours;
};

declareCollection(Waypoint);

ostream & operator << (ostream & o, const Waypoint & x);

#endif //waypointModule