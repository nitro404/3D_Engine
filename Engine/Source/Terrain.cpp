#include "Terrain.h"

Terrain::Terrain() : shader(NULL), name(NULL), textureMap(NULL), width(0), height(0), points(NULL) { }

Terrain::~Terrain() {
	if(name != NULL) { delete [] name; }
	if(points != NULL) { delete [] points; }
	delete group;
}

void Terrain::draw() {
	if(shader != NULL) { shader->activate(); }

	glDisable(GL_BLEND);
	textureMap->activate();
	glPushMatrix();
		glMultMatrixd(transformation->normal());
		group->draw();
	glPopMatrix();

	if(shader != NULL) { shader->deactivate(); }
}

void Terrain::import(ifstream & input, vector<Texture *> & textures, vector<char *> & heightMaps, vector<Shader *> shaders) {
	char line[256];
	char key[256];
	char value[256];
	char * str;
	
	transformation = DualTransformation::import(input);

	// input the properties
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfProperties = atoi(line);
	input.getline(line, 256, '\n');
	for(int propertyIndex=0;propertyIndex<numberOfProperties;propertyIndex++) {
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf_s(line, " \"%[^\"]\" => \"%[^\"]\"", key, 256, value, 256);
		str = new char[strlen(value) + 1];
		strcpy_s(str, strlen(value) + 1, value);
		
		// parse properties to local variables
		if(_stricmp(key, "name") == 0) {
			name = str;
		}
		else if (_stricmp(key, "heightmap") == 0) {
			heightMap = heightMaps.at(atoi(str));
			delete [] str;
		}
		else if (_stricmp(key, "texturemap") == 0) {
			textureMap = textures.at(atoi(str));
			delete [] str;
		}
		else if (_stricmp(key, "width") == 0) {
			width = atoi(value);
			delete [] str;
		}
		else if (_stricmp(key, "height") == 0) {
			height = atoi(value);
			delete [] str;
		}
		else if (_stricmp(key, "tiled") == 0) {
			tiled = atoi(str);
			delete [] str;
		}
		else if(_stricmp(key, "shader") == 0) {
			int shaderIndex = atoi(str);
			if(shaderIndex >= 0 && shaderIndex < (int) shaders.size()) { shader = shaders.at(shaderIndex); }
			delete [] str;
		}
		else {
			printf("WARNING: Encountered unexpected property when parsing terrain object: \"%s\".\n", key);
			delete [] str;
		}
	}

	// input the bounding box
	box = BoundingBox::import(input);

	// input the height map
	int size = width * height;
	int * heightMapData = new int[size];
	ifstream heightMapFile(heightMap, ios::binary);
	if(!heightMapFile.is_open()) {
		quit("Unable to open height map file: \"%s\"", heightMap);
	}
	for(int i=0;i<size;i++) {
		heightMapData[i] = heightMapFile.get();
	}
	heightMapFile.close();

	Point terrainSize = box->getExtent();

//	double tileSizeX = terrainSizeX / (width + 1);
//	double tileSizeZ = terrainSizeY / (width + 1);

	int currentPoint = 0;
	double x, y, z, tx, ty;
	points = new GamePoint[(width + 1) * (height  + 1)];
	for(int i=0;i<width+1;i++) {
		for(int j=0;j<height+1;j++) {
			x = ((i / (double) width) * terrainSize.x) + box->getMin().x;
			y = ((scaleHeight(i, j, heightMapData) / 255.0) * terrainSize.y) + box->getMin().y;
			z = ((j / (double) height) * terrainSize.z) + box->getMin().z;

			if(tiled == 1) {
				tx = i * 0.1;
				ty = j * 0.1;
			}
			else {
				tx = ((double) j / (height + 1.0f));
				ty = 1.0f - ((double) i / (double) (width + 1));
			}
			
			points[currentPoint].x = x;
			points[currentPoint].y = y;
			points[currentPoint].z = z;
			points[currentPoint].tx = tx;
			points[currentPoint].ty = ty;
			currentPoint++;
		}
	}

	delete [] heightMapData;

	group = new FaceGroup(points, width + 1, height + 1, 4);
}

double Terrain::scaleHeight(int x, int y, int * heightMapData) {
	int sum = 0;
	int vertexCount = 0;
	
	if(x-1 >= 0 && x-1 < width && y-1 >= 0 && y-1 < height) {
		sum += heightMapData[((x-1) * width) + (y-1)];
		vertexCount++;
	}
	
	if(x-1 >= 0 && x-1 < width && y >= 0 && y < height) {
		sum += heightMapData[((x-1) * width) + y];
		vertexCount++;
	}
	
	if(x >= 0 && x < width && y-1 >= 0 && y-1 < height) {
		sum += heightMapData[(x * width) + (y-1)];
		vertexCount++;
	}
	
	if(x >= 0 && x < width && y >= 0 && y < height) {
		sum += heightMapData[(x * width) + y];
		vertexCount++;
	}
	
	return (sum / (double) vertexCount); 
}
