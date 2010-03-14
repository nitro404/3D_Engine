// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.util.Vector;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class WorldcraftMap implements Map3D {
	
	public Vector<WorldcraftGroup> groups;
	
	public WorldcraftMap(File file) {
		this.groups = new Vector<WorldcraftGroup>();
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
	
	/*public static Vector<WorldcraftGroup> mergeObjects(Vector<WorldcraftGroup> oldGroups) {
		Vector<WorldcraftGroup> newGroups = new Vector<WorldcraftGroup>();
		
		for(int i=0;i<oldGroups.size();i++) {
			WorldcraftGroup newGroup = new WorldcraftGroup();
			for(int j=0;j<oldGroups.elementAt(i).properties.size();j++) {
				Property p = oldGroups.elementAt(i).properties.elementAt(j);
				newGroup.addProperty(p.key, p.value);
			}
			WorldcraftObject newObject = new WorldcraftObject();
			for(int j=0;j<oldGroups.elementAt(i).objects.size();j++) {
				WorldcraftObject oldObject = oldGroups.elementAt(i).objects.elementAt(j);
				for(int k=0;k<oldObject.sidePlanes.size();k++) {
					newObject.addPlane(oldObject.sidePlanes.elementAt(k));
				}
			}
			newGroups.add(newGroup);
		}
		
		return newGroups;
	}*/
	
	public void convertFrom(Map3D map) throws Exception {
		System.out.println("ERROR: Conversion to the Worldcraft Map format is not currently supported.");
		System.exit(1);
	}
	
	public void readFrom(File file) throws Exception {
		this.groups = new Vector<WorldcraftGroup>();
		BufferedReader in = new BufferedReader(new FileReader(file));
		String input;
		
		while((input = in.readLine()) != null) {
			if(input.equals("{")) {
				groups.add(new WorldcraftGroup(in));
			}
			else {
				System.out.println("ERROR: Expected \"{\", found \"" + input.charAt(0) + "\" for input: " + input + ".");
				System.exit(1);
			}
		}
		
	}
	
	public void writeTo(File file) throws Exception {
		System.out.println("ERROR: Writing of the Worldcraft Map format to file is not currently supported.");
		System.exit(1);
	}
	
}
