//*****************************************************************************************//
//                                       Camera                                            //
//*****************************************************************************************//

#ifndef cameraModule
#define cameraModule 

class Camera {
public:
	Camera () {};
	~Camera () {};

	DualTransformation cameraMatrix;

	static void setup ();
	static void wrapup ();

	void tick ();
	void draw ();

	void beginCamera ();
	void endCamera ();
	
	//Functions to move forward/back, left/right, up/down; rotate left/right, up/down; lean left/right.
	void moveBy (Point &translation) {cameraMatrix.translateBy (translation);}
	void rotateBy (Point &rotation) {cameraMatrix.rotateBy (rotation);}

	Point position () {return Zero * cameraMatrix;}
};

extern Camera *camera;

#endif //cameraModule