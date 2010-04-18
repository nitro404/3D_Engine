import java.io.*;
import java.util.Vector;

public class TextureCompiler {
	
	public static void main(String args[]) {
		parseParameters(args);
		// sample program call:
		// -t"../Textures/" -m"../Textures/Height Maps/" -a"animations.ini" -s"shaders.ini" -h"heightmaps.ini" -o"../Data/textures.ini"
	}

	public static void parseParameters(String[] args) {
		if(args == null || args.length == 0) {
			// print instructions on how to use program parameters
			System.out.println("Compiles a list of textures and animations for a 3D Engine.");
			System.out.println("");
			System.out.println("usage: java TextureCompiler -t\"..\\textures\" [-m\"..\\heightmaps\"] [-a\"animations.ini\"] [-s\"shaders.ini\"] [-h\"heightmaps.ini\"] -o\"textures.ini\" [-r]");
			System.out.println("");
			System.out.println(" -t :: directory containing a set of textures; e.g., -d\"..\\Textures\\\"");
			System.out.println(" -m :: directory containing a set of height maps; e.g., -d\"..\\Textures\\Height Maps\\\"");
			System.out.println(" -a :: input animation data file; e.g., -a\"animations.ini\"");
			System.out.println(" -h :: height map data file (e.g., -h\"heightmaps.ini\"");
			System.out.println(" -s :: shader data file (e.g., -s\"shaders.ini\"");
			System.out.println(" -o :: output texture data file (with animation data); e.g., -o\"textures.ini\"");
			System.out.println(" -r :: remove texture file name extensions (only if present)");
		}
		else {
			String textureDirectoryName = null;
			String heightMapDirectoryName = null;
			String animationFileName = null;
			String shaderFileName = null;
			String textureFileName = null;
			String heightMapFileName = null;
			boolean removeExtensions = false;
			
			File textureDirectory = null;
			File heightMapDirectory = null;
			File animationFile = null;
			File shaderFile = null;
			File textureFile = null;
			File heightMapFile = null;
			
			// parse through parameters
			for(int i=0;i<args.length;i++) {
				if(args[i].length() >= 2) {
					if(args[i].substring(0, 2).equalsIgnoreCase("-t")) {
						textureDirectoryName = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-m")) {
						heightMapDirectoryName = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-a")) {
						animationFileName = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-s")) {
						shaderFileName = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-h")) {
						heightMapFileName = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-o")) {
						textureFileName = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-r")) {
						removeExtensions = true;
					}
				}
			}
			
			// verify that files and directories exist, and initialize them
			if(textureDirectoryName != null) {
				textureDirectory = new File(textureDirectoryName);
				if(!textureDirectory.exists()) {
					System.out.println("ERROR: Specified texture directory does not exist.");
					System.exit(1);
				}
				if(!textureDirectory.isDirectory()) {
					System.out.println("ERROR: Invalid texture directory.");
					System.exit(1);
				}
			}
			if(heightMapDirectoryName != null) {
				heightMapDirectory = new File(heightMapDirectoryName);
				if(!heightMapDirectory.exists()) {
					System.out.println("ERROR: Specified height map directory does not exist.");
					System.exit(1);
				}
				if(!heightMapDirectory.isDirectory()) {
					System.out.println("ERROR: Invalid height map directory.");
					System.exit(1);
				}
			}
			if(animationFileName != null) {
				animationFile = new File(animationFileName);
				if(!animationFile.exists()) {
					System.out.println("ERROR: Specified animation data file does not exist.");
					System.exit(1);
				}
				if(!animationFile.isFile()) {
					System.out.println("ERROR: Invalid animation data file.");
					System.exit(1);
				}
			}
			if(shaderFileName != null) {
				shaderFile = new File(shaderFileName);
				if(!shaderFile.exists()) {
					System.out.println("ERROR: Specified shader data file does not exist.");
					System.exit(1);
				}
				if(!shaderFile.isFile()) {
					System.out.println("ERROR: Invalid shader data file.");
					System.exit(1);
				}
			}
			if(heightMapFileName != null) {
				heightMapFile = new File(heightMapFileName);
				if(!heightMapFile.exists()) {
					System.out.println("ERROR: Specified height map data file does not exist.");
					System.exit(1);
				}
				if(!heightMapFile.isFile()) {
					System.out.println("ERROR: Invalid height map data file.");
					System.exit(1);
				}
			}
			if(textureFileName != null) {
				textureFile = new File(textureFileName);
			}
			
			// verify that all required arguments were parsed
			if(textureDirectory == null) {
				System.out.println("ERROR: You must specify a texture directory.");
				System.exit(1);
			}
			if(textureFile == null) {
				System.out.println("ERROR: You must specify a texture data file to write to.");
				System.exit(1);
			}
			
			generateTextureData(textureDirectory, heightMapDirectory, animationFile, shaderFile, heightMapFile, textureFile, removeExtensions);
		}
	}

