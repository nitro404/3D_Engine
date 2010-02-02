import java.util.Vector;
import java.io.*;

public class UniversalObject {
	
	int objectIndex;
	public Transformation normal;
	public Transformation inverse;
	public Vector<Property> properties;
	public Vector<Face> faces;
	
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
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(key)) {
				return this.properties.elementAt(i).value;
			}
		}
		return null;
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
	
	
}
