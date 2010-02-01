//*****************************************************************************************//
//                                       Player                                            //
//*****************************************************************************************//

#ifndef playerModule
#define playerModule 

class Player {
public:
	Player () {};
	~Player () {};

	DualTransformation playerMatrix;

	static void setup ();
	static void wrapup ();

	void tick ();
	void draw ();

	void beginCamera ();
	void endCamera ();
	
	void moveBy (Point &translation);
	void rotateBy (Point &rotation);

	void reset (Point &worldPosition);
};

extern Player *player;

#endif //playerModule