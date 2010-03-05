// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

/*import java.util.Vector;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
*/
import java.io.*;

public class Terrain extends UniversalObject {
	
	public int heightMapWidth;
	public int heightMapHeight;
	public Point3D maxPoint;
	public Point3D minPoint;
	public GamePoint3D heightMapPoints[][];
	private int heightMap[][];
	private int stretched;
	
	public Terrain(UniversalObject object) {
		super(object.objectIndex, null, null, object.properties, null);
		
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
		
		addProperty("width", "64");
		addProperty("height", "64");
		
/*
		try {
			char lastChar = heightMapDirectory.getPath().charAt(heightMapDirectory.getPath().length() - 1);
			String fileName = "";
			if(!(lastChar == '\\' || lastChar == '/')) { fileName += '\\'; }
			fileName += getPropertyValue("heightmap");
			int length = getPropertyValue("heightmap").length();
			if(!getPropertyValue("heightmap").substring(length - 3, length).equalsIgnoreCase("raw")) { fileName += ".raw"; }
			File heightMapFile = new File(heightMapDirectory + fileName);
//			File heightMapFile = new File(heightMapDirectory + "\\test.raw");
//			File heightMapFile = new File(heightMapDirectory + "\\hugeTerrainHeightMap.raw");
			if(!heightMapFile.exists()) {
				System.out.println("ERROR: Height map \"" + heightMapFile + "\" does not exist!");
				System.exit(1);
			}
			if(!heightMapFile.isFile()) {
				System.out.println("ERROR: Height map \"" + heightMapFile + "\" is not a file!");
				System.exit(1);
			}
			
//			removeProperty("heightmap");
			
			generateTerrain(heightMapFile);
		}
		catch(Exception e) {
			System.out.println("ERROR: Error generating terrain from height map \"" + getPropertyValue("heightmap") + "\".");
			e.printStackTrace();
		}
*/
	}
	
	// can't be larger than 2^11
	private void generateTerrain(File heightMapFile) throws Exception {
PrintWriter test = null;
try {
test = new PrintWriter(new FileWriter("points.txt"));
}
catch(Exception wat) {}
//System.out.println("LOADING: " + heightMapFile.getPath());
		FileInputStream in = new FileInputStream(heightMapFile);
		
heightMapWidth = 64;
heightMapHeight = 64;
		
		addProperty("width", Integer.toString(heightMapWidth + 1));
		addProperty("height", Integer.toString(heightMapHeight + 1));
		
		double terrainSizeX = maxPoint.x - minPoint.x;
		double terrainSizeY = maxPoint.y - minPoint.y;
		double terrainSizeZ = maxPoint.z - minPoint.z;

/*
double tileSizeX = terrainSizeX / heightMapWidth;
double tileSizeY = terrainSizeX / heightMapHeight;



Point3D max = new Point3D(0, 0, 0);
Point3D min = new Point3D(0, 0, 0);
int maxPixelDepth = 0, minPixelDepth = 0;
//		int pixelCounter = 0;
*/
double tilingTextureX = (heightMapWidth - 1) / (double) heightMapWidth;
double tilingTextureY = (heightMapHeight - 1) / (double) heightMapHeight;
		
		double x, y, z, tx, ty;
		heightMap = new int[heightMapWidth][heightMapHeight];
		heightMapPoints = new GamePoint3D[heightMapWidth+1][heightMapHeight+1];
		for(int j=0;j<heightMapHeight;j++) {
			for(int i=0;i<heightMapWidth;i++) {
				heightMap[i][j] = in.read();
//System.out.println(heightMap[i][j]);
/*
				
if(i==0 && j==0) {
maxPixelDepth = heightMap[i][j];
minPixelDepth = heightMap[i][j];
}
else {
if(heightMap[i][j] > maxPixelDepth) { maxPixelDepth = heightMap[i][j]; }
if(heightMap[i][j] < minPixelDepth) { minPixelDepth = heightMap[i][j]; }
}
*/
			}
		}
		
		for(int i=0;i<heightMapWidth+1;i++) {
			for(int j=0;j<heightMapHeight+1;j++) {
				x = ((i / (double) heightMapWidth) * terrainSizeX) + minPoint.x;
				y = ((j / (double) heightMapHeight) * terrainSizeY) + minPoint.y;
//				z = ((heightMap[(i == heightMapWidth) ? i - 1 : i][(j == heightMapHeight) ? j - 1 : j] / 255.0) * terrainSizeZ) + minPoint.z;
				z = ((scaleHeight(i, j) / 255.0) * terrainSizeZ) + minPoint.z;
//System.out.println(z);
				
				// stretched?
				if(stretched == 0) {
					tx = tilingTextureX;
					ty = tilingTextureY;
				}
				else {
					tx = 1;
					ty = 1;
				}
				
				heightMapPoints[i][j] = new GamePoint3D(x, y, z, 0, 0, 0, tx, ty);
test.println(x + ", " + y + ", " + z + ", " + tx + ", " + ty);
/*
if(i == 0 && j == 0) {
max = new Point3D(x, y, z);
min = new Point3D(x, y, z);
}
else {
if(x > max.x) { max.x = x; }
if(y > max.y) { max.y = y; }
if(z > max.z) { max.z = z; }
if(x < min.x) { min.x = x; }
if(y < min.y) { min.y = y; }
if(z < min.z) { min.z = z; }
}
*/

//System.out.println("(" + i + ", " + j + ") [" + heightMap[i][j] + "] (" + x + ", " + y + ", " + z + ", " + tx + ", " + ty + ")");
//				pixelCounter++;
			}
		}
		
		/*for(int i=0;i<heightMapWidth+1;i++) {
			for(int j=0;j<heightMapHeight+1;j++) {
//System.out.println("(" + i + ", " + j + ") [" + heightMap[i][j] + "] (" + heightMapPoints[i][j].x + ", " + heightMapPoints[i][j].y + ", " + heightMapPoints[i][j].z + ", " + heightMapPoints[i][j].tx + ", " + heightMapPoints[i][j].ty + ")");
				heightMapPoints[i][j].z = scaleHeight(i, j);
//System.out.println("(" + i + ", " + j + ") [" + heightMap[i][j] + "] (" + heightMapPoints[i][j].x + ", " + heightMapPoints[i][j].y + ", " + heightMapPoints[i][j].z + ", " + heightMapPoints[i][j].tx + ", " + heightMapPoints[i][j].ty + ")");
//System.out.println();
			}
		}*/

/*
System.out.println("TERRAIN SIZE: " + terrainSizeX + ", " + terrainSizeY + ", " + terrainSizeZ);
System.out.println("TILE SIZE: " + tileSizeX + ", " + tileSizeY);
System.out.println("PIXEL DEPTH (MAX/MIN): " + maxPixelDepth + ", " + minPixelDepth);
System.out.println("TERRAIN MAX: (" + max.x + ", " + max.y + ", " + max.z + ")");
System.out.println("TERRAIN MIN: (" + min.x + ", " + min.y + ", " + min.z + ")");
System.out.println("BOX MAX: (" + maxPoint.x + ", " + maxPoint.y + ", " + maxPoint.z + ")");
System.out.println("BOX MIN: (" + minPoint.x + ", " + minPoint.y + ", " + minPoint.z + ")");
*/
test.close();
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		super.writeTo(out); 
		/*
		ByteArrayOutputStream byteStream; 
		DataOutputStream dataStream;
		for(int j=0;j<heightMapHeight+1;j++) {
			byteStream = new ByteArrayOutputStream();
			dataStream = new DataOutputStream(byteStream);
			for(int i=0;i<heightMapWidth+1;i++) {
				dataStream.writeDouble(heightMapPoints[i][j].x);
				dataStream.writeDouble(heightMapPoints[i][j].y);
				dataStream.writeDouble(heightMapPoints[i][j].z);
				dataStream.writeDouble(heightMapPoints[i][j].tx);
				dataStream.writeDouble(heightMapPoints[i][j].ty);
			}
			dataStream.flush();
			byte[] bytes = byteStream.toByteArray();
			for(int k=0;k<bytes.length;k++) {
				out.write(bytes[k]);
			}
		}
		out.println();
		*/
	}
	
	private double scaleHeight(int x, int y) {
		int sum = 0;
		int vertexCount = 0;
		
		if(x > 0 && y > 0) {
			sum += heightMap[x-1][y-1];
			vertexCount++;
//System.out.print(heightMap[x-1][y-1] + " + ");
		}
		
		if(x > 0 && y < heightMapHeight) {
			sum += heightMap[x-1][y];
			vertexCount++;
//System.out.print(heightMap[x-1][y] + " + ");
		}

		if(y > 0 && x < heightMapWidth) {
			sum += heightMap[x][y-1];
			vertexCount++;
//System.out.print(heightMap[x][y-1] + " + ");
		}
		
		if(x < heightMapWidth && y < heightMapHeight) {
			sum += heightMap[x][y];
			vertexCount++;
//System.out.print(heightMap[x][y] + " + ");
		}
//System.out.print(" = ");
		
		/*
		if(x > 0 && y < heightMapHeight) {
			vertexCount++;
			sum += heightMap[x-1][y];
		}
		
		if(y > 0 && x < heightMapWidth) {
			vertexCount++;
			sum += heightMap[x][y-1];
		}
		
		if(x > 0 && y > 0) {
			vertexCount++;
			sum += heightMap[x-1][y-1];
		}
		if(x < heightMapWidth && y < heightMapHeight) {
			vertexCount++;
			sum += heightMap[x][y];
		}
		*/
		
//System.out.println(sum + " / " + vertexCount + " = " + (sum / (double) vertexCount));
		return (sum / (double) vertexCount); 
	}
	
}
