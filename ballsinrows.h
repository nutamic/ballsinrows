#ifndef BALLS_H
#define BALLS_H
#include <QMainWindow>
#include <QSvgWidget>
#include "ui_ballsinrows.h"
#include "field.h"
class QShortcut;
class Menu;
class Pause;
class BallsInRows: public QMainWindow{
	Q_OBJECT
	Ui::BallsInRows ui;
	Field *field;
	QSvgWidget *gamerSvg, *winnerSvg;
	QShortcut *pauseShortcut;
	int winnerScore;
	short maxPersonHeight, lowestPersonY;
	short menuHeight;
	bool playing, gamerWin;
	bool confirmStopping();
	bool saveGame(bool cleanup = true);
	void fullscreenChanged();
	friend void displayMenu(BallsInRows *balls);
	friend void displayPause(BallsInRows *balls);
private slots:
	void on_newGameAction_triggered();
	void on_openGameAction_triggered();
	void on_saveGameAction_triggered();
	void on_abortGameAction_triggered();
	void on_highscoresAction_triggered();
	void on_picturesAction_triggered();
	void on_fullscreenAction_triggered();
	void on_changeNameAction_triggered();
	void on_aboutAction_triggered();
	void on_pauseButton_released();
	void stateChanged();
	void scoreChanged();
	void gameOver();
	void commitDataRequest(QSessionManager &manager);
protected:
	void closeEvent(QCloseEvent *event);
	void changeEvent(QEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
public:
	BallsInRows();
	~BallsInRows();
};
#endif
