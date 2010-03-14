// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.io.*;
import java.util.Vector;

public class UniversalMap implements Map3D {
	
	public Vector<UniversalObject> objects;
	
	private Vector<Texture> textures;
	
	public UniversalMap(File file) {
		this.textures = null;
		try {
			this.readFrom(file);
		}
		catch (Exception e) {
			System.out.println("ERROR: Error reading from map file " + file.getName() + ".");
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	public UniversalMap(Map3D map, File textureDirectory, Vector<Texture> textures) {
		this.textures = textures;
		
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
		this.objects = new Vector<UniversalObject>();
		
		if(map instanceof WorldcraftMap) {
			WorldcraftMap worldcraftMap = (WorldcraftMap) map;
			
			int currentObjectIndex = -1;
			for(int i=0;i<worldcraftMap.groups.size();i++) {
				WorldcraftGroup wcg = worldcraftMap.groups.elementAt(i);
				
				String type = wcg.getPropertyValue("classname");
				String name = wcg.getPropertyValue("targetname");
				if(type == null) {
					System.out.println("ERROR: Worldcraft Group missing classname.");
					System.exit(1);
				}
				if(name == null) {
					name = "";
				}
				
				UniversalObject lastObject = null;
				int currentFaceIndex = -1;
				for(int j=0;j<wcg.objects.size();j++) {
					WorldcraftObject wco = wcg.objects.elementAt(j);
					UniversalObject uo = null;
					if(lastObject == null || name.length() == 0 || !lastObject.getPropertyValue("name").equalsIgnoreCase(name)) {
						uo = new UniversalObject();
						if(type.equalsIgnoreCase("worldspawn")) { uo.addProperty("type", "static geometry"); }
						else { uo.addProperty("type", type.toLowerCase()); }
						uo.addProperty("name", name);
						
						for(int k=0;k<wcg.properties.size();k++) {
							Property p = wcg.properties.elementAt(k);
							if(!(p.key.equalsIgnoreCase("classname") && !p.key.equalsIgnoreCase("targetname"))) {
								uo.addProperty(p);
							}
						}
						
						currentFaceIndex = -1;
						lastObject = null;
					}
					else {
						uo = lastObject;
					}
					
					Vector<Face> faces = Face.convertFrom(wco.sidePlanes, currentFaceIndex, textures);
					if(faces.size() > 0) { currentFaceIndex = faces.elementAt(faces.size() - 1).faceIndex; }
					
					if(lastObject == null) {
						uo.objectIndex = currentObjectIndex++;
						this.objects.add(uo);
						lastObject = uo;
					}
				}
			}
		}
		else if(map instanceof UniversalMap) {
			this.objects = ((UniversalMap) map).objects;
		}
		else if(map instanceof World) {
			System.out.println("ERROR: Conversion from 3D Engine Worlds to Universal Maps is not currently implemented.");
			System.exit(1);
		}
		else {
			System.out.println("ERROR: Invalid map type to convert from.");
			System.exit(1);
		}
	}
	
	public void readFrom(File file) throws Exception {
		this.objects = new Vector<UniversalObject>();
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
		for(int i=0;i<numberOfObjects;i++) {
			this.objects.add(new UniversalObject(in));
		}
		
		if(in != null) { in.close(); }
	}
	
	public void writeTo(File file) throws Exception {
		PrintWriter out = new PrintWriter(new FileWriter(file));
		
		out.println("Universal Object");
		
		out.println("Objects: " + objects.size() + ";");
		
		for(int i=0;i<objects.size();i++) {
			objects.elementAt(i).writeTo(out);
		}
		
		out.close();
	}
	
}
