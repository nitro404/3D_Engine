
//95.402 Wilf LaLonde: Student development Worldcraft to Student world converter.
//This program creates world (or level) files for use by the game engine.

//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"
#include "search.h"

//*****************************************************************************************//
//                                        Main                                             //
//*****************************************************************************************//

char mapDirectory [_MAX_DIR] = {'.','.','\\','.','.','\\','.','.','\\','M','A','P','S','\0'};

void convertFiles () {
	//Prompt for one or more files to convert...
	char *filenames = promptForMapRead ();
	void displayWindow (); displayWindow (); //Force the reading texture to be displayed.
	if (filenames == NULL) return; //User must have cancelled.

	//Determine how many files were selected (count cannot be 0; otherwise NULL would have been returned)...
	char *name = &filenames [strlen (filenames) + 1];
	long count = 0;
	while (*name != '\0') {
		count++; name = &name [strlen (name) + 1];
	}
	if (count == 0) {
		//Only one file was selected and it's returned as part of the directory name...
		char *lastSlash = strrchr (filenames, '\\');
		//Make it look like separate direction / name strings...
		*lastSlash = '\0'; count = 1;
	}
	//::log ("\nThere were %d files returned...", count);

	//Copy the string with directory name1 name2 ... information into a new place to
	//ensure subsequent prompt don't overwrite it...
	long stringsLength = name - filenames + 1;
	char *strings = new char [stringsLength];
	CopyMemory (strings, filenames, stringsLength);
	char *inputDirectory = strings; char *outputDirectory;

	name = &strings [strlen (strings) + 1];
	bool firstUniversalObjectHasBeenOutput = false;
	for (long index = 0; index < count; index++) {
		char inputFileName [500]; 
		strcpy (inputFileName, inputDirectory);
		strcat (inputFileName, "\\");
		strcat (inputFileName, name);

		::log ("\nLoad file \"%s\"...", inputFileName);
		UniversalObject *universalObject = UniversalObject::read (inputFileName);
		if (universalObject != NULL) {
			//For debugging, see what it looks like...
			//universalObject->log ();

			//Change the actual suffix to a ".uni" format (it's already been read)...
			char *shortOutputName = strrchr (inputFileName, '\\') + 1; //Search right to left...
			char *suffix = strstr (shortOutputName, ".");
			*suffix = '\0'; strcat (suffix, ".uni");
			
			if (!firstUniversalObjectHasBeenOutput) {
				//Query the user for the output directory...
				//outputDirectory = promptForWriteFile (WorldChoice, "Universal File (*.uni)|*.uni||", shortOutputName);
				outputDirectory = promptForWriteFile (WorldChoice, "Universal File (*.uni)|*.uni||", inputFileName);
				char *lastSlash = strrchr (outputDirectory, '\\'); *lastSlash = '\0';
				firstUniversalObjectHasBeenOutput = true;
			}
		
			//Warning: The output file name will be the same as the input except for the suffix...
			//This is needed to deal with the fact that we can multi-select a whole folder of files...
			char outputFileName [500]; 
			strcpy (outputFileName, outputDirectory);
			strcat (outputFileName, "\\");
			strcat (outputFileName, shortOutputName);

			::log ("\nDump file \"%s\"...", outputFileName);
			universalObject->dump (outputFileName);
			delete universalObject;
		} else {
			delete [] strings;
			log ("\nCould not read file \"%s\".", inputFileName);
			halt ("\nCould not read file \"%s\".", inputFileName);
			return;
		} //Not successful.
		name = &name [strlen (name) + 1];
	}

	delete [] strings;
}

