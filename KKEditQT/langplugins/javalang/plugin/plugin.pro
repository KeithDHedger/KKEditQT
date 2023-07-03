TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = javalang.h
SOURCES         = javalang.cpp
TARGET          = $$qtLibraryTarget(javalang)
DESTDIR         = plugins

target.path = ../build/plugins
documentation.path = $${target.path}/docs

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok


