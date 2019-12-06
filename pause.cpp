#include <QShortcut>
#include "pause.h"
Pause::Pause(QWidget *parent):
	QDialog(parent){
	ui.setupUi(this);
	saveGameShortcut = new QShortcut(QKeySequence::Save, this);
	fullscreenShortcut = new QShortcut(Qt::Key_F11, this);
	connect(saveGameShortcut, &QShortcut::activated, this, &Pause::accept);
	connect(fullscreenShortcut, &QShortcut::activated, this, &Pause::accept);
}
Pause::~Pause(){
	delete saveGameShortcut;
	delete fullscreenShortcut;
}
