TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = kkeditqtPluginExample.h
SOURCES         = kkeditqtPluginExample.cpp
TARGET          = $$qtLibraryTarget(kkeditqtPluginExample)
DESTDIR         = ../plugins

# install
target.path =~/.KKEditQT/plugins/kkeditqtPluginExample
INSTALLS += target

CONFIG += install_ok

