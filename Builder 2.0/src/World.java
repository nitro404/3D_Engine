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
	public Vector<AnimatedTexture> animatedTextures;
	public Vector<UniversalObject> objects;
	public Vector<Waypoint> waypoints;
	public boolean externalTextureData;
	
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
	
	public World(Map3D map, Vector<String> textureNames, Vector<AnimatedTexture> animatedTextures) {
		this.textureNames = textureNames;
		this.animatedTextures = animatedTextures;
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
			this.startPosition = null;
			objects = new Vector<UniversalObject>();
			waypoints = new Vector<Waypoint>();
			Vector<Sprite> sprites = new Vector<Sprite>();
			externalTextureData = textureNames != null && animatedTextures != null;
			
			// collect a list of all the texture names in the map
			// and assign index pointers in place of where the texture name used to be
			if(!externalTextureData) { textureNames = new Vector<String>(); }
			String type;
			for(int i=0;i<universalMap.objects.size();i++) {
				object = universalMap.objects.elementAt(i);
				type = object.getPropertyValue("type");
				object.removeProperty("type");
				
				boolean duplicateTextureName;
				Face face;
				String textureName;
				String textureNameNoExtension;
				int textureIndex = -1;
				
				// get textures from data read from external texture file
				if(externalTextureData) {
					// get texture from regular texture list
					if(type.equalsIgnoreCase("sprite")) {
						textureName = object.getPropertyValue("picture");
						textureIndex = -1;
						
						for(int k=0;k<textureNames.size();k++) {
							textureNameNoExtension = Texture.removeExtension(textureNames.elementAt(k));
							if(textureNameNoExtension.equalsIgnoreCase(textureName)) {
								textureIndex = k;
								break;
							}
						}
						
						if(textureIndex == -1) {
							System.out.println("ERROR: Sprite missing texture: \"" + textureName + "\".");
							System.exit(1);
						}
						
						object.setPropertyValue("picture", Integer.toString(textureIndex));
					}
					else if(type.equalsIgnoreCase("environment")) {
						String baseTextureName = object.getPropertyValue("skyboxtexture");
						textureIndex = -1;
						object.removeProperty("skyboxtexture");
						
						for(int l=0;l<Environment.skyboxTextureExtensions.length;l++) {
							for(int k=0;k<textureNames.size();k++) {
								textureNameNoExtension = Texture.removeExtension(textureNames.elementAt(k));
								if(textureNameNoExtension.equalsIgnoreCase(baseTextureName + Environment.skyboxTextureExtensions[l])) {
									textureIndex = k;
									break;
								}
							}
							
							if(textureIndex == -1) {
								System.out.println("ERROR: Environment skybox missing texture: \"" + baseTextureName + Environment.skyboxTextureExtensions[l] + "\".");
								System.exit(1);
							}
							
							object.addProperty("skyboxtexture" + Environment.skyboxTextureExtensions[l], Integer.toString(textureIndex));
						}
					}
					else {
						for(int j=0;j<object.faces.size();j++) {
							face = object.faces.elementAt(j);
							textureName = face.getPropertyValue("texture");
							textureIndex = -1;
							
							// get texture from animated texture list
							if(type.equalsIgnoreCase("pool")) {
								for(int k=0;k<animatedTextures.size();k++) {
									textureNameNoExtension = Texture.removeExtension(textureNames.elementAt(animatedTextures.elementAt(k).firstTextureIndex));
									if(textureNameNoExtension.equalsIgnoreCase(textureName)) {
										textureIndex = k;
										break;
									}
								}
								
								if(textureIndex == -1) {
									System.out.println("ERROR: Pool missing animated texture: \"" + textureName + "\".");
									System.exit(1);
								}
							}
							// get texture from static texture list
							else {
								for(int k=0;k<textureNames.size();k++) {
									textureNameNoExtension = Texture.removeExtension(textureNames.elementAt(k));
									if(textureNameNoExtension.equalsIgnoreCase(textureName)) {
										textureIndex = k;
										break;
									}
								}
								
								if(textureIndex == -1) {
									System.out.println("ERROR: Object missing texture: \"" + textureName + "\".");
									System.exit(1);
								}
							}
							
							face.setPropertyValue("texture", Integer.toString(textureIndex));
						}
					}
				}
				// collect textures from universal map file
				else {
					if(type.equalsIgnoreCase("sprite")) {
						textureName = object.getPropertyValue("picture");
						duplicateTextureName = false;
						for(int k=0;k<textureNames.size();k++) {
							textureNameNoExtension = Texture.removeExtension(textureNames.elementAt(k));
							if(textureNameNoExtension.equalsIgnoreCase(textureName)) {
								duplicateTextureName = true;
								textureIndex = k;
								break;
							}
						}
						if(!duplicateTextureName) {
							textureNames.add(textureName);
							textureIndex = textureNames.size() - 1;
						}
						
						object.setPropertyValue("picture", Integer.toString(textureIndex));
					}
					else {
						for(int j=0;j<object.faces.size();j++) {
							face = object.faces.elementAt(j);
							duplicateTextureName = false;
							textureName = face.getPropertyValue("texture");
							for(int k=0;k<textureNames.size();k++) {
								textureNameNoExtension = Texture.removeExtension(textureNames.elementAt(k));
								if(textureNameNoExtension.equalsIgnoreCase(textureName)) {
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
					}
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
				else if(type.equalsIgnoreCase("environment")) {
					objects.add(new Environment(object));
				}
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
					Sprite newSprite = new Sprite(object, textureIndex);
					objects.add(newSprite);
					sprites.add(newSprite);
				}
				else if(type.equalsIgnoreCase("waypoint")) {
					waypoints.add(new Waypoint(object));
				}
				else if(type.equalsIgnoreCase("pool")) {
					objects.add(new Pool(object, textureIndex));
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
			for(int i=0;i<sprites.size();i++) {
				waypointName = sprites.elementAt(i).getPropertyValue("waypoint");
				waypointIndex = -1;
				for(int j=0;j<waypoints.size();j++) {
					if(waypoints.elementAt(j).getPropertyValue("name").equalsIgnoreCase(waypointName)) {
						waypointIndex = j;
						break;
					}
				}
				if(waypointIndex == -1) {
					System.out.println("ERROR: Invalid waypoint reference on sprite \"" + sprites.elementAt(i).getPropertyValue("name") + "\": \"" + waypointName + "\".");
					System.exit(1);
				}
				
				sprites.elementAt(i).setPropertyValue("waypoint", Integer.toString(waypointIndex));
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
		
		if(Converter.includeTextureData) {
			// print the textures header, followed by the textures
			out.println("Textures: " + this.textureNames.size() + ";");
			for(int i=0;i<this.textureNames.size();i++) {
				out.println("\t" + this.textureNames.elementAt(i));
			}
			
			// print the animated textures header, followed by the animated texture data (if appropriate)
			if(externalTextureData) {
				out.println("AnimatedTextures: " + this.animatedTextures.size() + ";");
				for(int i=0;i<this.animatedTextures.size();i++) {
					this.animatedTextures.elementAt(i).writeTo(out);
				}
			}
			else {
				out.println("AnimatedTextures: 0;");
			}
		}
		
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
