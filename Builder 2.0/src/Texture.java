import java.io.*;
import java.awt.Dimension;
import java.awt.image.BufferedImage;

import javax.imageio.ImageIO;

public class Texture {
	
	public File file;
	public String extension;
	public int width;
	public int height;
	
	public Texture(String fileName) {
		this(new File(fileName));
	}
	
	public Texture(String fileName, String directory) {
		this(new File(directory +
					  ((directory.charAt(directory.length() - 1) == '\\') ? "" :
					  ((directory.charAt(directory.length() - 1) == '/')  ? "" : "\\")) +
					  fileName));
	}
	
	public Texture(String fileName, File directory) {
		this(new File(directory.getPath() +
					  ((directory.getPath().charAt(directory.getPath().length() - 1) == '\\') ? "" :
					  ((directory.getPath().charAt(directory.getPath().length() - 1) == '/')  ? "" : "\\")) +
					  fileName));
	}
	
	public Texture(File file) {
		this.file = file;
		this.extension = getExtension(file.getName());
		try {
			Dimension dimensions = getDimensions(file);
			this.width = dimensions.width;
			this.height = dimensions.height;
		}
		catch(Exception e) {
			this.height = 0;
			this.width = 0;
		}
	}
	
	public String getFileNameNoExtension() {
		return Texture.removeExtension(file.getName());
	}
	
	public static String getExtension(String textureName) {
		if(textureName.lastIndexOf('.') != -1) {
			return textureName.substring(textureName.lastIndexOf('.') + 1, textureName.length());
		}
		else {
			return null;
		}
	}
	
	public static boolean hasExtension(String textureName, String fileExtension) {
		String textureExtension = getExtension(textureName);
		return textureExtension != null && textureExtension.equalsIgnoreCase(fileExtension);
	}
	
	public static String removeExtension(String textureName) {
		if(textureName.lastIndexOf('.') != -1) {
			return textureName.substring(0, textureName.lastIndexOf('.'));
		}
		else {
			return textureName;
		}
	}
	
	public static Dimension getDimensions(File file) throws IOException {
		if(file == null || !file.exists() || !file.isFile()) { return null; }
		
		Dimension dimensions = null;
		
		String fileExt = Texture.getExtension(file.getName()); 
		if(fileExt.equalsIgnoreCase("bmp")) {
			BufferedImage image = ImageIO.read(file);
			dimensions = new Dimension(image.getWidth(), image.getHeight());
		}
		else if(fileExt.equalsIgnoreCase("tga")) {
			dimensions = new Dimension(0, 0);
			
			byte[] header = new byte[18];
			int fileLength = (int) file.length() - header.length;
			if(fileLength < 0) { return dimensions; }
			
			RandomAccessFile in = new RandomAccessFile(file, "r");
			in.read(header);
			in.close();
			
			if((header[0] | header[1]) != 0) { return dimensions; }
			if(header[2] != 2) { return dimensions; }
			int w = 0, h = 0;
			w |= (header[12] & 0xFF) << 0;
			w |= (header[13] & 0xFF) << 8;
			h |= (header[14] & 0xFF) << 0;
			h |= (header[15] & 0xFF) << 8;
			
			dimensions = new Dimension(w, h);
		}
		else {
			System.out.println("ERROR: Incompatible image format: \"" + fileExt + "\".");
			System.exit(1);
		}
		
		return dimensions;
	}
	
}
