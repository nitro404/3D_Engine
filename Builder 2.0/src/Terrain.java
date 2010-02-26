// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.util.Vector;
import java.awt.image.BufferedImage;
import java.io.*;

import javax.imageio.ImageIO;

public class Terrain extends UniversalObject {
	
	public int heightMapWidth;
	public int heightMapHeight;
	public Point3D maxPoint;
	public Point3D minPoint;
	private int stretched;
	
	public Terrain(UniversalObject object, File heightMapDirectory) {
		super(object.objectIndex, null, null, object.properties, null);
		
		if(heightMapDirectory == null) {
			System.out.println("ERROR: Height map directory missing, cannot generate terrain.");
			System.exit(1);
		}
		
		try {
			stretched = Integer.valueOf(getPropertyValue("stretched"));
			removeProperty("stretched");
		}
		catch(NumberFormatException e) {
			System.out.println("ERROR: Invalid stretched texture property on terrain object \"" + getPropertyValue("name") + ".");
			System.exit(1);
		}
		
		Point3D p;
		for(int i=0;i<object.faces.size();i++) {
			for(int j=0;j<object.faces.elementAt(i).points.size();j++) {
				p = object.faces.elementAt(i).points.elementAt(j);
				if(i == 0 && j == 0) {
					maxPoint = new Point3D(p.x, p.y, p.z);
					minPoint = new Point3D(p.x, p.y, p.z);
				}
				
				if(p.x > maxPoint.x) { maxPoint.x = p.x; }
				if(p.y > maxPoint.y) { maxPoint.y = p.y; }
				if(p.z > maxPoint.z) { maxPoint.z = p.z; }
				
				if(p.x < minPoint.x) { minPoint.x = p.x; }
				if(p.y < minPoint.y) { minPoint.y = p.y; }
				if(p.z < minPoint.z) { minPoint.z = p.z; }
			}
		}
		addProperty("maxpoint", maxPoint.x + ", " + maxPoint.y + ", " + maxPoint.z);
		addProperty("minpoint", minPoint.x + ", " + minPoint.y + ", " + minPoint.z);
		
		try {
			char lastChar = heightMapDirectory.getPath().charAt(heightMapDirectory.getPath().length() - 1);
			String fileName = "";
			if(!(lastChar == '\\' || lastChar == '/')) { fileName += '\\'; }
			fileName += getPropertyValue("heightmap");
			int length = getPropertyValue("heightmap").length();
			if(!getPropertyValue("heightmap").substring(length - 3, length).equalsIgnoreCase("bmp")) { fileName += ".bmp"; }
			File heightMapFile = new File(heightMapDirectory + fileName);
			if(!heightMapFile.exists()) {
				System.out.println("ERROR: Height map \"" + heightMapFile + "\" does not exist!");
				System.exit(1);
			}
			if(!heightMapFile.isFile()) {
				System.out.println("ERROR: Height map \"" + heightMapFile + "\" is not a file!");
				System.exit(1);
			}
			
			removeProperty("heightmap");
			
			generateTerrain(heightMapFile);
		}
		catch(Exception e) {
			System.out.println("ERROR: Error generating terrain from height map \"" + getPropertyValue("heightmap") + "\".");
			e.printStackTrace();
		}
	}
	
	private void generateTerrain(File heightMapFile) throws Exception {
		faces = new Vector<Face>();
		
		BufferedImage heightMap = ImageIO.read(heightMapFile);
		heightMapWidth = heightMap.getWidth() + 1;
		heightMapHeight = heightMap.getHeight() + 1;
		addProperty("width", Integer.toString(heightMapWidth));
		addProperty("height", Integer.toString(heightMapHeight));
		int size = (heightMapWidth - 1) * (heightMapHeight - 1);
		
		ByteArrayOutputStream baos = new ByteArrayOutputStream(size);
		ImageIO.write(heightMap, "bmp", baos);
		baos.flush();
		byte[] heightMapBytes = baos.toByteArray();
		baos.close();
		if(heightMapBytes.length > size * 3) {
			System.out.println("WARNING: Height map \"" + heightMapFile + "\" number of bytes read (" + heightMapBytes.length + ") does not match size of bitmap (" + (size * 3) + "), difference of " + (heightMapBytes.length - (size * 3)) + ". Excess bytes will be truncated.");
		}
System.out.println("NOTE: Number of pixels = width * height * 3 for rgb");
System.out.println("NOTE: Can't handle 256x256 - arrays can't go that big.");
		
		double terrainSizeX = maxPoint.x - minPoint.x;
		double terrainSizeY = maxPoint.y - minPoint.y;
		double terrainSizeZ = maxPoint.z - minPoint.z;
		
		double tileSizeX = terrainSizeX / heightMapWidth;
		double tileSizeY = terrainSizeX / heightMapHeight;
		
		double tilingTextureX = (heightMapWidth - 1) / heightMapWidth;
		double tilingTextureY = (heightMapHeight - 1) / heightMapHeight;
		
		int pixelDepthR = 0;
		int pixelDepthG = 0;
		int pixelDepthB = 0;
		int pixelCounter = 0;
		double x, y, z, tx, ty;
		Face f;
		for(int i=0;i<heightMapWidth-1;i++) {
			for(int j=0;j<heightMapHeight-1;j++) {
				pixelDepthR = heightMapBytes[pixelCounter++] + 128;
				pixelDepthG = heightMapBytes[pixelCounter++] + 128;
				pixelDepthB = heightMapBytes[pixelCounter++] + 128;
//System.out.println(pixelDepthR + ", " + pixelDepthG + ", " + pixelDepthB);
				
				x = ((i / heightMapWidth) * terrainSizeX) + minPoint.x;
				y = ((j / heightMapHeight) * terrainSizeY) + minPoint.y;
				z = ((pixelDepthR / 255.0) * terrainSizeZ) + minPoint.z;
				
// stretched textures???
				
				f = new Face(i+j, null, null);
				f.addPoint(new GamePoint3D(x, y, z, 0, 0, 0, x, y));
				f.addPoint(new GamePoint3D(x, y, z, 0, 0, 0, x, y));
				f.addPoint(new GamePoint3D(x, y, z, 0, 0, 0, x, y));
				f.addPoint(new GamePoint3D(x, y, z, 0, 0, 0, x, y));
				faces.add(f);
			}
		}
		
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		super.writeTo(out);
		
		out.println("RAW DATA GOES HERE");
	}
	
}
