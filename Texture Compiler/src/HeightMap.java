import java.io.PrintWriter;

public class HeightMap {
	
	public int index;
	public String name;
	public int heightMapFileIndex;
	public int width;
	public int height;
	
	public HeightMap() { }
	
	public void writeTo(PrintWriter out) throws Exception {
		out.println("\tHeightMap: " + index + ";");
		out.println("\t\tProperties: 4;");
		out.println("\t\t\t\"name\" => \"" + name + "\"");
		out.println("\t\t\t\"heightmap\" => \"" + heightMapFileIndex + "\"");
		out.println("\t\t\t\"width\" => \"" + width + "\"");
		out.println("\t\t\t\"height\" => \"" + height + "\"");
	}
	
}
