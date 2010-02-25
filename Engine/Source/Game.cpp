#include "Game.h"

extern int screenWidth, screenHeight;

Game * game = NULL;

double DT;

Game::Game(Variables * settings) {
	// setup the font
	deviceContext = GetDC(NULL);
	HFONT font; //Windows font ID...
	fontBase = glGenLists(96); //Storage for 96 characters
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
	
	SelectObject(deviceContext, font);			//Selects The Font We Want
	wglUseFontBitmaps(deviceContext, 32, 96, fontBase); //Builds 96 characters starting at character 32
	
	this->settings = settings;
	displayHelp = false;
	drawFPS = true;
	fps = new char[12];
	fps[0] = '\0';
	selectionPointer = "->";
	menuOffsetX = 200;
	menuOffsetY = 200;
	menuIndex = 0;
	menuSpacing = 20;
	menuType = 0;
	menuItems.push_back("New Game");
	menuItems.push_back("Quit");
	menuTitles.push_back("Main Menu");
	menuTitles.push_back("Load Map");
	menuColour = Colour(255, 0, 0, 255);
	helpMessage = "Use WSAD to move around, Space/Z to move up/down and Escape to exit.";
	
	verifySettings();
	world = NULL;		
	worldFileFilter = "World File (*.wrl)|*.wrl|All Files (*.*)|*.*";
	
	loadTextures(settings->getValue("Texture Data File"), settings->getValue("Texture Directory"));
	loadMapList(settings->getValue("Map Directory"));
}

Game::~Game() {
	unsigned int i;
	if(fps != NULL) { delete [] fps; }
	if(settings != NULL) { delete settings; }
	//if(helpMessage != NULL) { delete [] helpMessage; } // The string is located in the code, not the heap
	//if(selectionPointer != NULL) { delete [] selectionPointer; } // The string is located in the code, not the heap
	//for(i=0;i<menuItems.size();i++) { // The entries of menuItems are located in the code, not the heap
	//	delete [] menuItems.at(i);		// Unless additional entries are added somewhere, this will not cause a problem
	//}
	//for(i=0;i<menuTitles.size();i++) { // The entries of menuTitles are located in the code, not the heap
	//	delete [] menuTitles.at(i);		// Unless additional entries are added somewhere, this will not cause a problem
	//}
	//if(worldFileFilter != NULL) { delete [] worldFileFilter; } // The string is located in the code, not the heap
	if(world != NULL) { delete world; }
	for(i=0;i<animatedTextures.size();i++) {
		delete animatedTextures.at(i);
	}
	for(i=0;i<textures.size();i++) {
		delete textures.at(i);
	}
	ReleaseDC(NULL, deviceContext);
	glDeleteLists(fontBase, 96);
}

void Game::tick() {
	inputManager->tick();
	camera->tick();
	player->tick();
	if(world != NULL) {
		world->tick ();
	}
}

void Game::draw() {
	camera->beginCamera();
	if(world != NULL) {
		world->draw();
	}
	player->draw();
	camera->endCamera();
	
	if(drawFPS) {
		drawFrameRate();
	}
	if(displayHelp || world == NULL) {
		drawText(1, screenHeight-21, helpMessage);
	}
	if(world == NULL) {
		drawMenu();
	}
}

void Game::menuNextItem() {
	if(world == NULL) {
		int lastItem = 0;
		if(menuType == 0) {
			lastItem = menuItems.size() - 1;
		}
		else if(menuType == 1) {
			lastItem = mapList.size() - 1;
		}
		
		if(menuIndex >= lastItem) {
			menuIndex = 0;
		}
		else {
			menuIndex++;
		}
	}
}

void Game::menuPrevItem() {
	if(world == NULL) {
		int lastItem = 0;
		if(menuType == 0) {
			lastItem = menuItems.size() - 1;
		}
		else if(menuType == 1) {
			lastItem = mapList.size() - 1;
		}
		
		if(menuIndex <= 0) {
			menuIndex = lastItem;
		}
		else { 
			menuIndex--;
		}
	}
}

void Game::selectMenuItem() {
	if(world == NULL) {
		if(menuType == 0) {
			if(menuIndex == 0) {
				menuType = 1;
			}
			else if(menuIndex == 1) {
				quit(0);
			}
		}
		else if(menuType == 1) {
			closeMap();
			loadMap((char *) mapList.at(menuIndex).c_str());
			menuIndex = 0;
			menuType = 0;
		}
	}
}

void Game::escapePressed() {
	if(world != NULL) {
		game->closeMap();
	}
	else {
		if(menuType == 0) {
			quit(0);
		}
		if(menuType == 1) {
			menuType = 0;
		}
	}
}

void Game::loadMap(char * fileName) {
	world = new World;
	world->import(fileName, textures, animatedTextures);
	player->reset(world->startPosition);
}

void Game::closeMap() {
	if(world != NULL) {
		delete world;
		world = NULL;
	}
}	

