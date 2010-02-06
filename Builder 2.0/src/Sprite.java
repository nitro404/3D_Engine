import java.io.PrintWriter;

public class Sprite extends UniversalObject {
	
	public int textureIndex;
	public Point3D position;
	public Point3D max;
	public Point3D min;

	public Sprite(UniversalObject object, int textureIndex) {
		super(object.objectIndex, null, null, object.properties, null);
		
		// store the texture index associated with the picture
		this.textureIndex = textureIndex;
		
		// get the position associated with the normal transformation and store that instead
		this.position = object.normal.getPosition();
		
		// calculate the max and min x, y, z values of the faces and store them instead
		for(int i=0;i<object.faces.size();i++) {
			for(int j=0;j<object.faces.elementAt(i).points.size();j++) {
				Point3D p = object.faces.elementAt(i).points.elementAt(j);
				if(i==0 && j==0) {
					this.max = new Point3D(p.x, p.y, p.z);
					this.min = new Point3D(p.x, p.y, p.z);
				}
				else {
					if(p.x > this.max.x) { this.max.x = p.x; }
					if(p.y > this.max.y) { this.max.y = p.y; }
					if(p.z > this.max.z) { this.max.z = p.z; }
					if(p.x < this.min.x) { this.min.x = p.x; }
					if(p.y < this.min.y) { this.min.y = p.y; }
					if(p.z < this.min.z) { this.min.z = p.z; }
				}
			}
		}
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
		out.println("\tMaximum: " + this.max.x + ", " + this.max.y + ", " + this.max.z + ";");
		out.println("\tMinimum: " + this.min.x + ", " + this.min.y + ", " + this.min.z + ";");
	}
	
}
