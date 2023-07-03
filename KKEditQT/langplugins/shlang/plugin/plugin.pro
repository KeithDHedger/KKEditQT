TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = shlang.h
SOURCES         = shlang.cpp
TARGET          = $$qtLibraryTarget(shlang)
DESTDIR         = plugins

target.path = ../build/plugins
documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok
