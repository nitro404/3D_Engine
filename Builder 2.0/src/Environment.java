import java.util.Vector;

// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

public class Environment extends WorldObject {
	
	public static String[] skyboxTextureExtensions = {"-left", "-right", "-front", "-back", "-up", "-down"};
	public static String[] skyboxTextureExtensionsAlt = {"_west", "_east", "_north", "_south", "_up", "_down"};
	
	// create the Environment subclass (throw away the transformations and faces)
	public Environment(UniversalObject object, Vector<String> textureNames, Vector<Shader> shaders) {
		super(object.objectIndex, null, null, object.properties, null);
		
		setShaderIndex(getPropertyValue("shader"), shaders);
		
		String baseTextureName = getPropertyValue("skyboxtexture");
		removeProperty("skyboxtexture");
		
		int textureIndex;
		for(int i=0;i<skyboxTextureExtensions.length;i++) {
			textureIndex = getTextureIndex(baseTextureName + skyboxTextureExtensions[i], textureNames);
			if(textureIndex == -1) { textureIndex = getTextureIndex(baseTextureName + skyboxTextureExtensionsAlt[i], textureNames); }
			if(textureIndex == -1) {
				System.out.println("ERROR: Missing skybox texture: \"" + baseTextureName + skyboxTextureExtensions[i] + "\".");
				System.exit(1);
			}
			addProperty("skyboxtexture" + skyboxTextureExtensions[i], Integer.toString(textureIndex));
		}
	}
	
}
