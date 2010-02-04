public class Translator extends UniversalObject {
	
	public int textureIndex;

	public Translator(UniversalObject object, int textureIndex) {
		super(object.objectIndex, object.normal, object.inverse, object.properties, object.faces);
		this.textureIndex = textureIndex;
	}
	
}
