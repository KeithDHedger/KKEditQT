TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport sql
HEADERS         = DocumentationPlugin.h
SOURCES         = DocumentationPlugin.cpp
INCLUDEPATH += /usr/include/KF6/KSyntaxHighlighting ../../../src 
TARGET          = $$qtLibraryTarget(DocumentationPlugin)
DESTDIR         = .plugins

documentation.files = docs/*
actions.files = actions/*

data.files = Doxyfile
image.files = LFSTux.png

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs
actions.path = $${target.path}/actions
image.path = $${target.path}

image.path = $${target.path}

INSTALLS += target
INSTALLS += documentation
INSTALLS += data
INSTALLS += actions
INSTALLS += image
CONFIG += install_ok
