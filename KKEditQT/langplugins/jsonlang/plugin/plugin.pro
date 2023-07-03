TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = jsonlang.h
SOURCES         = jsonlang.cpp
TARGET          = $$qtLibraryTarget(jsonlang)
DESTDIR         = plugins

target.path = ../build/plugins
documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok



