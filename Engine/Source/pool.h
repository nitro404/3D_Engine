#ifndef _POOL_H
#define _POOL_H

class Pool : public Object {
private:
	double maxX, maxY, maxZ;
	double minX, minY, minZ;
	char * name;
	double red;
	double green;
	double blue;
	double alpha;
public:
	DualTransformation transformation;
	FaceCollection faces;
	
	Pool() { }

	~Pool() {
		delete [] name;
		deleteFaceCollectionEntries(faces);
	}
	
	double distanceFrom(Point & p) const;
	bool insideOf(Point & p) const;
	
	void tick();
	void draw();

	void import (ifstream &input, AnimatedTextureCollection & animatedTextures);
};

declareCollection(Pool);

#endif