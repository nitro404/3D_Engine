//*****************************************************************************************//
//                                        Game                                             //
//*****************************************************************************************//

#ifndef gameModule
#define gameModule 

extern double DT; //Elapsed time since previous tick/draw....

class Game {
public:
	Game () {world = NULL; displayHelp = false;};
	~Game () {};

	World *world;
	bool displayHelp;

	static HDC deviceContext; static GLuint fontBase; 

	static void setup ();
	static void wrapup ();

	void tick ();
	void draw ();

	static void setupFont ();
	static void wrapupFont ();

	void drawMessage (long x, long y, const char *message, ...);
	void drawFrameRate ();
	void drawNote (const char *message, ...);

	void drawTeapots ();
	
private:
	void drawText (const char *message, ...);
	void begin2DDrawing(); void end2DDrawing(); 
	void privateDrawString (const char *text);
};


extern Game *game;

#endif //gameModule