// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.util.Vector;
import java.io.*;

public class UniversalObject extends Properties {
	
	public int objectIndex;
	public Transformation normal;
	public Transformation inverse;
	public Vector<Face> faces;
	
	public UniversalObject() {
		this.properties = new Vector<Property>();
		this.faces = new Vector<Face>();
	}
	
	public UniversalObject(BufferedReader in) {
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid object in map file.");
			System.exit(1);
		}
	}
	
	private void readFrom(BufferedReader in) throws Exception {
		this.properties = new Vector<Property>();
		this.faces = new Vector<Face>();
		String input;
		
		// input the object header
		input = in.readLine().trim();
		String objectHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!objectHeader.equalsIgnoreCase("Object")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Object\", found \"" + objectHeader + "\".");
			System.exit(1);
		}
		
		// input the object index
		this.objectIndex = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		if(objectIndex < 0) {
			System.out.println("ERROR: Invalid object index parsed: " + this.objectIndex + "\".");
			System.exit(1);
		}
		
		// input the transformation header
		input = in.readLine().trim();
		String transformationHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!transformationHeader.equalsIgnoreCase("Transformation")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Transformation\", found \"" + transformationHeader + "\".");
			System.exit(1);
		}
		
		// input the transformations (normal, inverse)
		this.normal = new Transformation(in);
		this.inverse = new Transformation(in);
		
		// input the properties header
		input = in.readLine().trim();
		String propertyHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!propertyHeader.equalsIgnoreCase("Properties")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Properties\", found \"" + propertyHeader + "\".");
			System.exit(1);
		}
		
		// input the properties
		int numberOfProperties = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		for(int i=0;i<numberOfProperties;i++) {
			this.properties.add(new Property(in));
		}
		
		// input the faces header
		input = in.readLine().trim();
		String facesHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!facesHeader.equalsIgnoreCase("Faces")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Faces\", found \"" + facesHeader + "\".");
			System.exit(1);
		}
		
		// input the faces
		int numberOfFaces = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		for(int i=0;i<numberOfFaces;i++) {
			this.faces.add(new Face(in));
		}
		
		// input the sub-objects header
		input = in.readLine().trim();
		String subObjectsHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!subObjectsHeader.equalsIgnoreCase("Sub-objects")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Sub-objects\", found \"" + subObjectsHeader + "\".");
			System.exit(1);
		}
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		// output the object header
		out.println("Object: " + this.objectIndex + ";");
		
		// output the transformations
		if(this.normal != null && this.inverse != null) {
			out.println("\tTransformation:");
			out.print("\t\t");
			this.normal.writeTo(out);
			out.println(";");
			out.print("\t\t");
			this.inverse.writeTo(out);
			out.println(";");
		}
		
		// output the properties
		if(this.properties != null) {
			out.println("\tProperties: " + this.properties.size() + ";");
			for(int i=0;i<this.properties.size();i++) {
				out.println("\t\t\"" + this.properties.elementAt(i).key + "\" => \"" + this.properties.elementAt(i).value + "\"");
			}
		}
		
		// output the faces
		out.println("\tFaces: " + this.faces.size() + ";");
		for(int i=0;i<this.faces.size();i++) {
			out.println("\tFace: " + i + ";");
			this.faces.elementAt(i).writeTo(out);
		}
		
		out.println("\tSub-objects: 0;");
	}
	
}
