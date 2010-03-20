// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.io.*;
import java.util.StringTokenizer;

public class Transformation {
	
	public double[][] transformation;
	
	public Transformation() {
		this.transformation = new double[4][4];
		for(int i=0;i<4;i++) {
			for(int j=0;j<4;j++) {
				this.transformation[i][j] = 0;
			}
		}
	}
	
	public Transformation(double m11, double m12, double m13, double m14,
						  double m21, double m22, double m23, double m24,
						  double m31, double m32, double m33, double m34,
						  double m41, double m42, double m43, double m44) {
		this.transformation = new double[4][4];
		set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
	}
	
	public Transformation(Transformation t) {
		this.transformation = new double[4][4];
		set(t);
	}
	
	public Transformation(BufferedReader in) {
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid transformation in map file.");
			System.exit(1);
		}
	}
	
	public void set(Transformation t) {
		if(t == null) {
			set(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
			return;
		}
		set(t.transformation[0][0], t.transformation[0][1], t.transformation[0][2], t.transformation[0][3],
			t.transformation[1][0], t.transformation[1][1], t.transformation[1][2], t.transformation[1][3],
			t.transformation[2][0], t.transformation[2][1], t.transformation[2][2], t.transformation[2][3],
			t.transformation[3][0], t.transformation[3][1], t.transformation[3][2], t.transformation[3][3]);
//		fixNegatives();
	}
	
	public void set(double m11, double m12, double m13, double m14,
					double m21, double m22, double m23, double m24,
					double m31, double m32, double m33, double m34,
					double m41, double m42, double m43, double m44) {
		transformation[0][0] = m11; transformation[0][1] = m12; transformation[0][2] = m13; transformation[0][3] = m14;
		transformation[1][0] = m21; transformation[1][1] = m22; transformation[1][2] = m23; transformation[1][3] = m24;
		transformation[2][0] = m31; transformation[2][1] = m32; transformation[2][2] = m33; transformation[2][3] = m34;
		transformation[3][0] = m41; transformation[3][1] = m42; transformation[3][2] = m43; transformation[3][3] = m44;
//		fixNegatives();
	}
	
	public static Transformation Identity() {
		return new Transformation(1, 0, 0, 0,
								  0, 1, 0, 0,
								  0, 0, 1, 0,
								  0, 0, 0, 1);
	}
	
	public Point3D getPosition() {
		return new Point3D(transformation[3][0], transformation[3][1], transformation[3][2]);
	}
	
	public void postScaleBy(Point3D scale) {
		Transformation scalingMatrix = new Transformation(scale.x,       0,       0,       0,
																0, scale.y,       0,       0,
																0,       0, scale.z,       0,
																0,       0,       0,       1);
		this.postMultiplyBy(scalingMatrix);
	}
	
	public void rotateToAxes(Point3D xAxis, Point3D yAxis, Point3D zAxis) { 
		this.set(xAxis.x, xAxis.y, xAxis.z, 0,
				 yAxis.x, yAxis.y, yAxis.z, 0,
				 zAxis.x, zAxis.y, zAxis.z, 0,
				       0,       0,       0, 1);
	}
	
	public void postRotateXAxis(double a) {
		double c = Math.cos(a);
		double s = Math.sin(a);
		Transformation xAxisRotationMatrix = new Transformation( 1,  0,  0,  0,
																 0,  c, -s,  0,
																 0,  s,  c,  0,
																 0,  0,  0,  1);
		this.postMultiplyBy(xAxisRotationMatrix);
	}
	
	public void postRotateYAxis(double a) {
		double c = Math.cos(a);
		double s = Math.sin(a);
		Transformation yAxisRotationMatrix = new Transformation( c,  0,  s,  0,
																 0,  1,  0,  0,
																-s,  0,  c,  0,
																 0,  0,  0,  c);
		this.postMultiplyBy(yAxisRotationMatrix);
	}
	
	public void postRotateZAxis(double a) {
		double c = Math.cos(a);
		double s = Math.sin(a);
		Transformation zAxisRotationMatrix = new Transformation( c, -s,  0,  0,
																 s,  c,  0,  0,
																 0,  0,  1,  0,
																 0,  0,  0,  1);
		this.postMultiplyBy(zAxisRotationMatrix);
	}
	
	public void postTranslateBy(Point3D translation) { // matrix * translation
		Transformation translationMatrix = new Transformation(1, 0, 0, translation.x,
															  0, 1, 0, translation.y,
															  0, 0, 1, translation.z,
															  0, 0, 0, 1);
		this.postMultiplyBy(translationMatrix);
	}
	
	public void postMultiplyBy(Transformation t) {
		Transformation t2 = new Transformation();
		for(int i=0;i<4;i++) {
			for(int j=0;j<4;j++) {
				for(int k=0;k<4;k++) {
					t2.transformation[i][j] += this.transformation[i][k] * t.transformation[k][j];
				}
			}
		}
		set(t2);
//		fixNegatives();
	}
	
/*	private void fixNegatives() {
		for(int i=0;i<4;i++) {
			for(int j=0;j<4;j++) {
				if(transformation[i][j] == 0) {
					transformation[i][j] = 0; // replace -0.0 with 0.0
				}
			}
		}
	}
*/
	
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
	
	public void writeTo(PrintWriter out) throws Exception {
		// output all of the points in the transformation
		for(int i=0;i<4;i++) {
			for(int j=0;j<4;j++) {
				out.print(transformation[i][j]);
				if(!(i == 3 && j == 3)) {
					out.print(", ");
				}
			}
		}
	}
	
	public boolean equals(Object o) {
		if(o == null || !(o instanceof Transformation)) { return false; }
		
		Transformation t = (Transformation) o;
		
		for(int i=0;i<4;i++) {
			for(int j=0;j<4;j++) {
				if(this.transformation[i][j] != t.transformation[i][j]) {
					return false;
				}
			}
		}
		return true;
	}
	
	public String toString() {
		String s = new String("");
		for(int i=0;i<4;i++) {
			for(int j=0;j<4;j++) {
				s += this.transformation[i][j];
				if(j<3) { s += ", "; }
				else { s += "\n"; }
			}
		}
		return s;
	}
	
}
