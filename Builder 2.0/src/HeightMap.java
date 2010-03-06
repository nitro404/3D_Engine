import java.io.BufferedReader;
import java.util.Vector;
import java.io.PrintWriter;

public class HeightMap {
	
	public int index;
	public String name;
	public int heightMapFileIndex;
	public int width;
	public int height;
	
	public HeightMap() { }
	
	public HeightMap(int index, String name, String heightMapName, int width, int height, Vector<String> heightMapNames) {
		this.index = index;
		this.name = name;
		this.width = width;
		this.height = height;
		for(int i=0;i<heightMapNames.size();i++) {
			if(heightMapNames.elementAt(i).equalsIgnoreCase(heightMapName)) {
				this.heightMapFileIndex = i;
				break;
			}
		}
	}
	
	public HeightMap(BufferedReader in) {
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid height map in texture data file.");
			System.exit(1);
		}
	}
	
	public void readFrom(BufferedReader in) throws Exception {
		String input;
		
		// input the height map index
		input = in.readLine().trim();
		this.index = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		if(this.index < 0) {
			System.out.println("ERROR: Invalid height map index parsed: " + this.index + "\".");
			System.exit(1);
		}
		
		// input the properties header
		input = in.readLine().trim();
		String propertyHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!propertyHeader.equalsIgnoreCase("Properties")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Properties\", found \"" + propertyHeader + "\".");
			System.exit(1);
		}
		
		// input the properties
		int numberOfProperties = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		Property newProperty;
		for(int i=0;i<numberOfProperties;i++) {
			newProperty = new Property(in);
			if(newProperty.key.equalsIgnoreCase("name")) {
				this.name = newProperty.value;
			}
			else if(newProperty.key.equalsIgnoreCase("texturemap")) {
				this.heightMapFileIndex = Integer.valueOf(newProperty.value);
			}
			else if(newProperty.key.equalsIgnoreCase("width")) {
				this.width = Integer.valueOf(newProperty.value);
			}
			else if(newProperty.key.equalsIgnoreCase("height")) {
				this.height = Integer.valueOf(newProperty.value);
			}
		}
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		out.println("\tHeightMap: " + index + ";");
		out.println("\t\tProperties: 4;");
		out.println("\t\t\t\"name\" => \"" + name + "\"");
		out.println("\t\t\t\"heightmap\" => \"" + heightMapFileIndex + "\"");
		out.println("\t\t\t\"width\" => \"" + width + "\"");
		out.println("\t\t\t\"height\" => \"" + height + "\"");
	}
	
}
