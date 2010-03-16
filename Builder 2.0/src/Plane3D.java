public class Plane3D {
	
	Point3D normal;
	double minusP0DotNormal;
	
	public Plane3D(Point3D normal, double minusP0DotNormal) {
		this.normal = normal;
		this.minusP0DotNormal = minusP0DotNormal;
	}
	
	public Plane3D(Point3D normal, Point3D point) {
		this.normal = normal;
		this.minusP0DotNormal = point.negative().dot(normal);
	}
	
	double distanceToPoint(Point3D point) {
		return normal.dot(point) + this.minusP0DotNormal;
	}
	
	Point3D projectionOfPoint(Point3D point) {
		double d = distanceToPoint(point);
		return new Point3D(point.x - (normal.x * d),
						   point.y - (normal.y * d),
						   point.z - (normal.z * d));
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof Plane3D)) { return false; }
		
		Plane3D p = (Plane3D) o;
		return this.normal == p.normal && this.minusP0DotNormal == p.minusP0DotNormal; 
	}
	
	public String toString() {
		return "[ " + this.normal + ", " + this.minusP0DotNormal + " ]"; 
	}
	
}
