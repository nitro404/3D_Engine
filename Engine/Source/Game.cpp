#include "Game.h"

extern int screenWidth, screenHeight;

Game * game = NULL;

double DT;

Game::Game(int windowWidth,
		   int windowHeight,
		   Variables * gameSettings)
			: fps(NULL),
			  settings(gameSettings),
			  world(NULL),
			  paused(true) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	drawFPS = isTrue(settings->getValue("Show FPS"));
	fps = new char[12];
	fps[0] = '\0';
	Font fpsFont(windowWidth, windowHeight, "Arial", 24, Font::BOLD, false, false, false, 0);
	fpsText = new Text(windowWidth, windowHeight, windowWidth - 101, windowHeight - 20, Colour(255, 0, 0, 255), &fpsFont, false, "FPS");
	
	verifySettings();
	
	loadTextures(settings->getValue("Texture Data File"), settings->getValue("Texture Directory"), settings->getValue("Shader Directory"), settings->getValue("Height Map Directory"));

#ifdef _DEBUG
	drawFPS = true;
#endif
}

Game::~Game() {
	delete [] fps;
	delete fpsText;
	delete settings;
	if(world != NULL) { delete world; }
	for(unsigned int i=0;i<animatedTextures.size();i++) {
		delete animatedTextures.at(i);
	}
	for(unsigned int i=0;i<shaders.size();i++) {
		delete shaders.at(i);
	}
	for(unsigned int i=0;i<heightMaps.size();i++) {
		delete [] heightMaps.at(i);
	}
	for(unsigned int i=0;i<textures.size();i++) {
		delete textures.at(i);
	}
}

void Game::tick() {
	if(paused) { return; }

	inputManager->tick();
	if(world != NULL) {
		world->tick ();
	}
}

void Game::draw() {
	if(world != NULL) { world->drawSkybox(); }
	camera->beginCamera();
	if(world != NULL) { world->draw(); }
	camera->endCamera();
	
	if(drawFPS) {
		drawFrameRate();
	}
}

void Game::resume() { if(world != NULL) { paused = false; } }
void Game::pause() { paused = true; }
void Game::unpause() { paused = false; }
void Game::togglePause() { paused = !paused; }
bool Game::isPaused() { return paused; }

void Game::loadMap(char * fileName) {
	world = new World();
	world->import(fileName, textures, heightMaps, animatedTextures, shaders);
	player->reset(world->startPosition);
	paused = false;
}

void Game::closeMap() {
	if(world != NULL) {
		delete world;
		world = NULL;
	}
}

void Game::drawFrameRate() {
	// draw the frame rate avoiding extreme fluctuations (since all you see is flickering).
	double frameRate = 1.0 / DT; // frames/sec = 1/(seconds per frame).
	static double stableRate = frameRate; // this initializes only the first time...
	static double oldFrameRate = frameRate;
	// if it changed by more than 2 per cent of the stable value, use the new value; otherwise use the stable one...
	if(fabs(frameRate - stableRate) > 2.0) {
		stableRate = frameRate;
	}
	sprintf_s(fps, 12, "%3.1f FPS", stableRate);
	fpsText->setPosition(screenWidth - 101 - ((stableRate > 99) ? 13 : 0), screenHeight - 20);
	fpsText->draw(fps);
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
	if(settings->getValue("Height Map Directory") == NULL) {
		quit("No height map directory specified in settings file.");
	}
	if(settings->getValue("Shader Directory") == NULL) {
		quit("No shader directory specified in settings file.");
	}
}

void Game::loadTextures(const char * fileName, const char * textureDirectory, const char * shaderDirectory, const char * heightMapDirectory) {
	char line[256];
	unsigned int i, j;
	int startIndex;
	int length;

	ifstream input;
	input.open(fileName); 
	if(!input.is_open()) {
		quit("Unable to open texture data file: \"%s\".", fileName);
	}
	
	// input the texture names and load the corresponding texture
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfTextures = atoi(line);
	input.getline(line, 256, '\n');
	char * textureName;
	string texturePath;
	Texture * newTexture;
	for(int textureIndex = 0; textureIndex < numberOfTextures; textureIndex++) {
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

		// load the texture
		texturePath.append(textureDirectory);
		length = strlen(textureDirectory);
		if(textureDirectory[length-1] != '\\' && textureDirectory[length-1] != '/') {
			texturePath.append("/");
		}
		texturePath.append(textureName);
		newTexture = Texture::readTexture((char *) texturePath.c_str());
		if(newTexture != NULL) {
			textures.push_back(newTexture);
		}
		else {
			prompt("Missing texture: %s", textureName);
		}
		texturePath.erase();
		delete [] textureName;
	}

	// input the height map names
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfHeightMaps = atoi(line);
	input.getline(line, 256, '\n');
	char * heightMapName;
	string heightMapPath;
	for(int heightMapIndex = 0; heightMapIndex < numberOfHeightMaps; heightMapIndex++) {
		input.getline(line, 256, '\n');
		startIndex = 0;
		for(i=startIndex;i<strlen(line);i++) {
			if(line[i] != ' ' && line[i] != '\t') {
				startIndex = i;
				break;
			}
		}
		heightMapName = new char[strlen(line) - startIndex + 1];
		j = 0;
		for(i=startIndex;i<strlen(line);i++) {
			heightMapName[j++] = line[i];
		}
		heightMapName[strlen(line) - startIndex] = '\0';
		
		heightMapPath.clear();
		heightMapPath.append(heightMapDirectory);
		length = strlen(heightMapDirectory);
		if(heightMapDirectory[length-1] != '\\' && heightMapDirectory[length-1] != '/') {
			heightMapPath.append("/");
		}
		heightMapPath.append(heightMapName);

		char * temp = new char[strlen(heightMapPath.c_str()) + 1];
		strcpy_s(temp, strlen(heightMapPath.c_str()) + 1, heightMapPath.c_str());
		heightMaps.push_back(temp);
		
		delete [] heightMapName;
	}
	
	// input the animated textures 
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	long animatedTexturesSize = atoi(line);
	input.getline(line, 256, '\n');
	for(int atIndex=0;atIndex<animatedTexturesSize;atIndex++) {
		// create the corresponding animated textures
		AnimatedTexture * animatedTexture = new AnimatedTexture;
		animatedTexture->import(input, textures);
		animatedTextures.push_back(animatedTexture);
	}

	// input the shaders
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfShaders = atoi(line);
	input.getline(line, 256, '\n');
	for(int shaderIndex = 0; shaderIndex < numberOfShaders; shaderIndex++) {
		Shader * shader = Shader::import(input, shaderDirectory);
		if(shader != NULL) {
			shaders.push_back(shader);
		}
	}
	
	input.close();
}
