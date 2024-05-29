TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webenginewidgets
HEADERS         = PLUGNAME.h
SOURCES         = PLUGNAME.cpp
TARGET          = $$qtLibraryTarget(PLUGNAME)
DESTDIR         = plugins

documentation.files = docs/*
image.files = PLUGNAME.png

target.path = ../build/plugins/PLUGNAME
data.path = $${target.path}
documentation.path = $${target.path}/docs
image.path = $${target.path}

INSTALLS += target
INSTALLS += data
INSTALLS += documentation
INSTALLS += image

CONFIG += install_ok


