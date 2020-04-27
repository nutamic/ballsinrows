#ifndef ARROWSHORTCUTS_H
#define ARROWSHORTCUTS_H
#include <QObject>
#define ARROWS_COUNT 4
class QShortcut;
class ArrowShortcuts: public QObject{
	Q_OBJECT
	QShortcut *shortcuts[ARROWS_COUNT];
private slots:
	void shortcuts_activated();
public:
	enum Key{
		Up,
		Down,
		Left,
		Right
	};
	ArrowShortcuts(QWidget *parent);
	~ArrowShortcuts();
signals:
	void activated(Key key);
};
#undef ARROWS_COUNT
#endif
