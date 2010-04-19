// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.io.PrintWriter;
import java.util.Vector;

public class Sprite extends WorldObject {
	
	public Point3D position;
	
	// create the Sprite subclass (throw away the transformations and faces)
	public Sprite(UniversalObject object, Vector<String> textureNames, Vector<Shader> shaders) {
		super(object.objectIndex, null, null, object.properties, null, new BoundingBox(object.faces));
		
		setPropertyValue("picture", Integer.toString(getTextureIndex(getPropertyValue("picture"), textureNames)));
		setShaderIndex(getPropertyValue("shader"), shaders);
		
		// get the position associated with the normal transformation and store that instead
		this.position = object.normal.getPosition();
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		// print the object type
		out.println("\t\"type\" => \"sprite\"");
		
		// output the position
		out.println("\tPosition: " + this.position.x + ", " + this.position.y + ", " + this.position.z + ";");
		
		// output the properties
		if(this.properties != null) {
			out.println("\tProperties: " + this.properties.size() + ";");
			for(int i=0;i<this.properties.size();i++) {
				out.println("\t\t\"" + this.properties.elementAt(i).key + "\" => \"" + this.properties.elementAt(i).value + "\"");
			}
		}
		
		// output the bounding box
		this.box.writeTo(out);
	}
	
}
