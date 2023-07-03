TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = jslang.h
SOURCES         = jslang.cpp
TARGET          = $$qtLibraryTarget(jslang)
DESTDIR         = plugins

target.path = ../build/plugins
documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok



