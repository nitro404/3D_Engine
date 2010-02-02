import java.io.*;
import java.util.StringTokenizer;

public class GamePoint3D extends Point3D {
	
	public double nx, ny, nz, tx, ty; 
	
	public GamePoint3D(double[] points) {
		super(points[0], points[1], points[2]);
		this.nx = points[3];
		this.ny = points[4];
		this.nz = points[5];
		this.tx = points[6];
		this.ty = points[7];
	}
	
	public GamePoint3D(double x, double y, double z, double nx, double ny, double nz, double tx, double ty) {
		super(x, y, z);
		this.nx = nx;
		this.ny = ny;
		this.nz = nz;
		this.tx = tx;
		this.ty = ty;
	}
	
	public GamePoint3D(BufferedReader in) {
		super();
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid game point in map file.");
			System.exit(1);
		}
	}
	
	public void readFrom(BufferedReader in) throws Exception {
		String input;
		
		// input the game point
		input = in.readLine().trim();
		super.parseFrom(input);
		StringTokenizer st = new StringTokenizer(input.substring(0, input.lastIndexOf(';')).trim(), ",", false);
		if(st.countTokens() != 8) {
			System.out.println("ERROR: Invalid game point parsed with " + st.countTokens() + ", expected 8.");
			System.exit(1);
		}
		st.nextToken();
		st.nextToken();
		st.nextToken();
		this.nx = Double.valueOf(st.nextToken().trim());
		this.ny = Double.valueOf(st.nextToken().trim());
		this.nz = Double.valueOf(st.nextToken().trim());
		this.tx = Double.valueOf(st.nextToken().trim());
		this.ty = Double.valueOf(st.nextToken().trim());
	}
	
}
