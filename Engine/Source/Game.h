#ifndef GAME_H
#define GAME_H

#include "Includes.h"
#include "SettingsManager.h"
#include "Menu.h"
#include "Text.h"
#include "PhysicsManager.h"
#include "Camera.h"
#include "World.h"
#include "Cube.h"

class Game {
public:
	Game();
	~Game();
	
	bool init();
	void update();
	void draw();

	void resume();
	void pause();
	void unpause();
	void togglePause();
	bool isPaused();

	void loadMap(char * fileName);
	void closeMap();

	void toggleFly();

	void throwGrassBlock();
	void clearObjects();
	
private:
	void updateFPS(double timeElapsed);
	void drawFPS();
	void drawPosition();
	void drawFly();
	void loadResources();
	
public:
	static Game * instance;
	static SettingsManager * settings;
	static Menu * menu;
	static Camera * camera;
	static PhysicsManager * physics;
	static World * world;

	vector<Texture *> textures;
	vector<char *> heightMaps;
	vector<AnimatedTexture *> animatedTextures;
	vector<Shader *> shaders;
	vector<Cube *> cubes;

	bool cullingEnabled;

private:
	double currentFPS;
	char * pos;
	char * fps;
	Text * fpsText;

	bool paused;
};

#endif // GAME_H
