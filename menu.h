#ifndef MENU_H
#define MENU_H
#include <QDialog>
#include "ui_menu.h"
class QShortcut;
class BallsInRows;
class Menu: public QDialog{
	Q_OBJECT
	Ui::Menu ui;
	QShortcut *newGameShortcut;
	QShortcut *openGameShortcut;
	QShortcut *fullscreenShortcut;
	friend void displayMenu(BallsInRows *balls);
public:
	explicit Menu(QWidget *parent);
	~Menu();
};
#endif
