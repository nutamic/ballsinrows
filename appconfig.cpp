#include <QSettings>
#include "appconfig.h"
constexpr char configPath[] = "ballsinrows";
constexpr char configGamer[] = "Gamer";
constexpr char configFullscreen[] = "Fullscreen";
AppConfig *appConfig;
QString getUserName();
AppConfig::AppConfig(){
	QSettings config(configPath);
	gamerName = config.value(configGamer).toString();
	fullscreen = config.value(configFullscreen).toBool();
	if(gamerName.isEmpty()){
		gamerName = getUserName();
		config.setValue(configGamer, gamerName);
	}
}
bool AppConfig::setGamerName(const QString &newName){
	if(newName == gamerName) return false;
	gamerName = newName;
	QSettings config(configPath);
	config.setValue(configGamer, gamerName);
	return true;
}
void AppConfig::sync(){
	QSettings config(configPath);
	config.setValue(configFullscreen, fullscreen);
}
