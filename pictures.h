#ifndef PICTURES_H
#define PICTURES_H
#include <QDialog>
#include "ui_pictures.h"
#include "arrowshortcuts.h"
#include "bonuses.h"
class QShortcut;
class Pictures: public QDialog{
	Q_OBJECT
	Ui::Pictures ui;
	ArrowShortcuts *arrowShortcuts;
	QPushButton *fragments;
	QImage image;
	int available;
	short index;
	long long pictures[PICTURE_COUNT];
	long long selected;
	short firstFragmentSize(const short width, const short height, short &x, short &y);
	void loadImage();
	void changeImage();
private slots:
	void arrowShortcuts_activated(ArrowShortcuts::Key key);
	void fragments_clicked(bool checked);
	void on_prevButton_released();
	void on_nextButton_released();
	void on_buyButton_released();
protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
public:
	explicit Pictures(QWidget *parent);
	~Pictures();
};
#endif
