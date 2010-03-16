import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.util.StringTokenizer;

public class WorldcraftSidePlane {
	
	public Point3D[] points;
	public String textureName;
	public Point3D[] axis;
	public double[] axisOffset;
	public double[] axisScale;
	public double rotation;
	
	public Plane3D plane;
	public boolean isDegenerate;
	public Transformation transformation;
	
	public WorldcraftSidePlane() {
		this.points = new Point3D[3];
		this.textureName = null;
		this.axis = new Point3D[2];
		this.axisOffset = new double[2];
		this.axisScale = new double[2];
		this.rotation = 0;
		
		this.plane = null;
		this.isDegenerate = false;
		this.transformation = null;
	}
	
	public WorldcraftSidePlane(WorldcraftSidePlane sidePlane) {
		this.points = new Point3D[3];
		for(int i=0;i<3;i++) {
			this.points[i] = new Point3D(sidePlane.points[i].x, sidePlane.points[i].y, sidePlane.points[i].z);
		}
		this.textureName = new String(sidePlane.textureName);
		this.axis = new Point3D[2];
		this.axisOffset = new double[2];
		this.axisScale = new double[2];
		for(int i=0;i<2;i++) {
			this.axis[i] = new Point3D(sidePlane.axis[i].x, sidePlane.axis[i].y, sidePlane.axis[i].z);
			this.axisOffset[i] = sidePlane.axisOffset[i];
			this.axisScale[i] = sidePlane.axisScale[i];
		}
		this.rotation = sidePlane.rotation;
		if(sidePlane.plane == null) { this.plane = null; }
		else { this.plane = new Plane3D(new Point3D(sidePlane.plane.normal.x, sidePlane.plane.normal.y, sidePlane.plane.normal.z),
										sidePlane.plane.minusP0DotNormal); }
		this.isDegenerate = sidePlane.isDegenerate;
		if(sidePlane.transformation == null) { this.transformation = null; }
		else { this.transformation = new Transformation(sidePlane.transformation); }
	}
	
	public WorldcraftSidePlane(String input) {
		try {
			this.parseFrom(input);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid worldcraft side plane in map file.");
			System.exit(1);
		}
	}
	
