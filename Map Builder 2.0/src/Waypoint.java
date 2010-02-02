import java.util.Vector;

public class Waypoint extends UniversalObject {
	
	public int objectIndex;
	public Transformation normal;
	public Transformation inverse;
	public Vector<Property> properties;
	public Vector<Face> faces;
	
	public Waypoint(UniversalObject object) {
		this.objectIndex = object.objectIndex;
		this.normal = object.normal;
		this.inverse = object.inverse;
		this.properties = object.properties;
		this.faces = new Vector<Face>();
	}
	
}
