// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.util.Vector;
import java.io.*;

public class Converter {
	
	// convert a set of files between two formats
	public static void convertFiles(File fileDirectory, File outDirectory, String inExtension, String outExtension, boolean subdirectories, File textureDirectory, File textureDataFile) { 	
		File[] files = fileDirectory.listFiles();
		File[] subFiles = null;
		String fileExtension;
		
		// iterate over all the files in the current directory
		for(int i=0;i<files.length;i++) {
			// ignore 0-length folder / file names (such as .svn folders if you use an SVN repository)
			if(files[i].isDirectory() && subdirectories && files[i].getName().charAt(0) != '.') {
				// iterate over all of the items in each subdirectory (if appropriate)
				subFiles = files[i].listFiles();
				for(int j=0;j<subFiles.length;j++) {
					// once again, ignore 0-length folder / file names
					if(subFiles[j].getName().charAt(0) != '.') {
						// recurse if the current file is a directory
						if(subFiles[j].isDirectory() && subdirectories && subFiles[j].getName().charAt(0) != '.') {
							convertFiles(subFiles[j], outDirectory, inExtension, outExtension, subdirectories, textureDirectory, textureDataFile);
						}
						// otherwise convert the selected file
						else if(subFiles[j].isFile()) {
							convertFile(subFiles[j], outDirectory, inExtension, outExtension, subdirectories, textureDirectory, textureDataFile);
						}
					}
				}
			}
			// convert the selected file between two specified formats
			else if(files[i].isFile()) {
				fileExtension = files[i].getName().substring(files[i].getName().lastIndexOf('.') + 1, files[i].getName().length());
				if(inExtension.equalsIgnoreCase(fileExtension)) {
					convertFile(files[i], outDirectory, inExtension, outExtension, subdirectories, textureDirectory,textureDataFile);
				}
			}
		}
	}
	
