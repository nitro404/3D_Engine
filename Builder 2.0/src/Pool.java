import java.util.Vector;

// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

public class Pool extends WorldObject {
	
	public int textureIndex;
	
	// create the Pool subclass
	public Pool(UniversalObject object, Vector<String> textureNames, Vector<AnimatedTexture> animatedTextures, Vector<Shader> shaders) {
		super(object.objectIndex, object.normal, object.inverse, object.properties, object.faces, new BoundingBox(object.faces));
		
		setAnimatedTextureIndicies(faces, textureNames, animatedTextures);
		setShaderIndex(getPropertyValue("shader"), shaders);
	}
	
}
