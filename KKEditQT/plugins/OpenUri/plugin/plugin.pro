TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webkitwidgets
HEADERS         = OpenUri.h
SOURCES         = OpenUri.cpp
TARGET          = $$qtLibraryTarget(OpenUri)
DESTDIR         = ../plugins

# install
target.path =~/.KKEditQT/plugins/OpenUri
INSTALLS += target

CONFIG += install_ok

