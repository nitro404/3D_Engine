import java.io.*;
import java.util.StringTokenizer;

public class Transformation {
	
	public double[][] transformation;
	
	public Transformation(BufferedReader in) {
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid transformation in map file.");
			System.exit(1);
		}
	}
	
	public Point3D getPosition() {
		return new Point3D(transformation[3][0], transformation[3][1], transformation[3][2]);
	}
	
	private void readFrom(BufferedReader in) throws Exception {
		String input;
		
		// input the transformation
		input = in.readLine().trim();
		StringTokenizer st = new StringTokenizer(input.substring(0, input.lastIndexOf(';')).trim(), ",", false);
		if(st.countTokens() != 16) {
			System.out.println("ERROR: Invalid transformation parsed with " + st.countTokens() + ", expected 16.");
			System.exit(1);
		}
		
		// input the transformation points
		transformation = new double[4][4];
		for(int i=0;i<4;i++) {
			for(int j=0;j<4;j++) {
			transformation[i][j] = Double.valueOf(st.nextToken());
			}
		}
	}
	
}
