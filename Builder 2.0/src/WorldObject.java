// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.util.Vector;
import java.io.*;

public class WorldObject {
	
	public int objectIndex;
	public Transformation normal;
	public Transformation inverse;
	public Vector<Property> properties;
	public Vector<Face> faces;
	
	public WorldObject() {
		this.properties = new Vector<Property>();
		this.faces = new Vector<Face>();
	}
	
	public WorldObject(int objectIndex, Transformation normal, Transformation inverse, Vector<Property> properties, Vector<Face> faces) {
		this.objectIndex = objectIndex;
		this.normal = normal;
		this.inverse = inverse;
		this.properties = properties;
		this.faces = faces;
	}
	
	public void addProperty(String key, String value) {
		this.addProperty(new Property(key, value));
	}
	
	public void addProperty(Property property) {
		if(property == null || property.key == null || property.value == null) { return; }
		
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(property.key)) {
				return;
			}
		}
		
		this.properties.add(property);
	}
	
	public String getPropertyValue(String key) {
		// search for a property based on a key and return its value if it is present
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(key)) {
				return this.properties.elementAt(i).value;
			}
		}
		return null;
	}
	
	public boolean setPropertyValue(String key, String newValue) {
		// search for a property based on a key and change its value if it is present
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(key)) {
				this.properties.elementAt(i).value = newValue;
				return true;
			}
		}
		return false;
	}
	
	public boolean removeProperty(String key) {
		// search for a property based on a key and remove it if it is present
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(key)) {
				this.properties.remove(i);
				return true;
			}
		}
		return false;
	}
	
	public void writeTo(PrintWriter out) throws Exception {
		// output the object type based on which subclass of UniversalObject it is
			 if(this instanceof Geometry)	{ out.println("\t\"type\" => \"static geometry\""); }
		else if(this instanceof Environment){ out.println("\t\"type\" => \"environment\""); }
		else if(this instanceof Vehicle)	{ out.println("\t\"type\" => \"vehicle\""); }
		else if(this instanceof Rotator)	{ out.println("\t\"type\" => \"rotator\""); }
		else if(this instanceof Translator)	{ out.println("\t\"type\" => \"translator\""); }
		else if(this instanceof Sprite)		{ out.println("\t\"type\" => \"sprite\""); }
		else if(this instanceof Waypoint)	{ out.println("\t\"type\" => \"waypoint\""); }
		else if(this instanceof Pool)		{ out.println("\t\"type\" => \"pool\""); }
		else if(this instanceof Terrain)	{ out.println("\t\"type\" => \"terrain\""); }
		else   								{ System.out.println("WARNING: Ignoring unexpected object of type \"" + this.getPropertyValue("type") + "\"."); }
		
		// output the transformations
		if(this.normal != null && this.inverse != null) {
			out.println("\tTransformation:");
			out.print("\t\t");
			this.normal.writeTo(out);
			out.println(";");
			out.print("\t\t");
			this.inverse.writeTo(out);
			out.println(";");
		}
		
		// output the properties
		if(this.properties != null) {
			out.println("\tProperties: " + this.properties.size() + ";");
			for(int i=0;i<this.properties.size();i++) {
				out.println("\t\t\"" + this.properties.elementAt(i).key + "\" => \"" + this.properties.elementAt(i).value + "\"");
			}
		}
		
		// output the faces
		if(this.faces != null) {
			out.println("\tFaces: " + this.faces.size() + ";");
			for(int i=0;i<this.faces.size();i++) {
				out.println("\tFace: " + i + ";");
				this.faces.elementAt(i).writeTo(out, !(this instanceof Sprite));
			}
		}
	}
	
}
