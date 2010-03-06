public class Texture {
	
	public static String getExtension(String textureName) {
		if(textureName.lastIndexOf('.') != -1) {
			return textureName.substring(textureName.lastIndexOf('.') + 1, textureName.length());
		}
		else {
			return null;
		}
	}
	
	public static boolean hasExtension(String textureName, String fileExtension) {
		String textureExtension = getExtension(textureName);
		return textureExtension != null && textureExtension.equalsIgnoreCase(fileExtension);
	}
	
	public static String removeExtension(String textureName) {
		if(textureName.lastIndexOf('.') != -1) {
			return textureName.substring(0, textureName.lastIndexOf('.'));
		}
		else {
			return textureName;
		}
	}
	
}
