public class Geometry extends UniversalObject {
	
	public int textureIndex;
	
	public Geometry(UniversalObject object, int textureIndex) {
		super(object.objectIndex, object.normal, object.inverse, null, object.faces);
		this.textureIndex = textureIndex;
	}
	
}
