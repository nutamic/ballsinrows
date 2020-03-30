#include <QApplication>
#include <QMessageBox>
#include <QPainter>
#include <QResizeEvent>
#include <QShortcut>
#include "pictures.h"
#include "appdata.h"
#include "resources.h"
#define FRAGMENTS_X 8
#define FRAGMENTS_Y 6
#define FRAGMENTS 48
#define MARGIN 10
#define BUTTONS_OFFSET 50
#define BUTTON_OUTER 60
#define BUTTON_INNER 40
#define FRAGMENT_SIDE 180
short Pictures::firstFragmentSize(const short width, const short height, short &x, short &y){
	short fragmentSize = (width - 2 * MARGIN) / FRAGMENTS_X;
	const short verticalSize = (height - 2 * MARGIN - BUTTONS_OFFSET) / FRAGMENTS_Y;
	if(verticalSize < fragmentSize){
		fragmentSize = verticalSize;
		x = (width - fragmentSize * 8) / 2;
		y = MARGIN;
	}else{
		x = MARGIN;
		y = (height - fragmentSize * 6 - BUTTONS_OFFSET) / 2;
	}
	return fragmentSize;
}
void Pictures::loadImage(){
	const QString &imagePath = AppData::picture(index);
	if(!image.load(imagePath)){
		if(imagePath.isEmpty()) QMessageBox::critical(this, tr("Pictures"), tr("Cannot find an image.\nReinstall the application."));
		else QMessageBox::critical(this, tr("Pictures"), tr("Cannot load an image from \"%1\".\nIf it is your file, fix or delete it. If not, reinstall the application.").arg(imagePath));
	}
}
void Pictures::changeImage(){
	loadImage();
	ui.buyButton->setEnabled(false);
	long long picture = pictures[index];
	QPushButton *const endFragment = fragments + FRAGMENTS;
	for(QPushButton *fragment = fragments; fragment != endFragment; fragment++){
		if(selected & 1){
			fragment->setChecked(false);
			available++;
		}
		fragment->show();
		if(picture & 1) fragment->hide();
		selected >>= 1;
		picture >>= 1;
	}
}
void Pictures::shortcuts_activated(){
	QPushButton *fragment = QPushButton_ptr(QApplication::focusWidget());
	if(fragment < fragments) return;
	QPushButton *const endFragments = fragments + FRAGMENTS;
	if(fragment >= endFragments) return;
	QShortcut *const shortcut = QShortcut_ptr(sender());
	if(shortcut == upShortcut){
		fragment -= FRAGMENTS_X;
		if(fragment < fragments) return;
	}else{
		fragment += FRAGMENTS_X;
		if(fragment >= endFragments) return;
	}
	fragment->setFocus();
}
void Pictures::fragments_clicked(bool checked){
	QPushButton *const fragment = QPushButton_ptr(sender());
	const long long mask = 1LL << (fragment - fragments);
	if(checked){
		if(!available){
			fragment->setChecked(false);
			return;
		}
		if(!selected) ui.buyButton->setEnabled(true);
		selected |= mask;
		available--;
	}else{
		selected &= ~mask;
		available++;
		if(!selected) ui.buyButton->setEnabled(false);
	}
}
void Pictures::on_prevButton_released(){
	switch(--index){
	case 0:
		ui.prevButton->setEnabled(false);
		break;
	case PICTURE_COUNT - 2:
		ui.nextButton->setEnabled(true);
	}
	changeImage();
}
void Pictures::on_nextButton_released(){
	switch(index++){
	case 0:
		ui.prevButton->setEnabled(true);
		break;
	case PICTURE_COUNT - 2:
		ui.nextButton->setEnabled(false);
	}
	changeImage();
}
void Pictures::on_buyButton_released(){
	pictures[index] |= selected;
	if(!Bonuses::buy(available, pictures)){
		pictures[index] &= ~selected;
		QMessageBox::warning(this, tr("Pictures"), tr("Cannot buy pictures.\nCheck whether you can write to \"%1\" and parent directory.").arg(AppData::bonuses()));
		return;
	}
	for(QPushButton *fragment = fragments; selected; fragment++){
		if(selected & 1){
			fragment->hide();
			fragment->setChecked(false);
		}
		selected >>= 1;
	}
	ui.buyButton->setText(tr("%n &bonus(es) available", Q_NULLPTR, available));
	ui.buyButton->setEnabled(false);
}
void Pictures::paintEvent([[maybe_unused]] QPaintEvent *event){
	short posX, posY;
	short fragmentSize = firstFragmentSize(short(width()), short(height()), posX, posY);
	QRect dest(0, 0, fragmentSize, fragmentSize);
	QRect src(0, 0, FRAGMENT_SIDE, FRAGMENT_SIDE);
	QPainter painter(this);
	char x = 0, y = 0;
	for(long long picture = pictures[index]; picture; picture >>= 1){
		if(picture & 1){
			dest.moveTo(posX + fragmentSize * x, posY + fragmentSize * y);
			src.moveTo(FRAGMENT_SIDE * x, FRAGMENT_SIDE * y);
			painter.drawImage(dest, image, src);
		}
		if(++x == FRAGMENTS_X){
			x = 0;
			y++;
		}
	}
	painter.end();
}
void Pictures::resizeEvent(QResizeEvent *event){
	const short width = short(event->size().width());
	const short height = short(event->size().height());
	short posX, posY;
	char x = 0, y = 0;
	short fragmentSize = firstFragmentSize(width, height, posX, posY);
	QRect rect(posX, posY, fragmentSize, fragmentSize);
	QPushButton *const endFragments = fragments + FRAGMENTS;
	for(QPushButton *fragment = fragments; fragment != endFragments; fragment++){
		fragment->setGeometry(rect);
		if(++x == FRAGMENTS_X){
			x = 0;
			rect.moveTop(posY + fragmentSize * ++y);
		}
		rect.moveLeft(posX + fragmentSize * x);
	}
	posY = height - BUTTONS_OFFSET;
	ui.prevButton->move(MARGIN, posY);
	ui.buyButton->setGeometry(BUTTON_OUTER, posY, width - 2 * BUTTON_OUTER, BUTTON_INNER);
	ui.nextButton->move(width - BUTTONS_OFFSET, posY);
}
Pictures::Pictures(QWidget *parent):
	QDialog(parent), index(0), pictures{}, selected(0){
	ui.setupUi(this);
	upShortcut = new QShortcut(Qt::Key_Up, this);
	downShortcut = new QShortcut(Qt::Key_Down, this);
	connect(upShortcut, &QShortcut::activated, this, &Pictures::shortcuts_activated);
	connect(downShortcut, &QShortcut::activated, this, &Pictures::shortcuts_activated);
	Bonuses bonuses;
	available = bonuses.count();
	ui.buyButton->setText(tr("%n &bonus(es) available", Q_NULLPTR, available));
	bonuses.getPictures(pictures);
	long long picture = *pictures;
	fragments = new QPushButton[FRAGMENTS];
	QPushButton *const endFragments = fragments + FRAGMENTS;
	for(QPushButton *fragment = fragments; fragment != endFragments; fragment++){
		fragment->setParent(this);
		fragment->setCheckable(true);
		connect(fragment, &QPushButton::clicked, this, &Pictures::fragments_clicked);
		if(picture & 1) fragment->hide();
		picture >>= 1;
	}
	loadImage();
}
Pictures::~Pictures(){
	delete upShortcut;
	delete downShortcut;
	delete[] fragments;
}
