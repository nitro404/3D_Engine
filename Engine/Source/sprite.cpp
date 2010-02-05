
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                       Sprite                                            //
//*****************************************************************************************//



void Sprite::tick () {

}

void Sprite::draw () {
	Transformation cameraMatrix;
	glGetMatrixd(cameraMatrix);
	Point newSpritePoint = (transformation.normal().position()) * cameraMatrix;
	
	if (picture->type == RGBAType) {
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable (GL_BLEND);
	} else {
		glDisable (GL_BLEND);
	}
	
	//Activate the texture to be drawn.
	picture->activate();
	
	//Setup one polygon to be drawn and draw it.
	glPushIdentity();
	/*
	glBegin(GL_POLYGON);
//GamePointCollection & points = faces.at(0)->points;
	for(long pointIndex = 0; pointIndex < points.size(); pointIndex++) {
		GamePoint &point = *points.at(pointIndex);
		glTexCoord2d (point.tx, point.ty);
		glVertex3d (point.x, point.y, point.z); //Must be last.
	}
	glEnd ();
	*/
for(int i=0;i<faces.size();i++) {
faces.at(i)->texture = picture;
faces.at(i)->draw();
}
	glPopMatrix();
}

void Sprite::import (ifstream &input, TextureCollection & textures, WaypointCollection & waypoints) {
	char line [256];
	
	//Input the transformation.
	SKIP_TO_COLON; CLEAR_THE_LINE;

	/*//Input the position
	for(int i=0;i<12;i++) {
		SKIP_TO_COMMA;
	}
	int x = atof(line);
	SKIP_TO_COMMA;
	int y = atof(line);
	SKIP_TO_COMMA;
	int z = atof(line);
	location = Point(x, y, z);
	CLEAR_THE_LINE;
	CLEAR_THE_LINE;*/

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
		else if(stricmp(key, "picture") == 0) {
			picture = textures.at(atoi(string));
			delete [] string;
		}
		else if(stricmp(key, "waypoint") == 0) {
			for(int i=0;i<waypoints.size();i++) {
				if(stricmp(waypoints.at(i)->name, string) == 0) {
					waypoint = waypoints.at(i);
					break;
				}
			}
			delete [] string;
		}
	}

	//Input the faces.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long facesSize = atoi (line);
	for (long faceIndex = 0; faceIndex < facesSize; faceIndex++) {
		Face *face = new Face;
		face->import(input);
		faces.push_back(face);
	}
}

void Sprite::printOn(ostream & o) const {
	o << "Sprite";
}

ostream & operator << (ostream & o, const Sprite & x) {
	x.printOn(o);
	return o;
}
