TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets printsupport webenginewidgets
HEADERS         = Favourites.h
SOURCES         = Favourites.cpp
TARGET          = $$qtLibraryTarget(Favourites)
DESTDIR         = plugins

documentation.files = docs/*
image.files = Favorite.png

target.path = ../build/plugins
data.path = $${target.path}
documentation.path = $${target.path}/docs
image.path = $${target.path}

INSTALLS += target
INSTALLS += data
INSTALLS += documentation
INSTALLS += image

CONFIG += install_ok


