// =================================== //
// COMP 4002A Universal World Builder  //
// Author: Kevin Scroggins             //
// E-Mail: nitro404@hotmail.com        //
// =================================== //

import java.io.File;

public interface Map3D {
	
	// map file interface (every type of map should be able to read/write from/to files and convert between formats)
	public void convertFrom(Map3D map) throws Exception; 
	public void readFrom(File file) throws Exception;
	public void writeTo(File file) throws Exception;
	
}
