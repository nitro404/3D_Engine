#include "Environment.h"

void Environment::tick() {
	
}

void Environment::draw() {
	double width = (255.0 / 256.0);

	double rotationX = -(camera->xRotation);
	double rotationY = -(camera->yRotation);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	glPushIdentity();
	
	glRotated(rotationX, 1, 0, 0);
	glRotated(rotationY, 0, 1, 0);
	
	glScaled(100, 100, 100);
	
	if(surrounds == 1) {
		// left side
		skyboxTextures[0]->activate();
		glBegin(GL_POLYGON);
		glTexCoord2d(1 - width, width);
		glVertex3d(-0.5, 0.5, 0.5);
		glTexCoord2d(1 - width, 1 - width);
		glVertex3d(-0.5, -0.5, 0.5);
		glTexCoord2d(width, 1 - width);
		glVertex3d(-0.5, -0.5, -0.5);
		glTexCoord2d(width, width);
		glVertex3d(-0.5, 0.5, -0.5);
		glEnd();
		
		// right side
		skyboxTextures[1]->activate();
		glBegin(GL_POLYGON);
		glTexCoord2d(1 - width, width);
		glVertex3d(0.5, 0.5, -0.5);
		glTexCoord2d(1 - width, 1 - width);
		glVertex3d(0.5, -0.5, -0.5);
		glTexCoord2d(width, 1 - width);
		glVertex3d(0.5, -0.5, 0.5);
		glTexCoord2d(width, width);
		glVertex3d(0.5, 0.5, 0.5);
		
		
		glEnd();

		// front side
		skyboxTextures[2]->activate();
		glBegin(GL_POLYGON);
		glTexCoord2d(1 - width, width);
		glVertex3d(-0.5, 0.5, -0.5);
		glTexCoord2d(1 - width, 1 - width);
		glVertex3d(-0.5, -0.5, -0.5);
		glTexCoord2d(width, 1 - width);
		glVertex3d(0.5, -0.5, -0.5);
		glTexCoord2d(width, width);
		glVertex3d(0.5, 0.5, -0.5);
		glEnd();

		// back side
		skyboxTextures[3]->activate();
		glBegin(GL_POLYGON);
		glTexCoord2d(1 - width, width);
		glVertex3d(0.5, 0.5, 0.5);
		glTexCoord2d(1 - width, 1 - width);
		glVertex3d(0.5, -0.5, 0.5);
		glTexCoord2d(width, 1 - width);
		glVertex3d(-0.5, -0.5, 0.5);
		glTexCoord2d(width, width);
		glVertex3d(-0.5, 0.5, 0.5);
		glEnd();

		// top side
		skyboxTextures[4]->activate();
		glBegin(GL_POLYGON);
		glTexCoord2d(1 - width, width);
		glVertex3d(-0.5, 0.5, 0.5);
		glTexCoord2d(1 - width, 1 - width);
		glVertex3d(-0.5, 0.5, -0.5);
		glTexCoord2d(width, 1 - width);
		glVertex3d(0.5, 0.5, -0.5);
		glTexCoord2d(width, width);
		glVertex3d(0.5, 0.5, 0.5);
		glEnd();

		// bottom side
		skyboxTextures[5]->activate();
		glBegin(GL_POLYGON);
		glTexCoord2d(1 - width, width);
		glVertex3d(-0.5, -0.5, -0.5);
		glTexCoord2d(1 - width, 1 - width);
		glVertex3d(-0.5, -0.5, 0.5);
		glTexCoord2d(width, 1 - width);
		glVertex3d(0.5, -0.5, 0.5);
		glTexCoord2d(width, width);
		glVertex3d(0.5, -0.5, -0.5);
		glEnd();
	}
	
	glPopMatrix();
	
	glEnable(GL_DEPTH_TEST);
}

void Environment::import(ifstream & input, vector<Texture *> & textures) {
	char * line;
	char * key;
	char * value;
	char * str;
	line = new char[256];
	key = new char[256];
	value = new char[256];
	
	//Input the properties
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfProperties = atoi(line);
	input.getline(line, 256, '\n');
	for(int propertyIndex=0;propertyIndex<numberOfProperties;propertyIndex++) {
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf(line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
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
	}
	
	delete [] line;
	delete [] key;
	delete [] value;
}
