
//95.4002 Wilf LaLonde: Student game engine.

//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                        Main                                             //
//*****************************************************************************************//

char * gameName = "3D Game Engine";
int mouseSensitivity = 5;
bool fullscreen = false;
long initialScreenWidth = 1200;
long initialScreenHeight = 720;

enum MenuItem {DifficultyEasy, DifficultyChallenging, DifficultyImpossible,
	DisplayModeWireframe, DisplayModeTextured, RunWorld, Quit};

MenuItem difficulty; MenuItem displayMode;

bool wireframe = false;
long screenWidth;
long screenHeight;
char filename [256];

#define escapeCharacter 27
#define enterCharacter 13
#define VK_ALT VK_MENU

#define matches(a,b) strlen(b) >= strlen(a) && memcmp(a,b,strlen(a)) == 0

inline bool keyIsDown (long key) {return (GetAsyncKeyState (key) & 0x8000) != 0;}

void setupOpenGL() {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glLineWidth(3.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0, 1.0, 100.0); //See resizeWindow for parameter explanation.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat lightColor [] = {1.0f, 1.0f, 1.0f, 1.0f}; //white
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
	glEnable(GL_LIGHT0); glEnable (GL_LIGHTING);
	glDisable(GL_LIGHTING); //We can deal with our own lighting.
	glEnable(GL_COLOR_MATERIAL); //Track color.

	glClearColor(0.0, 0.0, 0.0, 1.0); //black
	glClearDepth(1.0); //Far
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);	

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	glDisable(GL_ALPHA_TEST);
	glPolygonOffset(0.0, -3.0);

	//Setup materials.
	GLfloat	frontMaterialDiffuse [4] = {0.2f, 0.2f, 0.2f, 1.0f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, frontMaterialDiffuse);
	GLfloat	frontMaterialAmbient [4] = {0.8f, 0.8f, 0.8f, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, frontMaterialAmbient);
	GLfloat frontMaterialSpecular [4] = {0.1f, 0.1f, 0.1f, 1.0f};
	glMaterialfv(GL_FRONT, GL_SPECULAR, frontMaterialSpecular);
	GLfloat	frontMaterialShininess [] = {70.0f};
	glMaterialfv(GL_FRONT, GL_SHININESS, frontMaterialShininess);
	GLfloat	frontMaterialEmission [4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_EMISSION, frontMaterialEmission);

	GLfloat	backMaterialDiffuse [4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv(GL_BACK, GL_DIFFUSE, backMaterialDiffuse);
	GLfloat	backMaterialAmbient [4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv(GL_BACK, GL_AMBIENT, backMaterialAmbient);
	GLfloat	backMaterialSpecular [4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv(GL_BACK, GL_SPECULAR, backMaterialSpecular);
	GLfloat	backMaterialShininess [] = {0.0f};
	glMaterialfv(GL_BACK, GL_SHININESS, backMaterialShininess);
	GLfloat	backMaterialEmission [4] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv(GL_BACK, GL_EMISSION, backMaterialEmission);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//Use a global default texture environment mode.
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void resizeWindow(int width, int height) {
	//Setup a new viewport.
	glViewport(0, 0, width, height);
	screenWidth = width;
	screenHeight = height;

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
			quit(0);
			break;
		
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
		if(state == GLUT_DOWN) {
			
		}
		else {
			
		}
	}
	else if(button == GLUT_MIDDLE_BUTTON) {
		if(state == GLUT_DOWN) {
			
		}
		else {
			
		}
	}
}

void mouseMoved(int x, int y) {
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

void genericMenuHandler(int item) {
	switch(item) {
		case RunWorld:
			game->import();
//			game->world = new World();
//			game->world->import();
//				player->reset(game->world->startPosition);
			break;
	}
}

void createMenus () {
	int genericMenu = glutCreateMenu (genericMenuHandler);
	glutAddMenuEntry ("Run World", RunWorld);
	glutAttachMenu (GLUT_RIGHT_BUTTON);
}

int main(int parametersSize, char ** parameters) {

	//Setup general facilities.
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL | GLUT_MULTISAMPLE);
	glutInitWindowSize(initialScreenWidth, initialScreenHeight);
	glutInit(&parametersSize, parameters);
	
	if(fullscreen) {
		glutEnterGameMode();
	}
	else {
		glutCreateWindow(gameName);
		createMenus();
	}
	
    glutIgnoreKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutSetCursor(GLUT_CURSOR_NONE);

	//Specify function handlers.
	glutDisplayFunc(displayWindow);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc (normalKeyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutMouseFunc(mousePressed);
	glutMotionFunc (mouseMoved);
	glutVisibilityFunc(visibilityChanged);
    glutPassiveMotionFunc(mouseMoved); 
    glutSpecialUpFunc(specialKeyReleased);
    glutKeyboardUpFunc(normalKeyReleased);
	
	setupOpenGL();
	Game::setupFont();
	game = new Game;
	player = new Player;
	camera = new Camera;
	inputManager = new InputManager;
	
	try {
		glutMainLoop();
	}
	catch(const char * msg) {
		if(msg[0] != '\0') {
			printf("Terminating: %s\n", msg);
		}
	}
	Game::wrapupFont();
	if(game != NULL) { delete game; }
	if(player != NULL) { delete player; }
	if(camera != NULL) { delete camera; }
	if(inputManager != NULL) { delete inputManager; }
	
	return 0;
}
