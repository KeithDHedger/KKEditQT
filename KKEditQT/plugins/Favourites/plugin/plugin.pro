TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = Favourites.h
SOURCES         = Favourites.cpp
TARGET          = $$qtLibraryTarget(Favourites)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/Favourites)
	target.path = ~/.KKEditQT/plugins/Favourites
}
else {
	message(Installing in /tmp/plugins/Favourites)
	target.path = /tmp/plugins/Favourites
}

INSTALLS += target

CONFIG += install_ok

