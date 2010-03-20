#pragma once

#include "Includes.h"
#include "Colour.h"
#include "Text.h"
#include "Variables.h"

class Game;

// simple menu abstraction (only used for storing, rendering and interaction - function calls done by game engine)
class Menu {
public:
	Menu(int windowWidth, int windowHeight, Game * externalGame, Variables * externalSettings);
	~Menu();

	void addMenuItem(char * menuItem); // add a new item to the bottom of the menu

	void select(); // get the current menu position
	void back();
	void moveUp(); // move menu selection up
	void moveDown(); // move menu selection down

	bool isActive();
	void activate();
	void deactivate();
	void toggle();

	void draw(); // draw the menu

private:
	void loadMapList(char * mapDirectory);
	void setMenu(int type);
	void pauseGame();
	void resumeGame();

private:
	// menu data
	Font * titleFont;
	Font * itemFont;
	Text * mainMenuTitle;
	Text * mapMenuTitle;
	Text * helpMenuTitle;
	vector<Text *> mainMenuItems;
	vector<Text *> mapMenuItems;
	vector<Text *> helpMenuItems;

	Game * game;
	Variables * settings;

	vector<string> mapList;

	int index;
	int menuType;
	bool active;

	// menu colours
	Colour titleColour;
	Colour activeColour;
	Colour inactiveColour;

	// menu draw position information
	int menuItemOffset;
	int menuItemIncrement;

	// window dimensions
	int windowWidth;
	int windowHeight;
};
