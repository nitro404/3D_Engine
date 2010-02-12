// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.io.File;

public class WorldcraftMap implements Map3D {
	
	public WorldcraftMap(File file) {
		try {
			this.readFrom(file);
		}
		catch (Exception e) {
			System.out.println("ERROR: Error reading from map file " + file.getName() + ".");
			e.printStackTrace();
			System.exit(1);
		}
	}

	public WorldcraftMap(Map3D map) {
		try {
			this.convertFrom(map);
		}
		catch (Exception e) {
			System.out.println("ERROR: Error converting map.");
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	public void convertFrom(Map3D map) throws Exception {
		System.out.println("ERROR: Conversion to the Worldcraft Map format is not currently supported.");
		System.exit(1);
	}
	
	public void readFrom(File file) throws Exception {
		// TODO Auto-generated method stub
		
		System.out.println("TODO: Implement WorldcraftMap.readFrom(File file).");
	}
	
	public void writeTo(File file) throws Exception {
		System.out.println("ERROR: Writing of the Worldcraft Map format to file is not currently supported.");
		System.exit(1);
	}
	
}