	private static void generateTextureData(File textureDirectory, File heightMapDirectory, File animationFile, File shaderFile, File heightMapFile, File textureFile, boolean removeExtensions) {
		Vector<String> textureFileNames = new Vector<String>();
		Vector<AnimatedTexture> animatedTextures = new Vector<AnimatedTexture>();
		Vector<Shader> shaders = new Vector<Shader>();
		Vector<String> heightMapFileNames = new Vector<String>();
		Vector<HeightMap> heightMaps = new Vector<HeightMap>();
		
		readDirectory(textureDirectory, textureFileNames, null, removeExtensions);
		
		sortData(textureFileNames);
		
		readDirectory(heightMapDirectory, heightMapFileNames, "raw", removeExtensions);
		
		sortData(textureFileNames);
		
		readAmimationData(animationFile, animatedTextures, textureFileNames);
		
		readShaderData(shaderFile, shaders);
		
		readHeightMapData(heightMapFile, heightMapFileNames, heightMaps);
		
		writeTextureData(textureFile, textureFileNames, heightMapFileNames, animatedTextures, shaders, heightMaps);
	}
	
	public static void readAmimationData(File animationFile, Vector<AnimatedTexture> animatedTextures, Vector<String> textureFileNames) {
		if(animationFile == null || !animationFile.exists()) {
			return;
		}
		
		String input;
		BufferedReader in;
		AnimatedTexture newAnimatedTexture;
		try {
			in = new BufferedReader(new FileReader(animationFile));
			while((input = in.readLine()) != null) {
				newAnimatedTexture = new AnimatedTexture();
				
				// input the animated texture header
				String animatedTextureHeader = input.substring(0, input.lastIndexOf(':')).trim();
				if(!animatedTextureHeader.equalsIgnoreCase("AnimatedTexture")) {
					System.out.println("ERROR: Invalid animation data file format. Expected header \"AnimatedTexture\", found \"" + animatedTextureHeader + "\".");
					System.exit(1);
				}
				
				// parse the animated texture index
				int animatedTextureIndex = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
				if(animatedTextureIndex < 0) {
					System.out.println("ERROR: Invalid animated texture index parsed: " + animatedTextureIndex + "\".");
					System.exit(1);
				}
				newAnimatedTexture.index = animatedTextureIndex;
				
				// input the properties header
				input = in.readLine().trim();
				String propertyHeader = input.substring(0, input.lastIndexOf(':')).trim();
				if(!propertyHeader.equalsIgnoreCase("Properties")) {
					System.out.println("ERROR: Invalid animation data file format. Expected header \"Properties\", found \"" + propertyHeader + "\".");
					System.exit(1);
				}
				
				// input the properties
				int numberOfProperties = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
				Property newProperty;
				String firstTextureName;
				String textureNameNoExtension;
				for(int i=0;i<numberOfProperties;i++) {
					newProperty = new Property(in);
					if(newProperty.key.equalsIgnoreCase("name")) {
						newAnimatedTexture.name = newProperty.value;
					}
					else if(newProperty.key.equalsIgnoreCase("firsttexture")) {
						firstTextureName = Texture.removeExtension(newProperty.value);
						for(int j=0;j<textureFileNames.size();j++) {
							textureNameNoExtension = Texture.removeExtension(textureFileNames.elementAt(j));
							if(textureNameNoExtension.equalsIgnoreCase(firstTextureName)) {
								newAnimatedTexture.firstTextureIndex = j;
								break;
							}
						}
					}
					else if(newProperty.key.equalsIgnoreCase("frames")) {
						newAnimatedTexture.frames = Integer.valueOf(newProperty.value);
					}
					else if(newProperty.key.equalsIgnoreCase("speed")) {
						newAnimatedTexture.speed = Double.valueOf(newProperty.value);
					}
				}
				
				animatedTextures.add(newAnimatedTexture);
			}
		}
		catch(Exception e) {
			System.out.println("ERROR: Error reading from animation data file: " + e.getMessage());
			System.exit(1);
		}
	}

