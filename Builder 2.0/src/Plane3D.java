public class Plane3D {
	
	Point3D normal;
	double minusP0DotNormal;
	
	public Plane3D(Point3D normal, Point3D point) {
		this.normal = normal;
		this.minusP0DotNormal = point.negative().dot(normal);
	}
	
}
