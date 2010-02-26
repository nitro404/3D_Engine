// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.util.Vector;
import java.io.*;

public class Terrain extends UniversalObject {
	
	public Terrain(UniversalObject object, File heightMapDirectory) {
		super(object.objectIndex, object.normal, object.inverse, object.properties, null);
		try {
//			faces = generateTerrain(heightMapDirectory + "\\" + getPropertyValue("heightmap") + ".bmp");
		}
		catch(Exception e) {
			System.out.println("ERROR: Error generating terrain from height map \"" + getPropertyValue("heightmap") + "\".");
			e.printStackTrace();
		}
	}
	
	private static Vector<Face> generateTerrain(String fileName) throws IOException {
		Vector<Face> terrain = new Vector<Face>();
		int size = 8;
		byte[] terrainBuffer = new byte[size * size];
		int bytesRead = 0;
		int bytesToRead = terrainBuffer.length;
		BufferedInputStream in = new BufferedInputStream(new FileInputStream(fileName));
		int read;
//int i = 0;
		while(bytesToRead > 0) {
			read = in.read(terrainBuffer, bytesRead, bytesToRead);
//System.out.println(terrainBuffer[i++]);
			bytesRead += read;
			bytesToRead -= read;
		}
		return terrain;
	}
	
}
