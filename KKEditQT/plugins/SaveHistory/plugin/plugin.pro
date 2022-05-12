TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = SaveHistory.h
SOURCES         = SaveHistory.cpp
TARGET          = $$qtLibraryTarget(SaveHistory)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/SaveHistory)
	target.path = ~/.KKEditQT/plugins/SaveHistory
}
else {
	message(Installing in /tmp/plugins/SaveHistory)
	target.path = /tmp/plugins/SaveHistory
}

INSTALLS += target

CONFIG += install_ok



