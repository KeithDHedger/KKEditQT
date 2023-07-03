TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = makelang.h
SOURCES         = makelang.cpp
TARGET          = $$qtLibraryTarget(makelang)
DESTDIR         = plugins

target.path = ../build/plugins
documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok
