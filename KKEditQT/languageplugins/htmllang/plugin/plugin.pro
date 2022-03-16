TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = htmllang.h
SOURCES         = htmllang.cpp
TARGET          = $$qtLibraryTarget(htmllang)
DESTDIR         = ../plugins

# install
equals(LOCAL,1) {
	target.path = ~/.KKEditQT/langplugins/htmllang
}
else {
	target.path = /tmp/langplugins/htmllang
}

INSTALLS += target

CONFIG += install_ok

