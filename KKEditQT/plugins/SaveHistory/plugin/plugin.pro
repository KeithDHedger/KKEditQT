TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport
HEADERS         = SaveHistory.h
SOURCES         = SaveHistory.cpp
INCLUDEPATH += /usr/include/KF6/KSyntaxHighlighting ../../../src 
TARGET          = $$qtLibraryTarget(SaveHistory)
DESTDIR         = plugins
INCLUDEPATH += /usr/include/KF6/KSyntaxHighlighting

documentation.files = docs/*

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs
image.path = $${target.path}

INSTALLS += target
INSTALLS += documentation

CONFIG += install_ok







