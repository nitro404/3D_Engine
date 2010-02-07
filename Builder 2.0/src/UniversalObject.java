import java.util.Vector;
import java.io.*;

public class UniversalObject {
	
	public int objectIndex;
	public Transformation normal;
	public Transformation inverse;
	public Vector<Property> properties;
	public Vector<Face> faces;
	
	public UniversalObject() { }
	
	public UniversalObject(int objectIndex, Transformation normal, Transformation inverse, Vector<Property> properties, Vector<Face> faces) {
		this.objectIndex = objectIndex;
		this.normal = normal;
		this.inverse = inverse;
		this.properties = properties;
		this.faces = faces;
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
	
	private void readFrom(BufferedReader in) throws Exception {
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
		this.properties = new Vector<Property>();
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
		this.faces = new Vector<Face>();
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
		// output the object type based on which subclass of UniversalObject it is
			 if(this instanceof Geometry)	{ out.println("\t\"type\" => \"static geometry\""); }
		else if(this instanceof Environment){ out.println("\t\"type\" => \"environment\""); }
		else if(this instanceof Vehicle)	{ out.println("\t\"type\" => \"vehicle\""); }
		else if(this instanceof Rotator)	{ out.println("\t\"type\" => \"rotator\""); }
		else if(this instanceof Translator)	{ out.println("\t\"type\" => \"translator\""); }
		else if(this instanceof Sprite)		{ out.println("\t\"type\" => \"sprite\""); }
		else if(this instanceof Waypoint)	{ }
		else if(this instanceof Pool)		{ out.println("\t\"type\" => \"pool\""); }
		else   								{ System.out.println("WARNING: Ignoring unexpected object of type \"" + this.getPropertyValue("type") + "\"."); }
		
		// output the transformations
//		out.println("\tTransformation: //m11 m12 m13 m14 m21 m22 ... m44");
		out.println("\tTransformation:");
		out.print("\t\t");
		this.normal.writeTo(out);
//		out.println("; //Standard...");
		out.println(";");
		out.print("\t\t");
		this.inverse.writeTo(out);
//		out.println("; //Inverse...");
		out.println(";");
		
		// output the properties
		if(this.properties != null) {
			out.println("\tProperties: " + this.properties.size() + ";");
			for(int i=0;i<this.properties.size();i++) {
				out.println("\t\t\"" + this.properties.elementAt(i).key + "\" => \"" + this.properties.elementAt(i).value + "\"");
			}
		}
		
		// output the faces
		if(this.faces != null) {
			out.println("\tFaces: " + this.faces.size() + ";");
			for(int i=0;i<this.faces.size();i++) {
				out.println("\tFace: " + i + ";");
				this.faces.elementAt(i).writeTo(out, !(this instanceof Sprite));
			}
		}
	}
	
}
