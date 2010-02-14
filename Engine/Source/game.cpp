#include "includes.all"

extern int screenWidth, screenHeight;

Game * game = NULL;

double DT; 

HDC Game::deviceContext;
GLuint Game::fontBase; 

void Game::tick() {
	inputManager->tick();
	camera->tick();
	player->tick();
	if(world != NULL) {
		world->tick ();
	}
}

void Game::draw () {
	//If there is no world, draw a teapot; otherwise, draw the world...
	//Neither the input manager nor the camera draws itself...
	const bool logging = false;
	
	camera->beginCamera ();
	if(world != NULL) {
		world->draw();
	}
	else {
		drawMessage(125, screenHeight-20, "Right-click to load a map.");
	}
	player->draw ();
	camera->endCamera ();
	if(displayHelp || world == NULL) {
		drawFrameRate();
		drawMessage(1, screenHeight-52, "Use WSAD to move around, QE to move up/down and Escape to exit.");
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
	if(absolute(frameRate - stableRate) > 2.0) stableRate  = frameRate; 
	drawMessage(1, screenHeight-20, "FPS: %3.1f", stableRate);
}

void Game::drawNote (const char *message, ...) {
	char text[1000];
	va_list parameters;									
	if (message == NULL) return;
	
	//Extract variable length parameters and copy into text as in printf.
	va_start(parameters, message);					
	    vsprintf(text, message, parameters);		
	va_end(parameters);

	drawMessage(1, screenHeight-52, "%s", text);
}

void Game::import() {
	if(world != NULL) {
		delete world;
		world = NULL;
	}
	
	char * fileName = NULL;
	char * filenameBuffer = new char[MAX_PATH];
	filenameBuffer[0] = '\0';
	
	OPENFILENAME query;
	query.lStructSize = sizeof(OPENFILENAME);
	query.hwndOwner = NULL;
	query.hInstance = GetModuleHandle(NULL);
	query.lpstrFilter = worldFileFilter;
	query.lpstrCustomFilter = NULL;
	query.nMaxCustFilter = 0;
	query.nFilterIndex = 0;
	query.lpstrFile = filenameBuffer;
	query.nMaxFile = MAX_PATH;
	query.lpstrFileTitle = NULL;
	query.nMaxFileTitle = MAX_PATH;
	query.lpstrInitialDir = mapDirectory;
	query.lpstrTitle = NULL;
	query.Flags = OFN_NOCHANGEDIR;
	query.nFileOffset = 0;
	query.nFileExtension = 0;
	query.lpstrDefExt = NULL;
	query.lCustData = 0;
	query.lpfnHook = NULL;
	query.lpTemplateName = NULL;
	
	if(GetOpenFileName(&query)) {
		fileName = new char[strlen(query.lpstrFile) + 1];
		strcpy(fileName, query.lpstrFile);
		
		if(strlen(fileName) > 0) {
			world = new World;
			world->import(fileName);
			player->reset(world->startPosition);
		}
		
		delete [] fileName;
	}
	
	delete[] filenameBuffer;
}
