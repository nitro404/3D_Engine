import java.util.Vector;

public class Properties {
	
	public Vector<Property> properties;
	
	public Properties() {
		this.properties = new Vector<Property>();
	}
	
	public boolean addProperty(Property p) {
		if(p == null || p.key == null || p.key.length() == 0) { return false; }
		
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(p.key)) {
				return false;
			}
		}
		this.properties.add(p);
		return true;
	}
	
	public boolean addProperty(String key, String value) {
		if(key == null || key.length() == 0) { return false; }
		
		// add a property if it is not a duplicate
		for(int i=0;i<this.properties.size();i++) {
			if(this.properties.elementAt(i).key.equalsIgnoreCase(key)) {
				return false;
			}
		}
		this.properties.add(new Property(key, value));
		return true;
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
	
}
