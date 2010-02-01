
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                       Translator                                        //
//*****************************************************************************************//

Translator::Translator() {
	forwardDirectionX = true;
	forwardDirectionY = true;
	forwardDirectionZ = true;
}

Translator::~Translator() {
	delete [] name;
}

void Translator::tick () {
	//This could make an object rotate or change size or do nothing.
	if(offset.x != 0) {
		if(forwardDirectionX) { currentLocation.x += rateInMetersPerSecond; }
		else				  { currentLocation.x -= rateInMetersPerSecond; }
	}
	if(offset.y != 0) {
		if(forwardDirectionY) { currentLocation.y += rateInMetersPerSecond; }
		else				  { currentLocation.y -= rateInMetersPerSecond; }
	}
	if(offset.z != 0) {
		if(forwardDirectionZ) { currentLocation.z += rateInMetersPerSecond; }
		else				  { currentLocation.z -= rateInMetersPerSecond; }
	}
	//Reverse
	if(currentLocation.x > origin.x + offset.x) {
		currentLocation.x = origin.x + offset.x;
		forwardDirectionX = false;
	}
	if(currentLocation.y > origin.y + offset.y) {
		currentLocation.y = origin.y + offset.y;
		forwardDirectionY = false;
	}
	if(currentLocation.z > origin.z + offset.z) {
		currentLocation.z = origin.z + offset.z;
		forwardDirectionZ = false;
	}
	
	//Forwards
	if(currentLocation.x < origin.x) {
		currentLocation.x = origin.x;
		forwardDirectionX = true;
	}
	if(currentLocation.y < origin.y) {
		currentLocation.y = origin.y;
		forwardDirectionY = true;
	}
	if(currentLocation.z < origin.z) {
		currentLocation.z = origin.z;
		forwardDirectionZ = true;
	}
}

void Translator::draw () {
	//Draw the faces in this object.
	Point p = transformation.position();
	glPushMatrix();
	glTranslated(currentLocation.x, currentLocation.y, currentLocation.z);
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
		char *string = new char [strlen (value) + 1]; strcpy (string, value);
		
		//Parse properties to local variables
		if(stricmp(key, "name") == 0) {
			name = string;
		}
		else if(stricmp(key, "offset") == 0) {
			double xPos, yPos, zPos;
			char * temp = strchr(string + sizeof(char), ' ');
			*temp = '\0';
			xPos = atof(string);
			temp += sizeof(char);
			char * temp2 = strchr(temp, ' ');
			*temp2 = '\0';
			yPos = atof(temp);
			temp2 += sizeof(char);
			zPos = atof(temp2);
			offset = Point(xPos, yPos, zPos);
			delete [] string;
		}
		else if(stricmp(key, "rateInMetersPerSecond") == 0) {
			rateInMetersPerSecond = atof(string);
			delete [] string;
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
	currentLocation = origin;
}

void Translator::printOn(ostream & o) const {
	o << "Translator: NAME=\"" << this->name << "\" OFFSET=\"(" << this->offset.x << ", " << this->offset.y << ", " << this->offset.z << ")\" RATE=\"" << rateInMetersPerSecond << "\"";
}

ostream & operator << (ostream & o, const Translator & x) {
	x.printOn(o);
	return o;
}
