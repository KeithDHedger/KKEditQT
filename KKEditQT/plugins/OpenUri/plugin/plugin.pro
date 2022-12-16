TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = OpenUri.h
SOURCES         = OpenUri.cpp
TARGET          = $$qtLibraryTarget(OpenUri)
DESTDIR         = ../plugins

documentation.files = docs/*
image.files = OpenUri.png

destdir = $$(INSTALLTO)
isEmpty(destdir) {
	LOCAL = 1
}

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/OpenUri)
	target.path = ~/.KKEditQT/plugins/OpenUri
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/plugins/OpenUri
}

documentation.path = $${target.path}/docs
image.path = $${target.path}

INSTALLS += target
INSTALLS += documentation
INSTALLS += image

CONFIG += install_ok


