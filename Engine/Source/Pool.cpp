#include "Pool.h"

Pool::Pool() : transformation(NULL) { }

Pool::~Pool() {
	delete [] name;
	for(unsigned int i=0;i<faces.size();i++) {
		delete faces.at(i);
	}
	if(transformation != NULL) { delete transformation; }
}

double Pool::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - transformation->m41, 2) + pow(p.y - transformation->m42, 2) + pow(p.z - transformation->m43, 2) );
}

bool Pool::insideOf(Point & p) const {
	return p.x >= minX && p.x <= maxX &&
		   p.y >= minY && p.y <= maxY &&
		   p.z >= minZ && p.z <= maxZ;
}

void Pool::tick() {
	for(unsigned int i=0;i<faces.size();i++) {
		faces.at(i)->tick();
	}
}

void Pool::draw() {
	glPushMatrix();
		Transformation & normal = transformation->normal();
		glMultMatrixd(normal);
		for(unsigned int i=0;i<faces.size();i++) {
			faces.at(i)->draw(waterColour);
		}
	glPopMatrix(); 
}

void Pool::import(ifstream & input, vector<AnimatedTexture *> & animatedTextures) {
	unsigned int i, j;
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
		else if(_stricmp(key, "watercolour") == 0) {
			char * temp = strchr(str + sizeof(char), ' ');
			*temp = '\0';
			waterColour.setRed(atoi(str));
			temp += sizeof(char);
			char * temp2 = strchr(temp, ' ');
			*temp2 = '\0';
			waterColour.setGreen(atoi(temp));
			temp2 += sizeof(char);
			char * temp3 = strchr(temp2, ' ');
			*temp3 = '\0';
			waterColour.setBlue(atoi(temp2));
			temp3 += sizeof(char);
			waterColour.setAlpha(atoi(temp3));
			delete [] str;
		}
		else {
			printf("WARNING: Encountered unexpected property when parsing pool object: \"%s\".\n", key);
			delete [] str;
		}
	}
	
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int facesSize = atoi(line);
	for(int faceIndex=0;faceIndex<facesSize;faceIndex++) {
		AnimatedFace * face = new AnimatedFace;
		face->import(input, animatedTextures);
		faces.push_back(face);
	}

	GamePoint * p;
	
	for(i=0;i<faces.size();i++) {
		for(j=0;j<faces.at(i)->points.size();j++) {
			p = faces.at(i)->points.at(j);
			if(i==0 && j==0) {
				maxX = p->x; minX = p->x;
				maxY = p->y; minY = p->y;
				maxZ = p->z; minZ = p->z;
			}
			else {
				if(p->x > maxX) { maxX = p->x; }
				if(p->y > maxY) { maxY = p->y; }
				if(p->z > maxZ) { maxZ = p->z; }
				if(p->x < minX) { minX = p->x; }
				if(p->y < minY) { minY = p->y; }
				if(p->z < minZ) { minZ = p->z; }
			}
		}
	}
	minX += transformation->position().x;
	minY += transformation->position().y;
	minZ += transformation->position().z;
	maxX += transformation->position().x;
	maxY += transformation->position().y;
	maxZ += transformation->position().z;
}
