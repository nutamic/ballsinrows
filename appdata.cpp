#include <QDir>
#include <QStandardPaths>
#include "appdata.h"
constexpr char dirTranslations[] = "translations";
constexpr char filePersonsImages[] = "persons.tar.xz";
constexpr char fileBallsIcons[] = "balls.tar.gz";
constexpr char dirPictures[] = "images/%1";
constexpr char pathSavedGame[] = "/saved-game";
constexpr char pathEmergExit[] = "/emergency-exit";
constexpr char pathBonuses[] = "/bonuses";
constexpr char pathRecordsEmerg[] = "/records.xml";
QString verifyLocation(QStandardPaths::StandardLocation location);
bool getPublicRecordsPath(QString &path);
QString AppData::translations(){
	return QStandardPaths::locate(QStandardPaths::AppDataLocation, dirTranslations, QStandardPaths::LocateDirectory);
}
QString AppData::personsImages(){
	return QStandardPaths::locate(QStandardPaths::AppDataLocation, filePersonsImages);
}
QString AppData::ballsIcons(){
	return QStandardPaths::locate(QStandardPaths::AppDataLocation, fileBallsIcons);
}
QString AppData::picture(short index){
	return QStandardPaths::locate(QStandardPaths::AppDataLocation, QString(dirPictures).arg(index, 2, 16, QLatin1Char('0')));
}
QString AppData::savedGame(){
	return verifyLocation(QStandardPaths::AppDataLocation) + pathSavedGame;
}
QString AppData::emergExit(){
	return verifyLocation(QStandardPaths::AppDataLocation) + pathEmergExit;
}
QString AppData::bonuses(){
	return verifyLocation(QStandardPaths::AppDataLocation) + pathBonuses;
}
QString AppData::records(){
	QString path;
	if(getPublicRecordsPath(path)) return path;
	return verifyLocation(QStandardPaths::AppDataLocation) + pathRecordsEmerg;
}
QString verifyLocation(QStandardPaths::StandardLocation location){
	QString path = QStandardPaths::writableLocation(location);
	QDir dir(path);
	if(!dir.exists()) dir.mkpath(".");
	return path;
}
