TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport
HEADERS         = Clipboard.h
SOURCES         = Clipboard.cpp
INCLUDEPATH += /usr/include/KF6/KSyntaxHighlighting
TARGET          = $$qtLibraryTarget(Clipboard)
DESTDIR         = plugins

documentation.files = docs/*
image.files = ClipboardViewer.png

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs
image.path = $${target.path}

INSTALLS += target
INSTALLS += data
INSTALLS += documentation
INSTALLS += image

CONFIG += install_ok


