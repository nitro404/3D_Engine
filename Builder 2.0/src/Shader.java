import java.io.BufferedReader;
import java.io.PrintWriter;

public class Shader {

	public int index;
	public String name;
	public String vertexShader;
	public String fragmentShader;
	
	public Shader() { }
	
	public Shader(int index, String name, String vertexShader, String fragmentShader) {
		this.index = index;
		this.name = name;
		this.vertexShader = vertexShader;
		this.fragmentShader = fragmentShader;
	}
	
	public Shader(BufferedReader in) {
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid shader in texture data file.");
			System.exit(1);
		}
	}
	
	public void readFrom(BufferedReader in) throws Exception {
		String input;
		
		// input the shader index
		input = in.readLine().trim();
		this.index = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		if(this.index < 0) {
			System.out.println("ERROR: Invalid shader index parsed: " + this.index + "\".");
			System.exit(1);
		}
		
		// input the properties header
		input = in.readLine().trim();
		String propertyHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!propertyHeader.equalsIgnoreCase("Properties")) {
			System.out.println("ERROR: Invalid Shader format. Expected header \"Properties\", found \"" + propertyHeader + "\".");
			System.exit(1);
		}
		
		// input the properties
		int numberOfProperties = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		Property newProperty;
		for(int i=0;i<numberOfProperties;i++) {
			newProperty = new Property(in);
			if(newProperty.key.equalsIgnoreCase("name")) {
				this.name = newProperty.value;
			}
			else if(newProperty.key.equalsIgnoreCase("vertex_shader")) {
				this.vertexShader = newProperty.value;
			}
			else if(newProperty.key.equalsIgnoreCase("fragment_shader")) {
				this.fragmentShader = newProperty.value;
			}
		}
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		out.println("\tShader: " + index + ";");
		out.println("\t\tProperties: 3;");
		out.println("\t\t\t\"name\" => \"" + name + "\"");
		out.println("\t\t\t\"vertex_shader\" => \"" + vertexShader + "\"");
		out.println("\t\t\t\"fragment_shader\" => \"" + fragmentShader + "\"");
	}
	
}
