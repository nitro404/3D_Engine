#include "Environment.h"

Environment::Environment() {
	skyboxTextures = new Texture*[6];
}

Environment::~Environment() {
	delete [] name;
	delete [] skyboxTextures;
}

void Environment::tick() {
	
}

void Environment::draw() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	glPushIdentity();
		glRotated(-(camera->xRotation), 1, 0, 0);
		glRotated(-(camera->yRotation), 0, 1, 0);
		
		glScaled(100, 100, 100);

		for(int i=0;i<6;i++) {
			skyboxTextures[i]->activate();
			glCallList(skyboxList + i);
		}
	glPopMatrix();
	
	glEnable(GL_DEPTH_TEST);
}

void Environment::import(ifstream & input, vector<Texture *> & textures) {
	char line[256];
	char key[256];
	char value[256];
	char * str;
	
	//Input the properties
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfProperties = atoi(line);
	input.getline(line, 256, '\n');
	for(int propertyIndex=0;propertyIndex<numberOfProperties;propertyIndex++) {
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf_s(line, " \"%[^\"]\" => \"%[^\"]\"", key, 256, value, 256);
		str = new char[strlen(value) + 1];
		strcpy_s(str, strlen(value) + 1, value);
		
		//Parse properties to local variables
		if(_stricmp(key, "name") == 0) {
			name = str;
		}
		else if(_stricmp(key, "ambientlight") == 0) {
			char * temp = strchr(str + sizeof(char), ' ');
			*temp = '\0';
			ambientLight.setRed(atoi(str));
			temp += sizeof(char);
			char * temp2 = strchr(temp, ' ');
			*temp2 = '\0';
			ambientLight.setGreen(atoi(temp));
			temp2 += sizeof(char);
			ambientLight.setBlue(atoi(temp2));
			delete [] str;
		}
		else if(_stricmp(key, "cloudlayers") == 0) {
			cloudLayers = atoi(str);
			delete [] str;
		}
		else if(_stricmp(key, "skyboxtexture-left") == 0) {
			skyboxTextures[0] = textures.at(atoi(str));
			delete [] str;
		}
		else if(_stricmp(key, "skyboxtexture-right") == 0) {
			skyboxTextures[1] = textures.at(atoi(str));
			delete [] str;
		}
		else if(_stricmp(key, "skyboxtexture-front") == 0) {
			skyboxTextures[2] = textures.at(atoi(str));
			delete [] str;
		}
		else if(_stricmp(key, "skyboxtexture-back") == 0) {
			skyboxTextures[3] = textures.at(atoi(str));
			delete [] str;
		}
		else if(_stricmp(key, "skyboxtexture-up") == 0) {
			skyboxTextures[4] = textures.at(atoi(str));
			delete [] str;
		}
		else if(_stricmp(key, "skyboxtexture-down") == 0) {
			skyboxTextures[5] = textures.at(atoi(str));
			delete [] str;
		}
		else if(_stricmp(key, "skycolor") == 0) {
			char * temp = strchr(str + sizeof(char), ' ');
			*temp = '\0';
			skyColour.setRed(atoi(str));
			temp += sizeof(char);
			char * temp2 = strchr(temp, ' ');
			*temp2 = '\0';
			skyColour.setGreen(atoi(temp));
			temp2 += sizeof(char);
			skyColour.setBlue(atoi(temp2));
			delete [] str;
		}
		else if(_stricmp(key, "surrounds") == 0) {
			 surrounds = atoi(str);
			 delete [] str;
		}
		else {
			printf("WARNING: Encountered unexpected property when parsing environment object: \"%s\".\n", key);
			delete [] str;
		}
	}

	double width = (255.0 / 256.0); // temporarily hardcoded

	skyboxList = glGenLists(6);
	glNewList(skyboxList, GL_COMPILE);
		glBegin(GL_POLYGON); // left side
			glTexCoord2d(1 - width, width);
			glVertex3d(-0.5, 0.5, 0.5);
			glTexCoord2d(1 - width, 1 - width);
			glVertex3d(-0.5, -0.5, 0.5);
			glTexCoord2d(width, 1 - width);
			glVertex3d(-0.5, -0.5, -0.5);
			glTexCoord2d(width, width);
			glVertex3d(-0.5, 0.5, -0.5);
		glEnd();
	glEndList();
	glNewList(skyboxList + 1, GL_COMPILE);
		glBegin(GL_POLYGON); // right side
			glTexCoord2d(1 - width, width);
			glVertex3d(0.5, 0.5, -0.5);
			glTexCoord2d(1 - width, 1 - width);
			glVertex3d(0.5, -0.5, -0.5);
			glTexCoord2d(width, 1 - width);
			glVertex3d(0.5, -0.5, 0.5);
			glTexCoord2d(width, width);
			glVertex3d(0.5, 0.5, 0.5);
		glEnd();
	glEndList();
	glNewList(skyboxList + 2, GL_COMPILE);
		glBegin(GL_POLYGON); // front side
			glTexCoord2d(1 - width, width);
			glVertex3d(-0.5, 0.5, -0.5);
			glTexCoord2d(1 - width, 1 - width);
			glVertex3d(-0.5, -0.5, -0.5);
			glTexCoord2d(width, 1 - width);
			glVertex3d(0.5, -0.5, -0.5);
			glTexCoord2d(width, width);
			glVertex3d(0.5, 0.5, -0.5);
		glEnd();
	glEndList();
	glNewList(skyboxList + 3, GL_COMPILE);
		glBegin(GL_POLYGON); // back side
			glTexCoord2d(1 - width, width);
			glVertex3d(0.5, 0.5, 0.5);
			glTexCoord2d(1 - width, 1 - width);
			glVertex3d(0.5, -0.5, 0.5);
			glTexCoord2d(width, 1 - width);
			glVertex3d(-0.5, -0.5, 0.5);
			glTexCoord2d(width, width);
			glVertex3d(-0.5, 0.5, 0.5);
		glEnd();
	glEndList();
	glNewList(skyboxList + 4, GL_COMPILE);
		glBegin(GL_POLYGON); // top side
			glTexCoord2d(width, 1 - width);
			glVertex3d(-0.5, 0.5, 0.5);
			glTexCoord2d(width, width);
			glVertex3d(-0.5, 0.5, -0.5);
			glTexCoord2d(1 - width, width);
			glVertex3d(0.5, 0.5, -0.5);
			glTexCoord2d(1 - width, 1 - width);
			glVertex3d(0.5, 0.5, 0.5);
		glEnd();
	glEndList();
	glNewList(skyboxList + 5, GL_COMPILE);
		glBegin(GL_POLYGON); // bottom side
			glTexCoord2d(width, 1 - width);
			glVertex3d(-0.5, -0.5, -0.5);
			glTexCoord2d(width, width);
			glVertex3d(-0.5, -0.5, 0.5);
			glTexCoord2d(1 - width, width);
			glVertex3d(0.5, -0.5, 0.5);
			glTexCoord2d(1 - width, 1 - width);
			glVertex3d(0.5, -0.5, -0.5);
		glEnd();
	glEndList();
}
