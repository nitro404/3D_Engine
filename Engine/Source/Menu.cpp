#include "Menu.h"
#include "Game.h"

Menu::Menu(int windowWidth,
		   int windowHeight,
		   Game * externalGame,
		   Variables * externalSettings)
			: titleColour(255, 0, 0, 255),
			  inactiveColour(0, 0, 128, 255),
			  activeColour(0, 0, 255, 255),
			  game(externalGame),
			  settings(externalSettings),
			  index(0),
			  menuItemOffset(250),
			  menuItemIncrement(55),
			  menuType(0),
			  active(true) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	loadMapList(settings->getValue("Map Directory"));

	int _x, _y, _s = 200, _yi = 50, _ti = _yi * 2;
	titleFont = new Font(windowWidth, windowHeight, "System", 96, Font::BOLD, false, false, false, 0);
	itemFont = new Font(windowWidth, windowHeight, "System", 24, Font::BOLD, false, false, false, 0);
	char * titleText = (settings->getValue("Game Name") == NULL) ? "3D Game Engine" : settings->getValue("Game Name");

	_x = _s, _y = windowHeight - _s;
	mainMenuTitle = new Text(windowWidth, windowHeight, _x, _y, titleColour, titleFont, true, titleText);
	mainMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _ti, activeColour, itemFont, true, "Load World"));
	mainMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _yi, inactiveColour, itemFont, true, "Resume Game"));
	mainMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _yi, inactiveColour, itemFont, true, "Help"));
	mainMenuItems.push_back(new Text(windowWidth, windowHeight, _x, _y -= _yi, inactiveColour, itemFont, true, "Quit"));

	_x = _s, _y = windowHeight - _s;
	mapMenuTitle = new Text(windowWidth, windowHeight, _x, _y, titleColour, titleFont, true, "Load World");
	_y -= _ti;
	char * mapName;
	for(unsigned int i=0;i<mapList.size();i++) {
		mapName = strchr((char *) mapList.at(i).c_str(), '\\') + sizeof(char);
		mapMenuItems.push_back(new Text(windowWidth, windowHeight, _x, ((i == 0) ? _y : _y -= _yi), ((i == 0) ? activeColour : inactiveColour), itemFont, true, mapName));
	}
	
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
	for(unsigned int i=0;i<helpMenuItems.size();i++) {
		delete helpMenuItems.at(i);
	}

	delete mainMenuTitle;
	delete mapMenuTitle;
	delete helpMenuTitle;

	delete titleFont;
	delete itemFont;
}

void Menu::select() {
	if(!active) { return; }

	if(menuType == 0) {
		if(index == 0) {
			setMenu(1);
		}
		else if(index == 1) {
			setMenu(0);
			resumeGame();
		}
		else if(index == 2) {
			setMenu(2);
		}
		else if(index == 3) {
			exit(0);
		}
	}
	else if(menuType == 1) {
		game->closeMap();
		game->loadMap((char *) mapList.at(index).c_str());
		resumeGame();
		setMenu(0);
	}
	else if(menuType == 2) {
		setMenu(0);
	}
}

void Menu::back() {
	if(!active) {
		setMenu(0);
		pauseGame();
	}
	else {
		if(menuType == 0) {
			setMenu(0);
			resumeGame();
		}
		else if(menuType == 1 || menuType == 2) {
			setMenu(0);
		}
	}
}

void Menu::moveUp() {
	if(!active) { return; }

	// move the menu selection up (update colour on selected/previous object)
	if(menuType == 0) {
		mainMenuItems.at(index)->setColour(inactiveColour);
		index--;
		if(index < 0) { index = mainMenuItems.size() - 1; }
		mainMenuItems.at(index)->setColour(activeColour);
	}
	else if(menuType == 1) {
		mapMenuItems.at(index)->setColour(inactiveColour);
		index--;
		if(index < 0) { index = mapMenuItems.size() - 1; }
		mapMenuItems.at(index)->setColour(activeColour);
	}
}

void Menu::moveDown() {
	if(!active) { return; }

	// move the menu selection down (update colour on selected/previous object)
	if(menuType == 0) {
		mainMenuItems.at(index)->setColour(inactiveColour);
		index++;
		if(index >= (int) mainMenuItems.size()) { index = 0; }
		mainMenuItems.at(index)->setColour(activeColour);
	}
	else if(menuType == 1) {
		mapMenuItems.at(index)->setColour(inactiveColour);
		index++;
		if(index >= (int) mapMenuItems.size()) { index = 0; }
		mapMenuItems.at(index)->setColour(activeColour);
	}
}

bool Menu::isActive() { return active; }
void Menu::activate() { active = true; }
void Menu::deactivate() { active = false; }
void Menu::toggle() { active = !active; }

void Menu::draw() {
	if(!active) { return; }
	
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

	if(menuType == 0) {
		mainMenuTitle->draw();
		for(unsigned int i=0;i<mainMenuItems.size();i++) {
			mainMenuItems.at(i)->draw();
		}
	}
	else if(menuType == 1) {
		mapMenuTitle->draw();
		for(unsigned int i=0;i<mapMenuItems.size();i++) {
			mapMenuItems.at(i)->draw();
		}
	}
	else if(menuType == 2) {
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
	index = 0;
	menuType = type;
	vector<Text *> * items;
	if(type == 0) { items = &mainMenuItems; }
	else if(type == 1) { items = &mapMenuItems; }
	else if(type == 2) { menuType = 2; return; }
	else { return; }
	for(unsigned int i=0;i<items->size();i++) {
		items->at(i)->setColour((i == 0) ? activeColour : inactiveColour);
	}
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
