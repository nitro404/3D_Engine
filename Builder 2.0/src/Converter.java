// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.io.*;

public class Converter {
	
	// convert a set of files between two formats
	public static void convertFiles(File fileDirectory, File outDirectory, String inExtension, String outExtension, boolean subdirectories) {
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
							convertFiles(subFiles[j], outDirectory, inExtension, outExtension, subdirectories);
						}
						// otherwise convert the selected file
						else if(subFiles[j].isFile()) {
							convertFile(subFiles[j], outDirectory, inExtension, outExtension, subdirectories);
						}
					}
				}
			}
			// convert the selected file between two specified formats
			else if(files[i].isFile()) {
				fileExtension = files[i].getName().substring(files[i].getName().lastIndexOf('.') + 1, files[i].getName().length());
				if(inExtension.equalsIgnoreCase(fileExtension)) {
					convertFile(files[i], outDirectory, inExtension, outExtension, subdirectories);
				}
			}
		}
	}
	
	// convert a file between two formats
	public static void convertFile(File file, File outDirectory, String inExtension, String outExtension, boolean subdirectories) {
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
		
		// output the converted map
		if(outExtension.equalsIgnoreCase("uni")) {
			convertedMap = new UniversalMap(originalMap);
		}
		else if(outExtension.equalsIgnoreCase("wrl")) {
			convertedMap = new World(originalMap);
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
	
}
