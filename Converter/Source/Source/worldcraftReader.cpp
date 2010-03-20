//95.402 Wilf LaLonde: Student development Worldcraft to Student world converter.
//This program creates world (or level) files for use by the game engine.

//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                            Worldcraft Reader Discussion                                 //
//*****************************************************************************************//

//Worldcraft provides data that consists essentially of planes which define a convex region.
//By constructing these planes and using then to cut each other, we end up computing the values of
//coordinates that can be used to construct the faces. It does not give us the face coordinates.
//
//Worldcraft also provides data that allows use to compute texture coordinates given that
//we have face coordinates. It does not give us texture coordinates.

//To construct, a suitable transformation, we need to understand what Worldcraft had in mind.
//Essentially, they wanted to take their normal coordinates and transform them to the appropriate
//texture coordinate by writing 
//
//					normalCoordinate * M = textureCoordinate.
//
//But Worldcraft texture coordinates are bitmap size DEPENDENT (i.e., coordinates can run from
//0 to the width-1 in the x-direction and 0 to height-1 in the y-direction). By contrast, OpenGL
//is bitmap size INDEPENDENT (i.e., coordinates ranges from 0.0 to 1.0 no matter what the size
//of the bitmap). 
//
//Let N (for NORMALIZE) be the transformation that scales the extent (width and height) of a bitmap 
//down to range 0.0 to 1.0. Then the transformation we need looks like
//
//					normalCoordinate * M * N = textureCoordinate.
//
//Now, in general a transformation consists of three parts: a rotation R, a scale S, and a
//translation T. So the transformation can actually be written as
//
//					normalCoordinate * R * S * T * N = textureCoordinate.
//
//In general, it's important to scale before you translate because scaling moves points. So 
//if you scale first, then translation can move the point from where it happens to be to where
//you want it. If you translate first, you will have to translate it to a spot which gets
//it it to where you really want it to be after you scale it. It can be done this way but it's
//harder to control.
//
//Now for some esoteric knowledge. Whatever you do to normal coordinates, you must do the
//INVERSE to texture coordinates. This could be stated as a theorem but that wouldn't make
//it any clearer. Consider an example instead, a rectangle displaying the left half of a texture.
//
//		Rectangle:										Texture
//
//		   tx@ty=0.0@1.0			tx@ty=0.5@1.0		
//			  .____________________________.        .____________________________.
//			  .                            .        .                            .
//			  .     a       a      a       .        .  a  a  a         b  b  b   .
//			  .                            .        .                            .
//			  .                            .        .                            .
//			  .     a       a      a       .        .  a  a  a         b  b  b   .
//			  .                            .        .                            .
//			  .                            .        .                            .
//			  .     a       a      a       .        .  a  a  a         b  b  b  .
//			  .____________________________.        .____________________________.
//
//		   tx@ty=0.0@0.0			tx@ty=0.5@0.0		
//
//It doesn't matter what the normal coordinates of the rectangle are. But since the texture
//coordinates have a x part (tx) that goes from 0.0 to 0.5, what you will see drawn in the
//rectangle are the left half of the texture; i.e., JUST THE A's (actually twice as big as 
//the original A's but I couldn't draw bigger A's with the keyboard). 	
//
//Now, let's translate the texture coordinates by 0.25 to see what happens. We should get
//the following:
//
//		   tx@ty=0.25@1.0			tx@ty=0.75@1.0		
//			  .____________________________.        ._______|______|_____|_______.
//			  .                            .        .                            .
//			  .     a              b       .        .  a  a  a         b  b  b   .
//			  .                            .        .                            .
//			  .                            .        .                            .
//			  .     a              b       .        .  a  a  a         b  b  b   .
//			  .                            .        .                            .
//			  .                            .        .                            .
//			  .     a              b       .        .  a  a  a         b  b  b  .
//			  .____________________________.        .____________________________.
//
//		   tx@ty=0.25@0.0			tx@ty=0.75@0.0		
//
//Since the x part (tx) of range from 0.25 to 0.75, we see the rightmost column of A's
//and the leftmost column of B's. Now, let just translate the texture coordinates by 0.25
//one more time. After the translation, the texture coordinates are
//
//		   tx@ty=0.5@1.0			tx@ty=1.0@1.0		
//			  .____________________________.        .____________________________.
//			  .                            .        .                            .
//			  .     b       b      b       .        .  a  a  a         b  b  b   .
//			  .                            .        .                            .
//			  .                            .        .                            .
//			  .     b       b      b       .        .  a  a  a         b  b  b   .
//			  .                            .        .                            .
//			  .                            .        .                            .
//			  .     b       b      b       .        .  a  a  a         b  b  b  .
//			  .____________________________.        .____________________________.
//
//		   tx@ty=0.5@0.0			tx@ty=1.0@0.0	
//
//What you see first are the A's, then half A's and half B's, then just B's. You see
//the texture MOVING LEFT. THEOREM: TRANSLATING TEXTURE COORDINATE RIGHT CAUSES THE
//TEXTURE TO MOVE LEFT.
//
//I'll show one more example, this time in the context of scaling. Consider
//
//
//		   tx@ty=0.0@0.33			tx@ty=0.33@0.33		      
//			  .____________________________.        .____________________________.
//			  .                            .        .                            .
//			  .                            .        .  C   C   C    C            .
//			  .                            .        .                            .
//			  .                            .        .                            .
//			  .      A           A         .        .  B   B   B    C            .
//			  .                            .        .                            .
//			  .                            .        .                            .
//			  .                            .        .  A   A   B    C            .
//			  .____________________________.        .____________________________.
//
//		   tx@ty=0.0@0.0			tx@ty=0.33@0.0		
//
//With those texture coordinate, all you see are large A's. Let's scale the texture by 2.0. 
//Now you should see
//
//		   tx@ty=0.0@0.66			tx@ty=0.66@0.66		      
//			  .____________________________.        .____________________________.
//			  .                            .        .                            .
//			  .                            .        .  C   C   C    C            .
//			  .      B       B      B      .        .                            .
//			  .                            .        .                            .
//			  .                            .        .  B   B   B    C            .
//			  .                            .        .                            .
//			  .      A       A      B      .        .                            .
//			  .                            .        .  A   A   B    C            .
//			  .____________________________.        .____________________________.
//
//		   tx@ty=0.0@0.0			tx@ty=0.66@0.0		
//
//Scaling by 1.5 again gives us
//
//		   tx@ty=0.0@0.99			tx@ty=0.99@0.99		      
//			  .____________________________.        .____________________________.
//			  .                            .        .                            .
//			  .  C   C   C    C            .        .  C   C   C    C            .
//			  .                          .        .                            .
//			  .                            .        .                            .
//			  .  B   B   B    C            .        .  B   B   B    C            .
//			  .                            .        .                            .
//			  .                            .        .                            .
//			  .  A   A   B    C            .        .  A   A   B    C            .
//			  .____________________________.        .____________________________.
//
//		   tx@ty=0.0@0.0			tx@ty=0.99@0.0		
//
//What do you see visually? You see big A's, which shrink down until you see A' with B's, which
//shrink down further until you see A's with B's with C'. THEOREM: SCALING UP CAUSES THE
//TEXTURE TO SCALE DOWN.
//
//The moral of this story should be clear. Since the Worldcraft transformation R * S * T * N
//is actually being applied to textures, the inverse is needed. More specifically,
//
//					(R * S * T * N)-1				Notation: let M-1 means M inverse
//
//There is a well known theorem that I could prove that states "THE INVERSE OF A PRODUCT
//IS THE PRODUCT OF THE INVERSES WITH THE ELEMENT ORDER SWITCHED); i.e., the inverse of the
//above is 
//
//					N-1 * T-1 * S -1 * R-1
//
//I tried this transformation and it DID NOT WORK; i.e., what you saw when I rendered
//a polygon what NOT what you say in Worldcraft. So what I had to do is guess what 
//Worldcraft might be doing and do the same thing. Here's my guess. 
//
//I believe the Worldcraft implementers were not aware how inverses worked. They tried
//to use the original transformation; i.e.,
//	
//					R * S * T * N
//
//But this didn't work. When they rotated right in their editor, the texture rotated left
//(so they negated the sign of the rotation), when they translated right, the texture went
//left (so they negated the sign of the translation), when they scaled up, the texture
//shrunk (so they replaced the scale factor by 1/scale). Each of these fudges is replacing
//the original desire by the inverse. So they ended up with the following transformation:
//	
//					R-1 * S-1 * T-1 * N-1
//
//This means that what you see in Worldcraft is the result of applying this transformation
//to the normal textures. If we want to see the same thing, we need to do it this way too.

//Now Worldcraft doesn't provide one matrix or even the individual transformation matrices 
//directly (that would be too easy). Instead, they provide a number of independent pieces 
//that can be used to construct this. In particular, they provide 2 of the axes for the 
//rotation matrix (the new xAxis and new yAxis; we can compute the missing third zAxis). 
//We also have to do some work to construct a rotation matrix out of axis information.
//They also provide the scale and translation information as corresponding x- and y-components.
//To compute N, we also need to know the extent of the texture even though we don't
//need to render it in the reader. That means we have to read the file just to determine
//the extent.

//One final thing. Worldcraft uses it's own units; there are 8 worldcraft units to a foot.
//We would like our engine to use 1 unit to means something real; how about 1 meter?
//So we apply this final conversion worldcraftToMeters) to get that.

//*****************************************************************************************//
//                              Worldcraft Reader Terminology                              //
//*****************************************************************************************//

//The reader reads in raw worldcraft data that describes objects (brushes they call them) in 
//terms of planes (that we'll call a worldcraft side plane since it is not at all what a 
//standard plane looks like). The reader reads all this information in, converts the side 
//plane data into coordinates for faces and returns a vector of worldcraft objects (a map 
//that contains  everything worldcraft provided as key/value pairs (both keys and values are
//strings) except for one additional key:

//*****************************************************************************************//
//                      Utilities That Could Have Been Placed in Plane                     //
//*****************************************************************************************//

//But we didn't because there are so specialized that no one else would use them.

//Forward references...
bool isCounterClockwise (Plane &plane, Point &point1, Point &point2, Point &point3);
PointCollection *anyFarCornerPoints (Plane &plane);
PointCollection *clip (Plane &plane, PointCollection *candidatePoints);
Point whereIsIntersectionOfPlaneCrossingLine (Plane &plane, Point &fromPoint, Point &toPoint);
Point binarySearchForCrossingPoint (Plane &plane, Point &positivePoint, Point &negativePoint);

bool isCounterClockwise (Plane &plane, Point &point1, Point &point2, Point &point3) {
	//Are these three points on the plane in counter-clockwise order WITH RESPECT TO THE POSITIVE SIDE OF THIS PLANE?

	//Construct a temporary plane and see if the normal is pointing in the same direction.
	//If no such normal can be constructed, return false; (true would also be just as valid).
	Vector unnormalizedNormal; double squaredLength;
	if (!plane.normalIsValid (point1, point2, point3, unnormalizedNormal, squaredLength)) return false;
	Vector normalizedNormal = unnormalizedNormal * (1.0 / sqrt (squaredLength));
	return (normalizedNormal - plane.normal).squaredLength () < epsilon (); //i.e., new normal == plane.normal
}

PointCollection *anyFarCornerPoints (Plane &plane) {
    //Compute 4 points that form a rectangle in the plane ensuring that they are provided in 
	//counter-clockwise order.
    //	Approach: If we find which coordinate of the normal is maximal ignoring sign (call it r which is 
	//	either x, y, or z), then the other two can be set to maximal values and then we can
	//	solve for r to determine where on the plane it lies.

	//The equation of the plane is aX+bY+cZ+d = 0 where a=normal.x = x (for short), 
	//b=normal.y = y (for short), c=normal.z = z (for short), d = minusP0Dotnormal.
	//					i.e., the equation is xX+yY+zZ+d = 0.
	//When you solve for X, for example, you get X = (-d-yY-zZ)/x
	//Since every solution has -d in it, it will be convenient to negate d once and for all
	//so that the solution X is written (d-yY-zZ)/x = (d-Yy-Zz)/x.
	//Plugging in values like Y=p, z=m gives (d-py-mz)/x
    double x = plane.normal.x; double y = plane.normal.y; double z = plane.normal.z;
    double d = -plane.minusP0DotNormal; //To avoid using -d everywhere below.
    double p = 1.0e10; double m = -p; //p=plus and m=minus

	//Use short forms p1,p2,p3,p4 to shorten the code below.
	Point *p1 = new Point; Point *p2 = new Point; Point *p3 = new Point; Point *p4 = new Point; 

    if (absolute (x) > absolute (y)) {
		if (absolute (x) > absolute (z))  {
			//Solve for x given y,z components which are {p,p}, {p,m}, {m,m}, {m,p}.
			Point point1 ((d - (p * y) - (p * z)) / x, p, p); //e.g., (d-py-mz)/x from above.
			Point point2 ((d - (p * y) - (m * z)) / x, p, m);
			Point point3 ((d - (m * y) - (m * z)) / x, m, m);
			Point point4 ((d - (m * y) - (p * z)) / x, m, p);
			*p1 = point1; *p2 = point2; *p3 = point3; *p4 = point4;
		} else {
			//Solve for z given x,y components which are {p,p}, {p,m}, {m,m}, {m,p}.
			Point point1 (p, p, (d - (p * x) - (p * y)) / z);
			Point point2 (p, m, (d - (p * x) - (m * y)) / z);
			Point point3 (m, m, (d - (m * x) - (m * y)) / z);
			Point point4 (m, p, (d - (m * x) - (p * y)) / z);
			*p1 = point1; *p2 = point2; *p3 = point3; *p4 = point4;
		}
	} else {
		if (absolute (y) > absolute (z)) {
			//Solve for y given x,z components which are {p,p}, {p,m}, {m,m}, {m,p}.
			Point point1 (p, (d - (p * x) - (p * z)) / y, p);
			Point point2 (p, (d - (p * x) - (m * z)) / y, m);
			Point point3 (m, (d - (m * x) - (m * z)) / y, m);
			Point point4 (m, (d - (m * x) - (p * z)) / y, p);
			*p1 = point1; *p2 = point2; *p3 = point3; *p4 = point4;
		} else {
			//Solve for z given x,y components which are {p,p}, {p,m}, {m,m}, {m,p}.
			Point point1 (p, p, (d - (p * x) - (p * y)) / z);
			Point point2 (p, m, (d - (p * x) - (m * y)) / z);
			Point point3 (m, m, (d - (m * x) - (m * y)) / z);
			Point point4 (m, p, (d - (m * x) - (p * y)) / z);
			*p1 = point1; *p2 = point2; *p3 = point3; *p4 = point4;
		}
	}
	
	PointCollection *points = new PointCollection;
	if (isCounterClockwise (plane, *p1, *p2, *p3)) {
		//Already counter-clockwise.
		points->push_back (p1); points->push_back (p2);
		points->push_back (p3); points->push_back (p4);
	} else {
		//Must reverse the order to get counter-clockwise.
		points->push_back (p4); points->push_back (p3);
		points->push_back (p2); points->push_back (p1);
	}
	return points;
}

PointCollection *clip (Plane &plane, PointCollection *candidatePoints) {
    //Returns clipped points; i.e., those on the positive or zero side (also eliminates zero length edges).
    if (candidatePoints->empty ()) return new PointCollection; //Nothing to clip.

    //Keep what remains after clipping.
    PointCollection *points = new PointCollection;
    for (long index = 0; index < candidatePoints->size (); index++) {
        Point *point1 = (*candidatePoints) [index];
		//Let point2 be the next point and wraparound to the beginning if at the end.
        Point *point2 = (*candidatePoints) [index == candidatePoints->size () - 1 ? 0 : index + 1]; 
        LineSign lineSign = plane.whereIsLine (*point1, *point2);

		switch (plane.whereIsLine (*point1, *point2)) {
			case frontSign: 
				points->push_back (point1->newCopy ()); 
				break;
			case backSign: 
				//Discard. 
				break;
			case onSign: 
				points->push_back (point1->newCopy ()); 
				break;
			case straddlesSign:
				PointSign point1Sign = plane.whereIsPoint (*point1);
				PointSign point2Sign = plane.whereIsPoint (*point2);

				if (point1Sign == positiveSign || point1Sign == zeroSign) {
					points->push_back (point1->newCopy ()); 
				}

				if (point1Sign == positiveSign && point2Sign == negativeSign) {
					Point intersection = whereIsIntersectionOfPlaneCrossingLine (plane, *point1, *point2);
					points->push_back (intersection.newCopy ()); 
				} else if (point1Sign == negativeSign && point2Sign == positiveSign) {
					Point intersection = whereIsIntersectionOfPlaneCrossingLine (plane, *point2, *point1);
					points->push_back (intersection.newCopy ()); 
				}
		}
	}

    //Remove duplicates and useless points (less than 3 points means it can't be used to represent a face).
    PointCollection *trimmedPoints = new PointCollection;
    for (index = 0; index < points->size (); index++) {
        Point *previousPoint = (*points) [index == 0 ? points->size () - 1 : index - 1]; //wrap to right end
        Point *point = (*points) [index];
		if (point->squaredDistanceTo (*previousPoint) < squaredEpsilon ())
			; //Discard 
		else trimmedPoints->push_back (point->newCopy ());
	}

	deletePointCollection (points);
	if (trimmedPoints->size () >= 3) return trimmedPoints;
	deletePointCollection (trimmedPoints); //deletes points too...
	return new PointCollection;
}

Point whereIsIntersectionOfPlaneCrossingLine (Plane &plane, Point &fromPoint, Point &toPoint) {
	//Note: this is a special intersection method that REQUIRES the points to cross the plane;
	//i.e., we insist that one point be on the positive side and one be on the negative side.
	Vector direction = toPoint - fromPoint; direction.normalize ();
	if (plane.normalNearlyPerpendicular (direction)) {
		//Raw math doesn't work in such a situation.
		if (plane.whereIsPoint (fromPoint) == positiveSign) {
			if (plane.whereIsPoint (toPoint) != negativeSign) {halt ("\nPoints should cross the plane");}
			return binarySearchForCrossingPoint (plane, fromPoint, toPoint);
		} else {
			if (plane.whereIsPoint (toPoint) != positiveSign) {halt ("\nPoints should cross the plane");}
			return binarySearchForCrossingPoint (plane, toPoint, fromPoint);
		} 
	} else {
		//Use conventional math. More specifically, if N.p + d = 0 is the equation of the plane and
		//p = p0 + D*t for any t, then N.p0 + N.Dt + d = 0 and t = (-d - N.p0)/N.D. Solve for p.
		//Notice that 'aPlane distanceToPoint: p0 = N.p0 + d. So 
		//		t = -(aPlane distanceToPoint: p0) / N.D."
		double t = -plane.distanceToPoint (fromPoint) / (plane.normal.dot (direction));
		return fromPoint + direction * t;
	}
}

Point binarySearchForCrossingPoint (Plane &plane, Point &positivePoint, Point &negativePoint) {
    Point average = (positivePoint + negativePoint) * 0.5;
    PointSign averageSign = plane.whereIsPoint (average);
    if (averageSign == zeroSign) return average;
    if (averageSign == positiveSign)
        return binarySearchForCrossingPoint (plane, average, negativePoint);
    else return binarySearchForCrossingPoint (plane, positivePoint, average);
}


//*****************************************************************************************//
//                                   WorldcraftSidePlane                                   //
//*****************************************************************************************//

//The worldcraft side plane contains worldcraft data both as it came in and as computed.

struct WorldcraftSidePlane {
	Point point1; Point point2; Point point3;
	char *textureName;
	Point xAxis; double xAxisOffset; double xAxisScale;
	Point yAxis; double yAxisOffset; double yAxisScale;
	double rotation;
	//Computed from above.
	Plane plane; bool planeIsDegenerate;
	Transformation transformation;
};

declareCollection (WorldcraftSidePlane);

//*****************************************************************************************//
//                                    Forward References                                   //
//*****************************************************************************************//

WorldcraftGroupCollection *worldcraftReader (char *filename);
WorldcraftGroup *readRestOfWorldcraftGroup ();
char *readRestOfDoubleQuotedString ();
WorldcraftSidePlaneCollection *readRestOfSidePlanes ();
WorldcraftSidePlane *readRestOfSidePlane ();
char *readTextureName ();
FaceCollection *convertToFaces (WorldcraftSidePlaneCollection *sidePlanes);

//*****************************************************************************************//
//                            Worldcraft Object Implementation                             //
//*****************************************************************************************//

//The following routines extract a property from a property dictionary. Property keys and
//property values are strings. If a property is missing, NULL is returned in which case
//the default is used instead.

Point WorldcraftObject::asPointProperty (char *propertyName, char *propertyDefault) {
	char *propertyValue = (char *) get (properties, propertyName);
	if (propertyValue == NULL) propertyValue = propertyDefault;
	Point answer;
	sscanf (propertyValue, "%lf %lf %lf", &answer.x, &answer.y, &answer.z);
	return answer;
}

char *WorldcraftObject::asStringProperty (char *propertyName, char *propertyDefault) {
	//CAREFUL: User must ensure that the resulting string is deleted by the object that is 
	//to contain it...
	char *propertyValue = (char *) get (properties, propertyName);
	if (propertyValue == NULL) propertyValue = propertyDefault;
	char *answer = new char [strlen (propertyValue) + 1];
	strcpy (answer, propertyValue);
	return answer;
}

long WorldcraftObject::asIntegerProperty (char *propertyName, char *propertyDefault) {
	char *propertyValue = (char *) get (properties, propertyName);
	if (propertyValue == NULL) propertyValue = propertyDefault;
	long answer;
	sscanf (propertyValue, "%d", &answer);
	return answer;
}

double WorldcraftObject::asDoubleProperty (char *propertyName, char *propertyDefault) {
	char *propertyValue = (char *) get (properties, propertyName);
	if (propertyValue == NULL) propertyValue = propertyDefault;
	double answer;
	sscanf (propertyValue, "%lf", &answer);
	return answer;
}

void WorldcraftObject::log () {
	//For debugging, it might be useful to output the worldcraft object into the log file.

	//The type...
	::log ("\nWorldcraft object\n\tType %s", type == NULL ? "NONE SO FAR" : type);
	
	//The properties...
	::log ("\n\tProperties:");
	if (properties == NULL) {
		::log (" NONE SO FAR"); 
	} else {
		loopMap (next, *properties, WorldcraftGroup) //A group is a map
			const char *key = (*next).first.c_str (); void *value = (*next).second;
			if (stricmp (key, "objects") == 0) continue; //Log everything but the "objects" property that is supposed to be ignored.
			::log ("\n\t\t\"%s\" => \"%s\"", key, value);
		endloop
	}

	//The faces...
	if (faces == NULL) {::log ("\n\tFaces: NONE SO FAR"); return;}
	loopVector (faceIndex, *faces)
		Face &face = *((*faces) [faceIndex]);
		::log ("\n\tFace %d:", faceIndex);
		::log ("\n\t\tTexture \"%s\":", face.textureName);
		GamePointCollection &points = face.points;
		loopVector (pointIndex, points)
			GamePoint &point = *(points [pointIndex]);
			::log ("\n\t\t%d: point [%3.2f,%3.2f,%3.2f] normal: [%3.2f,%3.2f,%3.2f] texture: [%3.2f,%3.2f].",
				pointIndex, point.x, point.y, point.z, point.nx, point.ny, point.nz, point.tx, point.ty);
			//Access point's x, y, z, nx, ny, nz, tx, ty
		endloop
	endloop
	
	/*
	//The objects: A version WITHOUT loop macros.
		//The type...
		::log ("\nObject type %s", type);
		
		//The properties...
		::log ("\n\tProperties:");
		for (WorldcraftGroupIterator next = properties.begin (); next != properties.end (); ++next) {
			char *key = next->first.c_str (); void *value = next->second;
			if (stricmp (key, "objects") != 0) {
				//Log everything but the "objects" property that is supposed to be ignored.
				::log ("\n\t\t%s => %s", key, value);
			}
		}

		//The faces...
		for (long faceIndex = 0; faceIndex < faces.size (); faceIndex++) {
			Face &face = *((*faces) [faceIndex]);
			::log ("\n\tFace %d:", faceIndex);
			::log ("\n\t\tTexture \"%s\":", face.textureName);
			GamePointCollection &points = *(face.points);
			for (long pointIndex = 0; pointIndex < points.size (); pointIndex++) {
				GamePoint &point = *(points [pointIndex]);
				::log ("\n\t\t%d: point [%3.2f,%3.2f,%3.2f] normal: [%3.2f,%3.2f,%3.2f] texture: [%3.2f,%3.2f].",
					point.x, point.y, point.z, point.nx, point.ny, point.nz, point.tx, point.ty);
				//Access point's x, y, z, nx, ny, nz, tx, ty
			}
		}
	}
	*/
}

//*****************************************************************************************//
//                            Worldcraft Reader Implementation                             //
//*****************************************************************************************//

#define add(key,value) insert (WorldcraftGroup::value_type (key, value)) //easier to read

static ::ifstream file; 
static char mapDirectoryName [500];
static long mapCounter = 0; //For debugging only.

WorldcraftGroupCollection *worldcraftReader (char *filename) {
	//::log ("\nRead worldcraft file \"%s\".", filename); //Remove if you wish (it's just for debugging).
	file.open (filename, ios::in); 
	if (file.fail ()) {halt ("\nFile \"%s\" could not be opened.", filename);}
	//::log (" SUCCESSFUL"); //Remove if you wish (it's just for debugging).

	//Get the directory name (No longer used but left in just in case)...
	char drive [20], directory [500], name [100], extension [20];
	_splitpath (filename, drive, directory, name, extension);
	strcpy ((char *) &mapDirectoryName [0], (char *) &drive [0]);
	strcat ((char *) &mapDirectoryName [0], (char *) &directory [0]);

	//Do the real work...
	WorldcraftGroupCollection *objects = new WorldcraftGroupCollection;
	mapCounter = 0; //For debugging only.
	char next;
	while (file >> next) {//>> skips whitespace
		if (next == '{') 
			objects->push_back (readRestOfWorldcraftGroup ());
		else quit ("Unexpected input \"%c\" in worldcraft file.", next);
	}
	file.close ();
	return objects;
}

WorldcraftGroup *readRestOfWorldcraftGroup () {
    //Reads and creates a worldcraft object; assumes leading '{' already discarded.

	//Create an object and start to fill it in.
    WorldcraftGroup *group = new WorldcraftGroup; mapCounter++;
	WorldcraftObjectCollection *objects = new WorldcraftObjectCollection;
	group->add ("objects", objects);

    //Read what is to become contents for the group...
	char next;
	while (file >> next) {//>> skips whitespace
		switch (next) {
			case '"': {
				char *name = readRestOfDoubleQuotedString ();
				void convertToLowercase (char *name);
				convertToLowercase (name);
				if (!(file >> next)) quit ("Unexpected error after reading a double quote.");
				if (next != '"') quit ("Expected another string after \"%s\".", name);
				void *value = readRestOfDoubleQuotedString ();
				group->add (name, value);
				//We no longer need to keep the "char *" data (it's a c string) which 
				//was copied by the map into a key of type "string" which actually
				//holds a copy. If that were NOT so, the following delete would wreck things.
				delete [] name;
				//Note: can't delete value since it is NOT of type "string".
				break;}
			case '{': {
				WorldcraftSidePlaneCollection *sidePlanes = readRestOfSidePlanes ();
				WorldcraftObject *object = new WorldcraftObject (convertToFaces (sidePlanes));
				objects->push_back (object);
				break;}
			case '}': {
				return group;}
			default:
				quit ("Unexpected input while parsing a worldcraft group.");
		}
	}
    quit ("Unexpected end of file without finding \"}\".");
}

char *readRestOfDoubleQuotedString () {
	//Does NOT treat the escape character specially since it is in many worldcraft strings as a
	//file delimiter. No internal double quotes are possible since it stops reading on first encounter.
	char buffer [1000]; char *start = &buffer [0]; char *next = start - 1; //Should be plenty for worldcraft.

	*++next = file.get ();
	while (!file.eof ()) {
		switch (*next) {
			case '"': {
				//End of string. All done...
				*next = '\0'; //end of string character (added where get failed).
				char *space = new char [next - start + 1]; 
				strcpy (space, start);
				return space;}
			default:
				//Already stored in buffer, carry on...
				*++next = file.get ();
		}
	}
	*next = '\0'; //end of string character (added where get failed).
	quit ("Unexpected end of file while reading double quoted string \"%s\".", start);
}

WorldcraftSidePlaneCollection *readRestOfSidePlanes () {
    //Reads a bunch of worldcraft sidePlane packets.

	WorldcraftSidePlaneCollection *sidePlanes = new WorldcraftSidePlaneCollection;
	char next;
	while (file >> next) {//>> skips whitespace
		switch (next) {
			case '(':
				sidePlanes->push_back (readRestOfSidePlane ());
				break;
			case '}':
				return sidePlanes;
			default:
				quit ("Unexpected input while parsing a worldcraft brush's side plane; i.e., (...");
		}
	}
    quit ("Unexpected end of file without finding \")\" to match \"(\".");
}

char *doubleProblem = "Unexpected error while reading brush's side plane packed of form (..) (..) (..) ...";
char *leftParenthesisProblem = "Expected \"(\".";
char *rightParenthesisProblem = "Expected \")\".";
char *leftSquareBracketProblem = "Expected \"[\".";
char *rightSquareBracketProblem = "Expected \"]\".";

WorldcraftSidePlane *readRestOfSidePlane () {
    //Reads a side plane packet; 
	//	e.g. ( 176 -384 400 ) ( 176 -404 400 ) ( 429 -404 400 ) BRICK1 [ 1 0 0 -1 ] [ 0 -1 0 -2 ] 0 0.3 0.3 
	//where the first parenthesis has already been removed.

	WorldcraftSidePlane *sidePlane = new WorldcraftSidePlane;

	//Pick up point1.
	char next;
	if (!(file >> sidePlane->point1.x)) quit (doubleProblem);
	if (!(file >> sidePlane->point1.y)) quit (doubleProblem);
	if (!(file >> sidePlane->point1.z)) quit (doubleProblem);
	if (!(file >> next) || next != ')') quit (rightParenthesisProblem);

	//Pick up point2.
	if (!(file >> next) || next != '(') quit (leftParenthesisProblem);
	if (!(file >> sidePlane->point2.x)) quit (doubleProblem);
	if (!(file >> sidePlane->point2.y)) quit (doubleProblem);
	if (!(file >> sidePlane->point2.z)) quit (doubleProblem);
	if (!(file >> next) || next != ')') quit (rightParenthesisProblem);

	//Pick up point3.
	if (!(file >> next) || next != '(') quit (leftParenthesisProblem);
	if (!(file >> sidePlane->point3.x)) quit (doubleProblem);
	if (!(file >> sidePlane->point3.y)) quit (doubleProblem);
	if (!(file >> sidePlane->point3.z)) quit (doubleProblem);
	if (!(file >> next) || next != ')') quit (rightParenthesisProblem);

	//Pick up the texture name.
	sidePlane->textureName = readTextureName ();

	//Pick up the x-axis.
	if (!(file >> next) || next != '[') quit (leftSquareBracketProblem);
	if (!(file >> sidePlane->xAxis.x)) quit (doubleProblem);
	if (!(file >> sidePlane->xAxis.y)) quit (doubleProblem);
	if (!(file >> sidePlane->xAxis.z)) quit (doubleProblem);
	if (!(file >> sidePlane->xAxisOffset)) quit (doubleProblem);
	if (!(file >> next) || next != ']') quit (rightSquareBracketProblem);

	//Pick up the y-axis.
	if (!(file >> next) || next != '[') quit (leftSquareBracketProblem);
	if (!(file >> sidePlane->yAxis.x)) quit (doubleProblem);
	if (!(file >> sidePlane->yAxis.y)) quit (doubleProblem);
	if (!(file >> sidePlane->yAxis.z)) quit (doubleProblem);
	if (!(file >> sidePlane->yAxisOffset)) quit (doubleProblem);
	if (!(file >> next) || next != ']') quit (rightSquareBracketProblem);

    //Pick up the rotation"
	if (!(file >> sidePlane->rotation)) quit (doubleProblem);

    //Pick up the x axis scale"
	if (!(file >> sidePlane->xAxisScale)) quit (doubleProblem);

    //Pick up the y axis scale"
	if (!(file >> sidePlane->yAxisScale)) quit (doubleProblem);

	//All done.
	return sidePlane;
}

char *readTextureName () {
	//Does NOT treat the escape character specially since it is in many worldcraft strings as a
	//file delimiter. No internal double quotes are possible since it stops reading on first encounter.
	char buffer [1000]; char *start = &buffer [0]; char *next = start - 1; //Should be plenty for worldcraft.

	char nextCharacter;
	if (!(file >> nextCharacter)) quit ("End of file while reading texture name.");
	*++next = nextCharacter;
	if (nextCharacter == '"') return readRestOfDoubleQuotedString ();
	while (!file.eof ()) {
		switch (*next) {
			case ' ':
			case '\t':
			case '\n':
			case '\f': {
				//End of string. All done...
				*next = '\0'; //end of string character (added where get failed).
				char *space = new char [next - start + 1]; 
				strcpy (space, start);
				return space;}
			default:
				//Already stored in buffer, carry on...
				*++next = file.get ();
		}
	}
	*next = '\0'; //end of string character (added where get failed).
	quit ("Unexpected end of file while reading texture name \"%s\".", start);
}

PointCollection *cutUpBy (PointCollection *points, WorldcraftSidePlaneCollection *sidePlanes, long indexOfExcludedPlane) {
	PointCollection *oldPoints = points; //Don't delete points.
	for (long planeIndex = 0; planeIndex < sidePlanes->size (); planeIndex++) {
		if (planeIndex != indexOfExcludedPlane) {
			WorldcraftSidePlane *sidePlane = (*sidePlanes) [planeIndex];
			if (sidePlane->planeIsDegenerate) continue;
			Plane &plane = sidePlane->plane;
			Plane flippedPlane = Plane (-plane.normal, -plane.minusP0DotNormal);
//printf("(%f, %f, %f) : %f\n", flippedPlane.normal.x, flippedPlane.normal.y, flippedPlane.normal.z, flippedPlane.minusP0DotNormal);
			PointCollection *newPoints = clip (flippedPlane, oldPoints);
			if (oldPoints != points) deletePointCollection (oldPoints);
			oldPoints = newPoints;
		}
	}
	return oldPoints;
}

PointCollection *enlarge (PointCollection *points, Plane &plane, double enlargementFactor) {
	//Find the center (just the average).
	Point sum; sum.zero ();
	for (long pointIndex = 0; pointIndex < points->size (); pointIndex++) {
		sum += *((*points) [pointIndex]);
	}
	Point center = sum / (double) points->size ();

	//Move each point out from the center by the enlargement factor and projecting onto the plane
	//to make sure it's right on the plane (to make sure numerical inaccuracies don't creep in).
	PointCollection *newPoints = new PointCollection;
	for (pointIndex = 0; pointIndex < points->size (); pointIndex++) {
		Point *point = (*points) [pointIndex];
		Point outwardMovedPoint = plane.projectionOfPoint (center + (*point - center) * enlargementFactor);
		newPoints->push_back (outwardMovedPoint.newCopy ());
//printf("%f, %f, %f)\n", outwardMovedPoint.x, outwardMovedPoint.y, outwardMovedPoint.z);
	}
	return newPoints;
}

PointCollection *enlargeViaTranslating (PointCollection *points, Plane &plane, double enlargementTranslation) {
	//Find the center (just the average).
	Point sum; sum.zero ();
	for (long pointIndex = 0; pointIndex < points->size (); pointIndex++) {
		sum += *((*points) [pointIndex]);
	}
	Point center = sum / (double) points->size ();

	//Move each point out from the center by the enlargement factor and projecting onto the plane
	//to make sure it's right on the plane (to make sure numerical inaccuracies don't creep in).
	PointCollection *newPoints = new PointCollection;
	for (pointIndex = 0; pointIndex < points->size (); pointIndex++) {
		Point *point = (*points) [pointIndex];
		Point outwardMovedPoint = plane.projectionOfPoint (*point + (*point - center).normalized () * enlargementTranslation);
		newPoints->push_back (outwardMovedPoint.newCopy ());
	}
	return newPoints;
}

PointCollection *enlarge (PointCollection *points, Plane &plane) {
	return enlarge (points, plane, 2.0);
}

double inline worldcraftToMeters () {
	//A worldcraft unit is an inch...
    //Since 0.0254 m/in * 12in/ft = 0.3048 m/ft / 12wcu/ft = 0.0254 m/wcu (worldcraft units),
    //multiply by 0.0254 to convert to meters."
    return 0.0254; //meters per inch (or meters per worldcraft unit).
}

bool getTextureExtent (char *textureName, char *&pathName, long &textureWidth, long &textureHeight, char *suffix) {
	//Suffix is either ".dds", ".tga", ".bmp", or "" (in this case, a prompter is invoked)...
	//If successful, pathName is returned (it will be valid until the next prompt or getTextureExtent query)...
	const bool loggingSearch = false;
	if (*suffix == '\0') {
		char *fileName = promptForTexture (textureName); pathName = fileName;
		if (fileName == NULL) return false;
		return Texture::readTextureExtent (pathName, textureWidth, textureHeight);
	} else {
		extern char **textureSearchDirectories; extern long textureSearchDirectoriesSize;
		static char fileName [500]; pathName = fileName;		
		for (long directoryIndex = 0; directoryIndex < textureSearchDirectoriesSize; directoryIndex++) {
			strcpy (fileName, textureSearchDirectories [directoryIndex]);
			//strcpy (fileName, textureDirectory ());
			strcat (fileName, "\\");
			strcat (fileName, textureName);
			strcat (fileName, suffix);
			if (Texture::readTextureExtent (fileName, textureWidth, textureHeight)) return true;
			if (loggingSearch) ::log ("\nFailed to read \"%s\"... but continuing to look...", fileName);
		}
		strcpy (fileName, textureName);
		strcat (fileName, suffix);
		if (loggingSearch) ::log ("\nFailed to read \"%s\"...", fileName);
		return false;
	}
}

struct NameAndExtent {
	char *name;
	long width;
	long height;
};

declareCollection (NameAndExtent);
NameAndExtentCollection notFoundTextures;

