#include <QSaveFile>
#include "bonuses.h"
#include "appdata.h"
#include "resources.h"
#define PICTURE_BYTES 6
Bonuses::Bonuses():
	bonuses(0){
	file.setFileName(AppData::bonuses());
	if(!file.open(QIODevice::ReadOnly)) return;
	file.read(str(&bonuses), sizeof bonuses);
}
bool Bonuses::add(int count){
	file.close();
	if(!file.open(QIODevice::ReadWrite)) return false;
	bonuses += count;
	if(bonuses < 0) bonuses = 0x7fffffff;
	return file.write(const_str(&bonuses), sizeof bonuses) == sizeof bonuses;
}
void Bonuses::getPictures(long long *pictures){
	if(!file.isOpen()) return;
	long long *const endPictures = pictures + PICTURE_COUNT;
	while(pictures != endPictures) file.read(str(pictures++), PICTURE_BYTES);
}
bool Bonuses::buy(int bonuses, const long long *pictures){
	QSaveFile file(AppData::bonuses());
	if(!file.open(QIODevice::WriteOnly)) return false;
	file.write(const_str(&bonuses), sizeof bonuses);
	const long long *const endPictures = pictures + PICTURE_COUNT;
	while(pictures != endPictures) file.write(const_str(pictures++), PICTURE_BYTES);
	return file.commit();
}
