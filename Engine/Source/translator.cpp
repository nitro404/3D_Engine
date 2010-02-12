#include "includes.all"

double Translator::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - transformation.m41, 2) + pow(p.y - transformation.m42, 2) + pow(p.z - transformation.m43, 2) );
}

void Translator::tick () {
	if(forward) {
		position += unitVector * (rateInMetersPerSecond * DT);
		distanceTravelled = sqrt(pow(position.x - origin.x, 2) + pow(position.y - origin.y, 2) + pow(position.z - origin.z, 2));
	}
	else {
		position -= unitVector * (rateInMetersPerSecond * DT);
		distanceTravelled = sqrt(pow(destination.x - position.x, 2) + pow(destination.y - position.y, 2) + pow(destination.z - position.z, 2));
	}
	if(distanceTravelled >= distanceToTravel) {
		forward = !forward;
	}
}

void Translator::draw () {
	//Draw the faces in this object.
	Point p = transformation.position();
	glPushMatrix();
	glTranslated(position.x, position.y, position.z);
	for(int i=0;i<faces.size();i++) {
		faces.at(i)->draw();
	}
	glPopMatrix ();
}

void Translator::import (ifstream &input, TextureCollection & textures) {
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
		else if(stricmp(key, "offset") == 0) {
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
			offset = Point(xPos, yPos, zPos);
			delete [] str;
		}
		else if(stricmp(key, "rateInMetersPerSecond") == 0) {
			rateInMetersPerSecond = atof(str);
			delete [] str;
		}
	}
	
	//Input the faces.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long facesSize = atoi (line);
	for (long faceIndex = 0; faceIndex < facesSize; faceIndex++) {
		Face *face = new Face;
		face->import (input, textures);
		faces.push_back (face);
	}

	origin = transformation.position();
	position = origin;
	destination = origin + offset;
	distanceToTravel = sqrt(pow(destination.x - origin.x, 2) + pow(destination.y - origin.y, 2) + pow(destination.z - origin.z, 2));
	unitVector = Vector((destination.x - origin.x) / distanceToTravel, (destination.y - origin.y) / distanceToTravel, (destination.z - origin.z) / distanceToTravel);
	distanceTravelled = 0;
}
