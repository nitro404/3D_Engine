//*****************************************************************************************//
//                                        Waypoint                                         //
//*****************************************************************************************//

#ifndef waypointModule
#define waypointModule 

class Waypoint : public Object {
public:
	DualTransformation transformation;

	Waypoint() { }

	~Waypoint() {
		delete [] name;
		delete [] neighbourValues;
		for(int i=0;i<this->neighbours.size();i++) {
			delete this->neighbours.at(i);
		}
	}
	
	void addNeighbour(Waypoint * w);
	Waypoint * getRandomNeighbour();
	
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