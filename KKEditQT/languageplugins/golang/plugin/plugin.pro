TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = golang.h
SOURCES         = golang.cpp
TARGET          = $$qtLibraryTarget(golang)
DESTDIR         = ../plugins

documentation.files = docs/*
destdir = $$(INSTALLTO)
isEmpty(destdir) {
	LOCAL = 1
}

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/golang)
	target.path = ~/.KKEditQT/langplugins/golang
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/langplugins/golang
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok

