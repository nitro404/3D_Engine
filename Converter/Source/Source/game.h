//*****************************************************************************************//
//                                        Game                                             //
//*****************************************************************************************//

#ifndef gameModule
#define gameModule 

extern double DT; //Elapsed time since previous tick/draw....

class Game {
public:
	Game () {idling = true; showing = false; universalObject = NULL;};
	~Game () {delete universalObject;};

	bool idling; bool showing; UniversalObject *universalObject;

	static void setup ();
	static void wrapup ();

	void tick ();
	void draw ();

	static Texture *readingTexture; static Texture *idlingTexture;

	void idle () {
		if (universalObject != NULL) {delete universalObject; universalObject = NULL;}
		showing = false; idling = true; reset ();
	}

	void prompt () {
		if (universalObject != NULL) {delete universalObject; universalObject = NULL;}
		showing = false; idling = false; reset ();
	}

	void show () {
		showing = universalObject != NULL;
		idling = true;
	}

	void reset () {camera->reset ();}
};


extern Game *game;

#endif