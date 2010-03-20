public class Plane3D {
	
	public Point3D normal;
	public double minusP0DotNormal;
	
	public final static int POINT_NEGATIVE_SIGN = -1;
	public final static int POINT_ZERO_SIGN = 0;
	public final static int POINT_POSITIVE_SIGN = 1;
	
	public final static int LINE_BACK_SIGN = -1;
	public final static int LINE_ON_SIGN = 0;
	public final static int LINE_FRONT_SIGN = 1;
	public final static int LINE_STRADDLES_SIGN = 2;
	
	public Plane3D() {
		this.normal = new Point3D();
		this.minusP0DotNormal = 0;
	}
	
	public Plane3D(Point3D normal, double minusP0DotNormal) {
		this.normal = normal;
		this.minusP0DotNormal = minusP0DotNormal;
	}
	
	public Plane3D(Point3D normal, Point3D point) {
		this.normal = normal;
		this.minusP0DotNormal = point.negative().dot(normal);
	}
	
	public double distanceToPoint(Point3D point) {
		return normal.dot(point) + this.minusP0DotNormal;
	}
	
	public Point3D projectionOfPoint(Point3D point) {
		double d = distanceToPoint(point);
		return new Point3D(point.subtract(normal.multiply(d)));
	}
	
	public int whereIsPoint(Point3D p) {
		double distance = distanceToPoint(p);
		if(distance < -(1.0e-10)) { return POINT_NEGATIVE_SIGN; }
		if(distance > 1.0e-10) { return POINT_POSITIVE_SIGN; }
		else { return POINT_ZERO_SIGN; }
	}
	
	public int whereIsLine(Point3D p1, Point3D p2) {
		int s1 = whereIsPoint(p1);
		int s2 = whereIsPoint(p2);
		if(s1 == s2) {
			if(s1 == POINT_POSITIVE_SIGN) { return LINE_FRONT_SIGN; }
			if(s2 == POINT_NEGATIVE_SIGN) { return LINE_BACK_SIGN; }
			return LINE_ON_SIGN;
		}
		return LINE_STRADDLES_SIGN;
	}
	
	public Point3D intersectionOf(Point3D p1, Point3D p2) {
		Point3D direction = p1.subtract(p2);
		Point3D normalizedDirection = direction.normalize();
		
		if(this.normalAlmostPerpendicular(normalizedDirection)) {
			if(this.whereIsPoint(p1) == POINT_POSITIVE_SIGN) {
				if(this.whereIsPoint(p2) != POINT_NEGATIVE_SIGN) {
					System.out.println("ERROR: Line does not intersect plane.");
					System.exit(1);
				}
				return binarySearchForIntersectingPoint(p1, p2);
			}
			else {
				if(this.whereIsPoint(p2) != POINT_POSITIVE_SIGN) {
					System.out.println("ERROR: Line does not intersect plane.");
					System.exit(1);
				}
				return binarySearchForIntersectingPoint(p2, p1);
			}
		}
		else {
			double t = -(this.distanceToPoint(p1)) / (this.normal.dot(normalizedDirection));
			return p1.add(normalizedDirection).multiply(t);
		}
	}
	
	public Point3D binarySearchForIntersectingPoint(Point3D p1, Point3D p2) {
		Point3D average = (p1.add(p2)).multiply(0.5);
		int averageSign = this.whereIsPoint(average);
		if(averageSign == POINT_ZERO_SIGN) { return average; }
		if(averageSign == POINT_POSITIVE_SIGN) {
			return binarySearchForIntersectingPoint(average, p2);
		}
		else {
			return binarySearchForIntersectingPoint(p1, average);
		}
	}
	
	public boolean normalAlmostPerpendicular(Point3D unitDirection) {
		return Math.abs(normal.dot(unitDirection)) < 1.0e-10;
	}
	
	public boolean normalAlmostParallel(Point3D unitDirection) {
		return Math.abs(normal.dot(unitDirection)) - 1.0 < 1.0e-10;
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
