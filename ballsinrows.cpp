#include <QInputDialog>
#include <QMessageBox>
#include <QPainter>
#include <QResizeEvent>
#include <QShortcut>
#include "ballsinrows.h"
#include "about.h"
#include "appconfig.h"
#include "appdata.h"
#include "highscores.h"
#include "menu.h"
#include "pause.h"
#include "pictures.h"
#include "records.h"
#include "resources.h"
#include "unpacker.h"
#define BONUS_COST 256
bool BallsInRows::confirmStopping(){
	switch(QMessageBox::question(this, tr("New game"), tr("You haven't finished the game."), QMessageBox::StandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel), QMessageBox::Save)){
	case QMessageBox::Save:
		return saveGame(false);
	case QMessageBox::Discard:
		field->stopGame(false);
		return true;
	default:
		return false;
	}
}
bool BallsInRows::saveGame(bool cleanup){
	const QString &fileName = AppData::savedGame();
	QFile file(fileName);
	if(file.exists()){
		if(QMessageBox::question(this, tr("Save game"), tr("Overwrite an existing game?")) == QMessageBox::No) return false;
	}
	if(!field->saveGame(fileName, cleanup)){
		QMessageBox::warning(this, tr("Save game"), tr("Cannot save your game.\nCheck whether you can write to \"%1\" and parent directory.").arg(fileName));
		return false;
	}
	return true;
}
void BallsInRows::fullscreenChanged(){
	ui.fullscreenAction->setChecked(appConfig->fullscreen);
	if(appConfig->fullscreen){
		menuHeight = 0;
		ui.menu->hide();
		ui.pauseButton->show();
		pauseShortcut = new QShortcut(Qt::Key_Escape, this);
		connect(pauseShortcut, &QShortcut::activated, this, &BallsInRows::on_pauseButton_released);
		if(!playing) displayMenu(this);
	}else{
		menuHeight = short(ui.menu->heightForWidth(0));
		ui.menu->show();
		ui.pauseButton->hide();
		delete pauseShortcut;
	}
#ifdef _WIN32
	QResizeEvent event(size(), size());
	resizeEvent(&event);
#endif
}
void displayMenu(BallsInRows *balls){
	Menu *menu = new Menu(balls);
	menu->setAttribute(Qt::WA_DeleteOnClose);
	QObject::connect(menu->ui.newGameButton, &QPushButton::released, balls, &BallsInRows::on_newGameAction_triggered);
	QObject::connect(menu->newGameShortcut, &QShortcut::activated, balls, &BallsInRows::on_newGameAction_triggered);
	QObject::connect(menu->ui.openGameButton, &QPushButton::released, balls, &BallsInRows::on_openGameAction_triggered);
	QObject::connect(menu->openGameShortcut, &QShortcut::activated, balls, &BallsInRows::on_openGameAction_triggered);
	QObject::connect(menu->ui.highscoresButton, &QPushButton::released, balls, &BallsInRows::on_highscoresAction_triggered);
	QObject::connect(menu->ui.picturesButton, &QPushButton::released, balls, &BallsInRows::on_picturesAction_triggered);
	QObject::connect(menu, &Menu::rejected, balls, &BallsInRows::showNormal);
	QObject::connect(menu->ui.changeNameButton, &QPushButton::released, balls, &BallsInRows::on_changeNameAction_triggered);
	QObject::connect(menu->ui.quitButton, &QPushButton::released, balls, &BallsInRows::close);
	menu->open();
}
void displayPause(BallsInRows *balls){
	Pause *pause = new Pause(balls);
	pause->setAttribute(Qt::WA_DeleteOnClose);
	QObject::connect(pause->ui.saveGameButton, &QPushButton::released, balls, &BallsInRows::on_saveGameAction_triggered);
	QObject::connect(pause->saveGameShortcut, &QShortcut::activated, balls, &BallsInRows::on_saveGameAction_triggered);
	QObject::connect(pause->ui.abortGameButton, &QPushButton::released, balls, &BallsInRows::on_abortGameAction_triggered);
	QObject::connect(pause->ui.fullscreenButton, &QPushButton::released, balls, &BallsInRows::showNormal);
	QObject::connect(pause->fullscreenShortcut, &QShortcut::activated, balls, &BallsInRows::showNormal);
	QObject::connect(pause->ui.quitButton, &QPushButton::released, balls, &BallsInRows::close);
	pause->open();
}
void BallsInRows::on_newGameAction_triggered(){
	if(playing){
		if(!confirmStopping()) return;
	}
	field->newGame(playing);
}
void BallsInRows::on_openGameAction_triggered(){
	QFile file(AppData::savedGame());
	if(!file.exists()){
		QMessageBox::warning(this, tr("Open game"), tr("You haven't saved any game."));
		if(appConfig->fullscreen) displayMenu(this);
		return;
	}
	if(playing){
		if(QMessageBox::question(this, tr("Open game"), tr("You haven't finished the game.\nDo you want to abort it?")) == QMessageBox::No) return;
		field->stopGame(false);
	}
	field->openGame(&file, playing);
	file.remove();
}
void BallsInRows::on_saveGameAction_triggered(){
	saveGame();
}
void BallsInRows::on_abortGameAction_triggered(){
	field->stopGame();
}
void BallsInRows::on_highscoresAction_triggered(){
	Highscores highscores(this);
	highscores.exec();
}
void BallsInRows::on_picturesAction_triggered(){
	Pictures pictures(this);
	pictures.exec();
}
void BallsInRows::on_fullscreenAction_triggered(){
	if(appConfig->fullscreen) showNormal();
	else showFullScreen();
}
void BallsInRows::on_changeNameAction_triggered(){
	bool ok;
	QString newName = QInputDialog::getText(this, tr("Changing gamer's name"), tr("Enter new name:"), QLineEdit::Normal, appConfig->getGamerName(), &ok);
	if(ok){
		if(appConfig->setGamerName(newName)) ui.gamerNameLabel->setText(newName);
	}
}
void BallsInRows::on_aboutAction_triggered(){
	About about(this);
	about.exec();
}
void BallsInRows::on_pauseButton_released(){
	displayPause(this);
}
void BallsInRows::stateChanged(){
	playing = !playing;
	ui.saveGameAction->setEnabled(playing);
	ui.abortGameAction->setEnabled(playing);
	if(!playing){
		if(appConfig->fullscreen) displayMenu(this);
	}
}
void BallsInRows::scoreChanged(){
	ui.gamerScoreLabel->setText(QString::number(score));
	bool swap = false;
	if(gamerWin){
		if(!score){
			gamerWin = false;
			swap = true;
		}
	}else{
		if(score > winnerScore){
			gamerWin = true;
			swap = true;
		}
	}
	short gamerHeight;
	if(gamerWin) gamerHeight = short(float(maxPersonHeight) * winnerScore / score);
	else{
		if(!winnerScore) gamerHeight = 0;
		else gamerHeight = short(float(maxPersonHeight) * score / winnerScore);
	}
	QPoint gamerPos = gamerSvg->pos();
	gamerPos.setY(lowestPersonY - gamerHeight);
	if(swap){
		QPoint winnerPos = winnerSvg->pos();
		const short newWinnerX = short(gamerPos.x());
		gamerPos.setX(winnerPos.x());
		winnerPos.setX(newWinnerX);
		const short winnerHeight = (gamerWin || winnerScore) ? maxPersonHeight : 0;
		winnerPos.setY(lowestPersonY - winnerHeight);
		winnerSvg->move(winnerPos);
	}
	gamerSvg->move(gamerPos);
	update();
}
void BallsInRows::gameOver(){
	const int bonusesGot = score / BONUS_COST;
	QMessageBox::information(this, tr("Balls in Rows"), tr("Game over.\n%n bonus(es) received.", nullptr, bonusesGot));
	if(bonusesGot){
		Bonuses bonuses;
		if(!bonuses.add(bonusesGot)) QMessageBox::warning(this, tr("Balls in Rows"), tr("Cannot save the amount of your bonuses.\nCheck whether you can write to \"%1\".").arg(AppData::bonuses()));
	}
	Records records;
	if(!records.insert(score)) QMessageBox::warning(this, tr("Balls in Rows"), tr("Your highscore wasn't saved.\nCheck whether you can write to \"%1\" and parent directory.").arg(AppData::records()));
	if(gamerWin){
		ui.winnerNameLabel->setText(ui.gamerNameLabel->text());
		ui.winnerScoreLabel->setText(ui.gamerScoreLabel->text());
		winnerScore = score;
	}
}
void BallsInRows::commitDataRequest([[maybe_unused]] QSessionManager &manager){
	if(!playing) return;
	field->saveGame(AppData::emergExit(), false);
	playing = false;
}
void BallsInRows::closeEvent(QCloseEvent *event){
	if(playing){
		if(!confirmStopping()){
			event->ignore();
			return;
		}
		playing = false;
	}
	event->accept();
}
void BallsInRows::changeEvent(QEvent *event){
	if(event->type() != QEvent::WindowStateChange) return;
	const bool fullscreen = windowState() == Qt::WindowFullScreen;
	if(fullscreen == appConfig->fullscreen) return;
	appConfig->fullscreen = fullscreen;
	fullscreenChanged();
}
void BallsInRows::paintEvent([[maybe_unused]] QPaintEvent *event){
	QPainter painter(this);
	QRect rect = gamerSvg->geometry();
	const short yOffset = short(rect.width()) + menuHeight;
	QLinearGradient gradient(rect.left(), 0, rect.right(), 0);
	gradient.setColorAt(0, Qt::gray);
	gradient.setColorAt(0.5, Qt::lightGray);
	gradient.setColorAt(1, Qt::gray);
	rect.setTop(rect.y() + yOffset);
	rect.setBottom(lowestPersonY + yOffset);
	painter.fillRect(rect, gradient);
	rect = winnerSvg->geometry();
	gradient.setStart(rect.left(), 0);
	gradient.setFinalStop(rect.right(), 0);
	rect.setTop(rect.y() + yOffset);
	rect.setBottom(lowestPersonY + yOffset);
	painter.fillRect(rect, gradient);
	painter.end();
}
void BallsInRows::resizeEvent(QResizeEvent *event){
	const short width = short(event->size().width());
	const short height = short(event->size().height()) - menuHeight;
	Metrics mt(width, height);
	ui.winnerScoreLabel->setGeometry(mt.margin, mt.margin, mt.object, mt.base);
	ui.winnerNameLabel->setGeometry(mt.margin, mt.bottomBase, mt.object, mt.base);
	ui.gamerScoreLabel->setGeometry(mt.rightObject, mt.margin, mt.object, mt.base);
	ui.gamerNameLabel->setGeometry(mt.rightObject, mt.bottomBase, mt.object, mt.base);
	ui.newLabel->setGeometry(mt.columnLeft, mt.margin, mt.object, mt.base);
	ui.colorsLabel->setGeometry(mt.columnRight, mt.margin, mt.object, mt.base);
	if(appConfig->fullscreen) ui.pauseButton->setGeometry(mt.columnCenter, mt.bottomBase, mt.object, mt.base);
	maxPersonHeight = 2 * mt.object;
	lowestPersonY = mt.rowCenter + maxPersonHeight;
	short gamerX, winnerX;
	short gamerHeight, winnerHeight = maxPersonHeight;
	if(gamerWin){
		gamerX = mt.margin;
		winnerX = mt.rightObject;
		gamerHeight = short(float(maxPersonHeight) * winnerScore / score);
	}else{
		gamerX = mt.rightObject;
		winnerX = mt.margin;
		if(!winnerScore) winnerHeight = gamerHeight = 0;
		else gamerHeight = short(float(maxPersonHeight) * score / winnerScore);
	}
	gamerSvg->setGeometry(gamerX, lowestPersonY - gamerHeight, mt.object, mt.object);
	winnerSvg->setGeometry(winnerX, lowestPersonY - winnerHeight, mt.object, mt.object);
	field->resize(&mt);
}
BallsInRows::BallsInRows():
	playing(false), gamerWin(false){
	ui.setupUi(this);
	menuHeight = short(ui.menu->heightForWidth(0));
	setMinimumSize(640, 480 + menuHeight);
	field = new Field(ui.centralWidget);
	connect(field, &Field::stateChanged, this, &BallsInRows::stateChanged);
	connect(field, &Field::scoreChanged, this, &BallsInRows::scoreChanged);
	connect(field, &Field::gameOver, this, &BallsInRows::gameOver);
	connect(qApp, &QApplication::commitDataRequest, this, &BallsInRows::commitDataRequest);
	Records records(1);
	ui.winnerNameLabel->setText(records.getWinnerInfo(winnerScore));
	ui.winnerScoreLabel->setText(QString::number(winnerScore));
	ui.gamerNameLabel->setText(appConfig->getGamerName());
	Unpacker unpacker(AppData::personsImages(), archive_read_support_filter_xz);
	gamerSvg = new QSvgWidget(ui.centralWidget);
	gamerSvg->load(unpacker.readNextEntryData());
	winnerSvg = new QSvgWidget(ui.centralWidget);
	winnerSvg->load(unpacker.readNextEntryData());
	QFile file(AppData::emergExit());
	if(file.exists()){
		if(QMessageBox::question(Q_NULLPTR, tr("Restore game"), tr("You haven't finished the game due to application termination.\nDo you want to resume it?")) == QMessageBox::Yes) field->openGame(&file, false);
		file.remove();
	}
	if(appConfig->fullscreen){
		showFullScreen();
		fullscreenChanged();
	}
}
BallsInRows::~BallsInRows(){
	if(playing) field->saveGame(AppData::emergExit(), false);
	delete field;
	appConfig->sync();
}
