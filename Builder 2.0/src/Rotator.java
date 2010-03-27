import java.util.Vector;

// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

public class Rotator extends WorldObject {
	
	public int textureIndex;

	// create the Rotator subclass
	public Rotator(UniversalObject object, Vector<String> textureNames) {
		super(object.objectIndex, object.normal, object.inverse, object.properties, object.faces);
		
		setTextureIndicies(faces, textureNames);
	}
	
}
