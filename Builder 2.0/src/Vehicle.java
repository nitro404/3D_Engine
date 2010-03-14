// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

public class Vehicle extends WorldObject {
	
	public int textureIndex;
	
	// create the Vehicle subclass
	public Vehicle(UniversalObject object, int textureIndex) {
		super(object.objectIndex, object.normal, object.inverse, object.properties, object.faces);
		this.textureIndex = textureIndex;
	}
	
}
