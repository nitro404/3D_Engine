
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                       Pool                                              //
//*****************************************************************************************//

double Pool::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - transformation.m41, 2) + pow(p.y - transformation.m42, 2) + pow(p.z - transformation.m43, 2) );
}

bool Pool::insideOf(Point & p) const {
	return p.x >= minX && p.x <= maxX &&
		   p.y >= minY && p.y <= maxY &&
		   p.z >= minZ && p.z <= maxZ;
}

void Pool::tick () {
	for(int i=0;i<faces.size();i++) {
		faces.at(i)->tick();
	}
}

void Pool::draw () {
	glPushMatrix();
	Transformation & normal = transformation.normal();
	glMultMatrixd(normal);
	for(int i=0;i<faces.size();i++) {
		faces.at(i)->draw(red, green, blue, alpha);
	}
	glPopMatrix(); 
}

void Pool::import (ifstream &input, TextureCollection & textures) {
	char line [256];
	
	//Input the transformation.
	SKIP_TO_COLON; CLEAR_THE_LINE;
	
	//The standard transformation matrix elements...
	Transformation &normal = transformation.normal ();
	SKIP_TO_COMMA; normal.m11 = atof (line);
	SKIP_TO_COMMA; normal.m12 = atof (line);
	SKIP_TO_COMMA; normal.m13 = atof (line);
	SKIP_TO_COMMA; normal.m14 = atof (line);
	
	SKIP_TO_COMMA; normal.m21 = atof (line);
	SKIP_TO_COMMA; normal.m22 = atof (line);
	SKIP_TO_COMMA; normal.m23 = atof (line);
	SKIP_TO_COMMA; normal.m24 = atof (line);
	
	SKIP_TO_COMMA; normal.m31 = atof (line);
	SKIP_TO_COMMA; normal.m32 = atof (line);
	SKIP_TO_COMMA; normal.m33 = atof (line);
	SKIP_TO_COMMA; normal.m34 = atof (line);
	
	SKIP_TO_COMMA; normal.m41 = atof (line);
	SKIP_TO_COMMA; normal.m42 = atof (line);
	SKIP_TO_COMMA; normal.m43 = atof (line);
	SKIP_TO_SEMICOLON; normal.m44 = atof (line);
	CLEAR_THE_LINE;

	//The inverse transformation matrix elements...
	Transformation &inverse = transformation.inverse;
	SKIP_TO_COMMA; inverse.m11 = atof (line);
	SKIP_TO_COMMA; inverse.m12 = atof (line);
	SKIP_TO_COMMA; inverse.m13 = atof (line);
	SKIP_TO_COMMA; inverse.m14 = atof (line);
	
	SKIP_TO_COMMA; inverse.m21 = atof (line);
	SKIP_TO_COMMA; inverse.m22 = atof (line);
	SKIP_TO_COMMA; inverse.m23 = atof (line);
	SKIP_TO_COMMA; inverse.m24 = atof (line);
	
	SKIP_TO_COMMA; inverse.m31 = atof (line);
	SKIP_TO_COMMA; inverse.m32 = atof (line);
	SKIP_TO_COMMA; inverse.m33 = atof (line);
	SKIP_TO_COMMA; inverse.m34 = atof (line);
	
	SKIP_TO_COMMA; inverse.m41 = atof (line);
	SKIP_TO_COMMA; inverse.m42 = atof (line);
	SKIP_TO_COMMA; inverse.m43 = atof (line);
	SKIP_TO_SEMICOLON; inverse.m44 = atof (line);
	CLEAR_THE_LINE;
	
	//Input the properties
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long propertiesSize = atoi (line); CLEAR_THE_LINE;
	for (long propertiesIndex = 0; propertiesIndex < propertiesSize; propertiesIndex++) {
		SKIP_TO_ENDLINE;
		char key [256]; char value [256]; value [0] = '\0';
		sscanf (line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
		convertToLowercase (key);
		char *str = new char [strlen (value) + 1]; strcpy (str, value);
		
		//Parse properties to local variables
		if(stricmp(key, "name") == 0) {
			name = str;
		}
		else if(stricmp(key, "watercolour") == 0) {
			char * temp = strchr(str + sizeof(char), ' ');
			*temp = '\0';
			red = atof(str);
			temp += sizeof(char);
			char * temp2 = strchr(temp, ' ');
			*temp2 = '\0';
			green = atof(temp);
			temp2 += sizeof(char);
			char * temp3 = strchr(temp2, ' ');
			*temp3 = '\0';
			blue = atof(temp2);
			temp3 += sizeof(char);
			alpha = atof(temp3);
			delete [] str;
		}
	}
	
	//Input the faces.
	/*SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long facesSize = atoi (line);
	for (long faceIndex = 0; faceIndex < facesSize; faceIndex++) {
		Face *face = new Face;
		face->import (input, textures);
		faces.push_back (face);
	}*/
	
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long facesSize = atoi (line);
	for (long faceIndex = 0; faceIndex < facesSize; faceIndex++) {
		AnimatedFace *face = new AnimatedFace;
		face->import (input, textures);
		faces.push_back (face);
	}

	GamePoint * p;
	
	for(int i=0;i<faces.size();i++) {
		for(int j=0;j<faces.at(i)->points.size();j++) {
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
}
