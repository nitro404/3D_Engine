//*****************************************************************************************//
//                                       Camera                                            //
//*****************************************************************************************//

#ifndef cameraModule
#define cameraModule 

class Camera {
public:
	Camera () {
		xRotation = 0.0; target = player; offset = Point (0.0, 1.0, 1.0); //Above and behind...
	};
	~Camera () {};

	Player *target; Point offset; double xRotation;

	static void setup ();
	static void wrapup ();

	void tick ();
	void draw ();

	void beginCamera ();
	void endCamera ();
	
	//Movement functions that pass the buck to the target (but be careful with rotation)...
	void moveBy (Point &translation) {
		const bool logging = false;
//		if (logging) {::log ("\nCamera told to translate by "); translation.log ();}
		target->moveBy (translation);
	}

	void rotateBy (Point &rotation) {
		//Accumulate the x-rotation but don't pass it along to the player. Also, discard all z-rotation.
		const bool logging = false;
//		if (logging) {::log ("\nCamera told to rotate by "); rotation.log ();}
		xRotation += rotation.x;
//		if (logging) {
//			::log ("\nX rotation %3.2f recorded to get %3.2f; y rotation %3.2f passed to player.", rotation.x, xRotation, rotation.y);
//			::log ("\nOffset is "); offset.log ();
//		}
		target->rotateBy (Vector (0.0, rotation.y, 0.0));
	}
};

extern Camera *camera;

#endif //cameraModule