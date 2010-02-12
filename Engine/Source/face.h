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
	Texture * texture;
	GamePointCollection points;

	Face () {
		texture = NULL;
	}

	virtual ~Face () {
		deleteGamePointCollectionEntries (points);
	}

	virtual void tick();
	virtual void draw();
	virtual void draw (double red, double green, double blue, double alpha);
	virtual void import(ifstream &input, TextureCollection & textures);
	virtual void import(ifstream &input);
};

declareCollection (Face);

#endif //faceModule