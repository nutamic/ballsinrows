#include <QDesktopServices>
#include <QIcon>
#include <QUrl>
#include "about.h"
#define ICON_SIZE 48
void About::on_licenseLabel_linkActivated([[maybe_unused]] const QString &link){
	QDesktopServices::openUrl(QUrl("https://www.gnu.org/licenses/gpl-3.0.html"));
}
About::About(QWidget *parent):
	QDialog(parent){
	ui.setupUi(this);
	ui.iconLabel->setPixmap(QApplication::windowIcon().pixmap(ICON_SIZE));
}
