//*****************************************************************************************//
//                                        Sprite                                           //
//*****************************************************************************************//

#ifndef spriteModule
#define spriteModule 

class Sprite : public Object {
public:
	Sprite();
	~Sprite();

	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures);
	
	void printOn(ostream & o) const;
	
private:
	
};

declareCollection(Sprite);

ostream & operator << (ostream & o, const Sprite & x);

#endif //spriteModule