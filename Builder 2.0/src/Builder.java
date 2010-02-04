import java.io.File;

public class Builder {
	
	// output directory is always the root of the search, this may be an issue if it is not specified
	public static void main(String[] args) {
		parseParameters(args);
		// -dMaps -iuni -owrl -mMaps
		// -d"Maps" -iuni -owrl
		// -f"Maps/test.uni" -owrl
		// -d"../Map Stuff (Duplicate)/Maps/" -iuni -owrl -m"Maps"
		// -f"../Map Stuff (Duplicate)/Maps/test.uni" -owrl -m"Maps"
	}
	
	public static void parseParameters(String[] args) {
		
		if(args == null || args.length == 0) {
			// print instructions on how to use program parameters
			System.out.println("Builds maps for a 3D Engine. Currently supports the following file extensions:");
			System.out.println(" - Worldcraft (.map) Files (read only)");
			System.out.println(" - Universal Format (.uni) Files (read and write)");
			System.out.println(" - 3D Engine World (.wrl) Files (write only)");
			System.out.println("");
			System.out.println("usage: java Builder -f\"mapname.ext\" [-m\"path_to_new_maps\"] -oext [-s]");
			System.out.println("                    -d\"path_to_maps\" [-m\"path_to_new_maps\"] -iext -oext [-s]");
			System.out.println("");
			System.out.println(" -f :: file name; e.g., -f\"C:\\Test\\Maps\\room.map\"");
			System.out.println(" -d :: directory (containing map files to convert); e.g., -d\"..\\Project\\Maps\"");
			System.out.println(" -m :: output directory (optional), e.g., -m\"C:\\Project\\Maps\\Converted\\");
			System.out.println(" -i :: file extension to read (only required if -d specified), e.g., -imap");
			System.out.println(" -o :: file extension to output, e.g., -ouni");
			System.out.println(" -s :: recurse to subdirectories (only if present)");
		}
		else {
			String fileName = null;
			String fileDirectoryName = null;
			String inExtension = null;
			String outExtension = null;
			String outDirectoryName = null;
			boolean subdirectories = false;
			
			File file = null;
			File fileDirectory = null;
			File outDirectory = null;
			
			// parse through parameters
			for(int i=0;i<args.length;i++) {
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
				Converter.convertFile(file, outDirectory, inExtension, outExtension, subdirectories);
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
				Converter.convertFiles(fileDirectory, outDirectory, inExtension, outExtension, subdirectories);
			}
			
		}
		
	}
	
}