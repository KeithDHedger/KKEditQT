TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = Clipboard.h
SOURCES         = Clipboard.cpp
TARGET          = $$qtLibraryTarget(Clipboard)
DESTDIR         = ../plugins

documentation.files = docs/*
image.files = ClipboardViewer.png

destdir = $$(INSTALLTO)
isEmpty(destdir) {
	LOCAL = 1
}

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/Clipboard)
	target.path = ~/.KKEditQT/plugins/Clipboard
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/plugins/Clipboard
}

documentation.path = $${target.path}/docs
image.path = $${target.path}

INSTALLS += target
INSTALLS += documentation
INSTALLS += image

CONFIG += install_ok

