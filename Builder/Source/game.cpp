
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                        Game                                             //
//*****************************************************************************************//

Game *game = NULL;

Texture	*Game::idlingTexture = NULL;
Texture	*Game::readingTexture = NULL;

double DT; 

void setupOpenGL () {
	glEnable (GL_CULL_FACE); glEnable (GL_DEPTH_TEST); glEnable (GL_TEXTURE_2D);
	glLineWidth (3.0);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (40.0, 1.0, 1.0, 100.0); //See resizeWindow for parameter explanation.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	GLfloat lightColor [] = {1.0f, 1.0f, 1.0f, 1.0f}; //white
	glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1f);
	glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
	glEnable (GL_LIGHT0); glEnable (GL_LIGHTING);
	glDisable (GL_LIGHTING); //We can deal with our own lighting.
	glEnable (GL_COLOR_MATERIAL); //Track color.

	glClearColor (0.0, 0.0, 0.0, 1.0); //black
	glClearDepth (1.0); //Far
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawBuffer (GL_BACK); glReadBuffer (GL_BACK);
	glEnable (GL_DEPTH_TEST); glDepthMask (GL_TRUE); glDepthFunc (GL_LEQUAL);
	glShadeModel (GL_SMOOTH);	

	glFrontFace (GL_CCW); glCullFace (GL_BACK); glEnable (GL_CULL_FACE);

	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	glDisable (GL_ALPHA_TEST);
	glPolygonOffset (0.0, -3.0);

	//Setup materials.
	GLfloat	frontMaterialDiffuse [4] = {0.2f, 0.2f, 0.2f, 1.0f};
	glMaterialfv (GL_FRONT, GL_DIFFUSE, frontMaterialDiffuse);
	GLfloat	frontMaterialAmbient [4] = {0.8f, 0.8f, 0.8f, 1.0f};
	glMaterialfv (GL_FRONT, GL_AMBIENT, frontMaterialAmbient);
	GLfloat frontMaterialSpecular [4] = {0.1f, 0.1f, 0.1f, 1.0f};
	glMaterialfv (GL_FRONT, GL_SPECULAR, frontMaterialSpecular);
	GLfloat	frontMaterialShininess [] = {70.0f};
	glMaterialfv (GL_FRONT, GL_SHININESS, frontMaterialShininess);
	GLfloat	frontMaterialEmission [4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv (GL_FRONT, GL_EMISSION, frontMaterialEmission);

	GLfloat	backMaterialDiffuse [4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv (GL_BACK, GL_DIFFUSE, backMaterialDiffuse);
	GLfloat	backMaterialAmbient [4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv (GL_BACK, GL_AMBIENT, backMaterialAmbient);
	GLfloat	backMaterialSpecular [4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv (GL_BACK, GL_SPECULAR, backMaterialSpecular);
	GLfloat	backMaterialShininess [] = {0.0f};
	glMaterialfv (GL_BACK, GL_SHININESS, backMaterialShininess);
	GLfloat	backMaterialEmission [4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv (GL_BACK, GL_EMISSION, backMaterialEmission);

	glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//Use a global default texture environment mode.
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void Game::setup () {
	setupOpenGL ();
	game = new Game ();
	Camera::setup ();
	World::setup ();

//	idlingTexture = Texture::readTexture ("..\\textures\\rain#01.tga");
//	idlingTexture = Texture::readTexture ("..\\textures\\grate1.tga");
	idlingTexture = Texture::readTexture ("..\\textures\\idling.bmp");
	glClearColor (0.0, 1.0, 1.0, 1.0); //cyan; nicer than default black...
	readingTexture = Texture::readTexture ("..\\textures\\reading.bmp");

	if (idlingTexture != NULL) idlingTexture->load ();
	if (readingTexture != NULL) readingTexture->load ();
}

void Game::wrapup () {
	delete game;
	Camera::wrapup ();
	World::wrapup ();
	if (idlingTexture != NULL) idlingTexture->unload ();
	if (readingTexture != NULL) readingTexture->unload ();
	delete idlingTexture;
	delete readingTexture;
}

void Game::tick () {
	#if (MAKE_CAMERA_LOOK_AROUND == 1)
		camera->tick ();
	#endif
}

void Game::draw () {
	//Draw something to get the user's attention.

	camera->beginCamera ();

		//Setup the texture (if any).
		Texture *texture = idling ? idlingTexture : readingTexture;
		if (texture == NULL)
			glColor4d (1.0, 1.0, 0.0, 1.0); //yellow
		else {
			glColor4d (1.0, 1.0, 1.0, 1.0); //white
			texture->activate ();
		}

		//Permit blending if it's a texture with alpha...
		if (texture != NULL && texture->type == RGBAType) {
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable (GL_BLEND);
		} else {
			glDisable (GL_BLEND);
		}

		//Setup one polygon to be drawn.
		static double angle = 0.0; angle += 180.0 * DT; //180 degrees per second * DT
		glRotated (angle, 0.0, 1.0, 0.0); //rotate around the y-axis.
		//glutSolidCube (0.5); //The solid cube does NOT use the texture coordinates.
		GamePoint points [] = {//counter-clockwise order.
			{-0.5,-0.5,0.0, 0.0,0.0, 0.0,0.0,1.0}, //left bottom
			{+0.5,-0.5,0.0, 1.0,0.0, 0.0,0.0,1.0}, //right bottom
			{+0.5,+0.5,0.0, 1.0,1.0, 0.0,0.0,1.0}, //right top
			{-0.5,+0.5,0.0, 0.0,1.0, 0.0,0.0,1.0}}; //left top
		glScaled (10.0, 10.0, 10.0);

		//Draw it.
		glDisable (GL_CULL_FACE); //Just for this demo (not a good thing in general).
		glBegin (GL_POLYGON);
			for (long pointIndex = 0; pointIndex < 4; pointIndex++) {
				GamePoint &point = points [pointIndex];
				glNormal3d (point.nx, point.ny, point.nz);
				glTexCoord2d (point.tx, point.ty);
				glVertex3d (point.x, point.y, point.z); //Must be last.
			}
		glEnd ();
		glEnable (GL_CULL_FACE); //Restore it.

	camera->endCamera ();
}
