#pragma once

#include "Includes.h"
#include "Variable.h"
#include "Variables.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "InputManager.h"

extern double DT;

class Game {
public:
	Game(Variables * settings);
	~Game();
	
	void tick();
	void draw();
	
	void menuNextItem();
	void menuPrevItem();
	void selectMenuItem();
	void escapePressed();
	void closeMap();
	
private:
	void drawText(int x, int y, const char * text);
	void drawMenu();
	void drawFrameRate();

	void verifySettings();
	void loadTextures(char * fileName, char * textureDirectory, char * heightMapDirectory);
	void loadMapList(char * mapDirectory);
	void loadMapPrompt();
	void loadMap(char * fileName);
	
public:
	HDC deviceContext;
	GLuint fontBase;

	World * world;
	bool displayHelp;

	vector<Texture *> textures;
	vector<char *> heightMaps;
	vector<AnimatedTexture *> animatedTextures;

private:
	Variables * settings;

	bool drawFPS;
	char * fps;

	char * helpMessage;
	char * selectionPointer;
	Colour menuColour;
	int menuOffsetX;
	int menuOffsetY;
	int menuIndex;
	int menuSpacing;
	int menuType;
	vector<char *> menuItems;
	vector<char *> menuTitles;
	vector<string> mapList;
};

extern Game * game;
