import java.util.Vector;
import java.io.*;

public class World implements Map3D {
	
	public Point3D startPosition;
	public Vector<String> textureNames;
	public Vector<UniversalObject> objects;
	
	public World(File file) {
		try {
			this.readFrom(file);
		}
		catch (Exception e) {
			System.out.println("ERROR: Error reading from map file " + file.getName() + ".");
			System.exit(1);
		}
	}
	
	public World(Map3D map) {
		try {
			this.convertFrom(map);
		}
		catch (Exception e) {
			System.out.println("ERROR: Error converting map.");
			System.exit(1);
		}
	}
	
	public void convertFrom(Map3D map) throws Exception {
		// convert the map based on what map type it is an instance of
		if(map instanceof WorldcraftMap) {
			System.out.println("ERROR: Conversion from Worldcraft format maps to 3D Engine Worlds is not currently implemented.");
			System.exit(1);
		}
		else if(map instanceof UniversalMap) {
			UniversalMap universalMap = (UniversalMap) map;
			UniversalObject object;
			this.startPosition = null;
			objects = new Vector<UniversalObject>();
			
			// collect a list of all the texture names in the map
			// and assign index pointers in place of where the texture name used to be 
			textureNames = new Vector<String>();
			for(int i=0;i<universalMap.objects.size();i++) {
				object = universalMap.objects.elementAt(i);
				String type = object.getPropertyValue("type");
				object.removeProperty("type");
				
				boolean duplicateTextureName;
				Face face;
				String textureName;
				int textureIndex = 0;
				for(int j=0;j<object.faces.size();j++) {
					face = object.faces.elementAt(j);
					duplicateTextureName = false;
					textureName = face.getPropertyValue("texture");
					for(int k=0;k<textureNames.size();k++) {
						if(textureNames.elementAt(k).equalsIgnoreCase(textureName)) {
							duplicateTextureName = true;
							textureIndex = k;
							break;
						}
					}
					
					if(!duplicateTextureName) {
						textureNames.add(textureName);
						textureIndex = textureNames.size() - 1;
					}
					face.setPropertyValue("texture", Integer.toString(textureIndex));
				}				
				
				// add the appropriate subclass of UniversalObject based on the object type to a collection of objects
				if(type == null) {
					System.out.println("ERROR: Invalid object found - all universal objects must have a \"type\" property.");
					System.exit(1);
				}
				else if(type.equalsIgnoreCase("info_player_start")) {
					this.startPosition = object.normal.getPosition();
				}
				else if(type.equalsIgnoreCase("static geometry")) {
					objects.add(new Geometry(object, textureIndex));
				}
//				else if(type.equalsIgnoreCase("environment")) {
//					objects.add(object);
//				}
				else if(type.equalsIgnoreCase("vehicle")) {
					objects.add(new Vehicle(object, textureIndex));
				}
				else if(type.equalsIgnoreCase("rotator")) {
					objects.add(new Rotator(object, textureIndex));
				}
				else if(type.equalsIgnoreCase("translator")) {
					objects.add(new Translator(object, textureIndex));
				}
				else if(type.equalsIgnoreCase("sprite")) {
					objects.add(new Sprite(object, textureIndex));
				}
				else if(type.equalsIgnoreCase("waypoint")) {
					objects.add(new Waypoint(object));
				}
				else if(type.equalsIgnoreCase("pool")) {
					objects.add(new Pool(object, textureIndex));
				}
				else {
					System.out.println("WARNING: Ignoring unexpected object of type \"" + type + "\".");
				}
			}
			
		}
		else if(map instanceof World) {
			return;
		}
		else {
			System.out.println("ERROR: Invalid map type to convert from.");
			System.exit(1);
		}
	}
	
	public void readFrom(File file) throws Exception {
		System.out.println("ERROR: Reading of the 3D Engine Conversion to the Worldcraft Map format is not currently supported.");
		System.exit(1);
	}
	
	public void writeTo(File file) throws Exception {
		PrintWriter out = new PrintWriter(new FileWriter(file));
		
		// print the world header
		out.println("World");
		
		// print the start position
		out.print("Start position: ");
		this.startPosition.writeTo(out);
		out.println(";");
		
		//print the textures header, followed by the textures
		out.println("Textures: " + this.textureNames.size() + ";");
		for(int i=0;i<this.textureNames.size();i++) {
			out.println("\t" + this.textureNames.elementAt(i));
		}
		
		// print the objects header, followed by the objects
		out.println("Objects: " + this.objects.size() + ";");
		for(int i=0;i<this.objects.size();i++) {
			out.println("Object: " + i + ";");
			this.objects.elementAt(i).writeTo(out);
		}
		
		out.close();
	}
	
}