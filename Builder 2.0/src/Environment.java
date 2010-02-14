// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

public class Environment extends UniversalObject {
	
	public static String[] skyboxTextureExtensions = {"-left", "-right", "-front", "-back", "-up", "-down"};
	
	// create the Environment subclass (throw away the transformations and faces)
	public Environment(UniversalObject object) {
		super(object.objectIndex, null, null, object.properties, null);
	}
	
}
