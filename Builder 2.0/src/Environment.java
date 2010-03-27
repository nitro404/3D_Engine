import java.util.Vector;

// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

public class Environment extends WorldObject {
	
	public static String[] skyboxTextureExtensions = {"-left", "-right", "-front", "-back", "-up", "-down"};
	
	// create the Environment subclass (throw away the transformations and faces)
	public Environment(UniversalObject object, Vector<String> textureNames) {
		super(object.objectIndex, null, null, object.properties, null);
		
		String baseTextureName = getPropertyValue("skyboxtexture");
		removeProperty("skyboxtexture");
		
		for(int i=0;i<skyboxTextureExtensions.length;i++) {
			addProperty("skyboxtexture" + skyboxTextureExtensions[i], Integer.toString(getTextureIndex(baseTextureName + skyboxTextureExtensions[i], textureNames)));
		}
	}
	
}
