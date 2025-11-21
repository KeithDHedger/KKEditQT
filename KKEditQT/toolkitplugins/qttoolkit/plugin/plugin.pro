TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = qttoolkit.h
SOURCES         = qttoolkit.cpp
TARGET          = $$qtLibraryTarget(qttoolkit)
DESTDIR         = plugins

documentation.path = $${target.path}/docs
target.path = ../build/plugins
data.path = $${target.path}

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok




