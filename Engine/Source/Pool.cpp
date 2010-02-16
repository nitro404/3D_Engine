#include "Pool.h"

double Pool::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - transformation.m41, 2) + pow(p.y - transformation.m42, 2) + pow(p.z - transformation.m43, 2) );
}

bool Pool::insideOf(Point & p) const {
	return p.x >= minX && p.x <= maxX &&
		   p.y >= minY && p.y <= maxY &&
		   p.z >= minZ && p.z <= maxZ;
}

void Pool::tick() {
	for(int i=0;i<faces.size();i++) {
		faces.at(i)->tick();
	}
}

void Pool::draw() {
	glPushMatrix();
	Transformation & normal = transformation.normal();
	glMultMatrixd(normal);
	for(int i=0;i<faces.size();i++) {
		faces.at(i)->draw(waterColour);
	}
	glPopMatrix(); 
}

void Pool::import(ifstream & input, vector<AnimatedTexture *> & animatedTextures) {
	int i, j;
	char * line;
	char * key;
	char * value;
	char * str;
	line = new char[256];
	key = new char[256];
	value = new char[256];
	
	//Input the transformation.
	input.getline(line, 256, ':'); input.getline(line, 256, '\n');
	
	//The standard transformation matrix elements...
	Transformation &normal = transformation.normal ();
	input.getline(line, 256, ','); normal.m11 = atof (line);
	input.getline(line, 256, ','); normal.m12 = atof (line);
	input.getline(line, 256, ','); normal.m13 = atof (line);
	input.getline(line, 256, ','); normal.m14 = atof (line);
	
	input.getline(line, 256, ','); normal.m21 = atof (line);
	input.getline(line, 256, ','); normal.m22 = atof (line);
	input.getline(line, 256, ','); normal.m23 = atof (line);
	input.getline(line, 256, ','); normal.m24 = atof (line);
	
	input.getline(line, 256, ','); normal.m31 = atof (line);
	input.getline(line, 256, ','); normal.m32 = atof (line);
	input.getline(line, 256, ','); normal.m33 = atof (line);
	input.getline(line, 256, ','); normal.m34 = atof (line);
	
	input.getline(line, 256, ','); normal.m41 = atof (line);
	input.getline(line, 256, ','); normal.m42 = atof (line);
	input.getline(line, 256, ','); normal.m43 = atof (line);
	input.getline(line, 256, ';'); normal.m44 = atof (line);
	input.getline(line, 256, '\n');

	//The inverse transformation matrix elements...
	Transformation &inverse = transformation.inverse;
	input.getline(line, 256, ','); inverse.m11 = atof (line);
	input.getline(line, 256, ','); inverse.m12 = atof (line);
	input.getline(line, 256, ','); inverse.m13 = atof (line);
	input.getline(line, 256, ','); inverse.m14 = atof (line);
	
	input.getline(line, 256, ','); inverse.m21 = atof (line);
	input.getline(line, 256, ','); inverse.m22 = atof (line);
	input.getline(line, 256, ','); inverse.m23 = atof (line);
	input.getline(line, 256, ','); inverse.m24 = atof (line);
	
	input.getline(line, 256, ','); inverse.m31 = atof (line);
	input.getline(line, 256, ','); inverse.m32 = atof (line);
	input.getline(line, 256, ','); inverse.m33 = atof (line);
	input.getline(line, 256, ','); inverse.m34 = atof (line);
	
	input.getline(line, 256, ','); inverse.m41 = atof (line);
	input.getline(line, 256, ','); inverse.m42 = atof (line);
	input.getline(line, 256, ','); inverse.m43 = atof (line);
	input.getline(line, 256, ';'); inverse.m44 = atof (line);
	input.getline(line, 256, '\n');
	
	//Input the properties
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfProperties = atoi(line);
	input.getline(line, 256, '\n');
	for(int propertyIndex=0;propertyIndex<numberOfProperties;propertyIndex++) {
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf(line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
		str = new char[strlen(value) + 1];
		strcpy(str, value);
		
		//Parse properties to local variables
		if(stricmp(key, "name") == 0) {
			name = str;
		}
		else if(stricmp(key, "watercolour") == 0) {
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
	minX += transformation.position().x;
	minY += transformation.position().y;
	minZ += transformation.position().z;
	maxX += transformation.position().x;
	maxY += transformation.position().y;
	maxZ += transformation.position().z;

	delete [] line;
	delete [] key;
	delete [] value;
}