/*
void convertFile(char * filenames) {
	//Prompt for one or more files to convert...
	char *filenames = promptForMapRead ();
	void displayWindow (); displayWindow (); //Force the reading texture to be displayed.
	if (filenames == NULL) return; //User must have cancelled.

	//Determine how many files were selected (count cannot be 0; otherwise NULL would have been returned)...
	char *name = &filenames [strlen (filenames) + 1];
	long count = 0;
	while (*name != '\0') {
		count++; name = &name [strlen (name) + 1];
	}
	if (count == 0) {
		//Only one file was selected and it's returned as part of the directory name...
		char *lastSlash = strrchr (filenames, '\\');
		//Make it look like separate direction / name strings...
		*lastSlash = '\0'; count = 1;
	}
	//::log ("\nThere were %d files returned...", count);

	//Copy the string with directory name1 name2 ... information into a new place to
	//ensure subsequent prompt don't overwrite it...
	long stringsLength = name - filenames + 1;
	char *strings = new char [stringsLength];
	CopyMemory (strings, filenames, stringsLength);
	char *inputDirectory = strings; char *outputDirectory;

	name = &strings [strlen (strings) + 1];
	bool firstUniversalObjectHasBeenOutput = false;
	for (long index = 0; index < count; index++) {
		char inputFileName [500]; 
		strcpy (inputFileName, inputDirectory);
		strcat (inputFileName, "\\");
		strcat (inputFileName, name);

		::log ("\nLoad file \"%s\"...", inputFileName);
		UniversalObject *universalObject = UniversalObject::read (inputFileName);
		if (universalObject != NULL) {
			//For debugging, see what it looks like...
			//universalObject->log ();

			//Change the actual suffix to a ".uni" format (it's already been read)...
			char *shortOutputName = strrchr (inputFileName, '\\') + 1; //Search right to left...
			char *suffix = strstr (shortOutputName, ".");
			*suffix = '\0'; strcat (suffix, ".uni");
			
			if (!firstUniversalObjectHasBeenOutput) {
				//Query the user for the output directory...
				//outputDirectory = promptForWriteFile (WorldChoice, "Universal File (*.uni)|*.uni||", shortOutputName);
				outputDirectory = promptForWriteFile (WorldChoice, "Universal File (*.uni)|*.uni||", inputFileName);
				char *lastSlash = strrchr (outputDirectory, '\\'); *lastSlash = '\0';
				firstUniversalObjectHasBeenOutput = true;
			}
		
			//Warning: The output file name will be the same as the input except for the suffix...
			//This is needed to deal with the fact that we can multi-select a whole folder of files...
			char outputFileName [500]; 
			strcpy (outputFileName, outputDirectory);
			strcat (outputFileName, "\\");
			strcat (outputFileName, shortOutputName);

			::log ("\nDump file \"%s\"...", outputFileName);
			universalObject->dump (outputFileName);
			delete universalObject;
		} else {
			delete [] strings;
			log ("\nCould not read file \"%s\".", inputFileName);
			halt ("\nCould not read file \"%s\".", inputFileName);
			return;
		} //Not successful.
		name = &name [strlen (name) + 1];
	}

	delete [] strings;
}
*/

void convertFile(char * fileName) {
	if (fileName == NULL) {
		return;
	}

	UniversalObject *universalObject = UniversalObject::read (fileName);
	if (universalObject != NULL) {

		//Change the actual suffix to a ".uni" format (it's already been read)...
		char * shortOutputName = strrchr (fileName, '\\') + 1; //Search right to left...
		char * suffix = strrchr (shortOutputName, '.');
		*suffix = '\0';
		strcat(suffix, ".uni");
		
		char outputFileName [500]; 
		strcpy (outputFileName, mapDirectory);
		strcat (outputFileName, "\\");
		strcat (outputFileName, shortOutputName);

		universalObject->dump(outputFileName);
		delete universalObject;
	}
}


enum MenuItem {DifficultyEasy, DifficultyChallenging, DifficultyImpossible,
	DisplayModeWireframe, DisplayModeTextured, ConvertFiles, ShowConvertedFile, Quit};

MenuItem difficulty; MenuItem displayMode;
bool wireframe = false, fullscreen = false; long screenWidth, screenHeight;
char filename [256];

void resizeWindow (int width, int height) {
	//Setup a new viewport.
	glViewport (0, 0, width, height);
	screenWidth = width; screenHeight = height;

	//Setup a new perspective matrix.
	GLdouble verticalFieldOfViewInDegrees = 40;
	GLdouble aspectRatio = height == 0 ? 1.0 : (GLdouble) width / (GLdouble) height;
	GLdouble nearDistance = 1.0;
	GLdouble farDistance = 2000.0;

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (verticalFieldOfViewInDegrees, aspectRatio, nearDistance, farDistance);

	//Get back to default mode.
	glMatrixMode (GL_MODELVIEW);
}

