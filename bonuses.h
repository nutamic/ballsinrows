#ifndef BONUSES_H
#define BONUSES_H
#include <QFile>
#define PICTURE_COUNT 5
class Bonuses{
	QFile file;
	int bonuses;
public:
	Bonuses();
	bool add(int count);
	int count() const{
		return bonuses;
	}
	void getPictures(long long *pictures);
	static bool buy(int bonuses, const long long *pictures);
};
#endif
