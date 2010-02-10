
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                      Environment                                        //
//*****************************************************************************************//

char textureDirectory3 [_MAX_DIR] = {'T','E','X','T','U','R','E','S','\0'};

void Environment::tick () {
	
}

void Environment::draw () {
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
		skyboxTextures.at(0)->activate();
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
		skyboxTextures.at(1)->activate();
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
		skyboxTextures.at(2)->activate();
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
		skyboxTextures.at(3)->activate();
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
		skyboxTextures.at(4)->activate();
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
		skyboxTextures.at(5)->activate();
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

void Environment::import (ifstream &input, TextureCollection & textures) {
	char line [256];
	
	//Input the properties
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long propertiesSize = atoi (line); CLEAR_THE_LINE;
	for (long propertiesIndex = 0; propertiesIndex < propertiesSize; propertiesIndex++) {
		SKIP_TO_ENDLINE;
		char key [256]; char value [256]; value [0] = '\0';
		sscanf (line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
		convertToLowercase (key);
		char *str = new char [strlen (value) + 1]; strcpy (str, value);
		
		//Parse properties to local variables
		if(stricmp(key, "name") == 0) {
			name = str;
		}
		else if(stricmp(key, "ambientlight") == 0) {
			ambientLight = new double[3];
			char * temp = strchr(str + sizeof(char), ' ');
			*temp = '\0';
			ambientLight[0] = atof(str);
			temp += sizeof(char);
			char * temp2 = strchr(temp, ' ');
			*temp2 = '\0';
			ambientLight[1] = atof(temp);
			temp2 += sizeof(char);
			ambientLight[2] = atof(temp2);
			delete [] str;
		}
		else if(stricmp(key, "cloudlayers") == 0) {
			cloudLayers = atoi(str);
			delete [] str;
		}
		else if(stricmp(key, "skyboxtexture") == 0) {
			string tempstr;
			string texturePath;
			string texturePathTGA;
			string texturePathBMP;
			Texture * newTexture;

			tempstr.append(textureDirectory3);
			tempstr.append("/");
			tempstr.append(str);
			
			for(int j=0;j<6;j++) {
				texturePath.append(tempstr);
				texturePath.append(skyboxTextureExtensions[j]);
				texturePathTGA.append(texturePath);
				texturePathTGA.append(".tga");
				newTexture = Texture::readTexture((char *) texturePathTGA.c_str());
				if(newTexture == NULL) {
					texturePathBMP.append(texturePath);
					texturePathBMP.append(".bmp");
					newTexture = Texture::readTexture((char *) texturePathBMP.c_str());
				}
				if(newTexture != NULL) {
					newTexture->load();
					skyboxTextures.push_back(newTexture);
				}
				else {
					prompt("Missing texture or unknown format: %s", str);
				}
				texturePath.erase();
				texturePathTGA.erase();
				texturePathBMP.erase();
			}
			
			delete [] str;
		}
		else if(stricmp(key, "skycolor") == 0) {
			skyColour = new double[3];
			char * temp = strchr(str + sizeof(char), ' ');
			*temp = '\0';
			skyColour[0] = atof(str);
			temp += sizeof(char);
			char * temp2 = strchr(temp, ' ');
			*temp2 = '\0';
			skyColour[1] = atof(temp);
			temp2 += sizeof(char);
			skyColour[2] = atof(temp2);
			delete [] str;
		}
		else if(stricmp(key, "surrounds") == 0) {
			 surrounds = atoi(str);
			 delete [] str;
		}
	}
}
