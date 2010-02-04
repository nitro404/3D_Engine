
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                       Waypoint                                          //
//*****************************************************************************************//

Waypoint::Waypoint() {
	
}

Waypoint::~Waypoint() {
	
}

void Waypoint::addNeighbour(Waypoint * w) {
	if(w == NULL) { return; }
	
	bool valid = true;

	for(int i=0;i<this->neighbours.size();i++) {
		if(stricmp(this->neighbours.at(i)->name, w->name) == 0 || stricmp(this->name, w->name) == 0) {
			valid = false;
		}
	}
	
	if(valid) {
		this->neighbours.push_back(w);
	}
}

void Waypoint::tick () {

}

void Waypoint::draw () {
	
}

void Waypoint::import (ifstream &input) {
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
		else if(stricmp(key, "neighbours") == 0) {
			neighbourValues = string;
		}
	}

}

void Waypoint::printOn(ostream & o) const {
	o << "Waypoint " << name << ": ";
	for(int i=0;i<neighbours.size();i++) {
		o << neighbours.at(i)->name;
		if(i < neighbours.size() - 1) {
			o << ", ";
		}
	}
}

ostream & operator << (ostream & o, const Waypoint & x) {
	x.printOn(o);
	return o;
}
