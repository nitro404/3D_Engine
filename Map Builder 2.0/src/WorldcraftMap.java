import java.io.File;

public class WorldcraftMap implements Map3D {
	
	public WorldcraftMap(File file) {
		try {
			this.readFrom(file);
		}
		catch (Exception e) {
			System.out.println("ERROR: Error reading from map file " + file.getName() + ".");
			System.exit(1);
		}
	}

	public WorldcraftMap(Map3D map) {
		this.convertFrom(map);
	}
	
	public void convertFrom(Map3D map) {
		System.out.println("ERROR: Conversion to the Worldcraft Map format is not currently supported.");
		System.exit(1);
	}
	
	public void readFrom(File file) {
		// TODO Auto-generated method stub
		
		System.out.println("TODO: Implement WorldcraftMap.readFrom(File file).");
	}
	
	public void writeTo(File file) {
		System.out.println("ERROR: Writing of the Worldcraft Map format to file is not currently supported.");
		System.exit(1);
	}
	
}
