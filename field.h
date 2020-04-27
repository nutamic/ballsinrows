#ifndef FIELD_H
#define FIELD_H
#include <QPushButton>
#include "arrowshortcuts.h"
#include "metrics.h"
class QFile;
class QShortcut;
class Field: public QObject{
	Q_OBJECT
	ArrowShortcuts *arrowShortcuts;
	short available, selected;
	char *colors;
	QPushButton *balls;
	char *newColors;
	QPushButton *newBalls;
	QIcon *icons;
	void setup();
	void reset();
	void clear();
	char *getFreeLocation(ushort random);
	void generateColors();
	void moveTo(QPushButton *ball, short index);
	bool stripLinesAround(short index);
	void removeLine(char *first, char *last, short step, short length);
private slots:
	void arrowShortcuts_activated(ArrowShortcuts::Key key);
	void balls_clicked(bool checked);
public:
	explicit Field(QWidget *centralWidget);
	~Field();
	void resize(Metrics *mt);
	void newGame(bool playing);
	void openGame(QFile *file, bool playing);
	bool saveGame(const QString &fileName, bool cleanup = true);
	void stopGame(bool cleanup = true);
signals:
	void stateChanged();
	void scoreChanged();
	void gameOver();
};
extern int score;
#endif
