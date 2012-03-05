#include "Game.h"

#if _DEBUG
#include <crtdbg.h>
#include <vld.h>
#endif

Game * game = NULL;

void exitGame();

void resizeWindow(int width, int height) {
	//Setup a new viewport.
	glViewport(0, 0, width, height);
	Game::settings->windowWidth = width;
	Game::settings->windowHeight = height;

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

void displayWindow() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	game->draw();
	glutSwapBuffers();
}

void idle() {
	game->update();
	if(GetFocus()) {
		SetCursorPos(Game::settings->windowWidth >> 1, Game::settings->windowHeight >> 1); //Re-center the mouse...
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
			Game::menu->moveUp();
			break;
		case GLUT_KEY_DOWN:
			Game::menu->moveDown();
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
	glutPostRedisplay();
}

void normalKeyPressed(unsigned char character, int x, int y) {
	switch(character) {
		case 27: // escape
			Game::menu->back();
			break;
		
		case 'w':
		case 'W':
			if(!game->isPaused()) {
				Game::camera->translateAhead = true;
			}
			Game::menu->moveUp();
			break;
			
		case 's':
		case 'S':
			if(!game->isPaused()) {
				Game::camera->translateBack = true;
			}
			Game::menu->moveDown();
			break;
			
		case 'a':
		case 'A':
			if(!game->isPaused()) {
				Game::camera->translateLeft = true;
			}
			break;
			
		case 'd':
		case 'D':
			if(!game->isPaused()) {
				Game::camera->translateRight = true;
			}
			break;
		
		case 'q':
		case 'Q':
		case ' ':
			if(!game->isPaused()) {
				Game::camera->translateUp = true;
			}
			Game::menu->select();
			break;
		
		case 'e':
		case 'E':
		case 'z':
		case 'Z':
			if(!game->isPaused()) {
				Game::camera->translateDown = true;
			}
			break;
			
		case '?':
			break;
			
		case 13: // enter
			Game::menu->select();
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

void normalKeyReleased(unsigned char character, int x, int y) {
	switch (character) {
		case 'w':
		case 'W':
			if(!game->isPaused()) {
				Game::camera->translateAhead = false;
			}
			break;
			
		case 's':
		case 'S':
			if(!game->isPaused()) {
				Game::camera->translateBack = false;
			}
			break;
			
		case 'a':
		case 'A':
			if(!game->isPaused()) {
				Game::camera->translateLeft = false;
			}
			break;
			
		case 'd':
		case 'D':
			if(!game->isPaused()) {
				Game::camera->translateRight = false;
			}
			break;

		case 'q':
		case 'Q':
		case ' ':
			if(!game->isPaused()) {
				Game::camera->translateUp = false;
			}
			break;

		case 'e':
		case 'E':
		case 'z':
		case 'Z':
			if(!game->isPaused()) {
				Game::camera->translateDown = false;
			}
			break;
	}

	glutPostRedisplay();
}

void mousePressed(int button, int state, int x, int y) {
	static bool leftButtonDown = false;

	if(button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN) {
			if(!leftButtonDown) {
				game->throwGrassBlock();
			}
			leftButtonDown = true;
		}
		else {
			leftButtonDown = false;
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

	POINT screenPoint;
	GetCursorPos (& screenPoint); 
	Point point(screenPoint.x, screenPoint.y, 0.0);
	Point center(Game::settings->windowWidth >> 1, Game::settings->windowHeight >> 1, 0.0);

	Point difference = (point - center) * (Game::settings->mouseSensitivity / 10.0);
	Point rotation(-difference.y, -difference.x, 0.0);

	Game::camera->rotateBy(rotation);
}

void setupOpenGL() {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glLineWidth(3.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat lightColor [] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
	glEnable(GL_LIGHT0); glEnable (GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
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

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

int main(int argc, char *argv[]) {
	srand((unsigned int) time(NULL));

	game = new Game();
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL | GLUT_MULTISAMPLE);
	glutInitWindowSize(Game::settings->windowWidth, Game::settings->windowHeight);
	glutInit(&argc, argv);

	if(Game::settings->fullScreen) {
		glutEnterGameMode();
	}
	else {
		glutCreateWindow(Game::settings->gameName);
	}

	if(glewInit() != GLEW_OK) {
		quit("Unable to initialize GLEW.");
	}

	if(!GLEW_VERSION_1_5) {
		printf("WARNING: OpenGL 1.5 not supported, vertex buffers disabled.\n");
	}

	if(!GLEW_VERSION_2_0) {
		printf("WARNING: OpenGL 2.0 not supported, shaders disabled.\n");
	}
	
    glutIgnoreKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutSetCursor(GLUT_CURSOR_NONE);

	glutDisplayFunc(displayWindow);
	glutReshapeFunc(resizeWindow);
	glutKeyboardFunc(normalKeyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMoved);
	glutVisibilityFunc(visibilityChanged);
    glutPassiveMotionFunc(mouseMoved); 
    glutSpecialUpFunc(specialKeyReleased);
    glutKeyboardUpFunc(normalKeyReleased);

	setupOpenGL();

	if(!game->init()) {
		delete game;
		prompt("Failed to initialize game!");
		return 1;
	}
	
	atexit(exitGame);
	
	glutMainLoop();
	
	return 0;
}

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

	if(game != NULL) {
		Game::settings->save();
		delete game;
	}

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}
