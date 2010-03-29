#include "Game.h"
#include "Menu.h"

int mouseSensitivity = 4;

void exitGame();

bool wireframe = false;
int screenWidth;
int screenHeight;

Menu * menu = NULL;

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
	//Returns how much time has elapsed since the first call of this function... Accurate to a microsecond...
	static INT64 countsPerSecond;
	static INT64 oldTime;
	static bool firstTime = true;
	if(firstTime) {
		firstTime = false;
		QueryPerformanceCounter((LARGE_INTEGER *) & oldTime);
		QueryPerformanceFrequency((LARGE_INTEGER *) & countsPerSecond);
	}
	
	INT64 newTime;
	QueryPerformanceCounter((LARGE_INTEGER *) & newTime);
	INT64 elapsedCounts = newTime - oldTime; 
	
	double seconds = (double) elapsedCounts / (double) countsPerSecond; //count / (counts / second) = seconds
	
	//Compute elapsed time needed for controlling frame rate independent effects.
	//If running slower than 5 frames per second, pretend it's 5 frames/sec.
	//Note: 30 frames per second means 1/30 seconds per frame = 0.03333... seconds per frame.
	static double lastTimeInSeconds = seconds; //Pretend we are running 30 frames per second on the first tick.
	double timeInSeconds = seconds;
	DT = timeInSeconds - lastTimeInSeconds;
	if(DT > 0.2) DT = 0.2; //5 frames/sec means 1 frame in 1/5 (= 0.2) seconds.
	lastTimeInSeconds = timeInSeconds;
}

void displayWindow() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	game->draw();
	menu->draw();
	glutSwapBuffers();
}

void idle() {
	computeDT();
	game->tick();
	if(GetFocus()) {
		SetCursorPos(screenWidth >> 1, screenHeight >> 1); //Re-center the mouse...
	}
	glutPostRedisplay();
}

void visibilityChanged (int visible) {
	glutIdleFunc(visible == GLUT_VISIBLE ? idle : NULL);
}

