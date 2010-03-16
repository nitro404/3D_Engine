// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.io.File;

public class Builder {
	
	// output directory is always the root of the search, this may be an issue if it is not specified
	public static void main(String[] args) {
		parseParameters(args);
		// sample program calls:
		// -d../Maps -imap -ouni -m../Maps -e"../Content/Data/Textures/" -t"../Content/Data/textures.ini" -t../Content/Data/textures.ini -h"../Content/Data/Height Maps/"
		// -d../Maps -iuni -owrl -m../Maps -t../Content/Data/textures.ini -h"../Content/Data/Height Maps/"
	}
	
	public static void parseParameters(String[] args) {
		
		if(args == null || args.length == 0) {
			// print instructions on how to use program parameters
			System.out.println("Builds maps for a 3D Engine. Currently supports the following file extensions:");
			System.out.println(" - Worldcraft (.map) Files (read only)");
			System.out.println(" - Universal Format (.uni) Files (read and write)");
			System.out.println(" - 3D Engine World (.wrl) Files (write only)");
			System.out.println("");
			System.out.println("usage: java Builder -f\"mapname.ext\" [-m\"new_mapdir\"] -oext [-s] [-e\"texture_dir\"] [-t\"file.ext\"]");
			System.out.println("                    -d\"path_to_maps\" [-m\"new_mapdir\"] -iext -oext [-s] [-e\"texture_dir\"] [-t\"file.ext\"]");
			System.out.println("");
			System.out.println(" -f :: file name; e.g., -f\"C:\\Test\\Maps\\room.map\"");
			System.out.println(" -d :: directory (containing map files to convert); e.g., -d\"..\\Maps\"");
			System.out.println(" -m :: output directory (optional); e.g., -m\"C:\\Project\\Maps\\Converted\\");
			System.out.println(" -i :: file extension to read (only required if -d specified); e.g., -imap");
			System.out.println(" -o :: file extension to output; e.g., -ouni");
			System.out.println(" -s :: recurse to subdirectories (only if present)");
			System.out.println(" -e :: texture directory location (optional - required if reading .map files); e.g., -e\"..\\Textures\\");
			System.out.println(" -t :: read texture data from alternate file (optional); e.g., -t\"textures.ini\"");
		}
		else {
			String fileName = null;
			String fileDirectoryName = null;
			String inExtension = null;
			String outExtension = null;
			String outDirectoryName = null;
			boolean subdirectories = false;
			String textureDirectoryName = null;
			String textureDataFileName = null;
			
			File file = null;
			File fileDirectory = null;
			File outDirectory = null;
			File textureDirectory = null;
			File textureDataFile = null;
			
			// parse through parameters
			for(int i=0;i<args.length;i++) {
				if(args[i].length() >= 2) {
					if(args[i].substring(0, 2).equalsIgnoreCase("-f")) {
						fileName = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-d")) {
						fileDirectoryName = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-m")) {
						outDirectoryName = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-i")) {
						inExtension = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-o")) {
						outExtension = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-s")) {
						subdirectories = true;
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-e")) {
						textureDirectoryName = args[i].substring(2, args[i].length());
					}
					else if(args[i].substring(0, 2).equalsIgnoreCase("-t")) {
						textureDataFileName = args[i].substring(2, args[i].length());
					}
				}
			}
			
			// verify that files and directories exist, and initialize them
			if(fileName != null) {
				file = new File(fileName);
				if(!file.exists()) {
					System.out.println("ERROR: Specified map file does not exist.");
					System.exit(1);
				}
				if(!file.isFile()) {
					System.out.println("ERROR: Invalid input file.");
					System.exit(1);
				}
			}
			if(fileDirectoryName != null) {
				fileDirectory = new File(fileDirectoryName);
				if(!fileDirectory.exists()) {
					System.out.println("ERROR: Specified input directory does not exist.");
					System.exit(1);
				}
				if(!fileDirectory.isDirectory()) {
					System.out.println("ERROR: Invalid input directory.");
					System.exit(1);
				}
			}
			if(outDirectoryName != null) {
				outDirectory = new File(outDirectoryName);
				if(!outDirectory.exists()) {
					System.out.println("ERROR: Specified output directory does not exist.");
					System.exit(1);
				}
				if(!outDirectory.isDirectory()) {
					System.out.println("ERROR: Invalid output directory.");
					System.exit(1);
				}
			}
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
			if(textureDataFileName != null) {
				textureDataFile = new File(textureDataFileName);
				if(!textureDataFile.exists()) {
					System.out.println("ERROR: Specified texture data file does not exist.");
					System.exit(1);
				}
				if(!textureDataFile.isFile()) {
					System.out.println("ERROR: Invalid texture data file.");
					System.exit(1);
				}
			}
			
			// verify that the texture directory was given if the user is converting from a .map file
			if(inExtension != null && inExtension.equalsIgnoreCase("map")) {
				if(textureDirectory == null) {
					System.out.println("ERROR: You must specify a texture directory if converting from Worldcraft \".map\" file(s).");
					System.exit(1);
				}
				
				if(textureDataFile == null) {
					System.out.println("ERROR: You must specify a texture data file if converting from Worldcraft \".map\" file(s).");
					System.exit(1);
				}
			}
			
			// verify that either a file or a directory containing files was specified
			if(fileName == null && fileDirectoryName == null) {
				System.out.println("ERROR: You must specify a file name or directory containing files to convert.");
				System.exit(1);
			}
			// verify that the right parameters were specified for converting a single file
			else if(fileName != null) {
				if(fileDirectoryName != null) {
					System.out.println("ERROR: File directory and file name specified.");
					System.exit(1);
				}
				if(inExtension != null) {
					System.out.println("WARNING: Input extension file type specified, ignoring.");
				}
				if(outExtension == null) {
					System.out.println("ERROR: Output extension file type not specified.");
					System.exit(1);
				}
				else if(!(outExtension.equalsIgnoreCase("uni") || outExtension.equalsIgnoreCase("wrl"))) {
					System.out.println("ERROR: Output file extension is invalid.");
					System.exit(1);
				}
				if(outDirectoryName == null) {
					System.out.println("WARNING: Output directory not specified, setting output directory to input directory.");
					outDirectory = file.getParentFile();
				}
				inExtension = file.getName().substring(file.getName().lastIndexOf('.') + 1, file.getName().length());
				if(!(inExtension.equalsIgnoreCase("map") || inExtension.equalsIgnoreCase("uni"))) {
					System.out.println("ERROR: Input file extension is invalid.");
					System.exit(1);
				}
				if(inExtension.equalsIgnoreCase(outExtension)) {
					System.out.println("ERROR: Input and output extensions are identical.");
					System.exit(1);
				}
				
				// convert map file
				Converter.convertFile(file, outDirectory, inExtension, outExtension, subdirectories, textureDirectory, textureDataFile);
			}
			// verify that the right parameters were specified for converting a directory containing files
			else if(fileDirectoryName != null) {
				if(fileName != null) {
					System.out.println("ERROR: File directory and file name specified.");
					System.exit(1);
				}
				if(inExtension == null) {
					System.out.println("ERROR: Input extension file type not specified.");
					System.exit(1);
				}
				else if(!(inExtension.equalsIgnoreCase("map") || inExtension.equalsIgnoreCase("uni"))) {
					System.out.println("ERROR: Input file extension is invalid.");
					System.exit(1);
				}
				if(outExtension == null) {
					System.out.println("ERROR: Output extension file type not specified.");
					System.exit(1);
				}
				else if(!(outExtension.equalsIgnoreCase("uni") || outExtension.equalsIgnoreCase("wrl"))) {
					System.out.println("ERROR: Output file extension is invalid.");
					System.exit(1);
				}
				if(outDirectoryName == null) {
					System.out.println("WARNING: Output directory not specified, setting output directory to input directory.");
					outDirectory = fileDirectory;
				}
				if(inExtension.equalsIgnoreCase(outExtension)) {
					System.out.println("ERROR: Input and output extensions are identical.");
					System.exit(1);
				}
				
				// convert map files
				Converter.convertFiles(fileDirectory, outDirectory, inExtension, outExtension, subdirectories, textureDirectory, textureDataFile);
			}
			
		}
		
	}
	
}
