#include "TerrainMap.h"
#include "Includes.h"

#define USE_QUADS							0
#define USE_STRIPS							1
#define USE_FACE_GROUPS						2
#define USE_FACE_GROUPS_AND_FRUSTUM_CULLING 3 
#define TERRAIN_IMPLEMENTATION USE_QUADS

Terrain::Terrain(void)
{
	name = NULL;
	textureMap = NULL;
	width = 0;
	height = 0;
	minPoint = Point(0,0,0);
	maxPoint = Point(0,0,0);
	points = 0; // GamePoint[][]
	//drawType = GL_QUADS;
}

Terrain::~Terrain(void)
{
	if (name != NULL) {
		delete[] name;
	}
	if (points != NULL) {
		delete[] points;
	}
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
	else if (TERRAIN_IMPLEMENTATION == USE_FACE_GROUPS) {
		drawFull();
	}
	else if (TERRAIN_IMPLEMENTATION == USE_FACE_GROUPS) {
		drawFull();
	}
	else if (TERRAIN_IMPLEMENTATION == USE_FACE_GROUPS_AND_FRUSTUM_CULLING) {
		drawFrustrums();
	}
}

void Terrain::drawQuads() {
	for (int j = 0; j < height - 1;j++) {
		for (int i = 0; i < width;i++) {
			glBegin(GL_POLYGON);
				//Top-Left
				GamePoint* point = &points[i * width + j];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				//Bottom-Left
				point = &points[i * width + j + 1];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				//Bottom-Right
				point = &points[(i + 1) * width + j + 1];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				//Top-Right
				point = &points[(i + 1) * width + j];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);
			glEnd();
		}
	}
}

void Terrain::drawStrips() {
	GamePoint* point;

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

void Terrain::import(ifstream & input, vector<Texture *> & textures) {
	char line[256];
	char key[256];
	char value[256];
	char * str;
	
	//Input the properties
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
		
		//Parse properties to local variables
		if(_stricmp(key, "name") == 0) {
			name = str;
		}
		else if (_stricmp(key, "texturemap") == 0) {
			textureMap = textures.at(atoi(str));
			delete[] str;
		}
		else if (_stricmp(key, "width") == 0) {
			width = atoi(value);
			delete[] str;
		}
		else if (_stricmp(key, "height") == 0) {
			height = atoi(value);
			delete[] str;
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
		else if (_stricmp(key, "tiled") == 0) {
			
		}
		else if (_stricmp(key, "heightmap") == 0) {
			heightMapFile = str;
		}
	}
	
	delete [] line;
	delete [] key;
	delete [] value;
}