#include "SettingsManager.h"

const char * SettingsManager::defaultGameName = "3D Game Engine";
const char * SettingsManager::defaultSettingsFileName = "settings.ini";
const char * SettingsManager::defaultDataDirectoryName = "Data";
const char * SettingsManager::defaultMapDirectoryName = "Maps";
const char * SettingsManager::defaultSoundDirectoryName = "Sounds";
const char * SettingsManager::defaultHeightMapDirectoryName = "Height Maps";
const char * SettingsManager::defaultTextureDirectoryName = "Textures";
const char * SettingsManager::defaultShaderDirectoryName = "Shaders";
const char * SettingsManager::defaultTextureDataFileName = "textures.ini";
const int SettingsManager::defaultWindowWidth = 1024;
const int SettingsManager::defaultWindowHeight = 768;
const bool SettingsManager::defaultFullScreen = false;
const bool SettingsManager::defaultShowFPS = true;
const double SettingsManager::defaultMouseSensitivity = 5.0;

SettingsManager::SettingsManager() : gameName(NULL), dataDirectoryName(NULL), mapDirectoryName(NULL), soundDirectoryName(NULL), heightMapDirectoryName(NULL), textureDirectoryName(NULL), shaderDirectoryName(NULL), textureDataFileName(NULL) {
	m_variables = new VariableSystem();
	reset();
}

SettingsManager::~SettingsManager() {
	delete [] gameName;
	delete [] dataDirectoryName;
	delete [] mapDirectoryName;
	delete [] soundDirectoryName;
	delete [] heightMapDirectoryName;
	delete [] textureDirectoryName;
	delete [] shaderDirectoryName;
	delete [] textureDataFileName;

	delete m_variables;
}

void SettingsManager::reset() {
	if(gameName != NULL) { delete [] gameName; }
	gameName = new char[strlen(defaultGameName) + 1];
	strcpy_s(gameName, strlen(defaultGameName) + 1, defaultGameName);

	if(dataDirectoryName != NULL) { delete [] dataDirectoryName; }
	dataDirectoryName = new char[strlen(defaultDataDirectoryName) + 1];
	strcpy_s(dataDirectoryName, strlen(defaultDataDirectoryName) + 1, defaultDataDirectoryName);
	
	if(mapDirectoryName != NULL) { delete [] mapDirectoryName; }
	mapDirectoryName = new char[strlen(defaultMapDirectoryName) + 1];
	strcpy_s(mapDirectoryName, strlen(defaultMapDirectoryName) + 1, defaultMapDirectoryName);

	if(soundDirectoryName != NULL) { delete [] soundDirectoryName; }
	soundDirectoryName = new char[strlen(defaultSoundDirectoryName) + 1];
	strcpy_s(soundDirectoryName, strlen(defaultSoundDirectoryName) + 1, defaultSoundDirectoryName);

	if(heightMapDirectoryName != NULL) { delete [] heightMapDirectoryName; }
	heightMapDirectoryName = new char[strlen(defaultHeightMapDirectoryName) + 1];
	strcpy_s(heightMapDirectoryName, strlen(defaultHeightMapDirectoryName) + 1, defaultHeightMapDirectoryName);

	if(textureDirectoryName != NULL) { delete [] textureDirectoryName; }
	textureDirectoryName = new char[strlen(defaultTextureDirectoryName) + 1];
	strcpy_s(textureDirectoryName, strlen(defaultTextureDirectoryName) + 1, defaultTextureDirectoryName);

	if(shaderDirectoryName != NULL) { delete [] shaderDirectoryName; }
	shaderDirectoryName = new char[strlen(defaultShaderDirectoryName) + 1];
	strcpy_s(shaderDirectoryName, strlen(defaultShaderDirectoryName) + 1, defaultShaderDirectoryName);

	if(textureDataFileName != NULL) { delete [] textureDataFileName; }
	textureDataFileName = new char[strlen(defaultTextureDataFileName) + 1];
	strcpy_s(textureDataFileName, strlen(defaultTextureDataFileName) + 1, defaultTextureDataFileName);

	windowWidth = defaultWindowWidth;
	windowHeight = defaultWindowHeight;
	fullScreen = defaultFullScreen;
	showFPS = defaultShowFPS;
	mouseSensitivity = defaultMouseSensitivity;
}

VariableSystem * SettingsManager::getVariables() const {
	return m_variables;
}

bool SettingsManager::load() { return loadFrom(defaultSettingsFileName); }

bool SettingsManager::save() const { return saveTo(defaultSettingsFileName); }

