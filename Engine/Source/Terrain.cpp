#include "Terrain.h"

Terrain::Terrain() : name(NULL), textureMap(NULL), width(0), height(0), points(NULL) {
	minPoint = Point(0,0,0);
	maxPoint = Point(0,0,0);
}

Terrain::~Terrain() {
	if(name != NULL) { delete [] name; }
	if(points != NULL) { delete [] points; }
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
	
	if (TERRAIN_IMPLEMENTATION == USE_QUADS) {
		drawQuads();
	}
	else if (TERRAIN_IMPLEMENTATION == USE_STRIPS) {
		drawStrips();
	}
	else if (TERRAIN_IMPLEMENTATION == USE_FACE_GROUPS) {
		drawFull();
	}
	else if (TERRAIN_IMPLEMENTATION == USE_FACE_GROUPS_AND_FRUSTUM_CULLING) {
		drawFrustrums();
	}
}

void Terrain::drawQuads() {
	GamePoint * point;
	for(int j=0;j<height-1;j++) {
		for(int i=0;i<width;i++) {
			glBegin(GL_POLYGON);
				// top-left
				point = &points[i * width + j];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				// bottom-left
				point = &points[i * width + j + 1];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				// bottom-right
				point = &points[(i + 1) * width + j + 1];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				// top-right
				point = &points[(i + 1) * width + j];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);
			glEnd();
		}
	}
}

void Terrain::drawStrips() {
	GamePoint * point;

	glBegin(GL_TRIANGLE_STRIP);
	for (int j = 0; j < height - 1;j++) {
		point = &points[j * width];
		glTexCoord2d(point->tx, point->ty);
		glVertex3d(point->x, point->y, point->z);
		for (int i = 0; i < width;i++) {
			point = &points[j * width + i];
			glTexCoord2d(point->tx, point->ty);
			glVertex3d(point->x, point->y, point->z);

			point = &points[(j + 1) * width + i];
			glTexCoord2d(point->tx, point->ty);
			glVertex3d(point->x, point->y, point->z);
		}
		glTexCoord2d(point->tx, point->ty);
		glVertex3d(point->x, point->y, point->z);
	}
	glEnd();
}

void Terrain::drawFrustrums() {
	
}

void Terrain::drawFull() {
	
}

void Terrain::import(ifstream & input, vector<Texture *> & textures, vector<char *> & heightMaps) {
	char line[256];
	char key[256];
	char value[256];
	char * str;
	
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
			printf("WARNING: Encountered unexpected property when parsing terrain object: \"%s\"", key);
			delete [] str;
		}
	}

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
	
	// generate the terrain from the height map
	double terrainSizeX = maxPoint.x - minPoint.x;
	double terrainSizeY = maxPoint.y - minPoint.y;
	double terrainSizeZ = maxPoint.z - minPoint.z;
	
//	double tileSizeX = terrainSizeX / width;
//	double tileSizeY = terrainSizeY / height;
	
//	double tilingTextureWidth = (textureMap->width - 1) / (double) textureMap->width;
//	double tilingTextureHeight = (textureMap->height - 1) / (double) textureMap->height;
	
	int currentPoint = 0;
	double x, y, z, tx, ty;
	points = new GamePoint[(width + 1) * (height  + 1)];
	for(int i=0;i<width+1;i++) {
		for(int j=0;j<height+1;j++) {
			x = ((i / (double) width) * terrainSizeX) + minPoint.x;
			y = ((j / (double) height) * terrainSizeY) + minPoint.y;
			z = ((scaleHeight(i, j, heightMapData) / 255.0) * terrainSizeZ) + minPoint.z;

			if(tiled == 1) {
				tx = i;
				ty = j;
//				tx = i * tilingTextureWidth;
//				ty = j * tilingTextureHeight;
			}
			else {
				tx = i / textureMap->width;
				ty = j / textureMap->height;
//				tx = i * (textureMap->width / terrainSizeX);
//				ty = j * (textureMap->height / terrainSizeY);
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
}

double Terrain::scaleHeight(int x, int y, int * heightMapData) {
	int sum = 0;
	int vertexCount = 0;
	
	if(x > 0 && y > 0) {
		sum += heightMapData[((x-1) * width) + (y-1)];
		vertexCount++;
	}
	
	if(x > 0 && y < height) {
		sum += heightMapData[((x-1) * width) + y];
		vertexCount++;
	}
	
	if(y > 0 && x < width) {
		sum += heightMapData[(x * width) + (y-1)];
		vertexCount++;
	}
	
	if(x < width && y < height) {
		sum += heightMapData[(x * width) + y];
		vertexCount++;
	}
	
	return (sum / (double) vertexCount); 
}