inline void computeDT () {
	//Compute elapsed time needed for controlling frame rate independent effects.
	//If running slower than 5 frames per second, pretend it's 5 frames/sec.
	static double lastTimeInSeconds = timeNow () * 0.001; //Convert from millisecs to secs.
	double timeInSeconds = timeNow () * 0.001;
	DT = timeInSeconds - lastTimeInSeconds;
	if (DT < 0.001) DT = 0.001; //Less the 1 millisecond (the resolution of "timeNow").
	if (DT > 0.2) DT = 0.2; //5 frames/sec means 1 frame in 1/5 (= 0.2) seconds.
	lastTimeInSeconds = timeInSeconds;
}

void displayWindow () {
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	game->draw ();
	glutSwapBuffers ();
}

void idle () {
	computeDT ();
	game->tick ();
	//::log ("\nSet mouse to %d@%d", screenWidth >> 1, screenHeight >> 1); 
	//SetCursorPos (screenWidth >> 1, screenHeight >> 1); 
	glutPostRedisplay ();
}

void visibilityChanged (int visible) {
	glutIdleFunc (visible == GLUT_VISIBLE ? idle : NULL);
}

inline bool keyIsDown (long key) {return (GetAsyncKeyState (key) & 0x8000) != 0;}
#define VK_ALT VK_MENU

void specialKeyPressed (int character, int x, int y) {
	switch (character) {
		case GLUT_KEY_F1:
			break;
		case GLUT_KEY_F2:
			break;
		case GLUT_KEY_UP: {
			//A partial example...
			if (keyIsDown (VK_CONTROL)) {
			} else if (keyIsDown (VK_ALT)) {
			} else {
			}
			break;}
		case GLUT_KEY_DOWN: {
			break;}
		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;
		default: 
			//The log allows you to figure out what key something actually is!!!
			log ("\nPressed special key consisting of character '%c' decimal %d hex %x.", character, character, character);
			break;
	}
	glutPostRedisplay ();
}

void specialKeyReleased (int character, int x, int y) {
	switch (character) {
		case GLUT_KEY_F1:
			break;
		default: 
			//The log allows you to figure out what key something actually is!!!
			log ("\nReleased special key consisting of character '%c' decimal %d hex %x.", character, character, character);
			break;
	}
	glutPostRedisplay ();
}

#define escapeCharacter 27

void normalKeyPressed (unsigned char character, int x, int y) {
	//Handle the key and then force a redisplay.
	switch (character) {
		case escapeCharacter:
			Game::wrapup (); exit (0);
		case ' ':
			//Open door perhaps?
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': {
			//Run some tests perhaps?
			long code = character - '0';
			break;}

		//Use w/s for forward/back, a/d for turn left/right, q/z for turn up/down, e/c for up/down.
		case 'w':
		case 'W':
			camera->translateAhead = true;
			break;
		case 's':
		case 'S':
			camera->translateBack = true;
			break;

		case 'a':
		case 'A':
			camera->translateLeft = true;
			break;
		case 'd':
		case 'D':
			camera->translateRight = true;
			break;

		case 'q':
		case 'Q':
			camera->rotateUp = true;
			break;
		case 'z':
		case 'Z':
			camera->rotateDown = true;
			break;

		case 'e':
		case 'E':
			camera->translateUp = true;
			break;
		case 'c':
		case 'C':
			camera->translateDown = true;
			break;

		case 't':
		case 'T':
			wireframe = !wireframe;
			glPolygonMode (GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
			break;
		default:
			//The log allows you to figure out what key something actually is!!!
			log ("\nPressed normal key consisting of character '%c' decimal %d hex %x.", character, character, character);
			break;
	}

	glutPostRedisplay ();
}

