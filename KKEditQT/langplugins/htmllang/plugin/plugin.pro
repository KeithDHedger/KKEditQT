TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = htmllang.h
SOURCES         = htmllang.cpp
TARGET          = $$qtLibraryTarget(htmllang)
DESTDIR         = plugins

target.path = ../build/plugins
documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok
