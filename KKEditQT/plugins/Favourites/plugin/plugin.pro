TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = Favourites.h
SOURCES         = Favourites.cpp
TARGET          = $$qtLibraryTarget(Favourites)
DESTDIR         = ../plugins

documentation.files = docs/*
destdir = $$(INSTALLTO)
isEmpty(destdir) {
	LOCAL = 1
}

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/Favourites)
	target.path = ~/.KKEditQT/plugins/Favourites
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/plugins/Favourites
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok


