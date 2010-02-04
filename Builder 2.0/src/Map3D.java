import java.io.File;

public interface Map3D {
	
	public void convertFrom(Map3D map) throws Exception; 
	public void readFrom(File file) throws Exception;
	public void writeTo(File file) throws Exception;
	
}
