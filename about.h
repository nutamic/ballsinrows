#ifndef ABOUT_H
#define ABOUT_H
#include <QDialog>
#include "ui_about.h"
class About: public QDialog{
	Q_OBJECT
	Ui::About ui;
private slots:
	void on_licenseLabel_linkActivated(const QString &link);
public:
	explicit About(QWidget *parent);
};
#endif
