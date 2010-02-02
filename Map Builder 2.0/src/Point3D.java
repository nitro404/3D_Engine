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
	
	public void readFrom(BufferedReader in) throws Exception {
		String input;
		
		// input the point
		input = in.readLine().trim();
		StringTokenizer st = new StringTokenizer(input.substring(0, input.lastIndexOf(';')).trim(), ",", false);
		if(st.countTokens() != 3) {
			System.out.println("ERROR: Invalid point parsed with " + st.countTokens() + ", expected 3.");
			System.exit(1);
		}
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
		this.x = Double.valueOf(st.nextToken().trim());
		this.y = Double.valueOf(st.nextToken().trim());
		this.z = Double.valueOf(st.nextToken().trim());
	}
	
}
