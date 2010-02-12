#include "includes.all"

double Geometry::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - transformation.m41, 2) + pow(p.y - transformation.m42, 2) + pow(p.z - transformation.m43, 2) );
}

void Geometry::tick () {
	
}

void Geometry::draw () {
	//Draw the faces in this object.
	glPushMatrix();
	Transformation & normal = transformation.normal();
	glMultMatrixd(normal);
	for(int i=0;i<faces.size();i++) {
		faces.at(i)->draw();
	}
	glPopMatrix(); 
}

void Geometry::import (ifstream &input, TextureCollection & textures) {
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
	
	//Input the faces.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long facesSize = atoi (line);
	for (long faceIndex = 0; faceIndex < facesSize; faceIndex++) {
		Face *face = new Face;
		face->import (input, textures);
		faces.push_back (face);
	}
}
