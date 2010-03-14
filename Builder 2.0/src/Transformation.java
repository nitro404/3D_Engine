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
		transformation = new double[4][4];
		for(int i=0;i<4;i++) {
			for(int j=0;j<4;j++) {
				transformation[i][j] = 0;
			}
		}
	}
	
	public Transformation(double m11, double m12, double m13, double m14,
						  double m21, double m22, double m23, double m24,
						  double m31, double m32, double m33, double m34,
						  double m41, double m42, double m43, double m44) {
		transformation = new double[4][4];
		set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
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
	
	public void set(double m11, double m12, double m13, double m14,
					double m21, double m22, double m23, double m24,
					double m31, double m32, double m33, double m34,
					double m41, double m42, double m43, double m44) {
		transformation[0][0] = m11; transformation[0][1] = m12; transformation[0][2] = m13; transformation[0][3] = m14;
		transformation[1][0] = m21; transformation[1][1] = m22; transformation[1][2] = m23; transformation[1][3] = m24;
		transformation[2][0] = m31; transformation[2][1] = m32; transformation[2][2] = m33; transformation[2][3] = m34;
		transformation[3][0] = m41; transformation[3][1] = m42; transformation[3][2] = m43; transformation[3][3] = m44;
	}
	
	public Point3D getPosition() {
		return new Point3D(transformation[3][0], transformation[3][1], transformation[3][2]);
	}
	
	public void rotateToAxes(Point3D xAxis, Point3D yAxis, Point3D zAxis) { 
		this.set(xAxis.x, xAxis.y, xAxis.z, 0,
				 yAxis.x, yAxis.y, yAxis.z, 0,
				 zAxis.x, zAxis.y, zAxis.z, 0,
				       0,       0,       0, 1);
	}
	
	public void postScaleBy(Point3D scale) { // matrix * scale
		Transformation scalingMatrix = new Transformation(scale.x,       0,       0,       0,
																0, scale.y,       0,       0,
																0,       0, scale.z,       0,
																0,       0,       0,       1);
		this.postMultiplyBy(scalingMatrix);
	}
	
	public void postTranslateBy(Point3D translation) { // matrix * translation
		Transformation translationMatrix = new Transformation(1, 0, 0, translation.x,
															  0, 1, 0, translation.y,
															  0, 0, 1, translation.z,
															  0, 0, 0, 1);
		this.postMultiplyBy(translationMatrix);
	}
	
	public void postMultiplyBy(Transformation t) {
		for(int i=0;i<4;i++) {
			for(int j=0;j<4;j++) {
				this.transformation[i][j] *= t.transformation[j][i];
			}
		}
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
	
}
