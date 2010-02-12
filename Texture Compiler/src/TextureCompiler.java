import java.io.*;
import java.util.Vector;

public class TextureCompiler {
	
	final public static File inputFile = new File("../animations.ini");
	final public static File outputFile = new File("../textures.ini");
	final public static File textureDirectory = new File("../../Textures/");
	
	public static void main(String args[]) throws IOException {
		PrintWriter out = null;
		Vector<String> textureNames = new Vector<String>();
		Vector<AnimatedTexture> animatedTextures = new Vector<AnimatedTexture>();
		
		readDirectory(textureDirectory, textureNames);
		
		sortData(textureNames);
		
		readAmimationData(inputFile, animatedTextures, textureNames);
		
		try {
			out = new PrintWriter(new FileWriter(outputFile, false));
			
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
			System.out.println("ERROR: " + e.getMessage());
			System.exit(1);
		}
	}
	
	private static void readDirectory(File aFolder, Vector<String> data) throws IOException {
		if(aFolder == null || !aFolder.exists()) {
			return;
		}
		
		File[] contents = aFolder.listFiles();
		String fileName;
		
		for(int i=0;i<contents.length;i++) {
			if(contents[i].isFile() && !contents[i].getName().equals("Thumbs.db")) {
				fileName = removeExtensionFrom(contents[i].getName());
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
				i = -1;
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
	
	public static void readAmimationData(File file, Vector<AnimatedTexture> animatedTextures, Vector<String> textureNames) throws IOException {
		if(file == null || !file.exists()) {
			return;
		}
		
		String input;
		BufferedReader in = new BufferedReader(new FileReader(file));
		AnimatedTexture newAnimatedTexture;
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
			for(int i=0;i<numberOfProperties;i++) {
				newProperty = new Property(in);
				if(newProperty.key.equalsIgnoreCase("name")) {
					newAnimatedTexture.name = newProperty.value;
				}
				else if(newProperty.key.equalsIgnoreCase("firsttexture")) {
					firstTextureName = newProperty.value;
					for(int j=0;j<textureNames.size();j++) {
						if(textureNames.elementAt(j).equalsIgnoreCase(firstTextureName)) {
							newAnimatedTexture.firstTextureIndex = j;
							break;
						}
					}
				}
				else if(newProperty.key.equalsIgnoreCase("frames")) {
					newAnimatedTexture.frames = Integer.parseInt(newProperty.value);
				}
				else if(newProperty.key.equalsIgnoreCase("speed")) {
					newAnimatedTexture.speed = Integer.parseInt(newProperty.value);
				}
			}
			
			animatedTextures.add(newAnimatedTexture);
		}
	}
	
}