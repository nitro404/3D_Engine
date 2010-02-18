// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

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
	
	public void readFrom(BufferedReader in) throws IOException {
		String input;
		
		// parse the key and store it
		input = in.readLine().trim();
		this.key = input.substring(input.indexOf('\"') + 1, input.indexOf('\"', input.indexOf('\"') + 1)).trim();
		if(this.key.length() == 0) {
			System.out.println("ERROR: Empty property key in map file.");
			System.exit(1);
		}
		// parse the value and store it
		this.value = input.substring(input.lastIndexOf('\"', input.lastIndexOf('\"') - 1) + 1, input.lastIndexOf('\"')).trim();
	}
	
}
