import java.util.Vector;
import java.io.*;

public class AnimatedTexture {
	
	public int index;
	public String name;
	public int firstTextureIndex;
	public int frames;
	public int speed;
	
	public AnimatedTexture() { }
	
	public AnimatedTexture(int index, String name, String firstTextureName, int frames, int speed, Vector<String> textures) {
		this.index = index;
		this.name = name;
		this.frames = frames;
		this.speed = speed;
		for(int i=0;i<textures.size();i++) {
			if(textures.elementAt(i).equalsIgnoreCase(firstTextureName)) {
				this.firstTextureIndex = i;
				break;
			}
		}
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		out.println("AnimatedTexture: " + index + ";");
		out.println("\tProperties: 4;");
		out.println("\t\t\"name\" => \"" + name + "\"");
		out.println("\t\t\"firsttexture\" => \"" + firstTextureIndex + "\"");
		out.println("\t\t\"frames\" => \"" + frames + "\"");
		out.println("\t\t\"speed\" => \"" + speed + "\"");
	}
	
}
