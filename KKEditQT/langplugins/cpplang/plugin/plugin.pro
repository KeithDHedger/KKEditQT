TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = cpplang.h
SOURCES         = cpplang.cpp
TARGET          = $$qtLibraryTarget(cpplang)
DESTDIR         = plugins

target.path = ../build/plugins
documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok

