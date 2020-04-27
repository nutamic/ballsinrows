#include <QShortcut>
#include "arrowshortcuts.h"
#include "resources.h"
constexpr Qt::Key keys[] = {Qt::Key_Up, Qt::Key_Down, Qt::Key_Left, Qt::Key_Right};
void ArrowShortcuts::shortcuts_activated(){
	QShortcut *const shortcut = QShortcut_ptr(sender());
	for(short key = Up; key <= Right; key++){
		if(shortcuts[key] != shortcut) continue;
		emit activated(Key(key));
		break;
	}
}
ArrowShortcuts::ArrowShortcuts(QWidget *parent){
	for(short key = Up; key <= Right; key++){
		QShortcut *const shortcut = new QShortcut(keys[key], parent);
		shortcuts[key] = shortcut;
		QObject::connect(shortcut, &QShortcut::activated, this, &ArrowShortcuts::shortcuts_activated);
	}
}
ArrowShortcuts::~ArrowShortcuts(){
	for(short key = Up; key <= Right; key++) delete shortcuts[key];
}
