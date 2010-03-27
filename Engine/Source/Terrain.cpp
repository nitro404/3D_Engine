#include "Terrain.h"

Terrain::Terrain() : transformation(NULL), name(NULL), textureMap(NULL), width(0), height(0), points(NULL) {
	minPoint = Point(0,0,0);
	maxPoint = Point(0,0,0);
}

Terrain::~Terrain() {
	if(name != NULL) { delete [] name; }
	if(points != NULL) { delete [] points; }
	if(transformation != NULL) { delete transformation; }
	delete group;
}

void Terrain::draw() {
	textureMap->activate();
	
	if(textureMap->type == RGBAType) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
	glPushMatrix();
		glMultMatrixd(transformation->normal());
		group->draw();
	glPopMatrix();
}

void Terrain::import(ifstream & input, vector<Texture *> & textures, vector<char *> & heightMaps) {
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
		else if (_stricmp(key, "maxpoint") == 0) {
			double xPos, yPos, zPos;
			char * temp = strchr(str + sizeof(char), ' ');
			*temp = '\0';
			xPos = atof(str);
			temp += sizeof(char);
			char * temp2 = strchr(temp, ' ');
			*temp2 = '\0';
			yPos = atof(temp);
			temp2 += sizeof(char);
			zPos = atof(temp2);
			maxPoint = Point(xPos, yPos, zPos);
			delete [] str;
		}
		else if (_stricmp(key, "minpoint") == 0) {
			double xPos, yPos, zPos;
			char * temp = strchr(str + sizeof(char), ' ');
			*temp = '\0';
			xPos = atof(str);
			temp += sizeof(char);
			char * temp2 = strchr(temp, ' ');
			*temp2 = '\0';
			yPos = atof(temp);
			temp2 += sizeof(char);
			zPos = atof(temp2);
			minPoint = Point(xPos, yPos, zPos);
			delete [] str;
		}
		else if (_stricmp(key, "tiled") == 0) {
			tiled = atoi(str);
			delete [] str;
		}
		else {
			printf("WARNING: Encountered unexpected property when parsing terrain object: \"%s\".\n", key);
			delete [] str;
		}
	}

	printf("HEIGHT MAP (%d, %d): %s\n", width, height, heightMap);
	printf("TEXTURE MAP(%d, %d): %s\n", textureMap->width, textureMap->height, textureMap->textureName);

	// input the height map
	int size = width * height;
	int * heightMapData = new int[size];
	ifstream heightMapFile(heightMap);
	if(heightMapFile == NULL) {
		quit("Unable to open height map file: \"%s\"", heightMap);
	}
	for(int i=0;i<size;i++) {
		heightMapData[i] = heightMapFile.get();
	}
	heightMapFile.close();
	
	double terrainSizeX = maxPoint.x - minPoint.x;
	double terrainSizeY = maxPoint.y - minPoint.y;
	double terrainSizeZ = maxPoint.z - minPoint.z;

	int currentPoint = 0;
	double x, y, z, tx, ty;
	points = new GamePoint[(width + 1) * (height  + 1)];
	for(int i=0;i<width+1;i++) {
		for(int j=0;j<height+1;j++) {
			x = ((i / (double) width) * terrainSizeX) + minPoint.x;
			y = ((scaleHeight(i, j, heightMapData) / 255.0) * terrainSizeY) + minPoint.y;
			z = ((j / (double) height) * terrainSizeZ) + minPoint.z;

			if(tiled == 1) {
				tx = i;
				ty = j;
			}
			else {
				tx = ((float) i / (width + 1.0f));
				ty = ((float) j / (height + 1.0f));
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
