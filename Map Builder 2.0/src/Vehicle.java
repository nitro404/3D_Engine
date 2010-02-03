public class Vehicle extends UniversalObject {
	
	public int textureIndex;
	
	public Vehicle(UniversalObject object, int textureIndex) {
		super(object.objectIndex, object.normal, object.inverse, object.properties, object.faces);
		this.textureIndex = textureIndex;
	}
	
}
