#include "Menu.h"
#include "Game.h"

const int Menu::MENU_MAIN = 0;
const int Menu::MENU_NEWGAME = 1;
const int Menu::MENU_OPTIONS = 2;
const int Menu::MENU_HELP = 3;

Menu::Menu(int windowWidth,
		   int windowHeight,
		   Game * externalGame,
		   Variables * externalSettings)
			: titleColour(255, 0, 0, 255),
			  inactiveColour(0, 0, 128, 255),
			  activeColour(0, 0, 255, 255),
			  game(externalGame),
			  settings(externalSettings),
			  mainMenuIndex(0),
			  mapMenuIndex(0),
			  optionsMenuIndex(0),
			  menuItemOffset(250),
			  menuItemIncrement(55),
			  menuType(MENU_MAIN),
			  active(true) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	loadMapList(settings->getValue("Map Directory"));

	// initialize menu fonts
	int _x, _y, _s = 200, _yi = 50, _ti = _yi * 2;
	titleFont = new Font(windowWidth, windowHeight, "System", 96, Font::BOLD, false, false, false, 0);
	itemFont = new Font(windowWidth, windowHeight, "System", 24, Font::BOLD, false, false, false, 0);
	char * titleText = (settings->getValue("Game Name") == NULL) ? "3D Game Engine" : settings->getValue("Game Name");

	// create the main menu
	_x = _s, _y = windowHeight - _s;
	mainMenuTitle = new Text(windowWidth, windowHeight, _x, _y, titleColour, titleFont, true, titleText);
	mainMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _ti, activeColour, itemFont, true, "Load World"));
	mainMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _yi, inactiveColour, itemFont, true, "Resume Game"));
	mainMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _yi, inactiveColour, itemFont, true, "Options"));
	mainMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _yi, inactiveColour, itemFont, true, "Help"));
	mainMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _yi, inactiveColour, itemFont, true, "Quit"));

	// create the map loading menu
	_x = _s, _y = windowHeight - _s;
	mapMenuTitle = new Text(windowWidth, windowHeight, _x, _y, titleColour, titleFont, true, "Load World");
	_y -= _ti;
	char * mapName;
	for(unsigned int i=0;i<mapList.size();i++) {
		mapName = strchr((char *) mapList.at(i).c_str(), '\\') + sizeof(char);
		mapMenuItems.push_back(new Text(windowWidth, windowHeight, _x, ((i == 0) ? _y : _y -= _yi), ((i == 0) ? activeColour : inactiveColour), itemFont, true, mapName));
	}
	
	// create the options menu
	_x = _s, _y = windowHeight - _s;
	optionsMenuTitle = new Text(windowWidth, windowHeight, _x, _y, titleColour, titleFont, true, "Options");
	optionsMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _ti, activeColour, itemFont, true, "Enable Culling"));
	
	// create the help menu
	_x = _s, _y = windowHeight - _s;
	helpMenuTitle = new Text(windowWidth, windowHeight, _x, _y, titleColour, titleFont, true, "Help");
	helpMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _ti, inactiveColour, itemFont, true, "WSAD to move around."));
	helpMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _yi, inactiveColour, itemFont, true, "Space to move up."));
	helpMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _yi, inactiveColour, itemFont, true, "Z to move down."));
	helpMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _yi, inactiveColour, itemFont, true, "Escape to exit / go back."));
}

Menu::~Menu() {
	for(unsigned int i=0;i<mainMenuItems.size();i++) {
		delete mainMenuItems.at(i);
	}
	for(unsigned int i=0;i<mapMenuItems.size();i++) {
		delete mapMenuItems.at(i);
	}
	for(unsigned int i=0;i<optionsMenuItems.size();i++) {
		delete optionsMenuItems.at(i);
	}
	for(unsigned int i=0;i<helpMenuItems.size();i++) {
		delete helpMenuItems.at(i);
	}

	delete mainMenuTitle;
	delete mapMenuTitle;
	delete optionsMenuTitle;
	delete helpMenuTitle;
	delete titleFont;
	delete itemFont;
}

void Menu::select() {
	if(!active) { return; }

	// main menu selecting
	if(menuType == MENU_MAIN) {
		if(mainMenuIndex == 0) {
			setMenu(MENU_NEWGAME);
		}
		else if(mainMenuIndex == 1) {
			setMenu(MENU_MAIN);
			resumeGame();
		}
		else if(mainMenuIndex == 2) {
			setMenu(MENU_OPTIONS);
		}
		else if(mainMenuIndex == 3) {
			setMenu(MENU_HELP);
		}
		else if(mainMenuIndex == 4) {
			exit(0);
		}
	}
	// load map menu selecting
	else if(menuType == MENU_NEWGAME) {
		game->closeMap();
		game->loadMap((char *) mapList.at(mapMenuIndex).c_str());
		game->world->cullingEnabled = game->cullingEnabled;
		resumeGame();
	}
	// options menu selecting
	else if(menuType == MENU_OPTIONS) {
		if(optionsMenuIndex == 0) {
			game->cullingEnabled = !game->cullingEnabled;
			game->world->cullingEnabled = game->cullingEnabled;
		}
	}
	// help menu selecting
	else if(menuType == MENU_HELP) {
		setMenu(MENU_MAIN);
	}
}

void Menu::back() {
	if(!active) {
		pauseGame();
	}
	else {
		if(menuType == MENU_MAIN) {
			resumeGame();
		}
		else if(menuType == MENU_NEWGAME || menuType == MENU_OPTIONS || menuType == MENU_HELP) {
			setMenu(MENU_MAIN);
		}
	}
}

