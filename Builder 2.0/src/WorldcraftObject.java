import java.util.Vector;

public class WorldcraftObject {
	
	public Vector<WorldcraftSidePlane> sidePlanes;
	
	public WorldcraftObject() {
		this.sidePlanes = new Vector<WorldcraftSidePlane>();
	}
	
	public void addPlane(WorldcraftSidePlane worldcraftSidePlane) {
		if(worldcraftSidePlane != null) {
			sidePlanes.add(worldcraftSidePlane);
		}
	}
	
}
