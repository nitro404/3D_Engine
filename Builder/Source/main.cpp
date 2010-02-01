
//95.4002 Wilf LaLonde: This program builds world (or level) files for use by the game engine.

//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"
#include "search.h"

//*****************************************************************************************//
//                                        Main                                             //
//*****************************************************************************************//

enum MenuItem {DifficultyEasy, DifficultyChallenging, DifficultyImpossible,
	DisplayModeWireframe, DisplayModeTextured, BuildWorld, Quit};

MenuItem difficulty; MenuItem displayMode;
bool wireframe = false, fullscreen = false;
char filename [256];

void resizeWindow (int width, int height) {
	//Setup a new viewport.
	glViewport (0, 0, width, height);

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
	if (DT < 0.001) DT = 0.001; //Less than 1 millisecond (the resolution of "timeNow"); this shows up as 1000 frames per second.
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
			::log ("\nPressed special key consisting of character '%c' decimal %d hex %x.", character, character, character);
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
			::log ("\nReleased special key consisting of character '%c' decimal %d hex %x.", character, character, character);
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
		case 't':
		case 'T':
			wireframe = !wireframe;
			glPolygonMode (GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
			break;
		default:
			//The log allows you to figure out what key something actually is!!!
			::log ("\nPressed normal key consisting of character '%c' decimal %d hex %x.", character, character, character);
			break;
	}

	glutPostRedisplay ();
}

void normalKeyReleased (unsigned char character, int x, int y) {
	//Handle the key and then force a redisplay.
	switch (character) {
		case escapeCharacter:
			exit (0);
		default: 
			//The log allows you to figure out what keep something actually is!!!
			::log ("\nReleased normal key consisting of character '%c' decimal %d hex %x.", character, character, character);
			break;
	}

	glutPostRedisplay ();
}

void buildWorld () {
	//Used by when mouse clicking and via the popup menu...

	//Show that we are about to be busy for a while.
	game->idling = false;
	displayWindow (); //Force the reading texture to be displayed.

	//Read a world...
	World *world = World::read (); //Prompt for a ".uni" file and returns a converted world.
	if (world == NULL) {game->idling = true; return;} //Not successful.
	
	//For debugging, see what it looks like...
	world->log ();

	//Write the world in ".wrl" format (YOUR format).
	world->write ();
	delete world; world = NULL; //Now were done... whether it worked or not so discard everything...

	//Show that we are no longer busy (the idling texture will ultimately be displayed).
	game->idling = true;
}

void mousePressed (int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			//Start something at mouse coordinates x and y.
			buildWorld (); //Do this on left mouse button down only...
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

	::log ("\nPressed %s mouse button at %d@%d.", button == GLUT_LEFT_BUTTON ? "LEFT" : 
		(button == GLUT_MIDDLE_BUTTON ? "MIDDLE" : "RIGHT"), x, y);
}

void mouseMoved (int x, int y) {
	//::log ("\nMoved mouse to %d@%d.", x, y);
}

#define matches(a,b) strlen (b) >= strlen (a) && memcmp (a,b,strlen (a)) == 0

/*
void parseParameters (int parametersSize, char **parameters) {
	bool optionFound = false;
	for (long i = 1; i < parametersSize; i++) {//Skip program name.
		::log ("\n%d: Consider \"%s\".", i, parameters [i]);
		if (matches ("-w", parameters [i])) {
			::log ("\nMatched -w");
			wireframe = true; optionFound = true;
		} else if (matches ("-f", parameters [i])) {
			::log ("\nMatched \"-f\", extracting from \"%s\".", parameters [i]);
			::log ("\nAssigning into filename [0]");
			filename [0] = 20;
			::log (" WORKED...");
			strcpy (filename, parameters [i]+2);
			::log (" COPY WORKED TOO...");
			::log ("\nRead filename \"%s\".", filename);
			optionFound = true;
		} else 
			::log ("\nFailed to match.");
	}
	if (!optionFound) {
		printf ("\nusage: builder [-options] -fFilename");
		printf ("\n  -w         :: wireframe");
		printf ("\n  -f         :: file name; e.g., -fc:\\test\\data");
		exit (1);
	}
}
*/

void genericMenuHandler (int item) {
	switch (item) {
		case BuildWorld: {
			buildWorld ();
			break;}
		case Quit:
			Game::wrapup (); exit (0);
			break;
		default:
			::log ("\nUnknown generic menu selection %d (%x).", item, item);
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
			::log ("\nUnknown difficulty %d (%x).", item, item);
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
			::log ("\nUnknown display mode %d (%x).", item, item);
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
	//glutAddSubMenu ("Difficulty...", difficultyMenu);
	//glutAddSubMenu ("Mode...", displayModeMenu);
	glutAddMenuEntry ("Build World", BuildWorld);
	glutAddMenuEntry ("Quit", Quit);

	glutAttachMenu (GLUT_RIGHT_BUTTON);
}

void createWorld(char * fileName) {
	if(fileName == NULL) {
		exit(1);
	}
	
	//Read a world...
	World *world = World::import(fileName);
	if (world == NULL) {
		exit(1);
	}
	
	//For debugging, see what it looks like...
	world->log ();
	
	//Write the world in ".wrl" format (YOUR format).
	if(strstr(fileName, ".uni") != NULL) {
		char newFileName [MAX_PATH];
		strcpy(newFileName, fileName);
		newFileName[strlen(newFileName) - 4] = '\0';
		strcat(newFileName, ".wrl");
		fileName = newFileName;
	}
	
	world->write(fileName);
	delete world; world = NULL; //Now were done... whether it worked or not so discard everything...
}

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
			createWorld(filename);
			exit(1);
		}
		else if(matches ("-d", parameters [i])) {
			optionFound = true;
			filename [0] = 20;
			strcpy (filename, parameters [i]+2);
			vector<string> mapNames;
			int result = searchForMaps(mapNames, filename);
			if(result) { exit(1); }
			for(vector<string>::iterator iter = mapNames.begin(); iter != mapNames.end(); ++iter) {
				char * str = (char*) iter->c_str();
				createWorld(str);
			}
			exit(1);
		}
	}

	if (!optionFound) {
		printf ("\nusage: builder [-options] [-fmap_name.uni] [-dpath_to_maps]");
		printf ("\n  -w         :: wireframe");
		printf ("\n  -f         :: file name; e.g., -fC:\\Test\\Maps\\room.uni");
		printf ("\n  -d         :: directory (containing .uni files); e.g., -d..\\Project\\Maps");
		printf ("\n");
//		exit (1);
	}
}

int main (int parametersSize, char **parameters) {
	//Welcome...
	clearLog (); ::log ("\n\nStarting builder...");
	#if (_MSC_VER >= 1300)
		::log ("\nThis is a newer version of visual studio; i.e., version %d.", _MSC_VER);
	#else
		::log ("\nThis is an older version of visual studio; i.e., version %d.", _MSC_VER);
	#endif

	//Process the command line (if any)...
	parseParameters (parametersSize, parameters); //Better to prompt from a menu...

	//Setup general facilities.
	glutInitDisplayMode (GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL | GLUT_MULTISAMPLE);
	glutInitWindowSize (800, 600);
	glutInit (&parametersSize, parameters);
	if (fullscreen) /*glutEnterGameMode ()*/; else {glutCreateWindow ("world builder"); createMenus ();}
    glutIgnoreKeyRepeat (GLUT_KEY_REPEAT_ON);
    glutSetCursor (GLUT_CURSOR_NONE);

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
