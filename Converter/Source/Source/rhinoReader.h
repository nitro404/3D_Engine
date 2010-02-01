
#ifndef rhinoReaderModule
#define rhinoReaderModule

struct LoaderInfo;
class ModelPropertiesData;
class ON_Mesh;

struct RhinoPoint {
	float x, y, z;
	RhinoPoint () {}
	RhinoPoint (float x, float y, float z) : x (x), y (y), z (z) {}
};

typedef unsigned char BYTE;

struct RhinoVertex {
	float x, y, z;
	float nx, ny, nz;
	float tx, ty;
	float tx2, ty2;
	BYTE r, g, b, a;
};

struct RhinoMeshSection {
	RhinoVertex *vertices; long vertexCount;
	long *indices; long indexCount;
	char textureName [255];
	char look [255];

	ModelPropertiesData *properties;

	void modelProperties (ON_Mesh &onMesh);

	long lod ();
	double nearDistance ();
	double farDistance ();
	bool useForCollision ();
	bool centerAtOrigin ();
	bool makeUnitSize ();
	long normalType ();
	long shaderType ();
	long sortType () ;
	long collisionMaterialType ();

	RhinoMeshSection ();
	~RhinoMeshSection ();
};

struct RhinoMesh {
	RhinoMeshSection *sections; long sectionsCount;
	char name [255];
	RhinoVertex *points; long pointsCount;

	RhinoMesh ();
	~RhinoMesh ();
};

//------------------------------------------------------------------------
// Example usage:
//	RhinoLoader loader;
//	RhinoMesh *mesh = loader.loadMesh (filename);
//	if (mesh == NULL) return false;
//  ... copy the data out of the mesh ...
//  delete mesh;
//------------------------------------------------------------------------

class RhinoLoader {
public:
	RhinoLoader ();
	~RhinoLoader ();

	//Make sure you delete the mesh after you are done with it.
	RhinoMesh *loadMesh (const char *filename);
private:
	LoaderInfo *loaderInfo;
	bool readFile (const char *filename);
	RhinoMesh *createMesh ();
	bool buildMesh (RhinoMesh *mesh);
};

#endif //rhinoReaderModule
