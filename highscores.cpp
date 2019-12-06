#include <QResizeEvent>
#include "highscores.h"
#include "records.h"
#define COLUMNS 3
constexpr short columnWidths[COLUMNS] = {50, 150, 150};
void Highscores::resizeEvent(QResizeEvent *event){
	ui.highscoresTable->resize(event->size());
}
Highscores::Highscores(QWidget *parent):
	QDialog(parent){
	ui.setupUi(this);
	Records records;
	records.fillTable(ui.highscoresTable);
	for(short i = 0; i < COLUMNS; i++) ui.highscoresTable->setColumnWidth(i, columnWidths[i]);
}
