TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = golang.h
SOURCES         = golang.cpp
TARGET          = $$qtLibraryTarget(golang)
DESTDIR         = plugins

target.path = ../build/plugins
documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok
