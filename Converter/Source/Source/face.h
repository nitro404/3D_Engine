//*****************************************************************************************//
//                                        Face                                             //
//*****************************************************************************************//

#ifndef faceModule
#define faceModule 

struct GamePoint {
	double x, y, z;
	double nx, ny, nz;
	double tx, ty;

	inline Point asPoint () {Point answer = {x, y, z}; return answer;}
};

declareCollection (GamePoint);

class Face {
public:
	//A face owns it points and texture name and therefore deletes them on its own.
	Plane plane;
	char *textureName;
	GamePointCollection points;

	Face () {};
	Face (Plane &plane, char *textureName) {
		this->plane = plane; this->textureName = textureName;
	}
	~Face () {deleteGamePointCollectionEntries (points); delete [] textureName;};
};


declareCollection (Face);

#endif