void Game::drawMenu() {
	int yOffset = screenHeight-menuOffsetY;
	drawText(menuOffsetX, yOffset, menuTitles.at(menuType));
	yOffset -= menuSpacing;
	drawText(menuOffsetX-25, yOffset - (menuIndex * menuSpacing), selectionPointer);
	if(menuType == 0) {
		for(unsigned int i=0;i<menuItems.size();i++) {
			drawText(menuOffsetX, yOffset, menuItems.at(i));
			yOffset -= menuSpacing;
		}
	}
	else if(menuType == 1) {
		for(unsigned int i=0;i<mapList.size();i++) {
			drawText(menuOffsetX, yOffset, strchr((char *) mapList.at(i).c_str(), '\\') + sizeof(char));
			yOffset -= menuSpacing;
		}
	}
}

void Game::drawFrameRate() {
	//Draw the frame rate avoiding extreme fluctuations (since all you see is flickering).
	double frameRate = 1.0 / DT; //Frames/sec = 1/(seconds per frame).
	static double stableRate = frameRate; //This initializes only the first time...
	static double oldFrameRate = frameRate;
	//If it changed by more than 2 per cent of the stable value, use the new value; otherwise use the stable one...
	if(absolute(frameRate - stableRate) > 2.0) stableRate  = frameRate; 
	sprintf_s(fps, 12, "%3.1f FPS", stableRate);
	drawText(screenWidth-101, screenHeight-20, fps);
}

void Game::drawText(int x, int y, const char * text) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity ();
		glOrtho(0.0,(GLfloat) screenWidth,0.0,(GLfloat) screenHeight, -100.0f, 100.0f);
		glMatrixMode (GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity ();
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_LIGHTING);
				glColor4f(menuColour.red, menuColour.green, menuColour.blue, menuColour.alpha);
				glRasterPos2i(x, y);
				glPushAttrib(GL_LIST_BIT);	//Pushes the display list bits
				glListBase(fontBase - 32); //Sets the base character to 32
				glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); //Draws the display list text
				glPopAttrib(); //Pops the display list bits
				glColor4f(1, 1, 1, 1);
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Game::verifySettings() {
	if(settings->getValue("Map Directory") == NULL) {
		quit("No map directory specified in settings file.");
	}
	if(settings->getValue("Texture Directory") == NULL) {
		quit("No texture directory specified in settings file.");
	}
	if(settings->getValue("Texture Data File") == NULL) {
		quit("No texture data file specified in settings file.");
	}
}

void Game::loadMapList(char * mapDirectory) {
	string rootDirectory = mapDirectory;
	string filePath;
	string strPattern;
	string strFileName;
	HANDLE hFile;
	WIN32_FIND_DATA fileInformation;

	strPattern = rootDirectory + "\\*.wrl";

	hFile = ::FindFirstFile(strPattern.c_str(), & fileInformation);
	if(hFile != INVALID_HANDLE_VALUE) {
		do {
			if(fileInformation.cFileName[0] != '.') {
				filePath.erase();
				filePath = rootDirectory + "\\" + fileInformation.cFileName;

				if(!(fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					strFileName = fileInformation.cFileName;
					mapList.push_back(filePath);
				}
			}
		} while(::FindNextFile(hFile, &fileInformation) == TRUE);

		::FindClose(hFile);
	}
}

void Game::loadTextures(char * fileName, char * textureDirectory) {
	char * line;
	line = new char[256];
	unsigned int i, j;

	ifstream input;
	input.open(fileName); 
	if(input.bad()) {
		quit("Unable to open texture data file: \"%s\".", fileName);
	}
	
	//Input the texture names and load the corresponding texture
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int texturesSize = atoi(line);
	input.getline(line, 256, '\n');
	char * textureName;
	int startIndex;
	string texturePath;
	Texture * newTexture;
	for(int textureIndex = 0; textureIndex < texturesSize; textureIndex++) {
		input.getline(line, 256, '\n');
		startIndex = 0;
		for(i=startIndex;i<strlen(line);i++) {
			if(line[i] != ' ' && line[i] != '\t') {
				startIndex = i;
				break;
			}
		}
		textureName = new char[strlen(line) - startIndex + 1];
		j = 0;
		for(i=startIndex;i<strlen(line);i++) {
			textureName[j++] = line[i];
		}
		textureName[strlen(line) - startIndex] = '\0';

		//Load the texture
		texturePath.append(textureDirectory);
		texturePath.append("/");
		texturePath.append(textureName);
		newTexture = Texture::readTexture((char *) texturePath.c_str());
		if(newTexture != NULL) {
			newTexture->load();
			textures.push_back(newTexture);
		}
		else {
			prompt("Missing texture: %s", textureName);
		}
		texturePath.erase();
		delete [] textureName;
	}
	
	//Input the animated textures 
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	long animatedTexturesSize = atoi(line);
	input.getline(line, 256, '\n');
	for(int atIndex=0;atIndex<animatedTexturesSize;atIndex++) {
		//Create the corresponding animated textures
		AnimatedTexture * animatedTexture = new AnimatedTexture;
		animatedTexture->import(input, textures);
		animatedTextures.push_back(animatedTexture);
	}
	
	delete [] line;
	input.close();
}
