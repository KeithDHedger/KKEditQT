TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = Favourites.h
SOURCES         = Favourites.cpp
TARGET          = $$qtLibraryTarget(Favourites)
DESTDIR         = ../plugins

documentation.files = docs/*
image.files = Favorite.png

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
image.path = $${target.path}

INSTALLS += target
INSTALLS += documentation
INSTALLS += image

CONFIG += install_ok


