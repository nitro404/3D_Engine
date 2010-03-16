// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.awt.Dimension;
import java.io.*;
import java.util.Vector;

public class Face {
	
	public int faceIndex;
	public Vector<Property> properties;
	public Vector<GamePoint3D> points;
	
	public Face() {
		this.faceIndex = 0;
		this.properties = new Vector<Property>();
		this.points = new Vector<GamePoint3D>();
	}
	
	public Face(int faceIndex, Vector<Property> properties, Vector<GamePoint3D> points) {
		this.faceIndex = faceIndex;
		if(properties == null) {
			this.properties = new Vector<Property>();
		}
		else {
			this.properties = properties;
		}
		if(points == null) {
			this.points = new Vector<GamePoint3D>();
		}
		else {
			this.points = points;
		}
	}
	
	public Face(BufferedReader in) {
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid face in map file.");
			System.exit(1);
		}
	}
	
	public void addPoint(GamePoint3D point) {
		points.add(point);
	}
	
	public boolean addProperty(String key, String value) {
		// add a property if it is not a duplicate
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(key)) {
				return false;
			}
		}
		this.properties.add(new Property(key, value));
		return true;
	}
	
	public String getPropertyValue(String key) {
		// search for a property based on a key and return its value if it is present
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(key)) {
				return this.properties.elementAt(i).value;
			}
		}
		return null;
	}
	
	public boolean setPropertyValue(String key, String newValue) {
		// search for a property based on a key and change its value if it is present
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(key)) {
				this.properties.elementAt(i).value = newValue;
				return true;
			}
		}
		return false;
	}
	
	public boolean removeProperty(String key) {
		// search for a property based on a key and remove it if it is present
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(key)) {
				this.properties.remove(i);
				return true;
			}
		}
		return false;
	}
	
	public static Vector<Face> convertFrom(Vector<WorldcraftSidePlane> sidePlanes, int faceIndex, Vector<Texture> textures) throws Exception {
		
		Vector<Face> newFaces = new Vector<Face>();
		Vector<Transformation> transformations = new Vector<Transformation>();
		
		WorldcraftSidePlane sidePlane;
		Vector<WorldcraftSidePlane> tempPlanes = new Vector<WorldcraftSidePlane>();
		for(int x=0;x<sidePlanes.size();x++) {
			sidePlane = new WorldcraftSidePlane(sidePlanes.elementAt(x));
			
//			newFace.faceIndex = faceIndex++;
			
			Point3D[] standardPoints = new Point3D[3];
			for(int i=0;i<3;i++) {
				standardPoints[i] = new Point3D(sidePlane.points[i].x, sidePlane.points[i].z, -sidePlane.points[i].y);
			}
			
			Point3D normal = standardPoints[0].minus(standardPoints[1]).cross(standardPoints[2].minus(standardPoints[0]));
			
			if(!(normal.squaredLength() > 1.0e-10)) {
				System.out.println("WARNING: Ignoring zero-area triangle: (" + standardPoints[0] + "), (" + standardPoints[1] + "), (" + standardPoints[2] + ")");
				sidePlane.isDegenerate = true;
			}
			else {
				sidePlane.plane = new Plane3D(normal.multiplyBy(1.0 / Math.sqrt(normal.squaredLength())), standardPoints[1]);
				
				Point3D xAxis = new Point3D(sidePlane.axis[0].x, sidePlane.axis[0].z, -sidePlane.axis[0].y);
				Point3D yAxis = new Point3D(sidePlane.axis[1].x, sidePlane.axis[1].z, -sidePlane.axis[1].y);
				Point3D zAxis = xAxis.cross(yAxis);
				
				Point3D uAxis = new Point3D(xAxis.x, yAxis.x, zAxis.x);
				Point3D vAxis = new Point3D(xAxis.y, yAxis.y, zAxis.y);
				Point3D wAxis = new Point3D(xAxis.z, yAxis.z, zAxis.z);
				
				Dimension dimensions = null;
				
				Texture currentTexture;
				for(int i=0;i<textures.size();i++) {
					currentTexture = textures.elementAt(i);
					if(currentTexture.getFileNameNoExtension().equalsIgnoreCase(sidePlane.textureName)) {
						dimensions = new Dimension(currentTexture.width, currentTexture.height);
					}
				}
				
				if(dimensions == null) {
					System.out.println("ERROR: Unable to locate texture: \"" + sidePlane.textureName + "\".");
					System.exit(1);
				}
				
				Point3D axisScale = new Point3D(1.0 / sidePlane.axisScale[0], -1.0 / sidePlane.axisScale[1], 1);
				Point3D textureTranslation = new Point3D(sidePlane.axisOffset[0], -sidePlane.axisOffset[1], 0);
				Point3D textureScale = new Point3D(1.0 / dimensions.width, 1.0 / dimensions.height, 1);
				
				sidePlane.transformation = Transformation.Identity();
				sidePlane.transformation.rotateToAxes(uAxis, vAxis, wAxis);
				sidePlane.transformation.postScaleBy(axisScale);
				sidePlane.transformation.postTranslateBy(textureTranslation);
				sidePlane.transformation.postScaleBy(textureScale);
			}
			
			tempPlanes.add(sidePlane);
		}
		
		Plane3D plane;
		Transformation t;
		for(int x=0;x<sidePlanes.size();x++) {
			sidePlane = new WorldcraftSidePlane(sidePlanes.elementAt(x));
			plane = sidePlane.plane;
			t = sidePlane.transformation;
			
			if(sidePlane.isDegenerate) { continue; }
			
			Vector<Point3D> largeFacePoints = null;
			Vector<Point3D> cutUpFacePoints = null;
			Vector<Point3D> enlargedFacePoints = null;
			Vector<Point3D> moreAccurateCutUpFacePoints = null;
		}
		
		/*
			//Construct the points for the face coordinates
			PointCollection *largeFacePoints = anyFarCornerPoints (plane);
			PointCollection *cutUpFacePoints = cutUpBy (largeFacePoints, sidePlanes, sidePlaneIndex);
			PointCollection *enlargedFacePoints = enlarge (cutUpFacePoints, plane);
			PointCollection *moreAccurateCutUpFacePoints = cutUpBy (enlargedFacePoints, sidePlanes, sidePlaneIndex);

			deletePointCollection (largeFacePoints); deletePointCollection (cutUpFacePoints);
			deletePointCollection (enlargedFacePoints);

			//Enlarge just by a tiny amount to get rid of pixel leaks...
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
		}*/
		
		return newFaces;
	}
	
	Vector<Point3D> anyFarCornerPoints(Plane3D plane) {
		double x = plane.normal.x;
		double y = plane.normal.y;
		double z = plane.normal.z;
		double d = -plane.minusP0DotNormal;
		double p = 1.0e10;
		double m = -p;
		
		Point3D point[] = new Point3D[4];

		if(Math.abs(x) > Math.abs(y)) {
			if(Math.abs(x) > Math.abs(z)) {
				point[0] = new Point3D((d - (p * y) - (p * z)) / x, p, p);
				point[1] = new Point3D((d - (p * y) - (m * z)) / x, p, m);
				point[2] = new Point3D((d - (m * y) - (m * z)) / x, m, m);
				point[3] = new Point3D((d - (m * y) - (p * z)) / x, m, p);
			}
			else {
				point[0] = new Point3D(p, p, (d - (p * x) - (p * y)) / z);
				point[1] = new Point3D(p, m, (d - (p * x) - (m * y)) / z);
				point[2] = new Point3D(m, m, (d - (m * x) - (m * y)) / z);
				point[3] = new Point3D(m, p, (d - (m * x) - (p * y)) / z);
			}
		}
			else {
				if(Math.abs(y) > Math.abs(z)) {
				point[0] = new Point3D(p, (d - (p * x) - (p * z)) / y, p);
				point[1] = new Point3D(p, (d - (p * x) - (m * z)) / y, m);
				point[2] = new Point3D(m, (d - (m * x) - (m * z)) / y, m);
				point[3] = new Point3D(m, (d - (m * x) - (p * z)) / y, p);
				}
				else {
				point[0] = new Point3D(p, p, (d - (p * x) - (p * y)) / z);
				point[1] = new Point3D(p, m, (d - (p * x) - (m * y)) / z);
				point[2] = new Point3D(m, m, (d - (m * x) - (m * y)) / z);
				point[3] = new Point3D(m, p, (d - (m * x) - (p * y)) / z);
			}
		}
	    
		Vector<Point3D> points = new Vector<Point3D>();
		
		if(isCounterClockwise(plane, point[0], point[1], point[2])) {
			points.add(point[0]);
			points.add(point[1]);
			points.add(point[2]);
			points.add(point[3]);
		}
		else {
			points.add(point[3]);
			points.add(point[2]);
			points.add(point[1]);
			points.add(point[0]);
		}
		
	    return points;
	}
	
	Vector<Point3D> cutUpBy(Vector<Point3D> points, Vector<WorldcraftSidePlane> sidePlanes, int indexOfExcludedPlane) {
		Vector<Point3D> oldPoints = points;
		Plane3D plane, flippedPlane;
		for(int i=0;i<sidePlanes.size();i++) {
			if(sidePlanes.elementAt(i).isDegenerate) { continue; }
			
			plane = sidePlanes.elementAt(i).plane;
			flippedPlane = new Plane3D(new Point3D(-plane.normal.x, -plane.normal.y, -plane.normal.z), -plane.minusP0DotNormal);
			Vector<Point3D> newPoints = clip(flippedPlane, oldPoints);
			oldPoints = newPoints;
		}
		return oldPoints;
	}
	
	Vector<Point3D> enlarge(Vector<Point3D> points, Plane3D plane, double enlargementFactor) {
		Point3D sum = new Point3D();
		for(int i=0;i<points.size();i++) {
			sum.x += points.elementAt(i).x;
			sum.y += points.elementAt(i).y;
			sum.z += points.elementAt(i).z;
		}
		double s = points.size();
		Point3D center = new Point3D(sum.x / s, sum.y / s, sum.z / s);
		
		Vector<Point3D> newPoints = new Vector<Point3D>();
		Point3D newPoint;
		for(int i=0;i<points.size();i++) {
			newPoint = new Point3D(center.x + (points.elementAt(i).x - center.x) * enlargementFactor,
								   center.y + (points.elementAt(i).y - center.y) * enlargementFactor,
								   center.z + (points.elementAt(i).z - center.z) * enlargementFactor);
			newPoints.add(plane.projectionOfPoint(newPoint));
		}
		return newPoints;
	}
	
	boolean isCounterClockwise(Plane3D plane, Point3D p1, Point3D p2, Point3D p3) {
return false;
		/*
		Vector unnormalizedNormal; double squaredLength;
		if (!plane.normalIsValid (point1, point2, point3, unnormalizedNormal, squaredLength)) return false;
		Vector normalizedNormal = unnormalizedNormal * (1.0 / sqrt (squaredLength));
		return (normalizedNormal - plane.normal).squaredLength () < epsilon ();
		*/
	}

	Vector<Point3D> clip(Plane3D plane, Vector<Point3D> candidatePoints) {
		/*
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
		*/
		return new Vector<Point3D>();
	}
	
	public void readFrom(BufferedReader in) throws Exception {
		String input;
		
		// input the face header
		input = in.readLine().trim();
		String faceHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!faceHeader.equalsIgnoreCase("Face")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Face\", found \"" + faceHeader + "\".");
			System.exit(1);
		}
		
		// input the face index
		this.faceIndex = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		if(faceIndex < 0) {
			System.out.println("ERROR: Invalid face index parsed: " + this.faceIndex + "\".");
			System.exit(1);
		}
		
		// input the properties header
		input = in.readLine().trim();
		String propertiesHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!propertiesHeader.equalsIgnoreCase("Properties")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Properties\", found \"" + propertiesHeader + "\".");
			System.exit(1);
		}
		
		// input the properties
		int numberOfProperties = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		this.properties = new Vector<Property>();
		for(int i=0;i<numberOfProperties;i++) {
			this.properties.add(new Property(in));
		}
		
		// input the points header
		input = in.readLine().trim();
		String pointsHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!pointsHeader.equalsIgnoreCase("Points")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Points\", found \"" + pointsHeader + "\".");
			System.exit(1);
		}
		
		// input the points
		int numberOfPoints = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		this.points = new Vector<GamePoint3D>();
		for(int i=0;i<numberOfPoints;i++) {
			this.points.add(new GamePoint3D(in));
		}
		
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		if(properties != null) {
			// output the properties header, followed by all the properties
			out.println("\t\tProperties: " + this.properties.size() + ";");
			for(int i=0;i<this.properties.size();i++) {
				out.println("\t\t\t\"" + this.properties.elementAt(i).key + "\" => \"" + this.properties.elementAt(i).value + "\"");
			}
			
			// output the points header, followed by all the points
			out.println("\t\tPoints: " + this.points.size() + ";");
			for(int i=0;i<this.points.size();i++) {
				out.print("\t\t\t");
				this.points.elementAt(i).writeTo(out);
				out.println(";");
			}
		}
	}
	
	public void writeTo(PrintWriter out, boolean writeProperties) throws Exception {
		if(properties != null) {
			// output the properties header, followed by all the properties
			if(writeProperties && properties.size() != 0) {
				out.println("\t\t\"" + this.properties.elementAt(0).key + "\" => \"" + this.properties.elementAt(0).value + "\"");
			}
			
			// output the points header, followed by all the points
			out.println("\t\tPoints: " + this.points.size() + ";");
			for(int i=0;i<this.points.size();i++) {
				out.print("\t\t\t");
				this.points.elementAt(i).writeTo(out);
				out.println(";");
			}
		}
	}
	
}
