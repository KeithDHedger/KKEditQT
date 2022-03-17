TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = jsonlang.h
SOURCES         = jsonlang.cpp
TARGET          = $$qtLibraryTarget(jsonlang)
DESTDIR         = ../plugins

# install
equals(LOCAL,1) {
	target.path = ~/.KKEditQT/langplugins/jsonlang
}
else {
	target.path = /tmp/langplugins/jsonlang
}

INSTALLS += target

CONFIG += install_ok

