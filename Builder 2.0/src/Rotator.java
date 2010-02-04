public class Rotator extends UniversalObject {
	
	public int textureIndex;

	public Rotator(UniversalObject object, int textureIndex) {
		super(object.objectIndex, object.normal, object.inverse, object.properties, object.faces);
		this.textureIndex = textureIndex;
	}
	
}
