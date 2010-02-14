//*****************************************************************************************//
//                                        Game                                             //
//*****************************************************************************************//

#ifndef _GAME_H
#define _GAME_H

extern double DT; //Elapsed time since previous tick/draw....

class Game {

public:
	Game(Variables * settings) {
		this->settings = settings;
		verifySettings();
		world = NULL;		
		worldFileFilter = "World File (*.wrl)|*.wrl|All Files (*.*)|*.*";
		displayHelp = false;
		loadTextures(settings->getValue("Texture Data File"), settings->getValue("Texture Directory"));
	}
	
	~Game() {
		if(settings != NULL) { delete settings; }
		if(worldFileFilter != NULL) { delete [] worldFileFilter; }
		if(world != NULL) { delete world; }
		deleteAnimatedTextureCollectionEntries(animatedTextures);
		deleteTextureCollectionEntries(textures);
	}
	
	void tick();
	void draw();
	
	static void setupFont();
	static void wrapupFont();
	
	void drawMessage(long x, long y, const char * message, ...);
	void drawFrameRate();
	void drawNote(const char * message, ...);
	
	void verifySettings();
	void loadTextures(char * fileName, char * textureDirectory);
	void import();
	
	static HDC deviceContext;
	static GLuint fontBase; 
	
	World * world;
	bool displayHelp;

	TextureCollection textures;
	AnimatedTextureCollection animatedTextures;
private:
	char * worldFileFilter;
	Variables * settings;

	void drawText(const char * message, ...);
	void begin2DDrawing();
	void end2DDrawing(); 
	void privateDrawString(const char * text);
};

extern Game * game;

#endif
