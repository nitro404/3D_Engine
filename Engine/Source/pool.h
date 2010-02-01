//*****************************************************************************************//
//                                        Pool                                             //
//*****************************************************************************************//

#ifndef poolModule
#define poolModule 

class Pool : public Object {
public:
	Pool();
	~Pool();

	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures);
	
	void printOn(ostream & o) const;
	
private:
	char * name;
};

declareCollection(Pool);

ostream & operator << (ostream & o, const Pool & x);

#endif //poolModule