TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = OpenUri.h
SOURCES         = OpenUri.cpp
TARGET          = $$qtLibraryTarget(OpenUri)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/OpenUri)
	target.path = ~/.KKEditQT/plugins/OpenUri
}
else {
	message(Installing in /tmp/plugins/OpenUri)
	target.path = /tmp/plugins/OpenUri
}

INSTALLS += target

CONFIG += install_ok

