//*****************************************************************************************//
//                                        Face                                             //
//*****************************************************************************************//

#ifndef faceModule
#define faceModule 

struct GamePoint {
	double x, y, z;
	double tx, ty;
	//double nx, ny, nz;
};

declareCollection (GamePoint);

class Face {
public:
	//A face owns it's points and therefore deletes them on its own.
	//It does not own it's texture because other faces will refer to the same texture...
	//The world should own the texture...
	Texture *texture;
	GamePointCollection points;

	Face () {texture = NULL;}
	~Face () {deleteGamePointCollectionEntries (points);};

	void tick ();
	void draw ();
	void import (ifstream &input, TextureCollection & textures);
};


declareCollection (Face);

#endif //faceModule