FaceCollection *convertToFaces (WorldcraftSidePlaneCollection *sidePlanes) {
	//Convert the side planes into faces (one face per side plane unless a particular side plane is
	//degenerate; i.e., consists of a zero-area face).
	FaceCollection *faces = new FaceCollection;

	//Set up 2 new collections and iterators: planes and transformations.
	PlaneCollection *planes = new PlaneCollection;
	TransformationCollection *transformations = new TransformationCollection;

	//Get the point and the texture mapping transformation for each side point by iterating over the side planes.
	for (long sidePlaneIndex = 0; sidePlaneIndex < sidePlanes->size (); sidePlaneIndex++) {

		//Consider this side plane.
		WorldcraftSidePlane &sidePlane = *((*sidePlanes) [sidePlaneIndex]); sidePlane.planeIsDegenerate = false;

/*printf("( %d %d %d ) ( %d %d %d ) ( %d %d %d ) %s\n",
(int) sidePlane.point1.x, (int) sidePlane.point1.y, (int) sidePlane.point1.z,
(int) sidePlane.point2.x, (int) sidePlane.point2.y, (int) sidePlane.point2.z,
(int) sidePlane.point3.x, (int) sidePlane.point3.y, (int) sidePlane.point3.z,
(int) sidePlane.textureName);*/

/*printf("( %d %d %d ) ( %d %d %d ) ( %d %d %d ) %s [ %d %d %d %d ] [ %d %d %d %d ] %d %d %d\n",
(int) sidePlane.point1.x, (int) sidePlane.point1.y, (int) sidePlane.point1.z,
(int) sidePlane.point2.x, (int) sidePlane.point2.y, (int) sidePlane.point2.z,
(int) sidePlane.point3.x, (int) sidePlane.point3.y, (int) sidePlane.point3.z,
(int) sidePlane.textureName,
(int) sidePlane.xAxis.x, (int) sidePlane.xAxis.y, (int) sidePlane.xAxis.z, (int) sidePlane.xAxisOffset,
(int) sidePlane.yAxis.x, (int) sidePlane.yAxis.y, (int) sidePlane.yAxis.z, (int) sidePlane.yAxisOffset,
(int) sidePlane.rotation,
(int) sidePlane.xAxisScale, (int) sidePlane.yAxisScale);*/

		//Convert points to a standard coordinate system (worldcraft uses x +ve is right, y+ve is back, z+ve is up).
		//Also convert from clockwise order p1,p2,p3 to counter-clockwise order p2,p1,p3.
		//Note: these are NOT points in a face but merely 3 points which can be used to compute the plane of the face.
		Point point1 (sidePlane.point1.x, sidePlane.point1.z, -sidePlane.point1.y);
		Point point2 (sidePlane.point2.x, sidePlane.point2.z, -sidePlane.point2.y);
		Point point3 (sidePlane.point3.x, sidePlane.point3.z, -sidePlane.point3.y);
		
		//Compute the plane and carry on only if it's a legitimate plane (Yup... Worldcraft can give you junk).
		Plane *plane = Plane::fromDangerousPoints (point2, point1, point3);
		if (plane == NULL) {
			log ("\nDiscarded a zero-area triangle; points are \n\t%3.2f,%3.2f,%3.2f]\n\t%3.2f,%3.2f,%3.2f]\n\t%3.2f,%3.2f,%3.2f]",
				point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z);
			static bool thisIsFirstError = true;
			if (thisIsFirstError) {
				prompt ("\nDiscarded a zero-area triangle; points are \n\t%3.2f,%3.2f,%3.2f]\n\t%3.2f,%3.2f,%3.2f]\n\t%3.2f,%3.2f,%3.2f]",
					point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, point3.x, point3.y, point3.z);
				thisIsFirstError = false;
			}
			sidePlane.planeIsDegenerate = true;
		} else {
			//Just record the plane.
			sidePlane.plane = *plane;

			//Recall: worldcraft coordinates are of the form: x z -y.
			//Convert the worldcraft axes for texture mapping to the standard system.
			//These can be used as rows in a matrix that gives us R.
			Point xAxis (sidePlane.xAxis.x, sidePlane.xAxis.z, -sidePlane.xAxis.y);
			Point yAxis (sidePlane.yAxis.x, sidePlane.yAxis.z, -sidePlane.yAxis.y);
			Point zAxis = xAxis.cross (yAxis);

			//Replace by the inverse (the transpose is the inverse in this case). 
			//When these rows are placed in a transformation matrix, it gives us R-1.
			Point uAxis (xAxis.x, yAxis.x, zAxis.x);
			Point vAxis (xAxis.y, yAxis.y, zAxis.y);
			Point wAxis (xAxis.z, yAxis.z, zAxis.z);

			//Read the texture to get the extent (we assume it's in directory Texture)....
			static long maximumErrorMessagesAllowed = 4;
			bool readWorked = true;
			long textureWidth; long textureHeight; char *newName;
			if (
				getTextureExtent (sidePlane.textureName, newName, textureWidth, textureHeight, ".dds") ||
				getTextureExtent (sidePlane.textureName, newName, textureWidth, textureHeight, ".tga") ||
				getTextureExtent (sidePlane.textureName, newName, textureWidth, textureHeight, ".bmp") ||
				getTextureExtent (sidePlane.textureName, newName, textureWidth, textureHeight, ".avi")) {
			} else {
				//Give up and generate an error message.
				static bool firstTime = true; static bool cancelPrompts = false;
				if (firstTime) {
					firstTime = false;
					/*
					prompt ("Encountered unknown texture \"%s\"..."
						"\nAccurate texture sizes needed to compute correct texture coordinates."
						"\nWill next be prompting for this texture"
						"\nCancelling prompts will supply default 256x256.", 
						sidePlane.textureName);
					*/
					if (!booleanQuery ("Encountered unknown texture \"%s\"..."
						"\nAccurate texture sizes needed to compute correct texture coordinates."
						"\nIf we prompt you for this texture, you can either find the texture"
						"\nor a substitute or cancel the prompt to supply default 256x256."
						"\nYES => PROMPT, NO => DON'T EVER PROMPT...", 
						sidePlane.textureName)) cancelPrompts = true;
				}
				bool found = false;
				loopVector (nameIndex, notFoundTextures)
					NameAndExtent *nameAndExtent = notFoundTextures [nameIndex];
					if (stricmp (nameAndExtent->name, sidePlane.textureName) == 0) {
						found = true; 
						textureWidth = nameAndExtent->width;
						textureHeight = nameAndExtent->height;
						break;
					}
				endloop
				if (!found) { 
					if (cancelPrompts || !getTextureExtent (sidePlane.textureName, newName, textureWidth, textureHeight, "")) {
						textureWidth = textureHeight = 256;
					}
					log ("\nCould not find texture \"%s\".*; user supplied extent %dx%d.", sidePlane.textureName, textureWidth, textureHeight);
					char *copy = new char [strlen (sidePlane.textureName) + 1];
					strcpy (copy, sidePlane.textureName);
					NameAndExtent *nameAndExtent = new NameAndExtent;
					nameAndExtent->name = copy;
					nameAndExtent->width = textureWidth;
					nameAndExtent->height = textureHeight;
					notFoundTextures.push_back (nameAndExtent);
				}
				/*
				if (maximumErrorMessagesAllowed < 0) {
					prompt ("\nCould not find texture \"%s\".*; assuming extent 256@256.", sidePlane.textureName);
					textureWidth = 256; textureHeight = 256; readWorked = false;
					maximumErrorMessagesAllowed--;
					if (maximumErrorMessagesAllowed < 0) {halt ("Too many error messages... execution terminated...");}
				}
				*/
			}

			/*
			char tgaFilePathName [500];
			strcpy ((char *) &tgaFilePathName [0], "..\\Textures\\");
			strcat ((char *) &tgaFilePathName [0], sidePlane.textureName);
			strcat ((char *) &tgaFilePathName [0], ".tga");
			char *newName = (char *) &tgaFilePathName [0];
			long textureWidth; long textureHeight;
			bool readWorked = true;
			if (!Texture::readTextureExtent (newName, textureWidth, textureHeight)) {
				//Try again with .bmp suffix.
				char bmpFilePathName [500];
				strcpy ((char *) &bmpFilePathName [0], "..\\Textures\\");
				strcat ((char *) &bmpFilePathName [0], sidePlane.textureName);
				strcat ((char *) &bmpFilePathName [0], ".bmp");
				newName = (char *) &bmpFilePathName [0];
				if (!Texture::readTextureExtent (newName, textureWidth, textureHeight)) {
					//Give up and give a .bmp file error message.
					log ("\nCould not find texture \"%s\" or texture \"%s\".", &bmpFilePathName [0], &tgaFilePathName [0]);
					prompt ("\nCould not find texture \"%s\" or texture \"%s\".", &bmpFilePathName [0], &tgaFilePathName [0]);
					textureWidth = 1; textureHeight = 1; readWorked = false;
					maximumErrorMessagesAllowed--;
					if (maximumErrorMessagesAllowed < 0) {halt ("Too many error messages... execution terminated...");}
				}
			}
			*/
			const bool fullPathNameRemembered = false;
			if (readWorked && fullPathNameRemembered) {//Remember the new full path name.
				delete [] sidePlane.textureName; 
				sidePlane.textureName = new char [strlen (newName) + 1];
				strcpy (sidePlane.textureName, newName);
			}

			//Create and record a transformation to map texture coordinates from normal coordinates.
			Point axisScale (1.0/sidePlane.xAxisScale, -1.0/sidePlane.yAxisScale, 1.0);
			Point textureTranslation (sidePlane.xAxisOffset, -sidePlane.yAxisOffset, 0.0);
			Point textureScale (1.0/textureWidth, 1.0/textureHeight, 1.0);
			
			Transformation &transformation = sidePlane.transformation;
			transformation.rotateToAxes (uAxis, vAxis, wAxis); //R-1
			transformation.postScaleBy (axisScale); //S-1
			transformation.postTranslateBy (textureTranslation); //T-1
			transformation.postScaleBy (textureScale); //N-1

/*printf("%f, %f, %f, %f\n", transformation.m11, transformation.m12, transformation.m13, transformation.m14);
printf("%f, %f, %f, %f\n", transformation.m21, transformation.m22, transformation.m23, transformation.m24);
printf("%f, %f, %f, %f\n", transformation.m31, transformation.m32, transformation.m33, transformation.m34);
printf("%f, %f, %f, %f\n", transformation.m41, transformation.m42, transformation.m43, transformation.m44);
printf("\n");*/
		}
	}

	//Compute the face coordinates by starting with the points of a very large face and cutting it up
	//by ALL the planes. The resulting points are the coordinates of the faces we wish to build. Once 
	//done, compute the texture coordinates using the transformation computed earlier.

	//Should we or should we not enlarge the volume of the resulting points to eliminate pixel leaks between adjacent convex objects.
	enum EnlargementStrategy {DoNothing, ScaleFacePointsOutwardOnThePlane, TranslateFacePointsOutwardOnThePlane, MoveThePlanesOutward};
	//const EnlargementStrategy enlargementStrategy = ScaleFacePointsOutwardOnThePlane; 
	const EnlargementStrategy enlargementStrategy = ScaleFacePointsOutwardOnThePlane; 

	const double enlargementForScalingOnPlane = 1.0001; //1.0002; //1.0001; //wilf
	const double enlargementTranslatingOnPlane = 0.0001; 
	const double enlargementForMovingPlaneOut = 0.015; //worldcraft units.... = 0.000381 meters.
		//A worldcraft unit is an inch and there are 0.0254 meters per inch.
		//So 0.015 worldcraft units is 0.0254 * 0.015 (or 0.000381) meters.

	if (enlargementStrategy == MoveThePlanesOutward) {
		for (sidePlaneIndex = 0; sidePlaneIndex < sidePlanes->size (); sidePlaneIndex++) {
			WorldcraftSidePlane &sidePlane = *((*sidePlanes) [sidePlaneIndex]);
			Plane &plane = sidePlane.plane;
			plane.displaceByDistance (enlargementForMovingPlaneOut);
		}
	}

	for (sidePlaneIndex = 0; sidePlaneIndex < sidePlanes->size (); sidePlaneIndex++) {

		//Consider this side plane and it's associated heavily used parts.
		WorldcraftSidePlane &sidePlane = *((*sidePlanes) [sidePlaneIndex]);
		if (sidePlane.planeIsDegenerate) continue;
		Plane &plane = sidePlane.plane;
		Transformation &transformation = sidePlane.transformation;

		//Construct the points for the face coordinates
		PointCollection *largeFacePoints = anyFarCornerPoints (plane);
/*for(unsigned int v=0;v<largeFacePoints->size();v++) {
printf("(%f, %f, %f)\n",
largeFacePoints->at(v)->x, largeFacePoints->at(v)->y, largeFacePoints->at(v)->z);
}*/
		PointCollection *cutUpFacePoints = cutUpBy (largeFacePoints, sidePlanes, sidePlaneIndex);
// CURRENT PRINTF
/*for(unsigned int u=0;u<cutUpFacePoints->size();u++) {
printf("(%f, %f, %f)\n",
cutUpFacePoints->at(u)->x, cutUpFacePoints->at(u)->y, cutUpFacePoints->at(u)->z);
}*/
		PointCollection *enlargedFacePoints = enlarge (cutUpFacePoints, plane);
		PointCollection *moreAccurateCutUpFacePoints = cutUpBy (enlargedFacePoints, sidePlanes, sidePlaneIndex);

		deletePointCollection (largeFacePoints); deletePointCollection (cutUpFacePoints);
		deletePointCollection (enlargedFacePoints);

		//Enlarge just by a tiny amount to get rid of pixel leaks...
//		const bool enlargeByATinyAmountToGetRidOfPixelLeaks = true;
//		if (enlargeByATinyAmountToGetRidOfPixelLeaks) {
		if (enlargementStrategy == ScaleFacePointsOutwardOnThePlane) {
			PointCollection *leakFreePoints;
			leakFreePoints = enlarge (moreAccurateCutUpFacePoints, plane, enlargementForScalingOnPlane);
			deletePointCollection (moreAccurateCutUpFacePoints);
			moreAccurateCutUpFacePoints = leakFreePoints;
		} else if (enlargementStrategy == TranslateFacePointsOutwardOnThePlane) {
			PointCollection *leakFreePoints;
			leakFreePoints = enlargeViaTranslating (moreAccurateCutUpFacePoints, plane, enlargementTranslatingOnPlane);
			deletePointCollection (moreAccurateCutUpFacePoints);
			moreAccurateCutUpFacePoints = leakFreePoints;
		}

		//Finally, replace standard 3D points by game points.
		GamePointCollection *gamePoints = new GamePointCollection;
		for (long index = 0; index < moreAccurateCutUpFacePoints->size (); index++) {
			Point *worldcraftPoint = (*moreAccurateCutUpFacePoints) [index];
			Point coordinatePoint = *worldcraftPoint * worldcraftToMeters ();
			Point texturePoint = *worldcraftPoint * transformation;

			GamePoint *gamePoint = new GamePoint;
			gamePoint->x = coordinatePoint.x;
			gamePoint->y = coordinatePoint.y;
			gamePoint->z = coordinatePoint.z;
			gamePoint->nx = plane.normal.x;
			gamePoint->ny = plane.normal.y;
			gamePoint->nz = plane.normal.z;
			gamePoint->tx = texturePoint.x;
			gamePoint->ty = texturePoint.y;

			gamePoints->push_back (gamePoint);
		}
		deletePointCollection (moreAccurateCutUpFacePoints);

		//Create a new face only if there are sufficient points.
		if (gamePoints->empty ()) {
			deleteGamePointCollection (gamePoints);
			delete [] sidePlane.textureName;
			gamePoints = NULL;
		} else {
			//Recompute the plane since the coordinates have been transformed.
			Point point1 = (*gamePoints) [0]->asPoint ();
			Point point2 = (*gamePoints) [1]->asPoint ();
			Point point3 = (*gamePoints) [2]->asPoint ();
			Plane *newPlane = Plane::fromDangerousPoints (point1, point2, point3);
			if (newPlane == NULL) {
				Point pointingRight (1.0, 0.0, 0.0);
				newPlane = new Plane (pointingRight, point1);
			}
			//Face *face = new Face (*newPlane, sidePlane.textureName);
			Face *face = new Face (sidePlane.textureName);
			delete newPlane;
			//Give the points to the face's point collection.
			loopVector (pointIndex, *gamePoints)
				face->points.push_back ((*gamePoints) [pointIndex]);
			endloop
			faces->push_back (face);
		}
		//Delete the collection but not the game points now owned by the face.
		delete gamePoints; 
	}

	return faces;
}

