#ifndef HIGHSCORES_H
#define HIGHSCORES_H
#include <QDialog>
#include "ui_highscores.h"
class Highscores: public QDialog{
	Q_OBJECT
	Ui::Highscores ui;
protected:
	void resizeEvent(QResizeEvent *event);
public:
	explicit Highscores(QWidget *parent);
};
#endif
