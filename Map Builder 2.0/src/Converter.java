import java.io.*;

public class Converter {
	
	// convert a set of files between two formats
	public static void convertFiles(File fileDirectory, File outDirectory, String inExtension, String outExtension, boolean subdirectories) {
		File[] files = fileDirectory.listFiles();
		
		for(int i=0;i<files.length;i++) {
			String fileExtension = files[i].getName().substring(files[i].getName().lastIndexOf('.') + 1, files[i].getName().length());
			if(inExtension.equalsIgnoreCase(fileExtension)) {
				convertFile(files[i], outDirectory, inExtension, outExtension, subdirectories);
			}
		}
	}
	
	// convert a file between two formats
	public static void convertFile(File file, File outDirectory, String outExtension, boolean subdirectories) {
		String inExtension = file.getName().substring(file.getName().lastIndexOf('.') + 1, file.getName().length());
System.out.println("Converting " + file.getName() + " from ." + inExtension + " to ." + outExtension);
	}
	
}
