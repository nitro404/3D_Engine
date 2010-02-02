import java.io.File;

public class World implements Map3D {
	
	public World(File file) {
		this.readFrom(file);
		System.out.println("Reading of 3D Engine World formatted files is not currently supported.");
		System.exit(1);
	}
	
	public World(Map3D map) {
		this.convertFrom(map);
	}
	
	public void convertFrom(Map3D map) {
		// TODO Auto-generated method stub

		System.out.println("TODO: Implement World.convertFrom(Map3D map).");
	}
	
	public void readFrom(File file) {
		System.out.println("ERROR: Reading of the 3D Engine Conversion to the Worldcraft Map format is not currently supported.");
		System.exit(1);
	}
	
	public void writeTo(File file) {
		// TODO Auto-generated method stub
		
		System.out.println("TODO: Implement World.writeTo(File file).");
	}
	
}
