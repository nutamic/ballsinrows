#include <QIcon>
#include "about.h"
#define ICON_SIZE 48
About::About(QWidget *parent):
	QDialog(parent){
	ui.setupUi(this);
	ui.iconLabel->setPixmap(QApplication::windowIcon().pixmap(ICON_SIZE));
}
