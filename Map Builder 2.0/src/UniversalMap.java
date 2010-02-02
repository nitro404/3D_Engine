import java.io.*;
import java.util.Vector;

public class UniversalMap implements Map3D {
	
	Point3D startPosition;
	Vector<UniversalObject> objects;
	
	public UniversalMap(File file) {
		try {
			this.readFrom(file);
		}
		catch (Exception e) {
			System.out.println("ERROR: Error reading from map file " + file.getName() + ".");
			System.exit(1);
		}
	}
	
	public UniversalMap(Map3D map) {
		this.convertFrom(map);
	}
	
	public void convertFrom(Map3D map) {
		// TODO Auto-generated method stub
		
		System.out.println("TODO: Implement UniversalMap.convertFrom(Map3D map).");
	}
	
	public void readFrom(File file) throws Exception {
		BufferedReader in = new BufferedReader(new FileReader(file));
		String input;
		
		// input the header
		String mapHeader = in.readLine().trim();
		if(!mapHeader.equalsIgnoreCase("Universal Object")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Universal Object\", found \"" + mapHeader + "\".");
			System.exit(1);
		}
		
		// input the objects header
		input = in.readLine().trim();
		String objectsHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!objectsHeader.equalsIgnoreCase("Objects")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Objects\", found \"" + objectsHeader + "\".");
			System.exit(1);
		}
		
		// input the objects
		int numberOfObjects = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		this.objects = new Vector<UniversalObject>();
		UniversalObject newObject = null;
		for(int i=0;i<numberOfObjects;i++) {
			newObject = new UniversalObject(in);
			
			if(newObject.getPropertyValue("type").equalsIgnoreCase("info_player_start")) {
				this.startPosition = newObject.normal.getPosition();
			}
			else if(newObject.getPropertyValue("type").equalsIgnoreCase("static geometry") ||
					newObject.getPropertyValue("type").equalsIgnoreCase("environment") ||
					newObject.getPropertyValue("type").equalsIgnoreCase("vehicle") ||
					newObject.getPropertyValue("type").equalsIgnoreCase("rotator") ||
					newObject.getPropertyValue("type").equalsIgnoreCase("translator") ||
					newObject.getPropertyValue("type").equalsIgnoreCase("sprite") ||
					newObject.getPropertyValue("type").equalsIgnoreCase("waypoint") ||
					newObject.getPropertyValue("type").equalsIgnoreCase("pool")) {
				this.objects.add(newObject);
			}
			else {
				System.out.println("WARNING: Ignoring unexpected object of type \"" + newObject.getPropertyValue("type") + "\".");
			}
		}
	}
	
	public void writeTo(File file) {
		// TODO Auto-generated method stub
		
		System.out.println("TODO: Implement UniversalMap.writeTo(File file).");
	}
	
}
