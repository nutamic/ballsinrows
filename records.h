#ifndef RECORDS_H
#define RECORDS_H
#include <QDateTime>
#define MAX_RECORDS 10
class QTableWidget;
class Records{
	struct Record{
		int value;
		QString gamer;
		QDateTime time;
	};
	std::list<Record> list;
public:
	explicit Records(short load = MAX_RECORDS);
	bool insert(const int score);
	QString getWinnerInfo(int &score);
	void fillTable(QTableWidget *table);
};
#endif
