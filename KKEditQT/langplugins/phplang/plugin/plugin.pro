TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = phplang.h
SOURCES         = phplang.cpp
TARGET          = $$qtLibraryTarget(phplang)
DESTDIR         = plugins

target.path = ../build/plugins
documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok
