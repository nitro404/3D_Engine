#include "Game.h"

Game * Game::instance = NULL;
SettingsManager * Game::settings = NULL;
Menu * Game::menu = NULL;
Camera * Game::camera = NULL;
PhysicsManager * Game::physics = NULL;
World * Game::world = NULL;

Game::Game() : pos(NULL), fps(NULL), fpsText(NULL), cullingEnabled(false), paused(true) {
	instance = this;

	settings = new SettingsManager();
	if(!settings->load()) {
		settings->save();
	}

	camera = new Camera();

	physics = new PhysicsManager();
}

Game::~Game() {
	if(pos != NULL) { delete [] pos; }
	if(fps != NULL) { delete [] fps; }
	if(fpsText != NULL) { delete fpsText; }
	delete settings;
	if(menu != NULL) { delete menu; }
	delete camera;
	if(world != NULL) { delete world; }
	for(unsigned int i=0;i<cubes.size();i++) {
		delete cubes.at(i);
	}
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
	delete physics;
}

bool Game::init() {
	if(!physics->init()) { return false; }

	if(!camera->init()) { return false; }

	menu = new Menu();

	// initialize the fps font
	pos = new char[32];
	pos[0] = '\0';
	currentFPS = 0;
	fps = new char[12];
	fps[0] = '\0';
	Font fpsFont("Arial", 24, Font::BOLD, false, false, false, 0);
	fpsText = new Text(settings->windowWidth - 101, settings->windowHeight - 20, Colour(255, 0, 0, 255), &fpsFont, false, "FPS");
	
	// load the textures, animations, shaders and height map data
	loadResources();

	return true;
}

void Game::update() {
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
	double timeElapsed = timeInSeconds - lastTimeInSeconds;
	if(timeElapsed > 0.2) { timeElapsed = 0.2; } //5 frames/sec means 1 frame in 1/5 (= 0.2) seconds.
	lastTimeInSeconds = timeInSeconds;

	updateFPS(timeElapsed);

	if(paused) { return; }

	camera->update(timeElapsed);

	if(world != NULL) {
		world->update(timeElapsed);
	}

	for(unsigned int i=0;i<cubes.size();i++) {
		cubes.at(i)->update(timeElapsed);
	}
	
	physics->update(timeElapsed);

	camera->handleCollisions(timeElapsed);
}

void Game::draw() {
	if(world != NULL) { world->drawSkybox(); }

	camera->beginCamera();

	if(world != NULL) { world->draw(); }

	for(unsigned int i=0;i<cubes.size();i++) {
		cubes.at(i)->draw();
	}

	camera->endCamera();
	
	if(settings->showFPS) {
		drawFPS();
	}

	drawPosition();

	drawFly();

	menu->draw();

	physics->fetchResults(true);
}

void Game::resume() { if(world != NULL) { paused = false; } }
void Game::pause() { paused = true; }
void Game::unpause() { paused = false; }
void Game::togglePause() { paused = !paused; }
bool Game::isPaused() { return paused; }

void Game::loadMap(char * fileName) {
	clearObjects();
	physics->reset();

	world = new World();
	world->import(fileName, textures, heightMaps, animatedTextures, shaders);
	world->cullingEnabled = cullingEnabled;
	camera->reset(world->startPosition);
	paused = false;
}

void Game::closeMap() {
	if(world != NULL) {
		clearObjects();
		physics->reset();

		delete world;
		world = NULL;
	}
}

void Game::toggleFly() {
	camera->toggleFly();
}

void Game::throwGrassBlock() {
	Texture ** textures = new Texture*[6];
	textures[0] = this->textures[62];
	textures[1] = this->textures[62];
	textures[2] = this->textures[60];
	textures[3] = this->textures[62];
	textures[4] = this->textures[61];
	textures[5] = this->textures[62];

	Cube * c = new Cube(camera->cameraMatrix, 2, camera->forwardVector() * 10, 1, Colour(255, 255, 255, 255), textures);
	c->load();
	cubes.push_back(c);

	delete [] textures;
}

void Game::clearObjects() {
	for(unsigned int i=0;i<cubes.size();i++) {
		delete cubes.at(i);
	}
	cubes.clear();
}

void Game::updateFPS(double timeElapsed) {
	// draw the frame rate avoiding extreme fluctuations (since all you see is flickering).
	double frameRate = timeElapsed == 0 ? 0 : 1.0 / timeElapsed; // frames/sec = 1/(seconds per frame).
	static double oldFrameRate = frameRate;
	// if it changed by more than 2 per cent of the stable value, use the new value; otherwise use the stable one...
	if(fabs(frameRate - currentFPS) > 2.0) {
		currentFPS = frameRate;
	}
}

void Game::drawFPS() {
	sprintf_s(fps, 12, "%.0f FPS", currentFPS);
	fpsText->setPosition(settings->windowWidth - 101 - ((currentFPS > 99) ? 13 : 0), settings->windowHeight - 20);
	fpsText->draw(fps);
}

void Game::drawPosition() {
	Point p = camera->getPosition();
	sprintf_s(pos, 32, "(%.2f, %.2f, %.2f)", p.x, p.y, p.z);
	fpsText->setPosition((settings->windowWidth / 2) - 80, settings->windowHeight - 20);
	fpsText->draw(pos);
}

void Game::drawFly() {
	fpsText->setPosition(0, settings->windowHeight - 20);
	fpsText->draw(camera->fly ? "FLYING ENABLED" : "FLYING DISABLED");
}

void Game::loadResources() {
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