#undef add

//*****************************************************************************************//
//              Additional facilities that used to be in UniversalObject.cpp               //
//*****************************************************************************************//

Point worldcraftToStandardPoint (Point &worldcraftPoint) {
	//Convert points to a standard coordinate system (worldcraft uses x +ve is right, y+ve is back, z+ve is up).
	extern double worldcraftToMeters ();
	Point standardPoint (
		worldcraftPoint.x * worldcraftToMeters (), 
		worldcraftPoint.z * worldcraftToMeters (),
		-worldcraftPoint.y * worldcraftToMeters ());
	return standardPoint;
}

void addProperties (Object *object, WorldcraftObject &worldcraftObject, char *type) {
	object->properties.put ("type", type);
	//object->properties.put ("class", type); //A synonym for type: Type is clobbered by ModelEditor which considers everything to be a model...
	object->properties.put ("name", worldcraftObject.asStringProperty ("targetname", ""));
	if (worldcraftObject.properties == NULL) return;
	loopMap (next, *worldcraftObject.properties, WorldcraftGroup) //A group is a map
		const char *key = (*next).first.c_str (); void *value = (*next).second;
		if (stricmp (key, "objects") == 0) continue; //Consider everything but the "objects" property that is supposed to be ignored.
		if (stricmp (key, "classname") == 0) continue; //Renamed type...
		if (stricmp (key, "targetname") == 0) continue; //Renamed name...
		object->properties.put ((char *) key, (char *) value);
	endloop
}

extern bool tracing; //Defined in universalObject.cpp

void UniversalObject::add (WorldcraftObject &worldcraftObject) {
	//Create a NON-worldcraft object corresponding to the worldcraft object.
	
	//Multi-part vehicles with a roof and a body are passed along as separate objects in the map file
	//unless they are grouped together. We now track objects that were grouped so as to place all their
	//faces together in one object. If you wish to disable this facility, just set the next boolean to false...
	const bool wantToGroupObjects = true; 

	//Variables to remember the object and property dictionary processed last time.
	static Object *lastObject = NULL;
	static WorldcraftGroup *lastProperties = NULL; 

	//We are starting a new group of objects if it has a different property dictionary.
	const bool startingNewGroup = worldcraftObject.properties != lastProperties;
	lastProperties = worldcraftObject.properties; //Prepare for next time...

	//For debugging, let's see what the worldcraft object looks like.
	if (tracing) {::log ("\n\nInput object..."); worldcraftObject.log ();}

	//If we are starting a new group, create an object; otherwise, use the last one.
	//Insert duplicates of the faces into the object along with duplicates of it's properties...
	const bool isStaticGeometry = stricmp (worldcraftObject.type, "worldspawn") == 0;

	const bool needNewObject = startingNewGroup || !wantToGroupObjects || isStaticGeometry;
	Object *object = needNewObject ? new Object () : (Object *) lastObject;
	if (worldcraftObject.faces != NULL) object->copyWorldcraftFaces (*worldcraftObject.faces);

	char *type = isStaticGeometry ? "static geometry" : worldcraftObject.type;
	if (needNewObject) {
		objects.push_back (object); addProperties (object, worldcraftObject, type);
	}
	lastObject = object; //Prepare for next time...

	if (tracing) {::log ("\n\nOutput object%s...", wantToGroupObjects ? " SO FAR" : ""); object->log ();}
}

Point asPoint (CString &string) {
	Point point;
	sscanf ((LPCTSTR) string, "%lf %lf %lf", &point.x, &point.y, &point.z);
	return point;
}

Point asAngles (CString &string) {
	//Worldcraft reports the angles as Pitch, Yaw, and Roll -- this corresponds to x, y, and z rotations for us.
	//However, they are off by 90.0 degrees in the 'yaw'."
	Point angles;
	sscanf ((LPCTSTR) string, "%lf %lf %lf", &angles.x, &angles.y, &angles.z);
	angles.x = -angles.x;
	angles.y -= 90.0;
	angles.z = -angles.z;
	return angles;
}

double asFloat (CString &string) {
	double number;
	sscanf ((LPCTSTR) string, "%lf", &number);
	return number;
}

declareDictionary (PointCollection);

bool isDigit (char character) {
	return strchr ("0123456789", character) != NULL;
}

void UniversalObject::standardizeMapFilePathPoints () {
	//Interconnect all the path points...
	//PathPoints are named <pathname>, <pathname>01, <pathname>02, etc.; <pathname> is the starting point.

	//We need to track each path and delete the existing pathname objects...
	const bool logging = false;
	const bool newWay = true;
	if (logging) {::log ("\nPath points...");}

	PointCollectionDictionary paths;
	long nextObjectIndexToUse = 0;

	loopVector (objectIndex, objects)
		Object &object = *(objects [objectIndex]);
		CString *value = object.properties.get ("type");
		if (value != NULL && *value == "pathpoint") {
			//Extract the path name and the index at the end of the name...
			CString *name = object.properties.get ("name");
			if (name == NULL) {halt ("\nEncountered path point without a name..."); break;}
			if (logging) {::log ("\n\t\"%s\":", (LPCTSTR) *name);}

			Point *point = new Point;
			if (newWay) {
				CString *origin = object.properties.get ("origin");
				if (origin != NULL && origin->Left (8) != "<point> ") {
					halt ("\nExpected \"origin\" to have been previously standardized... it's \"%s\"...", (LPCTSTR) *origin);
				}
				*point = origin == NULL ? Zero : asPoint (origin->Right (origin->GetLength () - 8));
				if (logging) {
					::log ("\n\t\tStandardizePathPoints: Obtained origin \"%s\" and left it unchanged as %3.2f %3.2f %3.2f...",
						(LPCTSTR) *origin, point->x, point->y, point->z);
				}
			} else {
				*point = object.transformation.normal ().position ();
			}

			//Find the first digit at the end of the name... PERMIT INTERIOR DIGITS as in "laser2start"...
			//for (long firstDigit = 0; firstDigit < name->GetLength (); firstDigit++) {
			//	if (isDigit (((LPCTSTR) *name) [firstDigit])) break;
			//}
			for (long firstDigit = name->GetLength () - 1; firstDigit >= 0; firstDigit--) {
				if (!isDigit (((LPCTSTR) *name) [firstDigit])) break;
			}
			CString pathname = name->Left (firstDigit + 1); 
			long nameSuffix = 0;
			for (firstDigit++; firstDigit < name->GetLength (); firstDigit++) {
				nameSuffix = nameSuffix * 10 + (((LPCTSTR) *name) [firstDigit] - '0'); //Primitive conversion...
			}
			if (logging) {::log (" \"%s\" \"%d\" point %3.2f %3.2f %3.2f ", (LPCTSTR) pathname, nameSuffix, point->x, point->y, point->z);}

			//Get or create a point collection association with this path name... and insert the point..
			PointCollection *pointCollection = get (paths, (char *) (LPCTSTR) pathname);
			if (pointCollection == NULL) {//It's a new one...
				pointCollection = new PointCollection;
				put (paths, (char *) (LPCTSTR) pathname, pointCollection);
			}
			while (pointCollection->size () <= nameSuffix) {
				pointCollection->push_back (NULL);
			}
			(*pointCollection) [nameSuffix] = point;

			//Delete this pathpoint object.
			delete objects [objectIndex]; objects [objectIndex] = NULL;
		} else {
			//Don't delete this object...
			objects [nextObjectIndexToUse] = objects [objectIndex];
			nextObjectIndexToUse++;
		}
	endloop

	//Shrink the collection by the number of objects deleted...
	while (objects.size () > nextObjectIndexToUse) {
		objects.pop_back ();
	}

	//Create either a pointtracker or pathtracker object to represent the path...
	if (logging) {::log ("\nPoint and path trackers:");}
	loopDictionary (name, pointCollection, PointCollection, paths)
		//Count the number of points (allowing for missing points)...
		long truePointsSize = 0; Point *anyPoint = NULL;
		loopVector (pointIndex, *pointCollection)
			Point *point = (*pointCollection) [pointIndex];
			if (point != NULL) truePointsSize++;
			if (anyPoint == NULL && point != NULL) anyPoint = point;
		endloop

		Object *object = new Object;
		object->properties.put ("name", name);

		if (truePointsSize == 1) {
			object->properties.put ("type", "pointtracker");
			//object->properties.put ("class", "pointtracker");
			if (newWay) {
				CString pathPoint = "<point>";
				if (anyPoint == NULL) {
					pathPoint += " 0.0 0.0 0.0";
				} else {
					pathPoint += asString (" %f", anyPoint->x);
					pathPoint += asString (" %f", anyPoint->y);
					pathPoint += asString (" %f", anyPoint->z);
				}				
				if (logging) {
					::log ("\n\tPoint tracker \"%s\" at %3.2f %3.2f %3.2f", (LPCTSTR) name, anyPoint->x, anyPoint->y, anyPoint->z);
				}
				//object->transformation.translateTo (*anyPoint); //As opposed to emitting pathPoint as a "point" property...
				object->properties.put ("origin", pathPoint);
			} else {
				object->transformation.translateTo (*anyPoint); 
				if (logging) {
					::log ("\n\tPoint tracker \"%s\" at %3.2f %3.2f %3.2f (in transformation)", (LPCTSTR) name, anyPoint->x, anyPoint->y, anyPoint->z);
				}
			}
		} else {
			//Build a string containing the path points; tranformation already the identity...
			object->properties.put ("type", "pathtracker");
			//object->properties.put ("class", "pathtracker");
			CString pathPoints = "<points>";
			loopVector (pointIndex, *pointCollection)
				Point *point = (*pointCollection) [pointIndex];
				if (point != NULL) {
					pathPoints += asString (" [%f", point->x);
					pathPoints += asString (" %f", point->y);
					pathPoints += asString (" %f]", point->z);
				}
			endloop
			object->properties.put ("points", pathPoints);
			if (logging) {
				::log ("\n\tPath tracker \"%s\" at %s", (LPCTSTR) name, (LPCTSTR) pathPoints);
			}
		}

		objects.push_back (object);

		deletePointCollectionEntries (*pointCollection);
	endloop

	deletePointCollectionDictionaryEntries (paths);
}

