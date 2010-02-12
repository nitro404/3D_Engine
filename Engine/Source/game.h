//*****************************************************************************************//
//                                        Game                                             //
//*****************************************************************************************//

#ifndef _GAME_H
#define _GAME_H

extern double DT; //Elapsed time since previous tick/draw....

class Game {

public:
	Game() {
		world = NULL;
		displayHelp = false;
	}
	
	~Game() {
		if(world != NULL) { delete world; }
	}
	
	void tick();
	void draw();
	
	static void setupFont();
	static void wrapupFont();
	
	void drawMessage(long x, long y, const char * message, ...);
	void drawFrameRate();
	void drawNote(const char * message, ...);

	void import();
	
	static HDC deviceContext;
	static GLuint fontBase; 
	
	World * world;
	bool displayHelp;
private:
	void drawText(const char * message, ...);
	void begin2DDrawing();
	void end2DDrawing(); 
	void privateDrawString(const char * text);
};

extern Game * game;

#endif
