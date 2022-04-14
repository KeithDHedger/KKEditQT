TEMPLATE        = lib
CONFIG         += plugin
QT             += gui core
HEADERS         = gtk2toolkit.h
SOURCES         = gtk2toolkit.cpp
TARGET          = $$qtLibraryTarget(gtk2toolkit)
DESTDIR         = ../plugins

# install
equals(LOCAL,1) {
	target.path = ~/.KKEditQT/toolkitplugins/gtk2toolkit
}
else {
	target.path = /tmp/toolkitplugins/gtk2toolkit
}

INSTALLS += target

CONFIG += install_ok




