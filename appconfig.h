#ifndef APPCONFIG_H
#define APPCONFIG_H
#include <QString>
class AppConfig{
	QString gamerName;
public:
	bool fullscreen;
	AppConfig();
	const QString &getGamerName(){
		return gamerName;
	}
	bool setGamerName(const QString &newName);
	void sync();
};
extern AppConfig *appConfig;
#endif