	public static void readShaderData(File shaderFile, Vector<Shader> shaders) {
		if(shaderFile == null || !shaderFile.exists()) {
			return;
		}
		
		String input;
		BufferedReader in;
		Shader newShader;
		try {
			in = new BufferedReader(new FileReader(shaderFile));
			while((input = in.readLine()) != null) {
				newShader = new Shader();
				
				// input the shader header
				String shaderHeader = input.substring(0, input.lastIndexOf(':')).trim();
				if(!shaderHeader.equalsIgnoreCase("Shader")) {
					System.out.println("ERROR: Invalid shader data file format. Expected header \"Shader\", found \"" + shaderHeader + "\".");
					System.exit(1);
				}
				
				// parse the shader index
				int shaderIndex = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
				if(shaderIndex < 0) {
					System.out.println("ERROR: Invalid shader index parsed: " + shaderIndex + "\".");
					System.exit(1);
				}
				newShader.index = shaderIndex;
				
				// input the properties header
				input = in.readLine().trim();
				String propertyHeader = input.substring(0, input.lastIndexOf(':')).trim();
				if(!propertyHeader.equalsIgnoreCase("Properties")) {
					System.out.println("ERROR: Invalid shader data file format. Expected header \"Properties\", found \"" + propertyHeader + "\".");
					System.exit(1);
				}
				
				// input the properties
				int numberOfProperties = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
				Property newProperty;
				for(int i=0;i<numberOfProperties;i++) {
					newProperty = new Property(in);
					if(newProperty.key.equalsIgnoreCase("name")) {
						newShader.name = newProperty.value;
					}
					else if(newProperty.key.equalsIgnoreCase("vertex_shader")) {
						newShader.vertexShader = newProperty.value;
					}
					else if(newProperty.key.equalsIgnoreCase("fragment_shader")) {
						newShader.fragmentShader = newProperty.value;
					}
				}
				
				shaders.add(newShader);
			}
		}
		catch(Exception e) {
			System.out.println("ERROR: Error reading from shader data file: " + e.getMessage());
			System.exit(1);
		}
	}
	
	private static void readHeightMapData(File heightMapFile, Vector<String> heightMapFileNames, Vector<HeightMap> heightMaps) {
		if(heightMapFile == null || !heightMapFile.exists()) {
			return;
		}
		
		String input;
		BufferedReader in;
		HeightMap newHeightMap;
		try {
			in = new BufferedReader(new FileReader(heightMapFile));
			while((input = in.readLine()) != null) {
				newHeightMap = new HeightMap();
				
				// input the height map header
				String heightMapHeader = input.substring(0, input.lastIndexOf(':')).trim();
				if(!heightMapHeader.equalsIgnoreCase("HeightMap")) {
					System.out.println("ERROR: Invalid height map data file format. Expected header \"HeightMap\", found \"" + heightMapHeader + "\".");
					System.exit(1);
				}
				
				// parse the height map index
				int heightMapIndex = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
				if(heightMapIndex < 0) {
					System.out.println("ERROR: Invalid height map index parsed: " + heightMapIndex + "\".");
					System.exit(1);
				}
				newHeightMap.index = heightMapIndex;
				
				// input the properties header
				input = in.readLine().trim();
				String propertyHeader = input.substring(0, input.lastIndexOf(':')).trim();
				if(!propertyHeader.equalsIgnoreCase("Properties")) {
					System.out.println("ERROR: Invalid height map data file format. Expected header \"Properties\", found \"" + propertyHeader + "\".");
					System.exit(1);
				}
				
				// input the properties
				int numberOfProperties = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
				Property newProperty;
				String heightMapFileName;
				String currentHeightMapFileName;
				for(int i=0;i<numberOfProperties;i++) {
					newProperty = new Property(in);
					if(newProperty.key.equalsIgnoreCase("name")) {
						newHeightMap.name = newProperty.value;
					}
					else if(newProperty.key.equalsIgnoreCase("heightmap")) {
						heightMapFileName = Texture.removeExtension(newProperty.value);
						for(int j=0;j<heightMapFileNames.size();j++) {
							currentHeightMapFileName = Texture.removeExtension(heightMapFileNames.elementAt(j));
							if(currentHeightMapFileName.equalsIgnoreCase(heightMapFileName)) {
								newHeightMap.heightMapFileIndex = j;
								break;
							}
						}
					}
					else if(newProperty.key.equalsIgnoreCase("width")) {
						newHeightMap.width = Integer.valueOf(newProperty.value);
					}
					else if(newProperty.key.equalsIgnoreCase("height")) {
						newHeightMap.height = Integer.valueOf(newProperty.value);
					}
				}
				
				heightMaps.add(newHeightMap);
			}
		}
		catch(Exception e) {
			System.out.println("ERROR: Error reading from height map data file: " + e.getMessage());
			System.exit(1);
		}
	}

	private static void writeTextureData(File textureFile, Vector<String> textureFileNames, Vector<String> heightMapFileNames, Vector<AnimatedTexture> animatedTextures, Vector<Shader> shaders, Vector<HeightMap> heightMaps) {
		PrintWriter out = null;
		
		try {
			out = new PrintWriter(new FileWriter(textureFile, false));
			
			out.println("TextureNames: " + textureFileNames.size() + ";");
			
			for(String s : textureFileNames) {
				out.println("\t" + s);
			}
			
			out.println("HeightMapNames: " + heightMapFileNames.size() + ";");
			
			for(String s : heightMapFileNames) {
				out.println("\t" + s);
			}
			
			out.println("AnimatedTextures: " + animatedTextures.size() + ";");
			
			for(AnimatedTexture t : animatedTextures) {
				t.writeTo(out);
			}
			
			out.println("Shaders: " + shaders.size() + ";");
			
			for(Shader s : shaders) {
				s.writeTo(out);
			}
			
			out.println("HeightMaps: " + heightMaps.size() + ";");
			
			for(HeightMap h : heightMaps) {
				h.writeTo(out);
			}
			
			if(out != null) {
				out.close();
			}
		}
		catch(Exception e) {
			System.out.println("ERROR: Error writing to texture data file: " + e.getMessage());
			System.exit(1);
		}
	}
	
	private static void readDirectory(File aFolder, Vector<String> data, String fileExtension, boolean removeExtensions) {
		if(aFolder == null || !aFolder.exists()) {
			return;
		}
		
		File[] contents = aFolder.listFiles();
		String fileName;
		
		for(int i=0;i<contents.length;i++) {
			if(contents[i].isFile() && !contents[i].getName().equals("Thumbs.db")) {
				fileName = contents[i].getName();
				if(fileExtension == null || Texture.hasExtension(fileName, fileExtension)) {
					fileName = (removeExtensions) ? removeExtensionFrom(fileName) : fileName;
					
					if(!data.contains(fileName)) {
						data.add(fileName);
					}
				}
			}
			
		}
	}
	
	private static String removeExtensionFrom(String fileName) {
		if(fileName == null || fileName.length() == 0) return null;
		
		int index = fileName.length();
		for(int i=fileName.length()-1;i>=0;i--) {
			if(fileName.charAt(i) == '.') {
				index = i;
				break;
			}
		}
		
		return fileName.substring(0, index);
	}
	
	private static void sortData(Vector<String> data) {
		if(data == null || data.size() <= 1) return;
		int minIndex;
		String temp = null;
		
		for(int i=0;i<data.size()-1;i++) {
			minIndex = i;
			
			for (int j=i+1;j<data.size();j++) {
				if(data.elementAt(minIndex).compareToIgnoreCase(data.elementAt(j)) > 0) {
					minIndex = j;
				}
			}
			
			if(minIndex != i) {
				temp = data.elementAt(i);
				data.set(i, data.elementAt(minIndex));
				data.set(minIndex, temp);
			}
		}
	}
	
}