void Menu::moveUp() {
	if(!active) { return; }

	// move the menu selection up (update colour on selected/previous object)
	if(menuType == MENU_MAIN) {
		mainMenuItems.at(mainMenuIndex)->setColour(inactiveColour);
		mainMenuIndex--;
		if(mainMenuIndex < 0) { mainMenuIndex = mainMenuItems.size() - 1; }
		mainMenuItems.at(mainMenuIndex)->setColour(activeColour);
	}
	else if(menuType == MENU_NEWGAME) {
		mapMenuItems.at(mapMenuIndex)->setColour(inactiveColour);
		mapMenuIndex--;
		if(mapMenuIndex < 0) { mapMenuIndex = mapMenuItems.size() - 1; }
		mapMenuItems.at(mapMenuIndex)->setColour(activeColour);
	}
	else if(menuType == MENU_OPTIONS) {
		optionsMenuItems.at(optionsMenuIndex)->setColour(inactiveColour);
		optionsMenuIndex--;
		if(optionsMenuIndex < 0) { optionsMenuIndex = optionsMenuItems.size() - 1; }
		optionsMenuItems.at(optionsMenuIndex)->setColour(activeColour);
	}
}

void Menu::moveDown() {
	if(!active) { return; }

	// move the menu selection down (update colour on selected/previous object)
	if(menuType == MENU_MAIN) {
		mainMenuItems.at(mainMenuIndex)->setColour(inactiveColour);
		mainMenuIndex++;
		if(mainMenuIndex >= (int) mainMenuItems.size()) { mainMenuIndex = 0; }
		mainMenuItems.at(mainMenuIndex)->setColour(activeColour);
	}
	else if(menuType == MENU_NEWGAME) {
		mapMenuItems.at(mapMenuIndex)->setColour(inactiveColour);
		mapMenuIndex++;
		if(mapMenuIndex >= (int) mapMenuItems.size()) { mapMenuIndex = 0; }
		mapMenuItems.at(mapMenuIndex)->setColour(activeColour);
	}
	else if(menuType == MENU_OPTIONS) {
		optionsMenuItems.at(optionsMenuIndex)->setColour(inactiveColour);
		optionsMenuIndex++;
		if(optionsMenuIndex >= (int) optionsMenuItems.size()) { optionsMenuIndex = 0; }
		optionsMenuItems.at(optionsMenuIndex)->setColour(activeColour);
	}
}

bool Menu::isActive() { return active; }
void Menu::activate() { active = true; }
void Menu::deactivate() { active = false; }
void Menu::toggle() { active = !active; }

void Menu::draw() {
	if(!active) { return; }
	
	// clear the screen
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	glColor4f(0, 0, 0, 0.5);
	glDisable(GL_DEPTH_TEST);

	glPushIdentity (); //Camera already set up..
		glScaled(5, 5, 5);
		glBindTexture (GL_TEXTURE_2D, NULL);

		glBegin(GL_POLYGON);
		
			//bottom left
			glTexCoord2d(0, 0);
			glVertex3d(-0.5, -0.5, -0.5);

			//bottom right
			glTexCoord2d(0, 0);
			glVertex3d(0.5, -0.5, -0.5);

			//top right
			glTexCoord2d(0, 0);
			glVertex3d(0.5, 0.5, -0.5);

			//top left
			glTexCoord2d(0,0);
			glVertex3d(-0.5, 0.5, -0.5);

		glEnd();
	glPopMatrix();

	glColor4f(1, 1, 1, 1);
	glDisable(GL_BLEND);

	// render the main menu (if appropriate)
	if(menuType == MENU_MAIN) {
		mainMenuTitle->draw();
		for(unsigned int i=0;i<mainMenuItems.size();i++) {
			mainMenuItems.at(i)->draw();
		}
	}
	// render the load map menu (if appropriate)
	else if(menuType == MENU_NEWGAME) {
		mapMenuTitle->draw();
		for(unsigned int i=0;i<mapMenuItems.size();i++) {
			mapMenuItems.at(i)->draw();
		}
	}
	// render the options menu (if appropriate)
	else if(menuType == MENU_OPTIONS) {
		optionsMenuTitle->draw();
		for(unsigned int i=0;i<optionsMenuItems.size();i++) {
			if(i == 0) {
				if(game->cullingEnabled) {
					optionsMenuItems.at(i)->draw("Disable Culling");
				}
				else {
					optionsMenuItems.at(i)->draw("Enable Culling");
				}
			}
			
		}
	}
	// render the help menu (if appropriate)
	else if(menuType == MENU_HELP) {
		helpMenuTitle->draw();
		for(unsigned int i=0;i<helpMenuItems.size();i++) {
			helpMenuItems.at(i)->draw();
		}
	}
}

void Menu::loadMapList(char * mapDirectory) {
	string rootDirectory = mapDirectory;
	string filePath;
	string strPattern;
	string strFileName;
	HANDLE hFile;
	WIN32_FIND_DATA fileInformation;

	strPattern = rootDirectory + "\\*.wrl";

	// find and collect all .wrl files in the specified directory
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

void Menu::setMenu(int type) {
	menuType = type;
}

void Menu::pauseGame() {
	game->pause();
	active = true;
}

void Menu::resumeGame() {
	game->resume();
	if(!game->isPaused()) {
		active = false;
	}
}
