TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = kkeditqtPluginExample.h
SOURCES         = kkeditqtPluginExample.cpp
TARGET          = $$qtLibraryTarget(kkeditqtPluginExample)
DESTDIR         = ../plugins

equals(LOCAL,1) {
	message(Installing in ~/.KKEditQT/plugins/kkeditqtPluginExample)
	target.path = ~/.KKEditQT/plugins/kkeditqtPluginExample
}
else {
	message(Installing in /tmp/plugins/kkeditqtPluginExample)
	target.path = /tmp/plugins/kkeditqtPluginExample
}

INSTALLS += target

CONFIG += install_ok
