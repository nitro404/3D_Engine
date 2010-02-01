
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                        Game                                             //
//*****************************************************************************************//

extern long screenWidth, screenHeight;

Game *game = NULL;
double DT; 

HDC Game::deviceContext; GLuint Game::fontBase; 

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
	setupFont ();
	game = new Game ();
	Player::setup ();
	Camera::setup ();
	InputManager::setup ();
	World::setup ();
}

void Game::wrapup () {
	wrapupFont ();
	delete game;
	Player::wrapup ();
	Camera::wrapup ();
	InputManager::wrapup ();
	World::wrapup ();
}

void Game::tick () {
	inputManager->tick ();
	camera->tick ();
	player->tick ();
	if (world != NULL) world->tick ();
}

void Game::draw () {
	//If there is no world, draw a teapot; otherwise, draw the world...
	//Neither the input manager nor the camera draws itself...
	const bool logging = false;
	
	camera->beginCamera ();
	if(world != NULL) world->draw();
	player->draw ();
	camera->endCamera ();
//	if(displayHelp) drawFrameRate();
	drawFrameRate();
	if(displayHelp) {
		drawMessage (1, screenHeight-52, "Use WSAD to move around, QE to move up/down and Escape to exit.");
	}
}

void Game::setupFont () {
	deviceContext = GetDC (NULL);
	HFONT font; //Windows font ID...
	fontBase = glGenLists (96); //Storage for 96 characters
	font = CreateFont (	
		-24,							//Height of font
		0,								//Width of font
		0,								//Angle of escapement
		0,								//Orientation angle
		FW_BOLD,						//Font weight
		FALSE,							//Italic
		FALSE,							//Underline
		FALSE,							//Strikeout
		ANSI_CHARSET,					//Character set identifier
		OUT_TT_PRECIS,					//Output precision
		CLIP_DEFAULT_PRECIS,			//Clipping precision
		ANTIALIASED_QUALITY,			//Output quality
		FF_DONTCARE | DEFAULT_PITCH,	//Family and pitch
		"Arial");						//Font name

	SelectObject (deviceContext, font);			//Selects The Font We Want
	wglUseFontBitmaps (deviceContext, 32, 96, fontBase); //Builds 96 characters starting at character 32
}

void Game::wrapupFont () {
	ReleaseDC (NULL, deviceContext);
	glDeleteLists (fontBase, 96); //Delete all 96 characters
}

void Game::privateDrawString (const char *text) {
	glPushAttrib (GL_LIST_BIT);	//Pushes the display list bits
	glListBase (fontBase - 32); //Sets the base character to 32
	glCallLists (strlen (text), GL_UNSIGNED_BYTE, text); //Draws the display list text
	glPopAttrib (); //Pops the display list bits
}

void Game::begin2DDrawing() {
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
		glLoadIdentity ();
		glOrtho (0.0,(GLfloat) screenWidth,0.0,(GLfloat) screenHeight, -100.0f, 100.0f);
		glMatrixMode (GL_MODELVIEW);
		glPushMatrix ();
			glLoadIdentity ();
}

void Game::end2DDrawing() {
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Game::drawMessage (long x, long y, const char *message, ...) {
	char text [1000]; va_list parameters;									
	if (message == NULL) return;

	//Extract variable length parameters and copy into text as in printf.
	va_start (parameters, message);					
	    vsprintf (text, message, parameters);		
	va_end (parameters);

	//Determine the end of the string and convert each occurrence of '\n' to '\0'.
	char *end = text + strlen (text);
	for (char *next = text; *next != '\0'; next++) {
		if (*next == '\n') *next = '\0';
	}

	//Draw the multi-line message...
	begin2DDrawing ();
		glDisable (GL_TEXTURE_2D);
		glDisable (GL_LIGHTING);
		glColor4f (1.0f,1.0f,1.0f,1.0f);
		long yOffset = y;
		for (char *line = text; line < end; line += strlen (line) + 1) {
			glRasterPos2i (x, yOffset); yOffset -= 32;
			privateDrawString (line);
		}
	end2DDrawing ();
}

void Game::drawFrameRate () {
	//Draw the frame rate avoiding extreme fluctuations (since all you see is flickering).
	double frameRate = 1.0 / DT; //Frames/sec = 1/(seconds per frame).
	static double stableRate = frameRate; //This initializes only the first time...
	static double oldFrameRate = frameRate;
	//If it changed by more than 2 per cent of the stable value, use the new value; otherwise use the stable one...
	if (absolute (frameRate - stableRate) > 2.0) stableRate  = frameRate; 
	drawMessage (1, screenHeight-20, "FPS: %3.1f", stableRate);
}

void Game::drawNote (const char *message, ...) {
	char text [1000]; va_list parameters;									
	if (message == NULL) return;

	//Extract variable length parameters and copy into text as in printf.
	va_start (parameters, message);					
	    vsprintf (text, message, parameters);		
	va_end (parameters);

	drawMessage (1, screenHeight-52, "%s", text);
}