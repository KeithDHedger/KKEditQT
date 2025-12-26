TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport
HEADERS         = kkeditqtPluginExample.h
SOURCES         = kkeditqtPluginExample.cpp
INCLUDEPATH += /usr/include/KF6/KSyntaxHighlighting
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

