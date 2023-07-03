TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webenginewidgets
HEADERS         = kkeditqtPluginExample.h
SOURCES         = kkeditqtPluginExample.cpp
TARGET          = $$qtLibraryTarget(kkeditqtPluginExample)
DESTDIR         = .plugins

documentation.files = docs/*

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs
image.path = $${target.path}

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok

