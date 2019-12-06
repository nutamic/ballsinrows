#include <QApplication>
#include <signal.h>
void signalProc(int signum);
void initSigActions(){
	struct sigaction sa = {};
	sa.sa_handler = signalProc;
	sigaction(SIGTERM, &sa, nullptr);
	sigaction(SIGINT, &sa, nullptr);
}
void signalProc(int signum){
	Q_UNUSED(signum)
	QApplication::quit();
}
