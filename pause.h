#ifndef PAUSE_H
#define PAUSE_H
#include <QDialog>
#include "ui_pause.h"
class QShortcut;
class BallsInRows;
class Pause: public QDialog{
	Q_OBJECT
	Ui::Pause ui;
	QShortcut *saveGameShortcut;
	QShortcut *fullscreenShortcut;
	friend void displayPause(BallsInRows *balls);
public:
	explicit Pause(QWidget *parent);
	~Pause();
};
#endif
