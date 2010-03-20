#pragma once

#include "Includes.h"
#include "Text.h"
#include "Variable.h"
#include "Variables.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "InputManager.h"

extern double DT;

class Game {
public:
	Game(int windowWidth, int windowHeight, Variables * settings);
	~Game();
	
	void tick();
	void draw();

	void resume();
	void pause();
	void unpause();
	void togglePause();
	bool isPaused();

	void loadMap(char * fileName);
	void closeMap();
	
private:
	void drawFrameRate();

	void verifySettings();
	void loadTextures(char * fileName, char * textureDirectory, char * heightMapDirectory);
	
public:
	World * world;

	vector<Texture *> textures;
	vector<char *> heightMaps;
	vector<AnimatedTexture *> animatedTextures;

private:
	Variables * settings;

	bool drawFPS;
	char * fps;
	Text * fpsText;

	bool paused;

	int windowWidth;
	int windowHeight;
};

extern Game * game;
