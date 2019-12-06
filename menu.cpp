#ifndef _WIN32
#include <QDesktopWidget>
#endif
#include <QShortcut>
#include "menu.h"
Menu::Menu(QWidget *parent):
	QDialog(parent){
	ui.setupUi(this);
	newGameShortcut = new QShortcut(QKeySequence::New, this);
	openGameShortcut = new QShortcut(QKeySequence::Open, this);
	fullscreenShortcut = new QShortcut(Qt::Key_F11, this);
	connect(newGameShortcut, &QShortcut::activated, this, &Menu::accept);
	connect(openGameShortcut, &QShortcut::activated, this, &Menu::accept);
	connect(fullscreenShortcut, &QShortcut::activated, this, &Menu::reject);
#ifndef _WIN32
	QRect rect = geometry();
	rect.moveCenter(QApplication::desktop()->screenGeometry(this).center());
	setGeometry(rect);
#endif
}
Menu::~Menu(){
	delete newGameShortcut;
	delete openGameShortcut;
	delete fullscreenShortcut;
}
