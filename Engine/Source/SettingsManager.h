#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include "Utilities.h"
#include "VariableSystem.h"

class SettingsManager {
public:
	SettingsManager();
	~SettingsManager();
	
	void reset();

	VariableSystem * getVariables() const;
	
	bool load();
	bool save() const;

	bool loadFrom(const char * fileName);
	bool saveTo(const char * fileName) const;

public:
	const static char * defaultGameName;
	const static char * defaultSettingsFileName;
	const static char * defaultDataDirectoryName;
	const static char * defaultMapDirectoryName;
	const static char * defaultSoundDirectoryName;
	const static char * defaultHeightMapDirectoryName;
	const static char * defaultTextureDirectoryName;
	const static char * defaultShaderDirectoryName;
	const static char * defaultTextureDataFileName;
	const static int defaultWindowWidth;
	const static int defaultWindowHeight;
	const static bool defaultFullScreen;
	const static bool defaultShowFPS;
	const static double defaultMouseSensitivity;

	char * gameName;
	char * dataDirectoryName;
	char * mapDirectoryName;
	char * soundDirectoryName;
	char * heightMapDirectoryName;
	char * textureDirectoryName;
	char * shaderDirectoryName;
	char * textureDataFileName;
	int windowWidth;
	int windowHeight;
	bool fullScreen;
	bool showFPS;
	double mouseSensitivity;

private:
	VariableSystem * m_variables;
};

#endif // SETTINGS_MANAGER_H
