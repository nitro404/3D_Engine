import java.io.File;
import java.util.Vector;

public class World implements Map3D {
	
	public Point3D startPosition;
	public Vector<String> textureNames;
	public Vector<UniversalObject> objects;
	
	public World(File file) {
		this.readFrom(file);
		System.out.println("Reading of 3D Engine World formatted files is not currently supported.");
		System.exit(1);
	}
	
	public World(Map3D map) {
		this.convertFrom(map);
	}
	
	public void convertFrom(Map3D map) {
		if(map instanceof WorldcraftMap) {
			System.out.println("ERROR: Conversion from Worldcraft format maps to 3D Engine Worlds is not currently implemented.");
			System.exit(1);
		}
		else if(map instanceof UniversalMap) {
			UniversalMap universalMap = (UniversalMap) map;
			UniversalObject object;
			this.startPosition = null;
			objects = new Vector<UniversalObject>();
			
			for(int i=0;i<universalMap.objects.size();i++) {
				object = universalMap.objects.elementAt(i);
				String type = object.getPropertyValue("type");
				object.removeProperty("type");
				
				textureNames = new Vector<String>();
				boolean duplicateTextureName;
				Face face;
				for(int j=0;j<object.faces.size();j++) {
					face = object.faces.elementAt(j);
					duplicateTextureName = false;
					for(int k=0;k<textureNames.size();k++) {
						if(textureNames.elementAt(k).equalsIgnoreCase(face.textureName)) {
							duplicateTextureName = true;
						}
					}
					if(!duplicateTextureName) {
						textureNames.add(face.textureName);
					}
				}				
				
				if(type == null) {
					System.out.println("ERROR: Invalid object found - all universal objects must have a \"type\" property.");
					System.exit(1);
				}
				else if(type.equalsIgnoreCase("info_player_start")) {
					this.startPosition = object.normal.getPosition();
				}
				else if(type.equalsIgnoreCase("static geometry")) {
					objects.add(new Geometry(object));
				}
//				else if(type.equalsIgnoreCase("environment")) {
//					objects.add(object);
//				}
				else if(type.equalsIgnoreCase("vehicle")) {
					objects.add(new Vehicle(object));
				}
				else if(type.equalsIgnoreCase("rotator")) {
					objects.add(new Rotator(object));
				}
				else if(type.equalsIgnoreCase("translator")) {
					objects.add(new Translator(object));
				}
				else if(type.equalsIgnoreCase("sprite")) {
					objects.add(new Sprite(object));
				}
				else if(type.equalsIgnoreCase("waypoint")) {
					objects.add(new Waypoint(object));
				}
				else if(type.equalsIgnoreCase("pool")) {
					objects.add(new Pool(object));
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
