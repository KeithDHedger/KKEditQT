TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core 
HEADERS         = gtk2toolkit.h
SOURCES         = gtk2toolkit.cpp
TARGET          = $$qtLibraryTarget(gtk2toolkit)
DESTDIR         = plugins

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok

