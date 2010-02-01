//*****************************************************************************************//
//                                       Camera                                            //
//*****************************************************************************************//

#ifndef cameraModule
#define cameraModule 

class Camera {
public:
	Camera () {
		translateLeft = translateRight = translateAhead = translateBack = translateUp = translateDown = false;
		rotateLeft = rotateRight = rotateUp = rotateDown = false;
		totalXRotation = 0.0;
	};
	~Camera () {};

	DualTransformation cameraMatrix;

	static void setup ();
	static void wrapup ();

	void tick ();
	void draw ();
	void reset () {totalXRotation = 0.0;}

	void pushMatrix ();
	void popMatrix ();
	
	//Functions to move forward/back, left/right, up/down; rotate left/right, up/down; lean left/right.
	void moveBy (Point &translation) {cameraMatrix.translateBy (translation);}
	void rotateBy (Point &rotation) {totalXRotation += rotation.x; cameraMatrix.rotateBy (Vector (0.0, rotation.y, 0.0));}

	Point position () {return Zero * cameraMatrix;}

	bool translateLeft, translateRight, translateAhead, translateBack, translateUp, translateDown;
	bool rotateLeft, rotateRight, rotateUp, rotateDown;

	double totalXRotation;
};

extern Camera *camera;

#endif