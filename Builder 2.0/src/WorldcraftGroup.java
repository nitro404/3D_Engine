import java.util.Vector;
import java.util.StringTokenizer;
import java.io.*;

public class WorldcraftGroup extends Properties {
	
	public Vector<Property> properties;
	public Vector<WorldcraftObject> objects;

	public WorldcraftGroup() {
		this.objects = new Vector<WorldcraftObject>();
		this.properties = new Vector<Property>();
	}
	
	public WorldcraftGroup(BufferedReader in) {
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid worldcraft group in map file.");
			System.exit(1);
		}
	}
	
	private void readFrom(BufferedReader in) throws Exception {
		this.objects = new Vector<WorldcraftObject>();
		this.properties = new Vector<Property>();
		String input;
		
		while((input = in.readLine()) != null) {
			if(input.charAt(0) == '\"') {
				StringTokenizer propertyTokenizer = new StringTokenizer(input, "\"", false);
				if(propertyTokenizer.countTokens() != 3) {
					System.out.println("ERROR: Expected property name and value, found \"" + input + "\".");
					System.exit(1);
				}
				String name = propertyTokenizer.nextToken().trim().toLowerCase();
				propertyTokenizer.nextToken();
				String value = propertyTokenizer.nextToken().trim();
				addProperty(name, value);
			}
			else if(input.charAt(0) == '{') {
				WorldcraftObject o = new WorldcraftObject();
				
				while((input = in.readLine()) != null) {
					if(input.charAt(0) == '(') {
						o.addPlane(new WorldcraftSidePlane(input));
					}
					else if(input.charAt(0) == '}') {
						break;
					}
					else {
						System.out.println("ERROR: Expected one of \"(, }\", found \"" + input.charAt(0) + "\" for input: \"" + input + "\".");
						System.exit(1);
					}
				}
				
				objects.add(o);
			}
			else if(input.charAt(0) == '}') {
				break;
			}
			else {
				System.out.println("ERROR: Expected one of \"{, \", }\", found \"" + input.charAt(0) + "\" for input: \"" + input + "\".");
				System.exit(1);
			}
		}
		
	}
	
}
