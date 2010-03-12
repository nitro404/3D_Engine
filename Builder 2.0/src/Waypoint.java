import java.io.PrintWriter;

// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

public class Waypoint extends UniversalObject {
	
	public Point3D position;
	
	// create the Waypoint subclass (throw away the faces)
	public Waypoint(UniversalObject object) {
		super(object.objectIndex, null, null, object.properties, null);
		
		this.position = object.normal.getPosition();
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		// output the position
		out.println("\tPosition: " + this.position.x + ", " + this.position.y + ", " + this.position.z + ";");
		
		// output the properties
		if(this.properties != null) {
			out.println("\tProperties: " + this.properties.size() + ";");
			for(int i=0;i<this.properties.size();i++) {
				out.println("\t\t\"" + this.properties.elementAt(i).key + "\" => \"" + this.properties.elementAt(i).value + "\"");
			}
		}
	}
	
}
