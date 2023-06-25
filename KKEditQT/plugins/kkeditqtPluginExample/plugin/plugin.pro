TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webenginewidgets
HEADERS         = kkeditqtPluginExample.h
SOURCES         = kkeditqtPluginExample.cpp
TARGET          = $$qtLibraryTarget(kkeditqtPluginExample)
DESTDIR         = ../plugins

documentation.files = docs/*
destdir = $$(INSTALLTO)
isEmpty(destdir) {
	LOCAL = 1
}

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/kkeditqtPluginExample)
	target.path = ~/.KKEditQT/plugins/kkeditqtPluginExample
}
else {
	message(Installing in $$(INSTALLTO))
	target.path = $$(INSTALLTO)/plugins/kkeditqtPluginExample
}

documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok

