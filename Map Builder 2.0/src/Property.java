import java.io.*;

public class Property {
	
	public String key;
	public String value;
	
	public Property(String key, String value) {
		this.key = key;
		this.value = value;
	}
	
	public Property(BufferedReader in) {
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid property in map file.");
			System.exit(1);
		}
	}
	
	public void readFrom(BufferedReader in) throws Exception {
		String input;
		
		input = in.readLine().trim();
		this.key = input.substring(input.indexOf('\"') + 1, input.indexOf('\"', input.indexOf('\"') + 1)).trim();
		if(this.key.length() == 0) {
			System.out.println("ERROR: Empty property key in map file.");
			System.exit(1);
		}
		this.value = input.substring(input.lastIndexOf('\"', input.lastIndexOf('\"') - 1) + 1, input.lastIndexOf('\"')).trim();
	}
	
	/*public void readFrom(BufferedReader in) throws Exception {
		String input;
		
		StringTokenizer st;
		input = in.readLine().trim();
		st = new StringTokenizer(input, "\"", false);
		if(st.countTokens() != 3) {
			System.out.println("WARNING: Ignoring invalid object property: " + input + "\".");
		}
		else {
			this.key = st.nextToken().trim();
			st.nextToken();
			this.value = st.nextToken().trim();
		}
	}*/
	
}
