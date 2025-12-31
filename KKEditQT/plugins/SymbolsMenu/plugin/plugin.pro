TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport
HEADERS         = SymbolsMenu.h
SOURCES         = SymbolsMenu.cpp
INCLUDEPATH += /usr/include/KF6/KSyntaxHighlighting ../../../src 
TARGET          = $$qtLibraryTarget(SymbolsMenu)
DESTDIR         = plugins

data.files = indicator-chars.txt
documentation.files = docs/*
image.files = Symbols.png

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs
image.path = $${target.path}

INSTALLS += target
INSTALLS += data
INSTALLS += documentation
INSTALLS += image

CONFIG += install_ok

