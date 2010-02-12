#include "includes.all"

char textureDirectory2 [_MAX_DIR] = {'T','E','X','T','U','R','E','S','\0'};

double AnimatedFace::animationSpeed = 14;

void AnimatedFace::tick () {
	textureIndex += animationSpeed * DT;
	texture = textures[(int) textureIndex];
	if(textureIndex >= 31) {
		textureIndex = 0;
	}
}

void AnimatedFace::draw () {
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
	glEnd();
}

void AnimatedFace::draw (double red, double green, double blue, double alpha) {
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

void AnimatedFace::import (ifstream &input, TextureCollection & worldTextures) {
	char line [256]; //Working variable...

	//Input the header.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	long faceIndex = atoi (line);

	//Input the properties (either the texture property or nothing at all; not actually storing in a dictionary)...
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	long propertiesSize = atoi (line); CLEAR_THE_LINE;
	if(propertiesSize != 1) {
		quit("AnimatedFace cannot have more than one property.");
	}
	SKIP_TO_ENDLINE;
	char key [256]; char value [256]; value [0] = '\0';
	sscanf (line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
	
	textures = new Texture*[32];
	string texturePath;
	char * index = new char[3];
	for(int i=0;i<32;i++) {
		texturePath.append(textureDirectory2);
		texturePath.append("/");
		texturePath.append("caust");
		if(i<10) { texturePath.append("0"); }
		itoa(i, index, 10);
		texturePath.append(index);
		texturePath.append(".bmp");
		textures[i] = Texture::readTexture((char *) texturePath.c_str());
		if(textures[i] != NULL) {
			textures[i]->load();
		}
		texturePath.erase();
	}
	texture = textures[0];

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

void AnimatedFace::import (ifstream &input) {
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
