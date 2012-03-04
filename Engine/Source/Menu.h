#ifndef MENU_H
#define MENU_H

#include "Includes.h"
#include "Colour.h"
#include "Text.h"

class Game;

// simple menu abstraction (only used for storing, rendering and interaction - function calls done by game engine)
class Menu {
public:
	Menu();
	~Menu();

	void addMenuItem(char * menuItem); // add a new item to the bottom of the menu

	void select();
	void back();
	void moveUp(); // move menu selection up
	void moveDown(); // move menu selection down

	bool isActive();
	void activate();
	void deactivate();
	void toggle();

	void draw(); // draw the menu

private:
	void loadMapList();
	void setMenu(int type);
	void pauseGame();
	void resumeGame();

public:
	const static int MENU_MAIN;
	const static int MENU_NEWGAME;
	const static int MENU_HELP;
	const static int MENU_OPTIONS;

private:
	// menu data
	Font * titleFont;
	Font * itemFont;
	Text * mainMenuTitle;
	Text * mapMenuTitle;
	Text * optionsMenuTitle;
	Text * helpMenuTitle;
	vector<Text *> mainMenuItems;
	vector<Text *> mapMenuItems;
	vector<Text *> optionsMenuItems;
	vector<Text *> helpMenuItems;

	vector<string> mapList;

	int mainMenuIndex;
	int mapMenuIndex;
	int optionsMenuIndex;
	int menuType;
	bool active;

	// menu colours
	Colour titleColour;
	Colour activeColour;
	Colour inactiveColour;

	// menu draw position information
	int menuItemOffset;
	int menuItemIncrement;
};

#endif // MENU_H
