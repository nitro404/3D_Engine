import java.util.Vector;
import java.util.StringTokenizer;
import java.io.*;

public class WorldcraftGroup {
	
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

	public void addProperty(String key, String value) {
		this.addProperty(new Property(key, value));
	}
	
	public void addProperty(Property property) {
		if(property == null || property.key == null || property.value == null) { return; }
		
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(property.key)) {
				return;
			}
		}
		
		this.properties.add(property);
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