	// convert a file between two formats
	public static void convertFile(File file, File outDirectory, String inExtension, String outExtension, boolean subdirectories, File textureDirectory, File textureDataFile) {
		Map3D originalMap = null;
		Map3D convertedMap = null;
		File outputFile = null;
		
		// generate the output file name
		char dirLastChar = outDirectory.getPath().charAt(outDirectory.getPath().length() - 1);
		String outputFileName = outDirectory.getPath();
		if(!(dirLastChar == '/' || dirLastChar == '\\')) {
			outputFileName += '/';
		}
		outputFileName += file.getName().substring(0, file.getName().lastIndexOf('.'));
		outputFileName += '.';
		outputFileName += outExtension;
		
		// display converting message
		System.out.println("Converting " + file.getName() + " to " + (outExtension.equalsIgnoreCase("uni") ? "Universal Map Format (.uni)" :
																	 (outExtension.equalsIgnoreCase("wrl") ? "3D Engine World Format (.wrl)" :
																	 " Unknown Format"))
																	 + ".");
		
		// input the map
		if(inExtension.equalsIgnoreCase("map")) {
			originalMap = new WorldcraftMap(file);
		}
		else if(inExtension.equalsIgnoreCase("uni")) {
			originalMap = new UniversalMap(file);
		}
		
		// input the texture data (if appropriate)
		Vector<String> textureNames = null;
		Vector<String> heightMapNames = null;
		Vector<AnimatedTexture> animatedTextures = null;
		Vector<HeightMap> heightMaps = null;
		Vector<Texture> textures = null;
		if(textureDataFile != null) {
			textureNames = new Vector<String>();
			heightMapNames = new Vector<String>();
			animatedTextures = new Vector<AnimatedTexture>();
			heightMaps = new Vector<HeightMap>();
			try {
				BufferedReader in = new BufferedReader(new FileReader(textureDataFile));
				readTextureData(in, textureNames, heightMapNames, animatedTextures, heightMaps);
			}
			catch(Exception e) {
				System.out.println("ERROR: Error reading texture data file " + textureDataFile.getName() + ".");
				System.exit(1);
			}
			if(textureDirectory != null) {
				textures = new Vector<Texture>();
				for(int i=0;i<textureNames.size();i++) {
					textures.add(new Texture(textureNames.elementAt(i), textureDirectory));
				}
			}
		}
		
		// output the converted map
		if(outExtension.equalsIgnoreCase("uni")) {
			convertedMap = new UniversalMap(originalMap, textureDirectory, textures);
		}
		else if(outExtension.equalsIgnoreCase("wrl")) {
			if(textureDataFile == null) {
				convertedMap = new World(originalMap);
			}
			else {
				convertedMap = new World(originalMap, textureNames, heightMapNames, animatedTextures, heightMaps);
			}
		}
		
		// write the converted map to file
		outputFile = new File(outputFileName);
		try {
			convertedMap.writeTo(outputFile);
		}
		catch(Exception e) {
			System.out.println("ERROR: Error writing to map file " + outputFile.getName() + ".");
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	private static void readTextureData(BufferedReader in, Vector<String> textureNames, Vector<String> heightMapNames, Vector<AnimatedTexture> animatedTextures, Vector<HeightMap> heightMaps) throws Exception {
		String input;
		
		// input the texture file names header
		input = in.readLine().trim();
		String textureNamesHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!textureNamesHeader.equalsIgnoreCase("TextureNames")) {
			System.out.println("ERROR: Invalid texture data file format. Expected header \"TexturesNames\", found \"" + textureNamesHeader + "\".");
			System.exit(1);
		}
		
		// input the number of textures file names
		int numberOfTextureNames = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		if(numberOfTextureNames < 0) {
			System.out.println("ERROR: Texture name count is negative in texture data file.");
			System.exit(1);
		}
		
		// input the list of height map file names
		for(int i=0;i<numberOfTextureNames;i++) {
			textureNames.add(in.readLine().trim());
		}
		
		// input the height map file names header
		input = in.readLine().trim();
		String heightMapNamesHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!heightMapNamesHeader.equalsIgnoreCase("HeightMapNames")) {
			System.out.println("ERROR: Invalid texture data file format. Expected header \"HeightMapNames\", found \"" + heightMapNamesHeader + "\".");
			System.exit(1);
		}
		
		// input the number of height map names
		int numberOfHeightMapNames = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		if(numberOfHeightMapNames < 0) {
			System.out.println("ERROR: Height map name count is negative in texture data file.");
			System.exit(1);
		}
		
		// input the list of height map names
		for(int i=0;i<numberOfHeightMapNames;i++) {
			heightMapNames.add(in.readLine().trim());
		}
		
		// input the animated textures header
		input = in.readLine().trim();
		String animatedTexturesHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!animatedTexturesHeader.equalsIgnoreCase("AnimatedTextures")) {
			System.out.println("ERROR: Invalid texture data file format. Expected header \"AnimatedTextures\", found \"" + animatedTexturesHeader + "\".");
			System.exit(1);
		}
		
		// input the number of animated textures
		int numberOfAnimatedTextures = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		if(numberOfAnimatedTextures < 0) {
			System.out.println("ERROR: Animated texture count is negative in texture data file.");
			System.exit(1);
		}
		
		// input the list of animated textures
		for(int i=0;i<numberOfAnimatedTextures;i++) {
			animatedTextures.add(new AnimatedTexture(in));
		}
		
		// input the height maps header
		input = in.readLine().trim();
		String heightMapsHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!heightMapsHeader.equalsIgnoreCase("HeightMaps")) {
			System.out.println("ERROR: Invalid texture data file format. Expected header \"HeightMaps\", found \"" + heightMapsHeader + "\".");
			System.exit(1);
		}
		
		// input the number of height maps
		int numberOfHeightMaps = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		if(numberOfHeightMaps < 0) {
			System.out.println("ERROR: Height map count is negative in texture data file.");
			System.exit(1);
		}
		
		// input the list of height maps
		for(int i=0;i<numberOfHeightMaps;i++) {
			heightMaps.add(new HeightMap(in));
		}
	}
	
}
