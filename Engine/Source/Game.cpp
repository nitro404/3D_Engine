#include "Game.h"

double DT;

Game * Game::instance = NULL;
SettingsManager * Game::settings = NULL;
Menu * Game::menu = NULL;
World * Game::world = NULL;

Game::Game()
			: fps(NULL),
			  cullingEnabled(false),
			  paused(true) {
	instance = this;

	settings = new SettingsManager();
	if(!settings->load()) {
		settings->save();
	}
}

Game::~Game() {
	delete [] fps;
	delete fpsText;
	delete settings;
	delete menu;
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

bool Game::init() {
	menu = new Menu();

	// initialize the fps font
	fps = new char[12];
	fps[0] = '\0';
	Font fpsFont("Arial", 24, Font::BOLD, false, false, false, 0);
	fpsText = new Text(settings->windowWidth - 101, settings->windowHeight - 20, Colour(255, 0, 0, 255), &fpsFont, false, "FPS");
	
	// load the textures, animations, shaders and height map data
	loadTextures();

	// set lighting values (not currently used)
	GLfloat diffuseLight[] = {0, 0, 0};
	GLfloat ambientLight[] = {1, 1, 1};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	GLfloat lightPosition[] = {0, 5, 1, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	return true;
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
	
	if(settings->showFPS) {
		drawFrameRate();
	}

	menu->draw();
}

void Game::resume() { if(world != NULL) { paused = false; } }
void Game::pause() { paused = true; }
void Game::unpause() { paused = false; }
void Game::togglePause() { paused = !paused; }
bool Game::isPaused() { return paused; }

void Game::loadMap(char * fileName) {
	world = new World();
	world->import(fileName, textures, heightMaps, animatedTextures, shaders);
	world->cullingEnabled = cullingEnabled;
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
	fpsText->setPosition(settings->windowWidth - 101 - ((stableRate > 99) ? 13 : 0), settings->windowHeight - 20);
	fpsText->draw(fps);
}

void Game::loadTextures() {
	char line[256];
	unsigned int i, j;
	int startIndex;
	int length;

	string textureDataFilePath;
	textureDataFilePath.append(settings->dataDirectoryName);
	length = strlen(settings->dataDirectoryName);
	if(settings->dataDirectoryName[length-1] != '\\' && settings->dataDirectoryName[length-1] != '/') {
		textureDataFilePath.append("/");
	}
	textureDataFilePath.append(settings->textureDataFileName);

	ifstream input;
	input.open(textureDataFilePath.c_str()); 
	if(!input.is_open()) {
		quit("Unable to open texture data file: \"%s\".", textureDataFilePath.c_str());
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
		texturePath.append(settings->dataDirectoryName);
		length = strlen(settings->dataDirectoryName);
		if(settings->dataDirectoryName[length-1] != '\\' && settings->dataDirectoryName[length-1] != '/') {
			texturePath.append("/");
		}
		texturePath.append(settings->textureDirectoryName);
		length = strlen(settings->textureDirectoryName);
		if(settings->textureDirectoryName[length-1] != '\\' && settings->textureDirectoryName[length-1] != '/') {
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
		heightMapPath.append(settings->dataDirectoryName);
		length = strlen(settings->dataDirectoryName);
		if(settings->dataDirectoryName[length-1] != '\\' && settings->dataDirectoryName[length-1] != '/') {
			heightMapPath.append("/");
		}
		heightMapPath.append(settings->heightMapDirectoryName);
		length = strlen(settings->heightMapDirectoryName);
		if(settings->heightMapDirectoryName[length-1] != '\\' && settings->heightMapDirectoryName[length-1] != '/') {
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

	string shaderPath;
	shaderPath.append(settings->dataDirectoryName);
	length = strlen(settings->dataDirectoryName);
	if(settings->dataDirectoryName[length-1] != '\\' && settings->dataDirectoryName[length-1] != '/') {
		shaderPath.append("/");
	}
	shaderPath.append(settings->shaderDirectoryName);
	length = strlen(settings->shaderDirectoryName);
	if(settings->shaderDirectoryName[length-1] != '\\' && settings->shaderDirectoryName[length-1] != '/') {
		shaderPath.append("/");
	}

	// input the shaders
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfShaders = atoi(line);
	input.getline(line, 256, '\n');
	for(int shaderIndex = 0; shaderIndex < numberOfShaders; shaderIndex++) {
		Shader * shader = Shader::import(input, shaderPath.c_str());
		if(shader != NULL) {
			shaders.push_back(shader);
		}
	}
	
	input.close();
}
