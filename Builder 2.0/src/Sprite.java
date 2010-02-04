public class Sprite extends UniversalObject {
	
	public int textureIndex;

	public Sprite(UniversalObject object, int textureIndex) {
		super(object.objectIndex, object.normal, object.inverse, object.properties, object.faces);
		this.textureIndex = textureIndex;
	}
	
}
