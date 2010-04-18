import java.io.PrintWriter;

public class Shader {
	
	public int index;
	public String name;
	public String vertexShader;
	public String fragmentShader;
	
	public Shader() { }
	
	public void writeTo(PrintWriter out) throws Exception {
		out.println("\tShader: " + index + ";");
		out.println("\t\tProperties: 3;");
		out.println("\t\t\t\"name\" => \"" + name + "\"");
		out.println("\t\t\t\"vertex_shader\" => \"" + vertexShader + "\"");
		out.println("\t\t\t\"fragment_shader\" => \"" + fragmentShader + "\"");
	}
	
}

