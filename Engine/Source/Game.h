#ifndef GAME_H
#define GAME_H

#include "Includes.h"
#include "SettingsManager.h"
#include "Menu.h"
#include "Text.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "InputManager.h"

extern double DT;

class Game {
public:
	Game();
	~Game();
	
	bool init();
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
	void loadTextures();
	
public:
	static Game * instance;
	static SettingsManager * settings;
	static Menu * menu;
	static World * world;

	vector<Texture *> textures;
	vector<char *> heightMaps;
	vector<AnimatedTexture *> animatedTextures;
	vector<Shader *> shaders;

	bool cullingEnabled;

private:
	char * fps;
	Text * fpsText;

	bool paused;
};

#endif // GAME_H
