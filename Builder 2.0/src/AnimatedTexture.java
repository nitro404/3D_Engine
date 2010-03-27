// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.util.Vector;
import java.io.*;

public class AnimatedTexture {
	
	public int index;
	public String name;
	public int firstTextureIndex;
	public int frames;
	public double speed;
	
	public AnimatedTexture(int index, String name, String firstTextureName, int frames, double speed, Vector<String> textureNames) {
		this.index = index;
		this.name = name;
		this.frames = frames;
		this.speed = speed;
		for(int i=0;i<textureNames.size();i++) {
			if(textureNames.elementAt(i).equalsIgnoreCase(firstTextureName)) {
				this.firstTextureIndex = i;
				break;
			}
		}
	}
	
	public AnimatedTexture(int index, String name, int firstTextureIndex, int frames, double speed, Vector<String> textureNames) {
		this.index = index;
		this.name = name;
		this.frames = frames;
		this.speed = speed;
		this.firstTextureIndex = firstTextureIndex;
	}
	
	public AnimatedTexture(BufferedReader in) {
		try {
			this.readFrom(in);
		}
		catch(Exception e) {
			System.out.println("ERROR: Invalid animated texture in texture data file.");
			System.exit(1);
		}
	}
	
	public void readFrom(BufferedReader in) throws Exception {
		String input;
		
		// input the animated texture index
		input = in.readLine().trim();
		this.index = Integer.valueOf(input.substring(input.indexOf(':') + 1, input.lastIndexOf(';')).trim());
		if(this.index < 0) {
			System.out.println("ERROR: Invalid animated texture index parsed: " + this.index + "\".");
			System.exit(1);
		}
		
		// input the properties header
		input = in.readLine().trim();
		String propertyHeader = input.substring(0, input.lastIndexOf(':')).trim();
		if(!propertyHeader.equalsIgnoreCase("Properties")) {
			System.out.println("ERROR: Invalid Universal Map format. Expected header \"Properties\", found \"" + propertyHeader + "\".");
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
			else if(newProperty.key.equalsIgnoreCase("firsttexture")) {
				this.firstTextureIndex = Integer.valueOf(newProperty.value);
			}
			else if(newProperty.key.equalsIgnoreCase("frames")) {
				this.frames = Integer.valueOf(newProperty.value);
			}
			else if(newProperty.key.equalsIgnoreCase("speed")) {
				this.speed = Double.valueOf(newProperty.value);
			}
		}
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		out.println("\tAnimatedTexture: " + index + ";");
		out.println("\t\tProperties: 4;");
		out.println("\t\t\t\"name\" => \"" + name + "\"");
		out.println("\t\t\t\"firsttexture\" => \"" + firstTextureIndex + "\"");
		out.println("\t\t\t\"frames\" => \"" + frames + "\"");
		out.println("\t\t\t\"speed\" => \"" + speed + "\"");
	}
	
}
