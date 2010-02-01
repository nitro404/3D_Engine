//*****************************************************************************************//
//                                        Game                                             //
//*****************************************************************************************//

#ifndef gameModule
#define gameModule 

extern double DT; //Elapsed time since previous tick/draw....

class Game {
public:
	Game () {idling = true;};
	~Game () {};

	bool idling;

	static void setup ();
	static void wrapup ();

	void tick ();
	void draw ();

	static Texture *readingTexture; static Texture *idlingTexture;
};


extern Game *game;

#endif //gameModule