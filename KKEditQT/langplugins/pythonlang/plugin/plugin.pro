TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = pythonlang.h
SOURCES         = pythonlang.cpp
TARGET          = $$qtLibraryTarget(pythonlang)
DESTDIR         = plugins

target.path = ../build/plugins
documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok
