//*****************************************************************************************//
//                                    UniversalObject                                      //
//*****************************************************************************************//

#ifndef universalObjectModule
#define universalObjectModule 

class UniversalObject; //Forward reference...

struct GamePoint {
	double x, y, z;
	double nx, ny, nz;
	double tx, ty;

	Point asPoint () {return Point (x, y, z);}
	Point asNormal () {return Point (nx, ny, nz);}
	void log () {::log ("[%3.2f,%3.2f,%3.2f] [%3.2f,%3.2f,%3.2f] [%3.2f,%3.2f]", x, y, z, nx, ny, nz, tx, ty);}
};

declareCollection (GamePoint);

class Face {
public:
	//A face owns it points and texture name and therefore deletes them on its own.
//	Plane plane;
	Texture *texture; //Materialized only if loaded....
	char *textureName;
	GamePointCollection points;
	PropertyDictionary properties;

	Face () {texture = NULL; textureName = NULL;};
	Face (char *name) {texture = NULL; textureName = name;};
//	Face (Plane &plane) {
//		this->plane = plane; texture = NULL;
//	}
	~Face () {deleteGamePointCollectionEntries (points); delete [] textureName;};

	void tick ();
	void draw ();
	void drawOutline ();
	void computeBoundingBox (BoundingBox &box, Transformation &transformation);

	//-----------------------------------------------------------------------------------------//
	//                                        Face                                             //
	//-----------------------------------------------------------------------------------------//
	void load (::ifstream &input, UniversalObject *universalObject);
	void dump (long indent, ::ofstream &output, long faceIndex, vector<char*> &textures, vector<char*> &looks);
	long indexOf (vector<char*> &names, char *name);
	void addTextureNames (vector<char*> &textures);
	void addLookNames (vector<char*> &looks);

//	void isConvexLog ();
//	bool isConvex () const;
//	bool intersectsInfiniteLine (const Point &start, const Point &end, Point &intersection) const;
	Vector normal () {return (points [1]->asPoint () - points [0]->asPoint ()).cross (points [2]->asPoint () - points [0]->asPoint ()).normalized ();}
};


declareCollection (Face);
class Object; declareCollection (Object);

class Object {
public:
	DualTransformation transformation;
	BoundingBox box;
	ObjectCollection objects;
	FaceCollection faces;
	PropertyDictionary properties;
	Object () {};
	~Object () {deleteFaceCollectionEntries (faces);}

	void tick ();
	void draw ();
	void computeBoundingBox (BoundingBox &externalBox, Transformation &transformation); //Compute the object's bounding box and integrate with external box...
	void log ();
	void log (long objectIndex);
	void logFaces ();

	//-----------------------------------------------------------------------------------------//
	//                                        Object                                           //
	//-----------------------------------------------------------------------------------------//

	void load (::ifstream &input, UniversalObject *universalObject);
	void dump (long indent, ::ofstream &output, long objectIndex, vector<char*> &textures, vector<char*> &looks);
	void addTextureNames (vector<char*> &textures);
	void addLookNames (vector<char*> &looks);
	bool isWeldCandidate (GamePoint &edge0Start, GamePoint &edge0End, GamePoint &edge1Start, GamePoint &edge1End);
	bool canWeldVertices (Plane &facePlane, Point &start, Point &end, Point &mergePoint1, Point &mergePoint2, Point &intersection);
	void weldFaces (Face &face1, Face &face2);
	void weldAlmostEqualPoints ();

	//-----------------------------------------------------------------------------------------//
	//                       Worldcraft (.map files) specific methods.                         //
	//-----------------------------------------------------------------------------------------//

	void copyWorldcraftFaces (FaceCollection &faces);

	//bool isInFrustum (Point &insideFacePoint, SimpleFrustum &frustum, Camera *camera);
	void transformFacesBy (Transformation &transformation) {
		loopVector (faceIndex, faces)
			Face &face = *(faces [faceIndex]);
			loopVector (pointIndex, face.points)
				GamePoint &point = *(face.points [pointIndex]);
				Point transformedPoint = point.asPoint () * transformation;
				point.x = transformedPoint.x;
				point.y = transformedPoint.y;
				point.z = transformedPoint.z;
			endloop
		endloop
	}
};

//declareCollection (Texture);
class WorldcraftObject;

class UniversalObject {
public:
	ObjectCollection objects;
//	Point startPosition;
	TextureCollection textures; //Materialized only if loaded....

	UniversalObject () {} //startPosition.zero ();};
	~UniversalObject () {
		deleteObjectCollectionEntries (objects); 

		for (long textureIndex = 0; textureIndex < textures.size (); textureIndex++) {
			textures [textureIndex]->unload ();
		}
		deleteTextureCollectionEntries (textures);
	}

	static void setup ();
	static void wrapup ();

	void tick ();
	void draw ();
	void draw (Point &position);
	void computeBoundingBox (BoundingBox &box);
	void log ();

//	static UniversalObject *import (); //Prompts for a file name to read a ".wrl" file from and reads it into a new universalObject.
	void unload (); //Makes sure all textures are unloaded prior to deleting the universalObject.
	
	//-----------------------------------------------------------------------------------------//
	//                                     UniversalObject                                     //
	//-----------------------------------------------------------------------------------------//

	static UniversalObject *load (char *filename);
	void load (::ifstream &input);
	void dump (char *filename);
	void dump (::ofstream &output);
	void addTextureNames (vector<char*> &textureNames);
	void addLookNames (vector<char*> &lookNames);
	Texture *textureFor (char *name);
	void loadAllTextures ();
	void weldAlmostEqualPoints ();

	static UniversalObject *read (char *filename);

	//-----------------------------------------------------------------------------------------//
	//                       Worldcraft (.map files) specific methods.                         //
	//-----------------------------------------------------------------------------------------//

	void add (WorldcraftObject &worldcraftObject);
	void standardizeMapFileDefaults ();
	void standardizeMapFilePathPoints ();
	void OLDstandardizeFacePoints ();
	void standardizeFacePoints ();
	bool isTouching (Object &orienter, Object &candidate);
	Object *orienterFor (Object &candidate);
	Object *mirrorFor (Object &candidate);
	Object *coronaFor (Object &candidate, double &closestDistance);
	Transformation orientingTransformation (Object &orienter, Point &objectCenter, bool flip);
	void standardizeViaOrienters ();
	void standardizeMapFile ();
	static UniversalObject *readMapFile (char *filename);
	
	//-----------------------------------------------------------------------------------------//
	//                           Rhino (.3dm files) specific methods.                          //
	//-----------------------------------------------------------------------------------------//

	static UniversalObject *readRhinoFile (char *filename);
	
	//-----------------------------------------------------------------------------------------//
	//                       Softimage (.xsi files) specific methods.                          //
	//-----------------------------------------------------------------------------------------//

	static UniversalObject *readSoftimageFile (char *filename);
	
	//-----------------------------------------------------------------------------------------//
	//                   Learning Dimensions (.mesh files) specific methods.                   //
	//-----------------------------------------------------------------------------------------//

	static UniversalObject *readLearningDimensionsFile (char *filename);
};

//Utilities...
char *duplicate (char *string);
void convertToLowercase (char *name);
char *pathNameWithoutSuffix (char *string);
char *shortName (char *string);

#endif