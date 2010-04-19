import java.io.PrintWriter;
import java.util.Vector;

public class BoundingBox {
	
	public Point3D max;
	public Point3D min;
	
	public BoundingBox() {
		this.max = new Point3D();
		this.min = new Point3D();
	}
	
	public BoundingBox(Point3D max, Point3D min) {
		this.max = (max == null) ? new Point3D() : max;
		this.min = (min == null) ? new Point3D() : min;
	}
	
	public BoundingBox(double maxX, double maxY, double maxZ, double minX, double minY, double minZ) {
		this.max = new Point3D(maxX, maxY, maxZ);
		this.min = new Point3D(minX, minY, minZ);
	}
	
	public BoundingBox(Vector<Face> faces) {
		if(faces == null || faces.size() == 0) {
			this.max = new Point3D();
			this.min = new Point3D();
		}
		else {
			// calculate the max and min x, y, z values of the faces and store them
			for(int i=0;i<faces.size();i++) {
				for(int j=0;j<faces.elementAt(i).points.size();j++) {
					Point3D p = faces.elementAt(i).points.elementAt(j);
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
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		out.println("\tMaximum: " + this.max.x + ", " + this.max.y + ", " + this.max.z + ";");
		out.println("\tMinimum: " + this.min.x + ", " + this.min.y + ", " + this.min.z + ";");
	}
	
}
