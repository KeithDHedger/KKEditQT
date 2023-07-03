TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webenginewidgets
HEADERS         = OpenUri.h
SOURCES         = OpenUri.cpp
TARGET          = $$qtLibraryTarget(OpenUri)
DESTDIR         = plugins

documentation.files = docs/*
image.files = OpenUri.png

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs
image.path = $${target.path}

INSTALLS += target
INSTALLS += data
INSTALLS += documentation
INSTALLS += image

CONFIG += install_ok