void normalKeyReleased (unsigned char character, int x, int y) {
	//Handle the key and then force a redisplay.
	switch (character) {
		case escapeCharacter:
			Game::wrapup (); exit (0);

		//Use w/s for forward/back, a/d for turn left/right, q/z for turn up/down, e/c for up/down.
		case 'w':
		case 'W':
			camera->translateAhead = false;
			break;
		case 's':
		case 'S': 
			camera->translateBack = false;
			break;

		case 'a':
		case 'A':
			camera->translateLeft = false;
			break;
		case 'd':
		case 'D':
			camera->translateRight = false;
			break;

		case 'q':
		case 'Q':
			camera->rotateUp = false;
			break;
		case 'z':
		case 'Z':
			camera->rotateDown = false;
			break;

		case 'e':
		case 'E':
			camera->translateUp = false;
			break;
		case 'c':
		case 'C':
			camera->translateDown = false;
			break;

		default: 
			//The log allows you to figure out what key something actually is!!!
			log ("\nReleased normal key consisting of character '%c' decimal %d hex %x.", character, character, character);
			break;
	}

	glutPostRedisplay ();
}

void mousePressed (int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			//Start something at mouse coordinates x and y.
			game->prompt (); convertFiles (); game->idle ();
		} else {//state == GLUT_UP
			//End something at mouse coordinates x and y.
		}
	} else if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN) {
			//Start something at mouse coordinates x and y.
		} else {//state == GLUT_UP
			//End something at mouse coordinates x and y.
		}
	} else {//button == GLUT_RIGHT_BUTTON
		if (state == GLUT_DOWN) {
			//Start something at mouse coordinates x and y.
		} else {//state == GLUT_UP
			//End something at mouse coordinates x and y.
		}
	}
	//log ("\nPressed %s mouse button at %d@%d.", button == GLUT_LEFT_BUTTON ? "LEFT" : 
	//	(button == GLUT_MIDDLE_BUTTON ? "MIDDLE" : "RIGHT"), x, y);
}

void mouseMoved (int x, int y) {
	//Currently, we are interpreting movement by "d" pixels to mean "d" degrees. Doesn't take screen size into account.
	//log ("\nMoved mouse to %d@%d.", x, y);
	if (!game->showing) return;
	POINT screenPOINT; GetCursorPos (&screenPOINT); 
	Point point (screenPOINT.x, screenPOINT.y, 0.0);
	Point center (screenWidth >> 1, screenHeight >> 1, 0.0);
	double sensitivity = 0.1;
	Point difference = (point - center) * sensitivity;
	Point rotation (-difference.y, -difference.x, 0.0);
	camera->rotateBy (rotation); 
}

#define matches(a,b) strlen (b) >= strlen (a) && memcmp (a,b,strlen (a)) == 0

/*
void parseParameters (int parametersSize, char **parameters) {
	bool optionFound = false;
	for (long i = 1; i < parametersSize; i++) {//Skip program name.
		log ("\n%d: Consider \"%s\".", i, parameters [i]);
		if (matches ("-w", parameters [i])) {
			log ("\nMatched -w");
			wireframe = true; optionFound = true;
		} else if (matches ("-f", parameters [i])) {
			log ("\nMatched \"-f\", extracting from \"%s\".", parameters [i]);
			log ("\nAssigning into filename [0]");
			filename [0] = 20;
			log (" WORKED...");
			strcpy (filename, parameters [i]+2);
			log (" COPY WORKED TOO...");
			log ("\nRead filename \"%s\".", filename);
			optionFound = true;
		} else 
			log ("\nFailed to match.");
	}
	if (!optionFound) {
		printf ("\nusage: convert [-options] -fFilename");
		printf ("\n  -w         :: wireframe");
		printf ("\n  -f         :: file name; e.g., -fc:\\test\\data");
		exit (1);
	}
}
*/

void genericMenuHandler (int item) {
	switch (item) {
		case ConvertFiles: {
			game->prompt (); convertFiles (); game->idle ();
			break;}
		case ShowConvertedFile: {
			game->prompt ();
			char *filename = promptForWorldRead ();
			if (filename == NULL) {game->idle (); break;}
			game->universalObject = UniversalObject::load (filename);
			if (game->universalObject == NULL) {game->idle (); break;}
			game->universalObject->loadAllTextures ();
			game->show ();
			break;}
		case Quit:
			Game::wrapup (); exit (0);
			break;
		default:
			log ("\nUnknown generic menu selection %d (%x).", item, item);
	}
}

void difficultyMenuHandler (int item) {
	switch (item) {
		case DifficultyEasy:
		case DifficultyChallenging:
		case DifficultyImpossible:
			difficulty = (MenuItem) item; 
			break;
		default:
			log ("\nUnknown difficulty %d (%x).", item, item);
	}
}

