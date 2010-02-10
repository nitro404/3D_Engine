
//95.4002 Wilf LaLonde: Student game engine.

//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                        Main                                             //
//*****************************************************************************************//

char * gameName = "3D Engine";
int mouseSensitivity = 5;

enum MenuItem {DifficultyEasy, DifficultyChallenging, DifficultyImpossible,
	DisplayModeWireframe, DisplayModeTextured, RunWorld, Quit};

MenuItem difficulty; MenuItem displayMode;
bool wireframe = false;
bool fullscreen = false;
long screenWidth;
long screenHeight;
char filename [256];

#define escapeCharacter 27
#define enterCharacter 13
#define VK_ALT VK_MENU

#define matches(a,b) strlen(b) >= strlen(a) && memcmp(a,b,strlen(a)) == 0

inline bool keyIsDown (long key) {return (GetAsyncKeyState (key) & 0x8000) != 0;}

void resizeWindow(int width, int height) {
	//Setup a new viewport.
	glViewport(0, 0, width, height);
	screenWidth = width; screenHeight = height;

	//Setup a new perspective matrix.
	GLdouble verticalFieldOfViewInDegrees = 40;
	GLdouble aspectRatio = height == 0 ? 1.0 : (GLdouble) width / (GLdouble) height;
	GLdouble nearDistance = 1.0;
	GLdouble farDistance = 2000.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(verticalFieldOfViewInDegrees, aspectRatio, nearDistance, farDistance);

	//Get back to default mode.
	glMatrixMode(GL_MODELVIEW);
}

inline void computeDT() {
	//Compute elapsed time needed for controlling frame rate independent effects.
	//If running slower than 5 frames per second, pretend it's 5 frames/sec.
	//Note: 30 frames per second means 1/30 seconds per frame = 0.03333... seconds per frame.
	static double lastTimeInSeconds = timeNow() - 0.033; //Pretend we are running 30 frames per second on the first tick.
	double timeInSeconds = timeNow();
	DT = timeInSeconds - lastTimeInSeconds;
	if(DT > 0.2) DT = 0.2; //5 frames/sec means 1 frame in 1/5 (= 0.2) seconds.
	lastTimeInSeconds = timeInSeconds;
}

void displayWindow() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	game->draw();
	glutSwapBuffers();
}

void idle() {
	computeDT();
	game->tick();
	if(GetFocus()) {
		SetCursorPos (screenWidth >> 1, screenHeight >> 1); //Re-center the mouse...
	}
	glutPostRedisplay();
}

void visibilityChanged (int visible) {
	glutIdleFunc(visible == GLUT_VISIBLE ? idle : NULL);
}

void specialKeyPressed (int character, int x, int y) {
	switch (character) {
		case GLUT_KEY_F1:
			break;
		case GLUT_KEY_F2:
			break;
		case GLUT_KEY_UP:
			break;
		case GLUT_KEY_DOWN:
			break;
		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;
	}
	glutPostRedisplay ();
}

void specialKeyReleased (int character, int x, int y) {
	switch (character) {
		case GLUT_KEY_F1:
			break;
	}
	glutPostRedisplay ();
}

void normalKeyPressed (unsigned char character, int x, int y) {
	switch(character) {
		case escapeCharacter:
			Game::wrapup ();
			exit (0);
		
		case 'w':
		case 'W':
			inputManager->translateAhead = true;
			break;
			
		case 's':
		case 'S':
			inputManager->translateBack = true;
			break;
			
		case 'a':
		case 'A':
			inputManager->translateLeft = true;
			break;
			
		case 'd':
		case 'D':
			inputManager->translateRight = true;
			break;
			
		case ' ':
			inputManager->translateUp = true;
			break;
			
		case 'z':
		case 'Z':
			inputManager->translateDown = true;
			break;

		case 't':
		case 'T':
			wireframe = !wireframe;
			glPolygonMode (GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
			break;
			
		case '?':
			game->displayHelp = !game->displayHelp;
			break;

//		case enterCharacter:
//			if(keyIsDown (VK_ALT)) {
//				fullscreen = !fullscreen;
//				if(fullscreen) {
//					glutEnterGameMode();
//				}
//				else {
//					glutLeaveGameMode();
//				}
//			}
//			break;
	}

	glutPostRedisplay ();
}

void normalKeyReleased (unsigned char character, int x, int y) {
	switch (character) {
		case 'w':
		case 'W':
			inputManager->translateAhead = false;
			break;
			
		case 's':
		case 'S':
			inputManager->translateBack = false;
			break;
			
		case 'a':
		case 'A':
			inputManager->translateLeft = false;
			break;
			
		case 'd':
		case 'D':
			inputManager->translateRight = false;
			break;

		case ' ':
			inputManager->translateUp = false;
			break;

		case 'z':
		case 'Z':
			inputManager->translateDown = false;
			break;
	}

	glutPostRedisplay ();
}

void mousePressed(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN) {
			
		}
		else {
			
		}
	}
	else if(button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			
		}
		else {
			
		}
	}
	else if(button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN) {
			
		}
		else {
			
		}
	}
}

void mouseMoved (int x, int y) {
	//Note: Because we are re-centering the mouse on each tick (see function "idle ()"), here
	//we are determining how far the mouse was moved from the center point and we use this
	//to determine both (1) x-rotation amounts (using the vertical displacement) which we store
	//in rotation.x and (2) y-rotation amounts (using the horizontal displacement) which we store
	//in rotation.y. 
	
	//Keep in mind that in windows, x increases going right and y increases going down... 
	
	//So to rotate so as to look up by moving the mouse up, we get a negative change in y but a 
	//rotation to look up requires that the y-axis rotate toward the z-axis (a positive rotation). 
	//So we store the negative of this amount in rotation.x.

	//Similarly, to rotate so as to look right by moving the mouse right, we get a positive change
	//in x. But rotating the z-axis toward the x-axis is a positive rotation and this causes
	//more of the left to be seen, so we actually want to rotate by the negative of this amount
	//which we store in rotation.y.
	
	//::log ("\nMoved mouse to %d@%d.", x, y);
	POINT screenPOINT; GetCursorPos (&screenPOINT); 
	Point point (screenPOINT.x, screenPOINT.y, 0.0);
	Point center (screenWidth >> 1, screenHeight >> 1, 0.0);

	Point difference = (point - center) * (mouseSensitivity / 10.0);
	Point rotation (-difference.y, -difference.x, 0.0);

	inputManager->rotateBy (rotation * (InputManager::rotationSpeed * DT)); //degrees = degrees per second * second
}

void genericMenuHandler (int item) {
	switch (item) {
		case RunWorld: 
			if (game->world != NULL) {
				game->world->unload();
				delete game->world;
			}
			game->world = new World();
			game->world->read();
			if (game->world != NULL) {
				player->reset(game->world->startPosition);
			}
			break;
	}
}

void createMenus () {
  int genericMenu = glutCreateMenu (genericMenuHandler);
  glutAddMenuEntry ("Run World", RunWorld);
  glutAttachMenu (GLUT_RIGHT_BUTTON);
}

int main (int parametersSize, char **parameters) {

	//Setup general facilities.
	glutInitDisplayMode (GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL | GLUT_MULTISAMPLE);
	glutInitWindowSize (1200, 720);
	glutInit (&parametersSize, parameters);
	
	if (fullscreen) { 
		glutEnterGameMode();
	}
	else {
		glutCreateWindow(gameName);
		createMenus();
	}

    glutIgnoreKeyRepeat (GLUT_KEY_REPEAT_ON);
	glutSetCursor (GLUT_CURSOR_NONE);

	//Specify function handlers.
	glutDisplayFunc (displayWindow); glutReshapeFunc (resizeWindow); glutKeyboardFunc (normalKeyPressed);
	glutSpecialFunc (specialKeyPressed); glutMouseFunc (mousePressed); glutMotionFunc (mouseMoved);
	glutVisibilityFunc (visibilityChanged);
    glutPassiveMotionFunc (mouseMoved); 
    glutSpecialUpFunc (specialKeyReleased);
    glutKeyboardUpFunc (normalKeyReleased);
    
	Game::setup ();
	glutMainLoop ();
	Game::wrapup ();

	return 0;
}
