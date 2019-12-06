#include <QApplication>
#include <QTranslator>
#include "appconfig.h"
#include "appdata.h"
#include "ballsinrows.h"
void init();
#ifdef __linux__
void initSigActions();
#endif
int main(int argc, char *argv[]){
	QApplication app(argc, argv);
	init();
	BallsInRows ballsInRows;
	ballsInRows.show();
	return QApplication::exec();
}
void init(){
#ifdef __linux__
	initSigActions();
	QApplication::setWindowIcon(QIcon::fromTheme("ballsinrows"));
#endif
	srand(uint(time(nullptr)));
	appConfig = new AppConfig;
	QTranslator *translator = new QTranslator;
	if(!translator->load(QLocale::system().name(), AppData::translations())){
		delete translator;
		return;
	}
	QApplication::installTranslator(translator);
}
