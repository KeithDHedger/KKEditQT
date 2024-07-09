TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webenginewidgets
HEADERS         = DocumentationPlugin.h
SOURCES         = DocumentationPlugin.cpp
TARGET          = $$qtLibraryTarget(DocumentationPlugin)
DESTDIR         = .plugins

documentation.files = docs/*
actions.files = actions/*

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs
actions.path = $${target.path}/actions

image.path = $${target.path}

INSTALLS += target
INSTALLS += documentation
INSTALLS += actions
CONFIG += install_ok
