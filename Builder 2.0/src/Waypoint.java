// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

public class Waypoint extends UniversalObject {
	
	// create the Waypoint subclass (throw away the faces)
	public Waypoint(UniversalObject object) {
		super(object.objectIndex, object.normal, object.inverse, object.properties, null);
	}
	
}
