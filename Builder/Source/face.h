//*****************************************************************************************//
//                                        Face                                             //
//*****************************************************************************************//

#ifndef faceModule
#define faceModule 

struct GamePoint {
	double x, y, z;
	double tx, ty;
	double nx, ny, nz;

	inline Point asPoint () {return Point (x, y, z);}
};

preDeclareCollection (struct, GamePoint); //Just a test (not really needed)...

declareCollection (GamePoint);

preDeclareCollection (class, Face); //Just a test (not really needed)...

class World; //Forward reference for import method...

class Face {
public:
	//A face owns it's points and texture name and therefore deletes them on its own.
	char *textureName;
	GamePointCollection points;

	Face () {textureName = NULL;};
	Face (char *textureName) {
		this->textureName = textureName;
	}
	~Face () {deleteGamePointCollectionEntries (points); delete [] textureName;};

	void import (ifstream &input, World * world);
	void export (long indent, ofstream &output, long faceIndex, vector<char*> & textureNames);
};

declareCollection (Face);

#endif //faceModule