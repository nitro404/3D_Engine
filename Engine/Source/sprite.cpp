
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
	Point newPosition = position * cameraMatrix;
	
	if (picture->type == RGBAType) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
	
	picture->activate();
	
	glPushIdentity();

	glTranslated(newPosition.x, newPosition.y + (extent.y/2), newPosition.z);
	glScaled(extent.x, extent.y, extent.z);

	glBegin(GL_POLYGON);

	glNormal3d (0, 0, 1);
	glTexCoord2d (0, 1);
	glVertex3d (-0.5, 0.5, 0);

	glNormal3d (0, 0, 1);
	glTexCoord2d (0, 0);
	glVertex3d (-0.5, -0.5, 0);

	glNormal3d (0, 0, 1);
	glTexCoord2d (1, 0);
	glVertex3d (0.5, -0.5, 0);

	glNormal3d (0, 0, 1);
	glTexCoord2d (1, 1);
	glVertex3d (0.5, 0.5, 0);
	
	glEnd ();

	glPopMatrix();
	
}

void Sprite::import (ifstream &input, TextureCollection & textures, WaypointCollection & waypoints) {
	char line [256];
	
	//Input the position
	double x, y, z;
	SKIP_TO_COLON; SKIP_TO_COMMA;
	x = atof(line);
	SKIP_TO_COMMA;
	y = atof(line);
	SKIP_TO_SEMICOLON;
	z = atof(line);
	position = Point(x, y, z);
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

	//Input the bounding box
	Point max, min;

	// Input the maximum
	SKIP_TO_COLON; SKIP_TO_COMMA;
	x = atof(line);
	SKIP_TO_COMMA;
	y = atof(line);
	SKIP_TO_SEMICOLON;
	z = atof(line);
	max = Point(x, y, z);
	CLEAR_THE_LINE;

	// Input the minimum
	SKIP_TO_COLON; SKIP_TO_COMMA;
	x = atof(line);
	SKIP_TO_COMMA;
	y = atof(line);
	SKIP_TO_SEMICOLON;
	z = atof(line);
	min = Point(x, y, z);
	CLEAR_THE_LINE;
	
	// Calculate the center of the sprite
	center.x = (max.x + min.x) / 2;
	center.y = (max.y + min.y) / 2;
	center.z = (max.z + min.z) / 2;
	
	// Calculate the bottom center of the sprite
	bottomCenter.x = (max.x + min.x) / 2;
	bottomCenter.y = (max.y + min.y) / 2;
	bottomCenter.z = min.z;
	
	// Calculate the size of the sprite
	extent.x = max.x - min.x;
	extent.y = max.y - min.y;
	extent.z = max.z - min.z;
	
	if(waypoint != NULL) {
		position = waypoint->transformation.normal().position();
	}
}

