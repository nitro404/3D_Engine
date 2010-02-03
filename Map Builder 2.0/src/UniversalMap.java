import java.io.*;
import java.util.Vector;

public class UniversalMap implements Map3D {
	
	public Vector<UniversalObject> objects;
	
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
		try {
			this.convertFrom(map);
		}
		catch (Exception e) {
			System.out.println("ERROR: Error converting map.");
			System.exit(1);
		}
	}
	
	public void convertFrom(Map3D map) throws Exception {
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
		String type;
		for(int i=0;i<numberOfObjects;i++) {
			newObject = new UniversalObject(in);
			type = newObject.getPropertyValue("type");
			
			if(type.equalsIgnoreCase("info_player_start") ||
			   type.equalsIgnoreCase("static geometry") ||
			   type.equalsIgnoreCase("environment") ||
			   type.equalsIgnoreCase("vehicle") ||
			   type.equalsIgnoreCase("rotator") ||
			   type.equalsIgnoreCase("translator") ||
			   type.equalsIgnoreCase("sprite") ||
			   type.equalsIgnoreCase("waypoint") ||
			   type.equalsIgnoreCase("pool")) {
				this.objects.add(newObject);
			}
			else {
				System.out.println("WARNING: Ignoring unexpected object of type \"" + newObject.getPropertyValue("type") + "\".");
			}
		}
		
		if(in != null) { in.close(); }
	}
	
	public void writeTo(File file) throws Exception {
		// TODO Auto-generated method stub
		
		System.out.println("TODO: Implement UniversalMap.writeTo(File file).");
	}
	
}
