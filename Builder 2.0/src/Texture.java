public class Texture {
	
	public static String removeExtension(String textureName) {
		if(textureName.lastIndexOf('.') != -1) {
			return textureName.substring(0, textureName.lastIndexOf('.'));
		}
		else {
			return textureName;
		}
	}
	
}
