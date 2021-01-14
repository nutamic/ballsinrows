greaterThan(QT_MAJOR_VERSION, 5){
	QT += svgwidgets
}else{
	QT += svg
}
greaterThan(QT_MAJOR_VERSION, 4){
	QT += widgets
}
TARGET = ballsinrows
VERSION = 1.1
TEMPLATE = app
CONFIG += c++17
DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -larchive
SOURCES = *.cpp
HEADERS = *.h
FORMS = *.ui
TRANSLATIONS = translations/en.ts translations/ru.ts
INSTALLS = target
linux{
	SOURCES += linux/publicdata.cpp linux/sigactions.cpp linux/username.cpp
	target.path = /usr/bin
	data.files = share/*
	data.path = /usr/share/ballsinrows
	translations.files = translations/*.qm
	translations.path = /usr/share/ballsinrows/translations
	icons.files = linux/icons/*
	icons.path = /usr/share/icons/hicolor
	desktop.files = linux/ballsinrows.desktop
	desktop.path = /usr/share/applications
	INSTALLS += data translations icons desktop
}
win32{
	SOURCES += win32/publicdata.cpp win32/username.cpp
	RC_ICONS = win32/ballsinrows.ico
	QMAKE_TARGET_PRODUCT = "Balls in Rows"
	QMAKE_TARGET_COMPANY = "Maxim Glushkov"
	QMAKE_TARGET_COPYRIGHT = "Copyright (C) Maxim Glushkov 2018-2021"
	target.path = win32/installer/data
}
