import java.util.Vector;

// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

public class Geometry extends WorldObject {
	
	public int textureIndex;
	
	// create the Static Geometry subclass (throw away the properties)
	public Geometry(UniversalObject object, Vector<String> textureNames) {
		super(object.objectIndex, object.normal, object.inverse, null, object.faces);
		
		setTextureIndicies(faces, textureNames);
	}
	
}
