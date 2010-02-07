
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                        Faces                                            //
//*****************************************************************************************//

void Face::tick () {
	//This could change textures dynamically or change texture coordinates or do nothing.
}

void Face::draw () {
	//Draw this face... (see Game::draw () in the builder for an example).
	//Permit blending if it's a texture with alpha...
	if (texture->type == RGBAType) {
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable (GL_BLEND);
	} else {
		glDisable (GL_BLEND);
	}
	
	//Activate the texture to be drawn.
	texture->activate();
	
	//Setup one polygon to be drawn and draw it.
	glBegin (GL_POLYGON);
	for (long pointIndex = 0; pointIndex < points.size(); pointIndex++) {
		GamePoint &point = *points.at(pointIndex);
		glTexCoord2d (point.tx, point.ty);
		glVertex3d (point.x, point.y, point.z); //Must be last.
	}
	glEnd ();
}

void Face::draw (double red, double green, double blue, double alpha) {
	//Activate the texture to be drawn.
	texture->activate();
	
	//Setup one polygon to be drawn and draw it.
	glDisable(GL_CULL_FACE);
	glBegin(GL_POLYGON);
	glColor4d(red, green, blue, alpha);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
	for (long pointIndex = 0; pointIndex < points.size(); pointIndex++) {
		GamePoint &point = *points.at(pointIndex);
		glTexCoord2d (point.tx, point.ty);
		glVertex3d (point.x, point.y, point.z); //Must be last.
	}
	glColor4d(1, 1, 1, 1);
	glDisable(GL_BLEND);
	glEnd();
	glEnable(GL_CULL_FACE);
}

void Face::import (ifstream &input, TextureCollection & textures) {
	char line [256]; //Working variable...

	//Input the header.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long faceIndex = atoi (line); //Only useful for debugging or browsing.

	//Input the properties (either the texture property or nothing at all; not actually storing in a dictionary)...
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long propertiesSize = atoi (line); CLEAR_THE_LINE;
	char * textureIndex;
//	for (long propertiesIndex = 0; propertiesIndex < propertiesSize; propertiesIndex++) {
		SKIP_TO_ENDLINE;
		char key [256]; char value [256]; value [0] = '\0';
		sscanf (line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
		textureIndex = new char [strlen (value) + 1];
		strcpy (textureIndex, value);
//		break; //propertiesSize should be 1 but just in case it's not...
//	}
	texture = textures.at(atoi(textureIndex));
	delete [] textureIndex;

	//Input the texture.
	//Only if this code is in the game rather than the builder...
	//texture = textureName == NULL ? NULL : world->textureFor (textureName);

	//Input the points.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long pointsSize = atoi (line); CLEAR_THE_LINE;
	for (long pointIndex = 0; pointIndex < pointsSize; pointIndex++) {
		GamePoint *point = new GamePoint;
		SKIP_TO_COMMA; point->x = atof (line);
		SKIP_TO_COMMA; point->y = atof (line);
		SKIP_TO_COMMA; point->z = atof (line);
		SKIP_TO_COMMA; //point->nx = atof (line);
		SKIP_TO_COMMA; //point->ny = atof (line);
		SKIP_TO_COMMA; //point->nz = atof (line);
		SKIP_TO_COMMA; point->tx = atof (line);
		SKIP_TO_ENDLINE; point->ty = atof (line);
		points.push_back (point);
	}
}

void Face::import (ifstream &input) {
	char line [256];

	//Input the header
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	long faceIndex = atoi (line);
	
	//Input the points.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long pointsSize = atoi (line); CLEAR_THE_LINE;
	for (long pointIndex = 0; pointIndex < pointsSize; pointIndex++) {
		GamePoint *point = new GamePoint;
		SKIP_TO_COMMA; point->x = atof (line);
		SKIP_TO_COMMA; point->y = atof (line);
		SKIP_TO_COMMA; point->z = atof (line);
		SKIP_TO_COMMA;
		SKIP_TO_COMMA;
		SKIP_TO_COMMA;
		SKIP_TO_COMMA; point->tx = atof (line);
		SKIP_TO_ENDLINE; point->ty = atof (line);
		points.push_back (point);
	}
}
