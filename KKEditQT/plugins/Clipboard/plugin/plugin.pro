TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = Clipboard.h
SOURCES         = Clipboard.cpp
TARGET          = $$qtLibraryTarget(Clipboard)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/Clipboard)
	target.path = ~/.KKEditQT/plugins/Clipboard
}
else {
	message(Installing in /tmp/plugins/Clipboard)
	target.path = /tmp/plugins/Clipboard
}

INSTALLS += target

CONFIG += install_ok
