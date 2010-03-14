// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.io.*;
import java.util.StringTokenizer;

public class Point3D {
	
	public double x, y, z;
	
	public Point3D() { }
	
	public Point3D(double x, double y, double z) {
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	public Point3D(BufferedReader in) {
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid point in map file.");
			System.exit(1);
		}
	}
	
	public Point3D(String data) {
		this.parseFrom(data);
	}
	
	public Point3D negative() {
		return new Point3D(-x, -y, -z);
	}
	
	public Point3D minus(double c) {
		return new Point3D(x - c, y - c, z - c);
	}
	
	public Point3D minus(Point3D p) {
		return new Point3D(x - p.x, y - p.y, z - p.z);
	}
	
	public Point3D square() {
		return new Point3D(x * x, y * y, z * z);
	}
	
	public Point3D multiplyBy(double c) {
		return new Point3D(x * c, y * c, z * c);
	}
	
	public Point3D multiplyBy(Point3D p) {
		return new Point3D(x * p.x, y * p.y, z * p.z);
	}
	
	public double dot(Point3D p) {
		return (x * p.x) + (y * p.y) + (z * p.z);
	}
	
	public Point3D cross(Point3D p) {
		return new Point3D((y * p.z) - (z * p.y), (z * p.x) - (x * p.z), (x * p.y) - (y * p.x));
	}
	
	public double squaredLength() {
		return (x * x) + (y * y) + (z * z);
	}
	
	public void readFrom(BufferedReader in) throws Exception {
		String input;
		
		// input the point
		input = in.readLine().trim();
		StringTokenizer st = new StringTokenizer(input.substring(0, input.lastIndexOf(';')).trim(), ",", false);
		if(st.countTokens() != 3) {
			System.out.println("ERROR: Invalid point parsed with " + st.countTokens() + ", expected 3.");
			System.exit(1);
		}
		
		// parse the points and store them
		this.x = Double.valueOf(st.nextToken().trim());
		this.y = Double.valueOf(st.nextToken().trim());
		this.z = Double.valueOf(st.nextToken().trim());
	}
	
	public void parseFrom(String data) {
		// input the point
		StringTokenizer st = new StringTokenizer(data.substring(0, data.lastIndexOf(';')).trim(), ",", false);
		if(st.countTokens() < 3) {
			System.out.println("ERROR: Invalid point parsed with " + st.countTokens() + ", expected 3.");
			System.exit(1);
		}
		
		// parse the points and store them
		this.x = Double.valueOf(st.nextToken().trim());
		this.y = Double.valueOf(st.nextToken().trim());
		this.z = Double.valueOf(st.nextToken().trim());
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		// output the points
		out.print(x + ", " + y + ", " + z);
	}
	
}
