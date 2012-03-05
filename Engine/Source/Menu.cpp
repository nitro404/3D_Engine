#include "Game.h"
#include "Menu.h"

const int Menu::MENU_MAIN = 0;
const int Menu::MENU_NEWGAME = 1;
const int Menu::MENU_OPTIONS = 2;
const int Menu::MENU_HELP = 3;

Menu::Menu() : titleColour(255, 0, 0, 255),
			   inactiveColour(0, 0, 128, 255),
			   activeColour(0, 0, 255, 255),
			   mainMenuIndex(0),
			   mapMenuIndex(0),
			   optionsMenuIndex(0),
			   menuItemOffset(250),
			   menuItemIncrement(55),
			   menuType(MENU_MAIN),
			   active(true) {
	loadMapList();

	// initialize menu fonts
	int x, y, s = 200, yi = 50, ti = yi * 2;
	titleFont = new Font("System", 96, Font::BOLD, false, false, false, 0);
	itemFont = new Font("System", 24, Font::BOLD, false, false, false, 0);
	char * titleText = Game::settings->gameName;

	// create the main menu
	x = s, y = Game::settings->windowHeight - s;
	mainMenuTitle = new Text(x, y, titleColour, titleFont, true, titleText);
	mainMenuItems.push_back(new Text(x, y -= ti, activeColour, itemFont, true, "Load World"));
	mainMenuItems.push_back(new Text(x, y -= yi, inactiveColour, itemFont, true, "Resume Game"));
	mainMenuItems.push_back(new Text(x, y -= yi, inactiveColour, itemFont, true, "Options"));
	mainMenuItems.push_back(new Text(x, y -= yi, inactiveColour, itemFont, true, "Help"));
	mainMenuItems.push_back(new Text(x, y -= yi, inactiveColour, itemFont, true, "Quit"));

	// create the map loading menu
	x = s, y = Game::settings->windowHeight - s;
	mapMenuTitle = new Text(x, y, titleColour, titleFont, true, "Load World");
	y -= ti;
	int lastSlash;
	for(unsigned int i=0;i<mapList.size();i++) {
		lastSlash = -1;
		for(unsigned int j=0;j<mapList[i].size();j++) {
			if(mapList[i][j] == '\\' || mapList[i][j] == '/') {
				lastSlash = j;
			}
		}
		string mapName = mapList[i].substr(lastSlash + 1, mapList[i].size() - 1);
//strchr(mapList.at(i).c_str(), '\\') + sizeof(char);
		mapMenuItems.push_back(new Text(x, ((i == 0) ? y : y -= yi), ((i == 0) ? activeColour : inactiveColour), itemFont, true, mapName.c_str()));
	}
	
	// create the options menu
	x = s, y = Game::settings->windowHeight - s;
	optionsMenuTitle = new Text(x, y, titleColour, titleFont, true, "Options");
	optionsMenuItems.push_back(new Text(x, y -= ti, activeColour, itemFont, true, "Enable Culling"));
	
	// create the help menu
	x = s, y = Game::settings->windowHeight - s;
	helpMenuTitle = new Text(x, y, titleColour, titleFont, true, "Help");
	helpMenuItems.push_back(new Text(x, y -= ti, inactiveColour, itemFont, true, "WSAD to move around."));
	helpMenuItems.push_back(new Text(x, y -= yi, inactiveColour, itemFont, true, "Space / Q to move up."));
	helpMenuItems.push_back(new Text(x, y -= yi, inactiveColour, itemFont, true, "Z / E to move down."));
	helpMenuItems.push_back(new Text(x, y -= yi, inactiveColour, itemFont, true, "Left click to throw cubes."));
	helpMenuItems.push_back(new Text(x, y -= yi, inactiveColour, itemFont, true, "Escape to exit / go back."));
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
		if(mapList.size() == 0) { return; }

		Game::instance->closeMap();
		Game::instance->loadMap((char *) mapList.at(mapMenuIndex).c_str());

		resumeGame();
	}
	// options menu selecting
	else if(menuType == MENU_OPTIONS) {
		if(optionsMenuIndex == 0) {
			Game::instance->cullingEnabled = !Game::instance->cullingEnabled;
			Game::instance->world->cullingEnabled = Game::instance->cullingEnabled;
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
				if(Game::instance->cullingEnabled) {
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

void Menu::loadMapList() {
	string mapDirectory;
	string filePath;
	string strPattern;
	string strFileName;
	HANDLE hFile;
	WIN32_FIND_DATA fileInformation;

	mapDirectory.append(Game::settings->dataDirectoryName);
	int length = strlen(Game::settings->dataDirectoryName);
	if(Game::settings->dataDirectoryName[length-1] != '\\' && Game::settings->dataDirectoryName[length-1] != '/') {
		mapDirectory.append("/");
	}
	mapDirectory.append(Game::settings->mapDirectoryName);
	length = strlen(Game::settings->mapDirectoryName);
	if(Game::settings->mapDirectoryName[length-1] != '\\' && Game::settings->mapDirectoryName[length-1] != '/') {
		mapDirectory.append("/");
	}

	strPattern = mapDirectory + "*.wrl";

	// find and collect all .wrl files in the specified directory
	hFile = ::FindFirstFile(strPattern.c_str(), & fileInformation);
	if(hFile != INVALID_HANDLE_VALUE) {
		do {
			if(fileInformation.cFileName[0] != '.') {
				filePath.erase();
				filePath = mapDirectory + fileInformation.cFileName;

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
	Game::instance->pause();
	active = true;
}

void Menu::resumeGame() {
	Game::instance->resume();
	if(!Game::instance->isPaused()) {
		active = false;
	}
}