void displayModeMenuHandler (int item) {
	switch (item) {
		case DisplayModeWireframe:
			wireframe = true;
			glPolygonMode (GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
			break;
		case DisplayModeTextured:
			wireframe = false;
			glPolygonMode (GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
			break;
		default:
			log ("\nUnknown display mode %d (%x).", item, item);
	}
    glutPostRedisplay ();
}

void createMenus () {
	/* Commented out since not needed here...
	int difficultyMenu = glutCreateMenu (difficultyMenuHandler);
	glutAddMenuEntry ("Easy", DifficultyEasy);
	glutAddMenuEntry ("Hard", DifficultyChallenging);
	glutAddMenuEntry ("Impossible", DifficultyImpossible);

	int displayModeMenu = glutCreateMenu (displayModeMenuHandler);
	glutAddMenuEntry ("Wireframe", DisplayModeWireframe);
	glutAddMenuEntry ("Textured", DisplayModeTextured);
	*/

	int genericMenu = glutCreateMenu (genericMenuHandler);
	glutAddMenuEntry ("Convert files", ConvertFiles);
	glutAddMenuEntry ("Show converted file", ShowConvertedFile);
	glutAddMenuEntry ("Quit", Quit);

	glutAttachMenu (GLUT_RIGHT_BUTTON);
}

const bool OwnsElements = true;

void parseParameters (int parametersSize, char **parameters) {
	bool optionFound = false;

	for(long i=1;i<parametersSize;i++) {
		if (matches ("-w", parameters [i])) {
			wireframe = true;
			optionFound = true;
		}
		else if(matches ("-f", parameters [i])) {
			optionFound = true;
			filename [0] = 20;
			strcpy (filename, parameters [i]+2);
			convertFile(filename);
			exit(1);
		}
		else if(matches ("-d", parameters [i])) {
			optionFound = true;
			filename [0] = 20;
			strcpy (filename, parameters [i]+2);
			vector<string> mapNames;
			int result = searchForMaps(mapNames, filename);
			if(result) { exit(1); }
			for(int i=0;i<mapNames.size();i++) {
				convertFile((char *) mapNames.at(i).c_str());
			}
			exit(1);
		}
	}

	if (!optionFound) {
		printf ("\nusage: builder [-options] [-fmap_name.map] [-dpath_to_maps]");
		printf ("\n  -w         :: wireframe");
		printf ("\n  -f         :: file name; e.g., -fC:\\Test\\Maps\\room.map");
		printf ("\n  -d         :: directory (containing .map files); e.g., -d..\\Project\\Maps");
		printf ("\n");
//		exit (1);
	}
}

int main (int parametersSize, char **parameters) {
	//Welcome...
	clearLog (); log ("\n\nStarting...");

	setupStartupDirectory ();
	setupTextureSearchPaths ();
	setupFGDFilePath ();

	//Process the command line (if any)...
//	parseParameters (parametersSize, parameters); //Better to prompt from a menu...

	//Setup general facilities.
	glutInitDisplayMode (GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL | GLUT_MULTISAMPLE);
	glutInitWindowSize (800, 600);
	glutInit (&parametersSize, parameters);
	if (fullscreen) /*glutEnterGameMode ()*/; else {glutCreateWindow ("Universal 3D file converter"); createMenus ();}
    glutIgnoreKeyRepeat (GLUT_KEY_REPEAT_ON);
    //glutSetCursor (GLUT_CURSOR_NONE);

	//Specify function handlers.
	glutDisplayFunc (displayWindow); glutReshapeFunc (resizeWindow); glutKeyboardFunc (normalKeyPressed);
	glutSpecialFunc (specialKeyPressed); glutMouseFunc (mousePressed); glutMotionFunc (mouseMoved);
	glutVisibilityFunc (visibilityChanged);
    glutPassiveMotionFunc (mouseMoved); 
    glutSpecialUpFunc (specialKeyReleased);
    glutKeyboardUpFunc (normalKeyReleased);

	//Setup our own facilities.
	Game::setup ();

	//Rumble
	glutMainLoop ();

	//Cleanup
	Game::wrapup ();
	return 0;
}
