TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport
HEADERS         = TerminalPlugin.h QT_prefsWidgets.h
SOURCES         = TerminalPlugin.cpp QT_prefsWidgets.cpp
INCLUDEPATH += /usr/include/KF6/KSyntaxHighlighting
TARGET          = $$qtLibraryTarget(TerminalPlugin)
DESTDIR         = plugins

documentation.files = docs/*
image.files = TerminalPlugin.png
data.files = *.tpxrc

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs
image.path = $${target.path}

INSTALLS += target
INSTALLS += data
INSTALLS += documentation
INSTALLS += image

CONFIG += install_ok
