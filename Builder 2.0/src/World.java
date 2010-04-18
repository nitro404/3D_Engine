// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.util.StringTokenizer;
import java.util.Vector;
import java.io.*;

public class World implements Map3D {
	
	public Point3D startPosition;
	public Vector<String> textureNames;
	public Vector<String> heightMapNames;
	public Vector<AnimatedTexture> animatedTextures;
	public Vector<HeightMap> heightMaps;
	public Vector<WorldObject> objects;
	public Vector<Waypoint> waypoints;
	
	public World(File file) {
		try {
			this.readFrom(file);
		}
		catch (Exception e) {
			System.out.println("ERROR: Error reading from map file " + file.getName() + ".");
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	public World(Map3D map) {
		try {
			this.convertFrom(map);
		}
		catch (Exception e) {
			System.out.println("ERROR: Error converting map.");
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	public World(Map3D map, Vector<String> textureNames, Vector<String> heightMapNames, Vector<AnimatedTexture> animatedTextures, Vector<Shader> shaders, Vector<HeightMap> heightMaps) {
		this.textureNames = textureNames;
		this.heightMapNames = heightMapNames;
		this.animatedTextures = animatedTextures;
		this.heightMaps = heightMaps;
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
		// convert the map based on what map type it is an instance of
		if(map instanceof WorldcraftMap) {
			System.out.println("ERROR: Conversion from Worldcraft format maps to 3D Engine Worlds is not currently implemented.");
			System.exit(1);
		}
		else if(map instanceof UniversalMap) {
			UniversalMap universalMap = (UniversalMap) map;
			UniversalObject object;
			this.startPosition = new Point3D(0, 0, 0);
			objects = new Vector<WorldObject>();
			waypoints = new Vector<Waypoint>();
			
			// assign index pointers in place of texture names to corresponding locations in the texture and animated texture lists
			String type;
			for(int i=0;i<universalMap.objects.size();i++) {
				object = universalMap.objects.elementAt(i);
				type = object.getPropertyValue("type");
				object.removeProperty("type");
				
				// add the appropriate subclass of UniversalObject based on the object type to a collection of objects
				if(type == null) {
					System.out.println("ERROR: Invalid object found - all universal objects must have a \"type\" property.");
					System.exit(1);
				}
				else if(type.equalsIgnoreCase("info_player_start")) {
					this.startPosition = object.normal.getPosition();
				}
				else if(type.equalsIgnoreCase("static geometry")) {
					objects.add(new Geometry(object, textureNames));
				}
				else if(type.equalsIgnoreCase("environment")) {
					objects.add(new Environment(object, textureNames));
				}
				else if(type.equalsIgnoreCase("vehicle")) {
					objects.add(new Vehicle(object, textureNames));
				}
				else if(type.equalsIgnoreCase("rotator")) {
					objects.add(new Rotator(object, textureNames));
				}
				else if(type.equalsIgnoreCase("translator")) {
					objects.add(new Translator(object, textureNames));
				}
				else if(type.equalsIgnoreCase("sprite")) {
					Sprite newSprite = new Sprite(object, textureNames);
					objects.add(newSprite);
				}
				else if(type.equalsIgnoreCase("waypoint")) {
					waypoints.add(new Waypoint(object));
				}
				else if(type.equalsIgnoreCase("pool")) {
					objects.add(new Pool(object, textureNames, animatedTextures));
				}
				else if(type.equalsIgnoreCase("terrain")) {
					objects.add(new Terrain(object, textureNames, heightMaps));
				}
				else if(type.equalsIgnoreCase("waterfall")) {
					objects.add(new Waterfall(object, textureNames, animatedTextures));
				}
				else {
					System.out.println("WARNING: Ignoring unexpected object of type \"" + type + "\".");
				}
			}
			
			// replace waypoint neighbour names with pointers to the index in the array of each corresponding waypoint
			StringTokenizer tokenizer;
			String token;
			Integer waypointIndexObject;
			Vector<Integer> waypointIndexes;
			String neighbourReferences;
			for(int i=0;i<waypoints.size();i++) {
				tokenizer = new StringTokenizer(waypoints.elementAt(i).getPropertyValue("neighbours").trim(), ", ", false);
				waypointIndexes = new Vector<Integer>();
				while(tokenizer.hasMoreTokens()) {
					waypointIndexObject = null;
					token = tokenizer.nextToken().trim();
					for(int j=0;j<waypoints.size();j++) {
						if(waypoints.elementAt(j).getPropertyValue("name").equalsIgnoreCase(token)) {
							waypointIndexObject = new Integer(j);
							break;
						}
					}
					if(waypointIndexObject == null) {
						System.out.println("ERROR: Invalid waypoint neighbour \"" + token + "\" found on waypoint \"" + waypoints.elementAt(i).getPropertyValue("name") + "\".");
						System.exit(1);
					}
					if(!waypointIndexes.contains(waypointIndexObject)) {
						waypointIndexes.add(waypointIndexObject);
					}
				}
				
				neighbourReferences = "";
				for(int j=0;j<waypointIndexes.size();j++) {
					neighbourReferences += waypointIndexes.elementAt(j);
					if(j < waypointIndexes.size() - 1) {
						neighbourReferences += ", ";
					}
				}
				
				waypoints.elementAt(i).setPropertyValue("neighbours", neighbourReferences);
			}
			
			// replace the sprite waypoint reference names with actual pointers to the index of the same waypoint in the waypoint list
			String waypointName;
			int waypointIndex;
			for(int i=0;i<objects.size();i++) {
				if(objects.elementAt(i) instanceof Sprite) {
					Sprite sprite = (Sprite) objects.elementAt(i);
					waypointName = sprite.getPropertyValue("waypoint");
					waypointIndex = -1;
					for(int j=0;j<waypoints.size();j++) {
						if(waypoints.elementAt(j).getPropertyValue("name").equalsIgnoreCase(waypointName)) {
							waypointIndex = j;
							break;
						}
					}
					
					sprite.setPropertyValue("waypoint", Integer.toString(waypointIndex));
				}
			}
			
		}
		else if(map instanceof World) {
			World world = (World) map;
			this.startPosition = world.startPosition;
			this.textureNames = world.textureNames;
			this.heightMapNames = world.heightMapNames;
			this.animatedTextures = world.animatedTextures;
			this.heightMaps = world.heightMaps;
			this.objects = world.objects;
			this.waypoints = world.waypoints;
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
		
		// print the waypoints header, followed by the waypoints
		out.println("Waypoints: " + this.waypoints.size() + ";");
		for(int i=0;i<this.waypoints.size();i++) {
			out.println("Waypoint: " + i + ";");
			this.waypoints.elementAt(i).writeTo(out);
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
