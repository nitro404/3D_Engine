
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                        Faces                                            //
//*****************************************************************************************//

void Face::import (ifstream &input, World * world) {
	char line [256]; //Working variable...

	//Input the header.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long faceIndex = atoi (line); //Only useful for debugging or browsing.

	//Input the properties (either the texture property or nothing at all; not actually storing in a dictionary)...
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long propertiesSize = atoi (line); CLEAR_THE_LINE;
	for (long propertiesIndex = 0; propertiesIndex < propertiesSize; propertiesIndex++) {
		SKIP_TO_ENDLINE;
		char key [256]; char value [256]; value [0] = '\0';
		sscanf (line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
		textureName = new char [strlen (value) + 1];
		strcpy (textureName, value);
		break; //propertiesSize should be 1 but just in case it's not...
	}

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
		SKIP_TO_COMMA; point->nx = atof (line);
		SKIP_TO_COMMA; point->ny = atof (line);
		SKIP_TO_COMMA; point->nz = atof (line);
		SKIP_TO_COMMA; point->tx = atof (line);
		SKIP_TO_ENDLINE; point->ty = atof (line);
		points.push_back (point);
	}
}

void Face::export (long indent, ofstream &output, long faceIndex, vector<char*> & textureNames) {
	int textureIndex = -1;

	//Output a simple header..."
	output << "\n" << indentation (indent) << "Face: " << faceIndex << ";";

	//Output the properties...
	output << "\n" << indentation (indent + 1) << "Properties: " << (textureName == NULL ? 0 : 1) << ";";
	if (textureName != NULL) {
		for(int i=0;i<textureNames.size();i++) {
			if(strcmp(textureName, textureNames.at(i)) == 0) {
				textureIndex = i;
				break;
			}
		}
//		output << "\n" << indentation (indent + 2) << "\"" << "texture" << "\" => \"" << textureName << "\"";
		output << "\n" << indentation (indent + 2) << "\"" << "texture" << "\" => \"" << textureIndex << "\"";
	}

	//Output the points...
	output << "\n" << indentation (indent + 1) << "Points: " << points.size () << "; //x, y, z, nx, ny, nz, tx, ty";
	loopVector (pointIndex, points)
		GamePoint &point = *(points [pointIndex]);
		output << "\n" << indentation (indent + 2) << 
			point.x << ", " << point.y << ", " << point.z << ", " << 
			point.nx << ", " << point.ny << ", " << point.nz << ", " << 
			point.tx << ", " << point.ty << ";";
	endloop
}