void specialKeyPressed (int character, int x, int y) {
	switch(character) {
		case GLUT_KEY_F1:
			break;
		case GLUT_KEY_F2:
			break;
		case GLUT_KEY_UP:
			menu->moveUp();
			break;
		case GLUT_KEY_DOWN:
			menu->moveDown();
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

void specialKeyReleased(int character, int x, int y) {
	switch(character) {
		case GLUT_KEY_F1:
			break;
	}
	glutPostRedisplay ();
}

void normalKeyPressed(unsigned char character, int x, int y) {
	switch(character) {
		case 27: // escape
			menu->back();
			break;
		
		case 'w':
		case 'W':
			if(!game->isPaused()) {
				inputManager->translateAhead = true;
			}
			menu->moveUp();
			break;
			
		case 's':
		case 'S':
			if(!game->isPaused()) {
				inputManager->translateBack = true;
			}
			menu->moveDown();
			break;
			
		case 'a':
		case 'A':
			if(!game->isPaused()) {
				inputManager->translateLeft = true;
			}
			break;
			
		case 'd':
		case 'D':
			if(!game->isPaused()) {
				inputManager->translateRight = true;
			}
			break;
			
		case ' ':
			if(!game->isPaused()) {
				inputManager->translateUp = true;
			}
			menu->select();
			break;
			
		case 'z':
		case 'Z':
			if(!game->isPaused()) {
				inputManager->translateDown = true;
			}
			break;

		case 't':
		case 'T':
//			wireframe = !wireframe;
//			glPolygonMode (GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
			break;
			
		case '?':
			break;
			
		case 13: // enter
			menu->select();
//			if(keyIsDown (VK_MENU)) { // alt
//			if((GetAsyncKeyState(VK_MENU) & 0x8000) != 0) { // alt
//				fullscreen = !fullscreen;
//				if(fullscreen) {
//					glutEnterGameMode();
//				}
//				else {
//					glutLeaveGameMode();
//				}
//			}
			break;
	}

	glutPostRedisplay();
}

void normalKeyReleased (unsigned char character, int x, int y) {
	switch (character) {
		case 'w':
		case 'W':
			if(!game->isPaused()) {
				inputManager->translateAhead = false;
			}
			break;
			
		case 's':
		case 'S':
			if(!game->isPaused()) {
				inputManager->translateBack = false;
			}
			break;
			
		case 'a':
		case 'A':
			if(!game->isPaused()) {
				inputManager->translateLeft = false;
			}
			break;
			
		case 'd':
		case 'D':
			if(!game->isPaused()) {
				inputManager->translateRight = false;
			}
			break;

		case ' ':
			if(!game->isPaused()) {
				inputManager->translateUp = false;
			}
			break;

		case 'z':
		case 'Z':
			if(!game->isPaused()) {
				inputManager->translateDown = false;
			}
			break;
	}

	glutPostRedisplay();
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
	if(!GetFocus() || game->isPaused()) { return; }

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
	POINT screenPoint;
	GetCursorPos (& screenPoint); 
	Point point(screenPoint.x, screenPoint.y, 0.0);
	Point center(screenWidth >> 1, screenHeight >> 1, 0.0);

	Point difference = (point - center) * (mouseSensitivity / 10.0);
	Point rotation(-difference.y, -difference.x, 0.0);

	inputManager->rotateBy(rotation * (InputManager::rotationSpeed * DT)); //degrees = degrees per second * second
}

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

int main(int argc, char *argv[]) {
	bool fullscreen = false;
	int initialScreenWidth = 640;
	int initialScreenHeight = 480;

	srand((unsigned int) time(NULL));

	//Read the settings file and pass ownership over to the game itself
	Variables * settings = new Variables();
	if (!settings->parseFrom("settings.ini")) {
		delete settings;
		quit("Could not find the settings file.");
	}
	
	//Initialize game settings
	int temp;
	temp = atoi(settings->getValue("Mouse Sensitivity"));
	if(temp > 0) { mouseSensitivity = temp; }
	temp = atoi(settings->getValue("Screen Width"));
	if(temp > 0) { initialScreenWidth = temp; }
	temp = atoi(settings->getValue("Screen Height"));
	if(temp > 0) { initialScreenHeight = temp; }
	fullscreen = isTrue(settings->getValue("Fullscreen"));
	
	//Setup general facilities.
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL | GLUT_MULTISAMPLE);
	glutInitWindowSize(initialScreenWidth, initialScreenHeight);
	glutInit(&argc, argv);

	if(fullscreen) {
		glutEnterGameMode();
	}
	else {
		glutCreateWindow((settings->getValue("Game Name") == NULL) ? "3D Game Engine" : settings->getValue("Game Name"));
//		CMenu * menu = GetSystemMenu(false);
//		menu->ModifyMenu(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
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
	game = new Game(initialScreenWidth, initialScreenHeight, settings);
	player = new Player;
	camera = new Camera;
	inputManager = new InputManager;

	menu = new Menu(initialScreenWidth, initialScreenHeight, game, settings);
	
	atexit(exitGame);
	
	glutMainLoop();
	
	return 0;
}

#if _DEBUG
	#include <crtdbg.h>
	#include <vld.h>
#endif

void exitGame() {
	glutReshapeFunc(NULL);
	glutKeyboardFunc(NULL);
	glutSpecialFunc(NULL);
	glutMouseFunc(NULL);
	glutMotionFunc(NULL);
	glutVisibilityFunc(NULL);
    glutPassiveMotionFunc(NULL); 
    glutSpecialUpFunc(NULL);
    glutKeyboardUpFunc(NULL);

	if(menu != NULL) { delete menu; }
	if(game != NULL) { delete game; }
	if(player != NULL) { delete player; }
	if(camera != NULL) { delete camera; }
	if(inputManager != NULL) { delete inputManager; }

	#if _DEBUG
		_CrtDumpMemoryLeaks();
	#endif
}