	private void parseFrom(String input) throws Exception {
		if(input == null) { throw new NullPointerException(); }
		
		this.points = new Point3D[3];
		this.textureName = null;
		this.axis = new Point3D[2];
		this.axisOffset = new double[2];
		this.axisScale = new double[2];
		this.rotation = 0;
		
		this.plane = null;
		this.isDegenerate = false;
		this.transformation = null;
		
		double x, y, z;
		
		StringTokenizer sidePlaneTokenizer = new StringTokenizer(input, " ", false);
		
		if(sidePlaneTokenizer.countTokens() != 31) {
			System.out.println("ERROR: Invalid number of values (" + sidePlaneTokenizer.countTokens() + ") found in worldcraft side plane: \"" + input + "\", expected 31.");
			System.exit(1);
		}
		
		for(int i=0;i<3;i++) {
			if(sidePlaneTokenizer.nextToken().charAt(0) != '(') {
				System.out.print("ERROR: Expected \"(\" in worldcraft side plane: \"" + input + "\" before ");
				if(i == 0) { System.out.print("first"); }
				if(i == 1) { System.out.print("second"); }
				if(i == 2) { System.out.print("third"); }
				System.out.println(" point.");
				System.exit(1);
			}
			x = Double.valueOf(sidePlaneTokenizer.nextToken());
			y = Double.valueOf(sidePlaneTokenizer.nextToken());
			z = Double.valueOf(sidePlaneTokenizer.nextToken());
			if(sidePlaneTokenizer.nextToken().charAt(0) != ')') {
				System.out.print("ERROR: Expected \")\" in worldcraft side plane: \"" + input + "\" after ");
				if(i == 0) { System.out.print("first"); }
				if(i == 1) { System.out.print("second"); }
				if(i == 2) { System.out.print("third"); }
				System.out.println(" point.");
				System.exit(1);
			}
			points[i] = new Point3D(x, y, z);
		}
	
		textureName = sidePlaneTokenizer.nextToken();
		
		for(int i=0;i<2;i++) {
			if(sidePlaneTokenizer.nextToken().charAt(0) != '[') {
				System.out.print("ERROR: Expected \"[\" in worldcraft side plane: \"" + input + "\" before ");
				if(i == 0) { System.out.print("x"); }
				if(i == 1) { System.out.print("y"); }
				System.out.println(" axis.");
				System.exit(1);
			}
			x = Double.valueOf(sidePlaneTokenizer.nextToken());
			y = Double.valueOf(sidePlaneTokenizer.nextToken());
			z = Double.valueOf(sidePlaneTokenizer.nextToken());
			axis[i] = new Point3D(x, y, z);
			axisOffset[i] = Double.valueOf(sidePlaneTokenizer.nextToken());
			if(sidePlaneTokenizer.nextToken().charAt(0) != ']') {
				System.out.print("ERROR: Expected \"]\" in worldcraft side plane: \"" + input + "\" after ");
				if(i == 0) { System.out.print("x"); }
				if(i == 1) { System.out.print("y"); }
				System.out.println(" axis.");
				System.exit(1);
			} 
		}
		
		rotation = Double.valueOf(sidePlaneTokenizer.nextToken());
		
		for(int i=0;i<2;i++) {
			axisScale[i] = Double.valueOf(sidePlaneTokenizer.nextToken());
		}
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof WorldcraftSidePlane)) { return false; }
		
		WorldcraftSidePlane p = (WorldcraftSidePlane) o;
		
		if(this.points == null && p.points != null ||
		   this.points != null && p.points == null) { return false; }
		if(this.points != null && p.points != null) {
			for(int i=0;i<3;i++) {
				if(!(this.points[i].equals(p.points[i]))) {
					return false;
				}
			}
		}
		
		if(this.textureName == null && p.textureName != null ||
		   this.textureName != null && p.textureName == null) { return false; }
		if(this.textureName != null && p.textureName != null &&
		   !(this.textureName.equalsIgnoreCase((p.textureName)))) { return false; }
		
		if(this.axis == null && p.axis != null ||
		   this.axis != null && p.axis == null) { return false; }
		if(this.axisOffset == null && p.axisOffset != null ||
		   this.axisOffset != null && p.axisOffset == null) { return false; }
		if(this.axisScale == null && p.axisScale != null ||
		   this.axisScale != null && p.axisScale == null) { return false; }
		if(this.axis != null && p.axis != null &&
		   this.axisOffset != null && this.axisScale != null &&
		   this.axisScale != null && this.axisScale != null) {
			for(int i=0;i<2;i++) {
				if(!(this.axis[i].equals(p.axis[i])) ||
				   this.axisOffset[i] != p.axisOffset[i] ||
				   this.axisScale[i] != p.axisScale[i]) {
					return false;
				}
			}
		}
		
		if(this.plane == null && p.plane != null ||
		   this.plane != null && p.plane == null) { return false; }
		if(this.plane != null && p.plane != null &&
		   !(this.plane.equals(p.plane))) { return false; }
		
		if(this.transformation == null && p.transformation != null ||
		   this.transformation != null && p.transformation == null) { return false; }
		if(this.transformation != null && p.transformation != null &&
		   !(this.transformation.equals(p.transformation))) { return false; }
		
		return this.rotation == p.rotation && 
			   this.isDegenerate == p.isDegenerate;
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		DecimalFormat df = new DecimalFormat("#");
		for(int i=0;i<3;i++) {
			out.print("( ");
			out.print(df.format(points[i].x) + " ");
			out.print(df.format(points[i].y) + " ");
			out.print(df.format(points[i].z));
			out.print(" ) ");
		}
		out.print(textureName + " ");
		for(int i=0;i<2;i++) {
			out.print("[ ");
			out.print(df.format(axis[i].x) + " ");
			out.print(df.format(axis[i].y) + " ");
			out.print(df.format(axis[i].z) + " ");
			out.print(df.format(axisOffset[i]));
			out.print(" ] ");
		}
		out.print(df.format(rotation));
		for(int i=0;i<2;i++) {
			out.print(" " + df.format(axisScale[i]));
		}
		out.println();
	}
	
	public String toString() {
		String s = new String();
		DecimalFormat df = new DecimalFormat("#");
		for(int i=0;i<3;i++) {
			s += "( ";
			s += df.format(points[i].x) + " ";
			s += df.format(points[i].y) + " ";
			s += df.format(points[i].z);
			s += " ) ";
		}
		s += textureName + " ";
		for(int i=0;i<2;i++) {
			s += "[ ";
			s += df.format(axis[i].x) + " ";
			s += df.format(axis[i].y) + " ";
			s += df.format(axis[i].z) + " ";
			s += df.format(axisOffset[i]);
			s += " ] ";
		}
		s += df.format(rotation);
		for(int i=0;i<2;i++) {
			s += " " + df.format(axisScale[i]);
		}
		return s;
	}
	
}
