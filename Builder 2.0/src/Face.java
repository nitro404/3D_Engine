// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.awt.Dimension;
import java.io.*;
import java.util.Vector;

public class Face extends Properties {
	
	public int faceIndex;
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
			
			Point3D normal = standardPoints[0].subtract(standardPoints[1]).cross(standardPoints[2].subtract(standardPoints[0]));
			
			if(!(normal.squaredLength() > 1.0e-10)) {
				System.out.println("WARNING: Ignoring zero-area triangle: (" + standardPoints[0] + "), (" + standardPoints[1] + "), (" + standardPoints[2] + ")");
				sidePlane.isDegenerate = true;
			}
			else {
				sidePlane.plane = new Plane3D(normal.multiply(1.0 / Math.sqrt(normal.squaredLength())), standardPoints[1]);
				
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
		
//		double enlargementForScalingOnPlane = 1.0001;
//		double enlargementTranslatingOnPlane = 0.0001;
//		double enlargementForMovingPlaneOut = 0.015;
		
//		NOTE: sidePlanes == tempPlanes #################################################################
		
		Plane3D plane;
		Transformation t;
//		for(int x=0;x<sidePlanes.size();x++) {
//			sidePlane = new WorldcraftSidePlane(sidePlanes.elementAt(x));
		for(int x=0;x<tempPlanes.size();x++) {
		sidePlane = new WorldcraftSidePlane(tempPlanes.elementAt(x));
			plane = sidePlane.plane;
			t = sidePlane.transformation;
			
			if(sidePlane.isDegenerate) { continue; }
			
			Vector<Point3D> largeFacePoints = anyFarCornerPoints(plane);
/*for(int v=0;v<largeFacePoints.size();v++) {
System.out.println(largeFacePoints.elementAt(v));
}*/
			Vector<Point3D> cutUpFacePoints = cutUpBy(largeFacePoints, tempPlanes, x);
for(int u=0;u<cutUpFacePoints.size();u++) {
System.out.println(cutUpFacePoints.elementAt(u));
}
			Vector<Point3D> enlargedFacePoints = enlarge(cutUpFacePoints, plane, 2);
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
	
	public static Vector<Point3D> anyFarCornerPoints(Plane3D plane) {
		double x = plane.normal.x;
		double y = plane.normal.y;
		double z = plane.normal.z;
		double d = -plane.minusP0DotNormal;
//		double p = 1.0e10;
		double p = 10000000000.0;
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
	
	private static Vector<Point3D> cutUpBy(Vector<Point3D> points, Vector<WorldcraftSidePlane> sidePlanes, int indexOfExcludedPlane) {
		Vector<Point3D> oldPoints = points;
		Plane3D plane, flippedPlane;
		for(int i=0;i<sidePlanes.size();i++) {
			if(i != indexOfExcludedPlane) {
				if(sidePlanes.elementAt(i).isDegenerate) { continue; }
				
				plane = sidePlanes.elementAt(i).plane;
				flippedPlane = new Plane3D(new Point3D(plane.normal.negative()), -(plane.minusP0DotNormal));
				Vector<Point3D> newPoints = clip(flippedPlane, oldPoints);
				if(oldPoints != points) {
					oldPoints.clear();
				}
				oldPoints = newPoints;
			}
		}
		return oldPoints;
	}
	
	/*private static Vector<Point3D> cutUpBy(Vector<Point3D> points, Vector<WorldcraftSidePlane> sidePlanes, int indexOfExcludedPlane) {
		Vector<Point3D> oldPoints = points;
		Plane3D plane, flippedPlane;
		for(int i=0;i<sidePlanes.size();i++) {
			if(sidePlanes.elementAt(i).isDegenerate) { continue; }
			
			plane = sidePlanes.elementAt(i).plane;
			flippedPlane = new Plane3D(plane.normal.negative(), -plane.minusP0DotNormal);
//System.out.println(flippedPlane.normal + " : " + flippedPlane.minusP0DotNormal);
			
			Vector<Point3D> newPoints = clip(flippedPlane, oldPoints);
			oldPoints = newPoints;
		}
		return oldPoints;
	}*/
	
	private static Vector<Point3D> enlarge(Vector<Point3D> points, Plane3D plane, double enlargementFactor) {
		Point3D sum = new Point3D();
		for(int i=0;i<points.size();i++) {
			sum = sum.add(points.elementAt(i));
		}
		double s = points.size();
		Point3D center = new Point3D(sum.divide(s));
		
		Vector<Point3D> newPoints = new Vector<Point3D>();
		Point3D newPoint;
		for(int i=0;i<points.size();i++) {
			newPoint = new Point3D(center.add(points.elementAt(i).subtract(center).multiply(enlargementFactor))); 
//			newPoint = new Point3D(center.x + (points.elementAt(i).x - center.x) * enlargementFactor,
//								   center.y + (points.elementAt(i).y - center.y) * enlargementFactor,
//								   center.z + (points.elementAt(i).z - center.z) * enlargementFactor);
//System.out.println(newPoint);
			newPoints.add(plane.projectionOfPoint(newPoint));
		}
		return newPoints;
	}
	
	private static boolean isCounterClockwise(Plane3D plane, Point3D p1, Point3D p2, Point3D p3) {
		Point3D normal = p2.subtract(p1).cross(p3.subtract(p2));
		if(!(normal.squaredLength() > 1.0e-10)) { return false; }
		Point3D normalizedNormal = normal.multiply(1.0 / Math.sqrt(normal.squaredLength()));
		return normalizedNormal.subtract(plane.normal).squaredLength() < 1.0e-10;
	}

	private static Vector<Point3D> clip(Plane3D plane, Vector<Point3D> candidatePoints) {
		Vector<Point3D> points = new Vector<Point3D>();
		if(candidatePoints == null || candidatePoints.size() == 0) { return points; }
		
		for(int i=0;i<candidatePoints.size();i++) {
			Point3D p1 = candidatePoints.elementAt(i);
			Point3D p2 = candidatePoints.elementAt(i == candidatePoints.size() - 1 ? 0 : i + 1);
			
			int lineSign = plane.whereIsLine(p1, p2);
			
			switch(lineSign) {
				case Plane3D.LINE_BACK_SIGN:
					break;
					
				case Plane3D.LINE_ON_SIGN:
				case Plane3D.LINE_FRONT_SIGN:
					points.add(new Point3D(p1)); 
					break;
					
				case Plane3D.LINE_STRADDLES_SIGN:
					int s1 = plane.whereIsPoint(p1);
					int s2 = plane.whereIsPoint(p2);
					
					if(s1 == Plane3D.POINT_POSITIVE_SIGN || s1 == Plane3D.POINT_ZERO_SIGN) {
						points.add(new Point3D(p1));
					}
					
					if(s1 == Plane3D.POINT_POSITIVE_SIGN && s2 == Plane3D.POINT_NEGATIVE_SIGN) {
						Point3D intersection = plane.intersectionOf(p1, p2);
						points.add(intersection);
					}
					else if(s1 == Plane3D.POINT_NEGATIVE_SIGN && s2 == Plane3D.POINT_POSITIVE_SIGN) {
						Point3D intersection = plane.intersectionOf(p2, p1);
						points.add(intersection);
					}
					break;
				
			}
		}
		
		Vector<Point3D> trimmedPoints = new Vector<Point3D>();
		for(int i=0;i<points.size();i++) {
			Point3D previousPoint = points.elementAt(i == 0 ? points.size() - 1 : i - 1);
			Point3D point = points.elementAt(i);
			
			if(!(point.squaredDistanceTo(previousPoint) < 1.0e-10)) {
				trimmedPoints.add(new Point3D(point));
			}
		}
		
		if(trimmedPoints.size() >= 3) {
			return trimmedPoints;
		}
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
