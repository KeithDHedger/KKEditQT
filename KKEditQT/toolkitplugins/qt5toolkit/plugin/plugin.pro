TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = qt5toolkit.h
SOURCES         = qt5toolkit.cpp
TARGET          = $$qtLibraryTarget(qt5toolkit)
DESTDIR         = plugins

documentation.path = $${target.path}/docs
target.path = ../build/plugins
data.path = $${target.path}

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok




