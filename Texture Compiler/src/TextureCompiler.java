import java.io.*;
import java.util.Vector;

public class TextureCompiler {
	
	public static void main(String args[]) {
		parseParameters(args);
		// sample program calls:
		// -d"../Textures/" -i"animations.ini" -o"../../Data/textures.ini"
	}

	private static void generateTextureData(File textureDirectory, File animationFile, File textureFile, boolean removeExtensions) {
		Vector<String> textureNames = new Vector<String>();
		Vector<AnimatedTexture> animatedTextures = new Vector<AnimatedTexture>();
		
		readDirectory(textureDirectory, textureNames, removeExtensions);
		
		sortData(textureNames);
		
		readAmimationData(animationFile, animatedTextures, textureNames);
		
		writeTextureData(textureFile, textureNames, animatedTextures);
	}
	
	public static void parseParameters(String[] args) {
		if(args == null || args.length == 0) {
			// print instructions on how to use program parameters
			System.out.println("Compiles a list of textures and animations for a 3D Engine.");
			System.out.println("");
			System.out.println("usage: java TextureCompiler -d\"..\\textures\" -i\"animations.ini\" -o\"textures.ini\" [-r]");
			System.out.println("");
			System.out.println(" -d :: directory containing a set of textures; e.g., -d\"..\\Textures\\\"");
			System.out.println(" -i :: input animation data file; e.g., -i\"animations.ini\"");
			System.out.println(" -o :: output texture data file (with animation data); e.g., -o\"textures.ini\"");
			System.out.println(" -r :: remove texture file name extensions (only if present)");
		}
		else {
			String textureDirectoryName = null;
			String animationFileName = null;
			String textureFileName = null;
			boolean removeExtensions = false;
			
			File textureDirectory = null;
			File animationFile = null;
			File textureFile = null;
			
			// parse through parameters
			for(int i=0;i<args.length;i++) {
				if(args[i].length() >= 2) {
					if(args[i].substring(0, 2).equalsIgnoreCase("-d")) {
						textureDirectoryName = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-i")) {
						animationFileName = args[i].substring(2, args[i].length());
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
			if(textureFileName != null) {
				textureFile = new File(textureFileName);
			}
			
			// verify that all required arguments were parsed
			if(textureDirectory == null) {
				System.out.println("ERROR: You must specify a texture directory.");
				System.exit(1);
			}
			if(animationFile == null) {
				System.out.println("ERROR: You must specify an animation data file to read from.");
				System.exit(1);
			}
			if(textureFile == null) {
				System.out.println("ERROR: You must specify a texture data file to write to.");
				System.exit(1);
			}
			
			generateTextureData(textureDirectory, animationFile, textureFile, removeExtensions);
		}
	}
	
	private static void readDirectory(File aFolder, Vector<String> data, boolean removeExtensions) {
		if(aFolder == null || !aFolder.exists()) {
			return;
		}
		
		File[] contents = aFolder.listFiles();
		String fileName;
		
		for(int i=0;i<contents.length;i++) {
			if(contents[i].isFile() && !contents[i].getName().equals("Thumbs.db")) {
				if(removeExtensions) {
					fileName = removeExtensionFrom(contents[i].getName());
				}
				else {
					fileName = contents[i].getName();
				}
				
				if(!data.contains(fileName)) {
					data.add(fileName);
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
	
	public static void readAmimationData(File file, Vector<AnimatedTexture> animatedTextures, Vector<String> textureNames) {
		if(file == null || !file.exists()) {
			return;
		}
		
		String input;
		BufferedReader in;
		AnimatedTexture newAnimatedTexture;
		try {
			in = new BufferedReader(new FileReader(file));
			while((input = in.readLine()) != null) {
				newAnimatedTexture = new AnimatedTexture();
				
				// input the animated texture header
				//input = in.readLine().trim();
				String animatedTextureHeader = input.substring(0, input.lastIndexOf(':')).trim();
				if(!animatedTextureHeader.equalsIgnoreCase("AnimatedTexture")) {
					System.out.println("ERROR: Invalid Animation Data file format. Expected header \"AnimatedTexture\", found \"" + animatedTextureHeader + "\".");
					System.exit(1);
				}
				
				// parse the animated texture index
				int animatedTextureIndex = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
				if(animatedTextureIndex < 0) {
					System.out.println("ERROR: Invalid Animated Texture index parsed: " + animatedTextureIndex + "\".");
					System.exit(1);
				}
				newAnimatedTexture.index = animatedTextureIndex;
				
				// input the properties header
				input = in.readLine().trim();
				String propertyHeader = input.substring(0, input.lastIndexOf(':')).trim();
				if(!propertyHeader.equalsIgnoreCase("Properties")) {
					System.out.println("ERROR: Invalid Animation Data file format. Expected header \"Properties\", found \"" + propertyHeader + "\".");
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
						firstTextureName = newProperty.value;
						for(int j=0;j<textureNames.size();j++) {
							textureNameNoExtension = Texture.removeExtension(textureNames.elementAt(j));
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

	private static void writeTextureData(File textureFile, Vector<String> textureNames, Vector<AnimatedTexture> animatedTextures) {
		PrintWriter out = null;
		
		try {
			out = new PrintWriter(new FileWriter(textureFile, false));
			
			out.println("Textures: " + textureNames.size() + ";");
			
			for(String s : textureNames) {
				out.println("\t" + s);
			}
			
			out.println("AnimatedTextures: " + animatedTextures.size() + ";");
			
			for(AnimatedTexture t : animatedTextures) {
				t.writeTo(out);
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
	
}