bool SettingsManager::loadFrom(const char * fileName) {
	VariableSystem * newVariables = VariableSystem::readFrom(fileName);
	if(newVariables == NULL) { return false; }

	delete m_variables;
	m_variables = newVariables;

	const char * data = NULL;

	data = m_variables->getValue("Game Name", "Settings");
	if(data != NULL) {
		if(gameName != NULL) { delete [] gameName; }
		gameName = new char[strlen(data) + 1];
		strcpy_s(gameName, strlen(data) + 1, data);
	}

	data = m_variables->getValue("Data Directory", "Paths");
	if(data != NULL) {
		if(dataDirectoryName != NULL) { delete [] dataDirectoryName; }
		dataDirectoryName = new char[strlen(data) + 1];
		strcpy_s(dataDirectoryName, strlen(data) + 1, data);
	}

	data = m_variables->getValue("Map Directory", "Paths");
	if(data != NULL) {
		if(mapDirectoryName != NULL) { delete [] mapDirectoryName; }
		mapDirectoryName = new char[strlen(data) + 1];
		strcpy_s(mapDirectoryName, strlen(data) + 1, data);
	}

	data = m_variables->getValue("Sound Directory", "Paths");
	if(data != NULL) {
		if(soundDirectoryName != NULL) { delete [] soundDirectoryName; }
		soundDirectoryName = new char[strlen(data) + 1];
		strcpy_s(soundDirectoryName, strlen(data) + 1, data);
	}

	data = m_variables->getValue("Height Map Directory", "Paths");
	if(data != NULL) {
		if(heightMapDirectoryName != NULL) { delete [] heightMapDirectoryName; }
		heightMapDirectoryName = new char[strlen(data) + 1];
		strcpy_s(heightMapDirectoryName, strlen(data) + 1, data);
	}

	data = m_variables->getValue("Texture Directory", "Paths");
	if(data != NULL) {
		if(textureDirectoryName != NULL) { delete [] textureDirectoryName; }
		textureDirectoryName = new char[strlen(data) + 1];
		strcpy_s(textureDirectoryName, strlen(data) + 1, data);
	}

	data = m_variables->getValue("Shader Directory", "Paths");
	if(data != NULL) {
		if(shaderDirectoryName != NULL) { delete [] shaderDirectoryName; }
		shaderDirectoryName = new char[strlen(data) + 1];
		strcpy_s(shaderDirectoryName, strlen(data) + 1, data);
	}

	data = m_variables->getValue("Texture Data File Name", "Paths");
	if(data != NULL) {
		if(textureDataFileName != NULL) { delete [] textureDataFileName; }
		textureDataFileName = new char[strlen(data) + 1];
		strcpy_s(textureDataFileName, strlen(data) + 1, data);
	}

	data = m_variables->getValue("Window Width", "Settings");
	if(data != NULL) {
		int tempWidth = atoi(data);
		if(tempWidth > 0) { windowWidth = tempWidth; }
	}

	data = m_variables->getValue("Window Height", "Settings");
	if(data != NULL) {
		int tempHeight = atoi(data);
		if(tempHeight > 0) { windowHeight = tempHeight; }
	}
	
	data = m_variables->getValue("Full Screen", "Settings");
	if(data != NULL) {
		if(_stricmp(data, "true") == 0) {
			fullScreen = true;
		}
		else if(_stricmp(data, "false") == 0) {
			fullScreen = false;
		}
	}

	data = m_variables->getValue("Show FPS", "Settings");
	if(data != NULL) {
		if(_stricmp(data, "true") == 0) {
			showFPS = true;
		}
		else if(_stricmp(data, "false") == 0) {
			showFPS = false;
		}
	}

	data = m_variables->getValue("Mouse Sensitivity", "Settings");
	if(data != NULL) {
		double tempSensitivity = atof(data);
		if(mouseSensitivity > 0) { mouseSensitivity = tempSensitivity; }
	}

	return true;
}

bool SettingsManager::saveTo(const char * fileName) const {
	m_variables->setValue("Game Name", gameName, "Settings");
	m_variables->setValue("Data Directory", dataDirectoryName, "Paths");
	m_variables->setValue("Map Directory", mapDirectoryName, "Paths");
	m_variables->setValue("Sound Directory", soundDirectoryName, "Paths");
	m_variables->setValue("Height Map Directory", heightMapDirectoryName, "Paths");
	m_variables->setValue("Texture Directory", textureDirectoryName, "Paths");
	m_variables->setValue("Shader Directory", shaderDirectoryName, "Paths");
	m_variables->setValue("Texture Data File Name", textureDataFileName, "Paths");
	m_variables->setValue("Window Width", windowWidth, "Settings");
	m_variables->setValue("Window Height", windowHeight, "Settings");
	m_variables->setValue("Full Screen", fullScreen, "Settings");
	m_variables->setValue("Show FPS", showFPS, "Settings");
	m_variables->setValue("Mouse Sensitivity", mouseSensitivity, "Settings");

	// group the variables by categories
	m_variables->sort();

	// update the settings file with the changes
	return m_variables->writeTo(fileName);
}
