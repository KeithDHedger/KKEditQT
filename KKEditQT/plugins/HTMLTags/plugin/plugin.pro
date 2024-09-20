TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webenginewidgets
HEADERS         = HTMLTags.h
SOURCES         = HTMLTags.cpp
TARGET          = $$qtLibraryTarget(HTMLTags)
DESTDIR         = plugins

documentation.files = docs/*
actions.files = actions/*

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs
actions.path = $${target.path}/actions

image.path = $${target.path}

INSTALLS += target
INSTALLS += data
INSTALLS += documentation
INSTALLS += image
INSTALLS += actions

CONFIG += install_ok

