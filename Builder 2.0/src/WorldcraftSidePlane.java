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
	public boolean planeIsDegenerate;
	public Transformation transformation;
	
	public WorldcraftSidePlane() {
		this.points = new Point3D[3];
		this.textureName = null;
		this.axis = new Point3D[2];
		this.axisOffset = new double[2];
		this.axisScale = new double[2];
		this.rotation = 0;
		
		this.plane = null;
		this.planeIsDegenerate = false;
		this.transformation = null;
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
		this.planeIsDegenerate = false;
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
