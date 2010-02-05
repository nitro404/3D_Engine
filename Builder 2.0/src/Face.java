import java.io.*;
import java.util.Vector;

public class Face {
	
	public int faceIndex;
	public Vector<Property> properties;
	public Vector<GamePoint3D> points;
	
	public Face(BufferedReader in) {
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid face in map file.");
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
			out.println("\t\tPoints: " + this.points.size() + "; //x, y, z, nx, ny, nz, tx, ty");
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
			if(writeProperties) {
				out.println("\t\tProperties: " + this.properties.size() + ";");
				for(int i=0;i<this.properties.size();i++) {
					out.println("\t\t\t\"" + this.properties.elementAt(i).key + "\" => \"" + this.properties.elementAt(i).value + "\"");
				}
			}
			
			// output the points header, followed by all the points
			out.println("\t\tPoints: " + this.points.size() + "; //x, y, z, nx, ny, nz, tx, ty");
			for(int i=0;i<this.points.size();i++) {
				out.print("\t\t\t");
				this.points.elementAt(i).writeTo(out);
				out.println(";");
			}
		}
	}
	
}