bool UniversalObject::isTouching (Object &orienter, Object &candidate) {
	const bool logging = false;
	if (!orienter.box.intersects (candidate.box)) return false;
	
	//Loop through each face and create a collection of transformed points.
	loopVector (faceIndex, candidate.faces)
		Face &face = *(candidate.faces [faceIndex]);

		PointCollection *points = new PointCollection;
		loopVector (pointIndex, face.points)
			GamePoint &point = *(face.points [pointIndex]);
			Point transformedPoint = point.asPoint () * candidate.transformation;
			points->push_back (new Point (transformedPoint)); 
		endloop
		if (logging) ::log ("\n\t\tConsider face %d with %d points...", faceIndex, face.points.size ());

		//Clip the points by the planes of the orienter's bounding box. If anything is left, some point is in the box...
		#define CLIP(plane,planeNormal,planePoint) { \
			Plane plane (planeNormal, orienter.box.planePoint); points = clip (plane, points); \
			if (logging) {::log (" %s (%d)%s", #planeNormal, points->size (), points->size () > 0 ? "" : " FAILED");} \
			if (points->size () == 0) {deletePointCollection (points); continue;} /*Move on to the next face.*/}

		if (logging) {::log ("\n\t\t\tClip");}
		CLIP (left, Right, minimum);
		CLIP (right, Left, maximum);
		CLIP (up, Up, minimum);
		CLIP (down, Down, maximum);
		CLIP (ahead, Ahead, maximum);
		CLIP (back, Back, minimum);
		if (logging) {::log (" SUCCESS");}
		#undef CLIP

		//Made it through with something remaining...
		deletePointCollection (points); return true;
	endloop 

	//Everything was clipped.
	return false;
}

Object *UniversalObject::orienterFor (Object &candidate) {
	const bool logging = false;
	loopVector (objectIndex, objects)
		Object &orienter = *(objects [objectIndex]);
		CString *value = orienter.properties.get ("type");
		const bool isMirror = value != NULL && *value == "orienter";
		if (isMirror) {
			const bool intersects = orienter.box.intersects (candidate.box);
			const bool touching = isTouching (orienter, candidate);
			if (logging) {
				CString *name = orienter.properties.get ("name");
				if (name == NULL) ::log ("\n\tOrienter %x...", &orienter); else ::log ("\n\tOrienters \"%s\" %x...", (char *) (LPCTSTR) *name, &orienter);
				orienter.box.center ().log (); ::log (" extent "); orienter.box.extent ().log ();
				::log (" %s...", intersects ? "INTERSECTS" : "DOES NOT INTERSECT");
				if (intersects) {
					if (touching) ::log (" and TOUCHING ****"); else ::log (" but NOT TOUCHING");
				} else {
					if (touching && logging) ::log (" yet TOUCHING");
				}
			}
			
			if (touching) return &orienter;
		}
	endloop	
	return NULL;
}

Object *UniversalObject::mirrorFor (Object &candidate) {
	const bool logging = false;
	CString *type = candidate.properties.get ("type");
	if (type != NULL && *type == "mirror") return NULL; //Mirrors have no mirrors...

	loopVector (objectIndex, objects)
		Object &mirror = *(objects [objectIndex]);
		CString *value = mirror.properties.get ("type");
		const bool isMirror = value != NULL && *value == "mirror";
		if (isMirror) {
			const bool intersects = mirror.box.intersects (candidate.box);
			const bool touching = isTouching (mirror, candidate);
			if (logging) {
				CString *name = mirror.properties.get ("name");
				if (name == NULL) ::log ("\n\tMirror %x...", &mirror); else ::log ("\n\tMirrors \"%s\" %x...", (char *) (LPCTSTR) *name, &mirror);
				mirror.box.center ().log (); ::log (" extent "); mirror.box.extent ().log ();
				::log (" %s...", intersects ? "INTERSECTS" : "DOES NOT INTERSECT");
				if (intersects) {
					if (touching) ::log (" and TOUCHING ****"); else ::log (" but NOT TOUCHING");
				} else {
					if (touching && logging) ::log (" yet TOUCHING");
				}
				CString *objectName = candidate.properties.get ("name");
				CString *objectType = candidate.properties.get ("type");
				::log (" %s", asString (objectName), asString (objectType));
			}
			
			if (touching) return &mirror;
		}
	endloop	
	return NULL;
}

Object *UniversalObject::coronaFor (Object &candidate, double &closestDistance) {
	closestDistance = DBL_MAX; Object *closestCorona = NULL;
	loopVector (objectIndex, objects)
		Object &corona = *(objects [objectIndex]);
		CString *value = corona.properties.get ("type");
		const bool isCorona = value != NULL && *value == "corona";
		if (isCorona) {
			double distance = corona.box.center ().distanceTo (candidate.box.center ());
			if (distance < closestDistance) {closestCorona = &corona; closestDistance = distance;}
		}
	endloop	
	return closestCorona;
}

Transformation UniversalObject::orientingTransformation (Object &orienter, Point &objectCenter, bool flip) {
	//Returns a transformation that will orient an object to match the orienter's front and top faces. If there isn't one, 
	//it's a artist's bug. So we warn him and provide an identity transformation.

	//Loop through each face looking for two special texture names...
	Face *back = NULL; Face *top = NULL;

	loopVector (faceIndex, orienter.faces)
		Face &face = *(orienter.faces [faceIndex]);
		//CString *textureName = properties.get ("texture");
		if (face.textureName != NULL && stricmp (face.textureName, "ORIENT-Z") == 0) back = &face;
		if (face.textureName != NULL && stricmp (face.textureName, "ORIENT-Y") == 0) top = &face;
	endloop

	Transformation transformation;
	if (back == NULL || top == NULL) {
		::halt ("\n***Error, incorrect orienter, missing either orient-z or orient-y texture... Continue for identity");
		return transformation;
	}

	Vector zAxis = flip ? -back->normal () : back->normal ();
	Vector yAxis = top->normal ();
	Vector xAxis = yAxis.cross (zAxis);
	//transformation.rotateToAxes (xAxis, yAxis, zAxis, orienter.box.center ());
	transformation.rotateToAxes (xAxis, yAxis, zAxis, objectCenter);
	return transformation;
}

void UniversalObject::standardizeViaOrienters () {
	//Any object intersecting with an orienter is assumed to need a transformation built to orient and translate it...
	//We need to track each path and delete the existing pathname objects...
	const bool logging = true;
	if (logging) {::log ("\nObject bounding boxes...");}

	//Compute the bounding box of each object...
	long orientersCount = 0; long attachedOrientersCount = 0;
	loopVector (objectIndex, objects)
		Object &object = *(objects [objectIndex]);

		CString *type = object.properties.get ("type");
		if (type != NULL && *type == "orienter") orientersCount++;

		if (object.faces.size () > 0) {
			object.box = BoundingBox (&object);
		} else {
			CString *originString = object.properties.get ("origin");
			Point origin = Zero;
			if (originString != NULL)
				sscanf ((LPCTSTR) *originString, "<point> %lf %lf %lf", &origin.x, &origin.y, &origin.z);
			//::log ("\n%s %x has point ", (char *) (LPCTSTR) *(object.properties.get ("type")), &object); origin.log ();
			object.box = BoundingBox (origin);
		}
		if (logging) {::log ("\nObject %x has bounding box ", &object); object.box.log ();}
	endloop

	if (logging) {
		loopVector (objectIndex, objects)
			Object &orienter = *(objects [objectIndex]);
			CString *type = orienter.properties.get ("type");
			const bool isMirror = type != NULL && *type == "orienter";
			if (isMirror) {
				::log ("\nOrienter %x center ", &orienter); orienter.box.center ().log (); ::log (" extent "); orienter.box.extent ().log ();
			}
		endloop	
	}
		
	if (logging) {::log ("\nStandardize via %d orienters... Standardized objects include", orientersCount);}

	//Consider any object that is NOT an orienter...
	long standardizedObjectsCount = 0;
	loopVector (objectIndex, objects)
		Object &object = *(objects [objectIndex]);
		CString *type = object.properties.get ("type");
		CString *objectName = object.properties.get ("name");
		if (type != NULL && *type != "orienter" && *type != "static geometry") {
			if (logging) {
				//const bool isMirrorOrTree = type != NULL && (*type == "mirror" || *type == "treeobject");
				::log ("\n%2d: %s %s %x at box center ", standardizedObjectsCount++, (char *) (LPCTSTR) *type, 
					asString (objectName), &object); 
				object.box.center ().log (); ::log ("...");
			}

			Object *mirror = mirrorFor (object); //Just to get some debugging information...
			Object *orienter = orienterFor (object);

			if (orienter != NULL) {
				const bool orientMirrorsLikeEverythingElse = true;
				if (!orientMirrorsLikeEverythingElse && mirror != NULL) {
					//Version that does not use orienters on the mirrors...
					object.properties.put ("avoid orienting", "true");
					continue; 
				}
				attachedOrientersCount++;
				if (logging) {
					CString *name = orienter->properties.get ("name");
					//if (name == NULL) ::log (" attached to orienter %x...", orienter); else ::log (" attached to orienter \"%s\" %x...", (char *) (LPCTSTR) *name, orienter);
					::log ("\n\tOrienter %s is attached to %s...", 
						name == NULL ? asString ("%x", orienter) : asString ("\"%s\" %x", (char *) (LPCTSTR) *name, orienter), 
						(char *) (LPCTSTR) *type);
					double coronaDistance; Object *corona = coronaFor (object, coronaDistance);
					if (corona == NULL) 
						::log ("\nThere is NO closest corona");
					else {
						CString *name = corona->properties.get ("name");
						::log ("\nThe closest corona is \"%s\" at distance %3.2f", name == NULL ? "UNNAMED" : (char *) (LPCTSTR) *name, coronaDistance);
					}
				}

				//Oh, Oh: Orienters are backward on all mirrors.. Z-should point out rather than F-...
				Transformation transformation = orientingTransformation (*orienter, object.box.center (), mirror != NULL); 

				//Loop through each face and transform to local coordinates relative to "transformation".
				loopVector (faceIndex, object.faces)
					Face &face = *(object.faces [faceIndex]);

					loopVector (pointIndex, face.points)
						GamePoint &point = *(face.points [pointIndex]);
						Point transformedPoint = transformation.pointInverseTransform (point.asPoint ());
						point.x = transformedPoint.x;
						point.y = transformedPoint.y;
						point.z = transformedPoint.z;
					endloop

					//Recompute the normal in local coordinates.
					Point normal = face.normal ();
					loopVector (pointIndex, face.points)
						GamePoint &point = *(face.points [pointIndex]);
						point.nx = normal.x;
						point.ny = normal.y;
						point.nz = normal.z;
					endloop
				endloop
				
				object.transformation.normal () = transformation;
				object.transformation.inverse.scaleFreeInverse (transformation);
				if (logging) {
					::log ("\n\tTransformation        maps [0.0.0] to "); transformation.position ().log ();
					::log ("\n\tObject transformation maps [0.0.0] to "); object.transformation.normal ().position ().log ();
					//::log ("\n\tObject %s with orienter %s has transformation ", (char *) (LPCTSTR) *object.properties.get ("name"),
					//	(char *) (LPCTSTR) *orienter->properties.get ("name"));
					//object.transformation.normal ().log (2);
					//::log ("\n\tTransformation WAS "); transformation.log (2);
				}

				//Now recompute the bounding box... If there was a rotation, it can change it's extent.
				object.box = object.faces.size () > 0
					? BoundingBox (&object)
					: BoundingBox (object.transformation.normal ().position ());
				
				//TEMPORARY: Change the orienter's type so it will output and also move it ahead by exactly 1 meter...
				const bool forceOrientersToBeInTheWorld = true; //false; //wilf
				if (forceOrientersToBeInTheWorld) {
					orienter->properties.put ("type", "static geometry");
					orienter->transformation.translateBy (transformation.vectorTransform (Ahead));
				}
			}
		} //else if (logging) {::log (" SKIPPING");}
	endloop

	if (logging) {::log ("\n%d orienters are unattached...", orientersCount - attachedOrientersCount);}	
}

extern WorldcraftFGDReader *worldCraftFGDReader; //Defined in universalObject.cpp
	
void UniversalObject::standardizeMapFileDefaults () {
	const bool logging = false;

	loopVector (objectIndex, objects)
		Object &object = *(objects [objectIndex]);
		//::log ("\nStandardizing object %x...", &object); object.log ();

		//Deal with the origin property which is NOT in the FGD file.
		CString *origin = object.properties.get ("origin");
		if (origin != NULL) {
			Point position = worldcraftToStandardPoint (asPoint (*origin));
			CString point = "<point>";
			point += asString (" %f", position.x);
			point += asString (" %f", position.y);
			point += asString (" %f", position.z);
			if (logging) {
				::log ("\nStandardizeDefaults: In object \"%s\" called \"%s\", obtained origin \"%s\" and converted it to \"%s\"...",
					object.properties.valueAt ("type"), object.properties.valueAt ("name"), (LPCTSTR) *origin, (LPCTSTR) point);
			}
			object.properties.put ("origin", (char *) (LPCTSTR) point);
		}
		
		//Deal with the angles (pitch/yaw/roll) property which is in the FGD file but has no default.
		CString *angles = object.properties.get ("angles"); Point rotation; //Accessed in next two if statements.
		if (angles != NULL) {
			rotation = asAngles (*angles);
			CString point = "<point>";
			point += asString (" %f", rotation.x);
			point += asString (" %f", rotation.y);
			point += asString (" %f", rotation.z);
			object.properties.put ("angles", (char *) (LPCTSTR) point);
		}
		
		//If there is a pitch, redo the angles property too to make it consistent...
		CString *pitch = object.properties.get ("pitch");
		if (pitch != NULL) {
			double angleX = asFloat (*pitch);
			CString number = "<float>";
			number += asString (" %f", -angleX); //Note: negated (see asAngles);
			object.properties.put ("pitch", (char *) (LPCTSTR) number);

			//CString *angles = object.properties.get ("angles"); 
			if (angles == NULL) rotation = Zero;
			rotation.x = -angleX;

			//Do as above...
			CString point = "<point>";
			point += asString (" %f", rotation.x);
			point += asString (" %f", rotation.y);
			point += asString (" %f", rotation.z);
			object.properties.put ("angles", (char *) (LPCTSTR) point);
		}
		
		//Deal with the _light property which is no longer in the FGD file but has color instead which defaults to white.
		CString *_light = object.properties.get ("_light");
		if (_light != NULL) {
			object.properties.put ("color", (char *) (LPCTSTR) *_light);
			object.properties.removeKey ("_light");
		}

		worldCraftFGDReader->mapAllVariables (object.properties); //Except origin, angles, and pitch.
	endloop
}

void UniversalObject::OLDstandardizeFacePoints () {
	loopVector (objectIndex, objects)
        Object &object = *(objects [objectIndex]);
		BoundingBox box (&object); Point offset = box.center ();
		Transformation &t = object.transformation.normal ();
		if (t.isIdentity ()) {
			loopVector (faceIndex, object.faces)
				Face &face = *(object.faces [faceIndex]);
				loopVector (pointIndex, face.points)
					GamePoint &point = *(face.points [pointIndex]);
					Point transformedPoint = point.asPoint () - offset;
					point.x = transformedPoint.x;
					point.y = transformedPoint.y;
					point.z = transformedPoint.z;
				endloop
			endloop
		} else {
			loopVector (faceIndex, object.faces)
				Face &face = *(object.faces [faceIndex]);
				loopVector (pointIndex, face.points)
					GamePoint &point = *(face.points [pointIndex]);
					Point transformedPoint = point.asPoint () * t - offset;
					point.x = transformedPoint.x;
					point.y = transformedPoint.y;
					point.z = transformedPoint.z;
				endloop
			endloop
		}
		object.transformation.setToIdentity ();
		object.transformation.preTranslateBy (offset);
	endloop
}

void UniversalObject::standardizeFacePoints () {
	//object.properties
	loopVector (objectIndex, objects)
        Object &object = *(objects [objectIndex]);
	
		DualTransformation &transformation = object.transformation;

		bool includesOrigin = object.properties.get ("origin") != NULL;
		bool includesAngles = object.properties.get ("angles") != NULL;
		bool includesTransformation = !transformation.normal ().isIdentity ();

		//Bypass standardization if there is already a transformation associated with the object or if we decided not to earlier on...
		if (includesTransformation) continue;
		if (*(object.properties.get ("type")) == "mirror") continue; //Mirrors (the little box on the wall) use a global bounding box... Don't standardize...
		if (object.properties.get ("avoid orienting") != NULL) continue;

		//Convert faces to global coordinates...
		if (includesTransformation) {
			object.transformFacesBy (transformation.normal ());
		}

		//Compute modifications to the transformation.
		if (includesOrigin) {
			CString *originString = object.properties.get ("origin");
			Point origin;
			sscanf ((LPCTSTR) *originString, "<point> %lf %lf %lf", &origin.x, &origin.y, &origin.z);
			transformation.preTranslateBy (origin);
		} else {
			BoundingBox box (&object); Point offset = box.center ();
			transformation.preTranslateBy (offset);
		}

		if (includesAngles) {
			CString *anglesString = object.properties.get ("angles");
			Point angles;
			sscanf ((LPCTSTR) *anglesString, "<point> %lf %lf %lf", &angles.x, &angles.y, &angles.z);

			//Override pitch value if an explicit one is provided (as by spotlights).
			bool includesPitch = object.properties.get ("pitch") != NULL;
			if (includesPitch) {
				CString *pitchString = object.properties.get ("pitch");
				double pitch;
				sscanf ((LPCTSTR) *pitchString, "<float> %lf", &pitch);
				angles.x = pitch;
			}

			if (absolute (angles.y) > epsilon ()) transformation.preRotateBy (angles.y, Point (0.0, 1.0, 0.0));
			if (absolute (angles.x) > epsilon ()) transformation.preRotateBy (angles.x, Point (1.0, 0.0, 0.0));
			if (absolute (angles.z) > epsilon ()) transformation.preRotateBy (angles.z, Point (0.0, 0.0, 1.0));	
		}
			
		//Convert faces to local coordinates.
		if (!transformation.inverse.isIdentity ()) {
			object.transformFacesBy (transformation.inverse);
		}

		//Finally, discard all properties that are no longer needed and remap "targetname" to "name" if it exists...
		if (includesOrigin) {object.properties.removeKey ("origin");}
		if (includesAngles) {object.properties.removeKey ("angles");}
		if (object.properties.get ("pitch") != NULL) {object.properties.removeKey ("pitch");}
		CString *targetName = object.properties.get ("targetname");
		if (targetName != NULL) {
			CString *name = object.properties.get ("name");
			if (name != NULL && *name != "" && *name != "<string> ") {
				//This is the one we keep...
			} else {
				//This one should be replaced by whatever targetName is...
				object.properties.put ("name", *targetName);
			}
			object.properties.removeKey ("targetname");
		}
		
		if (object.properties.get ("spawnflags") != NULL) object.properties.removeKey ("spawnflags");
	endloop
}

void UniversalObject::standardizeMapFile () {
	standardizeMapFileDefaults ();
	standardizeViaOrienters ();
	standardizeMapFilePathPoints (); //So that origin will have been converted...
	standardizeFacePoints ();
}