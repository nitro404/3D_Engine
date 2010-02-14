#include "includes.all"

void AnimatedFace::tick() {
	texture = animatedTexture->textures.at((int) textureIndex);
	textureIndex += animationSpeed * DT;
	if(textureIndex >= animatedTexture->frames) {
		textureIndex = 0;
	}
}

void AnimatedFace::draw() {
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

void AnimatedFace::draw(double red, double green, double blue, double alpha) {
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

void AnimatedFace::import(ifstream &input, AnimatedTextureCollection & animatedTextures) {
	char line [256]; //Working variable...

	//Input the header.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	int currentIndex = atoi (line);
	CLEAR_THE_LINE;

	//Input the properties (either the texture property or nothing at all; not actually storing in a dictionary)...
//	SKIP_TO_COLON;
//	SKIP_TO_SEMICOLON;
//	int propertiesSize = atoi (line);
//	CLEAR_THE_LINE;
//	if(propertiesSize != 1) {
//		quit("AnimatedFace cannot have more than one property.");
//	}
	
	SKIP_TO_ENDLINE;
	char key [256]; char value [256]; value [0] = '\0';
	sscanf (line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
	
	//Get the animated texture
	animatedTexture = animatedTextures.at(atoi(value));
	if(animationSpeed < 0) {
		animationSpeed = animatedTexture->speed;
	}

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
		points.push_back(point);
	}
}
