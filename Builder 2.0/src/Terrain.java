// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.util.Vector;

public class Terrain extends WorldObject {
	
	public int heightMapIndex;
	
	public Terrain(UniversalObject object, Vector<String> textureNames, Vector<HeightMap> heightMaps, Vector<Shader> shaders) {
		super(object.objectIndex, object.normal, object.inverse, object.properties, null, new BoundingBox(object.faces));
		
		heightMapIndex = -1;
		HeightMap heightMap = null;
		String currentHeightMapIdentifier;
		String heightMapIdentifier = getPropertyValue("heightmap");
		for(int j=0;j<heightMaps.size();j++) {
			currentHeightMapIdentifier = heightMaps.elementAt(j).name;
			if(currentHeightMapIdentifier.equalsIgnoreCase(heightMapIdentifier)) {
				heightMapIndex = heightMaps.elementAt(j).heightMapFileIndex;
				heightMap = heightMaps.elementAt(j);
				break;
			}
		}
		
		/*
		Point3D p;
		Point3D maxPoint = null;
		Point3D minPoint = null;
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
		*/
		
		setPropertyValue("heightmap", Integer.toString(heightMapIndex));
		setPropertyValue("texturemap", Integer.toString(getTextureIndex(getPropertyValue("texturemap"), textureNames)));
		setShaderIndex(getPropertyValue("shader"), shaders);
		
		addProperty("width", Integer.toString(heightMap.width));
		addProperty("height", Integer.toString(heightMap.height));
	}
	
}
