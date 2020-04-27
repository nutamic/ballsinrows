#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QSaveFile>
#include "field.h"
#include "appdata.h"
#include "resources.h"
#include "routebuilder.h"
#include "unpacker.h"
#define COLORS 7
#define ICON_SIZES 8
#define ICON_PADDING 16
#define START_BALLS 5
#define BALLS_PER_STEP 3
#define MAX_NOT_A_LINE 4
constexpr short iconSizes[ICON_SIZES] = {144, 96, 72, 64, 48, 40, 32, 24};
int score;
void Field::setup(){
	memset(colors, FREE, SIZE);
	QPushButton *const endBalls = balls + SIZE;
	for(QPushButton *ball = balls; ball != endBalls; ball++){
		connect(ball, &QPushButton::clicked, this, &Field::balls_clicked);
		ball->setCheckable(true);
	}
	available = SIZE;
	selected = FREE;
	emit stateChanged();
}
void Field::reset(){
	memset(colors, FREE, SIZE);
	QIcon nullIcon;
	QPushButton *endBalls = balls + SIZE;
	for(QPushButton *ball = balls; ball != endBalls; ball++) ball->setIcon(nullIcon);
	endBalls = newBalls + BALLS_PER_STEP;
	for(QPushButton *ball = newBalls; ball != endBalls; ball++) ball->setIcon(nullIcon);
	available = SIZE;
	selected = FREE;
	score = 0;
	emit scoreChanged();
}
void Field::clear(){
	QIcon nullIcon;
	QPushButton *endBalls = balls + SIZE;
	for(QPushButton *ball = balls; ball != endBalls; ball++){
		disconnect(ball, &QPushButton::clicked, this, &Field::balls_clicked);
		ball->setIcon(nullIcon);
		ball->setCheckable(false);
	}
	endBalls = newBalls + BALLS_PER_STEP;
	for(QPushButton *ball = newBalls; ball != endBalls; ball++) ball->setIcon(nullIcon);
	score = 0;
	emit scoreChanged();
	emit stateChanged();
}
char *Field::getFreeLocation(ushort random){
	random %= available--;
	char *location = colors;
	for(;;){
		if(*location == FREE){
			if(!random) break;
			random--;
		}
		location++;
	}
	return location;
}
void Field::generateColors(){
	ushort random = ushort(rand());
	QPushButton *ball = newBalls;
	char *newColor = newColors;
	char *const endColors = newColors + BALLS_PER_STEP;
	while(newColor != endColors){
		const char color = random % COLORS;
		random /= COLORS;
		*newColor++ = color;
		ball++->setIcon(icons[short(color)]);
	}
}
void Field::moveTo(QPushButton *ball, short index){
	colors[index] = colors[selected];
	colors[selected] = FREE;
	ball->setIcon(icons[short(colors[index])]);
	QPushButton *const srcBall = balls + selected;
	srcBall->setIcon(QIcon());
	srcBall->setChecked(false);
	selected = FREE;
}
bool Field::stripLinesAround(short index){
	char *const base = colors + index;
	const char baseColor = *base;
	const char baseX = char(index) % SIDE, baseY = char(index) / SIDE;
	bool result = false;
	short length = 0;
	char *first = base, *last = base;
	for(char x = baseX; x--; length++){
		if(*--first != baseColor){
			first++;
			break;
		}
	}
	for(char x = baseX; ++x != SIDE; length++){
		if(*++last != baseColor){
			last--;
			break;
		}
	}
	if(length >= MAX_NOT_A_LINE){
		removeLine(first, last, 1, length);
		result = true;
	}
	length = 0;
	first = last = base;
	for(char x = baseX, y = baseY; x-- && y--; length++){
		first -= SIDE + 1;
		if(*first != baseColor){
			first += SIDE + 1;
			break;
		}
	}
	for(char x = baseX, y = baseY; ++x != SIDE && ++y != SIDE; length++){
		last += SIDE + 1;
		if(*last != baseColor){
			last -= SIDE + 1;
			break;
		}
	}
	if(length >= MAX_NOT_A_LINE){
		removeLine(first, last, SIDE + 1, length);
		result = true;
	}
	length = 0;
	first = last = base;
	for(char y = baseY; y--; length++){
		first -= SIDE;
		if(*first != baseColor){
			first += SIDE;
			break;
		}
	}
	for(char y = baseY; ++y != SIDE; length++){
		last += SIDE;
		if(*last != baseColor){
			last -= SIDE;
			break;
		}
	}
	if(length >= MAX_NOT_A_LINE){
		removeLine(first, last, SIDE, length);
		result = true;
	}
	length = 0;
	first = last = base;
	for(char x = baseX, y = baseY; ++x != SIDE && y--; length++){
		first -= SIDE - 1;
		if(*first != baseColor){
			first += SIDE - 1;
			break;
		}
	}
	for(char x = baseX, y = baseY; x-- && ++y != SIDE; length++){
		last += SIDE - 1;
		if(*last != baseColor){
			last -= SIDE - 1;
			break;
		}
	}
	if(length >= MAX_NOT_A_LINE){
		removeLine(first, last, SIDE - 1, length);
		result = true;
	}
	if(result) available++;
	return result;
}
void Field::removeLine(char *first, char *last, short step, short length){
	QIcon nullIcon;
	QPushButton *ball = balls + (first - colors);
	while(first <= last){
		*first = FREE;
		ball->setIcon(nullIcon);
		first += step;
		ball += step;
	}
	available += length;
	score += length * (length + 1) / 2;
	emit scoreChanged();
}
void Field::arrowShortcuts_activated(ArrowShortcuts::Key key){
	QPushButton *ball = QPushButton_ptr(QApplication::focusWidget());
	if(ball < balls) return;
	QPushButton *const endBalls = balls + SIZE;
	if(ball >= endBalls) return;
	switch(key){
	case ArrowShortcuts::Up:
		ball -= SIDE;
		if(ball < balls) return;
		break;
	case ArrowShortcuts::Down:
		ball += SIDE;
		if(ball >= endBalls) return;
		break;
	case ArrowShortcuts::Left:
		char x;
		x = (ball - balls) % SIDE;
		if(!x) return;
		ball--;
		break;
	case ArrowShortcuts::Right:
		ball++;
		x = (ball - balls) % SIDE;
		if(!x) return;
	}
	ball->setFocus();
}
void Field::balls_clicked([[maybe_unused]] bool checked){
	QPushButton *const ball = QPushButton_ptr(sender());
	const short released = short(ball - balls);
	if(colors[released] == FREE){
		ball->setChecked(false);
		if(selected == FREE) return;
		if(!RouteBuilder(selected, released, colors).build()) return;
		moveTo(ball, released);
		if(!stripLinesAround(released)){
			if(available <= BALLS_PER_STEP){
				stopGame();
				return;
			}
			char *const endColors = newColors + BALLS_PER_STEP;
			for(char *newColor = newColors; newColor != endColors; newColor++){
				char *const location = getFreeLocation(ushort(rand()));
				const char color = *newColor;
				*location = color;
				const short index = short(location - colors);
				balls[index].setIcon(icons[short(color)]);
				stripLinesAround(index);
			}
			generateColors();
		}
	}else{
		if(released == selected) selected = FREE;
		else{
			if(selected != FREE) balls[selected].setChecked(false);
			selected = released;
		}
	}
}
Field::Field(QWidget *centralWidget){
	arrowShortcuts = new ArrowShortcuts(centralWidget);
	connect(arrowShortcuts, &ArrowShortcuts::activated, this, &Field::arrowShortcuts_activated);
	colors = new char[SIZE];
	balls = new QPushButton[SIZE];
	QPushButton *endBalls = balls + SIZE;
	for(QPushButton *ball = balls; ball != endBalls; ball++) ball->setParent(centralWidget);
	newColors = new char[BALLS_PER_STEP];
	newBalls = new QPushButton[BALLS_PER_STEP];
	endBalls = newBalls + BALLS_PER_STEP;
	for(QPushButton *ball = newBalls; ball != endBalls; ball++){
		ball->setParent(centralWidget);
		ball->setFocusPolicy(Qt::NoFocus);
	}
	QPixmap pixmap;
	const QString &ballsPath = AppData::ballsIcons();
	Unpacker unpacker(ballsPath, archive_read_support_filter_gzip);
	icons = new QIcon[COLORS];
	QIcon *const endIcons = icons + COLORS;
	for(QIcon *icon = icons; icon != endIcons; icon++){
		for(short i = ICON_SIZES; i; i--){
			uint length;
			const uchar *const data = unpacker.readNextEntryData(length);
			if(data){
				if(pixmap.loadFromData(data, length)){
					icon->addPixmap(pixmap);
					continue;
				}
			}
			QMessageBox::critical(Q_NULLPTR, tr("Balls in Rows"), QObject::tr("The file \"%1\" has invalid content.\nIf it is your file, fix or delete it. If not, reinstall the application.").arg(ballsPath));
			return;
		}
	}
#ifndef __linux__
	QApplication::setWindowIcon(icons[0]);
#endif
}
Field::~Field(){
	delete arrowShortcuts;
	delete[] colors;
	delete[] balls;
	delete[] newColors;
	delete[] newBalls;
	delete[] icons;
}
void Field::resize(Metrics *mt){
	short iconSize = mt->base - ICON_PADDING;
	for(const short size: iconSizes){
		if(size < iconSize){
			iconSize = size;
			break;
		}
	}
	bool updateIconSize = balls->iconSize().width() != iconSize;
	char x = 0, y = 0;
	QSize size(iconSize, iconSize);
	QRect rect(mt->columnLeft, mt->rowCenter, mt->base, mt->base);
	QPushButton *endBalls = balls + SIZE;
	for(QPushButton *ball = balls; ball != endBalls; ball++){
		ball->setGeometry(rect);
		if(updateIconSize) ball->setIconSize(size);
		if(++x == SIDE){
			x = 0;
			rect.moveTop(mt->rowCenter + short(mt->fpBase * ++y));
		}
		rect.moveLeft(mt->columnLeft + short(mt->fpBase * x));
	}
	x = 0;
	rect.moveTo(mt->columnCenter, mt->margin);
	endBalls = newBalls + BALLS_PER_STEP;
	for(QPushButton *ball = newBalls; ball != endBalls; ball++){
		ball->setGeometry(rect);
		if(updateIconSize) ball->setIconSize(size);
		rect.moveLeft(mt->columnCenter + short(mt->fpBase * ++x));
	}
}
void Field::newGame(bool playing){
	if(playing) reset();
	else setup();
	for(short i = START_BALLS; i; i--){
		ushort random = ushort(rand());
		char *const location = getFreeLocation(random / COLORS);
		random %= COLORS;
		*location = char(random);
		balls[location - colors].setIcon(icons[random]);
	}
	generateColors();
}
void Field::openGame(QFile *file, bool playing){
	if(playing) reset();
	else setup();
	file->open(QIODevice::ReadOnly);
	file->read(str(&score), sizeof score);
	file->read(newColors, BALLS_PER_STEP);
	file->read(colors, SIZE);
	file->close();
	emit scoreChanged();
	char *endColors = newColors + BALLS_PER_STEP;
	for(char *color = newColors; color != endColors; color++) newBalls[color - newColors].setIcon(icons[short(*color)]);
	endColors = colors + SIZE;
	for(char *color = colors; color != endColors; color++){
		const short colorIndex = *color;
		if(colorIndex == FREE) continue;
		available--;
		balls[color - colors].setIcon(icons[colorIndex]);
	}
}
bool Field::saveGame(const QString &fileName, bool cleanup){
	QSaveFile file(fileName);
	if(!file.open(QIODevice::WriteOnly)) return false;
	file.write(const_str(&score), sizeof score);
	file.write(newColors, BALLS_PER_STEP);
	file.write(colors, SIZE);
	if(!file.commit()) return false;
	if(cleanup) clear();
	return true;
}
void Field::stopGame(bool cleanup){
	emit gameOver();
	if(cleanup) clear();
}
