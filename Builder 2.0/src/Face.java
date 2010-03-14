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
		
		WorldcraftSidePlane sidePlane;
		for(int x=0;x<sidePlanes.size();x++) {
			sidePlane = sidePlanes.elementAt(x);
			
			Face newFace = new Face();
			newFace.faceIndex = faceIndex++;
			
			Point3D[] standardPoints = new Point3D[3];
			for(int i=0;i<3;i++) {
				standardPoints[i] = new Point3D(sidePlane.points[i].x, sidePlane.points[i].z, -sidePlane.points[i].y);
			}
			
			Point3D normal = standardPoints[0].minus(standardPoints[1]).cross(standardPoints[2].minus(standardPoints[0]));
			
			if(!(normal.squaredLength() > 1.0e-10)) {
				System.out.println("WARNING: Ignoring zero-area triangle: (" + standardPoints[0] + "), (" + standardPoints[1] + "), (" + standardPoints[2] + ")");
				sidePlane.planeIsDegenerate = true;
			}
			
			Plane3D plane = new Plane3D(normal.multiplyBy(1.0 / Math.sqrt(normal.squaredLength())), standardPoints[1]);
			
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
			
			sidePlane.transformation = new Transformation();
			sidePlane.transformation.rotateToAxes(uAxis, vAxis, wAxis);
			sidePlane.transformation.postScaleBy(axisScale);
			sidePlane.transformation.postTranslateBy(textureTranslation);
			sidePlane.transformation.postScaleBy(textureScale);
		}
		
		return newFaces;
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
